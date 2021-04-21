#include "ModelContainer.h"

#if IS_ENGINE_MODE
#include "imgui\imgui.h"
#include "imgui\ImGuiFileDialog.h"
#endif // IS_ENGINE_MODE

#include "TestModelProbe.h"

#include <sstream>
#include <fstream>
#include <filesystem>

#include "json.hpp"

using json = nlohmann::json;
using namespace std::string_literals;

#if IS_ENGINE_MODE
ModelContainer::ModelContainer(const char* path, Graphics& gfx, Rgph::RenderGraph& rg, AppLog* aLog)
	:
	path(path),
	applog(aLog),
	gfx(gfx),
	rg(rg)
#else
ModelContainer::ModelContainer(const char* path, Graphics& gfx, Rgph::RenderGraph& rg)
	:
	path(path),
	gfx(gfx),
	rg(rg)
#endif // IS_ENGINE_MODE
{
#if IS_ENGINE_MODE
	applog->AddLog(MODEL_LOG, "Инициализация\n");
#endif // IS_ENGINE_MODE

	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о моделях");
	}

	json j;
	dataFile >> j;
	
	dataFile.close();

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* Получение имени объекта */

			std::string name = obj.at("name");
			std::transform(name.begin(), name.end(), name.begin(), tolower);

			/************************/

			/* Получение позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");
			float pos_z = obj.at("pos-z");

			DirectX::XMFLOAT3 position = { pos_x, pos_y, pos_z };

			/******************/

			/* Получение ориентации */

			float roll = obj.at("roll");
			float pitch = obj.at("pitch");
			float yaw = obj.at("yaw");

			DirectX::XMFLOAT3 orientation = { roll, pitch, yaw };

			/*********************/

			/* Получение пути к модели */

			std::string modelPath = obj.at("path");

			/***************************/

			/* Получение множителя размера */

			float modelScale = obj.at("scale");

			/*******************************/

			/* Инициализация модели */

			models.emplace_back(std::make_unique<Model>(name, modelPath, gfx, position, orientation, modelScale));

			/**************************************************/
		}
	}

	models.shrink_to_fit();

	Init();
}

ModelContainer::~ModelContainer()
{
}

void ModelContainer::LinkTechniques()
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->LinkTechniques(rg);

#if IS_ENGINE_MODE
		std::ostringstream oss;
		oss << "Добавлено к рендеру [" << models[i]->GetName() << "]\n";

		applog->AddLog(MODEL_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE
	}
}

void ModelContainer::Submit(size_t channels)
{
	for (auto& m : models)
	{
		m->Submit(channels);
	}
}

std::string ModelContainer::GetModelNameByIndex(size_t index)
{
	return models[index]->GetName();
}

std::unique_ptr<Model>* ModelContainer::GetPtr2ModelByName(std::string name)
{
	for (int i = 0; i < models.size(); i++)
	{
		if (models[i]->GetName() == name)
		{
			return &models[i];
		}
	}

	return nullptr;
}

template<typename T>
void ModelContainer::SetNewValue(const char* objectName, const char* param, T val)
{
	// Открытие файла с триггерами
	std::ifstream dataFile(this->path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о моделях сцен");
	}

#if IS_ENGINE_MODE
	std::ostringstream ostrlog;
	ostrlog << "Установка [" << param << " : " << std::to_string(val) << "] " << "для [" << objectName << "]\n";

	applog->AddLog(MODEL_LOG, ostrlog.str().c_str());
#endif // IS_ENGINE_MODE

	// Чтение файла
	json j;
	dataFile >> j;

	// Закрытие файла
	dataFile.close();

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		for (auto& obj : j.at(objectName))
		{
			obj[param] = val;
		}
	}

	// Запись в файл нового триггера
	std::ofstream ostr(this->path);
	ostr << j.dump();

	// Закрытие файла
	ostr.close();
}

void ModelContainer::LoadModel(std::string name, std::string path)
{
	models.emplace_back(std::make_unique<Model>(name, path, gfx));

	models.back()->LinkTechniques(rg);

#if IS_ENGINE_MODE
	std::ostringstream ostr;
	ostr << "Добавлено к рендеру [" << name << "]\n";

	applog->AddLog(MODEL_LOG, ostr.str().c_str());
#endif // IS_ENGINE_MODE

	using std::to_string;

	// Открытие файла с триггерами
	std::ifstream dataFile(this->path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о моделях сцен");
	}

#if IS_ENGINE_MODE
	applog->AddLog(MODEL_LOG, "Добавление\n");
#endif // IS_ENGINE_MODE

	// Чтение файла
	json j;
	dataFile >> j;

	// Закрытие файла
	dataFile.close();

	// Новый триггер
	std::ostringstream newModel;
	newModel << "\"" << name << "\":[{";

	newModel << "\"name\": \""  << name << "\",";
	newModel << "\"path\": \""  << path << "\",";
	newModel << "\"scale\": " << 1.0f << ",";

	newModel << "\"pos-x\": "  << 0.0f << ",";
	newModel << "\"pos-y\" : " << 0.0f << ",";
	newModel << "\"pos-z\" : " << 0.0f << ",";

	newModel << "\"roll\": "  << 0.0f << ",";
	newModel << "\"pitch\": " << 0.0f << ",";
	newModel << "\"yaw\": "   << 0.0f << "}]";

	// Подготовка к вставке в файл
	std::string json_str = j.dump();
	size_t pos_of_par = json_str.find_last_of('}');
	size_t pos_of_par2 = json_str.find_last_of(']');

	json_str.at(pos_of_par) = ' ';
	json_str.at(pos_of_par2 + 1) = ',';

	// Запись в файл нового триггера
	std::ofstream ostream(this->path);
	ostream << json_str + newModel.str() + '}';

	// Закрытие файла
	ostream.close();

#if IS_ENGINE_MODE
	std::ostringstream oss;
	oss << "Добавлено [" << name << "]\n";

	applog->AddLog(MODEL_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE
}

void ModelContainer::DeleteModel(std::string name)
{
	for (auto model = models.begin(); model != models.end(); ++model)
	{
		if (model->get()->GetName() == name)
		{
			models.erase(model);
		}
	}
}

void ModelContainer::Init()
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->SetRootTransform
		(
			dx::XMMatrixRotationX(models[i]->GetOrientation().x) *
			dx::XMMatrixRotationY(models[i]->GetOrientation().y) *
			dx::XMMatrixRotationZ(models[i]->GetOrientation().z) *
			dx::XMMatrixTranslation(
				models[i]->GetPosition().x,
				models[i]->GetPosition().y,
				models[i]->GetPosition().z
			)
		);

#if IS_ENGINE_MODE
		std::ostringstream oss;
		oss << "Установка положения [" << models[i]->GetName() << "]\n";

		applog->AddLog(MODEL_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE
	}
}

#if IS_ENGINE_MODE
void ModelContainer::ShowLeftPanel()
{
	if (ImGui::Begin("Объекты", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto& model : models)
		{
			char label[128];
			sprintf_s(label, model->GetName().c_str(), selected);

			ImGui::Bullet();
			if (ImGui::Selectable(label, selected == model->GetName().c_str()))
			{
				selected = model->GetName();
			}
		}

		ImGui::NewLine();

		if (ImGui::Button("Добавить"))
		{
			ImGuiFileDialog::Instance()->OpenDialog("ModelOD", "Выбирете файл", ".obj,.mtl,.gltf,.glb,.fbx", "");
			applog->AddLog(SYSTEM_LOG, "Добавить модель\n");
		}

		OpenDialog();
	}

	ImGui::End();
}

void ModelContainer::ShowRightPanel()
{
	if (ImGui::Begin("Опции", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (int k = 0; k < models.size(); k++)
		{
			if (models[k]->GetName().c_str() == selected)
			{
				// static MP probe{ modelsName.at(k) };
				// probe.SpawnChildWindow(*models.at(k));

				models[k]->SpawnDefaultControl();

				if (IsSave)
				{
					auto position = models.at(k)->GetPosition();
					auto orientation = models.at(k)->GetOrientation();
					auto name = models[k]->GetName();

					applog->AddLog(MODEL_LOG, "Сохранение позиции\n");

					SetNewValue<float>(name.c_str(), "pos-x", position.x);
					SetNewValue<float>(name.c_str(), "pos-y", position.y);
					SetNewValue<float>(name.c_str(), "pos-z", position.z);

					applog->AddLog(MODEL_LOG, "Сохранение ориентации\n");

					SetNewValue<float>(name.c_str(), "roll", orientation.x);
					SetNewValue<float>(name.c_str(), "pitch", orientation.y);
					SetNewValue<float>(name.c_str(), "yaw", orientation.z);

					IsSave = false;
				}

				if (IsDelete)
				{
					DeleteModel(selected);

					IsDelete = false;
				}

				break;
			}
		}

		if (ImGui::Button("Удалить", ImVec2(100, 20)))
		{
			IsDelete = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Сохранить", ImVec2(100, 20)))
		{
			IsSave = true;
		}

		ImGui::EndChild();
	}

	ImGui::End();
}

void ModelContainer::OpenDialog()
{
	if (ImGuiFileDialog::Instance()->Display("ModelOD"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string copy1 = filePathName;

			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			std::string copy2 = filePath;

			std::string name = copy1.replace(0, copy2.size() + 1, "");

			for (int i = 0; i != filePathName.size(); i++)
			{
				if (filePathName[i] == '\\')
				{
					filePathName[i] = '/';
				}
			}

			LoadModel(name.c_str(), filePathName.c_str());
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

size_t ModelContainer::ModelsAmount()
{
	return models.size();
}
#endif // IS_ENGINE_MODE
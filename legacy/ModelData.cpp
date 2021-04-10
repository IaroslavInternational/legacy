#include "ModelData.h"

#if IS_ENGINE_MODE
#include "imgui\imgui.h"
#include "imgui\ImGuiFileDialog.h"
#endif // IS_ENGINE_MODE

#include "TestModelProbe.h"

using json = nlohmann::json;
using namespace std::string_literals;

#if IS_ENGINE_MODE
ModelData::ModelData(const char* path, Graphics& gfx, AppLog* aLog)
	:
	path(path),
	applog(aLog)
{
	applog->AddLog(MODEL_LOG, "Инициализация\n");

	const auto dataPath = path;

	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о моделях");
	}

	json j;
	dataFile >> j;

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto d = m.key();
		
		for (const auto& obj : j.at(d))
		{
			/* Запись имени объекта */

			std::string name = obj.at("name");
			modelsName.push_back(name);

			/************************/

			/* Запись модели, инициализируя по пути и размеру */

			std::string modelPath = obj.at("path");
			float modelScale = obj.at("scale");
			models.emplace_back(std::make_unique<Model>(gfx, modelPath, modelScale));

			/**************************************************/

			/* Запись позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");
			float pos_z = obj.at("pos-z");

			DirectX::XMFLOAT3 position = { pos_x, pos_y, pos_z };

			modelsPos.emplace_back(position);

			/******************/

			/* Запись ориентации */

			float roll = obj.at("roll");
			float pitch = obj.at("pitch");
			float yaw = obj.at("yaw");

			DirectX::XMFLOAT3 orientation = { roll, pitch, yaw };

			modelsOrien.emplace_back(orientation);

			/*********************/
		}
	}

	models.shrink_to_fit();
	modelsPos.shrink_to_fit();
	modelsOrien.shrink_to_fit();
	modelsName.shrink_to_fit();

	Init();
}
#else
ModelData::ModelData(const char* path, Graphics& gfx)
	:
	path(path)
{
	const auto dataPath = path;

	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о моделях");
	}

	json j;
	dataFile >> j;

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* Запись имени объекта */

			std::string name = obj.at("name");
			modelsName.push_back(name);

			/************************/

			/* Запись модели, инициализируя по пути и размеру */

			std::string modelPath = obj.at("path");
			float modelScale = obj.at("scale");
			models.emplace_back(std::make_unique<Model>(gfx, modelPath, modelScale));

			/**************************************************/

			/* Запись позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");
			float pos_z = obj.at("pos-z");

			DirectX::XMFLOAT3 position = { pos_x, pos_y, pos_z };

			modelsPos.emplace_back(position);

			/******************/

			/* Запись ориентации */

			float roll = obj.at("roll");
			float pitch = obj.at("pitch");
			float yaw = obj.at("yaw");

			DirectX::XMFLOAT3 orientation = { roll, pitch, yaw };

			modelsOrien.emplace_back(orientation);

			/*********************/
		}
	}

	models.shrink_to_fit();
	modelsPos.shrink_to_fit();
	modelsOrien.shrink_to_fit();
	modelsName.shrink_to_fit();

	Init();
}
#endif // IS_ENGINE_MODE

ModelData::~ModelData()
{
}

void ModelData::LinkTechniques(Rgph::RenderGraph& rg)
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->LinkTechniques(rg);

#if IS_ENGINE_MODE
		std::ostringstream oss;
		oss << "Добавлено к рендеру [" << modelsName[i] << "]\n";

		applog->AddLog(MODEL_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE
	}
}

void ModelData::Submit(size_t channels)
{
	for (auto& m : models)
	{
		m->Submit(channels);
	}
}

const char* ModelData::GetModelNameByIndex(size_t index)
{
	return modelsName.at(index).c_str();
}

std::unique_ptr<Model>* ModelData::GetPtr2ModelByName(const char* name)
{
	for (int i = 0; i < modelsName.size(); i++)
	{
		if (modelsName[i] == name)
		{
			return &models[i];
		}
	}
}

void ModelData::AddModel(Graphics& gfx, Rgph::RenderGraph& rg, const char* path, const char* name)
{
	modelsName.push_back(name);
	models.emplace_back(std::make_unique<Model>(gfx, path, 1.0f));
	
	DirectX::XMFLOAT3 pos = {0.0f, 0.0f, 0.0f};
	modelsPos.emplace_back(pos);

	DirectX::XMFLOAT3 ori = { 0.0f, 0.0f, 0.0f };
	modelsOrien.emplace_back(ori);

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

void ModelData::Init()
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->SetRootTransform
		(
			dx::XMMatrixRotationX(modelsOrien[i].x) *
			dx::XMMatrixRotationY(modelsOrien[i].y) *
			dx::XMMatrixRotationZ(modelsOrien[i].z) *
			dx::XMMatrixTranslation(
				modelsPos[i].x,
				modelsPos[i].y,
				modelsPos[i].z
			)
		);

#if IS_ENGINE_MODE
		std::ostringstream oss;
		oss << "Установка положения [" << modelsName[i] << "]\n";

		applog->AddLog(MODEL_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE
	}
}

template<typename T>
void ModelData::SetNewValue(const char* modelName, const char* param, T val)
{
	// Открытие файла с триггерами
	std::ifstream dataFile(this->path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о моделях сцен");
	}

#if IS_ENGINE_MODE
	std::ostringstream ostrlog;
	ostrlog << "Установка [" << param << " : " << std::to_string(val) << "] " << "для [" << modelName << "]\n";

	applog->AddLog(MODEL_LOG, ostrlog.str().c_str());
#endif // IS_ENGINE_MODE

	// Чтение файла
	json j;
	dataFile >> j;

	// Закрытие файла
	dataFile.close();

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		for (auto& obj : j.at(modelName))
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

#if IS_ENGINE_MODE
void ModelData::ShowModelsInformation(Graphics& gfx, Rgph::RenderGraph& rg)
{
	if (ImGui::Begin("Объекты", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto& m_name : modelsName)
		{
			char label[128];
			sprintf_s(label, m_name.c_str(), selected);

			ImGui::Bullet();
			if (ImGui::Selectable(label, selected == m_name))
			{
				selected = m_name.c_str();
			}
		}

		if (ImGui::Button("Добавить"))
		{
			ImGuiFileDialog::Instance()->OpenDialog("ModelOD", "Выбирете файл", ".obj,.mtl,.gltf", "");
			applog->AddLog(SYSTEM_LOG, "Добавить модель\n");
		}

		OpenDialog(gfx, rg);
	}

	ImGui::End();
}

void ModelData::ShowModelsProperties()
{
	if (ImGui::Begin("Опции", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (int k = 0; k < modelsName.size(); k++)
		{
			if (modelsName.at(k) == selected)
			{
				// static MP probe{ modelsName.at(k) };
				// probe.SpawnChildWindow(*models.at(k));

				models.at(k)->SpawnDefaultControl();

				if (isSave)
				{
					auto pos = models.at(k)->GetCurrentPosition();
					auto orient = models.at(k)->GetCurrentOrientation();

					applog->AddLog(MODEL_LOG, "Сохранение позиции\n");

					SetNewValue<float>(modelsName.at(k).c_str(), "pos-x", pos.x);
					SetNewValue<float>(modelsName.at(k).c_str(), "pos-y", pos.y);
					SetNewValue<float>(modelsName.at(k).c_str(), "pos-z", pos.z);

					applog->AddLog(MODEL_LOG, "Сохранение ориентации\n");

					SetNewValue<float>(modelsName.at(k).c_str(), "roll", orient.x);
					SetNewValue<float>(modelsName.at(k).c_str(), "pitch", orient.y);
					SetNewValue<float>(modelsName.at(k).c_str(), "yaw", orient.z);

					isSave = false;
				}

				break;
			}
		}

		if (ImGui::Button("Сохранить"))
		{
			isSave = true;
		}
	}

	ImGui::End();
}

void ModelData::OpenDialog(Graphics& gfx, Rgph::RenderGraph& rg)
{
	if (ImGuiFileDialog::Instance()->Display("ModelOD"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string copy1 = filePathName;

			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			std::string copy2 = filePath;

			std::string test = copy1.replace(0, copy2.size() + 1, "");

			for (int i = 0; i != filePathName.size(); i++)
			{
				if (filePathName[i] == '\\')
				{
					filePathName[i] = '/';
				}
			}

			AddModel(gfx, rg, filePathName.c_str(), test.c_str());
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

size_t ModelData::ModelsAmount()
{
	return models.size();
}
#endif // IS_ENGINE_MODE
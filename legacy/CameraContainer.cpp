#include "CameraContainer.h"

#if IS_ENGINE_MODE
#include "imgui/imgui.h"
#endif // IS_ENGINE_MODE

#include "EngineFunctions.hpp"
#include "Camera.h"
#include "Graphics.h"
#include "RenderGraph.h"

#if IS_ENGINE_MODE
CameraContainer::CameraContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg, AppLog* aLog)
	:
	path(path),
	gfx(gfx),
	rg(rg),
	applog(aLog)
#else
CameraContainer::CameraContainer(std::string path, Graphics& gfx)
	:
	path(path),
	gfx(gfx)
#endif // IS_ENGINE_MODE
{
#if IS_ENGINE_MODE
	applog->AddLog(CAMERAS_LOG, "Инициализация\n");
#endif // IS_ENGINE_MODE

	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о камерах");
	}

	json j;
	dataFile >> j;

	dataFile.close();

	ProjectionData prd;

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto& d = m.key();

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

			float pitch = obj.at("pitch");
			float yaw = obj.at("yaw");

			DirectX::XMFLOAT2 orientation = { pitch, yaw };

			/*********************/

			/* Получение проекции */

			float proj_w = obj.at("proj-width");
			float proj_h = obj.at("proj-height");
			float proj_nZ = obj.at("proj-nearZ");
			float proj_fZ = obj.at("proj-farZ");

			prd = { proj_w, proj_h, proj_nZ, proj_fZ };

			/**********************/

			/* Инициализация камеры */

			cameras.emplace_back(std::make_shared<Camera>(gfx, name, position, orientation, prd));

			/************************/
		}
	}

	cameras.shrink_to_fit();
}

Camera* CameraContainer::operator->()
{
	return &GetActiveCamera();
}

CameraContainer::~CameraContainer()
{}

#if IS_ENGINE_MODE
void CameraContainer::LinkTechniques()
{
	for(int i = 0; i < cameras.size(); i++)
	{
		cameras[i]->LinkTechniques(rg);

		std::ostringstream oss;
		oss << "Добавлено к рендеру [" << cameras[i]->GetName() << "]\n";

		applog->AddLog(CAMERAS_LOG, oss.str().c_str());
	}
}
#endif // IS_ENGINE_MODE

void CameraContainer::Submit(size_t channels) const
{
	for( size_t i = 0; i < cameras.size(); i++ )
	{
		if( i != active )
		{
			cameras[i]->Submit( channels );
		}
	}
}

void CameraContainer::Bind()
{
	gfx.SetCamera((*this)->GetMatrix());
}

Camera& CameraContainer::GetActiveCamera()
{
	return *cameras[active];
}

// Test ?
std::shared_ptr<Camera> CameraContainer::GetPtr2ActiveCamera()
{
	return cameras[active];
}

std::shared_ptr<Camera> CameraContainer::GetPtr2CameraByName(std::string name)
{
	for (auto& cam : cameras)
	{
		if (cam->GetName() == name)
		{
			return cam;
		}
	}

	return nullptr;
}

#if IS_ENGINE_MODE
size_t CameraContainer::CamerasAmount()
{
	return cameras.size();
}

std::string CameraContainer::GetCameraNameByIndex(size_t index)
{
	return cameras.at(index)->GetName().c_str();
}

void CameraContainer::ShowLeftPanel()
{
	if (ImGui::Begin("Камеры", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (int i = 0; i < std::size(cameras); i++)
		{
			char label[128];
			sprintf_s(label, cameras[i]->GetName().c_str(), selected);

			ImGui::Bullet();
			if (ImGui::Selectable(label, selected == cameras[i]->GetName().c_str()))
			{
				selected = cameras[i]->GetName();
				active = i;
				controlled = i;
			}
		}

		ImGui::NewLine();

		if (ImGui::Button("Добавить"))
		{
			applog->AddLog(SYSTEM_LOG, "Добавить камеру\n");

			AddRuntimeCamera(std::make_shared<Camera>(gfx, EngineFunctions::AttachStrings(std::string("Camera"), std::to_string(cameras.size() + 1))));
		}

	}

	ImGui::End();
}

void CameraContainer::ShowRightPanel()
{
	if (ImGui::Begin("Опции", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (int k = 0; k < cameras.size(); k++)
		{
			if (cameras[k]->GetName() == selected)
			{
				GetControlledCamera().SpawnDefaultControl(gfx);

				if (IsSave)
				{
					auto position = cameras[k]->GetPosition();
					auto orientation = cameras[k]->GetOrientation();
					auto proj = cameras[k]->GetProjectionData();
					auto name = cameras[k]->GetName();

					applog->AddLog(CAMERAS_LOG, "Сохранение позиции\n");

					EngineFunctions::SetNewValue<float>(name, "pos-x", position.x, path, applog);
					EngineFunctions::SetNewValue<float>(name, "pos-y", position.y, path, applog);
					EngineFunctions::SetNewValue<float>(name, "pos-z", position.z, path, applog);

					applog->AddLog(CAMERAS_LOG, "Сохранение ориентации\n");

					EngineFunctions::SetNewValue<float>(name, "pitch", orientation.x, path, applog);
					EngineFunctions::SetNewValue<float>(name, "yaw", orientation.y, path, applog);

					applog->AddLog(CAMERAS_LOG, "Сохранение проекции\n");
					EngineFunctions::SetNewValue<float>(name, "proj-width", proj.width, path, applog);
					EngineFunctions::SetNewValue<float>(name, "proj-height", proj.height, path, applog);
					EngineFunctions::SetNewValue<float>(name, "proj-farZ", proj.farZ, path, applog);
					EngineFunctions::SetNewValue<float>(name, "proj-nearZ", proj.nearZ, path, applog);

					IsSave = false;
				}

				if (IsDelete)
				{
					DeleteCamera(cameras[k]->GetName());
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

void CameraContainer::OpenDialog()
{
}
#endif // IS_ENGINE_MODE

void CameraContainer::AddCamera(std::shared_ptr<Camera> pCam)
{
	cameras.push_back(std::move(pCam));

#if IS_ENGINE_MODE
	std::ostringstream oss;
	oss << "Добавлено [" << cameras.at(cameras.size() - 1)->GetName() << "]\n";

	applog->AddLog(CAMERAS_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE
}

void CameraContainer::AddRuntimeCamera(std::shared_ptr<Camera> pCam)
{
	using std::to_string;

	// Открытие файла с данными о моделях
	std::ifstream dataFile(this->path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о камерах сцены");
	}

#if IS_ENGINE_MODE
	applog->AddLog(CAMERAS_LOG, "Добавление\n");
#endif // IS_ENGINE_MODE

	// Чтение файла
	json j;
	dataFile >> j;

	// Закрытие файла
	dataFile.close();

	// Новая камера
	std::ostringstream newCam;
	newCam << "\"" << pCam->GetName() << "\":[{";

	newCam << "\"name\": \"" << pCam->GetName() << "\",";

	newCam << "\"pos-x\": " <<  pCam->GetPosition().x << ",";
	newCam << "\"pos-y\" : " << pCam->GetPosition().y << ",";
	newCam << "\"pos-z\" : " << pCam->GetPosition().z << ",";

	newCam << "\"proj-farZ\" : " << pCam->GetProjectionData().farZ << ",";
	newCam << "\"proj-height\" : " << pCam->GetProjectionData().height << ",";
	newCam << "\"proj-nearZ\" : " << pCam->GetProjectionData().nearZ << ",";
	newCam << "\"proj-width\" : " << pCam->GetProjectionData().width << ",";

	newCam << "\"pitch\": " << pCam->GetOrientation().x << ",";
	newCam << "\"yaw\": " << pCam->GetOrientation().y << "}]";

	// Подготовка к вставке в файл
	std::string json_str = j.dump();
	size_t pos_of_par = json_str.find_last_of('}');
	size_t pos_of_par2 = json_str.find_last_of(']');

	json_str.at(pos_of_par) = ' ';
	json_str.at(pos_of_par2 + 1) = ',';

	// Запись в файл данных новой модели
	std::ofstream ostream(this->path);
	ostream << json_str + newCam.str() + '}';

	// Закрытие файла
	ostream.close();

	pCam->LinkTechniques(rg);

	AddCamera(pCam);
}

void CameraContainer::DeleteCamera(std::string name)
{
	for (auto cam = cameras.begin(); cam != cameras.end(); ++cam)
	{
		if (cam->get()->GetName() == name)
		{
			cameras.erase(cam);
		}
	}

	active = 0;
	controlled = 0;
}

Camera& CameraContainer::GetControlledCamera()
{
	return *cameras[controlled];
}
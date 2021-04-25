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

			/* Инициализация камеры */

			cameras.emplace_back(std::make_shared<Camera>(gfx, name, position, orientation));

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
		if (ImGui::BeginCombo("Акт. камера", (*this)->GetName().c_str()))
		{
			for (int i = 0; i < std::size(cameras); i++)
			{
				const bool isSelected = i == active;
				if (ImGui::Selectable(cameras[i]->GetName().c_str(), isSelected))
				{
					active = i;
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Упр. камера", GetControlledCamera().GetName().c_str()))
		{
			for (int i = 0; i < std::size(cameras); i++)
			{
				const bool isSelected = i == controlled;
				if (ImGui::Selectable(cameras[i]->GetName().c_str(), isSelected))
				{
					controlled = i;
				}

				if (IsSave)
				{
					auto position = cameras[i]->GetPosition();
					auto orientation = cameras[i]->GetOrientation();
					auto name = cameras[i]->GetName();

					applog->AddLog(MODEL_LOG, "Сохранение позиции\n");

					EngineFunctions::SetNewValue<float>(name, "pos-x", position.x, path, applog);
					EngineFunctions::SetNewValue<float>(name, "pos-y", position.y, path, applog);
					EngineFunctions::SetNewValue<float>(name, "pos-z", position.z, path, applog);

					applog->AddLog(MODEL_LOG, "Сохранение ориентации\n");

					EngineFunctions::SetNewValue<float>(name, "pitch", orientation.x, path, applog);
					EngineFunctions::SetNewValue<float>(name, "yaw", orientation.y, path, applog);

					IsSave = false;
				}

				if (IsDelete)
				{

					IsDelete = false;
				}

			}
			ImGui::EndCombo();
		}

		GetControlledCamera().SpawnDefaultControl(gfx);

		if (ImGui::Button("Удалить", ImVec2(100, 20)))
		{
			IsDelete = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Сохранить", ImVec2(100, 20)))
		{
			IsSave = true;
		}
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

void CameraContainer::DeleteCamera(std::string name)
{
	for (auto cam = cameras.begin(); cam != cameras.end(); ++cam)
	{
		if (cam->get()->GetName() == name)
		{
			cameras.erase(cam);
		}
	}
}

Camera& CameraContainer::GetControlledCamera()
{
	return *cameras[controlled];
}
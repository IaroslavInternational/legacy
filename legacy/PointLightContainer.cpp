#include "PointLightContainer.h"

#include "EngineFunctions.hpp"

#if IS_ENGINE_MODE
#include "imgui\imgui.h"
#include "TestModelProbe.h"
#endif // IS_ENGINE_MODE

#if IS_ENGINE_MODE
PointLightContainer::PointLightContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg, AppLog* aLog)
	:
	path(path),
	gfx(gfx),
	rg(rg),
	applog(aLog)
#else
PointLightContainer::PointLightContainer(std::string path, Graphics& gfx)
	:
	path(path),
	gfx(gfx)
#endif // IS_ENGINE_MODE
{
#if IS_ENGINE_MODE
	applog->AddLog(POINTLIGHTS_LOG, "Инициализация\n");
#endif // IS_ENGINE_MODE

	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о точечном освещении");
	}

	json j;
	dataFile >> j;

	PointLightCBuf data;

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto& d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* Запись имени объекта */

			std::string name = d.c_str();

			/************************/

			/* Запись позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");
			float pos_z = obj.at("pos-z");

			data.pos = DirectX::XMFLOAT3(pos_x, pos_y, pos_z);

			/******************/

			/* Запись цветовых параметров */
			
			float ambient_r = obj.at("ambient-r");
			float ambient_g = obj.at("ambient-g");
			float ambient_b = obj.at("ambient-b");

			data.ambient = DirectX::XMFLOAT3(ambient_r, ambient_g, ambient_b);

			float diffuse_r = obj.at("diffuse-r");
			float diffuse_g = obj.at("diffuse-g");
			float diffuse_b = obj.at("diffuse-b");

			data.diffuseColor = DirectX::XMFLOAT3(diffuse_r, diffuse_g, diffuse_b);

			float diffuse_intensity = obj.at("diffuse-intensity");

			data.diffuseIntensity = diffuse_intensity;

			float att_const = obj.at("att-const");

			data.attConst = att_const;

			float att_lin = obj.at("att-lin");

			data.attLin = att_lin;

			float att_quad = obj.at("att-quad");

			data.attQuad = att_quad;

			/******************************/

			/* Запись источника освещения */

			pLights.emplace_back(std::make_unique<PointLight>(gfx, name, data));

			/**************************************************/
		}
	}
}

PointLightContainer::~PointLightContainer()
{
}

#if IS_ENGINE_MODE
void PointLightContainer::LinkTechniques()
{
	for (int i = 0; i < pLights.size(); i++)
	{
		pLights[i]->LinkTechniques(rg);

#if IS_ENGINE_MODE
		std::ostringstream oss;
		oss << "Добавлено к рендеру [" << pLights[i]->GetName() << "]\n";

		applog->AddLog(POINTLIGHTS_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE

	}
}

void PointLightContainer::Submit(size_t channels)
{
	for (auto& pl : pLights)
	{
		pl->Submit(channels);
	}
}
#endif // IS_ENGINE_MODE

void PointLightContainer::Bind(DirectX::FXMMATRIX view)
{
	for (auto& pl : pLights)
	{
		pl->Bind(gfx, view);
	}
}

void PointLightContainer::RgBindShadowCamera(Rgph::BlurOutlineRenderGraph& rg)
{
	for (auto& pl : pLights)
	{
		rg.BindShadowCamera(*pl->ShareCamera());
	}
}

void PointLightContainer::AddCamerasToLight(CameraContainer* camcon)
{
	for (auto& pl : pLights)
	{
		camcon->AddCamera(pl.get()->ShareCamera());
	}
}

#if IS_ENGINE_MODE
void PointLightContainer::ShowLeftPanel()
{
	if (ImGui::Begin("Источники освещения", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto& pl : pLights)
		{
			char label[128];
			sprintf_s(label, pl->GetName().c_str(), selected);

			ImGui::Bullet();
			if (ImGui::Selectable(label, selected == pl->GetName()))
			{
				selected = pl->GetName();
			}
		}
	}

	ImGui::End();
}

void PointLightContainer::ShowRightPanel()
{
	if (ImGui::Begin("Опции", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (int k = 0; k < pLights.size(); k++)
		{
			if (pLights[k]->GetName() == selected)
			{
				pLights[k]->SpawnDefaultControl();

				if (IsSave)
				{
					auto lightData = pLights[k]->GetData();
					auto name = pLights[k]->GetName();

					applog->AddLog(POINTLIGHTS_LOG, "Сохранение позиции\n");

					EngineFunctions::SetNewValue<float>(name, "pos-x", lightData.pos.x, path, applog);
					EngineFunctions::SetNewValue<float>(name, "pos-y", lightData.pos.y, path, applog);
					EngineFunctions::SetNewValue<float>(name, "pos-z", lightData.pos.z, path, applog);

					applog->AddLog(POINTLIGHTS_LOG, "Сохранение цветов\n");

					EngineFunctions::SetNewValue<float>(name, "ambient-r", lightData.ambient.x, path, applog);
					EngineFunctions::SetNewValue<float>(name, "ambient-g", lightData.ambient.y, path, applog);
					EngineFunctions::SetNewValue<float>(name, "ambient-b", lightData.ambient.z, path, applog);

					EngineFunctions::SetNewValue<float>(name, "diffuse-r", lightData.diffuseColor.x, path, applog);
					EngineFunctions::SetNewValue<float>(name, "diffuse-g", lightData.diffuseColor.y, path, applog);
					EngineFunctions::SetNewValue<float>(name, "diffuse-b", lightData.diffuseColor.z, path, applog);

					EngineFunctions::SetNewValue<float>(name, "diffuse-intensity", lightData.diffuseIntensity, path, applog);

					EngineFunctions::SetNewValue<float>(name, "att-const", lightData.attConst, path, applog);
					EngineFunctions::SetNewValue<float>(name, "att-lin", lightData.attLin, path, applog);
					EngineFunctions::SetNewValue<float>(name, "att-quad", lightData.attQuad, path, applog);

					IsSave = false;
				}

				break;
			}
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

	ImGui::End();
}
#endif // IS_ENGINE_MODE
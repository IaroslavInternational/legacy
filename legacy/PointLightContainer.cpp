#include "PointLightContainer.h"

#if IS_ENGINE_MODE
#include "imgui\imgui.h"
#include "TestModelProbe.h"
#endif // IS_ENGINE_MODE

#include <sstream>
#include <fstream>
#include <filesystem>
#include "json.hpp"

using json = nlohmann::json;
using namespace std::string_literals;

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

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* Запись имени объекта */

			std::string name = d.c_str();

			/************************/

			/* Запись позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");
			float pos_z = obj.at("pos-z");

			DirectX::XMFLOAT3 position = { pos_x, pos_y, pos_z };

			/******************/

			/* Запись источника освещения */

			pLights.emplace_back(std::make_unique<PointLight>(gfx, name, position));

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
				break;
			}
		}
	}
	ImGui::End();
}
#endif // IS_ENGINE_MODE
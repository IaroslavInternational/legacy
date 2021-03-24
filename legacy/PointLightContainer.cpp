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
PointLightContainer::PointLightContainer(const char* path, Graphics& gfx, AppLog* aLog)
	:
	applog(aLog)
{
	applog->AddLog(POINTLIGHTS_LOG, "Инициализация\n");

	const auto dataPath = path;

	std::ifstream dataFile(dataPath);
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
			pLightsName.push_back(name);

			/************************/

			/* Запись позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");
			float pos_z = obj.at("pos-z");

			DirectX::XMFLOAT3 position = { pos_x, pos_y, pos_z };

			pLightsPos.emplace_back(position);

			/******************/

			/* Запись источника освещения */

			pLights.emplace_back(std::make_unique<PointLight>(gfx, name, position));

			/**************************************************/
		}
	}
}
#else
PointLightContainer::PointLightContainer(const char* path, Graphics& gfx)
{
	const auto dataPath = path;

	std::ifstream dataFile(dataPath);
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
			pLightsName.push_back(name);

			/************************/

			/* Запись позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");
			float pos_z = obj.at("pos-z");

			DirectX::XMFLOAT3 position = { pos_x, pos_y, pos_z };

			pLightsPos.emplace_back(position);

			/******************/

			/* Запись источника освещения */

			pLights.emplace_back(std::make_unique<PointLight>(gfx, name, position));

			/**************************************************/
		}
	}
}
#endif // IS_ENGINE_MODE

PointLightContainer::~PointLightContainer()
{
}

void PointLightContainer::LinkTechniques(Rgph::RenderGraph& rg)
{
	for (int i = 0; i < pLights.size(); i++)
	{
		pLights[i]->LinkTechniques(rg);

#if IS_ENGINE_MODE
		std::ostringstream oss;
		oss << "Добавлено к рендеру [" << pLightsName[i] << "]\n";

		applog->AddLog(POINTLIGHTS_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE

	}
}

void PointLightContainer::Bind(Graphics& gfx, DirectX::FXMMATRIX view)
{
	for (auto& pl : pLights)
	{
		pl->Bind(gfx, view);
	}
}

void PointLightContainer::Submit(size_t channels)
{
	for (auto& pl : pLights)
	{
		pl->Submit(channels);
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
void PointLightContainer::ShowPLightsInformation()
{
	if (ImGui::Begin("Источники освещения", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto& pln : pLightsName)
		{
			char label[128];
			sprintf_s(label, pln.c_str(), selected);

			ImGui::Bullet();
			if (ImGui::Selectable(label, selected == pln))
			{
				selected = pln.c_str();
			}
		}
	}
	ImGui::End();
}

void PointLightContainer::ShowPLightsProperties()
{
	if (ImGui::Begin("Опции", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (int k = 0; k < pLightsName.size(); k++)
		{
			if (pLightsName.at(k) == selected)
			{
				pLights.at(k)->SpawnControlWindow();
				break;
			}
		}
	}
	ImGui::End();
}
#endif // IS_ENGINE_MODE
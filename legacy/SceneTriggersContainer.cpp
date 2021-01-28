#include "SceneTriggersContainer.h"

#include "imgui\imgui.h"

#include <sstream>
#include <fstream>
#include <filesystem>
#include "json.hpp"

using json = nlohmann::json;
using namespace std::string_literals;

SceneTriggersContainer::SceneTriggersContainer(const char* path, Graphics& gfx)
{
	const auto dataPath = path;

	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о триггерах сцен");
	}
	
	dx::XMFLOAT3 pos_lt;
	dx::XMFLOAT3 pos_rt;
	dx::XMFLOAT3 pos_lb;
	dx::XMFLOAT3 pos_rb;

	float roll;
	float pitch;
	float yaw;

	json j;
	dataFile >> j;

	for (json::iterator t = j.begin(); t != j.end(); ++t)
	{
		auto d = t.key();

		for (const auto& obj : j.at(d))
		{
			/* Запись позиции триггера */

			for (const auto& pos : obj.at("pos-lt"))
			{
				pos_lt = { pos.at("pos-x"), pos.at("pos-y"), pos.at("pos-z") };
			}

			for (const auto& pos : obj.at("pos-rt"))
			{
				pos_rt = { pos.at("pos-x"), pos.at("pos-y"), pos.at("pos-z") };
			}

			for (const auto& pos : obj.at("pos-lb"))
			{
				pos_lb = { pos.at("pos-x"), pos.at("pos-y"), pos.at("pos-z") };
			}

			for (const auto& pos : obj.at("pos-rb"))
			{
				pos_rb = { pos.at("pos-x"), pos.at("pos-y"), pos.at("pos-z") };
			}


			roll = obj.at("roll");
			pitch = obj.at("pitch");
			yaw = obj.at("yaw");

			/***************************/

			/* Запись указателя триггера */

			std::string ptr = obj.at("ptr");
			ptr2scs.emplace_back(ptr);

			/*****************************/

			trss.emplace_back(pos_lt, pos_rt, pos_lb, pos_rb, roll, pitch, yaw);
		}
	}

	for (int i = 0; i < trss.size(); i++)
	{
		trig_sc_container.emplace(ptr2scs.at(i).c_str(), std::make_unique<Trigger>(trss.at(i), gfx));
	}
}

SceneTriggersContainer::~SceneTriggersContainer()
{}

std::pair<const char*, bool> SceneTriggersContainer::CheckTriggers(dx::XMFLOAT3 pos)
{
	for (auto it = trig_sc_container.begin(); it != trig_sc_container.end(); it++)
	{
		if (it->second->Check(pos))
		{
			const char* HittedTriggerGoal;
			HittedTriggerGoal = it->first;

			return std::make_pair(HittedTriggerGoal, true);
		}
	}

	return std::make_pair("NULL", false);
}

void SceneTriggersContainer::ShowTrigInformation()
{
	if (ImGui::Begin("Триггеры", NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto& [key, value] : trig_sc_container)
		{
			char label[128];

			sprintf_s(label, key, selected);

			ImGui::Bullet();
			if (ImGui::Selectable(label, selected == key))
			{
				selected = key;
			}
		}
	}

	ImGui::End();
}

void SceneTriggersContainer::ShowTrigSettings()
{
	if (ImGui::Begin("Информация", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (const auto& [key, value] : trig_sc_container)
		{
			if (key == selected)
			{
				auto pos = value->GetPosition();

				ImGui::Text("Позиция");
				ImGui::Text("Левый верхний угол: \n x = %.3f; y = %.3f; z = %.3f;",
					pos[0].x, pos[0].y, pos[0].z);
				ImGui::Text("Правый верхний угол: \n x = %.3f; y = %.3f; z = %.3f;",
					pos[1].x, pos[1].y, pos[1].z);
				ImGui::Text("Левый нижний угол: \n x = %.3f; y = %.3f; z = %.3f;",
					pos[2].x, pos[2].y, pos[2].z);
				ImGui::Text("Правый нижний угол: \n x = %.3f; y = %.3f; z = %.3f;",
					pos[3].x, pos[3].y, pos[3].z);

				ImGui::Separator();
				ImGui::Text("Триггер указывает на: %s", selected);

				break;
			}
		}
	}

	ImGui::End();
}

std::map<const char*, std::unique_ptr<Trigger>>* SceneTriggersContainer::GetData()
{
	return &trig_sc_container;
}

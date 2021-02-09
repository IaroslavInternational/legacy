#include "SceneTriggersContainer.h"

#include "imgui\imgui.h"

#include <sstream>
#include <fstream>
#include <filesystem>
#include "json.hpp"

using json = nlohmann::json;
using namespace std::string_literals;

SceneTriggersContainer::SceneTriggersContainer(const char* path, Graphics& gfx)
	:
	filePath(path)
{
	std::ifstream dataFile(filePath);
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

	dataFile.close();
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
	bool IsAdd = false;

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

		if (ImGui::Button("Добавить"))
		{
			ImGui::OpenPopup("Создание триггера");
		}

		if (ImGui::BeginPopupModal("Создание триггера", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Введите параметры триггеры");
			ImGui::Separator();

			ImGui::InputText("Имя", name, sizeof(name));

			ImGui::InputText("Цель", goal, sizeof(goal));

			ImGui::SliderFloat3("Левый верхний угол", pos_lt, -80.0f, 80.0f);
			ImGui::SliderFloat3("Правый верхний угол", pos_rt, -80.0f, 80.0f);
			ImGui::SliderFloat3("Левый нижний угол", pos_lb, -80.0f, 80.0f);
			ImGui::SliderFloat3("Правый нижний угол", pos_rb, -80.0f, 80.0f);

			ImGui::SliderAngle("Крен", &orient[0], -180.0f, 180.0f);
			ImGui::SliderAngle("Тангаж", &orient[1], -180.0f, 180.0f);
			ImGui::SliderAngle("Рысканье", &orient[2], -180.0f, 180.0f);
			
			if (ImGui::Button("Добавить", ImVec2(120, 0)))
			{
				dx::XMFLOAT3 lt = { pos_lt[0], pos_lt[1], pos_lt[2] };;
				dx::XMFLOAT3 rt = { pos_rt[0], pos_rt[1], pos_rt[2] };;
				dx::XMFLOAT3 lb = { pos_lb[0], pos_lb[1], pos_lb[2] };;
				dx::XMFLOAT3 rb = { pos_rb[0], pos_rb[1], pos_rb[2] };;

				TriggerStruct trs;
				trs.PosTopLeft = lt;
				trs.PosTopRight = rt;
				trs.PosBottomLeft = lb;
				trs.PosBottomRight = rb;

				trs.Roll = orient[0];
				trs.Pitch = orient[1];
				trs.Yaw = orient[2];

				LoadTrigger(std::string(name), std::string(goal), trs);

				IsAdd = true;

				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Закрыть", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void SceneTriggersContainer::ShowTrigSettings()
{
	if (ImGui::Begin("Информация", NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
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

void SceneTriggersContainer::LoadTrigger(std::string name, std::string ptr, TriggerStruct& trs)
{
	using std::to_string;

	// Открытие файла с триггерами
	std::ifstream dataFile(filePath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о триггерах сцен");
	}

	// Чтение файла
	json j;
	dataFile >> j;

	// Закрытие файла
	dataFile.close();

	// Число триггеров в файле + 1
	int triggerIndex = trig_sc_container.size() + 1;

	// Новый триггер
	std::string newTrigger = "\"Trigger " + to_string(triggerIndex);
	
	newTrigger += "\": [{\"name\": \"" + name + "\",";

	newTrigger += "\"pos-lt\" :";
	newTrigger += "[{\"pos-x\": " + to_string(trs.PosTopLeft.x) + ",";
	newTrigger += "\"pos-y\" : " + to_string(trs.PosTopLeft.y) + ",";
	newTrigger += "\"pos-z\" : " + to_string(trs.PosTopLeft.z) + "}],";

	newTrigger += "\"pos-rt\" :";
	newTrigger += "[{\"pos-x\": " + to_string(trs.PosTopRight.x) + ",";
	newTrigger += "\"pos-y\" : " + to_string(trs.PosTopRight.y) + ",";
	newTrigger += "\"pos-z\" : " + to_string(trs.PosTopRight.z) + "}],";

	newTrigger += "\"pos-lb\" :";
	newTrigger += "[{\"pos-x\": " + to_string(trs.PosBottomLeft.x) + ",";
	newTrigger += "\"pos-y\" : " + to_string(trs.PosBottomLeft.y) + ",";
	newTrigger += "\"pos-z\" : " + to_string(trs.PosBottomLeft.z) + "}],";
	
	newTrigger += "\"pos-rb\" :";
	newTrigger += "[{\"pos-x\": " + to_string(trs.PosBottomRight.x) + ",";
	newTrigger += "\"pos-y\" : " + to_string(trs.PosBottomRight.y) + ",";
	newTrigger += "\"pos-z\" : " + to_string(trs.PosBottomRight.z) + "}],";

	newTrigger += "\"ptr\": \"" + ptr + "\",";

	newTrigger += "\"roll\": " + to_string(trs.Roll) + ",";
	newTrigger += "\"pitch\": " + to_string(trs.Pitch) + ",";
	newTrigger += "\"yaw\": " + to_string(trs.Yaw) + "}]";

	// Подготовка к вставке в файл
	std::string json_str = j.dump();
	size_t pos_of_par = json_str.find_last_of('}');
	size_t pos_of_par2 = json_str.find_last_of(']');

	json_str.at(pos_of_par) = ' ';
	json_str.at(pos_of_par2 + 1) = ',';

	// Запись в файл нового триггера
	std::ofstream ostr(filePath);
	ostr << json_str + newTrigger + '}';

	// Закрытие файла
	ostr.close();
}
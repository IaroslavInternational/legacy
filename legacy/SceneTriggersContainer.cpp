#include "SceneTriggersContainer.h"

#if IS_ENGINE_MODE
#include "imgui\imgui.h"
#endif // IS_ENGINE_MODE

#include <sstream>
#include <fstream>
#include <filesystem>

#include "json.hpp"

using json = nlohmann::json;
using namespace std::string_literals;

#if IS_ENGINE_MODE
SceneTriggersContainer::SceneTriggersContainer(const char* path, Graphics& gfx, AppLog* aLog)
	:
	filePath(path),
	applog(aLog)
{
	applog->AddLog(TRIGGER_LOG, "Инициализация\n");

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
		auto &d = t.key();

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

			std::string Ptr = obj.at("ptr");

			std::string Name = obj.at("name");

			/*****************************/

			trig_sc_container.emplace(
				Ptr, 
				std::make_unique<Trigger>
				(
					Name,
					TriggerStruct(pos_lt, pos_rt, pos_lb, pos_rb, roll, pitch, yaw), 
					gfx
				)
			);
		}
	}

	dataFile.close();
}
#else
SceneTriggersContainer::SceneTriggersContainer(const char* path)
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

			/*****************************/

			trig_sc_container.emplace(
				Ptr,
				std::make_unique<Trigger>
				(
					Name,
					TriggerStruct(pos_lt, pos_rt, pos_lb, pos_rb, roll, pitch, yaw)
				)
			);
		}
	}

	dataFile.close();
}
#endif // IS_ENGINE_MODE

SceneTriggersContainer::~SceneTriggersContainer()
{}

#if IS_ENGINE_MODE
void SceneTriggersContainer::LinkTechniques(Rgph::RenderGraph &rg)
{
	for (auto it = trig_sc_container.begin(); it != trig_sc_container.end(); ++it)
	{
		it->second->LinkTechniques(rg);
		it->second->SetDefault();

#if IS_ENGINE_MODE
		std::ostringstream oss;
		oss << "Добавлено к рендеру [" << std::string(it->first) << "]\n";

		applog->AddLog(TRIGGER_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE
	}
}

void SceneTriggersContainer::Submit(size_t channels)
{
	for (auto it = trig_sc_container.begin(); it != trig_sc_container.end(); ++it)
	{
		it->second->Submit(channels);
	}
}
#endif // IS_ENGINE_MODE

std::pair<std::string, bool> SceneTriggersContainer::CheckTriggers(dx::XMFLOAT3 pos)
{
	for (auto it = trig_sc_container.begin(); it != trig_sc_container.end(); it++)
	{
		if (it->second->Check(pos))
		{
			auto HittedTriggerGoal = it->first;

#if IS_ENGINE_MODE
			std::ostringstream oss;
			oss << "Касание [" << static_cast<std::string>(HittedTriggerGoal) << "]\n";

			applog->AddLog(TRIGGER_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE

			return std::make_pair(HittedTriggerGoal, true);
		}
	}

	return std::make_pair("NULL", false);
}

#if IS_ENGINE_MODE
void SceneTriggersContainer::ShowTrigInformation(Graphics& gfx, Rgph::RenderGraph& rg)
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

			sprintf_s(label, value->GetName().c_str(), selected.c_str());

			ImGui::Bullet();
			if (ImGui::Selectable(label, selected == value->GetName().c_str()))
			{
				selected = std::move(value->GetName());
			}
		}

		if (ImGui::Button("Добавить"))
		{
			ImGui::OpenPopup("Создание триггера");
		}

		if (ImGui::BeginPopupModal("Создание триггера", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Введите параметры триггера");
			ImGui::Separator();

			ImGui::InputText("Имя", name, sizeof(name));

			ImGui::InputText("Цель", goal, sizeof(goal));

			ImGui::InputFloat("Ширина", &new_w, -30.0f, 30.0f, "%.2f");
			ImGui::InputFloat("Высота", &new_h, -30.0f, 30.0f, "%.2f");
			
			if (ImGui::Button("Добавить", ImVec2(120, 0)))
			{
				dx::XMFLOAT3 lt = { new_w, new_h, new_w };
				dx::XMFLOAT3 rt = { new_w, new_h, new_w / 2 };
				dx::XMFLOAT3 lb = { new_w, 0.0f,  new_w };
				dx::XMFLOAT3 rb = { new_w, 0.0f,  new_w / 2 };

				TriggerStruct trs;
				trs.PosTopLeft = lt;
				trs.PosTopRight = rt;
				trs.PosBottomLeft = lb;
				trs.PosBottomRight = rb;

				trs.Roll = 0.0f;
				trs.Pitch = 1.57f;
				trs.Yaw = 0.0f;

				AddTrigger(gfx, std::string(name), std::string(goal), trs, rg);

				IsAdd = true;

				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Отмена", ImVec2(120, 0)))
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
			if (value->GetName() == selected)
			{
				auto pos = value->GetPosition();

				value->SpawnControl();
				
				ImGui::NewLine();

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
				ImGui::Text("Триггер указывает на: %s", key);
				
				ImGui::EndChild();
				
				break;
			}
		}
	}

	ImGui::End();
}

void SceneTriggersContainer::AddTrigger(Graphics& gfx, std::string name, std::string ptr, TriggerStruct& trs,
	Rgph::RenderGraph& rg)
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
	std::ostringstream newTrigger;
	newTrigger << "\"Trigger " << triggerIndex;
	
	newTrigger << "\": [{\"name\": \"" + name + "\",";

	newTrigger << "\"pos-lt\" :";
	newTrigger << "[{\"pos-x\": " << trs.PosTopLeft.x << ",";
	newTrigger << "\"pos-y\" : " << trs.PosTopLeft.y << ",";
	newTrigger << "\"pos-z\" : " << trs.PosTopLeft.z << "}],";

	newTrigger << "\"pos-rt\" :";
	newTrigger << "[{\"pos-x\": " << trs.PosTopRight.x << ",";
	newTrigger << "\"pos-y\" : " << trs.PosTopRight.y << ",";
	newTrigger << "\"pos-z\" : " << trs.PosTopRight.z << "}],";

	newTrigger << "\"pos-lb\" :";
	newTrigger << "[{\"pos-x\": " << trs.PosBottomLeft.x << ",";
	newTrigger << "\"pos-y\" : " << trs.PosBottomLeft.y << ",";
	newTrigger << "\"pos-z\" : " << trs.PosBottomLeft.z << "}],";
	
	newTrigger << "\"pos-rb\" :";
	newTrigger << "[{\"pos-x\": " << trs.PosBottomRight.x << ",";
	newTrigger << "\"pos-y\" : " << trs.PosBottomRight.y << ",";
	newTrigger << "\"pos-z\" : " << trs.PosBottomRight.z << "}],";

	newTrigger << "\"ptr\": \"" << ptr << "\",";

	newTrigger << "\"roll\": " << trs.Roll << ",";
	newTrigger << "\"pitch\": " << trs.Pitch << ",";
	newTrigger << "\"yaw\": " << trs.Yaw << "}]";

	// Подготовка к вставке в файл
	std::string json_str = j.dump();
	size_t pos_of_par = json_str.find_last_of('}');
	size_t pos_of_par2 = json_str.find_last_of(']');

	json_str.at(pos_of_par) = ' ';
	json_str.at(pos_of_par2 + 1) = ',';

	// Запись в файл нового триггера
	std::ofstream ostr(filePath);
	ostr << json_str << newTrigger.str() << '}';

	// Закрытие файла
	ostr.close();

	trig_sc_container.emplace(ptr, std::make_unique<Trigger>(name, trs, gfx));

	for (auto& [key, value] : trig_sc_container)
	{
		if (value->GetName() == name)
		{
			value->LinkTechniques(rg);
			value->SetDefault();

#if IS_ENGINE_MODE
			std::ostringstream oss;
			oss << "Добавлено к рендеру [" << name << "]\n";

			applog->AddLog(TRIGGER_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE
		}
	}
}
#endif // IS_ENGINE_MODE
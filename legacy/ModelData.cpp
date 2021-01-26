#include "ModelData.h"
#include "imgui\imgui.h"
#include "TestModelProbe.h"

using json = nlohmann::json;
using namespace std::string_literals;

ModelData::ModelData(const char* path, Graphics& gfx)
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
}

ModelData::~ModelData()
{
}

void ModelData::ShowInterface()
{
	ShowLeftSideInterface();
	ShowRightSideInterface();
}

void ModelData::ShowLeftSideInterface()
{
	/* Левая сторона */

	const float DISTANCE = 0.0f;
	static int corner = 0;

	ImGuiIO& io = ImGui::GetIO();
	
	ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - ImGui::GetMenuHeight() : ImGui::GetMenuHeight());
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, 0, window_pos_pivot);
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.2f, io.DisplaySize.y * 0.75f), ImGuiCond_FirstUseEver);
	
	if (ImGui::Begin("Объекты", (bool*)(true), 
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::Text("Объекты");
		for (auto& m_name : modelsName)
		{
			char label[128];
			sprintf_s(label, m_name.c_str(), selected);

			if (ImGui::Selectable(label, selected == m_name))
			{
				selected = m_name.c_str();
			}
		}
	}
	ImGui::End();
	ImGui::GetStyle().DisplayWindowPadding = { 0, 0 };
	ImGui::GetStyle().DisplaySafeAreaPadding = { 0, 0 };
	/* Конец левой стороны */
}

void ModelData::ShowRightSideInterface()
{
	/* Правая сторона */
	const float DISTANCE = 0.0f;
	static int corner = 1;

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - ImGui::GetMenuHeight() : ImGui::GetMenuHeight());
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, 0, window_pos_pivot);
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.2f, io.DisplaySize.y * 0.75f), ImGuiCond_FirstUseEver);
	
	if (ImGui::Begin("Опции", (bool*)(true),
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{
		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginChild("Положение"))
			{
				for (int k = 0; k < modelsName.size(); k++)
				{
					if (modelsName.at(k) == selected)
					{
						static MP probe{ modelsName.at(k) };
						probe.SpawnChildWindow(*models.at(k));

						break;
					}
				}
				ImGui::EndChild();
			}

			if (ImGui::BeginTabItem("Информация"))
			{
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();

	/* Конец правой стороны */
}

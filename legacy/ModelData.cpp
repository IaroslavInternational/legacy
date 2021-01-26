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

void ModelData::ShowModelsInformation(bool* is_Active)
{
	if (ImGui::Begin("Объекты", is_Active,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::Text("Объекты");
		ImGui::Separator();

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
	}
	ImGui::End();
}

void ModelData::ShowModelsProperties(bool* is_Active)
{
	if (ImGui::Begin("Опции", is_Active,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{

		ImGui::Text("Положение");
		ImGui::Separator();

		for (int k = 0; k < modelsName.size(); k++)
		{
			if (modelsName.at(k) == selected)
			{
				static MP probe{ modelsName.at(k) };
				probe.SpawnChildWindow(*models.at(k));

				break;
			}
		}
	}
	ImGui::End();
}

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

	Init();
}

ModelData::~ModelData()
{
}

void ModelData::LinkTechniques(Rgph::RenderGraph& rg)
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->LinkTechniques(rg);

		//std::ostringstream oss;
		//oss << "[Модели]: " << "Загружена модель [" << md.modelsName[i] << "]\n";

		//log.AddLog(oss.str().c_str());
	}
}

void ModelData::Submit(size_t channels)
{
	for (auto& m : models)
	{
		m->Submit(channels);
	}
}

void ModelData::AddTestModel(Graphics& gfx)
{
	modelsName.push_back("goblin");
	models.emplace_back(std::make_unique<Model>(gfx, "Scenes\\Scene 1\\Models\\gobber\\GoblinX.obj", 4.0f));
	
	DirectX::XMFLOAT3 pos = {-8.0f, 10.0f, 0.0f};
	modelsPos.emplace_back(pos);

	DirectX::XMFLOAT3 ori = { 0.0f, -1.57f, 0.0f };
	modelsOrien.emplace_back(ori);
}

void ModelData::Init()
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->SetRootTransform
		(
			dx::XMMatrixRotationX(modelsOrien[i].x) *
			dx::XMMatrixRotationY(modelsOrien[i].y) *
			dx::XMMatrixRotationZ(modelsOrien[i].z) *
			dx::XMMatrixTranslation(
				modelsPos[i].x,
				modelsPos[i].y,
				modelsPos[i].z
			)
		);
	}
}

void ModelData::ShowModelsInformation()
{
	if (ImGui::Begin("Объекты", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
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

void ModelData::ShowModelsProperties()
{
	if (ImGui::Begin("Опции", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
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
	}

	ImGui::End();
}

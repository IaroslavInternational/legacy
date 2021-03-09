#include "ModelData.h"
#include "imgui\imgui.h"
#include "TestModelProbe.h"

using json = nlohmann::json;
using namespace std::string_literals;

ModelData::ModelData(const char* path, Graphics& gfx)
	:
	path(path)
{
	const auto dataPath = path;

	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("�� ������ ������� ���� � ������� � �������");
	}

	json j;
	dataFile >> j;

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto d = m.key();
		
		for (const auto& obj : j.at(d))
		{
			/* ������ ����� ������� */

			std::string name = obj.at("name");
			modelsName.push_back(name);

			/************************/

			/* ������ ������, ������������� �� ���� � ������� */

			std::string modelPath = obj.at("path");
			float modelScale = obj.at("scale");
			models.emplace_back(std::make_unique<Model>(gfx, modelPath, modelScale));

			/**************************************************/

			/* ������ ������� */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");
			float pos_z = obj.at("pos-z");

			DirectX::XMFLOAT3 position = { pos_x, pos_y, pos_z };

			modelsPos.emplace_back(position);

			/******************/

			/* ������ ���������� */

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
		//oss << "[������]: " << "��������� ������ [" << md.modelsName[i] << "]\n";

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

void ModelData::AddModel(Graphics& gfx, Rgph::RenderGraph& rg, const char* path, const char* name)
{
	modelsName.push_back(name);
	models.emplace_back(std::make_unique<Model>(gfx, path, 1.0f));
	
	DirectX::XMFLOAT3 pos = {0.0f, 0.0f, 0.0f};
	modelsPos.emplace_back(pos);

	DirectX::XMFLOAT3 ori = { 0.0f, 0.0f, 0.0f };
	modelsOrien.emplace_back(ori);

	models.back()->LinkTechniques(rg);

	using std::to_string;

	// �������� ����� � ����������
	std::ifstream dataFile(this->path);
	if (!dataFile.is_open())
	{
		throw ("�� ������ ������� ���� � ������� � ������� ����");
	}

	// ������ �����
	json j;
	dataFile >> j;

	// �������� �����
	dataFile.close();

	// ����� �������
	std::ostringstream newModel;
	newModel << "\"" << name << "\":[{";

	newModel << "\"name\": \""  << name << "\",";
	newModel << "\"path\": \""  << path << "\",";
	newModel << "\"scale\": " << 1.0f << ",";

	newModel << "\"pos-x\": "  << 0.0f << ",";
	newModel << "\"pos-y\" : " << 0.0f << ",";
	newModel << "\"pos-z\" : " << 0.0f << ",";

	newModel << "\"roll\": "  << 0.0f << ",";
	newModel << "\"pitch\": " << 0.0f << ",";
	newModel << "\"yaw\": "   << 0.0f << "}]";

	// ���������� � ������� � ����
	std::string json_str = j.dump();
	size_t pos_of_par = json_str.find_last_of('}');
	size_t pos_of_par2 = json_str.find_last_of(']');

	json_str.at(pos_of_par) = ' ';
	json_str.at(pos_of_par2 + 1) = ',';

	// ������ � ���� ������ ��������
	std::ofstream ostr(this->path);
	ostr << json_str + newModel.str() + '}';

	// �������� �����
	ostr.close();
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

void ModelData::ShowModelsInformation(Graphics& gfx, Rgph::RenderGraph& rg)
{
	if (ImGui::Begin("�������", NULL,
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

		if (ImGui::Button("��������"))
		{
			ImGui::OpenPopup("���������� ������");
		}

		if (ImGui::BeginPopupModal("���������� ������", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("������� ���� � ������� � ��� ���");
			ImGui::Separator();

			ImGui::InputText("����", newPath, sizeof(newPath));
			ImGui::InputText("���", newName, sizeof(newName));

			if (ImGui::Button("��������"))
			{
				AddModel(gfx, rg, newPath, newName);
				ImGui::CloseCurrentPopup();
			}
		}
	}

	ImGui::End();
}

void ModelData::ShowModelsProperties()
{
	if (ImGui::Begin("�����", NULL,
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

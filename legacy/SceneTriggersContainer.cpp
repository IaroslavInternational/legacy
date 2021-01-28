#include "SceneTriggersContainer.h"

#include "imgui\imgui.h"

SceneTriggersContainer::SceneTriggersContainer(std::vector<const char*>& scenesNames,
											   std::vector<Trigger>& triggers)
{
	for (int i = 0; i < scenesNames.size(); i++)
	{
		trig_sc_container.insert(std::make_pair(scenesNames.at(i), triggers.at(i)));
	}
}

SceneTriggersContainer::~SceneTriggersContainer()
{}

std::pair<const char*, bool> SceneTriggersContainer::CheckTriggers(dx::XMFLOAT3 pos)
{
	for (auto it = trig_sc_container.begin(); it != trig_sc_container.end(); it++)
	{
		if (it->second.Check(pos))
		{
			const char* HittedTriggerGoal;
			HittedTriggerGoal = it->first;

			return std::make_pair(HittedTriggerGoal, true);
		}
		else
		{
			return std::make_pair("", false);
		}
	}
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
				auto pos = value.GetPosition();

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

#include "SceneTriggersContainer.h"

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

std::multimap<const char*, Trigger>* SceneTriggersContainer::GetInfo()
{
	return &trig_sc_container;
}

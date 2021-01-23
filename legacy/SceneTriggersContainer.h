#pragma once

#include "Trigger.h"

#include <map>
#include <vector>

// Триггер для перехода на следующую сцену
class SceneTriggersContainer
{
public:
	SceneTriggersContainer(std::vector<const char*>& scenesNames, std::vector<Trigger> &triggers);
	~SceneTriggersContainer();
	
	std::pair<const char*, bool> CheckTriggers(dx::XMFLOAT3 pos);

	std::multimap<const char*, Trigger>* GetInfo();
private:
	std::multimap<const char*, Trigger> trig_sc_container;
};
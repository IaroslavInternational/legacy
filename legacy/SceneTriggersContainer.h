#pragma once

#include "Trigger.h"

#include <map>
#include <vector>

// Триггер для перехода на следующую сцену
class SceneTriggersContainer
{
public:
	SceneTriggersContainer(const char* path);
	~SceneTriggersContainer();
	
	// Имя цели, статус | Проверка на пересечение триггера
	std::pair<const char*, bool> CheckTriggers(dx::XMFLOAT3 pos);

	// Показать триггеры *Левая панель*
	void ShowTrigInformation();

	// Показать триггеры *Правая панель*
	void ShowTrigSettings();

private:
	const char* selected = "";

	std::vector<std::string> ptr2scs;
	std::vector<TriggerStruct> trss;

	std::map<const char*, Trigger> trig_sc_container;
};
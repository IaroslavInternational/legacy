#pragma once

#include "Trigger.h"

#include <map>
#include <vector>

// Триггер для перехода на следующую сцену
class SceneTriggersContainer
{
public:
	SceneTriggersContainer(const char* path, Graphics& gfx);
	~SceneTriggersContainer();
	
	// Имя цели, статус | Проверка на пересечение триггера
	std::pair<const char*, bool> CheckTriggers(dx::XMFLOAT3 pos);

	// Показать триггеры *Левая панель*
	void ShowTrigInformation();

	// Показать триггеры *Правая панель*
	void ShowTrigSettings();

	// Данные триггеров
	std::map<const char*, std::unique_ptr<Trigger>>* GetData();
private:
	const char* filePath;

	const char* selected = "";

	std::vector<std::string> ptr2scs;
	std::vector<TriggerStruct> trss;

	std::map<const char*, std::unique_ptr<Trigger>> trig_sc_container;

	/* Данные для нового триггера */
	
	char name[128];
	char goal[128];

	float pos_lt[3] = { 0.0f, 0.0f, 0.0f };
	float pos_rt[3] = { 0.0f, 0.0f, 0.0f };
	float pos_lb[3] = { 0.0f, 0.0f, 0.0f };
	float pos_rb[3] = { 0.0f, 0.0f, 0.0f };

	float orient[3] = { 0.0f, 0.0f, 0.0f };
	
	/******************************/
private:
	void LoadTrigger(std::string name, std::string ptr, TriggerStruct& trs);
};
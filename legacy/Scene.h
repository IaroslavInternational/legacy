#pragma once

#include "Window.h"

#include "ImguiManager.h"
#include "AppLog.h"

#include "CameraContainer.h"
#include "PointLight.h"
#include "Model.h"
#include "BlurOutlineRenderGraph.h"
#include "ModelData.h"

#include "EngineTimer.h"

#include "SceneTriggersContainer.h"

namespace dx = DirectX;

class Scene
{
public:
	Scene() = default;
	Scene(const char* SceneName, const char* SceneID, std::shared_ptr<Window> _wnd, 
		  const char* PathToModelData, const char* PathToTriggerData);
	~Scene();

	/***** /Методы сцены\ *****/

	// Отрисовка сцены
	void Render(float dt);

	// Обработка входных данных
	void ProcessInput(float dt);

	std::pair<const char*, bool> IsOnTheSceneTrigger();

	// Удаление содердимого 
	void ClearAll();
	
	// Добавление 3Д-модели
	void LoadModel(const char* path, float scale);

	// Геттер имени сцены
	const char* GetName() const;

	/***** \Методы сцены/ *****/

	// Демо-интерфейс 
	void ShowImguiDemoWindow();
private:
	/* -- Интерфейс -- */

	/**** Панели ****/

	// Верхня панель меню
	void ShowMenu();

	// Левая панель
	void ShowLeftSide();

	// Правая панель
	void ShowRightSide();

	// Левая нижняя панель
	void ShowLeftBottomSide();

	// Нижняя панель
	void ShowBottomPanel();

	/****************/

	// Отслеживание пересечения триггера
	void ShowTrigCheck();

	// Показать FPS и графические адаптеры *Левая нижняя панель*
	void ShowFPSAndGPU();

	// Показать лог *Нижняя панель*
	void ShowLog();

	// Отключить боковые панели
	void DisableSides();

	/*******************/
private:
	// Идентификаторы сцены
	const char* sceneName;
	const char* ID;

	// Настройки
	bool savingDepth = false;
	bool showDemoWindow = true;
	bool onTrigger = false; const char* triggerGoal = nullptr;
	bool cursorState = false;
	bool CatchingTriggers = true;

	/* Интерфейс */

	// Левая панель моделей на сцене
	bool ShowModelsList = false;

	// Правая панель усправления настройками моделей
	bool ShowModelsSettings = false;

	// FPS и информация о графическом адаптере
	bool ShowHardwareInfo = true;

	// Лог
	bool ShowLogs = true;

	// Левая панель триггеров на сцене
	bool ShowTriggersList = true;

	// Правая панель триггеров на сцене
	bool ShowTriggersSettings = true;

	/*************/
	
private:
	// Лог панель
	AppLog log;

	// Контейнер камер
	CameraContainer cameras;

	// Указатель на главное окно 
	std::shared_ptr<Window> wnd;

	//
	Rgph::BlurOutlineRenderGraph rg{ wnd->Gfx() };

	/***** -Модели и объекты- *****/

	// Истоник света
	PointLight light;

	// Модели сцены
	ModelData md;

	SceneTriggersContainer strc;

	std::map<const char*, std::unique_ptr<Trigger>>* TrigData;
	/******************************/
};
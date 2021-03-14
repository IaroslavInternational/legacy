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
#include "PointLightContainer.h"

#include "SceneDataReader.h"

namespace dx = DirectX;

class Scene
{
public:
	Scene() = default;
	Scene(const char* SceneName,	   std::shared_ptr<Window> _wnd, 
		  const char* Data);
	~Scene();

	/***** /Методы сцены\ *****/

	// Отрисовка сцены
	void Render(float dt);

	// Обработка входных данных
	void ProcessInput(float dt);

	std::pair<const char*, bool> IsOnTheSceneTrigger();

	void ResetPos();

	// Удаление содержимого 
	void ClearAll();

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

	// Отключить все панели
	void DisableAll();

	// Цветовая гамма ImGui
	void SetGuiColors();

	/*******************/
private:
	// Идентификаторы сцены
	const char* sceneName;

	// Настройки
	bool savingDepth = false;
	bool showDemoWindow = true;
	bool onTrigger = false; const char* triggerGoal = nullptr;
	bool cursorState = false;
	bool CatchingTriggers = true;

	/* Интерфейс */

	// Левая панель моделей на сцене
	bool ShowModelsList = true;

	// Правая панель управления настройками моделей
	bool ShowModelsSettings = true;

	// FPS и информация о графическом адаптере
	bool ShowHardwareInfo = true;

	// Лог
	bool ShowLogs = true;

	// Левая панель триггеров на сцене
	bool ShowTriggersList = false;

	// Правая панель триггеров на сцене
	bool ShowTriggersSettings = false;

	// Левая панель точечных источников света на сцене
	bool ShowPLightsList = false;

	// Правая панель управления настройками точечных источников света
	bool ShowPLightsSettings = false;

	// Левая панель камер на сцене с настройками
	bool ShowCamsList = false;

	/*************/
private:
	// Лог панель
	AppLog log;

	// Контейнер камер
	CameraContainer cameras;

	// Указатель на главное окно 
	std::shared_ptr<Window> wnd;

	// Рендер граф
	Rgph::BlurOutlineRenderGraph rg{ wnd->Gfx() };

	/***** -Модели и объекты- *****/

	SceneDataReader sdr;

	// Истоники света
	PointLightContainer plc;

	// Модели сцены
	ModelData md;

	// Триггеры сцены
	SceneTriggersContainer strc;

	/******************************/
};
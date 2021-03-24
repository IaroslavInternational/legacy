#pragma once

#include "Window.h"
#include "ImguiManager.h"

#include "BlurOutlineRenderGraph.h"
#include "EngineTimer.h"

#include "SceneObjects.h"

namespace dx = DirectX;

class Scene
{
public:
	Scene() = default;
	Scene(const char* SceneName,	   std::shared_ptr<Window> _wnd, 
		  const char* Data);
	~Scene();

	/***** /Методы сцены\ *****/

	// Обработка входных данных
	void ProcessInput(float dt);

	// Отрисовка сцены
	void Render(float dt);

	std::pair<const char*, bool> IsOnTheSceneTrigger();

	// Возращение камеры на исходное положение
	void ResetPos();

	// Геттер имени сцены
	const char* GetName() const;

	/***** \Методы сцены/ *****/

	// Демо-интерфейс 
	void ShowImguiDemoWindow();
private:
#if IS_ENGINE_MODE

	/* -- Интерфейс -- */

	/**** Панели ****/

	// Установка положения и размеров панели
	void SetPanelWidthAndPos(int corner, float width, float height);

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

#endif // IS_ENGINE_MODE

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
	// Указатель на главное окно 
	std::shared_ptr<Window> wnd;

	// Рендер граф
	Rgph::BlurOutlineRenderGraph rg{ wnd->Gfx() };

	/***** -Объекты- *****/

	SceneObjects objects;

	/******************************/
};
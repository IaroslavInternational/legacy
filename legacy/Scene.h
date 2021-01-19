#pragma once

#include "Window.h"
#include "EngineTimer.h"
#include "ImguiManager.h"
#include "CameraContainer.h"
#include "PointLight.h"
#include "TestCube.h"
#include "Model.h"
#include "ScriptCommander.h"
#include "BlurOutlineRenderGraph.h"

class Scene
{
public:
	Scene(std::string SceneName, std::string SceneID, std::shared_ptr<Window> _wnd);
	~Scene();

	void Render();
	void ShowGUI(Graphics& Gfx, const char* name);
	void ProcessInput(float dt);
	std::string GetName();

	void ShowImguiDemoWindow();
private:
	// Идентификаторы сцены
	std::string sceneName;
	std::string ID;

	// Указатель на следующую сцену
	Scene* nextScenePtr;
private:

	CameraContainer cameras;
	std::shared_ptr<Window> wnd;
	Rgph::BlurOutlineRenderGraph rg{ wnd->Gfx() };

	PointLight light;
	Model sponza{ wnd->Gfx(), "Models\\sponza\\sponza.obj", 1.0f / 20.0f };

	bool savingDepth = false;
	bool showDemoWindow = false;
	bool onTrigger = false;
	bool cursorState = false;
};
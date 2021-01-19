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
#include "EngineMath.h"

#include "Scene.h"

class App
{
public:
	App( const std::string& commandLine = "" );
	// master frame / message loop
	int Go();
	~App();
private:
	void DoFrame( float dt );
	void HandleInput( float dt );
private:
	std::string commandLine;
	ImguiManager imgui;
	std::shared_ptr<Window> wnd;
	ScriptCommander scriptCommander;
	EngineTimer timer;
	float speed_factor = 1.0f;

	Scene EnterScene;
	//Scene SecondScene;
};
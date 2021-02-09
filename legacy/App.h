#pragma once

#include "Window.h"
#include "EngineTimer.h"
#include "ImguiManager.h"
#include "ScriptCommander.h"

#include "Scene.h"
#include <map>

class App
{
public:
	App( const std::string& commandLine = "" );
	~App();

	// master frame / message loop
	int Go();

private:
	void DoFrame( float dt );
	void HandleInput( float dt );
private:
	std::string commandLine;
	ImguiManager imgui;

	std::shared_ptr<Window> wnd;
	ScriptCommander scriptCommander;
	
	EngineTimer timer;
	float speed_factor = 2.0f;

	std::map<std::unique_ptr<Scene>, bool> scenes;
};
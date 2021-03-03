#include "App.h"

#include "imgui/imgui.h"
#include "EngineUtil.h"
#include "Channels.h"

namespace dx = DirectX;

App::App(const std::string& commandLine)
	:
	commandLine(commandLine),
	wnd(std::make_shared<Window>("Legacy")),
	scriptCommander(TokenizeQuoted(commandLine))
{
	scenes.emplace(std::make_unique<Scene>("Scene 1", wnd, "Scenes\\Scene 1\\scene_1.json"), true);
	scenes.emplace(std::make_unique<Scene>("Scene 2", wnd, "Scenes\\Scene 1\\scene_1.json"), false);
	scenes.emplace(std::make_unique<Scene>("Scene 3", wnd, "Scenes\\Scene 1\\scene_1.json"), false);
}

void App::HandleInput( float dt )
{
	for (auto& s : scenes)
	{
		if (s.second)
		{
			s.first->ProcessInput(dt);
		}
	}
}

void App::DoFrame( float dt )
{
	for (auto& s : scenes)
	{
		if (s.second)
		{
			s.first->Render(dt);
		}
	}
}

App::~App()
{}

int App::Go()
{
	while( true )
	{
		// process all messages pending, but to not block for new messages
		if( const auto ecode = Window::ProcessMessages() )
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		// execute the game logic
		const auto dt = timer.Mark() * speed_factor;
		HandleInput( dt );
		DoFrame( dt );
	}
}
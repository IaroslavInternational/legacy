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
	scenes.emplace(std::make_unique<Scene>("—цена 1", "1", wnd), true);
	scenes.emplace(std::make_unique<Scene>("—цена 2", "2", wnd), false);
	scenes.emplace(std::make_unique<Scene>("—цена 3", "3", wnd), false);
	//scenes.emplace(std::make_unique<Scene>("Scene 4", "4", wnd), false);
	//scenes.emplace(std::make_unique<Scene>("Scene 5", "5", wnd), false);
	//scenes.emplace(std::make_unique<Scene>("Scene 6", "6", wnd), false);
	//scenes.emplace(std::make_unique<Scene>("Scene 7", "7", wnd), false);
	//scenes.emplace(std::make_unique<Scene>("Scene 8", "8", wnd), false);
	//scenes.emplace(std::make_unique<Scene>("Scene 9", "9", wnd), false);
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
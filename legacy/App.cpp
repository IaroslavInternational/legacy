#include "App.h"
#include <algorithm>
#include "EngineMath.h"
#include "imgui/imgui.h"
#include "EngineUtil.h"
#include "Testing.h"
#include "PerfLog.h"
#include "TestModelProbe.h"
#include "Testing.h"
#include "Camera.h"
#include "Channels.h"

namespace dx = DirectX;

App::App(const std::string& commandLine)
	:
	commandLine(commandLine),
	wnd(std::make_shared <Window>(1280, 720, "Legacy")),
	scriptCommander(TokenizeQuoted(commandLine)),
	EnterScene("Scene 1", "1", wnd)
{}

void App::HandleInput( float dt )
{
	EnterScene.ProcessInput(dt);
}

void App::DoFrame( float dt )
{
	EnterScene.Render();
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
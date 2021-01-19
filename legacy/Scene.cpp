#include "Scene.h"
#include "EngineMath.h"
#include "EngineUtil.h"
#include "imgui/imgui.h"
#include "Channels.h"
#include "TestModelProbe.h"
#include "PerfLog.h"
#include "Camera.h"

#include <algorithm>

namespace dx = DirectX;

Scene::Scene(std::string SceneName, std::string SceneID, std::shared_ptr<Window> _wnd)
	:
	wnd(_wnd),
	light(wnd->Gfx(), { 10.0f, 5.0f, 0.0f })
{
	cameras.AddCamera(std::make_unique<Camera>(wnd->Gfx(), "A", dx::XMFLOAT3{ -13.5f,6.0f,3.5f }, 0.0f, PI / 2.0f));
	cameras.AddCamera(std::make_unique<Camera>(wnd->Gfx(), "B", dx::XMFLOAT3{ -13.5f,28.8f,-6.4f }, PI / 180.0f * 13.0f, PI / 180.0f * 61.0f));
	cameras.AddCamera(light.ShareCamera());

	light.LinkTechniques(rg);
	sponza.LinkTechniques(rg);
	cameras.LinkTechniques(rg);

	rg.BindShadowCamera(*light.ShareCamera());

	sceneName = SceneName;
	ID = SceneID;
}

Scene::~Scene()
{}

void Scene::Render()
{
	wnd->Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	light.Bind(wnd->Gfx(), cameras->GetMatrix());
	rg.BindMainCamera(cameras.GetActiveCamera());

	light.Submit(Chan::main);
	sponza.Submit(Chan::main);
	cameras.Submit(Chan::main);

	sponza.Submit(Chan::shadow);

	rg.Execute(wnd->Gfx());

	if (savingDepth)
	{
		rg.DumpShadowMap(wnd->Gfx(), "shadow.png");
		savingDepth = false;
	}

	// imgui windows
	static MP sponzeProbe{ "Sponza" };
	
	sponzeProbe.SpawnWindow(sponza);
	cameras.SpawnWindow(wnd->Gfx());
	light.SpawnControlWindow();

	rg.RenderWindows(wnd->Gfx());

	ShowGUI(wnd->Gfx(), sceneName.c_str());

	// present
	wnd->Gfx().EndFrame();
	rg.Reset();
}

void Scene::ShowGUI(Graphics& Gfx, const char* name)
{
	if (ImGui::Begin(name))
	{
		ImGui::Text("Scene");

		if (onTrigger)
		{
			ImGui::Text("You are on the Trigger");
		}
		else
		{
			ImGui::Text("You are not on the Trigger");
		}
	}
	ImGui::End();
}

void Scene::ProcessInput(float dt)
{
	while (const auto e = wnd->kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd->CursorEnabled())
			{
				wnd->DisableCursor();
				wnd->mouse.EnableRaw();
			}
			else
			{
				wnd->EnableCursor();
				wnd->mouse.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWindow = true;
			break;
		case VK_RETURN:
			savingDepth = true;
			break;
		}
	}

	if (!wnd->CursorEnabled())
	{
		if (wnd->kbd.KeyIsPressed('W'))
		{
			cameras->Translate({ 0.0f,0.0f,dt });
		}
		if (wnd->kbd.KeyIsPressed('A'))
		{
			cameras->Translate({ -dt,0.0f,0.0f });
		}
		if (wnd->kbd.KeyIsPressed('S'))
		{
			cameras->Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd->kbd.KeyIsPressed('D'))
		{
			cameras->Translate({ dt,0.0f,0.0f });
		}
		if (wnd->kbd.KeyIsPressed('R'))
		{
			cameras->Translate({ 0.0f,dt,0.0f });
		}
		if (wnd->kbd.KeyIsPressed('F'))
		{
			cameras->Translate({ 0.0f,-dt,0.0f });
		}
	}

	while (const auto delta = wnd->mouse.ReadRawDelta())
	{
		if (!wnd->CursorEnabled())
		{
			cameras->Rotate((float)delta->x, (float)delta->y);
		}
	}

	dx::XMFLOAT3 trPos = { 20.0f, 5.0f, 20.0f };

	if (cameras.GetActiveCamera().GetPos().x >= trPos.x)
		if(cameras.GetActiveCamera().GetPos().y >= trPos.y)
			if (cameras.GetActiveCamera().GetPos().z >= trPos.z)
			{
				onTrigger = true;
			}
	else
	{
		onTrigger = false;
	}
}

std::string Scene::GetName()
{
	return sceneName;
}

void Scene::ShowImguiDemoWindow()
{
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
}
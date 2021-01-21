#include "Scene.h"

#include "EngineMath.h"
#include "EngineUtil.h"
#include "imgui/imgui.h"
#include "Channels.h"
#include "TestModelProbe.h"
#include "Camera.h"

/***** /Сцена\ *****/
Scene::Scene(const char* SceneName, const char* SceneID, std::shared_ptr<Window> _wnd)
	:
	wnd(_wnd),
	light(wnd->Gfx(), { 10.0f, 5.0f, 0.0f }),
	scNames({ "Сцена 2", "Сцена 3", "Сцена 9" }),
	tr1(trs1),
	tr2(trs2),
	tr3(trs3),
	scTriggers({ tr1, tr2, tr3 }),
	strc(scNames, scTriggers),
	plane(wnd->Gfx(), 19.5f, 12.5f, { 200.0f, 100.0f, 10.0f, 0.7f })
{
	cameras.AddCamera(std::make_unique<Camera>(wnd->Gfx(), "A", dx::XMFLOAT3{ -13.5f,6.0f,3.5f }, 0.0f, PI / 2.0f));
	cameras.AddCamera(std::make_unique<Camera>(wnd->Gfx(), "B", dx::XMFLOAT3{ -13.5f,28.8f,-6.4f }, PI / 180.0f * 13.0f, PI / 180.0f * 61.0f));
	cameras.AddCamera(light.ShareCamera());

	plane.SetPos({ 24.4f, 12.5f, 32.0f });
	plane.SetRotation(0.0f, PI / 2.0f, 0.0f);

	light.LinkTechniques(rg);
	sponza.LinkTechniques(rg);
	cameras.LinkTechniques(rg);
	plane.LinkTechniques(rg);

	rg.BindShadowCamera(*light.ShareCamera());

	sceneName = SceneName;
	ID = SceneID;
}

Scene::~Scene()
{}

void Scene::Render(float dt)
{
	wnd->Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	/*
	if (CatchingTriggers)
	{*/
	auto info = IsOnTheTrigger();

	if (info.second)
	{
		onTrigger = info.second;
		triggerGoal = info.first;

		CatchingTriggers = false;
	}
	else
	{
		onTrigger = false;
	}
	//}
	
	light.Bind(wnd->Gfx(), cameras->GetMatrix());
	rg.BindMainCamera(cameras.GetActiveCamera());

	light.Submit(Chan::main);
	sponza.Submit(Chan::main);
	cameras.Submit(Chan::main);
	plane.Submit(Chan::main);

	sponza.Submit(Chan::shadow);
	plane.Submit(Chan::shadow);

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
	plane.SpawnControlWindow(wnd->Gfx(), "Trigger 1");

	rg.RenderWindows(wnd->Gfx());

	ShowGUI(sceneName);
	ShowImguiDemoWindow();

	// present
	wnd->Gfx().EndFrame();
	rg.Reset();
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
}

std::pair<const char*, bool> Scene::IsOnTheTrigger()
{
	dx::XMFLOAT3 camPos =
	{
		cameras.GetActiveCamera().GetPos().x,
		cameras.GetActiveCamera().GetPos().y,
		cameras.GetActiveCamera().GetPos().z
	};

	return strc.CheckTriggers(camPos);
}

void Scene::ShowGUI(const char* name)
{
	if (ImGui::Begin(name))
	{
		ImGui::Text("Сцена");

		if (onTrigger)
		{
			ImGui::Text("Столкновение с триггером");
			ImGui::Text("Триггер:");
			ImGui::Text(triggerGoal);
		}
		else
		{
			onTrigger = false;
			ImGui::Text("Не на триггере");
		}

		ImGui::Separator();
		ImGui::Text("FPS");
		ImGui::Text("%.3f мс/кадр (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Separator();

		if (ImGui::Button("Clear"))
		{
			ClearAll();
		}
	}
	ImGui::End();
}

void Scene::ClearAll()
{
	/*sponza.~Model();
	light.~PointLight();*/
}

const char* Scene::GetName() const
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
/***** \Сцены/ *****/
#include "Scene.h"

#include "EngineMath.h"
#include "EngineUtil.h"
#include "imgui/imgui.h"
#include "Channels.h"
#include "TestModelProbe.h"
#include "Camera.h"
#include "AdapterData.h"

Scene::Scene(const char* SceneName, const char* SceneID, std::shared_ptr<Window> _wnd,
			 const char* PathToModelData)
	:
	wnd(_wnd),
	light(wnd->Gfx(), { 10.0f, 5.0f, 0.0f }),
	scNames({ "Сцена 2", "Сцена 3", "Сцена 9" }),
	tr1(trs1),
	tr2(trs2),
	tr3(trs3),
	scTriggers({ tr1, tr2, tr3 }),
	strc(scNames, scTriggers),
	md(PathToModelData, wnd->Gfx()),
	plane(wnd->Gfx(), 16.6f, 12.5f, { 200.0f, 100.0f, 10.0f, 0.7f })
{
	cameras.AddCamera(std::make_unique<Camera>(wnd->Gfx(), "A", dx::XMFLOAT3{ -13.5f,6.0f,3.5f }, 0.0f, PI / 2.0f));
	cameras.AddCamera(std::make_unique<Camera>(wnd->Gfx(), "B", dx::XMFLOAT3{ -13.5f,28.8f,-6.4f }, PI / 180.0f * 13.0f, PI / 180.0f * 61.0f));
	cameras.AddCamera(light.ShareCamera());

	plane.SetPos({ 24.4f, 12.5f, 32.0f });
	plane.SetRotation(0.0f, PI / 2.0f, 0.0f);

	light.LinkTechniques(rg);
	cameras.LinkTechniques(rg);
	plane.LinkTechniques(rg);

	for (auto& m : md.models)
	{
		m->LinkTechniques(rg);
	}

	for (int i = 0; i < md.models.size(); i++)
	{
		md.models[i]->LinkTechniques(rg);
		md.models[i]->SetRootTransform
		(
			dx::XMMatrixRotationX(md.modelsOrien[i].x) *
			dx::XMMatrixRotationY(md.modelsOrien[i].y) *
			dx::XMMatrixRotationZ(md.modelsOrien[i].z) *
			dx::XMMatrixTranslation(md.modelsPos[i].x, md.modelsPos[i].y, md.modelsPos[i].z)
		);
	}

	rg.BindShadowCamera(*light.ShareCamera());

	sceneName = SceneName;
	ID = SceneID;

	/*Стиль интерфейса*/

	ImGui::GetStyle().FrameRounding = 4.0f;
	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::GetStyle().WindowRounding = 0.0f;

	/******************/
}

Scene::~Scene()
{}

void Scene::Render(float dt)
{
	wnd->Gfx().BeginFrame(0.07f, 0.0f, 0.12f);

	auto info = IsOnTheSceneTrigger();

	if (info.second)
	{
		onTrigger = info.second;
		triggerGoal = info.first;
	}
	else
	{
		onTrigger = false;
	}
	
	light.Bind(wnd->Gfx(), cameras->GetMatrix());
	rg.BindMainCamera(cameras.GetActiveCamera());

	light.Submit(Chan::main);
	cameras.Submit(Chan::main);
	plane.Submit(Chan::main);

	for (auto& m : md.models)
	{
		m->Submit(Chan::main);
		m->Submit(Chan::shadow);
	}

	plane.Submit(Chan::shadow);

	rg.Execute(wnd->Gfx());

	if (savingDepth)
	{
		rg.DumpShadowMap(wnd->Gfx(), "shadow.png");
		savingDepth = false;
	}

	// imgui windows
	//cameras.SpawnWindow(wnd->Gfx());
	//light.SpawnControlWindow();
	//plane.SpawnControlWindow(wnd->Gfx(), "Триггер 1");

	//rg.RenderWindows(wnd->Gfx());

	ShowFPS();
	ShowMenu();
	ShowLeftSide();
	ShowRightSide();

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

std::pair<const char*, bool> Scene::IsOnTheSceneTrigger()
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

		if (ImGui::Button("Clear"))
		{
			ClearAll();
		}
	}
	ImGui::End();
}

void Scene::ShowFPS()
{
	const float DISTANCE = 0.0f;
	static int corner = 2;

	ImGuiIO& io = ImGui::GetIO();

	ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y + ImGui::GetMenuHeight() : ImGui::GetMenuHeight());
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSize({io.DisplaySize.x * 0.2f, io.DisplaySize.y * 0.25f});
	
	auto GPU_Data = AdapterReader::GetAdapterData();

	if (ImGui::Begin("FPS", &ShowHardwareInfo, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Separator();
		ImGui::Text("FPS:");
		ImGui::Text("%.3f мс/кадр (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


		ImGui::Text("Графическое оборудование:");
		for (auto& d : GPU_Data)
		{
			char name_gpu[256];
			sprintf_s(name_gpu, "%ws", d.desc.Description);

			if (ImGui::TreeNode(name_gpu))
			{
				ImGui::TextColored({ 244.0f, 172.0f, 13.0f, 1.0f},"Память: ~%.1f ГБ", round(static_cast<double>(d.desc.DedicatedVideoMemory) / 1073741824));
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}

void Scene::ShowTriggersInfo()
{
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Триггеры", (bool*)(true), ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close")) 
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// left
		static const char* selected = "";
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);

		auto dataTrig = strc.GetInfo();

		for (auto it = dataTrig->begin(); it != dataTrig->end(); ++it)
		{
			char label[128];
			sprintf_s(label, it->first, selected);
			if (ImGui::Selectable(label, selected == it->first))
				selected = it->first;
		}
		ImGui::EndChild();
		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
		ImGui::Separator();
		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Информация"))
			{
				for (const auto& [key, value] : *dataTrig) {
					if (key == selected)
					{
						auto pos = value.GetPosition();

						ImGui::Text("Позиция");
						ImGui::Text("Левый верхний угол: \n x = %.3f; y = %.3f; z = %.3f;",
							pos[0].x, pos[0].y, pos[0].z);
						ImGui::Text("Правый верхний угол: \n x = %.3f; y = %.3f; z = %.3f;",
							pos[1].x, pos[1].y, pos[1].z);
						ImGui::Text("Левый нижний угол: \n x = %.3f; y = %.3f; z = %.3f;",
							pos[2].x, pos[2].y, pos[2].z);
						ImGui::Text("Правый нижний угол: \n x = %.3f; y = %.3f; z = %.3f;",
							pos[3].x, pos[3].y, pos[3].z);
						ImGui::Separator();

						ImGui::Text("Триггер указывает на: %s", selected);

						break;
					}
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndChild();
		if (ImGui::Button("Revert")) {}
		ImGui::SameLine();
		if (ImGui::Button("Save")) {}
		ImGui::EndGroup();
	}
	ImGui::End();
}

void Scene::ShowMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Файл"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Изменить"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Окна"))
		{
			if (ImGui::MenuItem("Модели"))
			{
				if (ShowModelsList && ShowModelsSettings)
				{
					ShowModelsList = false;
					ShowModelsSettings = false;
				}
				else
				{
					ShowModelsList = true;
					ShowModelsSettings = true;
				}
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Scene::ShowLeftSide()
{
	/* Левая сторона */

	const float DISTANCE = 0.0f;
	static int corner = 0;

	ImGuiIO& io = ImGui::GetIO();

	ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - ImGui::GetMenuHeight() : ImGui::GetMenuHeight());
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, 0, window_pos_pivot);
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.2f, io.DisplaySize.y * 0.75f), ImGuiCond_FirstUseEver);

	/* Содержимое */
	
	if (ShowModelsList)
	{
		md.ShowModelsInformation(&ShowModelsList);
	}

	/**************/

	ImGui::GetStyle().DisplayWindowPadding = { 0, 0 };
	ImGui::GetStyle().DisplaySafeAreaPadding = { 0, 0 };
	
	/* Конец левой стороны */
}

void Scene::ShowRightSide()
{
	/* Правая сторона */
	const float DISTANCE = 0.0f;
	static int corner = 1;

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - ImGui::GetMenuHeight() : ImGui::GetMenuHeight());
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, 0, window_pos_pivot);
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.2f, io.DisplaySize.y * 0.75f), ImGuiCond_FirstUseEver);

	/* Содержимое */

	if (ShowModelsSettings)
	{
		md.ShowModelsProperties(&ShowModelsSettings);
	}

	/**************/

	/* Конец правой стороны */
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
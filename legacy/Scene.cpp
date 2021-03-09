#include "Scene.h"

#include "EngineMath.h"
#include "EngineUtil.h"

#include "Channels.h"
#include "TestModelProbe.h"
#include "Camera.h"

#include "AdapterData.h"
#include "imgui/imgui.h"

#include <sstream>

Scene::Scene(const char* SceneName,		  std::shared_ptr<Window> _wnd, 
			 const char* Data)
	:
	wnd(_wnd),
	sdr(Data),
	plc(sdr.GetPaths().at(2).c_str(), wnd->Gfx()),
	md(sdr.GetPaths().at(0).c_str(), wnd->Gfx()),
	strc(sdr.GetPaths().at(1).c_str(), wnd->Gfx()),
	sceneName(SceneName)
{
	cameras.AddCamera(std::make_unique<Camera>(wnd->Gfx(), "A", dx::XMFLOAT3{ -13.5f,6.0f,3.5f }, 0.0f, PI / 2.0f));
	cameras.AddCamera(std::make_unique<Camera>(wnd->Gfx(), "B", dx::XMFLOAT3{ -13.5f,28.8f,-6.4f }, PI / 180.0f * 13.0f, PI / 180.0f * 61.0f));
	plc.AddCamerasToLight(cameras);
	
	plc.LinkTechniques(rg);
	cameras.LinkTechniques(rg);
	strc.LinkTechniques(rg);
	md.LinkTechniques(rg);

	plc.RgBindShadowCamera(rg);

	SetGuiColors();
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

		std::ostringstream oss;
		oss << "[Триггеры]: " << "Касание триггера [" << static_cast<std::string>(triggerGoal) << "]\n";

		log.AddLog(oss.str().c_str());
	}
	else
	{
		onTrigger = false;
	}

	plc.Bind(wnd->Gfx(), cameras->GetMatrix());

	rg.BindMainCamera(cameras.GetActiveCamera());

	plc.Submit(Chan::main);

	cameras.Submit(Chan::main);
	
	strc.Submit(Chan::main);
	strc.Submit(Chan::shadow);

	md.Submit(Chan::main);
	md.Submit(Chan::shadow);

	rg.Execute(wnd->Gfx());

	if (savingDepth)
	{
		rg.DumpShadowMap(wnd->Gfx(), "shadow.png");
		savingDepth = false;
	}

	ShowMenu();
	ShowLeftSide();
	ShowRightSide();
	ShowLeftBottomSide();
	ShowBottomPanel();

	// ShowImguiDemoWindow();
	//rg.RenderWindows(wnd->Gfx());
	//cameras.SpawnWindow(wnd->Gfx());

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

/***************** Интерфейс *****************/

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
			if (ImGui::MenuItem("Модели сцены"))
			{
				if (ShowModelsList && ShowModelsSettings)
				{
					DisableSides();

					ShowModelsList = false;
					ShowModelsSettings = false;
				}
				else
				{
					DisableSides();

					ShowModelsList = true;
					ShowModelsSettings = true;
				}
			}

			if (ImGui::MenuItem("Триггеры сцены"))
			{
				if (ShowTriggersList && ShowTriggersSettings)
				{
					DisableSides();

					ShowTriggersList = false;
					ShowTriggersSettings = false;
				}
				else
				{
					DisableSides();

					ShowTriggersList = true;
					ShowTriggersSettings = true;
				}
			}

			if (ImGui::MenuItem("Камеры сцены"))
			{
				if (ShowCamsList)
				{
					DisableSides();

					ShowCamsList = false;
				}
				else
				{
					DisableSides();

					ShowCamsList = true;
				}
			}

			if (ImGui::MenuItem("Точечные источники света сцены"))
			{
				if (ShowPLightsList && ShowPLightsSettings)
				{
					DisableSides();

					ShowPLightsList = false;
					ShowPLightsSettings = false;
				}
				else
				{
					DisableSides();

					ShowPLightsList = true;
					ShowPLightsSettings = true;
				}
			}

			if (ImGui::MenuItem("FPS & GPU"))
			{	
				ShowHardwareInfo ? ShowHardwareInfo = false : ShowHardwareInfo = true;
			}

			if (ImGui::MenuItem("Лог"))
			{
				ShowLogs ? ShowLogs = false : ShowLogs = true;
			}

			if (ImGui::MenuItem("Отключить панели"))
			{
				DisableAll();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Scene::ShowLeftSide()
{
	/* Левая сторона */

	ImGuiIO& io = ImGui::GetIO();
	int corner = 0;

	float MenuHeight = ImGui::GetMenuHeight();

	float LeftPanelW = io.DisplaySize.x * 0.2f;
	float LeftPanelH = io.DisplaySize.y * 0.75f;

	ImVec2 LeftPanelSize = ImVec2(
		LeftPanelW,
		LeftPanelH
	);

	ImVec2 LeftPanelPos = ImVec2(
		(corner & 1) ? io.DisplaySize.x : 0.0f,
		(corner & 2) ? io.DisplaySize.y - MenuHeight : MenuHeight
	);

	ImVec2 LeftPanelPivot = ImVec2(
		(corner & 1) ? 1.0f : 0.0f,
		(corner & 2) ? 1.0f : 0.0f
	);

	ImGui::SetNextWindowPos(LeftPanelPos, 0, LeftPanelPivot);
	ImGui::SetNextWindowSize(LeftPanelSize, ImGuiCond_FirstUseEver);

	/* Содержимое */
	
	if (ShowModelsList)
	{
		md.ShowModelsInformation(wnd->Gfx(), rg);
	}
	else if (ShowTriggersList)
	{
		strc.ShowTrigInformation();
	}
	else if (ShowPLightsList)
	{
		plc.ShowPLightsInformation();
	}
	else if (ShowCamsList)
	{
		cameras.ShowCamsInformationAndSettings(wnd->Gfx());
	}

	/**************/

	ImGui::GetStyle().DisplayWindowPadding = { 0.0f, 0.0f };
	ImGui::GetStyle().DisplaySafeAreaPadding = { 0.0f, 0.0f };
	
	/* Конец левой стороны */
}

void Scene::ShowRightSide()
{
	/* Правая сторона */

	ImGuiIO& io = ImGui::GetIO();
	int corner = 1;

	float MenuHeight = ImGui::GetMenuHeight();

	float RightPanelW = io.DisplaySize.x * 0.2f;
	float RightPanelH = io.DisplaySize.y * 0.75f;

	ImVec2 RightPanelSize = ImVec2(
		RightPanelW,
		RightPanelH
	);

	ImVec2 RightPanelPos = ImVec2(
		(corner & 1) ? io.DisplaySize.x : 0.0f,
		(corner & 2) ? io.DisplaySize.y - MenuHeight : MenuHeight
	);

	ImVec2 RightPanelPivot = ImVec2(
		(corner & 1) ? 1.0f : 0.0f,
		(corner & 2) ? 1.0f : 0.0f
	);

	ImGui::SetNextWindowPos(RightPanelPos, 0, RightPanelPivot);
	ImGui::SetNextWindowSize(RightPanelSize, ImGuiCond_FirstUseEver);
	
	/* Содержимое */

	if (ShowModelsSettings)
	{
		md.ShowModelsProperties();
	}
	else if (ShowTriggersSettings)
	{
		strc.ShowTrigSettings();

		ImGui::SetNextWindowPos({round(io.DisplaySize.x - RightPanelW), MenuHeight}, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.15f, io.DisplaySize.y * 0.15f}, ImGuiCond_FirstUseEver);
		ShowTrigCheck();
	}
	else if (ShowPLightsSettings)
	{
		plc.ShowPLightsProperties();

		ImGui::SetNextWindowPos({ round(io.DisplaySize.x - RightPanelW), MenuHeight }, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.15f, io.DisplaySize.y * 0.15f }, ImGuiCond_FirstUseEver);
	}

	/**************/

	/* Конец правой стороны */
}

void Scene::ShowLeftBottomSide()
{
	/* Левая нижняя сторона */

	ImGuiIO& io = ImGui::GetIO();
	int corner = 2;

	float MenuHeight = ImGui::GetMenuHeight();

	float LeftBottomPanelW = io.DisplaySize.x * 0.2f;
	float LeftBottomPanelH = io.DisplaySize.y * 0.25f;

	ImVec2 LeftBottomPanelSize = ImVec2(
		LeftBottomPanelW,
		LeftBottomPanelH
	);

	ImVec2 LeftBottomPanelPos = ImVec2(
		(corner & 1) ? io.DisplaySize.x : 0.0f,
		(corner & 2) ? io.DisplaySize.y + MenuHeight : MenuHeight
	);

	ImVec2 LeftBottomPanelPivot = ImVec2(
		(corner & 1) ? 1.0f : 0.0f,
		(corner & 2) ? 1.0f : 0.0f
	);

	ImGui::SetNextWindowPos(LeftBottomPanelPos, ImGuiCond_Always, LeftBottomPanelPivot);
	ImGui::SetNextWindowSize(LeftBottomPanelSize);

	/* Содержимое */

	if (ShowHardwareInfo)
	{
		ShowFPSAndGPU();
	}

	/**************/

	/* Конец левой нижней стороны */
}

void Scene::ShowBottomPanel()
{
	/* Нижняя стороны */

	ImGuiIO& io = ImGui::GetIO();
	int corner = 3;

	float MenuHeight = ImGui::GetMenuHeight();

	float BottomPanelW = round(io.DisplaySize.x * 0.8f);
	float BottomPanelH = io.DisplaySize.y * 0.25f;

	ImVec2 BottomPanelSize = ImVec2(
		BottomPanelW,
		BottomPanelH
	);

	ImVec2 BottomPanelPos = ImVec2(
		(corner & 1) ? io.DisplaySize.x : 0.0f,
		(corner & 2) ? io.DisplaySize.y + MenuHeight : MenuHeight
	);

	ImVec2 BottomPanelPivot = ImVec2(
		(corner & 1) ? 1.0f : 0.0f,
		(corner & 2) ? 1.0f : 0.0f
	);

	ImGui::SetNextWindowPos(BottomPanelPos, ImGuiCond_Always, BottomPanelPivot);
	ImGui::SetNextWindowSize(BottomPanelSize);

	/* Содержимое */

	if (ShowLogs)
	{
		ShowLog();
	}

	/**************/

	/* Конец нижней стороны */
}

void Scene::ShowFPSAndGPU()
{
	auto GPU_Data = AdapterReader::GetAdapterData();

	if (ImGui::Begin("Представление", NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		ImGui::Text("FPS:");
		ImGui::Text("%.3f мс/кадр (%.2f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Separator();

		ImGui::Text("Графическое оборудование:");
		for (auto& d : GPU_Data)
		{
			char name_gpu[256];
			sprintf_s(name_gpu, "%ws", d.desc.Description);

			if (ImGui::TreeNode(name_gpu))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(133.0f/255.0f, 219.0f/255.0f, 15.0f/255, 1.0f));
				ImGui::Text("Память: ~%.1f ГБ", round(static_cast<double>(d.desc.DedicatedVideoMemory) / 1073741824));
				ImGui::PopStyleColor();
				ImGui::TreePop();
			}
		}
	}

	ImGui::End();
}

void Scene::ShowLog()
{
	ImGui::Begin("Лог", NULL, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus);

	log.Draw("Лог", NULL);
}

void Scene::DisableSides()
{
	ShowModelsList = false;
	ShowModelsSettings = false;
	ShowTriggersList = false;
	ShowTriggersSettings = false;	
	ShowPLightsList = false;
	ShowPLightsSettings = false;
}

void Scene::DisableAll()
{
	DisableSides();
	
	ShowHardwareInfo = false;
	ShowLogs = false;
}

void Scene::SetGuiColors()
{
	/*Стиль интерфейса*/

	ImGui::GetStyle().FrameRounding = 4.0f;									// Закругление компонентов
	ImGui::GetStyle().WindowBorderSize = 0.0f;								// Размер границы
	ImGui::GetStyle().WindowRounding = 0.0f;								// Закругление окон

	// Цвета
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.36f, 0.39f, 1.00f);		// Главное меню
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.84f);			// Фон окна
	colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.00f, 0.20f, 0.73f);			// Меню окна
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.00f, 0.07f, 0.73f);	// Наведение на меню окна
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.50f, 0.38f, 0.54f);			// Компонента
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 0.18f, 0.15f, 0.40f);	// Наведение на компоненту
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.06f, 0.48f, 0.45f, 0.67f);	// Активные компонента
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);		// Галочка
	colors[ImGuiCol_SliderGrab] = ImVec4(0.37f, 0.70f, 0.00f, 1.00f);		// Ползунок слайдера
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.62f, 0.82f, 0.19f, 1.00f);	// Актвиный ползунок слайдера
	colors[ImGuiCol_Button] = ImVec4(0.56f, 0.05f, 0.05f, 0.59f);			// Кнопка
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.01f, 0.17f, 1.00f);	// Наведение на кнопку
	colors[ImGuiCol_ButtonActive] = ImVec4(0.03f, 0.55f, 0.48f, 1.00f);		// Активная кнопка
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.22f, 0.59f, 0.80f);	// Наведение на заголовк
	colors[ImGuiCol_Separator] = ImVec4(0.66f, 0.60f, 0.00f, 0.50f);		// Разделитель
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.08f, 0.27f, 0.86f);				// Раздел
	colors[ImGuiCol_TabHovered] = ImVec4(0.01f, 0.43f, 0.63f, 0.80f);		// Наведение на раздел
	colors[ImGuiCol_TabActive] = ImVec4(0.66f, 0.60f, 0.00f, 0.50f);		// Активный раздел

	/******************/
}

void Scene::ShowTrigCheck()
{
	if (ImGui::Begin("Попадание", NULL, 
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (onTrigger)
		{
			ImGui::Text("Отслежено cтолкновение");
			ImGui::Text("Цель триггера:");
			ImGui::Text(triggerGoal);
		}
		else
		{
			onTrigger = false;
			ImGui::Text("Не на триггере");
		}
	}

	ImGui::End();
}

/*********************************************/

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
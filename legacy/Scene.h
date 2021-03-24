#pragma once

#include "Window.h"
#include "ImguiManager.h"

#include "BlurOutlineRenderGraph.h"
#include "EngineTimer.h"

#include "SceneObjects.h"

namespace dx = DirectX;

class Scene
{
public:
	Scene() = default;
	Scene(const char* SceneName,	   std::shared_ptr<Window> _wnd, 
		  const char* Data);
	~Scene();

	/***** /������ �����\ *****/

	// ��������� ������� ������
	void ProcessInput(float dt);

	// ��������� �����
	void Render(float dt);

	std::pair<const char*, bool> IsOnTheSceneTrigger();

	// ���������� ������ �� �������� ���������
	void ResetPos();

	// ������ ����� �����
	const char* GetName() const;

	/***** \������ �����/ *****/

	// ����-��������� 
	void ShowImguiDemoWindow();
private:
#if IS_ENGINE_MODE

	/* -- ��������� -- */

	/**** ������ ****/

	// ��������� ��������� � �������� ������
	void SetPanelWidthAndPos(int corner, float width, float height);

	// ������ ������ ����
	void ShowMenu();

	// ����� ������
	void ShowLeftSide();

	// ������ ������
	void ShowRightSide();

	// ����� ������ ������
	void ShowLeftBottomSide();

	// ������ ������
	void ShowBottomPanel();	

	/****************/

	// ������������ ����������� ��������
	void ShowTrigCheck();

	// �������� FPS � ����������� �������� *����� ������ ������*
	void ShowFPSAndGPU();

	// �������� ��� *������ ������*
	void ShowLog();

	// ��������� ������� ������
	void DisableSides();

	// ��������� ��� ������
	void DisableAll();

	// �������� ����� ImGui
	void SetGuiColors();

	/*******************/

#endif // IS_ENGINE_MODE

private:
	// �������������� �����
	const char* sceneName;

	// ���������
	bool savingDepth = false;
	bool showDemoWindow = true;
	bool onTrigger = false; const char* triggerGoal = nullptr;
	bool cursorState = false;
	bool CatchingTriggers = true;

	/* ��������� */

	// ����� ������ ������� �� �����
	bool ShowModelsList = true;

	// ������ ������ ���������� ����������� �������
	bool ShowModelsSettings = true;

	// FPS � ���������� � ����������� ��������
	bool ShowHardwareInfo = true;

	// ���
	bool ShowLogs = true;

	// ����� ������ ��������� �� �����
	bool ShowTriggersList = false;

	// ������ ������ ��������� �� �����
	bool ShowTriggersSettings = false;

	// ����� ������ �������� ���������� ����� �� �����
	bool ShowPLightsList = false;

	// ������ ������ ���������� ����������� �������� ���������� �����
	bool ShowPLightsSettings = false;

	// ����� ������ ����� �� ����� � �����������
	bool ShowCamsList = false;

	/*************/
private:
	// ��������� �� ������� ���� 
	std::shared_ptr<Window> wnd;

	// ������ ����
	Rgph::BlurOutlineRenderGraph rg{ wnd->Gfx() };

	/***** -�������- *****/

	SceneObjects objects;

	/******************************/
};
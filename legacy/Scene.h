#pragma once

#include "Window.h"

#include "ImguiManager.h"
#include "AppLog.h"

#include "CameraContainer.h"
#include "PointLight.h"
#include "Model.h"
#include "BlurOutlineRenderGraph.h"
#include "ModelData.h"

#include "EngineTimer.h"

#include "SceneTriggersContainer.h"
#include "PointLightContainer.h"

#include "SceneDataReader.h"

namespace dx = DirectX;

class Scene
{
public:
	Scene() = default;
	Scene(const char* SceneName,	   std::shared_ptr<Window> _wnd, 
		  const char* Data);
	~Scene();

	/***** /������ �����\ *****/

	// ��������� �����
	void Render(float dt);

	// ��������� ������� ������
	void ProcessInput(float dt);

	std::pair<const char*, bool> IsOnTheSceneTrigger();

	void ResetPos();

	// �������� ����������� 
	void ClearAll();

	// ������ ����� �����
	const char* GetName() const;

	/***** \������ �����/ *****/

	// ����-��������� 
	void ShowImguiDemoWindow();
private:
	/* -- ��������� -- */

	/**** ������ ****/

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
	// ��� ������
	AppLog log;

	// ��������� �����
	CameraContainer cameras;

	// ��������� �� ������� ���� 
	std::shared_ptr<Window> wnd;

	// ������ ����
	Rgph::BlurOutlineRenderGraph rg{ wnd->Gfx() };

	/***** -������ � �������- *****/

	SceneDataReader sdr;

	// �������� �����
	PointLightContainer plc;

	// ������ �����
	ModelData md;

	// �������� �����
	SceneTriggersContainer strc;

	/******************************/
};
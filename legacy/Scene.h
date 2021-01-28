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

namespace dx = DirectX;

class Scene
{
public:
	Scene() = default;
	Scene(const char* SceneName, const char* SceneID, std::shared_ptr<Window> _wnd, 
		  const char* PathToModelData, const char* PathToTriggerData);
	~Scene();

	/***** /������ �����\ *****/

	// ��������� �����
	void Render(float dt);

	// ��������� ������� ������
	void ProcessInput(float dt);

	std::pair<const char*, bool> IsOnTheSceneTrigger();

	// �������� ����������� 
	void ClearAll();
	
	// ���������� 3�-������
	void LoadModel(const char* path, float scale);

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

	/*******************/
private:
	// �������������� �����
	const char* sceneName;
	const char* ID;

	// ���������
	bool savingDepth = false;
	bool showDemoWindow = true;
	bool onTrigger = false; const char* triggerGoal = nullptr;
	bool cursorState = false;
	bool CatchingTriggers = true;

	/* ��������� */

	// ����� ������ ������� �� �����
	bool ShowModelsList = false;

	// ������ ������ ����������� ����������� �������
	bool ShowModelsSettings = false;

	// FPS � ���������� � ����������� ��������
	bool ShowHardwareInfo = true;

	// ���
	bool ShowLogs = true;

	// ����� ������ ��������� �� �����
	bool ShowTriggersList = true;

	// ������ ������ ��������� �� �����
	bool ShowTriggersSettings = true;

	/*************/
	
private:
	// ��� ������
	AppLog log;

	// ��������� �����
	CameraContainer cameras;

	// ��������� �� ������� ���� 
	std::shared_ptr<Window> wnd;

	//
	Rgph::BlurOutlineRenderGraph rg{ wnd->Gfx() };

	/***** -������ � �������- *****/

	// ������� �����
	PointLight light;

	// ������ �����
	ModelData md;

	SceneTriggersContainer strc;

	std::map<const char*, std::unique_ptr<Trigger>>* TrigData;
	/******************************/
};
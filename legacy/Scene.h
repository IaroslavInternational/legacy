#pragma once

#include "Window.h"
#include "ImguiManager.h"
#include "CameraContainer.h"
#include "PointLight.h"
#include "Model.h"
#include "BlurOutlineRenderGraph.h"
#include "EngineTimer.h"
#include "SceneTriggersContainer.h"

namespace dx = DirectX;

class Scene
{
public:
	Scene() = default;
	Scene(const char* SceneName, const char* SceneID, std::shared_ptr<Window> _wnd);
	~Scene();

	/***** /������ �����\ *****/

	// ��������� �����
	void Render(float dt);

	// ��������� ������� ������
	void ProcessInput(float dt);

	std::pair<const char*, bool> IsOnTheSceneTrigger();

	/* ��������� */

	// ��������� ����������
	void ShowGUI(const char* name);

	void ShowFPS();

	/*************/

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
	// �������������� �����
	const char* sceneName;
	const char* ID;

	// ���������
	bool savingDepth = false;
	bool showDemoWindow = true;
	bool onTrigger = false; const char* triggerGoal = nullptr;
	bool cursorState = false;
	bool CatchingTriggers = true;

	std::vector<const char*> scNames;
	
	//dx::XMFLOAT3 tr1LT = 
	
	TriggerStruct trs1 = { {24.4f, 12.5f, 32.0f}, {24.4f, 12.5f, 15.4f}, 
						   {24.4f, 0.0f, 32.0f},  {24.4f, 0.0f, 15.4f} };
	TriggerStruct trs2 = { {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} };
	TriggerStruct trs3 = { {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} };

	Trigger tr1;
	Trigger tr2;
	Trigger tr3;

	std::vector<Trigger> scTriggers;
	
	SceneTriggersContainer strc;
	//std::multimap<const char*, Trigger> trig_sc_container;
private:
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
	Model sponza{ wnd->Gfx(), "Models\\sponza\\sponza.obj", 1.0f / 20.0f };
	Plate plane;
	/******************************/
};
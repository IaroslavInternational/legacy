#pragma once

#include "EngineConverter.h"
#include "Trigger.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#endif // IS_ENGINE_MODE

#include <map>
#include <vector>

// �������� ��� �������� �� ��������� �����
class TriggerContainer
{
public:
#if IS_ENGINE_MODE
	TriggerContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg, AppLog* aLog);
#else
	TriggerContainer(std::string path);
#endif // IS_ENGINE_MODE
	~TriggerContainer();
public:
#if IS_ENGINE_MODE
	void LinkTechniques();
	void Submit(size_t channels);
#endif // IS_ENGINE_MODE
public:
	std::pair<std::string, bool> Check(dx::XMFLOAT3 pos);		// ��� ����, ������ | �������� �� ����������� ��������
public:
#if IS_ENGINE_MODE
	void ShowLeftPanel();
	void ShowRightPanel();
#endif // IS_ENGINE_MODE
private:
	void AddTrigger(std::string name, std::string ptr);
	void DeleteTrigger(std::string name);									// ������� ������
private:
	std::string path;													// ���� � ����� � ������� � �������
	Graphics& gfx;														// ����� ������������ ����
	Rgph::RenderGraph& rg;												// ����� ������-�����
	std::multimap<std::string, std::unique_ptr<Trigger>> triggers;
private:
	std::string selected = "";

#if IS_ENGINE_MODE
	/* ������ ��� ������ �������� */

	mutable char name[128];
	mutable char goal[128];
	mutable float new_x = 0.0f;
	mutable float new_y = 0.0f;
	mutable float new_w = 0.0f;
	mutable float new_h = 0.0f;

	/******************************/
private:
	AppLog* applog;	// ���
#endif // IS_ENGINE_MODE
};
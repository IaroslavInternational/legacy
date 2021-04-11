#pragma once

#include "EngineConverter.h"

#include "Trigger.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#endif // IS_ENGINE_MODE

#include <map>
#include <vector>

// �������� ��� �������� �� ��������� �����
class SceneTriggersContainer
{
public:
#if IS_ENGINE_MODE
	SceneTriggersContainer(const char* path, Graphics& gfx, AppLog* aLog);
#else
	SceneTriggersContainer(const char* path);
#endif // IS_ENGINE_MODE
	~SceneTriggersContainer();
#if IS_ENGINE_MODE
	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit(size_t channels);
#endif // IS_ENGINE_MODE

	// ��� ����, ������ | �������� �� ����������� ��������
	std::pair<std::string, bool> CheckTriggers(dx::XMFLOAT3 pos);

#if IS_ENGINE_MODE
	// �������� �������� *����� ������*
	void ShowTrigInformation(Graphics& gfx, Rgph::RenderGraph& rg);

	// �������� �������� *������ ������*
	void ShowTrigSettings();
#endif // IS_ENGINE_MODE
private:
	const char* filePath;

	std::string selected = "";

	std::vector<std::string> ptr2scs;
	std::vector<std::string> names;
	std::vector<TriggerStruct> trss;

	std::map<std::string, std::unique_ptr<Trigger>> trig_sc_container;

#if IS_ENGINE_MODE
	/* ������ ��� ������ �������� */

	mutable char name[128];
	mutable char goal[128];

	mutable float pos_lt[3] = { 0.0f, 0.0f, 0.0f };
	mutable float pos_rt[3] = { 0.0f, 0.0f, 0.0f };
	mutable float pos_lb[3] = { 0.0f, 0.0f, 0.0f };
	mutable float pos_rb[3] = { 0.0f, 0.0f, 0.0f };

	mutable float orient[3] = { 0.0f, 0.0f, 0.0f };

	/******************************/

private:
	// �������� �������
	void LoadTrigger(Graphics& gfx, std::string name, std::string ptr, TriggerStruct& trs,
		Rgph::RenderGraph& rg);
private:

	// ���
	AppLog* applog;
#endif // IS_ENGINE_MODE
};
#pragma once

#include "Trigger.h"

#include <map>
#include <vector>

// ������� ��� �������� �� ��������� �����
class SceneTriggersContainer
{
public:
	SceneTriggersContainer(const char* path);
	~SceneTriggersContainer();
	
	// ��� ����, ������ | �������� �� ����������� ��������
	std::pair<const char*, bool> CheckTriggers(dx::XMFLOAT3 pos);

	// �������� �������� *����� ������*
	void ShowTrigInformation();

	// �������� �������� *������ ������*
	void ShowTrigSettings();

private:
	const char* selected = "";

	std::vector<std::string> ptr2scs;
	std::vector<TriggerStruct> trss;

	std::map<const char*, Trigger> trig_sc_container;
};
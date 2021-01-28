#pragma once

#include "Trigger.h"

#include <map>
#include <vector>

// ������� ��� �������� �� ��������� �����
class SceneTriggersContainer
{
public:
	SceneTriggersContainer(std::vector<const char*>& scenesNames, 
						   std::vector<Trigger>& triggers);
	~SceneTriggersContainer();
	
	// ��� ����, ������ | �������� �� ����������� ��������
	std::pair<const char*, bool> CheckTriggers(dx::XMFLOAT3 pos);

	// �������� �������� *����� ������*
	void ShowTrigInformation();

	// �������� �������� *������ ������*
	void ShowTrigSettings();

private:
	const char* selected = "";

	std::multimap<const char*, Trigger> trig_sc_container;
};
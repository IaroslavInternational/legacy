#pragma once

#include "Trigger.h"

#include <map>
#include <vector>

// ������� ��� �������� �� ��������� �����
class SceneTriggersContainer
{
public:
	SceneTriggersContainer(std::vector<const char*>& scenesNames, std::vector<Trigger> &triggers);
	~SceneTriggersContainer();
	
	std::pair<const char*, bool> CheckTriggers(dx::XMFLOAT3 pos);

	// �������� �� ������� ����� ����� ����������
	//bool Check(dx::XMFLOAT3 CameraPos);
private:
	std::multimap<const char*, Trigger> trig_sc_container;
};
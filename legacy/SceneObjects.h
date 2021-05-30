#pragma once

#include "ModelContainer.h"
#include "TriggerContainer.h"
#include "CameraContainer.h"
#include "PointLightContainer.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#include "NodeEditor.h"
#endif

#include "SceneDataReader.h"

class SceneObjects
{
public:
	SceneObjects(std::string pathToObjectsData, Graphics& gfx, Rgph::BlurOutlineRenderGraph& rg);
	~SceneObjects();
public:
	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit(size_t channels);

#if IS_ENGINE_MODE
	void DrawLog();
#endif // IS_ENGINE_MODE
private:
	// ����������� ����� � ������ � ������� ��� �����������
	SceneDataReader sdr;
	
#if IS_ENGINE_MODE
	// ���
	AppLog applog;
#endif // IS_ENGINE_MODE
public:
	/***** -�������- *****/
	
	// ��������� �����
	CameraContainer cameras;

	// �������� �����
	PointLightContainer pointLights;

	// ������ �����
	ModelContainer models;

	// �������� �����
	TriggerContainer triggers;
	
	/*********************/

#if IS_ENGINE_MODE
	NodeEditor nEditor;
#endif // IS_ENGINE_MODE
};
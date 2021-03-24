#pragma once

#include "ModelData.h"
#include "SceneTriggersContainer.h"
#include "CameraContainer.h"
#include "PointLightContainer.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#endif

#include "SceneDataReader.h"

class SceneObjects
{
public:
	SceneObjects(const char* pathToObjectsData, Graphics& gfx, Rgph::BlurOutlineRenderGraph& rg);
	~SceneObjects();
public:
	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit(size_t channels);

#if IS_ENGINE_MODE
	void DrawLog();
#endif
private:
	SceneDataReader sdr;
	
#if IS_ENGINE_MODE
	AppLog applog;
#endif
public:
	/***** -�������- *****/
	
	// ��������� �����
	CameraContainer cameras;

	// �������� �����
	PointLightContainer pointLights;

	// ������ �����
	ModelData models;

	// �������� �����
	SceneTriggersContainer triggersScene;

	/*********************/
};
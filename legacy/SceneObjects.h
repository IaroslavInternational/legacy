#pragma once

#include "ModelData.h"
#include "SceneTriggersContainer.h"
#include "CameraContainer.h"
#include "PointLightContainer.h"

#include "AppLog.h"

#include "SceneDataReader.h"

class SceneObjects
{
public:
	SceneObjects(const char* pathToObjectsData, Graphics& gfx, Rgph::BlurOutlineRenderGraph& rg);
	~SceneObjects();
public:
	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit(size_t channels);

	void DrawLog();
private:
	SceneDataReader sdr;
	
	AppLog applog;
public:
	/***** -Объекты- *****/
	
	// Контейнер камер
	CameraContainer cameras;

	// Истоники света
	PointLightContainer pointLights;

	// Модели сцены
	ModelData models;

	// Триггеры сцены
	SceneTriggersContainer triggersScene;

	/*********************/
};
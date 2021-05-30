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
	// Считыватель путей к файлам с данными для контейнеров
	SceneDataReader sdr;
	
#if IS_ENGINE_MODE
	// Лог
	AppLog applog;
#endif // IS_ENGINE_MODE
public:
	/***** -Объекты- *****/
	
	// Контейнер камер
	CameraContainer cameras;

	// Истоники света
	PointLightContainer pointLights;

	// Модели сцены
	ModelContainer models;

	// Триггеры сцены
	TriggerContainer triggers;
	
	/*********************/

#if IS_ENGINE_MODE
	NodeEditor nEditor;
#endif // IS_ENGINE_MODE
};
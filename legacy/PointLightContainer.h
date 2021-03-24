#pragma once

#include "EngineConverter.h"

#include "PointLight.h"
#include "BlurOutlineRenderGraph.h"
#include "CameraContainer.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#endif // IS_ENGINE_MODE

class PointLightContainer
{
public:
#if IS_ENGINE_MODE
	PointLightContainer(const char* path, Graphics& gfx, AppLog* aLog);
#else
	PointLightContainer(const char* path, Graphics& gfx);
#endif
	~PointLightContainer();

	void LinkTechniques(Rgph::RenderGraph& rg);
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view);
	void Submit(size_t channels);

	void RgBindShadowCamera(Rgph::BlurOutlineRenderGraph& rg);
	void AddCamerasToLight(CameraContainer* camcon);

#if IS_ENGINE_MODE
	// Показать источники освещения *Левая панель*
	void ShowPLightsInformation();

	// Показать источники освещения *Правая панель*
	void ShowPLightsProperties();
#endif // IS_ENGINE_MODE
private:
	std::vector<std::string> pLightsName;
	std::vector<DirectX::XMFLOAT3> pLightsPos;
	std::vector<std::unique_ptr<PointLight>> pLights;
private:
#if IS_ENGINE_MODE
	const char* selected = "";

	AppLog* applog;
#endif // IS_ENGINE_MODE
};
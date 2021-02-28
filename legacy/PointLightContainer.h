#pragma once

#include "PointLight.h"
#include "BlurOutlineRenderGraph.h"
#include "CameraContainer.h"

class PointLightContainer
{
public:
	PointLightContainer(const char* path, Graphics& gfx);
	~PointLightContainer();

	void LinkTechniques(Rgph::RenderGraph& rg);
	void Bind(Graphics& gfx, DirectX::XMMATRIX view);
	void Submit(size_t channels);

	void RgBindShadowCamera(Rgph::BlurOutlineRenderGraph& rg);
	void AddCamerasToLight(CameraContainer& camcon);

	// Показать источники освещения *Левая панель*
	void ShowPLightsInformation();

	// Показать источники освещения *Правая панель*
	void ShowPLightsProperties();
private:
	std::vector<std::string> pLightsName;
	std::vector<DirectX::XMFLOAT3> pLightsPos;
	std::vector<std::unique_ptr<PointLight>> pLights;
private:
	const char* selected = "";
};
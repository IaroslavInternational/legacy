#include "SceneObjects.h"
#include "EngineMath.h"

#include "Camera.h"

SceneObjects::SceneObjects(const char* pathToObjectsData, Graphics& gfx,
						   Rgph::BlurOutlineRenderGraph& rg)
	:
	sdr(pathToObjectsData),
#if IS_ENGINE_MODE
	cameras(sdr.GetCameraContainerPath().c_str(), gfx, &applog),
	models(sdr.GetModelContainerPath().c_str(), gfx, rg, &applog),
	pointLights(sdr.GetPointLightContainerPath().c_str(), gfx, &applog),
	triggersScene(sdr.GetTriggerContainerPath().c_str(), gfx, &applog),
	nEditor(cameras, models, &applog)
#else
	cameras(sdr.GetCameraContainerPath().c_str(), gfx),
	models(sdr.GetModelContainerPath().c_str(), gfx, rg),
	pointLights(sdr.GetPointLightContainerPath().c_str(), gfx),
	triggersScene(sdr.GetTriggerContainerPath().c_str().c_str())
#endif // IS_ENGINE_MODE
{
	pointLights.AddCamerasToLight(&cameras);	// Важно !	
	pointLights.RgBindShadowCamera(rg);
}

SceneObjects::~SceneObjects()
{
}

#if IS_ENGINE_MODE
void SceneObjects::LinkTechniques(Rgph::RenderGraph& rg)
{
	pointLights.LinkTechniques(rg);
	cameras.LinkTechniques(rg);
	triggersScene.LinkTechniques(rg);
	models.LinkTechniques();
}

void SceneObjects::Submit(size_t channels)
{
	pointLights.Submit(channels);
	cameras.Submit(channels);
	triggersScene.Submit(channels);
	models.Submit(channels);
}
#else

void SceneObjects::LinkTechniques(Rgph::RenderGraph& rg)
{
	pointLights.LinkTechniques(rg);
	models.LinkTechniques(rg);
}

void SceneObjects::Submit(size_t channels)
{
	pointLights.Submit(channels);
	models.Submit(channels);
}
#endif // IS_ENGINE_MODE

#if IS_ENGINE_MODE
void SceneObjects::DrawLog()
{
	ImGui::Begin("Лог", NULL, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus);

	applog.Draw("Лог", NULL);
	ImGui::End();
}
#endif
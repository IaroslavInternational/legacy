#include "SceneObjects.h"
#include "EngineMath.h"

#include "Camera.h"

SceneObjects::SceneObjects(std::string pathToObjectsData, Graphics& gfx,
						   Rgph::BlurOutlineRenderGraph& rg)
	:
	sdr(pathToObjectsData),
#if IS_ENGINE_MODE
	cameras(	sdr.GetCameraContainerPath(),	  gfx, rg, &applog),
	models(		sdr.GetModelContainerPath(),	  gfx, rg, &applog),
	pointLights(sdr.GetPointLightContainerPath(), gfx, rg, &applog),
	triggers(	sdr.GetTriggerContainerPath(),	  gfx, rg, &applog),
	nEditor(cameras, models, &applog)
#else
	cameras(	sdr.GetCameraContainerPath(),	  gfx),
	models(		sdr.GetModelContainerPath(),	  gfx, rg),
	pointLights(sdr.GetPointLightContainerPath(), gfx),
	triggers(	sdr.GetTriggerContainerPath()),
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
	pointLights.LinkTechniques();
	cameras.LinkTechniques();
	triggers.LinkTechniques();
	models.LinkTechniques();
}

void SceneObjects::Submit(size_t channels)
{
	pointLights.Submit(channels);
	cameras.Submit(channels);
	triggers.Submit(channels);
	models.Submit(channels);
}
#else
void SceneObjects::LinkTechniques(Rgph::RenderGraph& rg)
{
	models.LinkTechniques(rg);
}

void SceneObjects::Submit(size_t channels)
{
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
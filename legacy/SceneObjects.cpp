#include "SceneObjects.h"
#include "EngineMath.h"

#include "Camera.h"

SceneObjects::SceneObjects(const char* pathToObjectsData, Graphics& gfx,
						   Rgph::BlurOutlineRenderGraph& rg)
	:
	sdr(pathToObjectsData),
#if IS_ENGINE_MODE
	cameras(&applog),
	models(sdr.GetPaths().at(0).c_str(), gfx, &applog),
	pointLights(sdr.GetPaths().at(2).c_str(), gfx, &applog),
	triggersScene(sdr.GetPaths().at(1).c_str(), gfx, &applog),
	nEditor(cameras, models, &applog)
#else
	models(sdr.GetPaths().at(0).c_str(), gfx),
	pointLights(sdr.GetPaths().at(2).c_str(), gfx),
	triggersScene(sdr.GetPaths().at(1).c_str())
#endif // IS_ENGINE_MODE
{
	cameras.AddCamera(std::make_shared<Camera>(gfx, "main", dx::XMFLOAT3{ -13.5f,6.0f,3.5f }, 0.0f, PI / 2.0f));
	cameras.AddCamera(std::make_shared<Camera>(gfx, "to model", dx::XMFLOAT3{ -13.5f,28.8f,-6.4f }, PI / 180.0f * 13.0f, PI / 180.0f * 61.0f));
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
	models.LinkTechniques(rg);
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
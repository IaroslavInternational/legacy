#pragma once

#include "AppLog.h"

#include <vector>
#include <memory>

class Camera;
class Graphics;

namespace Rgph
{
	class RenderGraph;
}

class CameraContainer
{
public:
	Camera* operator->();
	
	CameraContainer(AppLog* aLog);
	~CameraContainer();
	
	void LinkTechniques( Rgph::RenderGraph& rg );
	void Submit( size_t channels ) const;
	void Bind(Graphics& gfx);
	void AddCamera(std::shared_ptr<Camera> pCam);

	Camera& GetActiveCamera();

	void ShowCamsInformationAndSettings(Graphics& gfx);
private:
	Camera& GetControlledCamera();
private:
	std::vector<std::shared_ptr<Camera>> cameras;
	
	int active = 0;
	int controlled = 0;

	AppLog* applog;
};
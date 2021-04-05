#pragma once
#include "EngineConverter.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#endif

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
	
#if IS_ENGINE_MODE
	CameraContainer(AppLog* aLog);
#endif
	~CameraContainer();
	
	void LinkTechniques( Rgph::RenderGraph& rg );
	void Submit( size_t channels ) const;
	void Bind(Graphics& gfx);
	void AddCamera(std::shared_ptr<Camera> pCam);

	Camera& GetActiveCamera();
	std::shared_ptr<Camera> GetPtr2ActiveCamera();

	void ShowCamsInformationAndSettings(Graphics& gfx);
private:
	Camera& GetControlledCamera();
private:
	std::vector<std::shared_ptr<Camera>> cameras;
	
	int active = 0;
	int controlled = 0;
#if IS_ENGINE_MODE
	AppLog* applog;
#endif
};
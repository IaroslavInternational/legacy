#pragma once
#include "EngineConverter.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#endif

#include <string>
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
#if IS_ENGINE_MODE
	CameraContainer(const char* path, Graphics& gfx, AppLog* aLog);
#else
	CameraContainer(const char* path, Graphics& gfx);
#endif // IS_ENGINE_MODE
	~CameraContainer();
	
public:
	Camera* operator->();
	
	void LinkTechniques( Rgph::RenderGraph& rg );
	void Submit( size_t channels ) const;
	void Bind(Graphics& gfx);

	void AddCamera(std::shared_ptr<Camera> pCam);

	Camera& GetActiveCamera();

	std::shared_ptr<Camera> GetPtr2ActiveCamera();

	std::shared_ptr<Camera> GetPtr2CameraByName(std::string name);

	void ShowCamsInformationAndSettings(Graphics& gfx);

#if IS_ENGINE_MODE
	size_t CamerasAmount();

	std::string GetCameraNameByIndex(size_t index);
#endif // IS_ENGINE_MODE
private:
	Camera& GetControlledCamera();
private:
	// Путь к файлу с данными о моделях
	const char* path;

	// Камеры
	std::vector<std::shared_ptr<Camera>> cameras;
	
	// Индекс активной камеры
	int active = 0;

	// Индекс управляемой камеры
	int controlled = 0;

#if IS_ENGINE_MODE
	// Лог
	AppLog* applog;
#endif
};
#pragma once

#include "EngineConverter.h"

#include <DirectXMath.h>
#include <string>
#include "Projection.h"
#include "CameraIndicator.h"

class Graphics;
namespace Rgph
{
	class RenderGraph;
}

class Camera
{
public:
	Camera(Graphics& gfx, std::string name, 
		   DirectX::XMFLOAT3 position =	   { 0.0f,0.0f,0.0f },
		   DirectX::XMFLOAT2 orientation = { 0.0f,0.0f },
		   ProjectionData prd =			   { 1.0f, 9.0f / 16.0f, 0.5f, 400.0f },
		   bool tethered = false) noexcept;
public:
#if IS_ENGINE_MODE
	void LinkTechniques(Rgph::RenderGraph& rg);				// Добавить к рендеру
	void Submit(size_t channel) const;						// Добавить к каналу отрисовки
#endif // IS_ENGINE_MODE
	void BindToGraphics(Graphics& gfx) const;				// Добавить камеру к цели рендера
public:
	DirectX::XMFLOAT3 GetPosition() const noexcept;			// Получить позицию
	DirectX::XMFLOAT2 GetOrientation() const noexcept;		// Получить ориентацию
	DirectX::XMMATRIX GetMatrix() const noexcept;			// Получить матрицу
	ProjectionData GetProjectionData() const noexcept;		// Получить данные проекции
	DirectX::XMMATRIX GetProjection() const noexcept;		// Получить проекцию
	void Reset(Graphics& gfx) noexcept;						// Сбросить
	void Rotate(float dx, float dy) noexcept;				// Вращать камеру
	void Translate(DirectX::XMFLOAT3 translation) noexcept;	// Переместить камеру
	void SetPos(const DirectX::XMFLOAT3& pos) noexcept;		// Установить позицию
	std::string GetName() const noexcept;					// Получить имя камеры
public:
#if IS_ENGINE_MODE
	void SpawnDefaultControl(Graphics& gfx) noexcept;		// Базовый интерфейс управления
#endif // IS_ENGINE_MODE
private:
	bool tethered;											//

	std::string name;										// Имя камеры

	DirectX::XMFLOAT3 homePosition;							// Исходная позиция
	DirectX::XMFLOAT2 homeOrientation;						// Исходная ориентация
	DirectX::XMFLOAT3 position;								// Позиция
	DirectX::XMFLOAT2 orientation;							// Ориентация

	float travelSpeed = 24.0f;								// Скорость перемещения
	float rotationSpeed = 0.004f;							// Скорость вращения
private:
	Projection proj;										// Проекция камеры

#if IS_ENGINE_MODE
	bool enableCameraIndicator = true;						// Разрешена ли отрисовка индикатора камеры
	bool enableFrustumIndicator = false;					// Разрешена ли отрисовка проекции камеры

	CameraIndicator indicator;								// Индикатор камеры
#endif // IS_ENGINE_MODE
};
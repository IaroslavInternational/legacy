#pragma once

#include "Graphics.h"

#if IS_ENGINE_MODE
#include "SolidSphere.h"
#endif // IS_ENGINE_MODE

#include "ConstantBuffers.h"
#include "ConditionalNoexcept.h"

#if IS_ENGINE_MODE
namespace Rgph
{
	class RenderGraph;
}
#endif // IS_ENGINE_MODE

class Camera;

struct PointLightCBuf												// Буфер данных ист. освещения для шейдера
{
	alignas(16) DirectX::XMFLOAT3 pos;
	alignas(16) DirectX::XMFLOAT3 ambient;
	alignas(16) DirectX::XMFLOAT3 diffuseColor;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
};

class PointLight
{
public:
	PointLight(Graphics& gfx, std::string name, PointLightCBuf data, float radius = 0.5f);
public:
#if IS_ENGINE_MODE
	void LinkTechniques(Rgph::RenderGraph& rg);							// Добавить к рендеру
	void Submit(size_t channels) const noxnd;							// Добавить к каналу отрисовки
#endif // IS_ENGINE_MODE
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;	// Обновить данные для шейдера
public:
	std::string GetName() const noexcept;								// Получить имя ист. света
	PointLightCBuf GetData() const noexcept;							// Получить данные ист. освещения
	void Reset() noexcept;												// Сбросить данные
	std::shared_ptr<Camera> ShareCamera() const noexcept;				// Получить укзатель на камеру
public:
#if IS_ENGINE_MODE
	void SpawnDefaultControl() noexcept;								// Базовый интерфейс управления
#endif // IS_ENGINE_MODE
private:
	PointLightCBuf home;												// Исходный буфер данных
	PointLightCBuf cbData;												// Изменяемый буфер данных
#if IS_ENGINE_MODE
	mutable SolidSphere mesh;											// Меш (сфера)
#endif // IS_ENGINE_MODE
	mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf;				// Буфер шейдера
	mutable std::shared_ptr<Camera> pCamera;							// Указатель на камеру
private:
	std::string name;													// Имя ист. света
	bool IsRendered = true;												// Разрешена ли отрисовка
};
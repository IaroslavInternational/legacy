#pragma once

#include "EngineConverter.h"

#include "TriggerStruct.h"

#if IS_ENGINE_MODE
#include "Plate.h"
#endif // IS_ENGINE_MODE

#include "BlurOutlineRenderGraph.h"

#include <DirectXMath.h>

namespace dx = DirectX;

// Базовый класс для триггеров
class Trigger
{
public:
#if IS_ENGINE_MODE
	Trigger(TriggerStruct& trs, Graphics& gfx);
	Trigger(dx::XMFLOAT3 PosTopLeft, dx::XMFLOAT3 PosTopRight,
		dx::XMFLOAT3 PosBottomLeft, dx::XMFLOAT3 PosBottomRight,
		float roll, float pitch, float yaw, Graphics& gfx);
#else
	Trigger(TriggerStruct& trs);
	Trigger(dx::XMFLOAT3 PosTopLeft, dx::XMFLOAT3 PosTopRight,
		dx::XMFLOAT3 PosBottomLeft, dx::XMFLOAT3 PosBottomRight,
		float roll, float pitch, float yaw);
#endif // IS_ENGINE_MODE

	~Trigger();

#if IS_ENGINE_MODE
	void LinkTechniques(Rgph::RenderGraph& rg);

	void Submit(size_t channels);
#endif // IS_ENGINE_MODE

	// Установка позиции
	void SetPosition(dx::XMFLOAT3 pos);

	// Установка ориентации
	void SetOrientation(dx::XMFLOAT3 orient);

	// Установка начального положение
	void SetDefault();

	// Проверка на касание триггера через координаты
	bool Check(dx::XMFLOAT3 ObjPos);

	// Установка ширины триггера
	void SetDeep(float TriggerDeep);

	// Получение позиции триггера
	const DirectX::XMFLOAT3* GetPosition() const;
private:
	// Глубина триггера по x
	float deep = 1.0f;
private:
	struct
	{
		dx::XMFLOAT3 triggerPosTopLeft;
		dx::XMFLOAT3 triggerPosTopRight;
		dx::XMFLOAT3 triggerPosBottomLeft;
		dx::XMFLOAT3 triggerPosBottomRight;
	} triggerPos;

	struct
	{
		float triggerRoll;
		float triggerPitch;
		float triggerYaw;
	} triggerOrien;

#if IS_ENGINE_MODE
	/* Визуальный контур триггера */

	float platew;
	float plateh;

	Plate plate;
	
	/*********************/
#endif // IS_ENGINE_MODE
};
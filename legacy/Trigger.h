#pragma once

#include "TriggerStruct.h"
#include "Plate.h"

#include <DirectXMath.h>

namespace dx = DirectX;

// Базовый класс для триггеров
class Trigger
{
public:
	Trigger(TriggerStruct& trs, Graphics& gfx);
	Trigger(dx::XMFLOAT3 PosTopLeft,	dx::XMFLOAT3 PosTopRight,
			dx::XMFLOAT3 PosBottomLeft, dx::XMFLOAT3 PosBottomRight,
			float roll, float pitch, float yaw,
			Graphics& gfx);
	~Trigger();

	// Отрисовка триггера
	Plate* GetPlate();

	void SetPosition(dx::XMFLOAT3 pos);

	void SetOrientation(dx::XMFLOAT3 orient);

	void SetDefault();

	// Проверка на триггер через координаты
	bool Check(dx::XMFLOAT3 CameraPos);

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

	/* 2Д-плита триггера */
	
	Graphics& Gfx;

	float platew;
	float plateh;

	Plate plate;
	
	/*********************/
};
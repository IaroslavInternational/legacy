#pragma once

#include "EngineConverter.h"
#include "TriggerStruct.h"
#include "BlurOutlineRenderGraph.h"
#include "VisibleObject.h"

#include <DirectXMath.h>

#if IS_ENGINE_MODE
#include "Plate.h"
#endif // IS_ENGINE_MODE

// Ѕазовый класс дл€ триггеров
class Trigger : public VisibleObject
{
public:
#if IS_ENGINE_MODE
	Trigger(std::string		  name, 
			Graphics&		  gfx,
			DirectX::XMFLOAT2 size,
			DirectX::XMFLOAT3 position =	{ 0.0f, 0.0f, 0.0f },
			DirectX::XMFLOAT3 orientation = { 0.0f, 1.57f, 0.0f },
			bool			  visibility =  true);
#else
	Trigger(TriggerStruct& trs);
	Trigger(DirectX::XMFLOAT3 PosTopLeft, DirectX::XMFLOAT3 PosTopRight,
		DirectX::XMFLOAT3 PosBottomLeft, DirectX::XMFLOAT3 PosBottomRight,
		float roll, float pitch, float yaw);
#endif // IS_ENGINE_MODE

#if IS_ENGINE_MODE
	void LinkTechniques(Rgph::RenderGraph& rg);

	void Submit(size_t channels);

	void SpawnControl();
#endif // IS_ENGINE_MODE

	// ”становка начального положение
	void SetDefault();

	// ѕерегруженный метод дл€ установки позиции триггера, вклю€ча€ платформу
	void SetPosition(DirectX::XMFLOAT3 position);

	// ѕерегруженный метод дл€ установки ориентации триггера, вклю€ча€ платформу
	void SetOrientation(DirectX::XMFLOAT3 orientation);

	// ѕроверка на касание триггера через координаты
	bool Check(DirectX::XMFLOAT3 ObjPos);

	// ”становка ширины триггера
	void SetDeep(float TriggerDeep);

	// ѕолучение позиции триггера
	const DirectX::XMFLOAT3* GetPosition() const;
private:
	// √лубина триггера по x
	float deep = 2.5f;
private:
	struct
	{
		void UpdatePoints(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 size) noexcept
		{
			PosTopLeft =	 { position.x, position.y,			position.z };
			PosTopRight =	 { position.x, position.y,			position.z - size.x };
			PosBottomLeft =  { position.x, position.y - size.y, position.z };
			PosBottomRight = { position.x, position.y - size.y, position.z - size.x };
		}

		DirectX::XMFLOAT3 PosTopLeft;
		DirectX::XMFLOAT3 PosTopRight;
		DirectX::XMFLOAT3 PosBottomLeft;
		DirectX::XMFLOAT3 PosBottomRight;
	} full_position;

	DirectX::XMFLOAT3 objpos = { 0.0f, 0.0f, 0.0f };

#if IS_ENGINE_MODE
	// ¬изуальный контур триггера
	Plate platform;

	DirectX::XMFLOAT2 size;
#endif // IS_ENGINE_MODE
};
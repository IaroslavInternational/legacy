#pragma once

#include <DirectXMath.h>

namespace dx = DirectX;

// Структура координат триггера
struct TriggerStruct
{

	dx::XMFLOAT3 PosTopLeft;	 // Позиция левого верхнего угла
	dx::XMFLOAT3 PosTopRight;	 // Позиция правого верхнего угла
	dx::XMFLOAT3 PosBottomLeft;  // Позиция левого нижнего угла
	dx::XMFLOAT3 PosBottomRight; // Позиция правого нижнего угла

	TriggerStruct(dx::XMFLOAT3 &ptl, dx::XMFLOAT3 &ptr,
				  dx::XMFLOAT3 &pbl, dx::XMFLOAT3 &pbr)
	{
		PosTopLeft = ptl;
		PosTopRight = ptr;
		PosBottomLeft = pbl;
		PosBottomRight = pbr;
	}

	// Оператор присваивания
	TriggerStruct& operator=(const TriggerStruct& rhs)
	{
		PosTopLeft = rhs.PosTopLeft;
		PosTopRight = rhs.PosTopRight;
		PosBottomLeft = rhs.PosBottomLeft;
		PosBottomRight = rhs.PosBottomRight;

		return *this;
	}

	// Оператор сравнения
	bool operator==(const TriggerStruct& rhs) const
	{
		return PosTopLeft.x == rhs.PosTopLeft.x &&
			PosTopLeft.y == rhs.PosTopLeft.y &&
			PosTopLeft.z == rhs.PosTopLeft.z &&
			PosTopRight.x == rhs.PosTopRight.x &&
			PosTopRight.y == rhs.PosTopRight.y &&
			PosTopRight.z == rhs.PosTopRight.z &&
			PosBottomLeft.x == rhs.PosBottomLeft.x &&
			PosBottomLeft.y == rhs.PosBottomLeft.y &&
			PosBottomLeft.z == rhs.PosBottomLeft.z &&
			PosBottomRight.x == rhs.PosBottomRight.x &&
			PosBottomRight.y == rhs.PosBottomRight.y &&
			PosBottomRight.z == rhs.PosBottomRight.z;
	}

	// Оператор не равно
	bool operator!=(const TriggerStruct& rhs) const
	{
		return !(*this == rhs);
	}
};

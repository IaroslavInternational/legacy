#pragma once

#include <DirectXMath.h>

namespace dx = DirectX;

// ��������� ��������� ��������
struct TriggerStruct
{

	dx::XMFLOAT3 PosTopLeft;	 // ������� ������ �������� ����
	dx::XMFLOAT3 PosTopRight;	 // ������� ������� �������� ����
	dx::XMFLOAT3 PosBottomLeft;  // ������� ������ ������� ����
	dx::XMFLOAT3 PosBottomRight; // ������� ������� ������� ����

	TriggerStruct(dx::XMFLOAT3 &ptl, dx::XMFLOAT3 &ptr,
				  dx::XMFLOAT3 &pbl, dx::XMFLOAT3 &pbr)
	{
		PosTopLeft = ptl;
		PosTopRight = ptr;
		PosBottomLeft = pbl;
		PosBottomRight = pbr;
	}

	// �������� ������������
	TriggerStruct& operator=(const TriggerStruct& rhs)
	{
		PosTopLeft = rhs.PosTopLeft;
		PosTopRight = rhs.PosTopRight;
		PosBottomLeft = rhs.PosBottomLeft;
		PosBottomRight = rhs.PosBottomRight;

		return *this;
	}

	// �������� ���������
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

	// �������� �� �����
	bool operator!=(const TriggerStruct& rhs) const
	{
		return !(*this == rhs);
	}
};

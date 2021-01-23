#pragma once

#include "TriggerStruct.h"
#include "Plate.h"

#include <DirectXMath.h>

namespace dx = DirectX;

// ������� ����� ��� ���������
class Trigger
{
public:
	Trigger(TriggerStruct& trs);
	Trigger(dx::XMFLOAT3 PosTopLeft, dx::XMFLOAT3 PosTopRight,
		dx::XMFLOAT3 PosBottomLeft, dx::XMFLOAT3 PosBottomRight);
	~Trigger();

	// �������� �� ������� ����� ����������
	bool Check(dx::XMFLOAT3 CameraPos);

	// ��������� ������ ��������
	void SetDeep(float TriggerDeep);

	// ��������� ������� ��������
	const DirectX::XMFLOAT3* GetPosition() const;
private:
	// ������� �������� �� x
	float deep = 1.0f;
private:
	struct
	{
		dx::XMFLOAT3 triggerPosTopLeft;
		dx::XMFLOAT3 triggerPosTopRight;
		dx::XMFLOAT3 triggerPosBottomLeft;
		dx::XMFLOAT3 triggerPosBottomRight;
	} triggerPos;

	/* 2�-����� �������� */
	
	float platew;
	float plateh;
	
	/*********************/
};
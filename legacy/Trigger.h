#pragma once

#include "TriggerStruct.h"
#include "Plate.h"
#include "BlurOutlineRenderGraph.h"

#include <DirectXMath.h>

namespace dx = DirectX;

// ������� ����� ��� ���������
class Trigger
{
public:
	Trigger(TriggerStruct& trs, Graphics& gfx);
	Trigger(dx::XMFLOAT3 PosTopLeft,	dx::XMFLOAT3 PosTopRight,
			dx::XMFLOAT3 PosBottomLeft, dx::XMFLOAT3 PosBottomRight,
			float roll, float pitch, float yaw, Graphics& gfx);
	~Trigger();

	void LinkTechniques(Rgph::RenderGraph& rg);

	void Submit(size_t channels);

	// ��������� �������
	void SetPosition(dx::XMFLOAT3 pos);

	// ��������� ����������
	void SetOrientation(dx::XMFLOAT3 orient);

	// ��������� ���������� ���������
	void SetDefault();

	// �������� �� ������� �������� ����� ����������
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

	struct
	{
		float triggerRoll;
		float triggerPitch;
		float triggerYaw;
	} triggerOrien;

	/* ���������� ������ �������� */

	float platew;
	float plateh;

	Plate plate;
	
	/*********************/
};
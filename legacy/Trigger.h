#pragma once

#include "EngineConverter.h"

#include "TriggerStruct.h"

#if IS_ENGINE_MODE
#include "Plate.h"
#endif // IS_ENGINE_MODE

#include "BlurOutlineRenderGraph.h"

#include <DirectXMath.h>

namespace dx = DirectX;

// ������� ����� ��� ���������
class Trigger
{
public:
#if IS_ENGINE_MODE
	Trigger(std::string, TriggerStruct trs, Graphics& gfx);
	Trigger(std::string, dx::XMFLOAT3 PosTopLeft, dx::XMFLOAT3 PosTopRight,
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

	void SpawnControl();
#endif // IS_ENGINE_MODE

	// ��������� �������
	void SetPosition(dx::XMFLOAT3 pos);

	// ��������� ����������
	void SetOrientation(dx::XMFLOAT3 orient);

	// ��������� ���������� ���������
	void SetDefault();

	// �������� �� ������� �������� ����� ����������
	bool Check(dx::XMFLOAT3 ObjPos);

	// ��������� ������ ��������
	void SetDeep(float TriggerDeep);

	// ��������� ������� ��������
	const DirectX::XMFLOAT3* GetPosition() const;

	std::string GetName();
private:
	// ������� �������� �� x
	float deep = 1.0f;

	std::string name;
private:
	struct
	{
		dx::XMFLOAT3 PosTopLeft;
		dx::XMFLOAT3 PosTopRight;
		dx::XMFLOAT3 PosBottomLeft;
		dx::XMFLOAT3 PosBottomRight;
	} position;

	dx::XMFLOAT3 orientation;

#if IS_ENGINE_MODE
	/* ���������� ������ �������� */

	float platew;
	float plateh;

	Plate plate;
	
	/*********************/
#endif // IS_ENGINE_MODE
};
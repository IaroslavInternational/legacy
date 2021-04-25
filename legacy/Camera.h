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
	void LinkTechniques(Rgph::RenderGraph& rg);				// �������� � �������
	void Submit(size_t channel) const;						// �������� � ������ ���������
#endif // IS_ENGINE_MODE
	void BindToGraphics(Graphics& gfx) const;				// �������� ������ � ���� �������
public:
	DirectX::XMFLOAT3 GetPosition() const noexcept;			// �������� �������
	DirectX::XMFLOAT2 GetOrientation() const noexcept;		// �������� ����������
	DirectX::XMMATRIX GetMatrix() const noexcept;			// �������� �������
	ProjectionData GetProjectionData() const noexcept;		// �������� ������ ��������
	DirectX::XMMATRIX GetProjection() const noexcept;		// �������� ��������
	void Reset(Graphics& gfx) noexcept;						// ��������
	void Rotate(float dx, float dy) noexcept;				// ������� ������
	void Translate(DirectX::XMFLOAT3 translation) noexcept;	// ����������� ������
	void SetPos(const DirectX::XMFLOAT3& pos) noexcept;		// ���������� �������
	std::string GetName() const noexcept;					// �������� ��� ������
public:
#if IS_ENGINE_MODE
	void SpawnDefaultControl(Graphics& gfx) noexcept;		// ������� ��������� ����������
#endif // IS_ENGINE_MODE
private:
	bool tethered;											//

	std::string name;										// ��� ������

	DirectX::XMFLOAT3 homePosition;							// �������� �������
	DirectX::XMFLOAT2 homeOrientation;						// �������� ����������
	DirectX::XMFLOAT3 position;								// �������
	DirectX::XMFLOAT2 orientation;							// ����������

	float travelSpeed = 24.0f;								// �������� �����������
	float rotationSpeed = 0.004f;							// �������� ��������
private:
	Projection proj;										// �������� ������

#if IS_ENGINE_MODE
	bool enableCameraIndicator = true;						// ��������� �� ��������� ���������� ������
	bool enableFrustumIndicator = false;					// ��������� �� ��������� �������� ������

	CameraIndicator indicator;								// ��������� ������
#endif // IS_ENGINE_MODE
};
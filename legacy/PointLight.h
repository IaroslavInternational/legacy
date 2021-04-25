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

struct PointLightCBuf												// ����� ������ ���. ��������� ��� �������
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
	void LinkTechniques(Rgph::RenderGraph& rg);							// �������� � �������
	void Submit(size_t channels) const noxnd;							// �������� � ������ ���������
#endif // IS_ENGINE_MODE
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;	// �������� ������ ��� �������
public:
	std::string GetName() const noexcept;								// �������� ��� ���. �����
	PointLightCBuf GetData() const noexcept;							// �������� ������ ���. ���������
	void Reset() noexcept;												// �������� ������
	std::shared_ptr<Camera> ShareCamera() const noexcept;				// �������� �������� �� ������
public:
#if IS_ENGINE_MODE
	void SpawnDefaultControl() noexcept;								// ������� ��������� ����������
#endif // IS_ENGINE_MODE
private:
	PointLightCBuf home;												// �������� ����� ������
	PointLightCBuf cbData;												// ���������� ����� ������
#if IS_ENGINE_MODE
	mutable SolidSphere mesh;											// ��� (�����)
#endif // IS_ENGINE_MODE
	mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf;				// ����� �������
	mutable std::shared_ptr<Camera> pCamera;							// ��������� �� ������
private:
	std::string name;													// ��� ���. �����
	bool IsRendered = true;												// ��������� �� ���������
};
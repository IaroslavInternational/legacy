#pragma once

#include "EngineConverter.h"

#include "PointLight.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#include "CameraContainer.h"
#endif // IS_ENGINE_MODE

#include "BlurOutlineRenderGraph.h"

class PointLightContainer
{
public:
#if IS_ENGINE_MODE
	PointLightContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg, AppLog* aLog);
#else
	PointLightContainer(std::string path, Graphics& gfx);
#endif
	~PointLightContainer();
public:
#if IS_ENGINE_MODE
	void LinkTechniques();										// �������� � �������
	void Submit(size_t channels);								// �������� � ������ ���������
#endif // IS_ENGINE_MODE
	void Bind(DirectX::FXMMATRIX view);							// �������� ������ ��� �������
public:
	void RgBindShadowCamera(Rgph::BlurOutlineRenderGraph& rg);	// ���������� ������� ������
	void AddCamerasToLight(CameraContainer* camcon);			// �������� ������ � ���. ���������
public:
#if IS_ENGINE_MODE
	void ShowLeftPanel();										// �������� ����� ������ ��� ���. ���������
	void ShowRightPanel();										// �������� ������ ������ ��� ���. ���������
#endif // IS_ENGINE_MODE
private:
	std::vector<std::unique_ptr<PointLight>> pLights;			// �������� ��������� ���������
private:
	std::string path;											// ���� � ����� � ������� �� ���. ���������
	Graphics& gfx;												// ����� ������������ ����
#if IS_ENGINE_MODE
	Rgph::RenderGraph& rg;										// ����� ������-�����
#endif // IS_ENGINE_MODE
private:
#if IS_ENGINE_MODE
	std::string selected = "";									// ��������� ������� � ������ ���. ���������
	
	bool IsSave = false;										// ���� ������ ������ ���������
	bool IsDelete = false;										// ���� ������ ������ �������
	
	AppLog* applog;												// ���
#endif // IS_ENGINE_MODE
};
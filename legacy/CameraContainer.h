#pragma once

#include "EngineConverter.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#endif

#include <string>
#include <vector>
#include <memory>

class Camera;
class Graphics;

namespace Rgph
{
	class RenderGraph;
}

class CameraContainer
{
public:
#if IS_ENGINE_MODE
	CameraContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg, AppLog* aLog);
#else
	CameraContainer(std::string path, Graphics& gfx);
#endif // IS_ENGINE_MODE
	~CameraContainer();
public:
	Camera* operator->();											// ��������� �� ������
#if IS_ENGINE_MODE
	void LinkTechniques();											// �������� � �������
#endif // IS_ENGINE_MODE
	void Submit(size_t channels) const;								// �������� � ������ ���������
	void Bind();													// ��������� ������ � ����������� ����
public:
	Camera& GetActiveCamera();
	std::shared_ptr<Camera> GetPtr2ActiveCamera();					// ��������� �� �������� ������
	std::shared_ptr<Camera> GetPtr2CameraByName(std::string name);	// ��������� �� ������ �� �����
public:
#if IS_ENGINE_MODE
	size_t CamerasAmount();											// ���-�� �����
	std::string GetCameraNameByIndex(size_t index);					// ��� ������ �� ������� � ����������
#endif // IS_ENGINE_MODE
public:
	void ShowLeftPanel();											// �������� ����� ������ ��� �����
	void OpenDialog();												// ������� ���������� ���� ��� ���������� ������ �� �����
	void AddCamera(std::shared_ptr<Camera> pCam);					// �������� ������
private:
	void DeleteCamera(std::string name);							// ������� ������
	Camera& GetControlledCamera();									// ����� ����������� ������
private:
	std::string path = "";											// ���� � ����� � ������� � �������
	Graphics& gfx;													// ����� ������������ ����
#if IS_ENGINE_MODE
	Rgph::RenderGraph& rg;											// ����� ������-�����
#endif // IS_ENGINE_MODE
	std::vector<std::shared_ptr<Camera>> cameras;					// ������
private:
	size_t active = 0;												// ������ �������� ������
	size_t controlled = 0;											// ������ ����������� ������
private:
#if IS_ENGINE_MODE
	bool IsSave = false;											// ���� ������ ������ ���������
	bool IsDelete = false;											// ���� ������ ������ �������

	AppLog* applog;													// ���
#endif
};
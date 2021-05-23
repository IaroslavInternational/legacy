#pragma once

#include "VisibleObject.h"
#include "Camera.h"

#include <memory>

class Node;
class Mesh;
class ModelWindow;

struct aiMesh;
struct aiMaterial;
struct aiNode;

namespace Rgph
{
	class RenderGraph;
}

// ����� ��� �������� ����������� 3-D �������. 
class Model : public VisibleObject
{
public:
	// ����������� ������ ������� ��������� ����:
	// name - ��� ������
	// path - ���� � �������
	// gfx - ����� ������������ ����
	// position - ������� ������
	// orientation - ���������� ������
	// visibility - ��������� ������
	Model(std::string		 name, 
		  const std::string& path, 
		  Graphics&			 gfx, 
		  DirectX::XMFLOAT3	 position =    { 0.0f, 0.0f, 0.0f }, 
		  DirectX::XMFLOAT3	 orientation = { 0.0f, 0.0f, 0.0f },
		  float				 scale =	   1.0f, 
		  bool				 visibility =  true);
	~Model() noexcept;
public:
	// ����� ��� ������������� ������ � Render Graph
	void LinkTechniques(Rgph::RenderGraph& rg);				

	// ����� ��� ������������� ������ � ������ ���������
	void Submit(size_t channels);

	// ������� ��������� ������������ ���������� ��� ������
	void Accept(class ModelProbe& probe);

	// ���������� ������ ������
	void SetScale(float scale);

	// ���������� ��������� � ������������ � ������� �������
	void SetRootTransform(DirectX::XMMATRIX tf) noexcept;

	// ���������� ������� ��������� ����������
	void SpawnDefaultControl();			

	// ������������ ������
	void ConnectCamera(std::shared_ptr<Camera> cam, DirectX::XMFLOAT3 offset);

	// �������� ������
	void DisconnectCamera();		

	// �������� ������������� ������
	bool IsCameraConne�ted();
private:
	// ������� ������� (��������� ������) ������
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node, float scale) noexcept;
private:
	// ��������� �� ��������� ������
	std::unique_ptr<Node> pRoot;							

	// ��������� �� ����
	std::vector<std::unique_ptr<Mesh>> meshPtrs;			

	// ��������� �������
	float scale = 1.0f;	

	// ��������� �� ������
	mutable std::shared_ptr<Camera> cam;	

	// ������ ������ �� ������ �� ����
	// 0 - ������ �� ��� X
	// 1 - ������ �� ��� Y
	// 2 - ������ �� ��� Z
	mutable DirectX::XMFLOAT3 offset = { 0.0f, 0.0f, 0.0f };
	
	// ������������� ������
	bool CameraConnection = false;
};
#pragma once

#include "VisibleObject.h"

#include "Graphics.h"
#include <string>
#include <memory>
#include <filesystem>

#include "Camera.h"

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

class Model : public VisibleObject
{
public:
	Model(std::string name, const std::string& path, Graphics& gfx, 
		  DirectX::XMFLOAT3 position =    { 0.0f, 0.0f, 0.0f }, 
		  DirectX::XMFLOAT3 orientation = { 0.0f, 0.0f, 0.0f },
		  float scale = 1.0f, bool visibility = true);
	~Model() noexcept;
public:
	void LinkTechniques(Rgph::RenderGraph& rg);				// �������� � �������
	void Submit(size_t channels);							// �������� � ������ ���������
	void Accept(class ModelProbe& probe);					// ������� ��������� ������������ ���������� ��� ������
	void SetScale(float scale);								// ��������� �������
	void SetRootTransform(DirectX::XMMATRIX tf) noexcept;	// ��������� ���������
	bool IsCamConnceted();									// ���� ������������ ������
	void SpawnDefaultControl();								// ������� ��������� ����������
	void ConnectCamera(std::shared_ptr<Camera> cam,	DirectX::XMFLOAT3 offset);			// ������������ ������
	void DisconnectCamera();								// �������� ������
private:
	std::unique_ptr<Node> ParseNode(int& nextId, 
		const aiNode& node, float scale) noexcept;			// ������� �������
private:
	std::unique_ptr<Node> pRoot;							// ��������� �� ��������� ������
	std::vector<std::unique_ptr<Mesh>> meshPtrs;			// ��������� �� ���
	float scale = 1.0f;										// ��������� �������
	mutable std::shared_ptr<Camera> cam;					// ��������� �� ������
	mutable float offset_x = 0.0f;							// ������ ������ �� ������ �� �x 
	mutable float offset_y = 0.0f;							// ������ ������ �� ������ �� �y
	mutable float offset_z = 0.0f;							// ������ ������ �� ������ �� �z
	bool isCamAdded = false;								// ���� ������ ����������
};
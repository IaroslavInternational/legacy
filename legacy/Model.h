#pragma once

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

// ������ ����
namespace Rgph
{
	class RenderGraph;
}

// 3D ������
class Model
{
public:
	// �����������
	// �������, ���� � ������, ������
	Model(std::string name, const std::string& path, Graphics& gfx, 
		  DirectX::XMFLOAT3 position =    { 0.0f, 0.0f, 0.0f }, 
		  DirectX::XMFLOAT3 orientation = { 0.0f, 0.0f, 0.0f },
		  float scale = 1.0f);
	~Model() noexcept;
public:
	// �������� � �������
	void LinkTechniques(Rgph::RenderGraph&);

	// �������� � ������ ���������
	void Submit(size_t channels) const noxnd;

	// ������� ��������� ������������ ���������� ��� ������
	void Accept(class ModelProbe& probe);

	// ��������� ���������
	void SetRootTransform(DirectX::XMMATRIX tf) noexcept;

	void SetScale(float scale);
	
	// ������� ��������� ����������
	void SpawnDefaultControl();

	std::string GetName();

	void SetRenderStatus(bool status);

	// ��������� �������
	DirectX::XMFLOAT3 GetPosition();

	// ��������� ����������
	DirectX::XMFLOAT3 GetOrientation();

	bool IsCamConnceted();

	// ������������ ������
	void ConnectCamera(std::shared_ptr<Camera> cam, DirectX::XMFLOAT3 offset);

	// �������� ������
	void DisconnectCamera();
private:
	// ������� ����
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials, const std::filesystem::path& path, float scale);
	
	// ������� �������
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node, float scale) noexcept;
private:
	std::unique_ptr<Node> pRoot;
	
	// ������� ������ ������?
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
private:
	/* ������ ������ */
	
	// ��� ������
	std::string name;

	// ��������� �������
	float scale = 1.0f;

	// �������
	DirectX::XMFLOAT3 position;

	// ����������
	DirectX::XMFLOAT3 orientation;

	// ��������� ��  ���������
	bool IsRendered = true;

	/*****************/
private:
	/* ����������� ������ */
	
	// ��������� �� ������
	mutable std::shared_ptr<Camera> cam;

	mutable float offset_x = 0.0f;	// ������ ������ �� ������ �� �x 
	mutable float offset_y = 0.0f;	// ������ ������ �� ������ �� �y
	mutable float offset_z = 0.0f;	// ������ ������ �� ������ �� �z
	
	// ���� ������ ����������
	bool isCamAdded = false;

	/**********************/
};
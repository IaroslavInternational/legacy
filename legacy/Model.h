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
	Model(Graphics& gfx, const std::string& pathString, float scale = 1.0f);
	~Model() noexcept;
public:
	void LinkTechniques(Rgph::RenderGraph&);
	void Submit(size_t channels) const noxnd;

	void Accept(class ModelProbe& probe);

	// ��������� �������
	void SetRootTransform(DirectX::FXMMATRIX tf) noexcept;
	
	// ������� ��������� ����������
	void SpawnDefaultControl();

	// ��������� �������
	DirectX::XMFLOAT3 GetCurrentPosition();

	// ��������� ����������
	DirectX::XMFLOAT3 GetCurrentOrientation();

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
	
	// sharing meshes here perhaps dangerous?!!!
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
private:
	/* ��������� */
	
	// �������
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };

	// ����
	float roll = 0.0f;
	
	// ������
	float pitch = 0.0f;
	
	// ��������
	float yaw = 0.0f;

	/*************/
private:
	/* ����������� ������ */
	
	// ��������� �� ������
	mutable std::shared_ptr<Camera> cam;

	float offset_x = 0.0f;	// ������ ������ �� ������ �� �x 
	float offset_y = 0.0f;	// ������ ������ �� ������ �� �y
	float offset_z = 0.0f;	// ������ ������ �� ������ �� �z
	
	// ���� ������ ����������
	bool isCamAdded = false;

	/**********************/
};
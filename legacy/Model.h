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

namespace Rgph
{
	class RenderGraph;
}

class Model
{
public:
	Model(Graphics& gfx, const std::string& pathString, float scale = 1.0f);
	~Model() noexcept;

	void Submit(size_t channels) const noxnd;
	void SetRootTransform(DirectX::FXMMATRIX tf) noexcept;
	void Accept(class ModelProbe& probe);
	void LinkTechniques(Rgph::RenderGraph&);
private:
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials, const std::filesystem::path& path, float scale);
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node, float scale) noexcept;
private:
	std::unique_ptr<Node> pRoot;
	// sharing meshes here perhaps dangerous?!!!
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
private:
	DirectX::XMFLOAT3 pos = { 0, 0 ,0 };

	float roll = 0;
	float pitch = 0;
	float yaw = 0;
private:
	mutable std::shared_ptr<Camera> cam;
	bool isCamAdded = false;
public:
	void ConnectCamera(std::shared_ptr<Camera> cam);
	void MoveX(float delta = 0.1f);

	DirectX::XMFLOAT3 GetCurrentPosition();
	DirectX::XMFLOAT3 GetCurrentOrientation();
public:
	void SpawnDefaultControl();
};
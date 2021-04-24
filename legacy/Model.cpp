#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ModelException.h"
#include "Node.h"
#include "Mesh.h"
#include "Material.h"
#include "EngineXM.h"

#if IS_ENGINE_MODE
#include "imgui\imgui.h"
#endif //IS_ENGINE_MODE

namespace dx = DirectX;

Model::Model(std::string name, const std::string& path, Graphics& gfx,
			 DirectX::XMFLOAT3 position,
			 DirectX::XMFLOAT3 orientation,
			 float scale)
	:
	name(name),
	scale(scale),
	position(position),
	orientation(orientation)
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(path.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiAnimBehaviour_REPEAT);

	if( pScene == nullptr )
	{
		throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
	}

	// parse materials
	std::vector<Material> materials;
	materials.reserve( pScene->mNumMaterials );
	for (size_t i = 0; i < pScene->mNumMaterials; i++)
	{
		materials.emplace_back(gfx, *pScene->mMaterials[i], path);
	}

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		const auto& mesh = *pScene->mMeshes[i];
		meshPtrs.push_back(std::make_unique<Mesh>(gfx, materials[mesh.mMaterialIndex], mesh, scale));
	}

	int nextId = 0;
	pRoot = ParseNode(nextId, *pScene->mRootNode, scale);
}

Model::~Model() noexcept
{}

void Model::LinkTechniques(Rgph::RenderGraph& rg)
{
	if (IsRendered)
	{
		for (auto& pMesh : meshPtrs)
		{
			pMesh->LinkTechniques(rg);
		}
	}
}

void Model::Submit(size_t channels) const noxnd
{
	if (IsRendered)
	{
		pRoot->Submit(channels, dx::XMMatrixIdentity());
	}
}

void Model::Accept(ModelProbe& probe)
{
	pRoot->Accept(probe);
}

void Model::SetScale(float scale)
{
	pRoot->SetAppliedTransform(ScaleTranslation(dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
		&pRoot->GetAppliedTransform()
	)), scale));
}

void Model::SetRootTransform(DirectX::XMMATRIX tf) noexcept
{
	pRoot->SetAppliedTransform(tf);
}

bool Model::IsCamConnceted()
{
	return isCamAdded;
}

std::string Model::GetName()
{
	return name;
}

DirectX::XMFLOAT3 Model::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Model::GetOrientation()
{
	return orientation;
}

#if IS_ENGINE_MODE
void Model::SpawnDefaultControl()
{
	if (ImGui::BeginChild(""))
	{
		bool rotDirty = false;
		bool posDirty = false;
		bool scaleDirty = false;

		const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };

		ImGui::Text("Позиция");
		dcheck(ImGui::SliderFloat("X", &position.x, -80.0f, 80.0f, "%.1f"), posDirty);
		dcheck(ImGui::SliderFloat("Y", &position.y, -80.0f, 80.0f, "%.1f"), posDirty);
		dcheck(ImGui::SliderFloat("Z", &position.z, -80.0f, 80.0f, "%.1f"), posDirty);

		ImGui::Text("Ориентация");
		dcheck(ImGui::SliderAngle("Крен", &orientation.x, 0.995f * -90.0f, 0.995f * 90.0f), rotDirty);
		dcheck(ImGui::SliderAngle("Тангаш",	&orientation.y, 0.995f * -90.0f, 0.995f * 90.0f), rotDirty);
		dcheck(ImGui::SliderAngle("Расканье", &orientation.z, -180.0f, 180.0f), rotDirty);

		ImGui::Text("Размер");
		dcheck(ImGui::SliderFloat("S", &scale, 0.0001f, 10.0f, "%.4f"), scaleDirty);

		ImGui::Checkbox("Скрыть", &IsRendered);

		if (isCamAdded)
		{
			cam->SetPos(DirectX::XMFLOAT3(position.x + offset_x, position.y + offset_y, position.z + offset_z));

			ImGui::Separator();

			ImGui::Text("Привязанная камера:");			
			ImGui::Text(cam->GetName().c_str());
		}

		SetRootTransform
		(
			DirectX::XMMatrixRotationX(orientation.x) *
			DirectX::XMMatrixRotationY(orientation.y) *
			DirectX::XMMatrixRotationZ(orientation.z) *
			DirectX::XMMatrixTranslation(
				position.x,
				position.y,
				position.z
			)
		);
	}

	// 	ImGui::EndChild() в соответсвтующем методе контейнера
}
#endif //IS_ENGINE_MODE

void Model::ConnectCamera(std::shared_ptr<Camera> cam, DirectX::XMFLOAT3 offset)
{
	this->cam = cam;
	isCamAdded = true;

	offset_x = offset.x;
	offset_y = offset.y;
	offset_z = offset.z;
}

void Model::DisconnectCamera()
{
	if (isCamAdded)
	{
		cam.reset();
		
		isCamAdded = false;
	}
}

std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode& node, float scale) noexcept
{
	const auto transform = ScaleTranslation(dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
		reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
	)), scale);

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
	}

	auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);

	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(nextId, *node.mChildren[i], scale));
	}

	return pNode;
}
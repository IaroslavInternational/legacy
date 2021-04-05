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

Model::Model( Graphics& gfx,const std::string& pathString,const float scale )
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile( pathString.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	if( pScene == nullptr )
	{
		throw ModelException( __LINE__,__FILE__,imp.GetErrorString() );
	}

	// parse materials
	std::vector<Material> materials;
	materials.reserve( pScene->mNumMaterials );
	for( size_t i = 0; i < pScene->mNumMaterials; i++ )
	{
		materials.emplace_back( gfx,*pScene->mMaterials[i],pathString );
	}

	for( size_t i = 0; i < pScene->mNumMeshes; i++ )
	{
		const auto& mesh = *pScene->mMeshes[i];
		meshPtrs.push_back( std::make_unique<Mesh>( gfx,materials[mesh.mMaterialIndex],mesh,scale ) );
	}

	int nextId = 0;
	pRoot = ParseNode( nextId,*pScene->mRootNode,scale );
}

Model::~Model() noexcept
{}

void Model::Submit( size_t channels ) const noxnd
{
	pRoot->Submit( channels,dx::XMMatrixIdentity() );
}

void Model::SetRootTransform( DirectX::FXMMATRIX tf ) noexcept
{
	pRoot->SetAppliedTransform( tf );
}

void Model::Accept( ModelProbe & probe )
{
	pRoot->Accept( probe );
}

void Model::LinkTechniques( Rgph::RenderGraph& rg )
{
	for( auto& pMesh : meshPtrs )
	{
		pMesh->LinkTechniques( rg );
	}
}

std::unique_ptr<Node> Model::ParseNode( int& nextId,const aiNode& node,float scale ) noexcept
{
	namespace dx = DirectX;
	const auto transform = ScaleTranslation( dx::XMMatrixTranspose( dx::XMLoadFloat4x4(
		reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
	) ),scale );

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve( node.mNumMeshes );
	for( size_t i = 0; i < node.mNumMeshes; i++ )
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back( meshPtrs.at( meshIdx ).get() );
	}

	auto pNode = std::make_unique<Node>( nextId++,node.mName.C_Str(),std::move( curMeshPtrs ),transform );
	for( size_t i = 0; i < node.mNumChildren; i++ )
	{
		pNode->AddChild( ParseNode( nextId,*node.mChildren[i],scale ) );
	}

	return pNode;
}

void Model::ConnectCamera(std::shared_ptr<Camera> cam)
{
	this->cam = std::move(cam);

	isCamAdded = true;
}

void Model::MoveX(float delta)
{
	pos.x += delta;

	if (isCamAdded)
		cam->SetPos(DirectX::XMFLOAT3(pos.x - 35.0f, 35.0f, -5.1f));
}

DirectX::XMFLOAT3 Model::GetCurrentPosition()
{
	return pos;
}

DirectX::XMFLOAT3 Model::GetCurrentOrientation()
{
	return DirectX::XMFLOAT3(roll, pitch, yaw);
}

#if IS_ENGINE_MODE
void Model::SpawnDefaultControl()
{
	if (ImGui::BeginChild(""))
	{
		bool rotDirty = false;
		bool posDirty = false;

		const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };

		ImGui::Text("Позиция");
		dcheck(ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.01f"), posDirty);
		dcheck(ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.01f"), posDirty);
		dcheck(ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.01f"), posDirty);

		ImGui::Text("Ориентация");
		dcheck(ImGui::SliderAngle("Крен", &roll, 0.995f * -90.0f, 0.995f * 90.0f), rotDirty);
		dcheck(ImGui::SliderAngle("Тангаш", &pitch, 0.995f * -90.0f, 0.995f * 90.0f), rotDirty);
		dcheck(ImGui::SliderAngle("Расканье", &yaw, -180.0f, 180.0f), rotDirty);

		SetRootTransform
		(
			DirectX::XMMatrixRotationX(roll) *
			DirectX::XMMatrixRotationY(pitch) *
			DirectX::XMMatrixRotationZ(yaw) *
			DirectX::XMMatrixTranslation(
				pos.x,
				pos.y,
				pos.z
			)
		);
	}

	ImGui::EndChild();
}
#endif //IS_ENGINE_MODE
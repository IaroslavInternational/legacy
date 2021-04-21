#include "PointLight.h"

#if IS_ENGINE_MODE
#include "imgui/imgui.h"
#endif // IS_ENGINE_MODE

#include "Camera.h"
#include "EngineMath.h"

PointLight::PointLight( Graphics& gfx, std::string name, DirectX::XMFLOAT3 pos, float radius)
	:
	mesh( gfx,radius ),
	cbuf( gfx )
{
	home = {
		pos,
		{ 0.05f,0.05f,0.05f },
		{ 1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.025f,
		0.0030f,
	};
	Reset();
	pCamera = std::make_shared<Camera>(gfx, name, cbData.pos);
}

#if IS_ENGINE_MODE
void PointLight::SpawnControlWindow() noexcept
{
	if( ImGui::BeginChild( "Точечный источник освещения" ) )
	{
		bool dirtyPos = false;
		const auto d = [&dirtyPos]( bool dirty ){dirtyPos = dirtyPos || dirty;};

		ImGui::Text( "Позиция" );
		d( ImGui::SliderFloat( "X",&cbData.pos.x,-80.0f,80.0f,"%.4f" ) );
		d( ImGui::SliderFloat( "Y",&cbData.pos.y,-80.0f,80.0f,"%.4f" ) );
		d( ImGui::SliderFloat( "Z",&cbData.pos.z,-80.0f,80.0f,"%.4f" ) );

		if( dirtyPos )
		{
			pCamera->SetPos( cbData.pos );
		}
		
		ImGui::Text( "Интенсивность и йвет" );
		ImGui::SliderFloat( "Интенсивность",&cbData.diffuseIntensity,0.01f,2.0f,"%.2f",2 );
		ImGui::ColorEdit3( "Цвет",&cbData.diffuseColor.x );
		ImGui::ColorEdit3( "Цвет среды",&cbData.ambient.x );
		
		ImGui::Text( "Тень" );
		ImGui::SliderFloat( "Constant",&cbData.attConst,0.05f,10.0f,"%.2f",4 );
		ImGui::SliderFloat( "Linear",&cbData.attLin,0.0001f,4.0f,"%.4f",8 );
		ImGui::SliderFloat( "Quadratic",&cbData.attQuad,0.0000001f,0.5f,"%.7f",10 );

		if( ImGui::Button( "Сбросить" ) )
		{
			Reset();
		}
	}
	ImGui::EndChild();
}
#endif // IS_ENGINE_MODE

void PointLight::Reset() noexcept
{
	cbData = home;
}

void PointLight::Submit( size_t channels ) const noxnd
{
	mesh.SetPos( cbData.pos );
	mesh.Submit( channels );
}

void PointLight::Bind( Graphics& gfx,DirectX::FXMMATRIX view ) const noexcept
{
	auto dataCopy = cbData;
	const auto pos = DirectX::XMLoadFloat3( &cbData.pos );
	DirectX::XMStoreFloat3( &dataCopy.pos,DirectX::XMVector3Transform( pos,view ) );
	cbuf.Update( gfx,dataCopy );
	cbuf.Bind( gfx );
}

void PointLight::LinkTechniques( Rgph::RenderGraph& rg )
{
	mesh.LinkTechniques( rg );
}

std::shared_ptr<Camera> PointLight::ShareCamera() const noexcept
{
	return pCamera;
}
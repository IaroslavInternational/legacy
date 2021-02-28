#include "Plate.h"
#include "Plane.h"
#include "BindableCommon.h"
#include "imgui/imgui.h"
#include "DynamicConstant.h"
#include "TechniqueProbe.h"
#include "Channels.h"
#include "ConstantBuffersEx.h"

Plate::Plate(Graphics& gfx, float in_width, float in_height, DirectX::XMFLOAT4 color )
	:
	pmc( { color } )
{
	width = in_width;
	height = in_height;

	using namespace Bind;
	namespace dx = DirectX;

	//auto model = Plane::MakeTesselatedTextured(Plane::Make().vertices.GetLayout(), 1, 1);
	auto model = Plane::Make();

	//model.Transform(dx::XMMatrixScaling(width, height, 1.0f));
	model.SetNormalsIndependentFlat();
	const auto geometryTag = "$plane." + std::to_string(width);
	pVertices = VertexBuffer::Resolve(gfx, geometryTag, model.vertices);
	pIndices = IndexBuffer::Resolve(gfx, geometryTag, model.indices);
	pTopology = Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	auto tcb = std::make_shared<TransformCbuf>(gfx);
	{
		/*Technique shade("Shade", Chan::main);
		{
			Step only("lambertian");

			//only.AddBindable(Texture::Resolve(gfx, "Images\\Triggers\\TriggerV6.jpg"));
			//only.AddBindable(Sampler::Resolve(gfx));

			//auto pvs = VertexShader::Resolve(gfx, "ShadowTest_VS.cso");
			//only.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), *pvs));
			//only.AddBindable(std::move(pvs));

			//only.AddBindable(PixelShader::Resolve(gfx, "ShadowTest_PS.cso"));

			Dcb::RawLayout lay;
			lay.Add<Dcb::Float3>("specularColor");
			lay.Add<Dcb::Float>("specularWeight");
			lay.Add<Dcb::Float>("specularGloss");
			auto buf = Dcb::Buffer(std::move(lay));
			buf["specularColor"] = dx::XMFLOAT3{ 1.0f,1.0f,1.0f };
			buf["specularWeight"] = 0.1f;
			buf["specularGloss"] = 20.0f;
			only.AddBindable(std::make_shared<Bind::CachingPixelConstantBufferEx>(gfx, buf, 1u));


			only.AddBindable(Rasterizer::Resolve(gfx, false));

			only.AddBindable(tcb);

			shade.AddStep(std::move(only));
		}
		AddTechnique(std::move(shade));
	}*/
		{
			Technique outline("Outline", Chan::main);
			{
				Step mask("outlineMask");

				// TODO: better sub-layout generation tech for future consideration maybe
				mask.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), *VertexShader::Resolve(gfx, "Solid_VS.cso")));

				mask.AddBindable(std::move(tcb));

				// TODO: might need to specify rasterizer when doubled-sided models start being used

				outline.AddStep(std::move(mask));
			}
			{
				Step draw("outlineDraw");

				Dcb::RawLayout lay;
				lay.Add<Dcb::Float4>("color");
				auto buf = Dcb::Buffer(std::move(lay));
				buf["color"] = pmc.color;
				draw.AddBindable(std::make_shared<Bind::CachingPixelConstantBufferEx>(gfx, buf, 1u));

				// TODO: better sub-layout generation tech for future consideration maybe
				draw.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), *VertexShader::Resolve(gfx, "Solid_VS.cso")));

				draw.AddBindable(std::make_shared<TransformCbuf>(gfx));

				// TODO: might need to specify rasterizer when doubled-sided models start being used

				outline.AddStep(std::move(draw));
			}
			AddTechnique(std::move(outline));
		}
		// shadow map technique
		/*{
			Technique map{ "ShadowMap",Chan::shadow,true };
			{
				Step draw("shadowMap");

				// TODO: better sub-layout generation tech for future consideration maybe
				draw.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), *VertexShader::Resolve(gfx, "Solid_VS.cso")));

				draw.AddBindable(std::make_shared<TransformCbuf>(gfx));

				// TODO: might need to specify rasterizer when doubled-sided models start being used

				map.AddStep(std::move(draw));
			}
			AddTechnique(std::move(map));
		}*/
	}
}

void Plate::SetPos( DirectX::XMFLOAT3 pos ) noexcept
{
	this->pos = pos;
	this->pos.y -= height / 2;
	this->pos.z -= width / 2;
}

void Plate::SetRotation( float roll,float pitch,float yaw ) noexcept
{
	this->roll = roll;
	this->pitch = pitch;
	this->yaw = yaw;
}

DirectX::XMMATRIX Plate::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw( roll,pitch,yaw ) *
		DirectX::XMMatrixTranslation( pos.x,pos.y,pos.z );
}

void Plate::SetWidth(float w) 
{
	width = w;
}

void Plate::SetHeight(float h) 
{
	height = h;
}

void Plate::SpawnControlWindow( Graphics& gfx,const std::string& name ) noexcept
{
	if( ImGui::Begin( name.c_str() ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X",&pos.x,-80.0f,80.0f,"%.1f" );
		ImGui::SliderFloat( "Y",&pos.y,-80.0f,80.0f,"%.1f" );
		ImGui::SliderFloat( "Z",&pos.z,-80.0f,80.0f,"%.1f" );
		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Roll",&roll,-180.0f,180.0f );
		ImGui::SliderAngle( "Pitch",&pitch,-180.0f,180.0f );
		ImGui::SliderAngle( "Yaw",&yaw,-180.0f,180.0f );
		//ImGui::Text("Size");
		//ImGui::SliderFloat("Size", &width, 0.0f, 25.0f);
		//ImGui::Text( "Shading" );
		//auto pBlender = QueryBindable<Bind::Blender>();
		//float factor = pBlender->GetFactor();
		//ImGui::SliderFloat( "Translucency",&factor,0.0f,1.0f );
		//pBlender->SetFactor( factor );
	}
	ImGui::End();
}

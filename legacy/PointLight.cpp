#include "PointLight.h"

#if IS_ENGINE_MODE
#include "imgui/imgui.h"
#endif // IS_ENGINE_MODE

#include "Camera.h"
#include "EngineMath.h"

PointLight::PointLight(Graphics& gfx, std::string name, PointLightCBuf data, float radius)
	:
	mesh(gfx, radius),
	cbuf(gfx),
	name(name),
	home(data)
{
	Reset();

	pCamera = std::make_shared<Camera>(gfx, name, cbData.pos);
}

#if IS_ENGINE_MODE
void PointLight::LinkTechniques(Rgph::RenderGraph& rg)
{
	if (IsRendered)
	{
		mesh.LinkTechniques(rg);
	}
}

void PointLight::Submit(size_t channels) const noxnd
{
	if (IsRendered)
	{
		mesh.SetPos(cbData.pos);
		mesh.Submit(channels);
	}
}
#endif // IS_ENGINE_MODE

void PointLight::Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept
{
	auto dataCopy = cbData;
	const auto pos = DirectX::XMLoadFloat3(&cbData.pos);
	DirectX::XMStoreFloat3(&dataCopy.pos, DirectX::XMVector3Transform(pos, view));
	cbuf.Update(gfx, dataCopy);
	cbuf.Bind(gfx);
}

#if IS_ENGINE_MODE
void PointLight::SpawnDefaultControl() noexcept
{
	if( ImGui::BeginChild( "Точечный источник освещения" ) )
	{
		bool dirtyPos = false;
		bool dirtyRender = false;
		
		const auto dP = [&dirtyPos](bool dirty) {dirtyPos = dirtyPos || dirty; };
		const auto dR = [&dirtyRender](bool dirty) {dirtyRender = dirtyRender || dirty; };

		ImGui::Text("Позиция");
		dP(ImGui::SliderFloat("X", &cbData.pos.x, -80.0f, 80.0f, "%.4f"));
		dP(ImGui::SliderFloat("Y", &cbData.pos.y, -80.0f, 80.0f, "%.4f"));
		dP(ImGui::SliderFloat("Z", &cbData.pos.z, -80.0f, 80.0f, "%.4f"));

		if( dirtyPos )
		{
			pCamera->SetPos(cbData.pos);
		}
		
		ImGui::Text("Интенсивность и цвет");
		ImGui::SliderFloat("Интенсивность", &cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2);
		ImGui::ColorEdit3("Цвет", &cbData.diffuseColor.x);
		ImGui::ColorEdit3("Цвет среды", &cbData.ambient.x);
		
		ImGui::Text("Тень");
		ImGui::SliderFloat("Constant", &cbData.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &cbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &cbData.attQuad, 0.00000001f, 0.2f, "%.8f", 10);

		ImGui::Checkbox("Скрыть", &IsRendered);

		if (ImGui::Button("Сбросить"))
		{
			Reset();
		}
	}

	ImGui::EndChild();
}
#endif // IS_ENGINE_MODE

std::string PointLight::GetName() const noexcept
{
	return name;
}

PointLightCBuf PointLight::GetData() const noexcept
{
	return cbData;
}

void PointLight::Reset() noexcept
{
	cbData = home;
}

std::shared_ptr<Camera> PointLight::ShareCamera() const noexcept
{
	return pCamera;
}
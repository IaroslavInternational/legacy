#include "Camera.h"

#if IS_ENGINE_MODE
#include "imgui/imgui.h"
#endif // IS_ENGINE_MODE

#include "EngineMath.h"
#include "Graphics.h"

namespace dx = DirectX;

Camera::Camera(Graphics& gfx, std::string name,
			   dx::XMFLOAT3 position,
			   dx::XMFLOAT2 orientation,
			   bool tethered) noexcept
	:
	name(std::move(name)),
	homePosition(position),
	homeOrientation(orientation),
	proj(gfx, 1.0f, 9.0f / 16.0f, 0.5f, 400.0f),
	indicator(gfx),
	tethered(tethered)
{
	if (tethered)
	{
		this->position = homePosition;

		indicator.SetPos(position);
		proj.SetPos(position);
	}

	this->orientation = homeOrientation;

	Reset(gfx);
}

#if IS_ENGINE_MODE
void Camera::LinkTechniques(Rgph::RenderGraph& rg)
{
	indicator.LinkTechniques(rg);
	proj.LinkTechniques(rg);
}

void Camera::Submit(size_t channels) const
{
	if (enableCameraIndicator)
	{
		indicator.Submit(channels);
	}
	if (enableFrustumIndicator)
	{
		proj.Submit(channels);
	}
}
#endif // IS_ENGINE_MODE

void Camera::BindToGraphics(Graphics& gfx) const
{
	gfx.SetCamera(GetMatrix());
	gfx.SetProjection(proj.GetMatrix());
}

DirectX::XMFLOAT3 Camera::GetPosition() const noexcept
{
	return position;
}

DirectX::XMFLOAT2 Camera::GetOrientation() const noexcept
{
	return orientation;
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	using namespace dx;

	const dx::XMVECTOR forwardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// apply the camera rotations to a base vector
	const auto lookVector = XMVector3Transform(forwardBaseVector,
		XMMatrixRotationRollPitchYaw(orientation.x, orientation.y, 0.0f)
	);
	// generate camera transform (applied to all objects to arrange them relative
	// to camera position/orientation in world) from cam position and direction
	// camera "top" always faces towards +Y (cannot do a barrel roll)
	const auto camPosition = XMLoadFloat3(&position);
	const auto camTarget = camPosition + lookVector;

	return XMMatrixLookAtLH(camPosition, camTarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

DirectX::XMMATRIX Camera::GetProjection() const noexcept
{
	return proj.GetMatrix();
}

void Camera::Reset(Graphics& gfx) noexcept
{
	if (!tethered)
	{
		position = homePosition;

		indicator.SetPos(position);
		proj.SetPos(position);
	}

	orientation = homeOrientation;

#if IS_ENGINE_MODE
	const dx::XMFLOAT3 angles = { orientation.x,orientation.y,0.0f };

	indicator.SetRotation(angles);
	proj.SetRotation(angles);
	proj.Reset(gfx);
#endif // IS_ENGINE_MODE
}

void Camera::Rotate(float dx, float dy) noexcept
{
	orientation.y = wrap_angle(orientation.y + dx * rotationSpeed);
	orientation.x = std::clamp(orientation.x + dy * rotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);

	const dx::XMFLOAT3 angles = { orientation.x,orientation.y,0.0f };

	indicator.SetRotation(angles);
	proj.SetRotation(angles);
}

void Camera::Translate(DirectX::XMFLOAT3 translation) noexcept
{
	if (!tethered)
	{
		dx::XMStoreFloat3(&translation, dx::XMVector3Transform(
			dx::XMLoadFloat3(&translation),
			dx::XMMatrixRotationRollPitchYaw(orientation.x, orientation.y, 0.0f) *
			dx::XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
		));

		position = {
			position.x + translation.x,
			position.y + translation.y,
			position.z + translation.z
		};

		indicator.SetPos(position);
		proj.SetPos(position);
	}
}

void Camera::SetPos(const DirectX::XMFLOAT3& pos) noexcept
{
	this->position = pos;

#if IS_ENGINE_MODE
	indicator.SetPos(pos);
	proj.SetPos(pos);
#endif // IS_ENGINE_MODE
}

std::string Camera::GetName() const noexcept
{
	return name;
}

#if IS_ENGINE_MODE
void Camera::SpawnDefaultControl(Graphics& gfx) noexcept
{
	bool rotDirty = false;
	bool posDirty = false;

	const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };

	if (!tethered)
	{
		ImGui::Text("Позиция");
		dcheck(ImGui::SliderFloat("X", &position.x, -80.0f, 80.0f, "%.1f"), posDirty);
		dcheck(ImGui::SliderFloat("Y", &position.y, -80.0f, 80.0f, "%.1f"), posDirty);
		dcheck(ImGui::SliderFloat("Z", &position.z, -80.0f, 80.0f, "%.1f"), posDirty);
	}

	ImGui::Text("Ориентация");
	dcheck(ImGui::SliderAngle("Тангаж", &orientation.x, 0.995f * -90.0f, 0.995f * 90.0f), rotDirty);
	dcheck(ImGui::SliderAngle("Расканье", &orientation.y, -180.0f, 180.0f), rotDirty);

	proj.RenderWidgets(gfx);

	ImGui::Checkbox("Индикатор", &enableCameraIndicator);
	ImGui::Checkbox("Отображение", &enableFrustumIndicator);

	if (ImGui::Button("Сбросить"))
	{
		Reset(gfx);
	}

	if (rotDirty)
	{
		const dx::XMFLOAT3 angles = { orientation.x, orientation.y, 0.0f };
		indicator.SetRotation(angles);
		proj.SetRotation(angles);
	}

	if (posDirty)
	{
		indicator.SetPos(position);
		proj.SetPos(position);
	}
}
#endif // IS_ENGINE_MODE
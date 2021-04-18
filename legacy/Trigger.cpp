#include "Trigger.h"

#include "imgui\imgui.h"

float CalcWidth(float x1, float x2, float z1, float z2)
{
	float wx = abs(x1 - x2);
	float wz = abs(z1 - z2);

	if (wx != 0)
	{
		return wx;
	}
	else if (wz != 0)
	{
		return wz;
	}
}

float CalcHeight(float y1, float y2)
{
	float hx = abs(y1 - y2);

	if (hx != 0)
	{
		return hx;
	}
}

#if IS_ENGINE_MODE
Trigger::Trigger(std::string name, TriggerStruct trs, Graphics& gfx)
	:
	Trigger(name, trs.PosTopLeft, trs.PosTopRight, trs.PosBottomLeft, trs.PosBottomRight, trs.Roll, trs.Pitch, trs.Yaw, gfx)
{}

Trigger::Trigger(std::string name, dx::XMFLOAT3 PosTopLeft, dx::XMFLOAT3 PosTopRight,
				 dx::XMFLOAT3 PosBottomLeft, dx::XMFLOAT3 PosBottomRight,
				 float roll, float pitch, float yaw,
				 Graphics& gfx)
	:
	platew(CalcWidth(PosTopLeft.x, PosTopRight.x, PosTopLeft.z, PosTopRight.z)),
	plateh(CalcHeight(PosTopLeft.y, PosBottomLeft.y)),
	plate(gfx, platew, plateh),
	name(name)
{
	position.PosTopLeft = PosTopLeft;
	position.PosTopRight = PosTopRight;
	position.PosBottomLeft = PosBottomLeft;
	position.PosBottomRight = PosBottomRight;

	orientation.x = roll;
	orientation.y = pitch;
	orientation.z = yaw;
}
#else

Trigger::Trigger(TriggerStruct& trs)
	:
	Trigger(trs.PosTopLeft, trs.PosTopRight, trs.PosBottomLeft, trs.PosBottomRight, trs.Roll, trs.Pitch, trs.Yaw)
{}

Trigger::Trigger(dx::XMFLOAT3 PosTopLeft, dx::XMFLOAT3 PosTopRight,
				 dx::XMFLOAT3 PosBottomLeft, dx::XMFLOAT3 PosBottomRight, 
				 float roll, float pitch, float yaw)
{
	position.PosTopLeft = PosTopLeft;
	position.PosTopRight = PosTopRight;
	position.PosBottomLeft = PosBottomLeft;
	position.PosBottomRight = PosBottomRight;

	triggerOrien.triggerRoll = roll;
	triggerOrien.triggerPitch = pitch;
	triggerOrien.triggerYaw = yaw;
}
#endif // IS_ENGINE_MODE

Trigger::~Trigger()
{
}

#if IS_ENGINE_MODE
void Trigger::LinkTechniques(Rgph::RenderGraph& rg)
{
	plate.LinkTechniques(rg);
}

void Trigger::Submit(size_t channels)
{
	plate.Submit(channels);
}

void Trigger::SpawnControl()
{
	bool rotDirty = false;
	bool posDirty = false;

	const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };

	if (ImGui::BeginChild("Триггер"))
	{
		ImGui::Text("Позиция");
		dcheck(ImGui::SliderFloat("X", &position.PosTopLeft.x, -80.0f, 80.0f, "%.2f"), posDirty);
		dcheck(ImGui::SliderFloat("Y", &position.PosTopLeft.y, -80.0f, 80.0f, "%.2f"), posDirty);
		dcheck(ImGui::SliderFloat("Z", &position.PosTopLeft.z, -80.0f, 80.0f, "%.2f"), posDirty);

		plate.SetPos(position.PosTopLeft);
	}
}

void Trigger::SetPosition(dx::XMFLOAT3 pos)
{
	plate.SetPos(pos);
}

void Trigger::SetOrientation(dx::XMFLOAT3 orientation )
{
	plate.SetRotation(orientation.x, orientation.y, orientation.z);
}

void Trigger::SetDefault()
{
	plate.SetPos(position.PosTopLeft);
	plate.SetRotation(orientation.x, orientation.y, orientation.z);
}
#endif // IS_ENGINE_MODE

bool Trigger::Check(dx::XMFLOAT3 ObjPos)
{
	return (ObjPos.x >= this->position.PosTopLeft.x 
		    && ObjPos.x <= this->position.PosTopLeft.x + deep) &&
		    ObjPos.y <= this->position.PosTopLeft.y &&
		    ObjPos.z <= this->position.PosTopLeft.z &&
		    
		    (ObjPos.x >= this->position.PosTopRight.x
		    && ObjPos.x <= this->position.PosTopRight.x + deep) &&
		    ObjPos.y <= this->position.PosTopRight.y &&
		    ObjPos.z >= this->position.PosTopRight.z &&
		    
		    (ObjPos.x >= this->position.PosBottomLeft.x
		    && ObjPos.x <= this->position.PosBottomLeft.x + deep) &&
		    ObjPos.y >= this->position.PosBottomLeft.y &&
		    ObjPos.z <= this->position.PosBottomLeft.z &&
		    
		    (ObjPos.x >= this->position.PosBottomRight.x 
		    && ObjPos.x <= this->position.PosBottomRight.x + deep) &&
		    ObjPos.y >= this->position.PosBottomRight.y &&
		    ObjPos.z >= this->position.PosBottomRight.z;
}

void Trigger::SetDeep(float TriggerDeep)
{
	deep = TriggerDeep;
}

const DirectX::XMFLOAT3* Trigger::GetPosition() const
{
	DirectX::XMFLOAT3 arr[] = 
	{ 
		position.PosTopLeft,	position.PosTopRight,
		position.PosBottomLeft, position.PosBottomRight 
	};

	return arr;
}

std::string Trigger::GetName()
{
	return name;
}

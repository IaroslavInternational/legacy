#include "Trigger.h"

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
Trigger::Trigger(TriggerStruct& trs, Graphics& gfx)
	:
	Trigger(trs.PosTopLeft, trs.PosTopRight, trs.PosBottomLeft, trs.PosBottomRight, trs.Roll, trs.Pitch, trs.Yaw, gfx)
{}

Trigger::Trigger(dx::XMFLOAT3 PosTopLeft, dx::XMFLOAT3 PosTopRight,
				 dx::XMFLOAT3 PosBottomLeft, dx::XMFLOAT3 PosBottomRight,
				 float roll, float pitch, float yaw,
				 Graphics& gfx)
	:
	platew(CalcWidth(PosTopLeft.x, PosTopRight.x, PosTopLeft.z, PosTopRight.z)),
	plateh(CalcHeight(PosTopLeft.y, PosBottomLeft.y)),
	plate(gfx, platew, plateh)
{
	triggerPos.triggerPosTopLeft = PosTopLeft;
	triggerPos.triggerPosTopRight = PosTopRight;
	triggerPos.triggerPosBottomLeft = PosBottomLeft;
	triggerPos.triggerPosBottomRight = PosBottomRight;

	triggerOrien.triggerRoll = roll;
	triggerOrien.triggerPitch = pitch;
	triggerOrien.triggerYaw = yaw;
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
	triggerPos.triggerPosTopLeft = PosTopLeft;
	triggerPos.triggerPosTopRight = PosTopRight;
	triggerPos.triggerPosBottomLeft = PosBottomLeft;
	triggerPos.triggerPosBottomRight = PosBottomRight;

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

void Trigger::SetPosition(dx::XMFLOAT3 pos)
{
	plate.SetPos(pos);
}

void Trigger::SetOrientation(dx::XMFLOAT3 orient )
{
	plate.SetRotation(orient.x, orient.y, orient.z);
}

void Trigger::SetDefault()
{
	plate.SetPos(triggerPos.triggerPosTopLeft);
	plate.SetRotation(triggerOrien.triggerRoll, triggerOrien.triggerPitch, triggerOrien.triggerYaw);
}
#endif // IS_ENGINE_MODE

bool Trigger::Check(dx::XMFLOAT3 ObjPos)
{
	return (ObjPos.x >= this->triggerPos.triggerPosTopLeft.x 
		    && ObjPos.x <= this->triggerPos.triggerPosTopLeft.x + deep) &&
		    ObjPos.y <= this->triggerPos.triggerPosTopLeft.y &&
		    ObjPos.z <= this->triggerPos.triggerPosTopLeft.z &&
		    
		    (ObjPos.x >= this->triggerPos.triggerPosTopRight.x
		    && ObjPos.x <= this->triggerPos.triggerPosTopRight.x + deep) &&
		    ObjPos.y <= this->triggerPos.triggerPosTopRight.y &&
		    ObjPos.z >= this->triggerPos.triggerPosTopRight.z &&
		    
		    (ObjPos.x >= this->triggerPos.triggerPosBottomLeft.x
		    && ObjPos.x <= this->triggerPos.triggerPosBottomLeft.x + deep) &&
		    ObjPos.y >= this->triggerPos.triggerPosBottomLeft.y &&
		    ObjPos.z <= this->triggerPos.triggerPosBottomLeft.z &&
		    
		    (ObjPos.x >= this->triggerPos.triggerPosBottomRight.x 
		    && ObjPos.x <= this->triggerPos.triggerPosBottomRight.x + deep) &&
		    ObjPos.y >= this->triggerPos.triggerPosBottomRight.y &&
		    ObjPos.z >= this->triggerPos.triggerPosBottomRight.z;
}

void Trigger::SetDeep(float TriggerDeep)
{
	deep = TriggerDeep;
}

const DirectX::XMFLOAT3* Trigger::GetPosition() const
{
	DirectX::XMFLOAT3 arr[] = 
	{ 
		triggerPos.triggerPosTopLeft, triggerPos.triggerPosTopRight,
		triggerPos.triggerPosBottomLeft, triggerPos.triggerPosBottomRight 
	};

	return arr;
}
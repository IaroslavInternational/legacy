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

Trigger::Trigger(TriggerStruct& trs, Graphics& gfx)
	:
	Trigger(trs.PosTopLeft, trs.PosTopRight, trs.PosBottomLeft, trs.PosBottomRight, trs.Roll, trs.Pitch, trs.Yaw, gfx)
{}

Trigger::Trigger(dx::XMFLOAT3 PosTopLeft, dx::XMFLOAT3 PosTopRight,
				 dx::XMFLOAT3 PosBottomLeft, dx::XMFLOAT3 PosBottomRight,
				 float roll, float pitch, float yaw,
				 Graphics& gfx)
	:
	Gfx(gfx),
	platew(CalcWidth(PosTopLeft.x, PosTopRight.x, PosTopLeft.z, PosTopRight.z)),
	plateh(CalcHeight(PosTopLeft.y, PosBottomLeft.y)),
	plate(Gfx, platew, plateh)
{
	triggerPos.triggerPosTopLeft = PosTopLeft;
	triggerPos.triggerPosTopRight = PosTopRight;
	triggerPos.triggerPosBottomLeft = PosBottomLeft;
	triggerPos.triggerPosBottomRight = PosBottomRight;

	triggerOrien.triggerRoll = roll;
	triggerOrien.triggerPitch = pitch;
	triggerOrien.triggerYaw = yaw;
}

Trigger::~Trigger()
{
}

Plate* Trigger::GetPlate()
{
	return &plate;
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

bool Trigger::Check(dx::XMFLOAT3 CameraPos)
{
	return (CameraPos.x >= this->triggerPos.triggerPosTopLeft.x 
		    && CameraPos.x <= this->triggerPos.triggerPosTopLeft.x + deep) &&
		    CameraPos.y <= this->triggerPos.triggerPosTopLeft.y &&
		    CameraPos.z <= this->triggerPos.triggerPosTopLeft.z &&
		    
		    (CameraPos.x >= this->triggerPos.triggerPosTopRight.x
		    && CameraPos.x <= this->triggerPos.triggerPosTopRight.x + deep) &&
		    CameraPos.y <= this->triggerPos.triggerPosTopRight.y &&
		    CameraPos.z >= this->triggerPos.triggerPosTopRight.z &&
		    
		    (CameraPos.x >= this->triggerPos.triggerPosBottomLeft.x
		    && CameraPos.x <= this->triggerPos.triggerPosBottomLeft.x + deep) &&
		    CameraPos.y >= this->triggerPos.triggerPosBottomLeft.y &&
		    CameraPos.z <= this->triggerPos.triggerPosBottomLeft.z &&
		    
		    (CameraPos.x >= this->triggerPos.triggerPosBottomRight.x 
		    && CameraPos.x <= this->triggerPos.triggerPosBottomRight.x + deep) &&
		    CameraPos.y >= this->triggerPos.triggerPosBottomRight.y &&
		    CameraPos.z >= this->triggerPos.triggerPosBottomRight.z;
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

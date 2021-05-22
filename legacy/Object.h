#pragma once

#include "Graphics.h"

class Object
{
public:
	Object(std::string name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 orientation);
public:
	std::string	GetName() const;
	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMFLOAT3 GetOrientation() const;
	void SetName(std::string name);
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetOrientation(DirectX::XMFLOAT3 orientation);
protected:
	std::string name;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 orientation;
};

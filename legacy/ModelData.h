#pragma once

#include "Model.h"

#include <sstream>
#include <fstream>
#include <filesystem>
#include "json.hpp"

class ModelData
{
public:
	ModelData(const char* path, Graphics& gfx);
	~ModelData();

	void ShowInterface();

	std::vector<std::string> modelsName;
	std::vector<std::unique_ptr<Model>> models;
	std::vector<DirectX::XMFLOAT3> modelsPos;
	std::vector<DirectX::XMFLOAT3> modelsOrien;
};


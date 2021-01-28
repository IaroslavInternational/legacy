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

	// Показать модели *Левая панель*
	void ShowModelsInformation();

	// Показать модели *Правая панель*
	void ShowModelsProperties();
public:
	std::vector<std::string> modelsName;
	std::vector<std::unique_ptr<Model>> models;
	std::vector<DirectX::XMFLOAT3> modelsPos;
	std::vector<DirectX::XMFLOAT3> modelsOrien;
private:
	const char* selected = "";
};


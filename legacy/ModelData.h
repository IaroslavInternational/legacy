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

	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit(size_t channels);
	void AddTestModel(Graphics& gfx);

	// Показать модели *Левая панель*
	void ShowModelsInformation();

	// Показать модели *Правая панель*
	void ShowModelsProperties();
private:
	std::vector<std::string> modelsName;
	std::vector<std::unique_ptr<Model>> models;
	std::vector<DirectX::XMFLOAT3> modelsPos;
	std::vector<DirectX::XMFLOAT3> modelsOrien;
private:
	void Init();
private:
	const char* selected = "";
};


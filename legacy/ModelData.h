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
	void AddModel(Graphics& gfx, Rgph::RenderGraph& rg, const char* path, const char* name);

	// Показать модели *Левая панель*
	void ShowModelsInformation(Graphics& gfx, Rgph::RenderGraph& rg);

	// Показать модели *Правая панель*
	void ShowModelsProperties();
private:
	std::vector<std::string> modelsName;
	std::vector<std::unique_ptr<Model>> models;
	std::vector<DirectX::XMFLOAT3> modelsPos;
	std::vector<DirectX::XMFLOAT3> modelsOrien;
private:
	void Init();
	void InitAt(size_t i);
private:
	/* Данные для новой модели */
	
	mutable char newPath[256];
	mutable char newName[256];

	/***************************/

	const char* path;

	const char* selected = "";
};


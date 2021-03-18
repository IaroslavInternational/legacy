#pragma once

#include "Model.h"

#include <sstream>
#include <fstream>
#include <filesystem>
#include "json.hpp"
#include "AppLog.h"

class ModelData
{
public:
	ModelData(const char* path, Graphics& gfx);
	~ModelData();

	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit(size_t channels);

	// Показать модели *Левая панель*
	void ShowModelsInformation(Graphics& gfx, Rgph::RenderGraph& rg);

	// Показать модели *Правая панель*
	void ShowModelsProperties(AppLog& log);

	void OpenDialog(Graphics& gfx, Rgph::RenderGraph& rg);
private:
	std::vector<std::string> modelsName;
	std::vector<std::unique_ptr<Model>> models;
	std::vector<DirectX::XMFLOAT3> modelsPos;
	std::vector<DirectX::XMFLOAT3> modelsOrien;
private:
	void AddModel(Graphics& gfx, Rgph::RenderGraph& rg, const char* path, const char* name);

	void Init();

	template<typename T>
	void SetNewValue(const char* modelName, const char* param, T val);
private:
	const char* path;

	const char* selected = "";

	bool isSave = false;
};
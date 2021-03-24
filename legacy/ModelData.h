#pragma once

#include "EngineConverter.h"

#include "Model.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#endif

#include <sstream>
#include <fstream>
#include <filesystem>

#include "json.hpp"

class ModelData
{
public:
#if IS_ENGINE_MODE
	ModelData(const char* path, Graphics& gfx, AppLog* aLog);
#else
	ModelData(const char* path, Graphics& gfx);
#endif // IS_ENGINE_MODE
	~ModelData();

	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit(size_t channels);

#if IS_ENGINE_MODE
	// �������� ������ *����� ������*
	void ShowModelsInformation(Graphics& gfx, Rgph::RenderGraph& rg);

	// �������� ������ *������ ������*
	void ShowModelsProperties();

	// ������� ���������� ���� ��� ���������� ������ �� �����
	void OpenDialog(Graphics& gfx, Rgph::RenderGraph& rg);
#endif
private:
	std::vector<std::string> modelsName;			// ����� ������� 
	std::vector<std::unique_ptr<Model>> models;		// ��������� �� ������
	std::vector<DirectX::XMFLOAT3> modelsPos;		// �������  �������
	std::vector<DirectX::XMFLOAT3> modelsOrien;		// ���������� �������
private:
	// �������� ������
	void AddModel(Graphics& gfx, Rgph::RenderGraph& rg, const char* path, const char* name);

	// ���������� ��������� � ���������� ������
	void Init();

	// ������ �������� ��������� ������ �����
	template<typename T>
	void SetNewValue(const char* modelName, const char* param, T val);
private:
	// ���� � ����� � ������� � �������
	const char* path;

#if IS_ENGINE_MODE
	// ��������� ������� � ������ �������
	const char* selected = "";

	// ���� ������ ������ ���������
	bool isSave = false;

	AppLog* applog;
#endif
};
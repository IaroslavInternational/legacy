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

class ModelContainer
{
public:
#if IS_ENGINE_MODE
	ModelContainer(const char* path, Graphics& gfx, AppLog* aLog);
#else
	ModelContainer(const char* path, Graphics& gfx);
#endif // IS_ENGINE_MODE
	~ModelContainer();

	void LinkTechniques(Rgph::RenderGraph& rg);

	void Submit(size_t channels);

	// ��� ������ �� �������
	std::string GetModelNameByIndex(size_t index);

	std::unique_ptr<Model>* GetPtr2ModelByName(std::string name);

#if IS_ENGINE_MODE
	// �������� ������ *����� ������*
	void ShowModelsInformation(Graphics& gfx, Rgph::RenderGraph& rg);

	// �������� ������ *������ ������*
	void ShowModelsProperties();

	// ������� ���������� ���� ��� ���������� ������ �� �����
	void OpenDialog(Graphics& gfx, Rgph::RenderGraph& rg);

	// ���-�� �������
	size_t ModelsAmount();
#endif
private:
	std::vector<std::unique_ptr<Model>> models;		// ��������� �� ������
private:
	// �������� ������
	void LoadModel(std::string name, std::string path, Graphics& gfx, Rgph::RenderGraph& rg);

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
	std::string selected = "";

	// ���� ������ ������ ���������
	bool isSave = false;

	// ���
	AppLog* applog;
#endif
};
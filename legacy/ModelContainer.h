#pragma once

#include "EngineConverter.h"

#include "Model.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#endif

class ModelContainer
{
public:
#if IS_ENGINE_MODE
	ModelContainer(const char* path, Graphics& gfx, Rgph::RenderGraph& rg, AppLog* aLog);
#else
	ModelContainer(const char* path, Graphics& gfx, Rgph::RenderGraph& rg);
#endif // IS_ENGINE_MODE
	~ModelContainer();
public:
	void LinkTechniques();												// �������� � �������
	void Submit(size_t channels);										// �������� � ������ ���������
public:
	std::string GetModelNameByIndex(size_t index);						// ��� ������ �� �������
	std::unique_ptr<Model>* GetPtr2ModelByName(std::string name);		// ��������� �� ������ �� �����
public:
#if IS_ENGINE_MODE
	void ShowLeftPanel();												// �������� ����� ������ ��� �������
	void ShowRightPanel();												// �������� ������ ������ ��� �������
	void OpenDialog();													// ������� ���������� ���� ��� ���������� ������ �� �����
	size_t ModelsAmount();												// ���-�� �������
#endif
private:
	template<typename T>
	void SetNewValue(const char* objectName, const char* param, T val);	// ���������� �������� 
	void LoadModel(std::string name, std::string path);					// �������� ������
	void DeleteModel(std::string name);									// ������� ������
	void Init();														// ���������� ��������� � ���������� ������
private:
	const char* path;													// ���� � ����� � ������� � �������
	Graphics& gfx;														// ����� ������������ ����
	Rgph::RenderGraph& rg;												// ����� ������-�����
	std::vector<std::unique_ptr<Model>> models;							// ��������� �� ������
private:
#if IS_ENGINE_MODE
	std::string selected = "";											// ��������� ������� � ������ �������
	
	bool IsSave = false;												// ���� ������ ������ ���������
	bool IsDelete = false;												// ���� ������ ������ �������
	
	AppLog* applog;														// ���
#endif
};
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
	// Показать модели *Левая панель*
	void ShowModelsInformation(Graphics& gfx, Rgph::RenderGraph& rg);

	// Показать модели *Правая панель*
	void ShowModelsProperties();

	// Открыть диалоговое окно для добавления модели на сцену
	void OpenDialog(Graphics& gfx, Rgph::RenderGraph& rg);
#endif
private:
	std::vector<std::string> modelsName;			// Имена моделей 
	std::vector<std::unique_ptr<Model>> models;		// Указатели на модели
	std::vector<DirectX::XMFLOAT3> modelsPos;		// Позиции  моделей
	std::vector<DirectX::XMFLOAT3> modelsOrien;		// Ориентация моделей
private:
	// Добавить модель
	void AddModel(Graphics& gfx, Rgph::RenderGraph& rg, const char* path, const char* name);

	// Установить положение и ориентацию модели
	void Init();

	// Замена значения параметра модели новым
	template<typename T>
	void SetNewValue(const char* modelName, const char* param, T val);
private:
	// Путь к файлу с данными о моделях
	const char* path;

#if IS_ENGINE_MODE
	// Выбранные элемент в списке моделей
	const char* selected = "";

	// Если нажата кнопка сохранить
	bool isSave = false;

	AppLog* applog;
#endif
};
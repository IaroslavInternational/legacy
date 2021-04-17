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

	// Имя модели по индексу
	std::string GetModelNameByIndex(size_t index);

	std::unique_ptr<Model>* GetPtr2ModelByName(std::string name);

#if IS_ENGINE_MODE
	// Показать модели *Левая панель*
	void ShowModelsInformation(Graphics& gfx, Rgph::RenderGraph& rg);

	// Показать модели *Правая панель*
	void ShowModelsProperties();

	// Открыть диалоговое окно для добавления модели на сцену
	void OpenDialog(Graphics& gfx, Rgph::RenderGraph& rg);

	// Кол-во моделей
	size_t ModelsAmount();
#endif
private:
	std::vector<std::unique_ptr<Model>> models;		// Указатели на модели
private:
	// Добавить модель
	void LoadModel(std::string name, std::string path, Graphics& gfx, Rgph::RenderGraph& rg);

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
	std::string selected = "";

	// Если нажата кнопка сохранить
	bool isSave = false;

	// Лог
	AppLog* applog;
#endif
};
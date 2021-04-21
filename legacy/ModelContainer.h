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
	void LinkTechniques();												// Добавить к рендеру
	void Submit(size_t channels);										// Добавить к каналу отрисовки
public:
	std::string GetModelNameByIndex(size_t index);						// Имя модели по индексу
	std::unique_ptr<Model>* GetPtr2ModelByName(std::string name);		// Указатель на модель по имени
public:
#if IS_ENGINE_MODE
	void ShowLeftPanel();												// Показать левую панель для моделей
	void ShowRightPanel();												// Показать правую панель для моделей
	void OpenDialog();													// Открыть диалоговое окно для добавления модели на сцену
	size_t ModelsAmount();												// Кол-во моделей
#endif
private:
	template<typename T>
	void SetNewValue(const char* objectName, const char* param, T val);	// Установить значение 
	void LoadModel(std::string name, std::string path);					// Добавить модель
	void DeleteModel(std::string name);									// Удалить модель
	void Init();														// Установить положение и ориентацию модели
private:
	const char* path;													// Путь к файлу с данными о моделях
	Graphics& gfx;														// Адрес графичсекого ядра
	Rgph::RenderGraph& rg;												// Адрес рендер-графа
	std::vector<std::unique_ptr<Model>> models;							// Указатели на модели
private:
#if IS_ENGINE_MODE
	std::string selected = "";											// Выбранные элемент в списке моделей
	
	bool IsSave = false;												// Если нажата кнопка сохранить
	bool IsDelete = false;												// Если нажата кнопка удалить
	
	AppLog* applog;														// Лог
#endif
};
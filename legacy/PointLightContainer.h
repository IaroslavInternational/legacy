#pragma once

#include "EngineConverter.h"

#include "PointLight.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#include "CameraContainer.h"
#endif // IS_ENGINE_MODE

#include "BlurOutlineRenderGraph.h"

class PointLightContainer
{
public:
#if IS_ENGINE_MODE
	PointLightContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg, AppLog* aLog);
#else
	PointLightContainer(std::string path, Graphics& gfx);
#endif
	~PointLightContainer();
public:
#if IS_ENGINE_MODE
	void LinkTechniques();										// Добавить к рендеру
	void Submit(size_t channels);								// Добавить к каналу отрисовки
#endif // IS_ENGINE_MODE
	void Bind(DirectX::FXMMATRIX view);							// Обновить данные для шейдера
public:
	void RgBindShadowCamera(Rgph::BlurOutlineRenderGraph& rg);	// Установить главную камеру
	void AddCamerasToLight(CameraContainer* camcon);			// Добавить камеру к ист. освещения
public:
#if IS_ENGINE_MODE
	void ShowLeftPanel();										// Показать левую панель для ист. освещения
	void ShowRightPanel();										// Показать правую панель для ист. освещения
#endif // IS_ENGINE_MODE
private:
	std::vector<std::unique_ptr<PointLight>> pLights;			// Точечные источники освещения
private:
	std::string path;											// Путь к файлу с данными об ист. освещения
	Graphics& gfx;												// Адрес графичсекого ядра
#if IS_ENGINE_MODE
	Rgph::RenderGraph& rg;										// Адрес рендер-графа
#endif // IS_ENGINE_MODE
private:
#if IS_ENGINE_MODE
	std::string selected = "";									// Выбранные элемент в списке ист. освещения
	
	bool IsSave = false;										// Если нажата кнопка сохранить
	bool IsDelete = false;										// Если нажата кнопка удалить
	
	AppLog* applog;												// Лог
#endif // IS_ENGINE_MODE
};
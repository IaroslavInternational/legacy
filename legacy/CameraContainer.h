#pragma once

#include "EngineConverter.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#endif

#include <string>
#include <vector>
#include <memory>

class Camera;
class Graphics;

namespace Rgph
{
	class RenderGraph;
}

class CameraContainer
{
public:
#if IS_ENGINE_MODE
	CameraContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg, AppLog* aLog);
#else
	CameraContainer(std::string path, Graphics& gfx);
#endif // IS_ENGINE_MODE
	~CameraContainer();
public:
	Camera* operator->();											// Указатель на камеру
#if IS_ENGINE_MODE
	void LinkTechniques();											// Добавить к рендеру
#endif // IS_ENGINE_MODE
	void Submit(size_t channels) const;								// Добавить к каналу отрисовки
	void Bind();													// Установка камеры в графическом ядре
public:
	Camera& GetActiveCamera();
	std::shared_ptr<Camera> GetPtr2ActiveCamera();					// Указатель на активную камеру
	std::shared_ptr<Camera> GetPtr2CameraByName(std::string name);	// Указатель на камеру по имени
public:
#if IS_ENGINE_MODE
	size_t CamerasAmount();											// Кол-во камер
	std::string GetCameraNameByIndex(size_t index);					// Имя камеры по индексу в контейнере
#endif // IS_ENGINE_MODE
public:
	void ShowLeftPanel();											// Показать левую панель для камер
	void OpenDialog();												// Открыть диалоговое окно для добавления камеры на сцену
	void AddCamera(std::shared_ptr<Camera> pCam);					// Добавить камеру
private:
	void DeleteCamera(std::string name);							// Удалить камеру
	Camera& GetControlledCamera();									// Адрес управляемой камеры
private:
	std::string path = "";											// Путь к файлу с данными о моделях
	Graphics& gfx;													// Адрес графичсекого ядра
#if IS_ENGINE_MODE
	Rgph::RenderGraph& rg;											// Адрес рендер-графа
#endif // IS_ENGINE_MODE
	std::vector<std::shared_ptr<Camera>> cameras;					// Камеры
private:
	size_t active = 0;												// Индекс активной камеры
	size_t controlled = 0;											// Индекс управляемой камеры
private:
#if IS_ENGINE_MODE
	bool IsSave = false;											// Если нажата кнопка сохранить
	bool IsDelete = false;											// Если нажата кнопка удалить

	AppLog* applog;													// Лог
#endif
};
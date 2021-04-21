#pragma once

#include "Graphics.h"
#include <string>
#include <memory>
#include <filesystem>

#include "Camera.h"

class Node;
class Mesh;
class ModelWindow;

struct aiMesh;
struct aiMaterial;
struct aiNode;

// Рендер граф
namespace Rgph
{
	class RenderGraph;
}

// 3D модель
class Model
{
public:
	// Конструктор
	// Графика, путь к модели, размер
	Model(std::string name, const std::string& path, Graphics& gfx, 
		  DirectX::XMFLOAT3 position =    { 0.0f, 0.0f, 0.0f }, 
		  DirectX::XMFLOAT3 orientation = { 0.0f, 0.0f, 0.0f },
		  float scale = 1.0f);
	~Model() noexcept;
public:
	// Добавить к рендеру
	void LinkTechniques(Rgph::RenderGraph&);

	// Добавить к каналу отрисовки
	void Submit(size_t channels) const noxnd;

	// Создать экземпляр расширенного интерфейса для модели
	void Accept(class ModelProbe& probe);

	// Установка положения
	void SetRootTransform(DirectX::XMMATRIX tf) noexcept;

	void SetScale(float scale);
	
	// Базовый интерфейс управления
	void SpawnDefaultControl();

	std::string GetName();

	void SetRenderStatus(bool status);

	// Получение позиции
	DirectX::XMFLOAT3 GetPosition();

	// Получение ориентации
	DirectX::XMFLOAT3 GetOrientation();

	bool IsCamConnceted();

	// Присоединить камеру
	void ConnectCamera(std::shared_ptr<Camera> cam, DirectX::XMFLOAT3 offset);

	// Отвязать камеру
	void DisconnectCamera();
private:
	// Парсинг меша
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials, const std::filesystem::path& path, float scale);
	
	// Парсинг записей
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node, float scale) noexcept;
private:
	std::unique_ptr<Node> pRoot;
	
	// Делится мэшами опасно?
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
private:
	/* Данные модели */
	
	// Имя модели
	std::string name;

	// Множитель размера
	float scale = 1.0f;

	// Позиция
	DirectX::XMFLOAT3 position;

	// Ориентация
	DirectX::XMFLOAT3 orientation;

	// Разрешена ли  отрисовка
	bool IsRendered = true;

	/*****************/
private:
	/* Привязанная камера */
	
	// Указатель на камеру
	mutable std::shared_ptr<Camera> cam;

	mutable float offset_x = 0.0f;	// Отступ камеры от модели по Оx 
	mutable float offset_y = 0.0f;	// Отступ камеры от модели по Оy
	mutable float offset_z = 0.0f;	// Отступ камеры от модели по Оz
	
	// Если камера привязанна
	bool isCamAdded = false;

	/**********************/
};
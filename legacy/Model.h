#pragma once

#include "VisibleObject.h"

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

namespace Rgph
{
	class RenderGraph;
}

class Model : public VisibleObject
{
public:
	Model(std::string name, const std::string& path, Graphics& gfx, 
		  DirectX::XMFLOAT3 position =    { 0.0f, 0.0f, 0.0f }, 
		  DirectX::XMFLOAT3 orientation = { 0.0f, 0.0f, 0.0f },
		  float scale = 1.0f, bool visibility = true);
	~Model() noexcept;
public:
	void LinkTechniques(Rgph::RenderGraph& rg);				// Добавить к рендеру
	void Submit(size_t channels);							// Добавить к каналу отрисовки
	void Accept(class ModelProbe& probe);					// Создать экземпляр расширенного интерфейса для модели
	void SetScale(float scale);								// Установка размера
	void SetRootTransform(DirectX::XMMATRIX tf) noexcept;	// Установка положения
	bool IsCamConnceted();									// Если присоединена камера
	void SpawnDefaultControl();								// Базовый интерфейс управления
	void ConnectCamera(std::shared_ptr<Camera> cam,	DirectX::XMFLOAT3 offset);			// Присоединить камеру
	void DisconnectCamera();								// Отвязать камеру
private:
	std::unique_ptr<Node> ParseNode(int& nextId, 
		const aiNode& node, float scale) noexcept;			// Парсинг записей
private:
	std::unique_ptr<Node> pRoot;							// Указатель на структуру модели
	std::vector<std::unique_ptr<Mesh>> meshPtrs;			// Указатели на мэш
	float scale = 1.0f;										// Множитель размера
	mutable std::shared_ptr<Camera> cam;					// Указатель на камеру
	mutable float offset_x = 0.0f;							// Отступ камеры от модели по Оx 
	mutable float offset_y = 0.0f;							// Отступ камеры от модели по Оy
	mutable float offset_z = 0.0f;							// Отступ камеры от модели по Оz
	bool isCamAdded = false;								// Если камера привязанна
};
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
	Model(Graphics& gfx, const std::string& pathString, float scale = 1.0f);
	~Model() noexcept;
public:
	void LinkTechniques(Rgph::RenderGraph&);
	void Submit(size_t channels) const noxnd;

	void Accept(class ModelProbe& probe);

	// Установка позиции
	void SetRootTransform(DirectX::FXMMATRIX tf) noexcept;
	
	// Базовый интерфейс управления
	void SpawnDefaultControl();

	// Получение позиции
	DirectX::XMFLOAT3 GetCurrentPosition();

	// Получение ориентации
	DirectX::XMFLOAT3 GetCurrentOrientation();

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
	
	// sharing meshes here perhaps dangerous?!!!
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
private:
	/* Положение */
	
	// Позиция
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };

	// Крен
	float roll = 0.0f;
	
	// Тангаш
	float pitch = 0.0f;
	
	// Рысканье
	float yaw = 0.0f;

	/*************/
private:
	/* Привязанная камера */
	
	// Указатель на камеру
	mutable std::shared_ptr<Camera> cam;

	float offset_x = 0.0f;	// Отступ камеры от модели по Оx 
	float offset_y = 0.0f;	// Отступ камеры от модели по Оy
	float offset_z = 0.0f;	// Отступ камеры от модели по Оz
	
	// Если камера привязанна
	bool isCamAdded = false;

	/**********************/
};
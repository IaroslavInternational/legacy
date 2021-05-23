#pragma once

#include "VisibleObject.h"
#include "Camera.h"

#include <memory>

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

// Класс для описания функционала 3-D моделей. 
class Model : public VisibleObject
{
public:
	// Конструктор модели требует следующие поля:
	// name - имя модели
	// path - путь к объекту
	// gfx - адрес графического ядра
	// position - позиция модели
	// orientation - ориентация модели
	// visibility - видимость модели
	Model(std::string		 name, 
		  const std::string& path, 
		  Graphics&			 gfx, 
		  DirectX::XMFLOAT3	 position =    { 0.0f, 0.0f, 0.0f }, 
		  DirectX::XMFLOAT3	 orientation = { 0.0f, 0.0f, 0.0f },
		  float				 scale =	   1.0f, 
		  bool				 visibility =  true);
	~Model() noexcept;
public:
	// Метод для присоединения модели к Render Graph
	void LinkTechniques(Rgph::RenderGraph& rg);				

	// Метод для присоединения модели к каналу отрисовки
	void Submit(size_t channels);

	// Создать экземпляр расширенного интерфейса для модели
	void Accept(class ModelProbe& probe);

	// Установить размер модели
	void SetScale(float scale);

	// Установить положение в пространстве с помощью матрицы
	void SetRootTransform(DirectX::XMMATRIX tf) noexcept;

	// Отрисовать базовый интерфейс управления
	void SpawnDefaultControl();			

	// Присоединить камеру
	void ConnectCamera(std::shared_ptr<Camera> cam, DirectX::XMFLOAT3 offset);

	// Отвязать камеру
	void DisconnectCamera();		

	// Проверка присоединения камеры
	bool IsCameraConneсted();
private:
	// Парсинг записей (составных частей) модели
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node, float scale) noexcept;
private:
	// Указатель на структуру модели
	std::unique_ptr<Node> pRoot;							

	// Указатели на меши
	std::vector<std::unique_ptr<Mesh>> meshPtrs;			

	// Множитель размера
	float scale = 1.0f;	

	// Указатель на камеру
	mutable std::shared_ptr<Camera> cam;	

	// Отступ камеры от модели по осям
	// 0 - отступ по оси X
	// 1 - отступ по оси Y
	// 2 - отступ по оси Z
	mutable DirectX::XMFLOAT3 offset = { 0.0f, 0.0f, 0.0f };
	
	// Привязанность камеры
	bool CameraConnection = false;
};
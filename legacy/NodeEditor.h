#pragma once

#include "imgui\imnodes.h"

#include "CameraContainer.h"
#include "ModelData.h"

#include <algorithm>
#include <vector>

// Blueprint
class NodeEditor
{
public:
	NodeEditor(CameraContainer& camcon, ModelData& mData);
	~NodeEditor();
public:
	// Инициализация нового кадра
	void BeginFrame();

	// Завершение нового кадра
	void EndFrame();

	// Показать Editor
	void Show();

	// Инициализация
	void Init();
private:
	// Структура блока камеры
	struct CameraNode
	{
		CameraNode(const int i, const char* n)
			:
			id(i),
			name(n)
		{}

		int id;
		const char* name;		
	};

	// Структура блока модели
	struct ModelNode
	{
		ModelNode(const int i, const char* n)
			:
			id(i),
			name(n)
		{}

		int id;
		const char* name;
	};

	// Структура связи блоков
	struct Link
	{
		int id;
		int start_attr;
		int end_attr;
	};
private:
	// Контекст среды разработки
	imnodes::EditorContext* context = nullptr;
	
	// nodes камер
	std::vector<CameraNode> cNodes;

	// nodes моделей
	std::vector<ModelNode> mNodes;
	
	// Связи между node
	std::vector<Link> links;

	// Текущий id в инициализации
	int current_id = 0;

	// Отступ для нового блока камер
	float current_delta_cam = 0.0f;

	// Отступ для нового блока моделей
	float current_delta_model = 0.0f;

	// Статус инициализации
	bool IsInit = false;

	// Статус отрисовки
	bool IsShown = true;

private:
	/* Контейнеры */

	// Адрес контейнера камер
	CameraContainer& camcon;

	// Адрес контейнера моделей
	ModelData& mData;

	// Индекс выбранной камеры
	size_t activeCam = 0;
	
	// Индекс выбранной модели
	size_t activeModel = 0;

	/**************/
private:
	// Отрисовка nodes
	void RenderNodes();

	// Добавить камера node
	void AddCameraNode(int id, const char* name);

	// Добавить модель node
	void AddModelNode(int id, const char* name);

	// Показать модели *Левая панель*
	void ShowLeftPanel(ImVec2 size);

	// Показать модели *Правая панель*
	void ShowRightPanel(ImVec2 size);
};


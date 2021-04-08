#pragma once

#include "imgui\imnodes.h"

#include <algorithm>
#include <vector>

// Blueprint
class NodeEditor
{
public:
	NodeEditor();
	~NodeEditor();
public:
	// Инициализация нового кадра
	void BeginFrame();

	// Завершение нового кадра
	void EndFrame();

	// Показать Editor
	void Show();
private:
	struct Node
	{
		Node(const int i, const float v)
			:
			id(i), value(v)
		{}

		int id;
		float value;
	};

	struct Link
	{
		int id;
		int start_attr;
		int end_attr;
	};

private:
	imnodes::EditorContext* context = nullptr;
	
	std::vector<Node> nodes;
	std::vector<Link> links;

	int current_id = 0;
};


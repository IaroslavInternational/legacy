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
	// ������������� ������ �����
	void BeginFrame();

	// ���������� ������ �����
	void EndFrame();

	// �������� Editor
	void Show();

	// �������������
	void Init();
private:
	// ��������� ����� ������
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

	// ��������� ����� ������
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

	// ��������� ����� ������
	struct Link
	{
		int id;
		int start_attr;
		int end_attr;
	};
private:
	// �������� ����� ����������
	imnodes::EditorContext* context = nullptr;
	
	// nodes �����
	std::vector<CameraNode> cNodes;

	// nodes �������
	std::vector<ModelNode> mNodes;
	
	// ����� ����� node
	std::vector<Link> links;

	// ������� id � �������������
	int current_id = 0;

	// ������ ��� ������ ����� �����
	float current_delta_cam = 0.0f;

	// ������ ��� ������ ����� �������
	float current_delta_model = 0.0f;

	// ������ �������������
	bool IsInit = false;

	// ������ ���������
	bool IsShown = true;

private:
	/* ���������� */

	// ����� ���������� �����
	CameraContainer& camcon;

	// ����� ���������� �������
	ModelData& mData;

	// ������ ��������� ������
	size_t activeCam = 0;
	
	// ������ ��������� ������
	size_t activeModel = 0;

	/**************/
private:
	// ��������� nodes
	void RenderNodes();

	// �������� ������ node
	void AddCameraNode(int id, const char* name);

	// �������� ������ node
	void AddModelNode(int id, const char* name);

	// �������� ������ *����� ������*
	void ShowLeftPanel(ImVec2 size);

	// �������� ������ *������ ������*
	void ShowRightPanel(ImVec2 size);
};


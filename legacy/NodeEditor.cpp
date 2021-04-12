#include "EngineConverter.h"

#if IS_ENGINE_MODE

#include "NodeEditor.h"

#include "imgui\imgui.h"

NodeEditor::NodeEditor(CameraContainer& camcon, ModelData& mData)
    :
    camcon(camcon),
    mData(mData)
{
    context = imnodes::EditorContextCreate();
}

NodeEditor::~NodeEditor()
{
    //imnodes::PopAttributeFlag();
    imnodes::EditorContextFree(context);
}

void NodeEditor::BeginFrame()
{
	imnodes::CreateContext();
}

void NodeEditor::EndFrame()
{
	imnodes::DestroyContext();
}

void NodeEditor::Show(bool *IsShown)
{
    ImGuiIO& io = ImGui::GetIO();

    const float width = 0.85f;
    const float height = 0.85f;

    float PanelW = round(io.DisplaySize.x * width);
    float PanelH = io.DisplaySize.y * height;

    ImVec2 PanelPos = ImVec2(
        io.DisplaySize.x * (1.0f - width) / 2.0f,
        io.DisplaySize.y * (1.0f - height) / 2.0f
    );

    ImGui::SetNextWindowPos(PanelPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(PanelW, PanelH));

    if (ImGui::Begin("�������� �����", IsShown,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_MenuBar))
    {
        // ����
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("��������"))
            {
                if (ImGui::MenuItem("�������"))
                {
                    *IsShown = false;
                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }
        }

        ImGui::EndMenuBar();

        ShowLeftPanel(ImVec2(PanelW * 0.2f, PanelH));

        ImGui::SameLine();

        ShowRightPanel(ImVec2(PanelW * 0.8f, PanelH));
    }

    ImGui::End();
}

void NodeEditor::Init()
{
    if (!IsInit)
    {
        imnodes::Style& style = imnodes::GetStyle();
        style.colors[imnodes::ColorStyle_TitleBar] = IM_COL32(232, 27, 86, 255);
        style.colors[imnodes::ColorStyle_TitleBarSelected] = IM_COL32(241, 108, 146, 255);

        imnodes::GetIO().link_detach_with_modifier_click.modifier = &ImGui::GetIO().KeyShift;
        imnodes::PushAttributeFlag(imnodes::AttributeFlags_EnableLinkDetachWithDragClick);

        IsInit = true;
    }
}

void NodeEditor::RenderNodes()
{
    // ����� �����
    for (auto& node : cNodes)
    {
        ImGui::PushItemWidth(150.0f);
        imnodes::BeginNode(node.id);
     
        // ���������
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(AttachStrings<const char*>("������", node.name));
        imnodes::EndNodeTitleBar();

        // ����
        // ---

        /* ������� */
        
        imnodes::BeginStaticAttribute(node.id << 16);
        ImGui::Text("�������:");
        ImGui::Text("x:"); ImGui::SameLine();
        ImGui::Text(std::to_string(node.pos.x).c_str());
        ImGui::Text("y:"); ImGui::SameLine();
        ImGui::Text(std::to_string(node.pos.y).c_str());
        ImGui::Text("z:"); ImGui::SameLine();
        ImGui::Text(std::to_string(node.pos.z).c_str());
        imnodes::EndStaticAttribute();

        ImGui::NewLine();

        imnodes::BeginStaticAttribute(node.id << 8);
        ImGui::Text("������ ������:");
        ImGui::SliderFloat("������-x", &node.offset.x, -150.0f, 150.0f, "%.3f");
        ImGui::SliderFloat("������-y", &node.offset.y, -150.0f, 150.0f, "%.3f");
        ImGui::SliderFloat("������-z", &node.offset.z, -150.0f, 150.0f, "%.3f");
        imnodes::EndStaticAttribute();
        
        /***********/
        
        // �����
        imnodes::BeginOutputAttribute(node.id << 24);
        const float text_width = ImGui::CalcTextSize("�����").x;
        ImGui::Indent(150.f + ImGui::CalcTextSize("value").x - text_width);
        ImGui::NewLine();
        ImGui::TextUnformatted("�����");
        imnodes::EndOutputAttribute();

        imnodes::EndNode();
        ImGui::PopItemWidth();
    }

    // ����� �������
    for (auto& node : mNodes)
    {
        ImGui::PushItemWidth(150.0f);
        imnodes::BeginNode(node.id);

        // ���������
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(AttachStrings<const char*>("������", node.name));
        imnodes::EndNodeTitleBar();

        // �������
        imnodes::BeginStaticAttribute(node.id << 16);
        ImGui::Text("�������:");
        ImGui::Text("x:"); ImGui::SameLine();
        ImGui::Text(std::to_string(node.pos.x).c_str());
        ImGui::Text("y:"); ImGui::SameLine();
        ImGui::Text(std::to_string(node.pos.y).c_str());
        ImGui::Text("z:"); ImGui::SameLine();
        ImGui::Text(std::to_string(node.pos.z).c_str());
        imnodes::EndStaticAttribute();

        // ����
        imnodes::BeginInputAttribute(node.id << 8);
        ImGui::NewLine();
        ImGui::TextUnformatted("������");
        imnodes::EndInputAttribute();

        // �����
        // --

        imnodes::EndNode();
        ImGui::PopItemWidth();
    }

    for (auto& link : links)
    {
        imnodes::Link(link.id, link.start_attr, link.end_attr);
    }
}

void NodeEditor::AddCameraNode(int id, const char* name)
{
    current_delta_cam += 10.0f;

    imnodes::SetNodeEditorSpacePos(id, ImVec2(0.0f, current_delta_cam));
    cNodes.emplace_back(CameraNode(id, name, camcon.GetPtr2CameraByName(name)->GetPos()));
}

void NodeEditor::AddModelNode(int id, const char* name)
{
    current_delta_model += 10.0f;

    imnodes::SetNodeEditorSpacePos(id, ImVec2(100.0f, current_delta_model));
    mNodes.emplace_back(ModelNode(id, name, mData.GetPtr2ModelByName(name)->get()->GetCurrentPosition()));
}

void NodeEditor::ShowLeftPanel(ImVec2 size)
{
    ImGui::BeginChild("�������� ����� | ����� ������", size);

    // ������ �����
    if (ImGui::BeginCombo("������", camcon->GetName().c_str()))
    {
        for (size_t i = 0; i < camcon.CamerasAmount(); i++)
        {
            const bool isSelected = i == activeCam;
            if (ImGui::Selectable(camcon.GetCameraNameByIndex(i), isSelected))
            {
                activeCam = i;
                AddCameraNode(i, camcon.GetCameraNameByIndex(i));
            }
        }
        ImGui::EndCombo();
    }

    // ������ �������
    if (ImGui::BeginCombo("������", mData.GetModelNameByIndex(0)))
    {
        for (size_t i = 0; i < mData.ModelsAmount(); i++)
        {
            const bool isSelected = i == activeModel;
            if (ImGui::Selectable(mData.GetModelNameByIndex(i), isSelected))
            {
                activeModel = i;
                AddModelNode(i + 100, mData.GetModelNameByIndex(i));
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("���������"))
    {
        if (links.size() != 0)
        {
            for (auto& link : links)
            {
                int link_start = link.start_attr;   // cam out id
                int link_end = link.end_attr;       // mod in id

                int camId = link.start_attr >> 24;  // cam id
                int modId = link.end_attr >> 8;     // mod id

                if (!mData.GetPtr2ModelByName(FindModNodeById(modId)->name)->get()->IsCamConnceted())
                {
                    mData.GetPtr2ModelByName(FindModNodeById(modId)->name)->get()->ConnectCamera(
                        camcon.GetPtr2CameraByName(FindCamNodeById(camId)->name), 
                        DirectX::XMFLOAT3(
                            FindCamNodeById(camId)->offset.x,
                            FindCamNodeById(camId)->offset.y,
                            FindCamNodeById(camId)->offset.z
                    ));
                }
            }
        }
    }

    ImGui::EndChild();
}

void NodeEditor::ShowRightPanel(ImVec2 size)
{
    imnodes::EditorContextSet(context);

    ImGui::BeginChild("�������� ����� | ������ ������", size);

    imnodes::BeginNodeEditor();

    Init();

    RenderNodes();

    imnodes::EndNodeEditor();

    {
        Link link;
        if (imnodes::IsLinkCreated(&link.start_attr, &link.end_attr))
        {
            link.id = ++current_id;
            links.push_back(link);
        }
    }

    {
        int link_id;        
        if (imnodes::IsLinkDestroyed(&link_id))
        {
            auto iter = std::find_if(
                links.begin(), links.end(), [link_id](const Link& link) -> bool {
                return link.id == link_id;
            });

            assert(iter != links.end());
            links.erase(iter);
        }
    }

    ImGui::EndChild();
}

NodeEditor::CameraNode* NodeEditor::FindCamNodeById(int id)
{
    for (auto& node : cNodes)
    {
        if (node.id == id)
        {
            return &node;
        }
    }
}

NodeEditor::ModelNode* NodeEditor::FindModNodeById(int id)
{
    for (auto& node : mNodes)
    {
        if (node.id == id)
        {
            return &node;
        }
    }
}

template<typename T>
const char* NodeEditor::AttachStrings(T str1, T str2)
{
    std::ostringstream oss;
    oss << str1 << " " << str2;

    return oss.str().c_str();
}

#endif // IS_ENGINE_MODE
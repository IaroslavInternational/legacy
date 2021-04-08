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

void NodeEditor::Show()
{
    if (IsShown)
    {
        ImGuiIO& io = ImGui::GetIO();

        const float width = 0.85f;
        const float height = 0.85f;

        float PanelW = round(io.DisplaySize.x * width);
        float PanelH = io.DisplaySize.y * height;

        ImGui::SetNextWindowSize(ImVec2(PanelW, PanelH));

        if (ImGui::Begin("Node Editor", NULL,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Editor"))
                {
                    if (ImGui::MenuItem("Закрыть"))
                    {
                        IsShown = false;
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
}

void NodeEditor::Init()
{
    if (!IsInit)
    {
        imnodes::Style& style = imnodes::GetStyle();
        style.colors[imnodes::ColorStyle_TitleBar] = IM_COL32(232, 27, 86, 255);
        style.colors[imnodes::ColorStyle_TitleBarSelected] = IM_COL32(241, 108, 146, 255);

        imnodes::PushAttributeFlag(imnodes::AttributeFlags_EnableLinkDetachWithDragClick);

        imnodes::IO& io = imnodes::GetIO();
        io.link_detach_with_modifier_click.modifier = &ImGui::GetIO().KeyCtrl;

        IsInit = true;
    }
}

void NodeEditor::RenderNodes()
{
    for (auto& node : cNodes)
    {
        imnodes::BeginNode(node.id);

        // Заголовок
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node.name);
        imnodes::EndNodeTitleBar();

        // Вход
        // ---

        // Контент
        imnodes::BeginStaticAttribute(node.id << 16);
        ImGui::PushItemWidth(80.0f);
        // smth
        ImGui::PopItemWidth();
        imnodes::EndStaticAttribute();

        // Выход
        imnodes::BeginOutputAttribute(node.id << 24);
        const float text_width = ImGui::CalcTextSize("Выход").x;
        ImGui::Indent(80.f + ImGui::CalcTextSize("value").x - text_width);
        ImGui::TextUnformatted("Выход");
        imnodes::EndOutputAttribute();

        imnodes::EndNode();
    }

    for (auto& node : mNodes)
    {
        imnodes::BeginNode(node.id);

        // Заголовок
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node.name);
        imnodes::EndNodeTitleBar();

        // Вход
        imnodes::BeginInputAttribute(node.id << 8);
        ImGui::TextUnformatted("Камера");
        imnodes::EndInputAttribute();

        // Контент
        imnodes::BeginStaticAttribute(node.id << 16);
        ImGui::PushItemWidth(120.0f);
        // smth
        ImGui::PopItemWidth();
        imnodes::EndStaticAttribute();

        // Выход
        // --

        imnodes::EndNode();
    }

    for (const Link& link : links)
    {
        imnodes::Link(link.id, link.start_attr, link.end_attr);
    }
}

void NodeEditor::AddCameraNode(int id, const char* name)
{
    current_delta_cam += 10.0f;

    imnodes::SetNodeEditorSpacePos(id, ImVec2(0.0f, current_delta_cam));
    cNodes.push_back(CameraNode(id, name));
}

void NodeEditor::AddModelNode(int id, const char* name)
{
    current_delta_model += 10.0f;

    imnodes::SetNodeEditorSpacePos(id, ImVec2(100.0f, current_delta_model));
    mNodes.push_back(ModelNode(id, name));
}

void NodeEditor::ShowLeftPanel(ImVec2 size)
{
    ImGui::BeginChild("Node Panel", size);

    // Список камер
    if (ImGui::BeginCombo("Камеры", camcon->GetName().c_str()))
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

    // Список моделей
    if (ImGui::BeginCombo("Модели", mData.GetModelNameByIndex(0)))
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

    ImGui::EndChild();
}

void NodeEditor::ShowRightPanel(ImVec2 size)
{
    imnodes::EditorContextSet(context);

    ImGui::BeginChild("Node Editor", size);

    imnodes::BeginNodeEditor();

    /*
    // Добавление node
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
        imnodes::IsEditorHovered() && ImGui::IsKeyReleased('A'))
    {
        AddNode();
    }
    */

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

            assert(iter != editor.links.end());
            links.erase(iter);
        }
    }

    ImGui::EndChild();
}
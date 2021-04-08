#include "NodeEditor.h"

#include "imgui\imgui.h"

NodeEditor::NodeEditor()
{
    context = imnodes::EditorContextCreate();
    /* imnodes::PushAttributeFlag(imnodes::AttributeFlags_EnableLinkDetachWithDragClick);

    imnodes::IO& io = imnodes::GetIO();
    io.link_detach_with_modifier_click.modifier = &ImGui::GetIO().KeyCtrl;*/
}

NodeEditor::~NodeEditor()
{
    imnodes::PopAttributeFlag();
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
    imnodes::EditorContextSet(context);

    ImGui::Begin("Node Editor");
    ImGui::TextUnformatted("A -- add node");

    imnodes::BeginNodeEditor();

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
        imnodes::IsEditorHovered() && ImGui::IsKeyReleased('A'))
    {
        const int node_id = ++current_id;
        imnodes::SetNodeScreenSpacePos(node_id, ImGui::GetMousePos());
        nodes.push_back(Node(node_id, 0.f));
    }

    for (Node& node : nodes)
    {
        imnodes::BeginNode(node.id);

        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("node");
        imnodes::EndNodeTitleBar();

        imnodes::BeginInputAttribute(node.id << 8);
        ImGui::TextUnformatted("input");
        imnodes::EndInputAttribute();

        imnodes::BeginStaticAttribute(node.id << 16);
        ImGui::PushItemWidth(120.0f);
        ImGui::DragFloat("value", &node.value, 0.01f);
        ImGui::PopItemWidth();
        imnodes::EndStaticAttribute();

        imnodes::BeginOutputAttribute(node.id << 24);
        const float text_width = ImGui::CalcTextSize("output").x;
        ImGui::Indent(120.f + ImGui::CalcTextSize("value").x - text_width);
        ImGui::TextUnformatted("output");
        imnodes::EndOutputAttribute();

        imnodes::EndNode();
    }

    for (const Link& link : links)
    {
        imnodes::Link(link.id, link.start_attr, link.end_attr);
    }

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

    ImGui::End();
}
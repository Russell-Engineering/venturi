#include "panels/NodeEditor.h"
#include "imnodes/imnodes.h"
#include "nodes.h"


namespace Venturi
{

    NodeEditor::NodeEditor()
        : Panel()
    {
        PushNode(new SampleNode(true));
        PushNode(new SampleNode(true));
    }
    NodeEditor::NodeEditor(const std::string& name, bool open, UI* parent)
        : Panel(), m_Parent(parent)
    {
        PushNode(new SampleNode(true));
        PushNode(new SampleNode(true));
    }

    void NodeEditor::PushLocalStyle()
    {
        ImNodesStyle& style = ImNodes::GetStyle();
        style.GridSpacing = 10.0f; 
        style.NodeCornerRounding = 0.0f;

        style.Colors[ImNodesCol_TitleBar] = IM_COL32(38, 38, 38, 204);
        style.Colors[ImNodesCol_TitleBarHovered] = IM_COL32(154, 185, 208, 255);
        style.Colors[ImNodesCol_TitleBarSelected] = IM_COL32(12,44, 68, 255);
            
        style.Colors[ImNodesCol_NodeBackground] = IM_COL32(51, 52, 51, 204);
        style.Colors[ImNodesCol_NodeBackgroundHovered] = IM_COL32(51, 52, 51, 255);
        style.Colors[ImNodesCol_NodeBackgroundSelected] = IM_COL32(51, 52, 51, 255);

    }

    void NodeEditor::OnUIRender(const char* name, bool& open)
    {
        //UI::m_PanelManager->GetPanelName(UI::NODE_EDITOR);
        ImGui::Begin(name, &open);
        ImNodes::BeginNodeEditor();
        for (Oak::Node* node : m_NodeStack)
        {
            node->SetLocalStyle();
            if (node->Visibility()) node->OnUIRender(node->p_open);

        }
        std::vector<std::pair<int, int>> links;
        // elsewhere in the code...
        for (int i = 0; i < links.size(); ++i)
        {
            const std::pair<int, int> p = links[i];
            // in this case, we just use the array index of the link
            // as the unique identifier
            ImNodes::Link(i, p.first, p.second);
        }


        ImNodes::MiniMap();
        ImNodes::EndNodeEditor();
        ImGui::End();

    }

    void NodeEditor::AddNode()
    {

    }
    
    void NodeEditor::RemoveNode(uint32_t id)
    {

    }

    void NodeEditor::PushNode(Oak::Node* node)
    {
        m_NodeStack.PushNode(node);
    }

}
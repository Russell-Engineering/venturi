#include "panels/NodeEditor.h"
#include "imnodes/imnodes.h"
#include "nodes.h"


namespace Venturi
{
    NodeEditor::NodeEditor(const std::string& name, bool open, UI* parent)
        : Panel(name, open), m_Parent(parent)
    {
        PushNode(new SampleNode(true));
        PushNode(new SampleNode(true));
    }

    void NodeEditor::SetLocalStyle()
    {

    }

    void NodeEditor::OnUIRender(bool* p_open)
    {
        ImGui::Begin(GetName().c_str());
        ImNodes::BeginNodeEditor();
        for (Oak::Node* node : m_NodeStack)
        {
            SetLocalStyle();
            node->SetLocalStyle();
            if (node->Visibility()) node->OnUIRender(node->p_open);
        }
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
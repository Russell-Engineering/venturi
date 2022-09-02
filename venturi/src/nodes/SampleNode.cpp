#include "nodes/SampleNode.h"
#include "imnodes/imnodes.h"

namespace Venturi
{
    SampleNode::SampleNode(bool open)
        : Oak::Node(open)
    {
    }

    void SampleNode::SetLocalStyle(){}

    void SampleNode::OnUIRender(bool* p_open)
    {
        ImNodes::BeginNode(GetID());

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("output node");
        ImNodes::EndNodeTitleBar();


        ImGui::Dummy(ImVec2(80.0f, 45.0f));
        ImNodes::BeginInputAttribute(++m_PinID);
        // in between Begin|EndAttribute calls, you can call ImGui
        // UI functions
        ImGui::Text("Input pin");
        ImNodes::EndOutputAttribute();
        ImNodes::BeginOutputAttribute(++m_PinID);
        // in between Begin|EndAttribute calls, you can call ImGui
        // UI functions
        ImGui::Text("output pin");
        ImNodes::EndOutputAttribute();



        //ImNodes::SnapNodeToGrid(GetID());




        ImNodes::EndNode();

    }
}
#pragma once

#include "oak.h"

namespace Venturi
{
    class NodeEditor : public Oak::Panel
    {
    public:
        NodeEditor();
        ~NodeEditor() = default;

        void PushLocalStyle() override;
        void OnUIRender(const char* name, bool& open) override;

        void AddNode();
        void PushNode(Oak::Node* Node);
        void RemoveNode(uint32_t id);
        void AllignVertical(std::vector<Oak::Node*> nodes);
        void AllignHorizontal(std::vector<Oak::Node*> nodes);
        void DrawToolBar();

        Oak::Node* GetNode(uint32_t id);


    private:
        inline static uint32_t m_NodeCount = 0;
        Oak::NodeStack m_NodeStack;




    };

}
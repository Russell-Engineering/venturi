#pragma once

#include "oak.h"
#include "UI.h"

namespace Venturi
{
    class NodeEditor : public Oak::Panel
    {
    public:
        NodeEditor(const std::string& name, bool open, UI* parent);
        ~NodeEditor() = default;

        void SetLocalStyle() override;
        void OnUIRender(bool* p_open) override;

        void AddNode();
        void PushNode(Oak::Node* Node);
        void RemoveNode(uint32_t id);
        void AllignVertical(std::vector<Oak::Node*> nodes);
        void AllignHorizontal(std::vector<Oak::Node*> nodes);
        

        Oak::Node* GetNode(uint32_t id);
        UI* GetParent() { return m_Parent; }


    private:
        inline static uint32_t m_NodeCount = 0;
        UI* m_Parent = nullptr;
        Oak::NodeStack m_NodeStack;




    };

}
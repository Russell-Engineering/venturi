#pragma once

#include "oak.h"
#include "UI.h"


namespace Venturi
{
    class DemoPanels : public Oak::Panel
    {
    public:
        DemoPanels(const std::string& name, bool open, UI* parent);
        ~DemoPanels() = default;

        void SetLocalStyle() override;
        void OnUIRender(bool* open) override;
        UI* GetParent() { return m_Parent; }
    
    private:
        UI* m_Parent = nullptr;
    };
}
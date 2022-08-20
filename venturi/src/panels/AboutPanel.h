#pragma once

#include "oak.h"
#include "UI.h"

namespace Venturi
{

    class AboutPanel : public Oak::Panel
    {
    public:
        AboutPanel(const std::string& name, bool show, UI* parent);
        ~AboutPanel() = default;

        void SetLocalStyle() override;
        void OnUIRender(bool* open) override;
        UI* GetParent() { return m_Parent; }
    
    private:
        UI* m_Parent = nullptr;

    };
}
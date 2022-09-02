#pragma once

#include "oak.h"
#include "UI.h"

namespace Venturi
{

    class AboutPanel : public Oak::Panel
    {
    public:
        AboutPanel() {}
        ~AboutPanel() {}
        //AboutPanel(const std::string& name, bool show, UI* parent);

        void PushLocalStyle() override;
        void OnUIRender(const char* name, bool& open) override;
        //UI* GetParent() { return m_Parent; }
    
    private:
        UI* m_Parent = nullptr;

    };
}
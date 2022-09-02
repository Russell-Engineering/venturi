#pragma once

#include "oak.h"
#include "UI.h"

namespace Venturi
{
    class OptionsPanel : public Oak::Panel
    {
    public:
        OptionsPanel() {}
        OptionsPanel(const std::string& name, bool open, UI* parent);
        ~OptionsPanel() = default;

        void PushLocalStyle() override;
        void OnUIRender(const char* name, bool& open) override;

        UI* GetParent() { return m_Parent; }

    private:
        UI* m_Parent;
    };

}
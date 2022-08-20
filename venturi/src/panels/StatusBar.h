#pragma once

#include "oak.h"
#include "UI.h"

namespace Venturi
{
    class StatusBar : public Oak::Panel
    {
    public:
        StatusBar(const std::string& name, bool open,  UI* parent);
        ~StatusBar() = default;

        void SetLocalStyle() override;
        void OnUIRender(bool* open) override;
        UI* GetParent() { return m_Parent;  }

    private:
        UI* m_Parent;
        int m_temp_counter;

    };

}

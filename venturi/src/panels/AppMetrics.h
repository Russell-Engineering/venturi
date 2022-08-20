#pragma once

#include "oak.h"
#include "UI.h"

namespace Venturi
{
    class AppMetrics : public Oak::Panel
    {
    public:
        AppMetrics(const std::string name, bool open, UI* parent);
        ~AppMetrics() = default;

        void SetLocalStyle();
        void OnUIRender(bool* p_open);
    private:
        UI* m_Parent;

    };
}
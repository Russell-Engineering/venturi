#pragma once

#include "oak.h"
#include "UI.h"

namespace Venturi
{
    class AppMetrics : public Oak::Panel
    {
    public:
        AppMetrics() {}
        AppMetrics(const std::string name, bool open, UI* parent);
        ~AppMetrics() = default;

        void PushLocalStyle();
        void OnUIRender(const char* name, bool& open);
    private:
        UI* m_Parent;

    };
}
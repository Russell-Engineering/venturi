#include "panels/AppMetrics.h"
#include "ImGui/imgui_demo.cpp"

namespace Venturi
{
    AppMetrics::AppMetrics(const std::string name, bool open, UI* parent)
        : Panel(), m_Parent(parent)
    {

    }

    void AppMetrics::PushLocalStyle()
    {
    }

    void AppMetrics::OnUIRender(const char* name, bool& open)
    {
        ImGui::ShowMetricsWindow(&open);
    }
}
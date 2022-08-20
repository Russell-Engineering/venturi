#include "panels/AppMetrics.h"
#include "ImGui/imgui_demo.cpp"

namespace Venturi
{
    AppMetrics::AppMetrics(const std::string name, bool open, UI* parent)
        : Panel(name, open), m_Parent(parent)
    {

    }

    void AppMetrics::SetLocalStyle()
    {
    }

    void AppMetrics::OnUIRender(bool* p_open)
    {
        ImGui::ShowMetricsWindow(p_open);
    }
}
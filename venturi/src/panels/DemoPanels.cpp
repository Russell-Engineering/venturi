
#include "panels/DemoPanels.h"
#include "ImGui/imgui.h"
#include "implot/implot.h"

namespace Venturi
{

    void DemoPanels::PushLocalStyle()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 5.0f)); m_StylePopCount++;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f)); m_StylePopCount++;
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f); m_StylePopCount++;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); m_StylePopCount++;
    }

    void DemoPanels::OnUIRender(const char*, bool& open)
    {
        ImGui::ShowDemoWindow(&open);
        ImPlot::ShowDemoWindow(&open);
    }

}
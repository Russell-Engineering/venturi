
#include "panels/DemoPanels.h"
#include "ImGui/imgui.h"
#include "implot/implot.h"

namespace Venturi
{
    DemoPanels::DemoPanels(const std::string& name, bool open, UI* parent)
        : Oak::Panel(name, open), m_Parent(parent)
    {
        OAK_TRACE("Creating Demo Panel named {} with id {}", GetName(), GetID());
    }

    void DemoPanels::SetLocalStyle()
    {
		ImGuiStyle& style = ImGui::GetStyle();
		style.FramePadding = ImVec2(5.0f, 5.0f);
		style.FrameBorderSize = 0.0f;
		style.WindowPadding = ImVec2(5.0f, 5.0f);
		style.WindowBorderSize = 0.0f;
    }

    void DemoPanels::OnUIRender(bool* open)
    {
        ImGui::ShowDemoWindow(open);
        ImPlot::ShowDemoWindow(open);

    }

}
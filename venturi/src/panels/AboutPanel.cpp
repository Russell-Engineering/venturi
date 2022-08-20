#include "panels/AboutPanel.h"
#include "ImGui/imgui.h"

namespace Venturi
{
    AboutPanel::AboutPanel(const std::string& name, bool show, UI* parent)
        : Oak::Panel(name, show), m_Parent(parent)
    {
        OAK_TRACE("Creating AboutPanel Panel named {} with id {}", GetName(), GetID());
    }

    void AboutPanel::SetLocalStyle()
    {
		ImGuiStyle& style = ImGui::GetStyle();
		style.FramePadding = ImVec2(5.0f, 1.0f);
		style.FrameBorderSize = 0.0f;
		style.WindowPadding = ImVec2(5.0f, 1.0f);
		style.WindowBorderSize = 0.0f;
    }

    void AboutPanel::OnUIRender(bool* open)
    {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowBgAlpha(0.8f);	
		if (ImGui::BeginPopupModal(GetName().c_str(), open))
		{
			ImGui::Text("Version: 0.1.0 (dAugust-2020)");
			ImGui::Separator();
			ImGui::Text("Icons by https://icons8.com");
			ImGui::EndPopup();
		}
        if (*open)
            ImGui::OpenPopup(GetName().c_str());
    }

}
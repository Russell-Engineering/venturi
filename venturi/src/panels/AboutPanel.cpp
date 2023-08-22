#include "panels/AboutPanel.h"
#include "ImGui/imgui.h"

namespace wi::Venturi
{
    void AboutPanel::PushLocalStyle()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 5.0f)); m_StylePopCount++;
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f); m_StylePopCount++;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f)); m_StylePopCount++;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); m_StylePopCount++;
    }

    void AboutPanel::OnUIRender(const char* name, bool& open)
    {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowBgAlpha(0.8f);	
		if (ImGui::BeginPopupModal(name, &open))
		{
			ImGui::Text("Version: 0.1.0 (dAugust-2020)");
			ImGui::Separator();
			ImGui::Text("Icons by https://icons8.com");
			ImGui::EndPopup();
		}
        if (open)
            ImGui::OpenPopup(name);
    }
}
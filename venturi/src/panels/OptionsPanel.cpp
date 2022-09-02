#include "panels/OptionsPanel.h"
#include "imgui/imgui.h"

namespace Venturi
{
    OptionsPanel::OptionsPanel(const std::string& name, bool open, UI* parent)
        : Panel(), m_Parent(parent)
    {
    }

    void OptionsPanel::PushLocalStyle()
    {
    }

    void OptionsPanel::OnUIRender(const char* name, bool& p_open)
    {
        ImGui::Begin(name, &p_open);
        
        if (ImGui::CollapsingHeader("SETTINGS"))
        {
            ImGui::Separator();
            ImGui::CollapsingHeader("Global Plot Settings");
            
            ImGui::Separator();
            ImGui::CollapsingHeader("Network Settings");

            ImGui::Separator();
            
            if(ImGui::CollapsingHeader("App Styling", ImGuiTreeNodeFlags_None))
            {
                //static const char* current_item = NULL;
                //if (ImGui::BeginCombo("##combo", NULL, ImGuiWindowFlags_None))
                //{
                //    for (auto& [id, panel] : GetParent()->GetPanelManager())
                //    {   
                //        std::string name = panel->GetName();
                //        uint32_t id = panel->GetID();
                //        OAK_WARN("Panel {}:{}", id, name);
                //        bool is_selected = (current_item == name.c_str());
                //        if (ImGui::Selectable(name.c_str(), is_selected))
                //            current_item = name.c_str();
                //        if (is_selected)
                //            ImGui::SetItemDefaultFocus();
                //    }
                //    ImGui::EndCombo();
                //}

            }
        }


        ImGui::End();
    }

}
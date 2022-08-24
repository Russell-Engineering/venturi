#include "panels/SideBar.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

namespace Venturi
{
    SideBar::SideBar(const std::string& name, bool open, UI* parent)
        : Oak::Panel(name, open), m_Parent(parent)
    {

        OAK_TRACE("Creating SideBar Panel named {} with id {}", GetName(), GetID());
        m_MenuIcon = Oak::Texture2D::Create("assets/textures/icons8-menu-32.png");
        m_ExplorerIcon = Oak::Texture2D::Create("assets/textures/icons8-documents-32.png");
        m_ConnectionIcon = Oak::Texture2D::Create("assets/textures/icons8-broadcasting-32.png");
        m_ModuleIcon = Oak::Texture2D::Create("assets/textures/module.png");
        m_ChartIcon = Oak::Texture2D::Create("assets/textures/icons8-combo-chart-32.png");
        m_ConsoleIcon = Oak::Texture2D::Create("assets/textures/icons8-console-32.png");
        m_AddPlotIcon = Oak::Texture2D::Create("assets/textures/icons8-add-new-32.png");
        m_SettingsIcon = Oak::Texture2D::Create("assets/textures/icons8-services-32.png");
    }

    void SideBar::SetLocalStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.FramePadding = ImVec2(10.0f, 10.0f);
        style.WindowPadding = ImVec2(1.0f, 1.0f);
        style.ItemSpacing = ImVec2(5.0f, 5.0f);
        style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_MenuBarBg];
        style.Colors[ImGuiCol_WindowBg] = style.Colors[ImGuiCol_MenuBarBg];
    }

    void SideBar::OnUIRender(bool* open)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGuiWindowFlags ui_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;
        ImGuiStyle& style = ImGui::GetStyle();
        if (ImGui::BeginViewportSideBar(GetName().c_str(), viewport, ImGuiDir_Left, 47.0f, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar))
        {
            float size = ImGui::GetContentRegionAvail().x - 10.0f;
            if (ImGui::BeginMenuBar())
            {

                /*float size_y = ImGui::GetFrameHeight() - 25.0f;
                ImGui::SetCursorPosY((ImGui::GetFrameHeight()- size_y) * 0.5f);
                */

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)m_MenuIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    OAK_INFO("OPEN CONNECTION POPUP/PANEL");
                    //GetParent()->GetPanel("Menu")->Toggle();
                }
                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

            }
            ImGui::EndMenuBar();
            {
                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)m_ExplorerIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    GetParent()->GetPanel("Explorer")->Toggle();
                    OAK_WARN("{}", GetParent()->GetPanel("Explorer")->Visibility());
                }

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                {
                    ImGui::SetTooltip("Ctrl+Shit+E");
                }

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)m_ConnectionIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    GetParent()->GetPanel("Configuration")->Toggle();
                    //ImGui::SetWindowFocus("Testing Module");
                }

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)m_ChartIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    //if (m_show_simple_plot)
                    //	ImGui::SetWindowFocus("plot1");
                    //else
                    //	m_show_simple_plot = !m_show_simple_plot;
                }

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)m_ConsoleIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    GetParent()->GetPanel("Log")->Toggle();
                    //m_show_app_log = !m_show_app_log;
                }

                ImGui::SetCursorPosY((ImGui::GetWindowContentRegionMax().y) - 2*(size + ImGui::GetStyle().FramePadding.y));
                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)m_AddPlotIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    GetParent()->AddPlot();
                }

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)m_SettingsIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    GetParent()->GetPanel("About")->Show();
                }
            }
        }
        ImGui::End();
    }
}
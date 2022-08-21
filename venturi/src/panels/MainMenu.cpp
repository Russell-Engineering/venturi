#include "panels/MainMenu.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

namespace Venturi
{
    MainMenu::MainMenu(const std::string& name, bool open, UI* parent)
        : Oak::Panel(name, open), m_Parent(parent)
    {
        OAK_TRACE("Creating MainMenu Panel named {} with id {}", GetName(), GetID());
        m_AppLogo = Oak::Texture2D::Create("assets/textures/icons8-opium-poppy-24.png");
        m_CloseIcon = Oak::Texture2D::Create("assets/textures/icons8-close-16.png");
        m_MinimizeIcon = Oak::Texture2D::Create("assets/textures/icons8-subtract-16.png");
        m_RestoreIcon = Oak::Texture2D::Create("assets/textures/icons8-restore-down-16.png");
        m_MaximizeIcon = Oak::Texture2D::Create("assets/textures/icons8-maximize-button-16.png");
    }

    void MainMenu::SetLocalStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.FramePadding = ImVec2(7.0f, 7.0f);
        style.WindowPadding = ImVec2(5.0f, 5.0f);
        style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_MenuBarBg];
    }

    void MainMenu::OnUIRender(bool* open)
    {
        
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGuiWindowFlags ui_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;
        ImGuiStyle& style = ImGui::GetStyle();
        ImGuiIO& io = ImGui::GetIO();

        if (ImGui::BeginViewportSideBar("##MainMenu", viewport, ImGuiDir_Up, ImGui::GetFrameHeight(), ui_flags))
        {       
                if (ImGui::IsWindowFocused() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                    ImVec2 current_pos = io.MousePos;
                    ImVec2 last_pos = GetParent()->GetLastMousePos();
                    ImVec2 delta_mouse = io.MouseDelta;
                    float x = current_pos.x - GetParent()->GetRelativeMousePos().x;
                    float y = current_pos.y - GetParent()->GetRelativeMousePos().y;


                    if (Oak::Application::Get().IsMaximized())
                    {
                         // if the window is maximized, add in a buffer to prevent accidentally restoring the window
                        //if ((std::abs(delta_mouse.x) > 10 || std::abs(delta_mouse.y) > 10))
                        if ((std::abs(delta_mouse.x) > 10 || std::abs(delta_mouse.y) > 10))
                        {
                            Oak::Application::Get().GetWindow().Restore();
                            Oak::Application::Get().GetWindow().MoveDelta(delta_mouse.x, delta_mouse.y);
                            //Oak::Application::Get().GetWindow().Move(x, y);
                        }
                    } else {
                        Oak::Application::Get().GetWindow().MoveDelta(delta_mouse.x, delta_mouse.y);
                        //Oak::Application::Get().GetWindow().Move(x, y);
                    }

                    //Oak::Application::Get().GetWindow().Move(current_pos.x- clicked_pos.x, current_pos.y - clicked_pos.y);
                }

                if (ImGui::IsWindowFocused() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                        if (Oak::Application::Get().IsMaximized()) Oak::Application::Get().GetWindow().Restore();
                        else if (Oak::Application::Get().IsRestored()) Oak::Application::Get().GetWindow().Maximize();
                }


            if (ImGui::BeginMenuBar())
            {
                int pad = 8; // padding around the image texture displayed on the button
                float size = ImGui::GetFrameHeight() - 2.0f * pad; // want the button to fill the frame so the image size is the frame height - 2x padding
                ImGui::SetCursorPosX(0.0f);
                ImGui::ImageButton((ImTextureID)m_AppLogo->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad);

                if (ImGui::BeginMenu("FILE"))
                {
                    if (ImGui::MenuItem("New", "Ctrl+N"))
                        GetParent()->NewFile();
                    if (ImGui::MenuItem("Open...", "Ctrl+O"))
                        GetParent()->OpenFile();
                    if (ImGui::MenuItem("Save", "Ctrl+S"))
                        GetParent()->SaveFile();
                    if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                        GetParent()->SaveFileAs();
                    if (ImGui::MenuItem("Close"))
                        GetParent()->CloseFile();
                    ImGui::Separator();
                    if (ImGui::BeginMenu("Recent Files"))
                    {
                        ImGui::MenuItem("example_file_1.csv");
                        // for each file in recent file stack ...
                        ImGui::EndMenu();
                    }
                    ImGui::Separator();
                    if (ImGui::MenuItem("Exit", "Alt+F4"))
                        Oak::Application::Get().Close();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("VIEW"))
                {
                    for (Oak::Panel* panel : GetParent()->GetPanelStack()->GetPanels())
                    {
                        std::stringstream ss;
                        if (panel->GetID() < 10)
                        {
                            ss << "ctrl + " << panel->GetID() % 10;
                        }
                        OAK_WARN("Adding Panel {}:{} to menu with state {}", panel->GetID(), panel->GetName(), *panel->p_open);
                        ImGui::MenuItem(panel->GetName().c_str(), ss.str().c_str(), panel->p_open);
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("OPTIONS"))
                {
                    bool vsync = Oak::Application::Get().GetWindow().IsVSync();
                    ImGui::MenuItem("VSync                ", NULL, &vsync);
                    Oak::Application::Get().GetWindow().SetVSync(vsync);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("TOOLS"))
                {
                    ImGui::MenuItem("App Metrics", "", GetParent()->GetPanel("Metrics")->p_open);
                    ImGui::EndMenu();
                }

                // window control buttons with no spacing
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
                // reposition cursor to draw the right side
                ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 3.0f * (size + 2.0f * pad));

                if (ImGui::ImageButton((ImTextureID)m_MinimizeIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
                {
                    // Oak::Application::Minimize()	Oak::Application::Get().GetWindow().Maximize();glfwIconifyWindow((GLFWwindow*)Oak::Application::Get().GetWindow().GetNativeWindow());
                    Oak::Application::Get().GetWindow().Minimize();
                }
                if (Oak::Application::Get().IsRestored())
                {
                    if (ImGui::ImageButton((ImTextureID)m_MaximizeIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
                        Oak::Application::Get().GetWindow().Maximize();
                }
                if (Oak::Application::Get().IsMaximized())
                {
                    if (ImGui::ImageButton((ImTextureID)m_RestoreIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
                        Oak::Application::Get().GetWindow().Restore();
                }
                if (ImGui::ImageButton((ImTextureID)m_CloseIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
                    Oak::Application::Get().Close();
                ImGui::PopStyleVar(); // ItemSpacing
                ImGui::EndMenuBar();
            }
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                OAK_INFO("Item clicked");
            }
            ImGui::End();
        }

    }
}
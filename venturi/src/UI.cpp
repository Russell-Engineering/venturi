#include "imgui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "implot/implot.h"
#include "imnodes/imnodes.h"

#include "oak/utilities/FileSystem.h"


#include "UI.h"
#include "panels.h"
#include "resources.h"

#include "oak/serialization/ImGuiStyleSerializer.h"

//temporary
#include "glad/include/glad/glad.h"
#include "GLFW/glfw3.h"




namespace Venturi
{

#define MAIN_MENU "MainMenu"
#define SIDEBAR "Sidebar"
#define STATUS_BAR "StatusBar"

#define IMGUI_DEMO_PANEL "ImGuiDemo"
#define FILE_EXPLORER_PANEL "FileExplorer"
#define EMBEDDED_TERMINAL "Terminal"
#define APPLICATION_METRICS_PANEL "ApplicationMetrics"
#define ABOUT_PANEL "AboutPanel"
#define NODE_EDITOR "NodeEditor"
#define SETTINGS_PANEL "SettingsPanel"

#define SHOW true
#define HIDE false
        

    void DrawCanvas()
    {
        uint32_t height = Oak::Application::Get().GetWindow().GetHeight();
        uint32_t width = Oak::Application::Get().GetWindow().GetWidth();

        float spacing = 1.0f / 60.0f;
        float dx = spacing;
        float dy = spacing * width / height;
        glLineWidth(0.01f);
        for (float i = -1.0f; i < 1.0f; i += dx)
        {

            glBegin(GL_LINES);
            glColor4f(1.0, 1.0, 1.0, 0.5);
            glVertex2f(i, -1);
            glVertex2f(i, 1);
            glEnd();

        }
        for (float j = -1.0f; j < 1.0f; j += dy)
        {
            glBegin(GL_LINES);
            glColor4f(1.0, 1.0, 1.0, 0.5);
            glVertex2f(-1, j);
            glVertex2f(1, j);
            glEnd();
        }

    }

    UI::UI()
        : Layer("VENTURI UI LAYER"), m_plotcount(0), m_Style(ImGui::GetStyle())
    {
       
    }

    void UI::OnAttach()
    {
        Resources::Init();

        Oak::ImGuiStyleSerializer styleSerializer = Oak::ImGuiStyleSerializer(m_Style);
        if (!styleSerializer.Deserialize("assets/styles/default.style"))
            OAK_ERROR("could not serialize style");
        
        ImGuiStyle& style = ImGui::GetStyle();
        style = m_Style;
        OAK_WARN_TAG("UI::OnAttach", "TAB ROUNDING {}", m_Style.TabRounding);

        // panels
        m_PanelManager = Oak::CreateScope<Oak::PanelManager>();

        m_PanelManager->AddPanel<ExplorerPanel>(Oak::PanelCategory::VIEW, FILE_EXPLORER_PANEL, "EXPLORER", HIDE);
        m_PanelManager->AddPanel<LogPanel>(Oak::PanelCategory::VIEW, EMBEDDED_TERMINAL, "LOG", HIDE);

        //m_PanelManager->AddPanel<OptionsPanel>(Oak::PanelCategory::EDIT, SETTINGS_PANEL, "Project Settings", false);
        m_PanelManager->AddPanel<OptionsPanel>(Oak::PanelCategory::EDIT, SETTINGS_PANEL, "SETTINGS", HIDE);
        m_PanelManager->AddPanel<NodeEditor>(Oak::PanelCategory::EDIT, NODE_EDITOR, "SETUP", HIDE);
        //m_PanelManager->AddPanel<OptionsPanel>(Oak::PanelCategory::EDIT, SETTINGS_PANEL, "User Preferences", false);

       // m_PanelManager->AddPanel<AppMetrics>(Oak::PanelCategory::TOOLS, APPLICATION_METRICS_PANEL, "APP METRICS", HIDE);
        //m_PanelManager->AddPanel<DemoPanels>(Oak::PanelCategory::TOOLS, IMGUI_DEMO_PANEL, "DEMOS", HIDE);
        
        m_PanelManager->AddPanel<AboutPanel>(Oak::PanelCategory::HELP, ABOUT_PANEL, "ABOUT", HIDE);
    }

    void UI::OnDetach()
    {
        Resources::Shutdown();
    }

    void UI::SetGlobalStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style = m_Style;
    }

    void UI::OnUpdate(Oak::Timestep ts)
    {

        // todo:    windows snapping if dragging to edges of screen
        //          resize with drag
        //          improve drag moving update speed. maybe the whole thing should live in the OnUpdate?
        //          fixed canvas spacing regardless of window size. move canvas to renderer 
        // 
        //UpdateWindowPos();
        Oak::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Oak::RenderCommand::Clear();
        //DrawCanvas();

        ImGuiIO& io = ImGui::GetIO();
        m_LastMousePos = io.MousePos;
        SetRelativeMousePos();

        if (Oak::Application::Get().IsRestored())
        {
            m_RestoredWidth = Oak::Application::Get().GetWindow().GetWidth();
        }


    }

    void UI::OnUIRender()
    {
        SetGlobalStyle();
       
        DrawMenu();
        DrawStatusBar();
        DrawSidebar();

        m_PanelManager->OnUIRender();
        SetGlobalStyle();
    }

    void UI::OnEvent(Oak::Event& e)
    {
        Oak::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Oak::KeyPressedEvent>(OAK_BIND_EVENT_FN(UI::OnKeyPressed));
        dispatcher.Dispatch<Oak::MouseButtonPressedEvent>(OAK_BIND_EVENT_FN(UI::OnMouseButtonPressed));
        dispatcher.Dispatch<Oak::MouseButtonReleasedEvent>(OAK_BIND_EVENT_FN(UI::OnMouseButtonReleased));
    }

    void UI::AddPlot()
    {
        std::stringstream ss;
        ss.str(std::string());
        ss << "Plot-" << m_plotcount++;
        OAK_INFO("Adding new plot {}", ss.str());

        m_PanelManager->AddPanel<RTPlot>(Oak::PanelCategory::VIEW, ss.str().c_str(), true);
    }

    void UI::SetRelativeMousePos()
    {
        ImGuiIO& io = ImGui::GetIO();

        float x = ImGui::GetIO().MousePos.x;
        float y = ImGui::GetIO().MousePos.y;
        int win_w = Oak::Application::Get().GetWindow().GetWidth();
        int win_h = Oak::Application::Get().GetWindow().GetHeight();
        int win_x = Oak::Application::Get().GetWindow().GetPosX();
        int win_y = Oak::Application::Get().GetWindow().GetPosY();

        m_RelativeMousePos = ImVec2((float)(x - win_x) / (float)win_w, (float)(y - win_y) / (float)win_h);



    }

    bool UI::OnMouseButtonPressed(Oak::MouseButtonPressedEvent& e)
    {
        //SetRelativeMousePos(ImGui::GetIO().MousePos.x - Oak::Application::Get().GetWindow().GetPosX(), ImGui::GetIO().MousePos.y - Oak::Application::Get().GetWindow().GetPosY());
        SetRelativeMousePos();
        return false;
    }

    bool UI::OnMouseButtonReleased(Oak::MouseButtonReleasedEvent& e)
    {
        SetLastWindowPos(Oak::Application::Get().GetWindow().GetPosX(), Oak::Application::Get().GetWindow().GetPosX());
        return false;
    }

    bool UI::OnKeyPressed(Oak::KeyPressedEvent& e)
    {

        //todo: F11 for pseudo full screent (hide toolbars)
        //      ctrl+d0-d9 for toggling focus on pannels
        //      windows snapping with superkey
        //      

        if (e.IsRepeat())
            return false;

        bool control = Oak::Input::IsKeyPressed(Oak::Key::LeftControl) || Oak::Input::IsKeyPressed(Oak::Key::RightControl);
        bool shift = Oak::Input::IsKeyPressed(Oak::Key::LeftShift) || Oak::Input::IsKeyPressed(Oak::Key::RightShift);
        bool alt = Oak::Input::IsKeyPressed(Oak::Key::LeftAlt) || Oak::Input::IsKeyPressed(Oak::Key::RightAlt);

        switch (e.GetKeyCode())
        {

        case Oak::Key::N:
        {
            if (control) NewFile();
            break;
        }
        case Oak::Key::O:
        {
            if (control) OpenFile();
            break;
        }
        case Oak::Key::S:
        {
            if (control)
            {
                if (shift) SaveFileAs();
                else SaveFile();
            }
            break;
        }
        case Oak::Key::E:
        {
            if (control && shift) m_PanelManager->TogglePanel(FILE_EXPLORER_PANEL);
            break;
        }

        case Oak::Key::Escape:
        {
            ImGui::CloseCurrentPopup();
            break;
        }
        }
        return false;
    }

    void UI::NewFile()
    {
        OAK_INFO("New File");
    }

    void UI::OpenFile()
    {
        std::filesystem::path filepath = Oak::FileSystem::OpenFileDialog("All Files (*.*)\0*.*\0");
        if (!filepath.empty())
        {
            // handle the open file here
            //m_CurrentWorkingFile = filepath;
            OAK_INFO("Open File {}", filepath);
            // m_current_filepath 
        }
        else {
            OAK_WARN("File dialog canceled");
        }
    }

    void UI::SaveFile()
    {
        // if (!m_current_filepath.empty()) 
            // save...
        SaveFileAs();
    }

    void UI::SaveFileAs()
    {
        std::filesystem::path filepath = Oak::FileSystem::SaveFileDialog("All Files (*.*)\0*.*\0");
        if (!filepath.empty())
        {
            OAK_INFO("Save File As {}", filepath);
            // handle the save file here

        }
        else {
            OAK_WARN("File dialog canceled");
        }
    }

    void UI::CloseFile()
    {
        //if (!m_CurrentWorkingFile.empty())
        //{
        //	OAK_INFO("Closeing File {}", m_CurrentWorkingFile);
        //	m_CurrentWorkingFile.clear();

        //}
    }
    
    void UI::UpdateWindowPos()
    {
        // if the right part of the frame is clicke
        uint32_t current_x = Oak::Application::Get().GetWindow().GetPosX();
        uint32_t current_y = Oak::Application::Get().GetWindow().GetPosY();

        ImGuiIO& io = ImGui::GetIO();
        ImVec2 mouse_clicked = io.MouseClickedPos[ImGuiMouseButton_Left];
        ImVec2 mouse_position = io.MousePos;

        uint32_t offset_x = current_x-mouse_clicked.x;
        uint32_t offset_y = current_y-mouse_clicked.y;

        uint32_t dx = mouse_position.x - offset_x;
        uint32_t dy = mouse_position.y - offset_y;

        Oak::Application::Get().GetWindow().Move(dx, dy);
    }

    void UI::DrawMenu()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGuiIO& io = ImGui::GetIO();

        int style_pop_count = 0;
        int color_pop_count = 0;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(7.0f, 7.0f)); style_pop_count++;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f)); style_pop_count++;

        ImGui::PushStyleColor(ImGuiCol_Button, m_Style.Colors[ImGuiCol_MenuBarBg]); color_pop_count++;
      
        
        ImGuiWindowFlags ui_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;

        if (ImGui::BeginViewportSideBar("##MAINMENU", viewport, ImGuiDir_Up, ImGui::GetFrameHeight(), ui_flags))
        {       
                if (ImGui::IsWindowFocused() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                    ImVec2 current_pos = io.MousePos;
                    ImVec2 last_pos = GetLastMousePos();
                    ImVec2 delta_mouse = io.MouseDelta;
                    float x = GetRelativeMousePos().x * (float) GetRestoredWidth();
                    float y = GetRelativeMousePos().y * (float) GetRestoredHeight();


                    if (Oak::Application::Get().IsMaximized())
                    {
                         // if the window is maximized, add in a buffer to prevent accidentally restoring the window
                        //if ((std::abs(delta_mouse.x) > 10 || std::abs(delta_mouse.y) > 10))
                        if ((std::abs(delta_mouse.x) > 10 || std::abs(delta_mouse.y) > 10))
                        {
                            OAK_WARN("{}, {}", x, y);
                            Oak::Application::Get().GetWindow().Restore();
                            Oak::Application::Get().GetWindow().Move((int)(current_pos.x -x), (int)(current_pos.y)-y);
                           // Oak::Application::Get().GetWindow().MoveDelta(delta_mouse.x, delta_mouse.y);
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
                ImGui::ImageButton((ImTextureID)Resources::AppLogo->GetRendererID(), ImVec2(24, 16), ImVec2(0, 0), ImVec2(1, 1), pad);

                if (ImGui::BeginMenu("FILE"))
                {
                    if (ImGui::MenuItem("New", "Ctrl+N"))
                        NewFile();
                    if (ImGui::MenuItem("Open...", "Ctrl+O"))
                        OpenFile();
                    if (ImGui::MenuItem("Save", "Ctrl+S"))
                        SaveFile();
                    if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                        SaveFileAs();
                    if (ImGui::MenuItem("Close"))
                        CloseFile();
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
                    for (auto& [id, panelData] : m_PanelManager->GetPanels(Oak::PanelCategory::VIEW))
                    {

                        OAK_WARN("Adding Panel {}:{} to menu with state {}", panelData.ID, panelData.Name, panelData.IsOpen);
                        ImGui::MenuItem(panelData.Name, nullptr, &panelData.IsOpen);
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
                    for (auto& [id, panelData] : m_PanelManager->GetPanels(Oak::PanelCategory::TOOLS))
                    {

                        OAK_WARN("Adding Panel {}:{} to menu with state {}", panelData.ID, panelData.Name, panelData.IsOpen);
                        ImGui::MenuItem(panelData.Name, nullptr, &panelData.IsOpen);
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("HELP"))
                {
                    for (auto& [id, panelData] : m_PanelManager->GetPanels(Oak::PanelCategory::HELP))
                    {

                        OAK_WARN("Adding Panel {}:{} to menu with state {}", panelData.ID, panelData.Name, panelData.IsOpen);
                        ImGui::MenuItem(panelData.Name, nullptr, &panelData.IsOpen);
                    }
                    ImGui::EndMenu();
                }

                // window control buttons with no spacing
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
                // reposition cursor to draw the right side
                ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 3.0f * (size + 2.0f * pad));

                if (ImGui::ImageButton((ImTextureID)Resources::MinimizeIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
                {
                    Oak::Application::Get().GetWindow().Minimize();
                }
                if (Oak::Application::Get().IsRestored())
                {
                    if (ImGui::ImageButton((ImTextureID)Resources::MaximizeIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
                        Oak::Application::Get().GetWindow().Maximize();
                }
                if (Oak::Application::Get().IsMaximized())
                {
                    if (ImGui::ImageButton((ImTextureID)Resources::RestoreIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
                        Oak::Application::Get().GetWindow().Restore();
                }
                if (ImGui::ImageButton((ImTextureID)Resources::CloseIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
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

        ImGui::PopStyleVar(style_pop_count);
        ImGui::PopStyleColor(color_pop_count);


    }

    void UI::DrawSidebar()
    {
        int style_pop_count = 0;
        int color_pop_count = 0; 
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));    style_pop_count++;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));     style_pop_count++;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));       style_pop_count++;
        ImGui::PushStyleColor(ImGuiCol_Button, m_Style.Colors[ImGuiCol_MenuBarBg]); color_pop_count++;
        ImGui::PushStyleColor(ImGuiCol_WindowBg, m_Style.Colors[ImGuiCol_MenuBarBg]); color_pop_count++;

        
        
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGuiWindowFlags ui_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;
        
        if (ImGui::BeginViewportSideBar("##SIDEBAR", viewport, ImGuiDir_Left, 47.0f, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar))
        {
            float size = ImGui::GetContentRegionAvail().x - 10.0f;
            if (ImGui::BeginMenuBar())
            {

                /*float size_y = ImGui::GetFrameHeight() - 25.0f;
                ImGui::SetCursorPosY((ImGui::GetFrameHeight()- size_y) * 0.5f);
                */

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)Resources::MenuIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    //GetParent()->GetPanel("Menu")->Toggle();
                }
                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

            }
            ImGui::EndMenuBar();
            {
                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)Resources::ExplorerIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    //GetParent()->GetPanel("Explorer")->Toggle();
                    m_PanelManager->TogglePanel(FILE_EXPLORER_PANEL);
                    //OAK_WARN("{}", GetParent()->GetPanel("Explorer")->Visibility());
                }

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                {
                    ImGui::SetTooltip("Ctrl+Shit+E");
                }

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)Resources::ConnectionIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    if (!m_PanelManager->IsPanelOpen(NODE_EDITOR))
                        m_PanelManager->TogglePanel(NODE_EDITOR);
                    
                    ImGui::SetWindowFocus(m_PanelManager->GetPanelName(NODE_EDITOR));
                    //GetParent()->GetPanel("Configuration")->Toggle();
                    //ImGui::SetWindowFocus("Testing Module");
                }

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)Resources::ChartIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    //if (m_show_simple_plot)
                    //	ImGui::SetWindowFocus("plot1");
                    //else
                    //	m_show_simple_plot = !m_show_simple_plot;
                }

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)Resources::ConsoleIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    //GetParent()->GetPanel("Log")->Toggle();
                    //m_show_app_log = !m_show_app_log;
                }

                ImGui::SetCursorPosY((ImGui::GetWindowContentRegionMax().y) - 2 * (size + ImGui::GetStyle().FramePadding.y));
                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)Resources::AddPlotIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    //GetParent()->AddPlot();
                }

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (ImGui::ImageButton((ImTextureID)Resources::SettingsIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
                {
                    //GetParent()->GetPanel("Settings")->Toggle();
                }
            }
        ImGui::End();
        }
      
        ImGui::PopStyleVar(style_pop_count);
        ImGui::PopStyleColor(color_pop_count);
    
    }

    void UI::DrawStatusBar()
    {

        int style_pop_count = 0;
        int color_pop_count = 0;

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f)); style_pop_count++;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f)); style_pop_count++;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f)); style_pop_count++;


        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGuiWindowFlags ui_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground;
        if (ImGui::BeginViewportSideBar("##STATUSBAR", viewport, ImGuiDir_Down, ImGui::GetFrameHeight(), ui_flags))
        {
            if (ImGui::BeginMenuBar())
            {
                ImGui::Text("File: ");
                //if (!m_CurrentWorkingFile.empty()) ImGui::Text(m_CurrentWorkingFile.c_str());
                //else ImGui::Text("No file loaded ...");
                std::string status_text = "Status: ";// +m_StatusStr;
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(status_text.c_str()).x - 7.0f);
                ImGui::Text(status_text.c_str());

                ImGui::EndMenuBar();
            }
            ImGui::End();
        }

        ImGui::PopStyleVar(style_pop_count);
        ImGui::PopStyleColor(color_pop_count);

    }

}
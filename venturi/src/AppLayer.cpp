#include "imgui/imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"
#include "implot/implot.h"
#include "imnodes/imnodes.h"

#include "oak/utilities/FileSystem.h"
#include "oak/console/ConsolePanel.h"


#include "AppLayer.h"
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
#define PLOT_PANEL(n) "Plot-" ##n

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

    AppLayer::AppLayer()
        : Layer("VENTURI AppLayer LAYER"), m_plotcount(0), m_Style(ImGui::GetStyle())
    {
       
    }

    void AppLayer::OnAttach()
    {
        Resources::Init();

        UpdateStyle();

        // panels
        m_PanelManager = Oak::CreateScope<Oak::PanelManager>();

        m_PanelManager->AddPanel<ExplorerPanel>(Oak::PanelCategory::VIEW, FILE_EXPLORER_PANEL, "EXPLORER", HIDE);
        m_PanelManager->AddPanel<Oak::ConsolePanel>(Oak::PanelCategory::VIEW, EMBEDDED_TERMINAL, "LOG", SHOW);
        m_PanelManager->AddPanel<PlotPanel>(Oak::PanelCategory::VIEW, PLOT_PANEL("1"), "PLOT-1", SHOW);

        //m_PanelManager->AddPanel<OptionsPanel>(Oak::PanelCategory::EDIT, SETTINGS_PANEL, "Project Settings", false);
        m_PanelManager->AddPanel<OptionsPanel>(Oak::PanelCategory::EDIT, SETTINGS_PANEL, "SETTINGS", HIDE);
        m_PanelManager->AddPanel<NodeEditor>(Oak::PanelCategory::EDIT, NODE_EDITOR, "SETUP", HIDE);
        //m_PanelManager->AddPanel<OptionsPanel>(Oak::PanelCategory::EDIT, SETTINGS_PANEL, "User Preferences", false);

        m_PanelManager->AddPanel<AppMetrics>(Oak::PanelCategory::TOOLS, APPLICATION_METRICS_PANEL, "APP METRICS", HIDE);
        m_PanelManager->AddPanel<DemoPanels>(Oak::PanelCategory::TOOLS, IMGUI_DEMO_PANEL, "DEMOS", HIDE);
        
        m_PanelManager->AddPanel<AboutPanel>(Oak::PanelCategory::HELP, ABOUT_PANEL, "ABOUT", HIDE);

        OAK_TRACE("Testing OAK_TRACE");
        OAK_CORE_TRACE("Testing OAK_CORE_TRACE");
        OAK_INFO("Testing OAK_INFO");
        OAK_WARN("Testing OAK_WARN");
        OAK_ERROR("Testing OAK_ERROR");
        OAK_FATAL_TAG("TEST TAG", "Testing OAK_FATAL");



    }

    void AppLayer::OnDetach()
    {
        Resources::Shutdown();
    }

    void AppLayer::SetGlobalStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style = m_Style;
    }

    void AppLayer::UpdateStyle()
    {
        Oak::ImGuiStyleSerializer styleSerializer = Oak::ImGuiStyleSerializer(m_Style);
        if (!styleSerializer.Deserialize("assets/styles/default.style"))
            OAK_ERROR("could not serialize style");

        ImGuiStyle& style = ImGui::GetStyle();
        style = m_Style;
    }

    void AppLayer::OnUpdate(Oak::Timestep ts)
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

    void AppLayer::OnUIRender()
    {
        SetGlobalStyle();
       
        DrawMenu();
        DrawStatusBar();
        DrawSidebar();

        m_PanelManager->OnUIRender();
        SetGlobalStyle();
        Oak::UI::RenderMessageBoxes();
    }

    void AppLayer::OnEvent(Oak::Event& e)
    {
        Oak::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Oak::KeyPressedEvent>(OAK_BIND_EVENT_FN(AppLayer::OnKeyPressed));
        dispatcher.Dispatch<Oak::MouseButtonPressedEvent>(OAK_BIND_EVENT_FN(AppLayer::OnMouseButtonPressed));
        dispatcher.Dispatch<Oak::MouseButtonReleasedEvent>(OAK_BIND_EVENT_FN(AppLayer::OnMouseButtonReleased));
    }

    void AppLayer::AddPlot()
    {
       
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowBgAlpha(0.8f);
        bool open = true;
        bool apply = false;
        std::string plotName;
        if (ImGui::BeginPopupModal("Add New Plotting Workspace", &open))
        {
            ImGui::InputTextWithHint("##PlotName", "Enter workspace name ...", &plotName);
            ImGui::Separator();
            if (ImGui::Button("Apply"))
                apply = true;
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (open)
            ImGui::OpenPopup("Add New Plotting Workspace");
        if (apply)
        {
            m_PanelManager->AddPanel<PlotPanel>(Oak::PanelCategory::VIEW, plotName.c_str(), plotName.c_str(), SHOW);
            ImGui::CloseCurrentPopup();
        }
    }

    void AppLayer::SetRelativeMousePos()
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

    bool AppLayer::OnMouseButtonPressed(Oak::MouseButtonPressedEvent& e)
    {
        //SetRelativeMousePos(ImGui::GetIO().MousePos.x - Oak::Application::Get().GetWindow().GetPosX(), ImGui::GetIO().MousePos.y - Oak::Application::Get().GetWindow().GetPosY());
        SetRelativeMousePos();
        return false;
    }

    bool AppLayer::OnMouseButtonReleased(Oak::MouseButtonReleasedEvent& e)
    {
        SetLastWindowPos(Oak::Application::Get().GetWindow().GetPosX(), Oak::Application::Get().GetWindow().GetPosX());
        return false;
    }

    bool AppLayer::OnKeyPressed(Oak::KeyPressedEvent& e)
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

    void AppLayer::NewFile()
    {
        OAK_INFO("New File");
    }

    void AppLayer::OpenFile()
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

    void AppLayer::SaveFile()
    {
        // if (!m_current_filepath.empty()) 
            // save...
        SaveFileAs();
    }

    void AppLayer::SaveFileAs()
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

    void AppLayer::CloseFile()
    {
        //if (!m_CurrentWorkingFile.empty())
        //{
        //	OAK_INFO("Closeing File {}", m_CurrentWorkingFile);
        //	m_CurrentWorkingFile.clear();

        //}
    }
    
    void AppLayer::UpdateWindowPos()
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

    void AppLayer::DrawMenu()
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
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_Style.Colors[ImGuiCol_MenuBarBg]);
                //Oak::UI::ImageButton(Resources::AppLogo, ImVec2(24, 16), pad);
                ImGui::ImageButton((ImTextureID)Resources::AppLogo->GetRendererID(), ImVec2(24, 16), ImVec2(0, 0), ImVec2(1, 1), pad);
                ImGui::PopStyleColor();

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
                        ImGui::MenuItem(panelData.Name, nullptr, &panelData.IsOpen);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("OPTIONS"))
                {
                    bool vsync = Oak::Application::Get().GetWindow().IsVSync();
                    ImGui::MenuItem("VSync                ", NULL, &vsync);
                    Oak::Application::Get().GetWindow().SetVSync(vsync);

                    if (ImGui::MenuItem("Reload Styles"))
                        UpdateStyle();

                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("TOOLS"))
                {
                    for (auto& [id, panelData] : m_PanelManager->GetPanels(Oak::PanelCategory::TOOLS))
                        ImGui::MenuItem(panelData.Name, nullptr, &panelData.IsOpen);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("HELP"))
                {
                    for (auto& [id, panelData] : m_PanelManager->GetPanels(Oak::PanelCategory::HELP))
                        ImGui::MenuItem(panelData.Name, nullptr, &panelData.IsOpen);
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
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.92f, 0.25f, 0.20f, 0.5f));
                if (ImGui::ImageButton((ImTextureID)Resources::CloseIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
                    Oak::Application::Get().Close();
                ImGui::PopStyleColor();
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

    void AppLayer::DrawSidebar()
    {
       
        int style_pop_count = 0;
        int color_pop_count = 0; 
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));    style_pop_count++;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));     style_pop_count++;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));       style_pop_count++;
        ImGui::PushStyleColor(ImGuiCol_Button, m_Style.Colors[ImGuiCol_MenuBarBg]); color_pop_count++;
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_Style.Colors[ImGuiCol_MenuBarBg]); color_pop_count++;
        ImGui::PushStyleColor(ImGuiCol_WindowBg, m_Style.Colors[ImGuiCol_MenuBarBg]); color_pop_count++;

        
        
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGuiWindowFlags ui_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;
        
        float sidebar_width = m_SideBarExpanded ? 147.0f : 47.0f;

        if (ImGui::BeginViewportSideBar("##SIDEBAR", viewport, ImGuiDir_Left, sidebar_width, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar))
        {
            //float size = ImGui::GetContentRegionAvail().x - 10.0f;
            float size = 37.0f;
            if (ImGui::BeginMenuBar())
            {
                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (Oak::UI::ImageButton(Resources::MenuIcon, ImVec2(Resources::MenuIcon->GetWidth(), Resources::MenuIcon->GetHeight())))
                {
                    m_SideBarExpanded ^= true;
                    //GetParent()->GetPanel("Menu")->Toggle();
                }
                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

            }
            ImGui::EndMenuBar();
            {
                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (Oak::UI::ImageButton(Resources::ExplorerIcon, ImVec2(Resources::ExplorerIcon->GetWidth(), Resources::ExplorerIcon->GetHeight())))
                    m_PanelManager->TogglePanel(FILE_EXPLORER_PANEL);

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                    ImGui::SetTooltip("Ctrl+Shit+E");

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (Oak::UI::ImageButton(Resources::ConnectionIcon, ImVec2(Resources::ConnectionIcon->GetWidth(), Resources::ConnectionIcon->GetHeight())))
                {
                    if (!m_PanelManager->IsPanelOpen(NODE_EDITOR))
                        m_PanelManager->TogglePanel(NODE_EDITOR);
                    
                    ImGui::SetWindowFocus(m_PanelManager->GetPanelName(NODE_EDITOR));
                    //GetParent()->GetPanel("Configuration")->Toggle();
                    //ImGui::SetWindowFocus("Testing Module");
                }

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (Oak::UI::ImageButton(Resources::ChartIcon, ImVec2(Resources::ChartIcon->GetWidth(), Resources::ChartIcon->GetHeight())))
                {
                    //if (m_show_simple_plot)
                    //	ImGui::SetWindowFocus("plot1");
                    //else
                    //	m_show_simple_plot = !m_show_simple_plot;
                }

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (Oak::UI::ImageButton(Resources::ConsoleIcon, ImVec2(Resources::ConsoleIcon->GetWidth(), Resources::ConsoleIcon->GetHeight())))
                    m_PanelManager->TogglePanel(EMBEDDED_TERMINAL);

                ImGui::SetCursorPosY((ImGui::GetWindowContentRegionMax().y) - 2 * (size + ImGui::GetStyle().FramePadding.y));
                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (Oak::UI::ImageButton(Resources::AddPlotIcon, ImVec2(Resources::AddPlotIcon->GetWidth(), Resources::AddPlotIcon->GetHeight())))
                {
                    //UI_ShowPlotCreationPopUp();
                    m_PanelManager->AddPanel<PlotPanel>(Oak::PanelCategory::VIEW, PLOT_PANEL("2"), "PLOT-2", SHOW);
                    //AddPlot();
                }

                ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
                if (Oak::UI::ImageButton(Resources::SettingsIcon, ImVec2(Resources::SettingsIcon->GetWidth(), Resources::SettingsIcon->GetHeight())))
                    m_PanelManager->TogglePanel(SETTINGS_PANEL);
            }
        ImGui::End();
        }
      
        ImGui::PopStyleVar(style_pop_count);
        ImGui::PopStyleColor(color_pop_count);
    
    }

    void AppLayer::DrawStatusBar()
    {

        int style_pop_count = 0;
        int color_pop_count = 0;

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f)); style_pop_count++;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.0f, 3.0f)); style_pop_count++;
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
                const char* m_StatusString = "";
                ImGuiIO& io = ImGui::GetIO();
                std::string status_text = fmt::format("Status: {} ({:.1f} fps) ", m_StatusString, io.Framerate);// +m_StatusStr;
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(status_text.c_str()).x - 7.0f);
                ImGui::Text(status_text.c_str());

                ImGui::EndMenuBar();
            }
            ImGui::End();
        }

        ImGui::PopStyleVar(style_pop_count);
        ImGui::PopStyleColor(color_pop_count);

    }
    void AppLayer::UI_ShowPlotCreationPopUp()
    {
        Oak::UI::ShowMessageBox("Create Plot", []()
            {
                PlotSpec settings = {};
                ImGui::Text("Define Plot Properties");
                ImGui::Separator();
                ImGui::InputTextWithHint("##NAME", "Enter plot title.. ", &settings.Name);
                ImGui::InputTextWithHint("##TITLE", "Enter plot title.. ", &settings.Title);
                ImGui::InputTextWithHint("##XLABEL", "x-label.. ", &settings.LabelX);
                ImGui::InputTextWithHint("##YLABEL", "y-label.. ", &settings.LabelY);
                if (ImGui::Button("OK"))
                    ImGui::CloseCurrentPopup();
            }, 600);
    }
}
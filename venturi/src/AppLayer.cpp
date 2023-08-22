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

namespace wi::Venturi
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
        

    AppLayer::AppLayer(DataManager& dataManager) // AppLayer::AppLayer(const Oak::Ref<Oak::UserPreferences>& userPreferences)
        : Layer("VENTURI AppLayer LAYER"), m_plotcount(0), m_Style(ImGui::GetStyle()), m_DataManager(&dataManager)
    {
    }

    AppLayer::~AppLayer()
    {
    }

    void AppLayer::OnAttach()
    {
        Resources::Init();

        UpdateStyle();

        // panels
        m_PanelManager = Oak::CreateScope<Oak::PanelManager>();

        m_PanelManager->AddPanel<Oak::ConsolePanel>(Oak::PanelCategory::VIEW, EMBEDDED_TERMINAL, "LOG", SHOW);
        m_PanelManager->AddPanel<ExplorerPanel>(Oak::PanelCategory::VIEW, FILE_EXPLORER_PANEL, "EXPLORER", HIDE);
        m_PanelManager->AddPanel<PlotPanel>(Oak::PanelCategory::VIEW, PLOT_PANEL("1"), "PLOT-1", SHOW, m_DataManager);
        
        m_PanelManager->AddPanel<OptionsPanel>(Oak::PanelCategory::EDIT, SETTINGS_PANEL, "SETTINGS", HIDE);
        m_PanelManager->AddPanel<NodeEditor>(Oak::PanelCategory::EDIT, NODE_EDITOR, "SETUP", HIDE);
        
        m_PanelManager->AddPanel<AppMetrics>(Oak::PanelCategory::TOOLS, APPLICATION_METRICS_PANEL, "APP METRICS", HIDE);
        m_PanelManager->AddPanel<DemoPanels>(Oak::PanelCategory::TOOLS, IMGUI_DEMO_PANEL, "DEMOS", HIDE);
        m_PanelManager->AddPanel<AboutPanel>(Oak::PanelCategory::HELP, ABOUT_PANEL, "ABOUT", HIDE);

        LOG_TRACE("Testing LOG_TRACE");
        LOG_CORE_TRACE("Testing LOG_CORE_TRACE");
        LOG_INFO("Testing LOG_INFO");
        LOG_WARN("Testing LOG_WARN");
        LOG_ERROR("Testing LOG_ERROR");
        LOG_FATAL_TAG("TEST TAG", "Testing LOG_FATAL");
        ASSERT(true, "TEST ASSERT");
        CORE_ASSERT(true, "TEST ASSERT");

    }

    void AppLayer::OnDetach()
    {
        Resources::Shutdown();
        m_PanelManager.reset();
    }

    void AppLayer::OnUpdate(Timestep ts)
    {
        //Oak::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Oak::RenderCommand::SetClearColor({ 0.5f, 0.5f, 0.5f, 0.5 });
        Oak::RenderCommand::Clear();
    }

    void AppLayer::OnUIRender()
    {
        // load styles from file
        SetGlobalStyle();
       
        // ImGui + Dockspace Setup ---------------------------------------------------------
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigWindowsResizeFromEdges = io.BackendFlags & ImGuiBackendFlags_HasMouseCursors;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoScrollbar;

        bool isMaximized = Oak::Application::Get().IsMaximized();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

        ImGui::Begin("Main Window", NULL, window_flags);

        ImGui::PopStyleVar(4);

        ImGui::PushStyleColor(ImGuiCol_Border, m_Style.Colors[ImGuiCol_MenuBarBg]);
        if (!isMaximized)
            Oak::UI::RenderWindowOuterBorders(ImGui::GetCurrentWindow());

        UI_HandleManualWindowResize();
        ImGui::PopStyleColor();

        float titlebarHeight = UI_DrawTitlebar();
        float statusbarHeight = UI_DrawStatusBar();
        
        ImGui::SetCursorPos(ImVec2(ImGui::GetCurrentWindow()->WindowPadding.x, titlebarHeight + ImGui::GetCurrentWindow()->WindowPadding.y));
        float sidebarWidth = UI_DrawSidebar(ImGui::GetWindowHeight() - titlebarHeight - statusbarHeight - ImGui::GetCurrentWindow()->WindowPadding.y);

        ImGui::SetCursorPosX(sidebarWidth + ImGui::GetCurrentWindow()->WindowPadding.x);
        ImGui::SetCursorPosY(titlebarHeight + ImGui::GetCurrentWindow()->WindowPadding.y);
        ImGuiID m_dockspace_id = ImGui::GetID("Main Dockspace");
        //ImGui::DockSpace(m_dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        ImGui::DockSpace(
            m_dockspace_id, 
            ImVec2(ImGui::GetWindowWidth() - sidebarWidth - ImGui::GetCurrentWindow()->WindowPadding.x*2.0f,
            ImGui::GetWindowHeight() - titlebarHeight - statusbarHeight - ImGui::GetCurrentWindow()->WindowPadding.y*2.0f), 
            dockspace_flags
        );

        SetGlobalStyle();

        m_PanelManager->OnUIRender();
        Oak::UI::RenderMessageBoxes();
    }
    
    void AppLayer::OnEvent(Oak::Event& e)
    {
        m_PanelManager->OnEvent(e);

        Oak::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Oak::KeyPressedEvent>(OAK_BIND_EVENT_FN(AppLayer::OnKeyPressed));
        dispatcher.Dispatch<Oak::MouseButtonPressedEvent>(OAK_BIND_EVENT_FN(AppLayer::OnMouseButtonPressed));
        dispatcher.Dispatch<Oak::MouseButtonReleasedEvent>(OAK_BIND_EVENT_FN(AppLayer::OnMouseButtonReleased));
        dispatcher.Dispatch<WindowTitleBarHitTestEvent>([this](WindowTitleBarHitTestEvent& event)
            {
                event.SetHit(UI_TitleBarHitTest(event.GetX(), event.GetY()));
                return m_TitleBarHovered;
            });
    }

    // ==========================================================================================================================
    void AppLayer::SetGlobalStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style = m_Style;
    }

    void AppLayer::UpdateStyle()
    {
        Oak::ImGuiStyleSerializer styleSerializer = Oak::ImGuiStyleSerializer(m_Style);
        if (!styleSerializer.Deserialize("assets/styles/default.style"))
            LOG_ERROR("could not serialize style");

        ImGuiStyle& style = ImGui::GetStyle();
        style = m_Style;
    }

    // ==========================================================================================================================
    void AppLayer::UI_DrawMenubar()
    {
        const ImRect menuBarRect = { ImGui::GetCursorPos(), {ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeightWithSpacing()} };

        ImGui::BeginGroup();
        if (Oak::UI::BeginMenuBar(menuBarRect))
        {
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
        }
        Oak::UI::EndMenuBar();
        ImGui::EndGroup();
    }
    
    void AppLayer::UI_DrawWindowButtons()
    {
        // window buttons
        int pad = 6; // padding around the image texture displayed on the button
        float size = ImGui::GetFrameHeight() - 2.0f * pad; // want the button to fill the frame so the image size is the frame height - 2x padding
        // window control buttons with no spacing
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, m_Style.Colors[ImGuiCol_MenuBarBg]); 
        
        // reposition cursor to draw the right side
        ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 3.0f * (size + 2.0f * pad) - 2.0f);

        if (ImGui::ImageButton((ImTextureID)Resources::MinimizeIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
            Oak::Application::Get().DispatchEvent<WindowMinimizeEvent, false>();
        if (Oak::Application::Get().IsRestored())
        {
            if (ImGui::ImageButton((ImTextureID)Resources::MaximizeIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
                Oak::Application::Get().DispatchEvent<WindowMaximizeEvent, false>();
        }
        if (Oak::Application::Get().IsMaximized())
        {
            if (ImGui::ImageButton((ImTextureID)Resources::RestoreIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
                Oak::Application::Get().DispatchEvent<WindowRestoreEvent, false>();
        }
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.92f, 0.25f, 0.20f, 0.5f));
        if (ImGui::ImageButton((ImTextureID)Resources::CloseIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
            Oak::Application::Get().DispatchEvent<WindowCloseEvent, false>();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(); // ItemSpacing

    }

    float AppLayer::UI_DrawTitlebar()
    {
        // set up ----------------------------------------------------------------------------------------------------------------------------------------
        const float titlebarHeight = ImGui::GetFrameHeightWithSpacing();
        const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;
        
        ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y));
        const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
        const ImVec2 titlebarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
                                        ImGui::GetCursorScreenPos().y + titlebarHeight };
        auto* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(titlebarMin, titlebarMax, Oak::UI::Colours::ImVec4toImU32(m_Style.Colors[ImGuiCol_MenuBarBg]));
        
        // logo ----------------------------------------------------------------------------------------------------------------------------------------
        {
            const int logoWidth = Resources::AppLogo->GetWidth();
            const int logoHeight = Resources::AppLogo->GetHeight();
            const ImVec2 logoOffset(8.0f + windowPadding.x, 8.0f + windowPadding.y);
            const ImVec2 logoRectStart = { ImGui::GetItemRectMin().x + logoOffset.x, ImGui::GetItemRectMin().y + logoOffset.y };
            const ImVec2 logoRectMax = { logoRectStart.x + logoWidth, logoRectStart.y + logoHeight };
            drawList->AddImage(Oak::UI::GetImTextureID(Resources::AppLogo), logoRectStart, logoRectMax);
        }
        
        ImGui::BeginHorizontal("Titlebar", { ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing() });

        static float moveOffsetX;
        static float moveOffsetY;
        //const float w = ImGui::GetContentRegionAvail().x;
        const float w = ImGui::GetContentRegionMax().x;
        const float buttonsAreaWidth = 90;

        // title bar drag area
#ifdef WI_PLATFORM_WINDOWS
        ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight));
        m_TitleBarHovered = ImGui::IsItemHovered() && (Oak::Input::GetCursorMode() != Oak::CursorMode::Locked);
#else
        ///todo
#endif                  
        
        // menu ----------------------------------------------------------------------------------------------------------------------------------------
       ImGui::SuspendLayout();
        {
            ImGui::SetItemAllowOverlap();
            const float logoOffset = 8.0f * 2.0f + 41.0f + windowPadding.x;
            ImGui::SetCursorPos(ImVec2(logoOffset, 2.0f));

            UI_DrawMenubar();
            if (ImGui::IsItemHovered())
                m_TitleBarHovered = false;
        }
        const float menuBarRight = ImGui::GetItemRectMax().x - ImGui::GetCurrentWindow()->Pos.x;
        // project name
        {
            ImGui::SameLine();
            const std::string projectName = "placeholder project name";
            const ImVec2 textSize = ImGui::CalcTextSize(projectName.c_str());
            // ImGui::SetCursorPosX(menuBarRight + 50.0f);
            //ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonsAreaWidth - textSize.x - 10.f );
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.f);
            ImGui::SetCursorPosY(2.0f + windowPadding.y);
            ImGui::PushStyleColor(ImGuiCol_Text, Oak::UI::Colours::Theme::textDarker);
            ImGui::Text(projectName.c_str());
            ImGui::PopStyleColor();
        }

        ImGui::ResumeLayout();
        ImGui::Spring();

        UI_DrawWindowButtons();
       

        ImGui::EndHorizontal();

      return titlebarHeight;

    }

    float AppLayer::UI_DrawSidebar(float sidebarHeight)
    {
       
        int style_pop_count = 0;
        int color_pop_count = 0; 
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));       style_pop_count++;
        ImGui::PushStyleColor(ImGuiCol_Button, m_Style.Colors[ImGuiCol_MenuBarBg]); color_pop_count++;
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_Style.Colors[ImGuiCol_MenuBarBg]); color_pop_count++;
        ImGui::PushStyleColor(ImGuiCol_WindowBg, m_Style.Colors[ImGuiCol_MenuBarBg]); color_pop_count++;

        const float sidebarWidth = 47.0f;
        const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

        const ImVec2 sidebarMin = ImGui::GetCursorScreenPos();
        const ImVec2 sidebarMax = { ImGui::GetCursorScreenPos().x + sidebarWidth,
                                    ImGui::GetCursorScreenPos().y + sidebarHeight};
        auto* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(sidebarMin, sidebarMax, Oak::UI::Colours::ImVec4toImU32(m_Style.Colors[ImGuiCol_MenuBarBg]));

        float size = (float)Resources::MenuIcon->GetHeight();
        ImGui::SetCursorPosX(windowPadding.x + sidebarWidth * 0.5f - size * 0.5f);
        if (Oak::UI::ImageButton(Resources::MenuIcon, ImVec2((float)Resources::MenuIcon->GetWidth(), (float)Resources::MenuIcon->GetHeight())))
        {
            //m_PanelManager->TogglePanel(PROPERTY_TREE_PANEL);
            //m_PanelManager->TogglePanel(PROPERTY_EDIT_PANEL);
        }

        ImGui::SetCursorPosX(windowPadding.x + sidebarWidth * 0.5f - size * 0.5f);
        if (Oak::UI::ImageButton(Resources::ExplorerIcon, ImVec2((float)Resources::ExplorerIcon->GetWidth(), (float)Resources::ExplorerIcon->GetHeight())))
            m_PanelManager->TogglePanel(FILE_EXPLORER_PANEL);

        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            ImGui::SetTooltip("Ctrl+Shit+E");

        ImGui::SetCursorPosX(windowPadding.x + sidebarWidth * 0.5f - size * 0.5f);
        if (Oak::UI::ImageButton(Resources::ConnectionIcon, ImVec2((float)Resources::ConnectionIcon->GetWidth(), (float)Resources::ConnectionIcon->GetHeight())))
        {
            if (!m_PanelManager->IsPanelOpen(NODE_EDITOR))
                m_PanelManager->TogglePanel(NODE_EDITOR);
                    
            ImGui::SetWindowFocus(m_PanelManager->GetPanelName(NODE_EDITOR));
        }

        ImGui::SetCursorPosX(windowPadding.x + sidebarWidth * 0.5f - size * 0.5f);
        if (Oak::UI::ImageButton(Resources::ChartIcon, ImVec2((float)Resources::ChartIcon->GetWidth(), (float)Resources::ChartIcon->GetHeight())))
        {
            //if (m_show_simple_plot)
            //	ImGui::SetWindowFocus("plot1");
            //else
            //	m_show_simple_plot = !m_show_simple_plot;
        }

        ImGui::SetCursorPosX(windowPadding.x + sidebarWidth * 0.5f - size * 0.5f);
        if (Oak::UI::ImageButton(Resources::ConsoleIcon, ImVec2((float)Resources::ConsoleIcon->GetWidth(), (float)Resources::ConsoleIcon->GetHeight())))
            m_PanelManager->TogglePanel(EMBEDDED_TERMINAL);

        ImGui::SetCursorPosY(sidebarHeight - 2.0f * (size * 0.5f + ImGui::GetStyle().ItemSpacing.y)  );
        ImGui::SetCursorPosX(windowPadding.x + sidebarWidth * 0.5f - size * 0.5f);
        if (Oak::UI::ImageButton(Resources::AddPlotIcon, ImVec2((float)Resources::AddPlotIcon->GetWidth(), (float)Resources::AddPlotIcon->GetHeight())))
        {
            //UI_ShowPlotCreationPopUp();
            m_PanelManager->AddPanel<PlotPanel>(Oak::PanelCategory::VIEW, PLOT_PANEL("2"), "PLOT-2", SHOW, m_DataManager);
            //AddPlot();
        }

        ImGui::SetCursorPosX(windowPadding.x + sidebarWidth * 0.5f - size * 0.5f);
        if (Oak::UI::ImageButton(Resources::SettingsIcon, ImVec2((float)Resources::SettingsIcon->GetWidth(), (float)Resources::SettingsIcon->GetHeight())))
            m_PanelManager->TogglePanel(SETTINGS_PANEL);
      
        ImGui::PopStyleVar(style_pop_count);
        ImGui::PopStyleColor(color_pop_count);
    
        return sidebarWidth;
    }

    float AppLayer::UI_DrawStatusBar()
    {
        int style_pop_count = 0;
        int color_pop_count = 0;
        
        const float statusbarHeight = ImGui::GetFrameHeight();
        const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

        ImGui::SetCursorPos(ImVec2(windowPadding.x, ImGui::GetWindowHeight() - statusbarHeight - windowPadding.y));
        const ImVec2 sidebarMin = ImGui::GetCursorScreenPos();
        const ImVec2 sidebarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.x * 2.0f,
                                        ImGui::GetCursorScreenPos().y + statusbarHeight };
        auto* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(sidebarMin, sidebarMax, Oak::UI::Colours::ImVec4toImU32(m_Style.Colors[ImGuiCol_MenuBarBg]));

        ImGui::BeginHorizontal("Statusbar", { ImGui::GetWindowWidth() - windowPadding.y * 2.0f, statusbarHeight });


        ImGui::PushStyleColor(ImGuiCol_Text, Oak::UI::Colours::Theme::textDarker); color_pop_count++;
        ImGui::SetCursorPosX(windowPadding.x+ 14.0f);
        ImGui::Text("File: "); 
        //if (!m_CurrentWorkingFile.empty()) ImGui::Text(m_CurrentWorkingFile.c_str());
        //else ImGui::Text("No file loaded ...");
        const char* m_StatusString = "";
        ImGuiIO& io = ImGui::GetIO();
        std::string status_text = fmt::format("Status: {} ({:.1f} fps) ", m_StatusString, io.Framerate);// +m_StatusStr;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(status_text.c_str()).x - 7.0f);
        ImGui::Text(status_text.c_str());

        ImGui::EndHorizontal();

        ImGui::PopStyleVar(style_pop_count);
        ImGui::PopStyleColor(color_pop_count);
        
        return statusbarHeight;
    }

    // ==========================================================================================================================
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
            m_PanelManager->AddPanel<PlotPanel>(Oak::PanelCategory::VIEW, plotName.c_str(), plotName.c_str(), SHOW, m_DataManager);
            ImGui::CloseCurrentPopup();
        }
    }

    // ==========================================================================================================================
    void AppLayer::NewFile()
    {
        LOG_INFO("New File");
    }

    void AppLayer::OpenFile()
    {
        std::filesystem::path filepath = Oak::FileSystem::OpenFileDialog("All Files (*.*)\0*.*\0");
        if (!filepath.empty())
        {
            // handle the open file here
            //m_CurrentWorkingFile = filepath;
            LOG_INFO("Open File {}", filepath);
            // m_current_filepath 
        }
        else {
            LOG_WARN("File dialog canceled");
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
            LOG_INFO("Save File As {}", filepath);
            // handle the save file here

        }
        else {
            LOG_WARN("File dialog canceled");
        }
    }

    void AppLayer::CloseFile()
    {
        //if (!m_CurrentWorkingFile.empty())
        //{
        //	LOG_INFO("Closeing File {}", m_CurrentWorkingFile);
        //	m_CurrentWorkingFile.clear();

        //}
    }

    // ==========================================================================================================================
    bool AppLayer::OnMouseButtonPressed(Oak::MouseButtonPressedEvent& e)
    {
        return false;
    }

    bool AppLayer::OnMouseButtonReleased(Oak::MouseButtonReleasedEvent& e)
    {
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
    
    // ==========================================================================================================================
    void AppLayer::UI_HandleManualWindowResize()
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        const bool maximized = (bool)glfwGetWindowAttrib(window, GLFW_MAXIMIZED);

        ImVec2 newSize, newPosition;
        if (!maximized && Oak::UI::UpdateWindowManualResize(ImGui::GetCurrentWindow(), newSize, newPosition))
        {
            // On Windows we hook into the GLFW win32 window internals
#ifndef WI_PLATFORM_WINDOWS

            glfwSetWindowPos(window, newPosition.x, newPosition.y);
            glfwSetWindowSize(window, newSize.x, newSize.y);
#endif
        }
    }

    bool AppLayer::UI_TitleBarHitTest(int /*x*/, int /*y*/) const
    {
        return m_TitleBarHovered;
    }

}
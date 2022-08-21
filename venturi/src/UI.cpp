#include "imgui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "implot/implot.h"

#include "oak/utils/PlatformUtils.h"


#include "UI.h"
#include "panels.h"

//temporary
#include "glad/include/glad/glad.h"
#include "GLFW/glfw3.h"




namespace Venturi
{

void DrawCanvas()
{
    uint32_t height = Oak::Application::Get().GetWindow().GetHeight();
    uint32_t width = Oak::Application::Get().GetWindow().GetWidth();

    float spacing = 1.0f / 60.0f;
    float dx = spacing;
    float dy = spacing * width/height;
    glLineWidth(0.01f);
    for (float i = -1.0f; i < 1.0f; i += dx )    
    {

        glBegin(GL_LINES);
        glColor4f(1.0, 1.0, 1.0, 0.5);
        glVertex2f(i,-1);
        glVertex2f(i,1);
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

#define SHOW true
#define HIDE false

    UI::UI()
     : Layer("VENTURI UI LAYER"), m_plotcount(0)
    {
    }

    void UI::OnAttach()
    {
        PushPanel(new DemoPanels("Demos", HIDE, this));
        PushPanel(new ExplorerPanel("Explorer", HIDE, this));
        PushPanel(new LogPanel("Log", HIDE, this));
        PushPanel(new AppMetrics("Metrics", SHOW, this));
        PushPanel(new AboutPanel("About", HIDE, this));

        // todo: consider isolating these three to be permanent and not to show up in the view menu
        PushPanel(new MainMenu("##MainMenu", SHOW, this));
        PushPanel(new StatusBar("##StatusBar", SHOW, this));
        PushPanel(new SideBar("##SideBar", SHOW, this));

    }

    void UI::OnDetach()
    {
    }

    void UI::PushPanel(Oak::Panel* panel)
    {
        m_PanelStack.PushPanel(panel);
    }

    void UI::QueuePanel(Oak::Panel* panel)
    {
        m_PanelQueue.PushPanel(panel);
    }

    void UI::OnUpdate(Oak::Timestep ts)
    {
        //UpdateWindowPos();
        Oak::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Oak::RenderCommand::Clear();
        DrawCanvas();

        for (Oak::Panel* panel : m_PanelQueue)
        {
           // OAK_INFO("adding panel {}:{} to m_PanelStack", panel->GetID(), panel->GetName());
            PushPanel(&(*panel));
            OAK_INFO("added");
        }
        m_PanelQueue.ClearStack();

        ImGuiIO& io = ImGui::GetIO();
        m_LastMousePos = io.MousePos;

    }

    void UI::OnUIRender()
    {
        for (Oak::Panel* panel : m_PanelStack)
        {
            //OAK_WARN("looping panel stack: current pannel {}:{}", panel->GetID(), panel->GetName());
            SetGlobalStyle();
            panel->SetLocalStyle();
            if (panel->Visibility()) panel->OnUIRender(panel->p_open);
        }
        SetGlobalStyle();
    }

    Oak::Panel* UI::GetPanel(std::string name)
    {
        for (Oak::Panel* panel : m_PanelStack)
        {
          if (panel->GetName() == name)
            return panel;

        }
        OAK_ASSERT(false, "No panel found");
        return nullptr;
    }

    Oak::Panel* UI::GetPanel(uint32_t id)
    {
        for (Oak::Panel* panel : m_PanelStack)
        {
            if (panel->GetID() == id)
                return panel;

        }
        OAK_ASSERT(false, "No panel found");
        return nullptr;
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
        QueuePanel(new RTPlot(ss.str(), true, this));
    }

    bool UI::OnMouseButtonPressed(Oak::MouseButtonPressedEvent& e)
    {
        SetRelativeMousePos(ImGui::GetIO().MousePos.x - Oak::Application::Get().GetWindow().GetPosX(), ImGui::GetIO().MousePos.y - Oak::Application::Get().GetWindow().GetPosY());
        return false;
    }

    bool UI::OnMouseButtonReleased(Oak::MouseButtonReleasedEvent& e)
    {
        SetLastWindowPos(Oak::Application::Get().GetWindow().GetPosX(), Oak::Application::Get().GetWindow().GetPosX());
        return false;
    }

    bool UI::OnKeyPressed(Oak::KeyPressedEvent& e)
    {
        if (e.IsRepeat())
            return false;

        bool control = Oak::Input::IsKeyPressed(Oak::Key::LeftControl) || Oak::Input::IsKeyPressed(Oak::Key::RightControl);
        bool shift = Oak::Input::IsKeyPressed(Oak::Key::LeftShift) || Oak::Input::IsKeyPressed(Oak::Key::RightShift);
        bool alt = Oak::Input::IsKeyPressed(Oak::Key::LeftAlt) || Oak::Input::IsKeyPressed(Oak::Key::RightAlt);

        switch (e.GetKeyCode())
        {
        case Oak::Key::D1:
        {
            if (control) GetPanel(1)->Toggle();
            break;
        }
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
            GetPanel("Explorer")->Toggle();
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
        std::string filepath = Oak::FileDialogs::OpenFile("All Files (*.*)\0*.*\0");
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
        std::string filepath = Oak::FileDialogs::SaveFile("All Files (*.*)\0*.*\0");
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


    void UI::SetGlobalStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.FramePadding = ImVec2(5.0f, 5.0f);
        style.FrameBorderSize = 0.0f;
        style.WindowPadding = ImVec2(0.0f, 0.0f);
        style.WindowBorderSize = 0.0f;
        style.TabRounding = 0.0f;
        style.ScrollbarRounding = 0.0f;

        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        //colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        // Title
        //colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        //colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        //colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBg] = colors[ImGuiCol_FrameBg];
        colors[ImGuiCol_TitleBgActive] = colors[ImGuiCol_FrameBg];
        colors[ImGuiCol_TitleBgCollapsed] = colors[ImGuiCol_FrameBg];

    }

}
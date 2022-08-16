#include "imgui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "implot/implot.h"
#include "imgui/imgui_demo.cpp"

#include "oak/utils/PlatformUtils.h"

#include "VenturiUI.h"

VenturiUI::VenturiUI()
 	: Layer("VentruiUI")
{
	m_AppLogo = Oak::Texture2D::Create("assets/textures/venturi-2.png");
	m_WiLogo  = Oak::Texture2D::Create("assets/textures/wi.png");
	m_SettingsIcon = Oak::Texture2D::Create("assets/textures/icons8-services-32.png");
	m_ConnectionIcon = Oak::Texture2D::Create("assets/textures/icons8-broadcasting-32.png");
	m_ExplorerIcon = Oak::Texture2D::Create("assets/textures/icons8-documents-32.png");
	m_MenuIcon = Oak::Texture2D::Create("assets/textures/icons8-menu-32.png");

}

void VenturiUI::OnAttach()
{


}

void VenturiUI::OnDetach()
{
}

void VenturiUI::OnUpdate(Oak::Timestep ts)
{
	Oak::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Oak::RenderCommand::Clear();
}

void VenturiUI::UI_MainMenu(bool* p_open)
{
	if (ImGui::BeginMenuBar())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
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
			if (ImGui::MenuItem("Exit", "Alt+F4"))
				Oak::Application::Get().Close();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Example PLots        ", NULL, &m_show_simple_plot);
			ImGui::MenuItem("File Explorer        ", NULL, &m_show_explorer);
			ImGui::MenuItem("Output Log           ", NULL, &m_show_app_log);
			ImGui::MenuItem("ImGui Demo           ", NULL, &m_show_imgui_demo);
			ImGui::MenuItem("ImPlot Demo          ", NULL, &m_show_implot_demo);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Option"))
		{
			ImGui::MenuItem("VSync                ", NULL, &m_toggle_vsync);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("App Metrics", "", &m_show_metrics);
			ImGui::EndMenu();
		}
		ImGui::PopStyleVar();
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void VenturiUI::UI_Sidebar(bool* p_open)
{
	bool pop_open = true;
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 5.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0, 5.0f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

	if (ImGui::BeginMenuBar())
	{
		ImGui::SetCursorPosY(3.5f);
		ImGui::Image((ImTextureID)m_AppLogo->GetRendererID(), ImVec2(ImGui::GetContentRegionAvail().x-2.0f, ImGui::GetFrameHeight()-7.0f));
	}
	ImGui::EndMenuBar();

	float size = ImGui::GetContentRegionAvail().x - 10.0f;

	{
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)m_MenuIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
		{
			OAK_INFO("OPEN CONNECTION POPUP/PANEL");
			m_show_explorer = !m_show_explorer;
		}
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)m_ExplorerIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
		{
			OAK_INFO("OPEN CONNECTION POPUP/PANEL");
			m_show_explorer = !m_show_explorer;
		}

		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)m_ConnectionIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
		{
			OAK_INFO("OPEN CONNECTION POPUP/PANEL");
			m_StatusStr.clear();
		}


		ImGui::SetCursorPosY((ImGui::GetWindowContentRegionMax().y) - (size));
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)m_SettingsIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 0.5f)))
			ImGui::OpenPopup("About");


		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowBgAlpha(0.8f);	
		if (ImGui::BeginPopupModal("About", &pop_open))
		{
			ImGui::Text("Version: 0.1.0 (dAugust-2020)");
			ImGui::Separator();
			ImGui::Text("Icons by https://icons8.com");
			ImGui::EndPopup();
		}
	}


	ImGui::PopStyleColor(1);
	ImGui::PopStyleVar(2);
	

}



void VenturiUI::UI_Statusbar(bool* p_open)
{
	if (ImGui::BeginMenuBar())
	{	
		ImGui::Text("File: ");
		if (!m_CurrentWorkingFile.empty()) ImGui::Text(m_CurrentWorkingFile.c_str());
		else ImGui::Text("No file loaded ...");

		std::string status_text = "Status: " + m_StatusStr;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(status_text.c_str()).x - 7.0f);
		ImGui::Text(status_text.c_str());


	}
	ImGui::EndMenuBar();
}

void VenturiUI :: OnUIRender()
{	
	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_AutoHideTabBar;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Main DockSpace", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();
	if (opt_fullscreen)
		ImGui::PopStyleVar(2);
	
	// DockSpace
	ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0.0f);
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID m_dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(m_dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.TabMinWidthForCloseButton = FLT_MAX;
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.FrameBorderSize = 0.0f;
	style.WindowBorderSize = 0.0f;
	
	// Set up Fixed UI bars (note that BeginViewportSidebar has is still in imgui_internal.h and therefore may be unstable).  
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGuiWindowFlags ui_flags =  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground;
	if (ImGui::BeginViewportSideBar("##StatusBar", viewport, ImGuiDir_Down, ImGui::GetFrameHeight(), ui_flags))
		UI_Statusbar(NULL);
	ImGui::End();

	if (ImGui::BeginViewportSideBar("##SideBar", viewport, ImGuiDir_Left, 47.0f, ui_flags))
		UI_Sidebar(NULL);
	ImGui::End();

	if (ImGui::BeginViewportSideBar("##MainMenu", viewport, ImGuiDir_Up, ImGui::GetFrameHeight(), ui_flags))
		UI_MainMenu(NULL);
	ImGui::End();

	

	ImGui::PopStyleVar(3);

	// generate UI
	if (m_show_app_log) 		m_LogPanel.OnUIRender(&m_show_app_log);
	if (m_show_explorer) 		m_ExplorerPanel.OnUIRender(&m_show_explorer);
	if (m_show_simple_plot)		m_SimplePlotPanel.OnUIRender("plot1", &m_show_simple_plot);
	if (m_show_simple_plot)		m_SimplePlotPanel2.OnUIRender("plot2", &m_show_simple_plot);

	if (m_show_imgui_demo) 		ImGui::ShowDemoWindow(&m_show_imgui_demo);
	if (m_show_implot_demo) 	ImPlot::ShowDemoWindow(&m_show_implot_demo);
	if (m_show_metrics)			ImPlot::ShowMetricsWindow(&m_show_metrics);

	// Toggle Options
	Oak::Application& app = Oak::Application::Get();
	Oak::Window& window = app.GetWindow();
	window.SetVSync(m_toggle_vsync);


}

void VenturiUI::OnEvent(Oak::Event& e)
{
	Oak::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Oak::KeyPressedEvent>(OAK_BIND_EVENT_FN(VenturiUI::OnKeyPressed));
	//dispatcher.Dispatch<Oak::MouseButtonPressedEvent>(OAK_BIND_EVENT_FN(VenturiUI::OnMouseButtonPressed));
}

bool VenturiUI::OnKeyPressed(Oak::KeyPressedEvent& e)
{
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
	case Oak::Key::Escape:
	{
		OAK_INFO("EcapeKey pressed");
		ImGui::CloseCurrentPopup();
		break;
	}
	}
}


void VenturiUI::NewFile()
{
	OAK_INFO("New File");

}

void VenturiUI::OpenFile()
{
	std::string filepath = Oak::FileDialogs::OpenFile("All Files (*.*)\0*.*\0");
	if (!filepath.empty())
	{
		m_CurrentWorkingFile = filepath;
		OAK_INFO("Open File {}", filepath);
		m_AppLogo = Oak::Texture2D::Create(filepath);
		// handle the open file here
		// m_current_filepath 
	}
	else {
		OAK_WARN("File dialog canceled");
	}
}

void VenturiUI::SaveFile()
{
	// if (!m_current_filepath.empty()) 
		// save...
	SaveFileAs();
}

void VenturiUI::SaveFileAs()
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

void VenturiUI::CloseFile()
{
	if (!m_CurrentWorkingFile.empty())
	{
		OAK_INFO("Closeing File {}", m_CurrentWorkingFile);
		m_CurrentWorkingFile.clear();

	}
}


#include "imgui/imgui.h"
#include "implot/implot.h"
#include "imgui/imgui_demo.cpp"

#include "VenturiUI.h"

VenturiUI::VenturiUI()
 	: Layer("VentruiUI")
{
	m_Image = std::make_shared<Oak::OpenGLImage>("assets/textures/venturi.png");
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

void VenturiUI :: OnUIRender()
{	

	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_AutoHideTabBar;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
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
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 5.0f;
	style.TabMinWidthForCloseButton = FLT_MAX;
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	static bool show_app_log = 		true;
	static bool show_explorer = 	true;
	static bool show_simple_plot = 	true;
	static bool show_imgui_demo = 	true;
	static bool show_implot_demo = 	true;
	static bool toggle_vsync = 		true;

	if (show_app_log) 		m_LogPanel.OnUIRender(&show_app_log);
	if (show_explorer) 		m_ExplorerPanel.OnUIRender(&show_explorer);
	if (show_simple_plot)	m_SimplePlotPanel.OnUIRender("plot1", &show_simple_plot);
	if (show_simple_plot)	m_SimplePlotPanel2.OnUIRender("plot2", &show_simple_plot);


	if (show_imgui_demo) 	ImGui::ShowDemoWindow(&show_imgui_demo);
	if (show_implot_demo) 	ImPlot::ShowDemoWindow(&show_implot_demo);

 
 	style.WindowMinSize.x = minWinSizeX;

	Oak::Application& app = Oak::Application::Get();
	Oak::Window& window = app.GetWindow();
	window.SetVSync(toggle_vsync);


	if (ImGui::BeginMenuBar())
	{
	//	ImGui::Image((ImTextureID)m_Image->GetTextureID(), ImVec2(500.0f, ImGui::GetContentRegionAvail().y));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
			if (ImGui::MenuItem("New", "Ctrl+N"))
				Oak::Application::Get().Close();
				// NewScene();
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
				Oak::Application::Get().Close();
				// OpenScene();
			if (ImGui::MenuItem("Save", "Ctrl+S"))
				Oak::Application::Get().Close();
				// SaveScene();
			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				Oak::Application::Get().Close();
				// SaveSceneAs();
			ImGui::Separator();
			if (ImGui::MenuItem("Exit")) 
				Oak::Application::Get().Close();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
            ImGui::MenuItem("Example PLots", "", &show_simple_plot);
            ImGui::MenuItem("File Explorer", "", &show_explorer);
            ImGui::MenuItem("Output Log", "", &show_app_log);
            ImGui::MenuItem("ImGui Demo", "", &show_imgui_demo);
            ImGui::MenuItem("ImPlot Demo", "", &show_implot_demo);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Option"))
		{
			ImGui::MenuItem("VSync", "", &toggle_vsync);
			ImGui::EndMenu();
		}
		ImGui::PopStyleVar();
		ImGui::EndMenuBar();
	}
	ImGui::End();

	ImGui::Begin("Image");
	ImGui::Image((ImTextureID)m_Image->GetTextureID(), {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y});
	ImGui::End();
	// static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration;
	


	ImPlot::ShowMetricsWindow(&dockspaceOpen);
	ImGui::PopStyleVar(3);

}


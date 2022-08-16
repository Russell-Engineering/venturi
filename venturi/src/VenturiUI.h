#pragma once


#include "oak.h"
#include "oak/events/KeyEvent.h"

#include "panels/LogPanel.h"
#include "panels/ExplorerPanel.h"
#include "panels/SimplePlotPanel.h"



class VenturiUI : public Oak::Layer
{
public:
    VenturiUI();
    virtual ~VenturiUI() = default;
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    
    void OnUpdate(Oak::Timestep ts) override;
    virtual void OnUIRender() override;
    void OnEvent(Oak::Event& e) override;



private:
    
    // File Dialogs return empty string if cancelled. 
    void NewFile();
    void OpenFile();
    void SaveFile();
    void SaveFileAs();
    void CloseFile();

    std::string GetCurrentWokingFile() { return m_CurrentWorkingFile; }

    bool OnKeyPressed(Oak::KeyPressedEvent& e);
	// bool OnMouseButtonPressed(Oak::MouseButtonPressedEvent& e);
	
	// void OnOverlayRender();
    // 
	// UI Panels
	void UI_MainMenu(bool* p_open);
    void UI_Sidebar(bool* p_open);
    void UI_Statusbar(bool* p_open);
private:
    // dockspace
    ImGuiID m_dockspace_id;

    // Panels
    LogPanel m_LogPanel;
    ExplorerPanel m_ExplorerPanel;
    SimplePlotPanel m_SimplePlotPanel;
    SimplePlotPanel m_SimplePlotPanel2;

    // 
    std::string m_CurrentWorkingFile;
    std::string m_StatusStr;


    //assets
    Oak::Ref<Oak::Texture2D> m_AppLogo;
    Oak::Ref<Oak::Texture2D> m_WiLogo;
    Oak::Ref<Oak::Texture2D> m_SettingsIcon;
    Oak::Ref<Oak::Texture2D> m_ConnectionIcon;
    Oak::Ref<Oak::Texture2D> m_ExplorerIcon;
    Oak::Ref<Oak::Texture2D> m_MenuIcon;

    // view options
    bool m_show_simple_plot = true;
    bool m_show_explorer = true;
    bool m_show_app_log = true;
    bool m_show_imgui_demo = true;
    bool m_show_implot_demo = true;
    bool m_show_metrics = true;
    bool m_toggle_vsync = true;

};


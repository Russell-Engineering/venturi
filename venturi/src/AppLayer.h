#pragma once

#include "Oak.h"
#include "wi.h"
#include "ImGui/imgui.h"


namespace wi::Venturi
{
    class AppLayer : public Oak::Layer
    {

    public:

        AppLayer(DataManager& dataManager);
        ~AppLayer();

        void OnAttach() override; // called when layer is pushed to layerstack
        void OnDetach() override; // called when layer is popped from layerstack
        
        void OnUpdate(Timestep ts) override; // called every frame, use this to only clear the buffer for now
        void OnUIRender() override; // called every frame to draw ImGui ui, this is called by the engine between ImGuiLayer::Begin() and End() calls, which creates the new ImGui frames for us. 
        void OnEvent(Oak::Event& e) override;
        
    
        void LoadStyle(const std::string& filepath, const std::string& name);
        void SaveStyle(const std::string& filepath, const std::string& name);
        void SetGlobalStyle();
        void UpdateStyle();

        // permanent UI features
        //void DrawCanvas();
        void UI_DrawMenubar();
        void UI_DrawWindowButtons();
        float UI_DrawTitlebar();
        float UI_DrawSidebar(float sidebarHeight);
        float UI_DrawStatusBar();
        //void AppLayer::UI_ShowPlotCreationPopUp();

        void AddPlot();
        void NewFile();
        void OpenFile();
        void CloseFile();
        void SaveFile();
        void SaveFileAs();

        inline Oak::PanelManager& GetPanelManager() { return *m_PanelManager; }
        inline Ref<DataManager> GetDataManager() { return m_DataManager; }

    
    private:
        ImGuiStyle m_Style;
        uint32_t m_plotcount;
       
        Scope<Oak::PanelManager> m_PanelManager;
        Ref<DataManager> m_DataManager;

        // UI elements
        bool m_TitleBarHovered = false;
    
    private:
        // event callbacks
        bool OnKeyPressed(Oak::KeyPressedEvent& e);
        bool OnMouseButtonPressed(Oak::MouseButtonPressedEvent& e);
        bool OnMouseButtonReleased(Oak::MouseButtonReleasedEvent& e);

        void UI_HandleManualWindowResize();
        bool UI_TitleBarHitTest(int x, int y) const;
    };
}
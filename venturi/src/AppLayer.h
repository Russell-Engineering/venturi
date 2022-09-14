#pragma once

#include "Oak.h"
#include "ImGui/imgui.h"

namespace Venturi
{
    class AppLayer : public Oak::Layer
    {

    public:

        AppLayer();

        void OnAttach() override; // called when layer is pushed to layerstack
        void OnDetach() override; // called when layer is popped from layerstack
        
        void OnUpdate(Oak::Timestep ts) override; // called every frame, use this to only clear the buffer for now
        void OnUIRender() override; // called every frame to draw ImGui ui, this is called by the engine between ImGuiLayer::Begin() and End() calls, which creates the new ImGui frames for us. 
        void OnEvent(Oak::Event& e) override;
        
    
        void LoadStyle(const std::string& filepath, const std::string& name);
        void SaveStyle(const std::string& filepath, const std::string& name);
        void SetGlobalStyle();
        void UpdateStyle();

        // permanent UI features
        //void DrawCanvas();
        void DrawMenu();
        void DrawSidebar();
        void DrawStatusBar();

        void AddPlot();
        void NewFile();
        void OpenFile();
        void CloseFile();
        void SaveFile();
        void SaveFileAs();

        void UpdateWindowPos();
        ImVec2 GetLastMousePos() {return m_LastMousePos;}
        void SetLastWindowPos(float x, float y) { m_LastMousePos.x = x; m_LastMousePos.y = y; }

        uint32_t GetRestoredWidth() { return m_RestoredWidth; }
        uint32_t GetRestoredHeight() { return m_RestoredHeight; }
        ImVec2 GetRelativeMousePos() { return m_RelativeMousePos; }
        //void SetRelativeMousePos(float x, float y) { m_RelativeMousePos.x = x; m_RelativeMousePos.y = y; }
        void SetRelativeMousePos();

        inline Oak::PanelManager& GetPanelManager() { return *m_PanelManager; }


    
    private:
        ImGuiStyle m_Style;
        
        uint32_t m_plotcount;
        uint32_t m_RestoredWidth=1600;
        uint32_t m_RestoredHeight=900;
        ImVec2 m_LastMousePos; 
        ImVec2 m_RelativeMousePos;
       
        Oak::Scope<Oak::PanelManager> m_PanelManager;
        bool m_SideBarExpanded = false;

        bool OnKeyPressed(Oak::KeyPressedEvent& e);
        bool OnMouseButtonPressed(Oak::MouseButtonPressedEvent& e);
        bool OnMouseButtonReleased(Oak::MouseButtonReleasedEvent& e);
    };
}
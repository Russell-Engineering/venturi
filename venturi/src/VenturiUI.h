#pragma once


#include "oak.h"
#include "panels/LogPanel.h"
#include "panels/ExplorerPanel.h"
#include "panels/SimplePlotPanel.h"
#include "platform/OpenGL/OpenGLImage.h"



class VenturiUI : public Oak::Layer
{
public:
    VenturiUI();
    virtual ~VenturiUI() = default;
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    
    void OnUpdate(Oak::Timestep ts) override;
    virtual void OnUIRender() override;
    // void OnEvent(Oak::Event& e) override;
private:
	// bool OnKeyPressed(Oak::KeyPressedEvent& e);
	// bool OnMouseButtonPressed(Oak::MouseButtonPressedEvent& e);
	
	// void OnOverlayRender();
	// UI Panels
	void UI_Toolbar();
private:
    // Panels
    LogPanel m_LogPanel;
    ExplorerPanel m_ExplorerPanel;
    SimplePlotPanel m_SimplePlotPanel;
    SimplePlotPanel m_SimplePlotPanel2;

    //assets
    Oak::Ref<Oak::OpenGLImage> m_Image;
};


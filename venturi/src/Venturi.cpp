
#include "wi.h"
#include "oak.h"
#include "vk.h"
#include "oak/core/EntryPoint.h"
#include "AppLayer.h"
#include "DataLayer.h"


class VenturiApp : public Oak::Application
{
public:

    VenturiApp(const Oak::ApplicationSpecification& spec)
        : Oak::Application(spec)
    {
        m_Clock = wi::Ref<wi::Clock>::Create();
        m_DataManager = wi::CreateScope<wi::DataManager>(m_Clock);
       
        // load last project
        
        wi::Venturi::AppLayer*   appLayer = new wi::Venturi::AppLayer(*m_DataManager);
        wi::Venturi::DataLayer* dataLayer = new wi::Venturi::DataLayer(*m_DataManager);
        PushLayer(dataLayer);
        PushLayer(appLayer);
        //PushLayer(new wi::Venturi::ServerLayer());
    }

    VenturiApp::~VenturiApp()
    { 
    }
private:
    wi::Scope<wi::DataManager> m_DataManager;
    wi::Ref<wi::Clock> m_Clock;

};

Oak::Application* Oak::CreateApplication(int argc, char** argv)
{


    Oak::ApplicationSpecification spec;
    spec.Name = "Venturi";
    spec.Fullscreen = false;
    spec.WindowDecorated = false;
    spec.Resizable = true;
    spec.StartMaximized = true;
  

    return new VenturiApp(spec);
}


#include "oak.h"
#include "oak/core/EntryPoint.h"
#include "AppLayer.h"
//#include "layers/Workspace.h"


class VenturiApp : public Oak::Application
{
public:

	VenturiApp(const Oak::ApplicationSpecification& spec)
		: Oak::Application(spec)
	{
		// load last project
		PushLayer(new Venturi::AppLayer());
	}

	VenturiApp::~VenturiApp()
	{
	}

};

Oak::Application* Oak::CreateApplication(int argc, char** argv)
{


	Oak::ApplicationSpecification spec;
	spec.name = "Venturi";
	spec.fullscreen = true;

	return new VenturiApp(spec);
}

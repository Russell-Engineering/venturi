
#include "oak/core/Application.h"
#include "oak/core/EntryPoint.h"


#include "VenturiUI.h"



class Venturi : public Oak::Application
{
public:
	Venturi(const Oak::ApplicationSpecification& spec)
		: Oak::Application(spec)
	{
		PushLayer(new VenturiUI());
	}
		
	~Venturi()
	{
	}
};

Oak::Application* Oak::CreateApplication(int argc, char** argv)
{
	OAK_TRACE("Testing OAK_TRACE");
	OAK_INFO("Testing OAK_INFO");
	OAK_WARN("Testing OAK_WARN");
	OAK_ERROR("Testing OAK_ERROR");
	OAK_CRITICAL("Testing OAK_CRITICAL");

	Oak::ApplicationSpecification spec;
	spec.name = "Venturi";
	spec.iconPath = "assets/textures/venturi.png";

	return new Venturi(spec);
}



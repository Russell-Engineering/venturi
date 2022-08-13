
#include "oak/core/Application.h"
#include "oak/core/EntryPoint.h"


#include "VenturiUI.h"

#include "spdlog/spdlog.h"


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
	spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

	Oak::ApplicationSpecification spec;
	spec.name = "WI Venturi";
	spec.iconPath = "venturi/assets/wi.png";

	return new Venturi(spec);
}



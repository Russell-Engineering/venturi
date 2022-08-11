
#include "Application.h"
#include "EntryPoint.h"


#include "VenturiUI.h"

#include "spdlog/spdlog.h"


Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

	Walnut::ApplicationSpecification spec;
	spec.Name = "WI Venturi";
	spec.iconPath = "venturi/assets/wi.png";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<VenturiUI>();
	app->SetMenubarCallback([app]()
	{
		// std::shared_ptr<Walnut::Image> img = std::make_shared<Walnut::Image>("venturi/assets/wi.png");
		// ImGui::Image(img->GetDescriptorSet(), {10.0f, 10.0f});
		// if (ImGui::ImageButton(app->logo->GetDescriptorSet(), {15.0f, 15.0f})){

		// }
			
		if (ImGui::BeginMenu("File"))
		{	
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::EndMenu();
		}


	});
	return app;
}



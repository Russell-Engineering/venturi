
#include "Application.h"
#include "EntryPoint.h"


#include "VenturiUI.h"




Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{


	Walnut::ApplicationSpecification spec;
	spec.Name = "";
	spec.iconPath = "Venturi/assets/wi-logo.png";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<VenturiUI>();
	app->SetMenubarCallback([app]()
	{
		std::shared_ptr<Walnut::Image> img = std::make_shared<Walnut::Image>("Venturi/assets/venturi-logo3.png");
		// ImGui::Image(img->GetDescriptorSet(), {10.0f, 10.0f});
		if (ImGui::ImageButton(img->GetDescriptorSet(), {80.0f, 10.0f})){

		}
			
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



#include "ExplorerPanel.h"


ExplorerPanel::ExplorerPanel()
{
}

void ExplorerPanel::OnUIRender(bool* p_open)
{
	ImGui::Begin("Navigation",p_open, ImGuiWindowFlags_NoBackground);
	ImGui::Text("Todo: Add file exploere and a way to set working directory");
	ImGui::End();
}


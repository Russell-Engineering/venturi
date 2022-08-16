#include "ExplorerPanel.h"


ExplorerPanel::ExplorerPanel()
{
}

void ExplorerPanel::OnUIRender(bool* p_open)
{
	if (ImGui::Begin("##Explorer"))
	{
		ImGui::TextWrapped("Todo: Add a folder tree view into the current working directory");
	}
	ImGui::End();
}


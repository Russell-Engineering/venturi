#include "panels/StatusBar.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

namespace Venturi
{

    StatusBar::StatusBar(const std::string& name, bool show, UI* parent)
		: Panel(name, show), m_Parent(parent), m_temp_counter(0)
    {
		OAK_TRACE("Creating StatusBar Panel named {} with id {}", GetName(), GetID());
    }

    void StatusBar::OnUIRender(bool* open)
    {
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGuiWindowFlags ui_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground;
		if (ImGui::BeginViewportSideBar(GetName().c_str(), viewport, ImGuiDir_Down, ImGui::GetFrameHeight(), ui_flags))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("File: ");
				//if (!m_CurrentWorkingFile.empty()) ImGui::Text(m_CurrentWorkingFile.c_str());
				//else ImGui::Text("No file loaded ...");
				ImGui::Text("count: %i", m_temp_counter);
				++m_temp_counter;
				std::string status_text = "Status: ";// +m_StatusStr;
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(status_text.c_str()).x - 7.0f);
				ImGui::Text(status_text.c_str());

				ImGui::EndMenuBar();
			}
			ImGui::End();
		}
    }
	
	void StatusBar::SetLocalStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.FramePadding = ImVec2(1.0f, 1.0f);
		style.WindowPadding = ImVec2(5.0f, 5.0f);
		style.ItemSpacing = ImVec2(5.0f, 5.0f);
	}


}
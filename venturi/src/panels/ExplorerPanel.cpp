#include "panels/ExplorerPanel.h"
#include "ImGui/imgui.h"

#include "resources.h"

namespace wi::Venturi
{
	struct DirectoryTreeItem
	{

		std::string value;
		DirectoryTreeItem* parent;
		DirectoryTreeItem* first_child;
		DirectoryTreeItem* next_sibling;

		DirectoryTreeItem()
			: parent(nullptr), first_child(nullptr), next_sibling(nullptr)
		{
		}

		DirectoryTreeItem(const std::string& value, DirectoryTreeItem* parent)
			: parent(nullptr), first_child(nullptr), next_sibling(nullptr)

		{
		}

		DirectoryTreeItem* LastChild()
		{
			DirectoryTreeItem* next_child = first_child;
			DirectoryTreeItem* child = nullptr;

			while (next_child != nullptr)
			{
				child = next_child;
				next_child = next_child->next_sibling;
			}

		}

		bool IsFile() { return (first_child == nullptr) ? true : false; }
		bool IsDir() { return !IsDir(); }

	};



	void ExplorerPanel::PushLocalStyle()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); m_StylePopCount++;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f)); m_StylePopCount++;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize,  0.0f); m_StylePopCount++;
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,  0.0f); m_StylePopCount++;
	}


	void ExplorerPanel::OnUIRender(const char *name, bool& open)

	{

		const std::string& directoryPath = Oak::Application::Get().GetSpecification().WorkingDirectory;

		ImGui::Begin(name, &open);

		
		ImGui::SetNextItemOpen(m_expanded);
		m_expanded = ImGui::CollapsingHeader("EXPLORER");
		bool hovered = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.5f));
		ImGui::Text(" %s\\", Oak::Application::Get().GetSpecification().WorkingDirectory.c_str());
		ImGui::PopStyleColor();
		if (hovered)
		{
			int pad = 2;
			float size = ImGui::GetFrameHeight() - 2.0f * pad;
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 3.0f*(size+2.0f*pad));
			ImGui::BeginChild("##REFRESH_EXPLORER", ImVec2(3.0f*(size+2.0f*pad), (size+2.0f*pad)));
			ImGui::ImageButton((ImTextureID)Resources::RefreshIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad);
		
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Refresh Explorer");
			}

			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)Resources::NewFileIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
			{
				// new file to explorer
				// prompt for name
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("New File");
			}

			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)Resources::NewDirIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), pad))
			{
				// new directory to explorer
				// prompt for name
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("New Directory");
			}

			ImGui::PopStyleVar(); //ImGuiStyleVar_ItemSpacing
			ImGui::EndChild();
		}
		if (m_expanded)
		{

			uint32_t count = 0;
			for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
				count++;

			static int selection_mask = 0;

			auto clickState = DirectoryTreeViewRecursive(directoryPath, &count, &selection_mask);

			if (clickState.first)
			{
				// Update selection state
				// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
				if (ImGui::GetIO().KeyCtrl)
					selection_mask ^= 1 << clickState.second;          // CTRL+click to toggle
				else //if (!(selection_mask & (1 << clickState.second))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
					selection_mask = 1 << clickState.second;           // Click to single-select
			}
		}

		if (ImGui::CollapsingHeader("OPEN FILES"))
		{
			
		}


		ImGui::TextWrapped("TODO: This tree seems to be slowing down the program alot when lots of directories are opened. Consider optimizing. Maybe store the directory statically until the user requrests an update.");
		ImGui::End();

	}


	std::pair<bool, uint32_t> ExplorerPanel::DirectoryTreeViewRecursive(const std::filesystem::path& path, uint32_t* count, int* selection_mask)
	{
		ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

		bool any_node_clicked = false;
		uint32_t node_clicked = 0;

		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			ImGuiTreeNodeFlags node_flags = base_flags;
			const bool is_selected = (*selection_mask & 1 << (*count)) != 0;
			if (is_selected)
				node_flags |= ImGuiTreeNodeFlags_Selected;

			std::string name = entry.path().string();

			auto lastSlash = name.find_last_of("/\\");
			lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
			name = name.substr(lastSlash, name.size() - lastSlash);

			bool entryIsFile = !std::filesystem::is_directory(entry.path());
			if (entryIsFile)
				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)(*count), node_flags, name.c_str());

			if (ImGui::IsItemClicked())
			{
				node_clicked = *count;
				any_node_clicked = true;
			}

			(*count)--;

			if (!entryIsFile)
			{
				if (node_open)
				{

					auto clickState = DirectoryTreeViewRecursive(entry.path(), count, selection_mask);

					if (!any_node_clicked)
					{
						any_node_clicked = clickState.first;
						node_clicked = clickState.second;
					}

					ImGui::TreePop();
				}
				else
				{
					for (const auto& e : std::filesystem::recursive_directory_iterator(entry.path()))
						(*count)--;
				}
			}
		}

		return { any_node_clicked, node_clicked };
	}


}

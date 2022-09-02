#pragma once

#include "oak.h"
#include "UI.h"

namespace Venturi
{
    class ExplorerPanel : public Oak::Panel
    {
    public:
        ExplorerPanel() {}
        ExplorerPanel(const std::string& name, bool show, UI* parent);
        ~ExplorerPanel() = default;

        void PushLocalStyle() override;
        void OnUIRender(const char* name, bool& open);
        std::pair<bool, uint32_t> DirectoryTreeViewRecursive(const std::filesystem::path& path, uint32_t* count, int* selection_mask);

    private:
        UI* m_Parent;
        bool m_expanded = false;
};

}

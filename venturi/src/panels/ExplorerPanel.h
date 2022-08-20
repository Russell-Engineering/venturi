#pragma once

#include "oak.h"
#include "UI.h"

namespace Venturi
{
    class ExplorerPanel : public Oak::Panel
    {
    public:
        ExplorerPanel(const std::string& name, bool show, UI* parent);
        ~ExplorerPanel() = default;

        void SetLocalStyle() override;
        void OnUIRender(bool* open);
        std::pair<bool, uint32_t> DirectoryTreeViewRecursive(const std::filesystem::path& path, uint32_t* count, int* selection_mask);

    private:
        UI* m_Parent;
        Oak::Ref<Oak::Texture2D> m_RefreshIcon;

        bool m_expanded;
};

}

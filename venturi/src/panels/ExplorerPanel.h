#pragma once

#include "oak.h"

namespace Venturi
{
    class ExplorerPanel : public Oak::Panel
    {
    public:
        ExplorerPanel() {}
        ~ExplorerPanel() = default;

        void PushLocalStyle() override;
        void OnUIRender(const char* name, bool& open);
        std::pair<bool, uint32_t> DirectoryTreeViewRecursive(const std::filesystem::path& path, uint32_t* count, int* selection_mask);

    private:
        bool m_expanded = false;
};

}

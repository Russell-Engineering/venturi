
#include "imgui/imgui.h"

class ExplorerPanel
{
public:
    ExplorerPanel();
    ~ExplorerPanel() = default;

    void OnUIRender(bool* p_open);
};


#pragma once

#include "oak.h"
#include "UI.h"


namespace Venturi
{
    class DemoPanels : public Oak::Panel
    {
    public:
        DemoPanels() {}
        ~DemoPanels() = default;

        void PushLocalStyle() override;
        void OnUIRender(const char* name, bool& open) override;
    
    };
}
#pragma once

#include "oak.h"

namespace Venturi
{

    class AboutPanel : public Oak::Panel
    {
    public:
        AboutPanel() {}
        ~AboutPanel() {}

        void PushLocalStyle() override;
        void OnUIRender(const char* name, bool& open) override;
    
    private:

    };


}
#pragma once

#include "oak.h"

namespace Venturi
{
    class OptionsPanel : public Oak::Panel
    {
    public:
        OptionsPanel() {}
        ~OptionsPanel() = default;

        void PushLocalStyle() override;
        void OnUIRender(const char* name, bool& open) override;
    private:
    };

}
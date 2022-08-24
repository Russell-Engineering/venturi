#pragma once
#include "oak.h"


namespace Venturi
{
    class SampleNode : public Oak::Node
    {
    public:
        SampleNode(bool open);
        ~SampleNode() = default;

        void SetLocalStyle() override;
        void OnUIRender(bool* p_open) override;
    private:    
        inline static uint32_t m_PinID = 0;
    };
}
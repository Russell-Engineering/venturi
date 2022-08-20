#pragma once
#include "oak.h"
#include "imgui/imgui.h"
#include "implot/implot.h"
#include "UI.h"

namespace Venturi
{
    struct RTBuffer
    {
        float Span;
        ImVector<ImVec2> Data;
        RTBuffer(){
            Span = 10.0f;
            Data.reserve(10000);
        }
        void AddPoint(float x, float y)
        {
            float xmod = fmodf(x, Span);
            if (!Data.empty() && xmod < Data.back().x)
                Data.shrink(0);
            Data.push_back(ImVec2(xmod, y));
        }
    };

    class RTPlot : public Oak::Panel
    {
    public:
        RTPlot(const std::string& name, bool open, UI* parent);
        ~RTPlot() = default;

        void SetLocalStyle();
        void OnUIRender(bool* p_open);

    private:
        UI* m_Parent;
        RTBuffer rdata1;
        RTBuffer rdata2;
    };

}


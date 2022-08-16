#pragma once
#include "oak.h"
#include "imgui/imgui.h"
#include "implot/implot.h"


struct RollingBuffer 
{
    float Span;
    ImVector<ImVec2> Data;
    RollingBuffer() {
        Span = 10.0f;
        Data.reserve(2000);
    }
    void AddPoint(float x, float y) {
        float xmod = fmodf(x, Span);
        if (!Data.empty() && xmod < Data.back().x)
            Data.shrink(0);
        Data.push_back(ImVec2(xmod, y));
    }
};

class SimplePlotPanel
{
public:
    SimplePlotPanel();
    ~SimplePlotPanel() = default;

    void OnUIRender(const char* title, bool* p_open);

    RollingBuffer rdata1;
    RollingBuffer rdata2;

};


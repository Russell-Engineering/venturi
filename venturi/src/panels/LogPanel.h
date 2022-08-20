#pragma once
#include "oak/core/Panel.h"
#include "imgui/imgui.h"
#include "spdlog/sinks/base_sink.h"

#include "UI.h"

namespace Venturi
{

class LogPanel : public Oak::Panel
{
public:
    LogPanel(const std::string& name, bool show, UI* parent);
    ~LogPanel() = default;

    void OnUIRender(bool* p_open) override;
    void SetLocalStyle() override;

    void Clear();
    void AddLog(const char* fmt, ...);

public: 
    ImGuiTextBuffer Buf;
    ImGuiTextFilter Filter;
    ImVector<int>   LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool            AutoScroll; // Keep scrolling if already at the bottom.


private:
    UI* m_Parent;
    // bool m_AutoSroll;
    // ImVector<int> m_LineOffsets;
    // ImGuiTextBufffer



};

}


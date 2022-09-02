#pragma once
#include "oak/UI/Panel.h"
#include "imgui/imgui.h"

#include "UI.h"

namespace Venturi
{

class LogPanel : public Oak::Panel
{
public:
    LogPanel() {}
    LogPanel(const std::string& name, bool show, UI* parent);
    ~LogPanel() = default;

    void OnUIRender(const char* name, bool& open) override;
    void PushLocalStyle() override;

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


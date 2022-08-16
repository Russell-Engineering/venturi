#pragma once
#include "imgui/imgui.h"
#include "spdlog/sinks/base_sink.h"





class LogPanel 
{
public:
    LogPanel();
    ~LogPanel() = default;
    ImGuiTextBuffer Buf;
    ImGuiTextFilter Filter;
    ImVector<int>   LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool            AutoScroll; // Keep scrolling if already at the bottom.

    void Clear();
    void AddLog(const char* fmt, ...);
    void OnUIRender(bool* p_open);
};


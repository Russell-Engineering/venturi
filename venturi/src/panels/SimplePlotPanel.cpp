#include "SimplePlotPanel.h"


SimplePlotPanel::SimplePlotPanel()
{
}

void SimplePlotPanel::OnUIRender(const char* title, bool* p_open)
{
	ImGui::Begin(title, p_open);
    ImGui::BulletText("Move your mouse to change the data!");
    ImGui::BulletText("This example assumes 60 FPS. Higher FPS requires larger buffer size.");
    
    
    ImGui::BeginChild(title, ImVec2(0, ImGui::GetContentRegionAvail().y - 30), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImVec2 mouse = ImGui::GetMousePos();
    static float t = 0;
    t += ImGui::GetIO().DeltaTime;
    rdata1.AddPoint(t, mouse.x * 0.0005f);
    rdata2.AddPoint(t, mouse.y * 0.0005f);

    static float history = 10.0f;
    ImGui::SliderFloat("History",&history,1,30,"%.1f s");
    rdata1.Span = history;
    rdata2.Span = history;

    static ImPlotAxisFlags flags = ImPlotAxisFlags_None;


    if (ImPlot::BeginPlot("##Rolling", ImVec2(-1,ImGui::GetContentRegionAvail().y))) {
        ImPlot::SetupAxes(NULL, NULL, flags, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1,0,history, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,0,1);
        ImPlot::PlotLine("Mouse X", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 0, 2 * sizeof(float));
        ImPlot::PlotLine("Mouse Y", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 0, 2 * sizeof(float));
        ImPlot::EndPlot();
    }
    ImGui::EndChild();
    ImGui::Separator();
    ImGui::BeginChild("Target File", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Text("No File Loaded ...");
    ImGui::SameLine();
    ImGui::Button("Set Target File", ImVec2(-0.0f, 0.0f));
    ImGui::EndChild();
   
	ImGui::End();
}

#include "LogPanel.h"

namespace Venturi

{

    LogPanel::LogPanel(const std::string& name, bool show, UI* parent)
        : Oak::Panel(name, show), m_Parent(parent)
    {
        AutoScroll = true;
        Clear();
    }

    void LogPanel::Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void LogPanel::AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void LogPanel::SetLocalStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.FramePadding = ImVec2(5.0f, 5.0f);
        style.WindowPadding = ImVec2(5.0f, 5.0f);
        style.FrameBorderSize = 0.0f;
        style.WindowBorderSize = 0.0f;
    }
    void LogPanel::OnUIRender(bool* p_open = NULL)
    {

        if (!ImGui::Begin("##LOG", p_open, ImGuiWindowFlags_NoTitleBar))
        {
            ImGui::End();
            return;
        }
        // Main window.
        ImGui::Text("Todo: Inherit the spdlog::sinks::base_sink<Mutex> to use this panel as a sink for the Oak::Log");
        ImGui::BeginChild("scrolling", ImVec2(0, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().FramePadding.y), false, ImGuiWindowFlags_HorizontalScrollbar);

        const char* buf = Buf.begin();
        const char* buf_end = Buf.end();
        if (Filter.IsActive())
        {
            // In this example we don't use the clipper when Filter is enabled.
            // This is because we don't have a random access on the result on our filter.
            // A real application processing logs with ten of thousands of entries may want to store the result of
            // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
            for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
            {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                if (Filter.PassFilter(line_start, line_end))
                    ImGui::TextUnformatted(line_start, line_end);
            }
        }
        else
        {
            // The simplest and easy way to display the entire buffer:
            //   ImGui::TextUnformatted(buf_begin, buf_end);
            // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
            // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
            // within the visible area.
            // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
            // on your side is recommended. Using ImGuiListClipper requires
            // - A) random access into your data
            // - B) items all being the  same height,
            // both of which we can handle since we an array pointing to the beginning of each line of text.
            // When using the filter (in the block of code above) we don't have random access into the data to display
            // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
            // it possible (and would be recommended if you want to search through tens of thousands of entries).
            ImGuiListClipper clipper;
            clipper.Begin(LineOffsets.Size);
            while (clipper.Step())
            {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    ImGui::TextUnformatted(line_start, line_end);
                }
            }
            clipper.End();
        }

        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::Separator();
        ImGui::BeginChild("LogControls", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");

        ImGui::SameLine();
        if (ImGui::Button("Clear"))
            Clear();
        ImGui::SameLine();



        ImGui::Text("Filter");
        ImGui::SameLine();
        Filter.Draw("##Filter", ImGui::GetContentRegionAvail().x);
        ImGui::EndChild();
        ImGui::BeginChild("scrolling");
        ImGui::EndChild();
        ImGui::End();
        //ImGui::PopStyleVar();

    }


}
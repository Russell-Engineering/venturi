#pragma once

#include "wi.h"
#include "oak.h"
#include "vk.h"

#include "imgui.h"
#include "implot.h"

namespace wi::Venturi
{
    enum PlotType
    {
        NONE = 0, REALTIME, STATIC, FFT, _COUNT
    };

    static const char* PlotTypeStrings[] = { "NONE", "REALTIME", "STATIC", "FFT" };


    struct PlotSpec
    {
        wi::UUID            PlotID      = 0;
        PlotType            Type        = PlotType::NONE;
        std::string         Name        = "Plot";
        std::string         Title       = "";
        std::string         LabelX      = "";
        std::string         LabelY      = "";
        ImVec2              RangeX      = ImVec2(0.0f,1.0f);
        ImVec2              RangeY      = ImVec2(-1.1f, 1.1f);
        ImVec2              Size        = ImVec2(-1.0f,-1.0f);
        ImPlotFlags         Flags       = ImPlotFlags_None;
        ImPlotFlags         AxisFlagsX  = ImPlotFlags_None | ImPlotAxisFlags_AutoFit;
        ImPlotFlags         AxisFlagsY  = ImPlotFlags_None | ImPlotAxisFlags_AutoFit;
        ImPlotCond          AxisCondX   = ImGuiCond_Once;
        ImPlotCond          AxisCondY   = ImGuiCond_Once;
        uint32_t            Downsample  = 100;
        std::unordered_map<uint64_t, bool> IncludeData;

        PlotSpec() {}
        PlotSpec(wi::UUID _uuid) : PlotID(_uuid) {}
        ~PlotSpec() { IncludeData.clear(); }
    };

    class Plot : public RefCounted
    {
    public:
        Plot() {}
        Plot(PlotSpec _settings, Ref<DataManager> data) : m_Spec(_settings), m_DataManager(data) {}
        ~Plot();
        void Draw();
        void RegisterSeries(uint64_t id, bool show);
        void DisplaySeries(uint64_t id, bool show);
        void RemoveSeries(uint64_t id);
        //void AddNewSeries(const char* name);
        //void AddSeries(Oak::DataSeries<wi::f64_t>& _dataSeries);
        //void AddSeries(Ref<Oak::DataSeries<wi::f64_t>>& _dataSeries);
        //void RemoveSeries(Ref<Oak::DataSeries<wi::f64_t>>& _dataSeries);
        //void AddDataPoint(Oak::UUID _seriesID, wi::Vec2 _point);
        //void OverwiteDataSeriesData(Oak::UUID _seriesID, std::vector<wi::f64_t> x, std::vector<wi::f64_t> y);
        
        void UpdateSettings(PlotSpec& _settings) { m_Spec = _settings; }
        void ActiveFlags();

        const Oak::UUID& GetID() { return m_Spec.PlotID; }
        const std::string GetStrID() { return std::to_string(m_Spec.PlotID); }
        const std::string GetName() { return m_Spec.Name; }

        PlotSpec& GetSettings() { return m_Spec; }
    private:
        PlotSpec m_Spec;
        Ref<DataManager> m_DataManager;
    };

}

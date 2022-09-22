#pragma once

#include "oak.h"
#include "vk.h"

#include "imgui.h"
#include "implot.h"

namespace Venturi
{
    enum class PlotType
    {
        NONE = 0, REALTIME, STATIC, FFT
    };

    struct SignalDataSeries : public Oak::DataSeries<vk::f64_t>
    {
        vk::Signals::SignalGenerator  Signal = {};
      
        SignalDataSeries(const char* _name, uint32_t _buff)
            : DataSeries<vk::f64_t>(_name, _buff)
        {
        }
        SignalDataSeries(const char* _name, uint32_t _buff, vk::Signals::SignalSpec& _spec)
            : DataSeries<vk::f64_t>(_name, _buff), Signal(_spec)
        {
        }
        ~SignalDataSeries()
        {
        }
        
        //void OnUpdate(vk::f64_t t);
        void OnUpdate();
        void SetWaveForm(vk::Signals::Waveform wave) { Signal.SetWaveform(wave); }

    };

    struct PlotSpec
    {
        Oak::UUID           PlotID      = 0;
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

        PlotSpec() {}
        PlotSpec(Oak::UUID _uuid) : PlotID(_uuid) {}
    };

    class Plot : public Oak::RefCounted
    {
    public:
        Plot() {}
        Plot(PlotSpec _settings) : m_Spec(_settings){}
        ~Plot();
        void Draw();
        void AddNewSeries(const char* name);
        void AddSeries(Oak::DataSeries<vk::f64_t>& _dataSeries);
        void AddSeries(Oak::Ref<Oak::DataSeries<vk::f64_t>>& _dataSeries);
        void RemoveSeries(Oak::UUID _seriesID);
        void AddDataPoint(Oak::UUID _seriesID, vk::Vec2 _point);
        void OverwiteDataSeriesData(Oak::UUID _seriesID, std::vector<vk::f64_t> x, std::vector<vk::f64_t> y);
        
        void UpdateSettings(PlotSpec& _settings) { m_Spec = _settings; }
        void ActiveFlags();

        const Oak::UUID& GetID() { return m_Spec.PlotID; }
        const std::string GetStrID() { return std::to_string(m_Spec.PlotID); }
        const std::string GetName() { return m_Spec.Name; }

        PlotSpec& GetSettings() { return m_Spec; }
        std::map<Oak::UUID, Oak::Ref<Oak::DataSeries<vk::f64_t>>>& GetDataSet() { return m_DataSet; }
    private:
        PlotSpec m_Spec;
        std::map<Oak::UUID, Oak::Ref<Oak::DataSeries<vk::f64_t>>> m_DataSet;

        bool m_Running;
        bool m_Paused;

    };

}

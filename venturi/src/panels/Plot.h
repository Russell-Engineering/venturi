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

    template<typename vec2T>
    struct DataSeries : public Oak::RefCounted
    {

        Oak::UUID           SeriesID   = 0;
        std::string         Name       = "";
        uint32_t            Buffersize = 0;
        uint32_t            Offset     = 0;
        std::vector<vec2T>  Data;

        DataSeries() : Buffersize(10000) {}
        DataSeries(uint32_t _buffersize);
        DataSeries(Oak::UUID _uuid, uint32_t _buffersize);
        DataSeries(Oak::UUID _uuid, const char* _name, uint32_t _buffersize);
        ~DataSeries();
        
        //DataSeries(DataSeries& other);
        DataSeries(const DataSeries& other);
        DataSeries(DataSeries&& other) noexcept;
        

        DataSeries& operator=(DataSeries& other);
        const DataSeries& operator=(const DataSeries& other);
        DataSeries& operator=(DataSeries&& other) noexcept;
        
        void AddPoint(vec2T _point);
        void Erase();

    };

    struct PlotSettings
    {
        Oak::UUID           PlotID      = 0;
        PlotType            Type        = PlotType::NONE;
        std::string         Name        = "";
        std::string         Title       = "";
        std::string         LabelX      = "";
        std::string         LabelY      = "";
        ImVec2              RangeX      = ImVec2(0.0f,1.0f);
        ImVec2              RangeY      = ImVec2(-1.1f, 1.1f);
        ImVec2              Size        = ImVec2(-1.0f,-1.0f);
        ImPlotFlags         Flags       = ImPlotFlags_None;
        ImPlotFlags         AxisFlagsX  = ImPlotFlags_None;
        ImPlotFlags         AxisFlagsY  = ImPlotFlags_None;
        ImPlotCond          AxisCondX   = ImGuiCond_Once;
        ImPlotCond          AxisCondY   = ImGuiCond_Once;

        PlotSettings() {}
        PlotSettings(Oak::UUID _uuid) : PlotID(_uuid) {}
    };

    class Plot : public Oak::RefCounted
    {
    public:
        Plot() {}
        Plot(PlotSettings _settings) : m_Settings(_settings){}
        ~Plot();
        void Draw();
        void AddNewSeries(const char* name);
        void AddSeries(DataSeries<vk::Vec2>& _dataSeries);
        void AddSeries(Oak::Ref<DataSeries<vk::Vec2>>& _dataSeries);
        void RemoveSeries(Oak::UUID _seriesID);
        void AddDataPoint(Oak::UUID _seriesID, vk::Vec2 _point);
        void OverwiteDataSeriesData(Oak::UUID _seriesID, std::vector<vk::Vec2> _newData);
        
        void UpdateSettings(PlotSettings& _settings) { m_Settings = _settings; }
        void ActiveFlags();

        const Oak::UUID& GetID() { return m_Settings.PlotID; }
        const std::string GetStrID() { return std::to_string(m_Settings.PlotID); }
        const std::string GetName() { return m_Settings.Name; }

        PlotSettings& GetSettings() { return m_Settings; }
        std::map<Oak::UUID, Oak::Ref<DataSeries<vk::Vec2>>>& GetDataSet() { return m_DataSet; }
    private:
        PlotSettings m_Settings;
        std::map<Oak::UUID, Oak::Ref<DataSeries<vk::Vec2>>> m_DataSet;

        bool m_Running;
        bool m_Paused;

    };

}

#pragma once

#include "oak.h"
#include "vk.h"

#include "implot/implot.h"
#include "ImGui/imgui.h"
#include "oak/imgui/imgui_stdlib.h"
#include "Plot.h"

#include <map>
#include <memory>
 
namespace Venturi
{
    class PlotPanel : public Oak::Panel
    {
    public:

        PlotPanel();
        ~PlotPanel();
        void PushLocalStyle() override;
        void OnUIRender(const char* name, bool& open) override;

        void DrawToolBar();
        void DrawPlots();
        void DrawPlotOptions();


        void AddPlot(); 
        void RemovePlot(Oak::UUID uuid);
        void CreatDataSeries();
        void AddSeries(DataSeries<vk::Vec2>& _dataSeries);

        void UpdateData();

    private:
        uint32_t id; 
        inline static uint32_t m_idGenerator = 0;
        
        Oak::Scope<vk::Clock> m_clock;
        Oak::Scope<vk::Signals::SignalGenerator> m_SigGen;
        
        std::vector<Oak::Ref<Plot>> m_Plots;
        //std::map<Oak::UUID, DataSeries<vk::Vec2>> m_DataSet;
        std::map<Oak::UUID, Oak::Ref<DataSeries<vk::Vec2>>> m_DataSet;


        bool m_Running = false;
        bool m_Recording = true;
        bool m_ShowPlotOptions = false;
        float m_History = 30.0f;
        float m_freq = 2.0; // * M_PI
        static inline const char* const m_waveforms[] = { "RECTANGULAR", "SINE", "COSINE", "SQUARE", "SAWTOOTH", "RANDOM"};
        int m_selectedWaveform = 1;

    };
}
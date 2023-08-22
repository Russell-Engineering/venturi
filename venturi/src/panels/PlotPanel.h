#pragma once

#include "oak.h"
#include "vk.h"
#include "wi.h"

#include "implot/implot.h"
#include "ImGui/imgui.h"
#include "oak/imgui/imgui_stdlib.h"
#include "Plot.h"

#include "events/DataEvents.h"
#include "series/SeriesIO.h"

#include <map>
#include <memory>
 
namespace wi::Venturi
{
    class PlotPanel : public Oak::Panel
    {
    public:

        PlotPanel(Ref<DataManager> dataset);
        ~PlotPanel();
        
        void PushLocalStyle() override;
        void OnUIRender(const char* name, bool& open) override;
        virtual void OnEvent(Oak::Event& e) override;

        bool OnStream(DataStreamEvent& e);
        bool OnClear(DataClearEvent& e);
        bool OnCreate(DataSeriesCreateEvent& e);
        bool OnRemove(DataSeriesRemoveEvent& e);


        void DrawToolBar();
        void DrawPlots();
        void DrawPlotOptions();

        void AddPlot(PlotSpec& _settings); 
        void CreatePlot();
        void RemovePlot(Oak::UUID& _uuid);

        void UI_ShowPlotCreationPopUp();
        void UI_ShowDataCreationPopUp();

    private:
        uint32_t id; 
        inline static uint32_t m_idGenerator = 0;
        
        std::vector<Ref<Plot>> m_Plots;
        Ref<DataManager> m_DataManager;
        
        bool m_ShowPlotOptions = false;
        bool m_CreatePlotPopup = false;
        
        // move all this somewhwer else
        float m_History = 30.0f;
        float m_freq = 2.0; // * M_PI
        static inline const char* const m_waveforms[] = { "RECTANGULAR", "SINE", "COSINE", "SQUARE", "SAWTOOTH", "RANDOM"};
        int m_selectedWaveform = 1;

    

    };
}

#include "Plot.h"
#include <mutex>


namespace wi::Venturi
{

    std::string PlotTypeToString(PlotType type)
    {
        switch (type)
        {
        case PlotType::REALTIME:  return "Realtime";
        case PlotType::STATIC:    return "Static";
        case PlotType::FFT:       return "FFT";
        default:                  return "Unknown Plot Type!";
        }
    }
    Plot::~Plot()
    {
        LOG_TRACE_TAG("~Plot", "Destroying plot {0} ('{1}')", GetID(), GetName());
        //m_DataSet.clear();
    }
    void Plot::Draw()
    {
        ActiveFlags();
        std::string plot_title = m_Spec.Title.empty() ? "##untitled" : m_Spec.Title;

        if (ImPlot::BeginPlot(plot_title.c_str(), m_Spec.Size, m_Spec.Flags))
        {
            ImPlot::SetupAxes(m_Spec.LabelX.c_str(), m_Spec.LabelY.c_str(), m_Spec.AxisFlagsX , m_Spec.AxisFlagsY);
            ImPlot::SetupAxisLimits(ImAxis_X1, m_Spec.RangeX[0], m_Spec.RangeX[1], m_Spec.AxisCondX);
            ImPlot::SetupAxisLimits(ImAxis_Y1, m_Spec.RangeY[0], m_Spec.RangeY[1], m_Spec.AxisCondY);
            for (const auto& [id, show] : m_Spec.IncludeData)
            {
                if (show)
                { 
                    DataSeriesSpec* _series = m_DataManager->GetSeriesSpec(id);
                    ImPlot::SetNextLineStyle({ (float)_series->Color.x, (float)_series->Color.y, (float)_series->Color.z, (float)_series->Color.w}); // maybe add to plot getColor(id) and keep this separate? tbd
                    if (_series && _series->Data)
                    {
                        uint32_t& offset = _series->Data->offset;
                        uint32_t  length = (uint32_t)_series->Data->RawData->x.size() - offset;
                        uint32_t  stride = m_Spec.Downsample * sizeof(_series->Data->RawData->y[0]);
                        // ImPlot::PlotLine(_series->Name.c_str(), &_series->Data->RawData->x[0], &_series->Data->RawData->y[0], (int)_series->Data->RawData->y.size(), 0, 0, sizeof(_series->Data->RawData->y[0]));
                        ImPlot::PlotLine(_series->Name.c_str(), &_series->Data->RawData->x[0], &_series->Data->RawData->y[0], (int)_series->Data->RawData->y.size()/m_Spec.Downsample, 0, 0, stride);
                        offset += length;
                    }
                }
            }
            ImPlot::EndPlot();
        }
    }

    void Plot::RegisterSeries(uint64_t id, bool show)
    {
        if (m_Spec.IncludeData.find(id) != m_Spec.IncludeData.end())
        {
            LOG_WARN_TAG("Plot::RegisterSeries", "A data series with id '{0}' has already been added to this plot.", id);
            return;
        }
        m_Spec.IncludeData[id] = show;
    }

    void Plot::DisplaySeries(uint64_t id, bool show)
    {
        if (m_Spec.IncludeData.find(id) == m_Spec.IncludeData.end())
        {
             LOG_ERROR_TAG("Plot::DisplaySeries", "Could not find data series with id {0}", id);
             return;
        }
        m_Spec.IncludeData[id] = show;
    }

    void Plot::RemoveSeries(uint64_t id)
    {
        if (m_Spec.IncludeData.find(id) == m_Spec.IncludeData.end())
        {
            LOG_ERROR_TAG("Plot::Remove", "Could not find data series with id {0}", id);
            return;
        }
        m_Spec.IncludeData.erase(id);
    }

    
    void Plot::ActiveFlags()
    {
        // create an ActiveFlags function. 
        switch (m_Spec.Type)
        {
        case PlotType::REALTIME:
        {
            // might have to rest flags every frame incase the plot type changes. 
            //m_Spec.AxisCondX = ImGuiCond_Always;
            break;
        }
        case PlotType::STATIC:
        {
            break;
        }
        case PlotType::FFT:
        {
            break;
        }
        }
    }


}

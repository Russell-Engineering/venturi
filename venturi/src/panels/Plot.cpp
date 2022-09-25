
#include "Plot.h"
#include <mutex>


namespace Venturi
{
    std::mutex mutex;
    //void SignalDataSeries::OnUpdate(vk::f64_t t)
    void SignalDataSeries::OnUpdate()
    {
        std::vector<vk::f64_t> tmp;
        
        //std::swap(buff, tmp);
        //tmp.reserve(buff.size());
        tmp = std::move(buff);
        //buff.clear();

        for (int i = 0; i < tmp.size(); i++) // process new values of y
        {
            AddPoint(tmp[i], Signal.Generate(tmp[i]));
        }
        tmp.clear();
        //AddPoint(vk::Vec2(t, y));
        //OAK_TRACE("Adding point ({0},{1}) to Series {2}", t, y, Name);
    }

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
        OAK_TRACE_TAG("~Plot", "Destroying plot {0} ('{1}')", GetID(), GetName());
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
            for (const auto& [_id, _series] : m_DataSet)
            {
                ImPlot::SetNextLineStyle(_series->Color);
                ImPlot::PlotLine(_series->Name.c_str(), &_series->x[0], &_series->y[0], _series->y.size(), 0, _series->Offset, sizeof(_series->y[0]));
                //ImPlot::PlotLine(_series.Name.c_str(), &_series.Data[0].x, &_series.Data[0].y, _series.Data.size(), 0, _series.Offset, sizeof(_series.Data[0]));

            }
            ImPlot::EndPlot();
        }
    }

    void Plot::AddNewSeries(const char* name)
    {
        m_DataSet.emplace(Oak::DataSeries<vk::f64_t>::GetNextID(), &Oak::DataSeries<vk::f64_t>(name, 25000));
    }

    void Plot::AddSeries(Oak::DataSeries<vk::f64_t>& _dataSeries)
    {
        OAK_TRACE_TAG("Plot::AddSeries", "Adding DataSeries {0} ('{1}') to Plot {2} ('{3}') ", _dataSeries.SeriesID, _dataSeries.Name.c_str(), GetID(), GetName());
        if (m_DataSet.find(_dataSeries.SeriesID) != m_DataSet.end())
        {
            OAK_WARN_TAG("Plot::AddSeries", "........DataSeries {0} ('{1}') already exsists!",_dataSeries.SeriesID, _dataSeries.Name.c_str());
            return;
        }
        m_DataSet[_dataSeries.SeriesID] = &_dataSeries;
    }
    void Plot::AddSeries(Oak::Ref<Oak::DataSeries<vk::f64_t>>& _dataSeries)
    {
        OAK_TRACE_TAG("Plot::AddSeries", "Adding DataSeries {0} ('{1}') to Plot {2} ('{3}') ", _dataSeries->SeriesID, _dataSeries->Name.c_str(), GetID(), GetName());
        if (m_DataSet.find(_dataSeries->SeriesID) != m_DataSet.end())
        {
            OAK_WARN_TAG("Plot::AddSeries", "........DataSeries {0} ('{1}') already exsists!", _dataSeries->SeriesID, _dataSeries->Name.c_str());
            return;
        }
        m_DataSet[_dataSeries->SeriesID] = _dataSeries;
    }

    void Plot::RemoveSeries(Oak::Ref<Oak::DataSeries<vk::f64_t>>& _dataSeries)
    {
        if (m_DataSet.find(_dataSeries->SeriesID) == m_DataSet.end())
            return;
        m_DataSet.erase(_dataSeries->SeriesID);
    }

    void Plot::AddDataPoint(Oak::UUID _seriesID, vk::Vec2 _point)
    {
        if (m_DataSet.find(_seriesID) == m_DataSet.end())
        {
            OAK_WARN_TAG("Plot::AddDataPoint", "Data set does not contain Series with ID {0}", _seriesID);
            return;
        }
        m_DataSet.at(_seriesID)->x.push_back(_point.x);
        m_DataSet.at(_seriesID)->y.push_back(_point.y);
       // m_DataSet.at(_seriesID).Data.push_back(_point);
    }

    void Plot::OverwiteDataSeriesData(Oak::UUID _seriesID, std::vector<vk::f64_t> x, std::vector<vk::f64_t> y)
    {
        if (m_DataSet.find(_seriesID) == m_DataSet.end())
        {
            OAK_WARN_TAG("Plot::AddDataPoint", "Data set does not contain Series with ID {0}", _seriesID);
            return;
        }
        m_DataSet.at(_seriesID)->Erase();
        m_DataSet.at(_seriesID)->Offset = 0;
        m_DataSet.at(_seriesID)->Buffersize = x.size();
        m_DataSet.at(_seriesID)->x.reserve(x.size()); // might not be necessary
        m_DataSet.at(_seriesID)->y.reserve(x.size()); // might not be necessary
        m_DataSet.at(_seriesID)->x = x;
        m_DataSet.at(_seriesID)->y = y;
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
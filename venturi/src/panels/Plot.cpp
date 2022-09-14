
#include "Plot.h"

namespace Venturi
{
    template<typename T>
    DataSeries<T>::DataSeries(uint32_t _buffersize)
        : Buffersize(_buffersize)
    {
        OAK_TRACE_TAG("DataSeries","Created with size {0}", Buffersize );
        Data.reserve(Buffersize);
    }

    template<typename T>
    DataSeries<T>::DataSeries(Oak::UUID _uuid, uint32_t _buffersize)
        : SeriesID(_uuid), Buffersize(_buffersize)
    {
        OAK_TRACE_TAG("DataSeries", "Created with size {0}, ID {1}", Buffersize, SeriesID);  
        Data.reserve(Buffersize);
    }

    template<typename T>
    DataSeries<T>::DataSeries(Oak::UUID _uuid, const char* _name, uint32_t _buffersize)
        : SeriesID(_uuid), Name(_name), Buffersize(_buffersize)
    {
        OAK_TRACE_TAG("DataSeries", "Created with size {0}, ID {1}, Name '{2}'", Buffersize, SeriesID, Name);  
        Data.reserve(Buffersize);
    }

    template<typename T>
    DataSeries<T>::~DataSeries()
    {
        OAK_TRACE_TAG("DataSeries", "Destryed {0} ('{1}') References Remaining: {2}", SeriesID, Name, GetRefCount());
        //Data.clear();
    }

    template<typename T>
    DataSeries<T>::DataSeries(const DataSeries<T>& other)
        : SeriesID(other.SeriesID), Name(other.Name), Buffersize(other.Buffersize), Offset(other.Offset), Data(other.Data)
    {
        OAK_TRACE_TAG("DataSeries", "Copied {0} ('{1}')", SeriesID, Name);
    }

    template<>
    DataSeries<vk::Vec2>::DataSeries(DataSeries<vk::Vec2>&& other) noexcept
        : SeriesID(other.SeriesID), Name(other.Name), Buffersize(other.Buffersize), Offset(other.Offset), Data(other.Data)
    {
        OAK_TRACE_TAG("DataSeries", "Moved {0} ('{1}')", SeriesID, Name);
        other.SeriesID = 0;
        other.Name = "";
        other.Buffersize = 0;
        other.Offset = 0;
        other.Data.resize(0);
    }


    template<typename T>
    DataSeries<T>& DataSeries<T>::operator=(DataSeries<T>& other) 
    {
        SeriesID = other.SeriesID;
        Name = other.Name;
        Buffersize = other.Buffersize;
        Offset = other.Offset;
        Data = other.Data;
        OAK_TRACE_TAG("DataSeries", "copy assigning {0} ('{1}')", SeriesID, Name);
        return *this;
    }

    template<typename T>
    const DataSeries<T>& DataSeries<T>::operator=( const DataSeries<T>& other )
    {
        SeriesID = other.SeriesID;
        Name = other.Name;
        Buffersize = other.Buffersize;
        Offset = other.Offset;
        Data = other.Data;
        OAK_TRACE_TAG("DataSeries", "copy assigning {0} ('{1}')", SeriesID, Name);
        return *this;
    }

    template<typename T>
    DataSeries<T>& DataSeries<T>::operator=(DataSeries<T>&& other) noexcept
    {
        if (this != &other)
        {
            SeriesID   = std::move(other.SeriesID);
            Name       = std::move(other.Name);
            Buffersize = std::move(other.Buffersize);
            Offset     = std::move(other.Offset);
            Data       = std::move(other.Data);

            other.SeriesID = 0;
            other.Name = "";
            other.Buffersize = 0;
            other.Offset = 0;
            other.Data.clear();
        }
        OAK_TRACE_TAG("DataSeries", "Move assigning {0} ('{1}')", SeriesID, Name);
        return *this;
    }

    template<>
    void DataSeries<vk::Vec2>::AddPoint(vk::Vec2 _point)
    {
        if (Data.size() < Buffersize)
        {
            Data.push_back(_point);
            return;
        }
        Buffersize += 10000;
        OAK_WARN_TAG("DataSeries::AddPoint", "Increasing Buffersie of {0} ('{1}') to {2}", SeriesID, Name, Buffersize);
        Data.reserve(Buffersize);
        Data.push_back(_point);
    }
   
    template<>
    void DataSeries<vk::Vec2>::Erase()
    {
        if (Data.size() > 0)
        {
            Data.resize(0);
            Offset = 0;
        }
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
        OAK_TRACE_TAG("Plot::~Plot()", "Destroying plot {0} ('{1}')", GetID(), GetName());
        //m_DataSet.clear();
    }
    void Plot::Draw()
    {
        ActiveFlags();
        std::string plot_title = m_Settings.Title.empty() ? "##untitled" : m_Settings.Title;

        if (ImPlot::BeginPlot(plot_title.c_str(), m_Settings.Size, m_Settings.Flags))
        {
            ImPlot::SetupAxes(m_Settings.LabelX.c_str(), m_Settings.LabelY.c_str(), m_Settings.AxisFlagsX , m_Settings.AxisFlagsY);
            ImPlot::SetupAxisLimits(ImAxis_X1, m_Settings.RangeX[0], m_Settings.RangeX[1], m_Settings.AxisCondX);
            ImPlot::SetupAxisLimits(ImAxis_Y1, m_Settings.RangeY[0], m_Settings.RangeY[1], m_Settings.AxisCondY);
            for (const auto& [_id, _series] : m_DataSet)
            {

                ImPlot::PlotLine(_series->Name.c_str(), &_series->Data[0].x, &_series->Data[0].y, _series->Data.size(), 0, _series->Offset, sizeof(_series->Data[0]));
                //ImPlot::PlotLine(_series.Name.c_str(), &_series.Data[0].x, &_series.Data[0].y, _series.Data.size(), 0, _series.Offset, sizeof(_series.Data[0]));

            }
            ImPlot::EndPlot();
        }
    }

    void Plot::AddNewSeries(const char* name)
    {
        Oak::UUID id = Oak::UUID();
        m_DataSet.emplace(id, &DataSeries<vk::Vec2>(id, name, 25000));
    }

    void Plot::AddSeries(DataSeries<vk::Vec2>& _dataSeries)
    {
        OAK_TRACE_TAG("Plot::AddSeries", "Adding DataSeries {0} ('{1}') to Plot {2} ('{3}') ", _dataSeries.SeriesID, _dataSeries.Name.c_str(), GetID(), GetName());
        if (m_DataSet.find(_dataSeries.SeriesID) != m_DataSet.end())
        {
            OAK_WARN_TAG("Plot::AddSeries", "........DataSeries {0} ('{1}') already exsists!",_dataSeries.SeriesID, _dataSeries.Name.c_str());
            return;
        }
        m_DataSet[_dataSeries.SeriesID] = &_dataSeries;
    }
    void Plot::AddSeries(Oak::Ref<DataSeries<vk::Vec2>>& _dataSeries)
    {
        OAK_TRACE_TAG("Plot::AddSeries", "Adding DataSeries {0} ('{1}') to Plot {2} ('{3}') ", _dataSeries->SeriesID, _dataSeries->Name.c_str(), GetID(), GetName());
        if (m_DataSet.find(_dataSeries->SeriesID) != m_DataSet.end())
        {
            OAK_WARN_TAG("Plot::AddSeries", "........DataSeries {0} ('{1}') already exsists!", _dataSeries->SeriesID, _dataSeries->Name.c_str());
            return;
        }
        m_DataSet[_dataSeries->SeriesID] = _dataSeries;
    }

    void Plot::RemoveSeries(Oak::UUID _seriesID)
    {
        if (m_DataSet.find(_seriesID) == m_DataSet.find(_seriesID))
            return;
        m_DataSet.erase(_seriesID);
    }

    void Plot::AddDataPoint(Oak::UUID _seriesID, vk::Vec2 _point)
    {
        if (m_DataSet.find(_seriesID) == m_DataSet.end())
        {
            OAK_WARN_TAG("Plot::AddDataPoint", "Data set does not contain Series with ID {0}", _seriesID);
            return;
        }
        m_DataSet.at(_seriesID)->Data.push_back(_point);
       // m_DataSet.at(_seriesID).Data.push_back(_point);
    }

    void Plot::OverwiteDataSeriesData(Oak::UUID _seriesID, std::vector<vk::Vec2> _newData)
    {
        if (m_DataSet.find(_seriesID) == m_DataSet.end())
        {
            OAK_WARN_TAG("Plot::AddDataPoint", "Data set does not contain Series with ID {0}", _seriesID);
            return;
        }
        m_DataSet.at(_seriesID)->Data.clear();
        m_DataSet.at(_seriesID)->Offset = 0;
        m_DataSet.at(_seriesID)->Buffersize = _newData.size();
        m_DataSet.at(_seriesID)->Data.reserve(_newData.size()); // might not be necessary
        m_DataSet.at(_seriesID)->Data = _newData;
    }

    void Plot::ActiveFlags()
    {
        // create an ActiveFlags function. 
        switch (m_Settings.Type)
        {
        case PlotType::REALTIME:
        {
            // might have to rest flags every frame incase the plot type changes. 
            m_Settings.AxisCondX = ImGuiCond_Always;
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
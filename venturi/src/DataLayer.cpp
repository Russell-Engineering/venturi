#include "DataLayer.h"
#include "series/OscillatorIO.h"

namespace wi::Venturi
{
    DataLayer::DataLayer(DataManager& dataManager) 
        : Layer("Data Layer"), m_DataManager(&dataManager)
    {
        m_Clock = m_DataManager->GetClock();
    }

    DataLayer::~DataLayer()
    {
    }
    void DataLayer::OnAttach()
    {   
        // vk::Signals::SignalSpec sigspec;
        // sigspec.amp = 1.5f;
        // sigspec.offset = 2.5f;
        // sigspec.waveform = vk::Signals::SQUARE;
        // sigspec.samplerate = 1.0f;
        // Ref<OscillatorIO> io = Ref<OscillatorIO>::Create(m_Clock, sigspec);
        // m_DataManager->AddSeries("testID", "testDisplayName", io, (size_t)100000, (size_t)100);
    }

    void DataLayer::OnDetach()
    {
    }

    void DataLayer::OnUpdate(Oak::Timestep ts)
    {
        m_DataManager->OnUpdate(ts);
    }

    void DataLayer::OnEvent(Oak::Event& e)
    {
        Oak::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<DataStreamEvent>(OAK_BIND_EVENT_FN(DataLayer::OnStream));
        dispatcher.Dispatch<DataClearEvent>(OAK_BIND_EVENT_FN(DataLayer::OnClear));
        dispatcher.Dispatch<DataSeriesCreateEvent>(OAK_BIND_EVENT_FN(DataLayer::OnCreate));
        dispatcher.Dispatch<DataSeriesRemoveEvent>(OAK_BIND_EVENT_FN(DataLayer::OnRemove));
    }

    bool DataLayer::OnStream(DataStreamEvent& e)
    {
        LOG_TRACE_TAG("DataLayer::OnStream", e.ToString());
        m_DataManager->Stream(e.GetStreamCmd());
        return false;
    }

    bool DataLayer::OnClear(DataClearEvent& e)
    {
        LOG_TRACE_TAG("DataLayer::OnClear", e.ToString());
        m_DataManager->Clear();
        return false;
    }
    
    bool DataLayer::OnRemove(DataSeriesRemoveEvent& e)
    {
        m_DataManager->RemoveSeries(e.ID);
        return true;
    }

    bool DataLayer::OnCreate(DataSeriesCreateEvent& e)
    {

        LOG_TRACE_TAG("DataLayer::OnCreate", e.ToString());
        m_DataManager->AddSeries(e.GetSeriesSpec());
        // m_DataManager->AddSeries(DataSeriesSpec());
        return true;
    }
}

#pragma once
#include "oak.h"

namespace wi::Venturi
{
    // need to use the Oak namespace to have access to the Macros
    
    using namespace Oak;
    class DataStreamEvent : public Oak::Event
    {
    public:
        DataStreamEvent(bool streamCmd)
            : m_StreamCmd(streamCmd)
        {
        }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "DataStreamEvent: streaming = ";
            ss << m_StreamCmd;
            return ss.str();
        }

        bool GetStreamCmd() { return m_StreamCmd;}

        EVENT_CLASS_TYPE(DataStreamEvent)
        EVENT_CLASS_CATEGORY(EventCategoryData)
    private:
        bool m_StreamCmd = false;

    };

    class DataClearEvent : public Oak::Event
    {
    public:
        DataClearEvent()
        {
        }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "DataClearEvent: Clearing Data";
            return ss.str();
        }

        EVENT_CLASS_TYPE(DataClearEvent)
        EVENT_CLASS_CATEGORY(EventCategoryData)
    };

    class DataRecordEvent : public Oak::Event
    {
    public:
        DataRecordEvent(bool recordCmd)
            : m_Recording(recordCmd)
        {
        }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "DataRecordEvent: Recording Data = ";
            ss << m_Recording;
            return ss.str();
        }

        EVENT_CLASS_TYPE(DataRecordEvent)
        EVENT_CLASS_CATEGORY(EventCategoryData)
    private:
        bool m_Recording = false;

    };

    class DataSeriesCreateEvent : public Oak::Event
    {
    public:
        DataSeriesCreateEvent(const DataSeriesSpec spec) : m_Spec(spec)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "DataSeriesCreateEvent: Create DataSeries with ID ";
            ss << m_Spec.ID;
            return ss.str();
        }
        
        DataSeriesSpec GetSeriesSpec() { return m_Spec; }

        EVENT_CLASS_TYPE(DataSeriesCreateEvent)
        EVENT_CLASS_CATEGORY(EventCategoryData)
    private:
        DataSeriesSpec m_Spec;
    };

    class DataSeriesRemoveEvent : public Oak::Event
    {
    public:
        DataSeriesRemoveEvent(uint64_t id)
            : ID(id)
        {
        }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "DataClearEvent: Removing DataSeries with ID";
            ss << ID;
            return ss.str();
        }

        EVENT_CLASS_TYPE(DataSeriesRemoveEvent)
        EVENT_CLASS_CATEGORY(EventCategoryData)
     
        uint64_t ID;
    };



}

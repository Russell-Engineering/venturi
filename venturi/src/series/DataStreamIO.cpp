#include "DataStreamIO.h"

namespace wi::Venturi 
{
DataStreamIO::DataStreamIO()
{
    // if(!m_Connection.connect())
    // {
    //      LOG_ERROR_TAG("DataStreamIO::DataStreamIO", "Could not establish connection to {0} on port {1}", m_Connection.ip, m_Connection.port);
    // }
    // m_Connection.
}

DataStreamIO::~DataStreamIO()
{
}

DataBuffer DataStreamIO::Load(std::atomic<bool>& streaming, size_t buffer)
{
    DataBuffer buff;

    return buff;
}

// 
};


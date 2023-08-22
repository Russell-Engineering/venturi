#pragma once
#include "DataStreamIO.h"
#include "OscillatorIO.h"
//#include "FileStreamIO.h"

namespace wi::Venturi
{
    class UndefinedIO : public DataIO
    {
    public:
        DataBuffer Load(std::atomic<bool>& streaming, size_t buffsize)
        { 
            DataBuffer  buff;
            buff.reserve(buffsize);
            return buff;
        }
        bool Write() { return false; }
    };

enum class SeriesIO {
    UNDEFINED = 0,
    OSCIILLATOR, 
    DATASTREAM,
    FILESTREAM, 
    _COUNT
};

static SeriesIO GetIOType(Ref<DataIO> io)
{
    if (typeid(*io.Raw()) == typeid(OscillatorIO))
        return SeriesIO::OSCIILLATOR;
    if (typeid(*io.Raw()) == typeid(DataStreamIO))
        return SeriesIO::DATASTREAM;
    if (typeid(*io.Raw()) == typeid(UndefinedIO))
        return SeriesIO::UNDEFINED;
    return SeriesIO::UNDEFINED;

}

static const char* SeriesIOStrings[] = { "UNDEFINED", "OSCILLATOR", "DATASTREAM", "FILESTREAM" };


};

#pragma once

#include "wi.h"
#include "vk.h"

namespace wi::Venturi 
{
class DataStreamIO : public DataIO
    {
    public:
        DataStreamIO();
        ~DataStreamIO();

        virtual DataBuffer Load(std::atomic<bool>& streaming, size_t buffsize) override;
        virtual bool Write() override {return false;} 

    private:
        // Connection m_Connection;
        //
    };
};

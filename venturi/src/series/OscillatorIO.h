#pragma once

#include "wi.h"
#include "vk.h"

namespace wi::Venturi
{

    class OscillatorIO : public DataIO
    {
    public:
        OscillatorIO(Ref<Clock> clock, vk::Signals::SignalSpec& spec);
        ~OscillatorIO() {};

        virtual DataBuffer Load(std::atomic<bool>& streaming, size_t buffsize) override; // load the data from the appropriate stream
        virtual bool Write() override { return false; }

        vk::Signals::SignalGenerator Signal;
        vk::Signals::SignalSpec SigSpec;
        
    private:
        Ref<Clock> m_Clock;
    };


}

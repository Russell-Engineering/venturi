#include "OscillatorIO.h"

namespace wi::Venturi
{
    OscillatorIO::OscillatorIO(Ref<Clock> clock, vk::Signals::SignalSpec& spec)
        : DataIO(), m_Clock(clock), Signal(vk::Signals::SignalGenerator(spec)), SigSpec(spec)
    {
    }
    
    DataBuffer OscillatorIO::Load(std::atomic<bool>& streaming, size_t buffsize)
    {
        DataBuffer  buff;
        buff.reserve(buffsize);

        for (int i = 0; i < buffsize; i++)
        {
            if (!streaming)
                break;
            f64_t x = m_Clock->poll(SigSpec.samplerate);
            f64_t y = Signal.Generate(x);
            buff.x.push_back(x);
            buff.y.push_back(y);

        }
        
        return buff;

    }


};

#pragma once

#include "Write.h"
#include "Pipe.h"

template <class Stream>
class Out {
public:
    Out(Stream& stream) : stream_(stream) {}

    template <class Adapter>
    auto MakeSomeMagic(const Adapter& adapter) const {
        return adapter | Write(stream_, '\n');
    }
private:
    Stream& stream_;
};
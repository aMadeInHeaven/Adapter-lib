#pragma once

#include "Pipe.h"

class DropNullopt {
public:
    template<class Adapter>
    auto MakeSomeMagic(const Adapter& adapter) const {
        return adapter | Filter([](auto elem){ return elem != std::nullopt; });
    }
};

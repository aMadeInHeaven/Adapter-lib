#pragma once

#include "Range.h"
#include "Iterators/EnumerateIterator.h"

class Enumerate {
public:
    template <class Adapter>
    auto MakeSomeMagic(const Adapter& adapter) const {
        using Iterator = decltype(adapter.begin());
        return Range<EnumerateIterator<Iterator>>(
            EnumerateIterator<Iterator>(adapter.begin(), 0),
            EnumerateIterator<Iterator>(adapter.end(), 0)
        );
    }
};

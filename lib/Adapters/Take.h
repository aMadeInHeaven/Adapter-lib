#pragma once

#include "Range.h"
#include "Iterators/TakeIterator.h"

class Take {
public:
    Take(size_t limit) : limit_(limit) {}

    template <class Adapter>
    auto MakeSomeMagic(const Adapter& adapter) const {
        using Iterator = decltype(adapter.begin());
        return Range<TakeIterator<Iterator>>(
            TakeIterator<Iterator>(adapter.begin(), adapter.end(), limit_, 0),
            TakeIterator<Iterator>(adapter.end(), adapter.end(), limit_, limit_)
        );
    }
private:
    size_t limit_;
};

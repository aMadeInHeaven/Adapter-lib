#pragma once

#include <string>

#include "Iterators/SplitIterator.h"
#include "Range.h"

class Split {
public:
    Split(const std::string& delimeter) : delimeter_(delimeter) {}

    template <class Adapter>
    auto MakeSomeMagic(const Adapter& adapter) const {
        using Iterator = decltype(adapter.begin());
        return Range<SplitIterator<Iterator>>(
            SplitIterator<Iterator>(adapter.begin(), adapter.end(), delimeter_),
            SplitIterator<Iterator>(adapter.end(), adapter.end(), delimeter_)
        );
    }
private:
    std::string delimeter_;
};

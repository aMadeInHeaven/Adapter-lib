#pragma once

#include "Iterators/FilterIterator.h"
#include "Range.h"

template <class Function>
class Filter {
public:
    Filter(const Function& func) : func_(func) {}

    template <class Adapter>
    auto MakeSomeMagic(const Adapter& adapter) const {
        using Iterator = decltype(adapter.begin());
        return Range<FilterIterator<Iterator, Function>>(
            FilterIterator<Iterator, Function>(adapter.begin(), adapter.end(), func_),
            FilterIterator<Iterator, Function>(adapter.end(), adapter.end(), func_)
        );
    }
private:
    Function func_;
};
#pragma once

#include <memory>

#include "Range.h"
#include "Iterators/SplitExpectedIterator.h"

class SplitExpected {
public:
    template <class Adapter>
    auto MakeSomeMagic(const Adapter& adapter) const {
        using Iterator = typename Adapter::iterator;
        using expected_value_type = typename std::iterator_traits<Iterator>::value_type;

        using GoodType = typename expected_value_type::value_type;
        using ErrorType = typename expected_value_type::error_type;

        using SpltExpData = SplitExpectedData<Iterator, GoodType, ErrorType>;

        auto data = std::make_shared<SpltExpData>(adapter.begin(), adapter.end());
        auto data_end = std::make_shared<SpltExpData>(adapter.end(), adapter.end());

        auto error_range = Range<SplitExpectedIteratorBad<SpltExpData>>(
            SplitExpectedIteratorBad<SpltExpData>(data),
            SplitExpectedIteratorBad<SpltExpData>(data_end)
        );

        auto vals_range = Range<SplitExpectedIteratorGood<SpltExpData>>(
            SplitExpectedIteratorGood<SpltExpData>(data),
            SplitExpectedIteratorGood<SpltExpData>(data_end)
        );

        return std::make_pair(std::move(error_range), std::move(vals_range));
    }
};
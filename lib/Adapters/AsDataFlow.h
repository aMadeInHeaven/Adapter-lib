#pragma once

#include "Utils.h"

template <class Range>
class AsDataFlow {
public:
    using value_type = typename Range::value_type;
    using iterator = Condition<IsConst<Range>::value, typename Range::const_iterator, typename Range::iterator>::value;

    using const_iterator = typename Range::const_iterator;
    using difference_type = typename Range::difference_type;
    using size_type = typename Range::size_type;

    AsDataFlow(Range& range) : begin_(range.begin()), end_(range.end()) {}

    iterator begin() const { return begin_; }
    iterator end() const { return end_; }
private:
    iterator begin_;
    iterator end_;
};

template <class Range>
AsDataFlow(Range&) -> AsDataFlow<Range>;

template <class Range>
AsDataFlow(const Range&) -> AsDataFlow<const Range>;

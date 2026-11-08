#pragma once

#include <cstdint>

template <class Iterator>
class Range {
public:
    using differense_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;
    using iterator = Iterator;
    using const_iterator = Iterator;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;

    Range(const Iterator& begin, const Iterator& end) : begin_(begin), end_(end) {}

    Iterator begin() const { return begin_; }
    Iterator end() const { return end_; }
private:
    Iterator begin_;
    Iterator end_;
};
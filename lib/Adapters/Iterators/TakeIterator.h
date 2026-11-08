#pragma once

template<class Iterator>
class TakeIterator {
public:
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using reference = decltype(*std::declval<Iterator>());
    using const_reference = const reference;
    using difference_type = std::ptrdiff_t;
    using size_type = size_t;
    using iterator_category = std::input_iterator_tag;

    TakeIterator(const Iterator& it, const Iterator& end, size_t limit, size_t counter) :
        it_(it), end_(end), limit_(limit), counter_(counter) {}

    reference operator*() const {
        return *it_;
    }

    TakeIterator& operator++() {
        if (it_ != end_) {
            ++it_;
            ++counter_;
        }

        return *this;
    }

    TakeIterator operator++(int) {
        TakeIterator cpy = *this;
        ++(*this);

        return cpy;
    }

    bool operator==(const TakeIterator& other) const {
        if (counter_ >= limit_ || counter_ >= other.limit_) {
            return true;
        }

        return it_ == other.it_;
    }

    bool operator!=(const TakeIterator& other) const {
        return !(*this == other);
    }
private:
    Iterator it_;
    Iterator end_;

    size_t limit_;
    size_t counter_;
};
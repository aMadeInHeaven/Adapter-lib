#pragma once

template<class Iterator>
class EnumerateIterator {
public:
    using iterator_value_type = std::iterator_traits<Iterator>::value_type;
    using iterator_reference_type = decltype(*std::declval<Iterator>());

    using value_type = std::pair<size_t, iterator_value_type>;
    using reference = std::pair<size_t, iterator_reference_type>;
    using const_reference = const reference;
    using difference_type = std::ptrdiff_t;
    using size_type = size_t;
    using iterator_category = std::input_iterator_tag;

    EnumerateIterator(const Iterator& it, size_t counter) :
        it_(it), counter_(counter) {}

    reference operator*() const {
        return reference{counter_, *it_};
    }

    EnumerateIterator& operator++() {
        ++it_;
        ++counter_;

        return *this;
    }

    EnumerateIterator operator++(int) {
        EnumerateIterator cpy = *this;
        ++(*this);

        return cpy;
    }

    bool operator==(const EnumerateIterator& other) const {
        return it_ == other.it_;
    }

    bool operator!=(const EnumerateIterator& other) const {
        return !(*this == other);
    }
private:
    Iterator it_;

    size_t counter_;
};
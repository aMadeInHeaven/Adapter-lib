#pragma once

#include <iterator>
#include <cstddef>
#include <type_traits>


template <class Iterator, class Function>
class TransformIterator {
public:
    using reference = decltype(std::declval<Function>()(*std::declval<Iterator>()));
    using value_type = std::decay_t<reference>;
    using const_reference = const reference;
    using difference_type = std::ptrdiff_t;
    using size_type = size_t;
    using iterator_category = std::input_iterator_tag;

    TransformIterator(const Iterator& it, const Function& func) : it_(it), func_(func) {}
    TransformIterator(const TransformIterator& other) = default;
    TransformIterator& operator=(const TransformIterator& other) = default;
    ~TransformIterator() = default;

    reference operator*() const {
        return func_(*it_);
    }

    TransformIterator& operator++() {
        ++it_;
        return *this;
    }

    TransformIterator operator++(int) {
        TransformIterator cpy = *this;
        ++(*this);
        return cpy;
    }

    bool operator==(const TransformIterator& other) const {
        return it_ == other.it_;
    }

    bool operator!=(const TransformIterator& other) const {
        return !(*this == other);
    }
private:
    Iterator it_;
    Function func_;
};

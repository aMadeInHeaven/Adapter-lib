#pragma once

template <class Iterator, class Function>
class FilterIterator {
public:
    using value_type = std::iterator_traits<Iterator>::value_type;
    using reference = decltype(*std::declval<Iterator>());
    using const_reference = const reference;
    using difference_type = std::ptrdiff_t;
    using size_type = size_t;
    using iterator_category = std::input_iterator_tag;

    FilterIterator(const Iterator& it, const Iterator& end, const Function& func) : it_(it), end_(end), func_(func) {
        FindNextValid();
    }

    bool operator==(const FilterIterator& other) const {
        return it_ == other.it_;
    }

    bool operator!=(const FilterIterator& other) const {
        return !(*this == other);
    }

    reference operator*() const {
        return const_cast<value_type&>(*it_val_);
    }

    FilterIterator& operator++() {
        ++it_;
        FindNextValid();

        return *this;
    }

    FilterIterator operator++(int) {
        FilterIterator cpy = *this;
        ++(*this);

        return cpy;
    }

private:
    Iterator it_;
    std::optional<value_type> it_val_;
    Iterator end_;
    Function func_;

    void FindNextValid() {
        while (it_ != end_) {
            value_type current = *it_; 
            if (func_(current)) {
                it_val_ = std::move(current);
                return; 
            }
            ++it_;
        }
        
        it_val_ = std::nullopt;
    }
};

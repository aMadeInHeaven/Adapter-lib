#pragma once

#include <string>

template <class Iterator>
class SplitIterator {
public:
    using value_type = std::string;
    using reference = value_type;
    using const_reference = const reference;
    using difference_type = std::ptrdiff_t;
    using size_type = size_t;
    using iterator_category = std::input_iterator_tag;

    SplitIterator(const Iterator& it, const Iterator& end, const std::string& delimeter) : 
                                            it_(it), end_(end), delimeter_(delimeter) {
        if (it_ != end_) {
            FindValidPart();
        }
    }

    std::string operator*() const {
        return current_valid_;
    }

    SplitIterator& operator++() {
        FindValidPart();

        return *this;
    }

    SplitIterator operator++(int) {
        SplitIterator cpy = *this;
        ++(*this);

        return cpy;
    }

    bool operator==(const SplitIterator& other) const {
        return it_ == other.it_;
    }

    bool operator!=(const SplitIterator& other) const {
        return !(*this == other);
    }

private:
    Iterator it_;
    Iterator end_;
    std::string delimeter_;
    std::string current_valid_;

    void FindValidPart() {
        if (it_ != end_ && (*it_).eof()) {
            ++it_;
        }

        char s;
        current_valid_.clear();
        bool read = false;
        while (it_ != end_) {
            read = false;
            auto& stream = *it_;
            while (stream.get(s)) {
                read = true;
                if (delimeter_.find(s) == std::string::npos) {
                    current_valid_ += s;
                } else {
                    return;
                }
            }
            if (read || !current_valid_.empty()) {
                return;
            }

            ++it_;
        }
    }
};

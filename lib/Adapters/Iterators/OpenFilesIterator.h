#pragma once

#include <fstream>

template <class Iterator>
class OpenFileIterator {
public:
    using value_type = std::ifstream;
    using reference = std::ifstream&;
    using difference_type = std::ptrdiff_t;
    using size_type = size_t;
    using iterator_category = std::input_iterator_tag;

    OpenFileIterator(const Iterator& it, const Iterator& end) : it_(it), end_(end) {
        OpenFile();
    }

    reference operator*() const {
        return *filik_;
    }

    OpenFileIterator& operator++() {
        ++it_;
        OpenFile();
        return *this;
    }

    OpenFileIterator operator++(int) {
        OpenFileIterator cpy = *this;
        ++(*this);

        return cpy;
    }

    bool operator==(const OpenFileIterator& other) const {
        return it_ == other.it_;
    }

    bool operator!=(const OpenFileIterator& other) const {
        return !(*this == other);
    }
private:
    Iterator it_;
    Iterator end_;
    std::shared_ptr<std::ifstream> filik_;

    void OpenFile() {
        while (it_ != end_) {
            filik_ = std::make_shared<std::ifstream>(*it_);
            if ((*filik_).is_open()) {
                return;
            }

            ++it_;
        }
    }
};
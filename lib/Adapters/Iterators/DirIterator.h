#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class DirIterator {
public:
    using value_type = fs::path;
    using difference_type = std::ptrdiff_t;
    using pointer = const fs::path*;
    using reference = const fs::path&;
    using iterator_category = std::input_iterator_tag;

    DirIterator() = default;
    DirIterator(const fs::path& path, bool is_recursive) : is_recursive_(is_recursive), it_(path) {
        FindNextFile();
    }

    reference operator*() const {
        return *it_;
    }

    DirIterator& operator++() {
        ++it_;
        FindNextFile();

        return *this;
    }

    DirIterator operator++(int) {
        DirIterator cpy = *this;
        ++(*this);

        return cpy;
    }

    bool operator==(const DirIterator& other) const {
        return it_ == other.it_;
    }

    bool operator!=(const DirIterator& other) const {
        return !(*this == other);
    }

private:
    bool is_recursive_;
    fs::recursive_directory_iterator it_;
    fs::recursive_directory_iterator end_;

    void FindNextFile() {
        while (it_ != end_ && it_->is_directory()) {
            if (!is_recursive_) {
                it_.disable_recursion_pending();
            }

            ++it_;
        }
    }
};
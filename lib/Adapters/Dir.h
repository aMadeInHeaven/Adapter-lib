#pragma once

#include <filesystem>

#include "Iterators/DirIterator.h"

namespace fs = std::filesystem;

class Dir {
public:
    using iterator = DirIterator;
    using const_iterator = DirIterator;
    
    Dir(const fs::path& path, bool recursive) : recursive_(recursive), path_(path), begin_(path, recursive) {}

    const_iterator begin() const {
        return begin_;
    }

    const_iterator end() const {
        return end_;
    }
private:
    bool recursive_;
    fs::path path_;
    const_iterator begin_;
    const_iterator end_;
};

#pragma once

#include "Iterators/OpenFilesIterator.h"
#include "Range.h"

class OpenFiles {
public:
    template <class Adapter>
    auto MakeSomeMagic(const Adapter& adapter) const {
        using Iterator = decltype(adapter.begin());
        return Range<OpenFileIterator<Iterator>>(
            OpenFileIterator<Iterator>(adapter.begin(), adapter.end()),
            OpenFileIterator<Iterator>(adapter.end(), adapter.end())
        );
    }
};
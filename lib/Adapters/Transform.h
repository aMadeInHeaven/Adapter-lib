#pragma once

#include "processing.h"
#include "Range.h"
#include "Iterators/TransformIterator.h"

template <class TransformingFunction>
class Transform {
public:
    Transform(TransformingFunction f) : function_(f) {}

    template <class Adapter>
    auto MakeSomeMagic(const Adapter& adapter) const {
        using Iterator = decltype(adapter.begin());
        return Range<TransformIterator<Iterator, TransformingFunction>>(
            TransformIterator<Iterator, TransformingFunction>(adapter.begin(), function_),
            TransformIterator<Iterator, TransformingFunction>(adapter.end(), function_)
        );
    }

private:
    TransformingFunction function_;
};

#pragma once

#include <vector>

class AsVector {
public:
    template <class Adapter>
    auto MakeSomeMagic(const Adapter& adapter) const {
        std::vector<typename std::iterator_traits<typename Adapter::iterator>::value_type> result;

        for (auto&& element : adapter) {
            result.push_back(std::move(element));
        }

        return result;
    }
};
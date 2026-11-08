#pragma once

#include "JoinResult.h"
#include "KV.h"

static auto get_key_from_kv = [](const auto& kv) { return kv.key; };

template <class Adapter, class Func1, class Func2>
class Join {
public:
    Join(const Adapter& adapter, Func1 func1, Func2 func2) : adapter_(adapter), func1_(func1), func2_(func2) {}

    template <class Adapter1>
    auto MakeSomeMagic(const Adapter1& adapter1) const {
        using BaseIterator = decltype(adapter1.begin());
        using Base = std::iterator_traits<BaseIterator>::value_type;

        using JoinedIterator = decltype(adapter_.begin());
        using Joined = std::iterator_traits<JoinedIterator>::value_type;

        std::vector<Joined> joined_to_vector;
        for (auto&& elem : adapter_) {
            joined_to_vector.push_back(std::forward<decltype(elem)>(elem));
        }

        std::vector<JoinResult<Base, Joined>> result;

        bool is_found = false;
        for (const auto& base: adapter1) {
            for (const auto& joined : joined_to_vector) {
                if (func1_(base) == func2_(joined)) {
                    is_found = true;
                    result.push_back({base, joined});
                }
            }

            if (!is_found) {
                result.push_back({base, std::nullopt});
            }

            is_found = false;
        }
        
        return result;
    }
private:
    Adapter adapter_;
    Func1 func1_;
    Func2 func2_;
};

template <class Adapter>
class Join<Adapter, void, void> {
public:
    Join(const Adapter& adapter) : adapter_(adapter), func1_(get_key_from_kv), func2_(get_key_from_kv) {}

    template <class Adapter1>
    auto MakeSomeMagic(const Adapter1& adapter1) const {
        using Base = decltype((*adapter1.begin()).value);
        using Joined = decltype((*adapter_.begin()).value);

        std::vector<typename Adapter::value_type> joined_to_vector;
        for (auto&& elem : adapter_) {
            joined_to_vector.push_back(std::forward<decltype(elem)>(elem));
        }
        
        std::vector<JoinResult<Base, Joined>> result;

        bool is_found = false;
        for (const auto& base: adapter1) {
            for (const auto& joined : joined_to_vector) {
                if (func1_(base) == func2_(joined)) {
                    is_found = true;
                    result.push_back({base.value, joined.value});
                }
            }
            if (!is_found) {
                result.push_back({base.value, std::nullopt});
            }
            
            is_found = false;
        }
        
        return result;
    }
private:
    using FuncType = decltype(get_key_from_kv);
    Adapter adapter_;
    FuncType func1_;
    FuncType func2_;
};

template <typename Adapter>
Join(Adapter) -> Join<Adapter, void, void>;


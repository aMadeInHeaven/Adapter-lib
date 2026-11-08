#pragma once

#include <unordered_map>

template <class BasicValueType, class AgregatorFunc, class KeyFunc>
class AggregateByKey {
public:
    AggregateByKey(const BasicValueType& value, const AgregatorFunc& agregate_func, const KeyFunc& key_func) :
        value_(value), agregate_func_(agregate_func), key_func_(key_func) {}

    template <class Adapter>
    auto MakeSomeMagic(const Adapter& adapter) const {
        using KeyIterator = decltype(adapter.begin());
        using AdapterElemType = decltype(*adapter.begin());
        using KeyType = decltype(std::declval<KeyFunc>()(std::declval<AdapterElemType>()));
        
        
        std::unordered_map<KeyType, BasicValueType> agregated;
        std::vector<KeyType> keys_order;

        for (auto it = adapter.begin(); it != adapter.end(); ++it) {
            auto&& it_elem = *it;
            KeyType key = key_func_(it_elem);

            if (!agregated.contains(key)) {
                keys_order.push_back(key);
                agregated[key] = value_;
            }

            agregate_func_(std::forward<AdapterElemType>(it_elem), agregated[key]);
        }

        std::vector<std::pair<KeyType, BasicValueType>> result;
        result.reserve(keys_order.size());

        for (auto& key : keys_order) {
            result.push_back(std::make_pair(std::move(key), std::move(agregated[key])));
        }

        return result;
    }
private:
    BasicValueType value_;
    AgregatorFunc agregate_func_;
    KeyFunc key_func_;
};
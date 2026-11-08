#pragma once

#include <queue>
#include <memory>

#include "../../Utils.h"

template <class Iter, class GoodType, class ErrorType>
struct SplitExpectedData {
    using exp_type = GoodType;
    using unexp_type = ErrorType;
    using iterator = Iter;

    std::queue<GoodType> expected_q;
    std::queue<ErrorType> unexpected_q;

    Iter it;
    Iter end;

    SplitExpectedData(const Iter& b, const Iter& e) : it(b), end(e) {}

    void FindNext() {
        if (it != end) {
            auto&& value = *it;

            if (value) {
                expected_q.push(std::move(*value));
            } else {
                unexpected_q.push(std::move(value.error()));
            }

            ++it;
        }
    }
};

template <class SplitData>
class SplitExpectedIteratorGood {
public:
    using value_type = typename SplitData::exp_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using difference_type = std::ptrdiff_t;
    using size_type = size_t;
    using iterator_category = std::input_iterator_tag;

    SplitExpectedIteratorGood(std::shared_ptr<SplitData> data) : data_(data) {
        Next();
    }

    value_type operator*() {
        return data_->expected_q.front();
    }

    SplitExpectedIteratorGood& operator++() {
        if (!data_->expected_q.empty()) {
            data_->expected_q.pop();
        }

        Next();

        return *this;
    }

    SplitExpectedIteratorGood operator++(int) {
        SplitExpectedIteratorGood cpy = *this;
        ++(*this);

        return cpy;
    }
    
    bool operator==(const SplitExpectedIteratorGood& other) const {
        bool this_end = (data_->expected_q.empty() && data_->it == data_->end);
        bool other_end = (other.data_->expected_q.empty() && other.data_->it == other.data_->end);

        if (this_end && other_end) {
            return true;
        }
        if (this_end != other_end) {
            return false;
        }

        return (data_ == other.data_) && (data_->it == other.data_->it);
    }

    bool operator!=(const SplitExpectedIteratorGood& other) const {
        return !(*this == other);
    }
private:
    std::shared_ptr<SplitData> data_;

    void Next() {
        while (data_->expected_q.empty() && data_->it != data_->end) {
            data_->FindNext();
        }
    }
};

template <class SplitData>
class SplitExpectedIteratorBad {
public:
    using value_type = typename SplitData::unexp_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using difference_type = std::ptrdiff_t;
    using size_type = size_t;
    using iterator_category = std::input_iterator_tag;

    SplitExpectedIteratorBad(std::shared_ptr<SplitData> data) : data_(data) {
        Next();
    }

    value_type operator*() {
        return data_->unexpected_q.front();
    }

    SplitExpectedIteratorBad& operator++() {
        if (!data_->unexpected_q.empty()) {
            data_->unexpected_q.pop();
        }

        Next();

        return *this;
    }

    SplitExpectedIteratorBad operator++(int) {
        SplitExpectedIteratorBad cpy = *this;
        ++(*this);

        return cpy;
    }
    
    bool operator==(const SplitExpectedIteratorBad& other) const {
        bool this_end = (data_->unexpected_q.empty() && data_->it == data_->end);
        bool other_end = (other.data_->unexpected_q.empty() && other.data_->it == other.data_->end);

        if (this_end && other_end) {
            return true;
        }
        if (this_end != other_end) {
            return false;
        }

        return (data_ == other.data_) && (data_->it == other.data_->it);
    }

    bool operator!=(const SplitExpectedIteratorBad& other) const {
        return !(*this == other);
    }
private:
    std::shared_ptr<SplitData> data_;

    void Next() {
        while (data_->unexpected_q.empty() && data_->it != data_->end) {
            data_->FindNext();
        }
    }
};

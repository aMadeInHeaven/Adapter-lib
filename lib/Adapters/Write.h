#pragma once

template <class Stream, class Delimeter>
class Write {
public:
    Write(Stream& stream, const Delimeter& delimeter) : stream_(stream), delimeter_(delimeter) {}

    template <class Range>
    auto MakeSomeMagic(const Range& range) const {
        for (auto i : range) {
            stream_ << i << delimeter_;
        }

        return range;
    }
private:
    Stream& stream_;
    Delimeter delimeter_;
};

#pragma once

template <typename Base, typename Joined>
struct JoinResult {
	Base base;
	std::optional<Joined> joined;

    bool operator==(const JoinResult& other) const {
        return (base == other.base) && (joined == other.joined);
    }

    bool operator!=(const JoinResult& other) const {
        return !(*this == other);
    }
};

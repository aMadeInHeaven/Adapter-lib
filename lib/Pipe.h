#pragma once

#include <utility>

template <class Left, class Right>
auto operator|(Left&& l, Right&& r) -> decltype(r.MakeSomeMagic(std::forward<Left>(l))){
	return r.MakeSomeMagic(std::forward<Left>(l));
}

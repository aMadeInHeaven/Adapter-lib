#pragma once

template <bool Cond, class T, class F>
struct Condition {
    using value = T;
};

template <class T, class F>
struct Condition<false, T, F> {
    using value = F;
};

template <class T>
struct IsConst {
    static constexpr bool value = false; 
};

template <class T>
struct IsConst<const T> {
    static constexpr bool value = true;
};

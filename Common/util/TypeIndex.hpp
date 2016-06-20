#pragma once

template<int N, typename T, typename Head, typename ... Tail> struct TypeIndexImpl {
    static constexpr int index = TypeIndexImpl<N + 1, T, Tail...>::index;
};

template<int N, typename T, typename ... Tail> struct TypeIndexImpl<N, T, T, Tail...> {
    static constexpr int index = N;
};

template<int N, typename T> struct TypeIndexImpl<N, T, T> {
    static constexpr int index = N;
};

template<int N, typename T, typename Head> struct TypeIndexImpl<N, T, Head> {
    static constexpr int index = -1;
};

template<typename T, typename ... Types> struct TypeIndex {
    static constexpr int index = TypeIndexImpl<0, T, Types...>::index;
};

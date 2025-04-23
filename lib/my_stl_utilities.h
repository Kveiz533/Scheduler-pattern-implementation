#pragma once


namespace my_stl {
    template<typename T>
    struct IsVoid {
        static constexpr bool value = false;
    };

    template<>
    struct IsVoid<void> {
        static constexpr bool value = true;
    };

    template<>
    struct IsVoid<const void> {
        static constexpr bool value = true;
    };

    template<>
    struct IsVoid<volatile void> {
        static constexpr bool value = true;
    };

    template<>
    struct IsVoid<const volatile void> {
        static constexpr bool value = true;
    };

    template<typename T>
    constexpr bool IsVoidV = IsVoid<T>::value;


    template<typename T>
    struct RemoveReference {
        using type = T;
    };

    template<typename T>
    struct RemoveReference<T&> {
        using type = T;
    };

    template<typename T>
    struct RemoveReference<T&&> {
        using type = T;
    };

    template<typename T>
    using RemoveReferenceT = typename RemoveReference<T>::type;


    template <typename T>
    struct RemoveCv {
        using type = T;
    };

    template <typename T>
    struct RemoveCv<const T> {
        using type = T;
    };

    template <typename T>
    struct RemoveCv<volatile T> {
        using type = T;
    };

    template <typename T>
    struct RemoveCv<const volatile T> {
        using type = T;
    };


    template <typename T>
    using RemoveCvT = typename RemoveCv<T>::type;

};
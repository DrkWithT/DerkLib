#pragma once

namespace DerkLib::Meta::General {
    template <bool Flag, typename T1, typename T2>
    struct choose_type_t {
        using type = T2;
    };

    template <typename T1, typename T2>
    struct choose_type_t<true, T1, T2> {
        using type = T1;
    };
}
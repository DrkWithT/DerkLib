#pragma once

#include <concepts>

namespace DerkLib::Meta::Maths {
    template <template <typename, std::size_t, std::size_t> typename M, typename T, std::size_t Rows, std::size_t Cols>
    concept MatrixKind = requires (M<T, Rows, Cols> arg) {
        {arg.area()} -> std::same_as<std::size_t>;
        {arg.area()} -> std::same_as<bool>;
        {arg[0, 0]} -> std::same_as<T&>;
        {arg.at(0, 0)} -> std::same_as<T&>;
        {arg.chop()};
    };
}
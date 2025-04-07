#pragma once

#include <concepts>
#include "meta/general.hpp"

namespace DerkLib::Meta::Maths {
    template <template <typename, std::size_t, std::size_t> typename M, typename T, std::size_t Rows, std::size_t Cols>
    concept MatrixKind = requires (M<T, Rows, Cols> arg) {
        {arg.area()} -> std::same_as<std::size_t>;
        {arg.isSquare()} -> std::same_as<bool>;
        {arg[0, 0]} -> std::same_as<T&>;
        {arg.at(0, 0)} -> std::same_as<T&>;
        {arg.template chop<0, 0, Rows, Cols>()};
    };

    template <std::size_t ARows, std::size_t ACols, std::size_t BRows, std::size_t BCols>
    struct are_multipliable_matrix_dims_t {
        static constexpr auto flag = ACols == BRows;
    };

    template <std::size_t ARows, std::size_t ACols, std::size_t BRows, std::size_t BCols>
    constexpr bool AreMatDimsCompatible = are_multipliable_matrix_dims_t<ARows, ACols, BRows, BCols>::flag;

    /**
     * @brief Deduces a type of a matrix multiplication result or `void` on incompatible matrices.
     * 
     * @tparam `A` for 1st operand of matrix product
     * @tparam `AItem` stored type in matrix A
     * @tparam `ARows` 
     * @tparam `ACols` 
     * @tparam `B` for 2nd operand of matrix product
     * @tparam `BItem` stored type in matrix B
     * @tparam `BRows` 
     * @tparam `BCols` 
     */
    template <template <typename, std::size_t, std::size_t> typename A, typename AItem, std::size_t ARows, std::size_t ACols, template <typename, std::size_t, std::size_t> typename B, typename BItem, std::size_t BRows, std::size_t BCols>
    struct deduce_matrix_product_t {
        using type = General::choose_type_t<AreMatDimsCompatible<ARows, ACols, BRows, BCols>, A<AItem, ARows, BCols>, void>::type;
    };

    /**
     * @brief Alias for `deduce_matrix_product_t<...>::type`.
     * 
     * @tparam `A`
     * @tparam `AItem`
     * @tparam `ARows`
     * @tparam `ACols`
     * @tparam `B`
     * @tparam `BItem`
     * @tparam `BRows`
     * @tparam `BCols`
     */
    template <template <typename, std::size_t, std::size_t> typename A, typename AItem, std::size_t ARows, std::size_t ACols, template <typename, std::size_t, std::size_t> typename B, typename BItem, std::size_t BRows, std::size_t BCols>
    using ProductOfMatrices = deduce_matrix_product_t<A, AItem, ARows, ACols, B, BItem, BRows, BCols>::type;
}
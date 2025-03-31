#pragma once

#include "mathematics/matrices.hpp"
#include "meta/maths.hpp"

namespace DerkLib::Algorithms::Matrix {
    template <template <typename, std::size_t, std::size_t> typename M, typename T, std::size_t Rows, std::size_t Cols> requires (Meta::Maths::MatrixKind<M, T, Rows, Cols>)
    void applyRowScale(M<T, Rows, Cols>& matrix, int target_row, T factor) {
        const auto target_row_szt = static_cast<std::size_t>(target_row);

        if (target_row_szt >= Rows) {
            return;
        }

        for (auto row_idx = 0UL; row_idx < Cols; row_idx++) {
            matrix[target_row, row_idx] *= factor;
        }
    }

    template <template <typename, std::size_t, std::size_t> typename M, typename T, std::size_t Rows, std::size_t Cols> requires (Meta::Maths::MatrixKind<M, T, Rows, Cols>)
    void applyRowAdd(M<T, Rows, Cols>& matrix, int row_from, int row_to) {
        const auto from_row_szt = static_cast<std::size_t>(row_from);
        const auto to_row_szt = static_cast<std::size_t>(row_to);

        if (from_row_szt >= Rows or to_row_szt >= Rows) {
            return;
        }

        for (auto row_idx = 0UL; row_idx < Cols; row_idx++) {
            matrix[row_to, row_idx] += matrix[row_from, row_idx];
        }
    }

    template <template <typename, std::size_t, std::size_t> typename M, typename T, std::size_t Rows, std::size_t Cols> requires (Meta::Maths::MatrixKind<M, T, Rows, Cols>)
    void applyRowAddScaled(M<T, Rows, Cols>& matrix, int row_from, int row_to, T factor) {
        const auto from_row_szt = static_cast<std::size_t>(row_from);
        const auto to_row_szt = static_cast<std::size_t>(row_to);

        if (from_row_szt >= Rows or to_row_szt >= Rows) {
            return;
        }

        for (auto row_idx = 0UL; row_idx < Cols; row_idx++) {
            matrix[row_to, row_idx] += factor * matrix[row_from, row_idx];
        }
    }

    /// TODO: Add Jordan-Gauss elimination for RREF's and inversing matrices?
}
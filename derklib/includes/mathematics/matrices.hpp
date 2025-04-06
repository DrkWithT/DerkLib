#pragma once

#include <algorithm>
#include <array>
#include <stdexcept>
#include <type_traits>
#include "meta/maths.hpp"

namespace DerkLib::Mathematics::Matrices {
    enum class MatrixDefaultingOpt {
        zeroed,
        identity
    };

    /**
     * @brief Represents an NxM matrix where `N` is rows & `M` is cols.
     * 
     * @tparam T 
     * @tparam Rows 
     * @tparam Cols 
     */
    template <typename T, std::size_t Rows, std::size_t Cols>
    class Matrix {
    private:
        std::array<std::array<T, Cols>, Rows> m_data;

    public:
        explicit Matrix(MatrixDefaultingOpt opt = MatrixDefaultingOpt::zeroed, T filler = T {}) noexcept(std::is_nothrow_assignable_v<T, T>)
        : m_data {} {
            if (opt == MatrixDefaultingOpt::identity and Rows == Cols) {
                for (auto row_col_i = 0UL; row_col_i < Rows; row_col_i++) {
                    m_data[row_col_i][row_col_i] = filler;
                }

                return;
            }

            if (opt == MatrixDefaultingOpt::zeroed) {
                for (auto& row : m_data) {
                    std::fill(row.begin(), row.end(), T {});
                }
            }
        }

        template <typename T2 = T>
        explicit Matrix(T2&& arg) noexcept(std::is_nothrow_assignable_v<T2, T>)
        : m_data {} {
            for (auto fill_row = 0UL; fill_row < Rows; fill_row++) {
                for (auto fill_col = 0UL; fill_col < Cols; fill_col++) {
                    m_data[fill_row][fill_col] = arg;
                }
            }
        }

        [[nodiscard]] constexpr std::size_t area() const noexcept {
            return Rows * Cols;
        }

        [[nodiscard]] constexpr bool isSquare() const noexcept {
            return Rows == Cols;
        }

        T& operator[](int row, int col) noexcept {
            return m_data[row][col];
        }

        const T& operator[](int row, int col) const noexcept {
            return m_data[row][col];
        }

        T& at(int row, int col) {
            if (row < 0 or row >= Rows or col < 0 or col >= Cols) {
                throw std::logic_error {"Invalid row-col index of Matrix."};
            }

            return m_data[row][col];
        }

        template <std::size_t StartRow, std::size_t StartCol, std::size_t RowsC, std::size_t ColsC> requires (
            (StartRow >= 0 and StartRow < Rows)
            and (StartCol >= 0 and StartCol < Cols)
            and (RowsC >= 0 and RowsC <= Rows)
            and (ColsC >= 0 and ColsC <= Cols)
        )
        [[nodiscard]] auto chop() noexcept (std::is_nothrow_assignable_v<T, T>) -> Matrix<T, RowsC, ColsC> {
            Matrix<T, RowsC, ColsC> temp {};

            const auto start_row_n = static_cast<int>(StartRow);
            const auto start_col_n = static_cast<int>(StartCol);
            const auto end_row_n = static_cast<int>(RowsC);
            const auto end_col_n = static_cast<int>(ColsC);

            for (int row_i = start_row_n; row_i < end_row_n; row_i++) {
                for (int col_i = start_col_n; col_i < end_col_n; col_i++) {
                    temp[row_i, col_i] = m_data[row_i][col_i];
                }
            }

            return temp;
        }

        Matrix& operator+=(const Matrix<T, Rows, Cols>& rhs) noexcept (std::is_nothrow_assignable_v<T, T>) {
            const auto rows_n = static_cast<int>(Rows);
            const auto cols_n = static_cast<int>(Cols);

            for (auto row_idx = 0; row_idx < rows_n; row_idx++) {
                for (auto col_idx = 0; col_idx < cols_n; col_idx++) {
                    m_data[row_idx][col_idx] += rhs[row_idx, col_idx];
                }
            }

            return *this;
        }

        Matrix& operator-=(const Matrix<T, Rows, Cols>& rhs) noexcept (std::is_nothrow_assignable_v<T, T>) {
            const auto rows_n = static_cast<int>(Rows);
            const auto cols_n = static_cast<int>(Cols);

            for (auto row_idx = 0; row_idx < rows_n; row_idx++) {
                for (auto col_idx = 0; col_idx < cols_n; col_idx++) {
                    m_data[row_idx][col_idx] -= rhs[row_idx, col_idx];
                }
            }

            return *this;
        }

        Matrix& operator*=(T scalar) noexcept (std::is_nothrow_assignable_v<T, T>) {
            const auto rows_n = static_cast<int>(Rows);
            const auto cols_n = static_cast<int>(Cols);

            for (auto row_idx = 0; row_idx < rows_n; row_idx++) {
                for (auto col_idx = 0; col_idx < cols_n; col_idx++) {
                    m_data[row_idx][col_idx] *= scalar;
                }
            }

            return *this;
        }

        template <template <typename, std::size_t, std::size_t> typename OtherMat, typename OtherItem, std::size_t OtherRows, std::size_t OtherCols> requires (Meta::Maths::MatrixKind<OtherMat, OtherItem, OtherRows, OtherCols>)
        [[nodiscard]] auto operator*(const OtherMat<OtherItem, OtherRows, OtherCols> other) noexcept (std::is_nothrow_assignable_v<T, OtherItem>) -> Meta::Maths::ProductOfMatrices<Matrix, T, Rows, Cols, OtherMat, OtherItem, OtherRows, OtherCols> {
            if constexpr (not Meta::Maths::AreMatDimsCompatible<Rows, Cols, OtherRows, OtherCols>) {
                throw std::logic_error {"Invalid dimensions passed for Matrix<T, Rows, Cols>::operator*(Matrix<T2, Rows2, Cols2>)."};
            }

            using AnsMatrix = Meta::Maths::ProductOfMatrices<Matrix, T, Rows, Cols, Matrix, T, OtherRows, OtherCols>; // Represents mat<A_rows, B_cols> or void!

            const auto self_row_n = static_cast<int>(Rows);
            const auto other_row_n = static_cast<int>(OtherRows);
            const auto other_col_n = static_cast<int>(OtherCols);

            AnsMatrix ans;

            for (auto self_row_i = 0; self_row_i < self_row_n; self_row_i++) {
                for (auto other_col_i = 0; other_col_i < other_col_n; other_col_i++) {
                    for (auto other_row_i = 0; other_row_i < other_row_n; other_row_i++) {
                        ans[self_row_i, other_col_i] += m_data[self_row_i][other_row_i] * other[other_row_i, other_col_i];
                    }
                }
            }

            return ans;
        }

        template <template <typename, std::size_t, std::size_t> typename OtherMat, typename OtherT, std::size_t OtherRows, std::size_t OtherCols>
        constexpr bool operator==(const OtherMat<OtherT, OtherRows, OtherCols> other) const noexcept {
            if (&other == this) {
                return true;
            }

            if (Rows != OtherRows or Cols != OtherCols) {
                return false;
            }

            const auto rows_n = static_cast<int>(OtherRows);
            const auto cols_n = static_cast<int>(OtherCols);

            for (auto cmp_row_idx = 0; cmp_row_idx < rows_n; cmp_row_idx++) {
                for (auto cmp_col_idx = 0; cmp_col_idx < cols_n; cmp_col_idx++) {
                    if (m_data[cmp_row_idx][cmp_col_idx] != other[cmp_row_idx, cmp_col_idx]) {
                        return false;
                    }
                }
            }

            return true;
        }
    };

    /**
     * @brief 2x2 matrix alias
     * 
     * @tparam T 
     */
    template <typename T>
    using Mat2x2 = Matrix<T, 2, 2>;

    /**
     * @brief 3x3 matrix alias
     * 
     * @tparam T 
     */
    template <typename T>
    using Mat3x3 = Matrix<T, 3, 3>;

    /**
     * @brief N-vector alias
     * 
     * @tparam T 
     * @tparam N 
     */
    template <typename T, std::size_t N>
    using VecN = Matrix<T, N, 1>;
}
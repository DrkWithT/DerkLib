#pragma once

#include <algorithm>
#include <array>
#include <stdexcept>
#include <type_traits>

namespace DerkLib::Mathematics::Matrices {
    enum class MatrixDefaultingOpt {
        zeroed,
        identity
    };

    template <typename T, std::size_t Rows, std::size_t Cols>
    class Matrix {
    private:
        std::array<std::array<T, Cols>, Rows> m_data;

    public:
        explicit Matrix(MatrixDefaultingOpt opt = MatrixDefaultingOpt::zeroed) noexcept(std::is_nothrow_assignable_v<T, T>)
        : m_data {} {
            if (opt == MatrixDefaultingOpt::identity) {
                if constexpr (isSquare()) {
                    for (auto row_col_i = 0UL; row_col_i < Rows; row_col_i++) {
                        m_data[row_col_i][row_col_i] = T {};
                    }
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
            for (auto row_col_i = 0UL; row_col_i < Rows; row_col_i++) {
                m_data[row_col_i][row_col_i] = arg;
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

        T& at(int row, int col) {
            if (row < 0 or row >= Rows or col < 0 or col >= Cols) {
                throw std::logic_error {"Invalid row-col index of Matrix."};
            }

            return m_data[row][col];
        }

        template <std::size_t RowsC, std::size_t ColsC> requires ((RowsC >= 0 and RowsC < Rows) or (ColsC >= 0 and ColsC < Cols))
        [[nodiscard]] auto chop() noexcept (std::is_nothrow_assignable_v<T, T>) -> Matrix<T, RowsC, ColsC> {
            Matrix<T, RowsC, ColsC> temp {};

            for (int row_i = 0; row_i < RowsC; row_i++) {
                for (int col_i = 0; col_i < ColsC; col_i++) {
                    temp[row_i, col_i] = m_data[row_i][col_i];
                }
            }

            return temp;
        }

        Matrix& operator+(const Matrix<T, Rows, Cols>& rhs) noexcept (std::is_nothrow_assignable_v<T, T>) {
            for (auto row_idx = 0; row_idx < Rows; row_idx++) {
                for (auto col_idx = 0; col_idx < Cols; col_idx++) {
                    m_data[row_idx][col_idx] += rhs[row_idx, col_idx];
                }
            }

            return *this;
        }

        Matrix& operator-(const Matrix<T, Rows, Cols>& rhs) noexcept (std::is_nothrow_assignable_v<T, T>) {
            for (auto row_idx = 0; row_idx < Rows; row_idx++) {
                for (auto col_idx = 0; col_idx < Cols; col_idx++) {
                    m_data[row_idx][col_idx] -= rhs[row_idx, col_idx];
                }
            }

            return *this;
        }

        Matrix& operator*(T scalar) noexcept (std::is_nothrow_assignable_v<T, T>) {
            for (auto row_idx = 0; row_idx < Rows; row_idx++) {
                for (auto col_idx = 0; col_idx < Cols; col_idx++) {
                    m_data[row_idx][col_idx] *= scalar;
                }
            }

            return *this;
        }

        /// TODO: add matrix-to-matrix multplication!
    };

    template <typename T>
    using Mat2x2 = Matrix<T, 2, 2>;

    template <typename T>
    using Mat3x3 = Matrix<T, 3, 3>;

    template <typename T, std::size_t N>
    using VecN = Matrix<T, N, 1>;
}
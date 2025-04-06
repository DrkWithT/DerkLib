#include "mathematics/matrices.hpp"
#include <iostream>
#include <print>

int main() {
    using namespace DerkLib::Mathematics;

    /**
     * @brief Represents a 2x2 matrix of:
     [1 2],
     [2 1]
     */
    Matrices::Matrix<int, 2, 2> square_1 {1};
    square_1[0, 1] = 2;
    square_1[1, 0] = 2;

    if (const auto s1_is_square = square_1.isSquare(); not s1_is_square) {
        std::print(std::cerr, "Unexpected value {} of square_1.isSquare()!\n", s1_is_square);
        return 1;
    }

    if (const auto s1_area = square_1.area(); s1_area != 4UL) {
        std::print(std::cerr, "Unexpected value {} of square_1.area()!\n", s1_area);
        return 1;
    }

    /**
     * @brief Represents another 2x2 matrix of:
     * [3, 3],
     * [3, 3]
     */
    Matrices::Matrix<int, 2, 2> square_2 {3};

    if (square_1 == square_2) {
        std::print(std::cerr, "Unexpected match of square_1 & square_2!\n");
        return 1;
    }

    auto vec_of_square_2 = square_2.chop<0, 0, 2, 1>();

    Matrices::Matrix<int, 2, 1> vec_to_check {3};

    if (vec_of_square_2 != vec_to_check) {
        std::print(std::cerr, "Unexpected mismatch of vec_of_square_2 & vec_to_check!\n");
        return 1;
    }
}
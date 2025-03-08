#include <print>
#include "meta/typelist.hpp"

template <typename... Args>
using MyTypeList = DerkLib::Meta::TypeList::TypeList<Args...>;

int main() {
    MyTypeList<> none;
    MyTypeList<bool> sample_1;
    MyTypeList<char, int, long> sample_2;

    if (none) {
        std::print("typelist none was unexpectedly filled.\n");
        return 1;
    }

    if (not sample_1) {
        std::print("typelist sample_1 was unexpectedly empty.\n");
        return 1;
    }

    if (auto none_len = none.length(); none_len != 0) {
        std::print("typelist none had wrong length: {}\n", none_len);
        return 1;
    }

    if (auto sample_2_len = sample_2.length(); sample_2_len != 0) {
        std::print("typelist sample_2 had wrong length: {}\n", sample_2_len);
        return 1;
    }

    if (auto void_type_pos = sample_2.findType<void>(); void_type_pos != -1) {
        std::print("typelist sample_2 had wrong search index: {}\n", void_type_pos);
        return 1;
    }

    if (auto int_type_pos = sample_2.findType<int>(); int_type_pos != 1) {
        std::print("typelist sample_2 had wrong search index: {}\n", int_type_pos);
        return 1;
    }

    MyTypeList<char, int, long> sample_3;

    if (sample_2 != sample_3) {
        std::print("typelists sample_2 != sample3 unexpectedly.\n");
        return 1;
    }

    std::print("OK\n");
}
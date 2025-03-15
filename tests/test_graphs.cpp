#include <type_traits>
#include <iostream>
#include <print>
#include <vector>
#include "containers/graph.hpp"
#include "algorithms/traversals.hpp"

template <typename Item>
constexpr auto is_trivially_comparable_v = std::is_same_v<Item, bool> or std::is_enum_v<Item> or std::is_arithmetic_v<Item>;

template <typename Item>
[[nodiscard]] bool checkTraversalResults(const std::vector<Item>& result, const std::vector<Item>& expected) {
    if (result.size() != expected.size()) {
        return false;
    }

    if constexpr (is_trivially_comparable_v<Item>) {
        return std::equal(result.cbegin(), result.cend(), expected.cbegin());
    }

    return false;
}

template <typename Item, typename Predicate>
[[nodiscard]] bool checkTraversalResults(const std::vector<Item>& result, const std::vector<Item>& expected, Predicate&& equalizer) noexcept {
    if (result.size() != expected.size()) {
        return false;
    }

    if constexpr (is_trivially_comparable_v<Item>) {
        return std::equal(result.cbegin(), result.cend(), expected.cbegin());
    }

    return std::equal(result.cbegin(), result.cend(), expected.cbegin(), equalizer);
}

int main() {
    using namespace DerkLib;
    using EdgeWeightPolicy = Containers::Graph::PathPolicy;
    using EdgeDirection = Containers::Graph::DirectFlag;

    Containers::Graph::Graph<EdgeWeightPolicy::unweighted, int> valued_tree;

    valued_tree.add(10);
    valued_tree.add(3);
    valued_tree.add(15);
    valued_tree.add(-8);

    if (not valued_tree.connect(10, 3, EdgeDirection::one_way)) {
        std::print(std::cerr, "Unexpected failure of connecting 10 -> 3\n");
        return 1;
    }

    if (not valued_tree.connect(10, 15, EdgeDirection::one_way)) {
        std::print(std::cerr, "Unexpected failure of connecting 10 -> 15\n");
        return 1;
    }

    if (not valued_tree.connect(3, -8, EdgeDirection::one_way)) {
        std::print(std::cerr, "Unexpected failure of connecting 3 -> -8\n");
        return 1;
    }

    auto bfs_results = Algorithms::Graph::traverseBFS(valued_tree, [](int arg) noexcept {
        return arg > 0;
    });

    if (not checkTraversalResults(bfs_results, {true, true, true, false})) {
        std::print(std::cerr, "Unexpected mismatch in result: \n");
        return 1;
    }
}

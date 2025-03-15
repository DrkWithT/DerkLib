#pragma once

#include <type_traits>
// #include <set> /// NOTE: add DFS later using a stack frontier.
#include <queue>
// #include <stack>
#include <vector>
#include "containers/graph.hpp"

namespace DerkLib::Algorithms::Graph {
    using PathPolicy = DerkLib::Containers::Graph::PathPolicy;

    template <typename T, typename NodeItem>
    concept CallableForItemKind = requires(T arg, NodeItem item) {
        {arg(item)};
    };

    template <typename Fn, template <PathPolicy, typename> typename Graph, PathPolicy P, typename Item> requires (CallableForItemKind<Fn, Item>)
    [[nodiscard]] auto traverseBFS(const Graph<P, Item>& arg, Fn&& fn) noexcept -> std::vector<std::remove_reference_t<decltype(fn(arg.first()))>> {
        using ResultType = std::remove_reference_t<decltype(fn(arg.first()))>;

        if (arg.size() == 0) {
            return {};
        }

        std::queue<Item> frontier;
        std::vector<ResultType> results;

        frontier.push(arg.first());

        while (not frontier.empty()) {
            auto temp = frontier.front();
            frontier.pop();

            results.emplace_back(fn(temp));

            for (const auto& adj_ptr : arg.neighborsOf(temp)) {
                frontier.push(*adj_ptr);
            }
        }

        return results;
    }
}
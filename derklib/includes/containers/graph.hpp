#pragma once

#include <utility>
#include <algorithm>
#include <forward_list>
#include <optional>
#include <vector>

namespace DerkLib::Containers::Graph {
    enum class DirectFlag {
        one_way,
        two_way
    };

    enum class PathPolicy {
        unweighted,
        weighted
    };

    template <PathPolicy P, typename T>
    class Graph {};

    template <typename T>
    class Graph <PathPolicy::unweighted, T> {
    public:
        using AdjList = std::forward_list<int>;
        using PosOpt = std::optional<int>;
        using ItemPtr = const T*;

    private:
        std::vector<T> m_items;
        std::vector<AdjList> m_adj;

        [[nodiscard]] PosOpt indexOfItem(const T& item) const noexcept {
            auto index = 0;

            for (const auto& temp : m_items) {
                if (item == temp) {
                    return index;
                }

                ++index;
            }

            if (static_cast<std::size_t>(index) < m_items.size()) {
                return index;
            }

            return {};
        }

    public:
        Graph()
        : m_items {}, m_adj {} {}

        [[nodiscard]] std::size_t size() const noexcept {
            return m_items.size();
        }

        T& first() & noexcept {
            return m_items[0];
        }

        const T& first() const& noexcept {
            return m_items[0];
        }

        template <typename T2 = T>
        [[maybe_unused]] bool add(T2&& arg) {
            if (std::find(m_items.cbegin(), m_items.cend(), arg) != m_items.cend()) {
                return false;
            }

            m_items.emplace_back(arg);
            m_adj.emplace_back(AdjList {});

            return true;
        }

        [[nodiscard]] bool connect(const T& from, const T& to, DirectFlag flag) {
            PosOpt from_index = indexOfItem(from);
            PosOpt to_index = indexOfItem(to);

            if (not from_index or not to_index) {
                return false;
            }

            m_adj[from_index.value()].emplace_front(to_index.value());

            if (flag == DirectFlag::two_way) {
                m_adj[to_index.value()].emplace_front(from_index.value());
            }

            return true;
        }

        bool remove(const T& target) {
            PosOpt target_index = indexOfItem(target);

            if (not target_index) {
                return false;
            }

            m_items.erase(m_items.begin() + target_index.value());
            m_adj.erase(m_adj.begin() + target_index.value());

            return true;
        }

        [[nodiscard]] std::vector<ItemPtr> neighborsOf(const T& arg) const& {
            auto target_index = indexOfItem(arg);

            if (not target_index) {
                return {};
            }

            const auto& neighbor_list = m_adj.at(target_index.value());
            std::vector<ItemPtr> result;

            for (const auto neighbor_position : neighbor_list) {
                result.emplace_back(m_items.data() + neighbor_position);
            }

            return result;
        }
    };

    template <typename T>
    class Graph <PathPolicy::weighted, T> {
    public:
        using WeightedEdge = std::pair<int, int>; // <cost, destination-index>
        using AdjList = std::forward_list<WeightedEdge>;
        using PosOpt = std::optional<int>;
        using ItemPtr = const T*;

    private:
        std::vector<T> m_items;
        std::vector<AdjList> m_adj;

        [[nodiscard]] PosOpt indexOfItem(const T& item) const noexcept {
            auto index = 0;

            for (const auto& temp : m_items) {
                if (item == temp) {
                    return index;
                }

                ++index;
            }

            if (static_cast<std::size_t>(index) < m_items.size()) {
                return index;
            }

            return {};
        }

    public:
        Graph()
        : m_items {}, m_adj {} {}

        [[nodiscard]] std::size_t size() const noexcept {
            return m_items.size();
        }

        T& first() & noexcept {
            return m_items[0];
        }

        const T& first() const& noexcept {
            return m_items[0];
        }

        template <typename T2 = T>
        [[maybe_unused]] bool add(T2&& arg) {
            if (std::find(m_items.cbegin(), m_items.cend(), arg) != m_items.cend()) {
                return false;
            }

            m_items.emplace_back(arg);
            m_adj.emplace_back(AdjList {});

            return true;
        }

        [[nodiscard]] bool connect(const T& from, const T& to, int cost, DirectFlag flag) {
            PosOpt from_index = indexOfItem(from);
            PosOpt to_index = indexOfItem(to);

            if (not from_index or not to_index) {
                return false;
            }

            m_adj[from_index.value()].emplace_front(cost, to_index.value());

            if (flag == DirectFlag::two_way) {
                m_adj[to_index.value()].emplace_front(cost, from_index.value());
            }

            return true;
        }

        bool remove(const T& target) {
            PosOpt target_index = indexOfItem(target);

            if (not target_index) {
                return false;
            }

            m_items.erase(m_items.begin() + target_index.value());
            m_adj.erase(m_adj.begin() + target_index.value());

            return true;
        }

        [[nodiscard]] std::vector<ItemPtr> neighborsOf(const T& arg) const& {
            auto target_index = indexOfItem(arg);

            if (not target_index) {
                return {};
            }

            const auto& neighbor_list = m_adj.at(target_index.value());
            std::vector<ItemPtr> result;

            for (const auto neighbor_position : neighbor_list) {
                result.emplace_back(m_items.data() + neighbor_position);
            }

            return result;
        }
    };
}
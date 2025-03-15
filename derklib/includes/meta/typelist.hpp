#pragma once

#include <type_traits>

namespace DerkLib::Meta::TypeList {
    enum class InsertPolicy {
        the_front = 0,
        the_back
    };

    namespace Impl {
        template <typename... Args>
        struct type_count_t {
            static constexpr auto value = sizeof...(Args);
        };

        template <int Index, typename Next, typename... Args>
        struct index_type_t {
            using type = typename index_type_t<Index - 1, Args...>::type;
        };

        template <typename Next, typename... Args>
        struct index_type_t <0, Next, Args...> {
            using type = Next;
        };

        template <int Index, typename... Args>
        using type_at_t = index_type_t<Index, Args...>::type;

        template <InsertPolicy P, typename Arg, template <typename...> typename Sequence, typename... Args>
        struct add_type_t {};

        template <typename Arg, template <typename...> typename Sequence, typename... Args>
        struct add_type_t<InsertPolicy::the_front, Arg, Sequence, Args...> {
            using type = Sequence<Arg, Args...>;
        };

        template <typename Arg, template <typename...> typename Sequence, typename... Args>
        struct add_type_t<InsertPolicy::the_back, Arg, Sequence, Args...> {
            using type = Sequence<Args..., Arg>;
        };

        template <int N, typename Target>
        [[nodiscard]] constexpr int implGetTypeIndexOf() noexcept {
            return -1;
        }

        template <int N, typename Target, typename Next, typename... Rest>
        [[nodiscard]] constexpr int implGetTypeIndexOf() noexcept {
            if constexpr (std::is_same_v<Target, Next>) {
                return N;
            }
    
            return implGetTypeIndexOf<N + 1, Target, Rest...>();
        }
    }

    template <typename... Args>
    struct TypeList {
        TypeList() = default;

        [[nodiscard]] constexpr int length() const noexcept {
            return Impl::type_count_t<Args...>::value;
        }

        template <typename Target>
        [[nodiscard]] constexpr int findType() const noexcept {
            if constexpr (sizeof...(Args) == 0) {
                return Impl::implGetTypeIndexOf<0, Target>();
            }
    
            return Impl::implGetTypeIndexOf<0, Target, Args...>();
        }

        template <InsertPolicy P, template <typename> typename TypeHolder, typename InsideT>
        [[nodiscard]] auto append([[maybe_unused]] TypeHolder<InsideT> arg) -> decltype(Impl::index_type_t<static_cast<int>(P), TypeList<InsideT, Args...>, TypeList<Args..., InsideT>>::type) {
            return {};
        }

        template <typename... Args2>
        [[nodiscard]] TypeList<Args..., Args2...> append([[maybe_unused]] TypeList<Args2...> other) const noexcept {
            return {};
        }

        /* Operators */

        [[nodiscard]] explicit constexpr operator bool() const noexcept {
            return Impl::type_count_t<Args...>::value > 0;
        }

        template <typename... OtherArgs>
        [[nodiscard]] constexpr bool operator==(TypeList<OtherArgs...> other) noexcept {
            using self_type = TypeList<Args...>;
            using other_type = decltype(other);

            return std::is_same_v<self_type, other_type>;
        }
    };
}
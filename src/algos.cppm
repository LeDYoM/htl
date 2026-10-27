module;

#include <algorithm>
#include <initializer_list>

export module htypes:algos;

import :btypes;

namespace htps
{
export template <typename Container, typename F>
constexpr void for_each(Container const& container, F&& function) noexcept
{
    std::for_each(std::begin(container), std::end(container),
                  forward<F>(function));
}

export template <typename Container, typename F>
constexpr void for_each(Container& container, F&& function) noexcept
{
    std::for_each(std::begin(container), std::end(container),
                  forward<F>(function));
}

export template <typename T>
constexpr T min(std::initializer_list<T> values)
{
    return std::min(htps::move(values));
}

export template <typename T>
constexpr T max(std::initializer_list<T> values)
{
    return std::max(htps::move(values));
}

export template <typename ForwardIt1, typename ForwardIt2, typename UnaryPred>
constexpr ForwardIt2 copy(ForwardIt1 first, ForwardIt1 last, ForwardIt2 d_first)
{
    return std::copy(first, last, d_first);
}

template <class ForwardIt1, class Size, class ForwardIt2>
constexpr ForwardIt2 copy_n(ForwardIt1 first, Size count, ForwardIt2 d_first)
{
    return std::copy_n(first, count, d_first);
}

template <class InputIt1, class InputIt2>
constexpr bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
    return std::equal(first1, last1, first2, last2);
}

template <class InputIt1, class InputIt2>
constexpr bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
    return std::equal(first1, last1, first2);
}
}  // namespace htps

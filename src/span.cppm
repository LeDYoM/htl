module;

#include <cassert>
#include <type_traits>

export module htypes:span;

import :btypes;
import :vector_base;

namespace htps
{
export template <typename T>
class span
{
public:
    static constexpr bool const is_const_v{std::is_const_v<T>};
    using iterator        = rptr<T>;
    using const_iterator  = rptr<T const>;
    using reference       = T&;
    using const_reference = T const&;
    using value_type      = T;

    constexpr span() noexcept : m_begin{nullptr}, m_end{nullptr} {}
    constexpr span(T* begin, T* end) noexcept : m_begin{begin}, m_end{end} {}
    constexpr span(T& begin) noexcept : m_begin{&begin}, m_end{m_begin + 1U} {}

    template <size_type Size>
    constexpr span(T (&sp_data)[Size]) noexcept :
        span{&sp_data[0U], sp_data + Size}
    {}

    template <typename T2         = T,
              typename Allocator  = AllocatorType<T>,
              typename GrowPolicy = GrowPolicyUnary>
    span(vector_base<T2, Allocator, GrowPolicy>& v) noexcept :
        span{v.begin(), v.end()}
    {}

    template <typename T2         = T,
              typename Allocator  = AllocatorType<T>,
              typename GrowPolicy = GrowPolicyUnary>
    span(vector_base<T2, Allocator, GrowPolicy> const& v) noexcept :
        span{v.begin(), v.end()}
    {}

    constexpr size_type size() const noexcept
    {
        return static_cast<size_type>(m_end - m_begin);
    }

    constexpr bool empty() const noexcept { return m_begin == m_end; }

    constexpr iterator begin() noexcept { return m_begin; }
    constexpr const_iterator begin() const noexcept { return m_begin; }
    constexpr const_iterator cbegin() const noexcept { return m_begin; }
    constexpr iterator end() noexcept { return m_end; }
    constexpr const_iterator end() const noexcept { return m_end; }
    constexpr const_iterator cend() const noexcept { return m_end; }

    constexpr reference operator[](const size_type index)
    {
        assert(index < size());
        return *(m_begin + index);
    }

    constexpr const_reference operator[](const size_type index) const
    {
        assert(index < size());
        return *(m_begin + index);
    }

private:
    rptr<T> const m_begin;
    rptr<T> const m_end;
};

template <typename T,
          typename Allocator  = AllocatorType<T>,
          typename GrowPolicy = GrowPolicyUnary>
span(vector_base<T, Allocator, GrowPolicy>& v) -> span<T>;

template <typename T,
          typename Allocator  = AllocatorType<T>,
          typename GrowPolicy = GrowPolicyUnary>
span(vector_base<T, Allocator, GrowPolicy> const& v) -> span<T const>;

template <typename T, size_type Size>
span(T (&)[Size]) -> span<T>;

}  // namespace htps

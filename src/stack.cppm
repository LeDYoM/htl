module;

#include <initializer_list>

export module htypes:stack;

import :btypes;
import :vector;

namespace htps
{
/**
 * @brief Data type to store a stack of elements.
 * @details This class is a container to store a stack of elements with common
 * semantics as push and pops, basically.
 *
 * @tparam T Type of the contained element.
 * @tparam Allocator Allocator to be used by the vector
 * @tparam GrowPolicy Policy class to dinamically increment the capacity
 */
export template <typename T,
          typename Allocator  = AllocatorType<T>,
          typename GrowPolicy = GrowPolicyUnary>
class stack final
{
public:
    using iterator        = T*;
    using const_iterator  = const T*;
    using reference       = T&;
    using const_reference = const T&;

    /**
     * @brief Default constructor. Makes an empty stack.
     */
    constexpr stack() noexcept = default;

    /**
     * @brief Construct that reserves space for a given number of elements.
     * @param The number of elements to reserve internally.
     */
    explicit constexpr stack(const size_type size) noexcept : m_data(size) {}

    constexpr stack(std::initializer_list<T> ilist) noexcept :
        m_data{htps::move(ilist)}
    {}

    constexpr stack(const stack& other)                = default;
    constexpr stack(stack&& other) noexcept            = default;
    constexpr stack& operator=(const stack& other)     = default;
    constexpr stack& operator=(stack&& other) noexcept = default;

    constexpr size_type capacity() const noexcept { return m_data.capacity(); }
    constexpr size_type size() const noexcept { return m_data.size(); }
    constexpr bool empty() const noexcept { return m_data.empty(); }
    constexpr T& back() noexcept { return m_data.back(); }
    constexpr const T& cback() const noexcept { return m_data.cback(); }
    constexpr T& top() noexcept { return m_data.back(); }
    constexpr const T& ctop() const noexcept { return m_data.cback(); }

    constexpr void swap(stack& other) noexcept { m_data.swap(other.m_data); }

    template <typename... Args>
    constexpr void emplace_back(Args&&... args)
    {
        m_data.emplace_back(htps::forward<Args>(args)...);
    }

    constexpr void shrink_to_fit() { m_data.shrink_to_fit(); }

    constexpr void push_back(const T& value) { m_data.push_back(value); }

    constexpr void push_back(T&& value) { m_data.push_back(htps::move(value)); }

    constexpr void pop_back() noexcept { m_data.pop_back(); }

    constexpr void clear() noexcept { m_data.clear(); }

    template <class A>
    friend constexpr bool operator==(const stack<A>& lhs,
                                     const stack<A>& rhs) noexcept;

    template <class A>
    friend constexpr bool operator!=(const stack<A>& lhs,
                                     const stack<A>& rhs) noexcept;

private:
    vector<T, Allocator, GrowPolicy> m_data;
};

template <class A>
constexpr void swap(stack<A>& lhs, stack<A>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <class A>
constexpr bool operator==(const stack<A>& lhs, const stack<A>& rhs) noexcept
{
    return lhs.m_data == rhs.m_data;
}

template <class A>
constexpr bool operator!=(const stack<A>& lhs, const stack<A>& rhs) noexcept
{
    return lhs.m_data != rhs.m_data;
}
}  // namespace htps

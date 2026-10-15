export module htypes:counter;

import :btypes;

namespace htps::shptr_detail
{
class counter
{
public:
    constexpr counter() noexcept = default;

    constexpr counter(const counter&) noexcept            = delete;
    constexpr counter& operator=(const counter&) noexcept = delete;

    constexpr counter(counter&&)            = default;
    constexpr counter& operator=(counter&&) = default;

    constexpr void reset() noexcept { m_counter = 0U; }

    [[nodiscard]] constexpr fast_u32 get() const noexcept { return m_counter; }
    [[nodiscard]] constexpr fast_u32 get_weak() const noexcept
    {
        return m_weak_counter;
    }

    constexpr void increment() noexcept { ++m_counter; }
    constexpr void increment_weak() noexcept { ++m_weak_counter; }

    constexpr void decrement() noexcept { --m_counter; }
    constexpr void decrement_weak() noexcept { --m_weak_counter; }

    [[nodiscard]] constexpr bool decrement_destroy()
    {
        decrement();
        if (m_counter == 0U)
        {
            if (m_weak_counter == 0U)
            {
                destroy();
                return true;
            }
        }
        return false;
    }

    constexpr virtual ~counter() noexcept     = default;
    constexpr virtual void destroy() noexcept = 0;

private:
    fast_u32 m_counter{1U};
    fast_u32 m_weak_counter{0U};
};
}  // namespace htps::shptr_detail

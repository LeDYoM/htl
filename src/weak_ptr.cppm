export module htypes:weak_ptr;

import :btypes;
import :shared_ptr;

namespace htps
{
export template <typename T>
class wptr final
{
public:
    constexpr wptr() noexcept = default;
    constexpr wptr(std::nullptr_t) noexcept : wptr{} {}

    constexpr wptr(wptr&& wp) noexcept :
        wptr{htps::move(wp.m_ptr), htps::move(wp.m_counter)}
    {
        wp.m_ptr     = nullptr;
        wp.m_counter = nullptr;
    }

    constexpr wptr(wptr const& wp) noexcept :
        wptr{htps::move(wp.m_ptr), htps::move(wp.m_counter)}
    {
        if (m_counter != nullptr)
        {
            m_counter->increment_weak();
        }
    }

    constexpr wptr(sptr<T> const& sp) noexcept : wptr{sp.m_ptr, sp.m_counter}
    {
        if (m_counter != nullptr)
        {
            m_counter->increment_weak();
        }
    }

    template <typename T2>
    constexpr wptr(sptr<T2> const& sp) noexcept : wptr{sp.m_ptr, sp.m_counter}
    {
        if (m_counter != nullptr)
        {
            m_counter->increment_weak();
        }
    }

    constexpr wptr& operator=(wptr&& wp) noexcept
    {
        auto tmp_ptr{htps::move(wp.m_ptr)};
        auto tmp_counter{htps::move(wp.m_counter)};
        wp.m_ptr     = htps::move(m_ptr);
        wp.m_counter = htps::move(m_counter);
        m_ptr        = htps::move(tmp_ptr);
        m_counter    = htps::move(tmp_counter);
        return *this;
    }

    constexpr wptr& operator=(wptr const& wp) noexcept
    {
        *this = wptr{wp.m_ptr, wp.m_counter};
        if (m_counter != nullptr)
        {
            m_counter->increment_weak();
        }
        return *this;
    }

    constexpr wptr& operator=(sptr<T> const& sp) noexcept
    {
        *this = wptr{sp.m_ptr, sp.m_counter};
        if (m_counter != nullptr)
        {
            m_counter->increment_weak();
        }
        return *this;
    }

    [[nodiscard]] constexpr u32 use_count() const noexcept
    {
        return m_counter == nullptr ? 0U : static_cast<u32>(m_counter->get());
    }

    void reset() noexcept { *this = wptr{nullptr, nullptr}; }

    [[nodiscard]] constexpr bool expired() const noexcept
    {
        return m_ptr == nullptr || m_counter == nullptr ||
            m_counter->get() == 0U;
    }

    [[nodiscard]] constexpr operator bool() const noexcept
    {
        return !expired();
    }

    [[nodiscard]] constexpr sptr<T> lock() const noexcept
    {
        if (expired())
        {
            return sptr<T>{};
        }
        else
        {
            sptr<T> tmp{m_ptr, m_counter};
            if (tmp.m_counter != nullptr)
            {
                tmp.m_counter->increment();
            }
            return tmp;
        }
    }

    ~wptr() noexcept
    {
        if (m_counter != nullptr)
        {
            m_counter->decrement_weak();

            if (m_counter->get() == 0U && m_counter->get_weak() == 0U)
            {
                m_counter->destroy();
                AllocatorType<shptr_detail::counter>::delete_one(m_counter);
            }
            m_counter = nullptr;
        }
        m_ptr = nullptr;
    }

private:
    constexpr wptr(T* sp, shptr_detail::counter* cnt) noexcept :
        m_ptr{htps::move(sp)}, m_counter{htps::move(cnt)}
    {}

    T* m_ptr{nullptr};
    shptr_detail::counter* m_counter{nullptr};
};

export template <typename T>
wptr(wptr<T>) -> wptr<T>;

export template <typename T>
[[nodiscard]] bool operator==(wptr<T> const& lhs, std::nullptr_t) noexcept
{
    return lhs.expired();
}

export template <typename T>
[[nodiscard]] bool operator==(wptr<T> const& lhs, wptr<T> const& rhs) noexcept
{
    if (lhs.expired() == true && rhs.expired() == true)
    {
        return true;
    }
    else if (lhs.expired() || rhs.expired())
    {
        return false;
    }
    else
    {
        return lhs.lock() == rhs.lock();
    }
}

}  // namespace htps

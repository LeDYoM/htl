module;

#include <concepts>

export module htypes:unique_ptr;

import :btypes;
import :allocator;

namespace htps
{
export template <typename T>
class uptr
{
public:
    using type = T;

    constexpr uptr() noexcept = default;
    constexpr uptr(nullptr_t) noexcept : m_pointer{nullptr} {}
    constexpr uptr(rptr<T> pointer) noexcept : m_pointer{pointer} {}

    uptr(uptr const&)            = delete;
    uptr& operator=(uptr const&) = delete;

    template <typename U>
    uptr(uptr<U> const&) = delete;

    template <typename U>
    uptr& operator=(uptr<U> const&) = delete;

    constexpr uptr(uptr&& other) noexcept :
        m_pointer{move(other.m_pointer)}
    {
        other.m_pointer = nullptr;
    }

    template <typename U>
    constexpr uptr(uptr<U>&& other) noexcept :
        m_pointer{move(other.m_pointer)}
    {
        other.m_pointer = nullptr;
    }

    constexpr uptr& operator=(uptr&& other) noexcept
    {
        auto tmp{m_pointer};
        m_pointer       = other.m_pointer;
        other.m_pointer = move(tmp);
        return *this;
    }

    constexpr uptr& operator=(nullptr_t) noexcept
    {
        reset();
        return *this;
    }

    [[nodiscard]] constexpr T* release() noexcept
    {
        auto tmp{move(m_pointer)};
        m_pointer = nullptr;
        return tmp;
    }

    constexpr void reset(T* newValue) noexcept
    {
        reset();
        m_pointer = move(newValue);
    }

    constexpr void reset() noexcept
    {
        if (m_pointer != nullptr)
        {
            AllocatorType<T>::delete_one(m_pointer);
            m_pointer = nullptr;
        }
    }

    [[nodiscard]] constexpr T* operator->() const noexcept { return m_pointer; }
    [[nodiscard]] constexpr T* get() const noexcept { return m_pointer; }
    [[nodiscard]] constexpr T& operator*() const noexcept { return *m_pointer; }
    [[nodiscard]] constexpr operator bool() const noexcept
    {
        return m_pointer != nullptr;
    }

    [[nodiscard]] constexpr bool operator==(uptr const& rhs) const noexcept =
        default;

    ~uptr() noexcept { reset(); }

private:
    template <typename U>
    friend class uptr;

    rptr<T> m_pointer{nullptr};
};

export template <typename T>
[[nodiscard]] constexpr bool operator==(uptr<T> const& lhs, nullptr_t) noexcept
{
    return lhs.get() == nullptr;
}

export template <typename T>
[[nodiscard]] constexpr bool operator!=(uptr<T> const& lhs, nullptr_t) noexcept
{
    return lhs.get() != nullptr;
}

export template <typename T, typename... Args>
[[nodiscard]] uptr<T> muptr(Args&&... args)
{
    T* p{AllocatorType<T>::make_one(forward<Args>(args)...)};
    return uptr<T>{move(p)};
}

static_assert(!std::copy_constructible<uptr<s32>>);
static_assert(std::move_constructible<uptr<s32>>);

}  // namespace htps

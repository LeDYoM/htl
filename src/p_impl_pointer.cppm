export module htypes:pimplp;

import :btypes;

namespace htps
{
export template <typename T>
class PImplPointer final
{
public:
    explicit constexpr PImplPointer(rptr<T> pointer) noexcept :
        m_pointer{pointer}
    {}

    PImplPointer(PImplPointer const&) = delete;
    PImplPointer& operator=(PImplPointer const&) = delete;

    template <typename Y>
    PImplPointer(PImplPointer<Y> const&) = delete;

    template <typename Y>
    PImplPointer& operator=(PImplPointer<Y> const&) = delete;

    template <typename Y>
    PImplPointer(PImplPointer<Y>&&) = delete;

    template <typename Y>
    PImplPointer& operator=(PImplPointer<Y>&&) = delete;

    constexpr PImplPointer(PImplPointer&& other) noexcept :
        m_pointer{other.m_pointer}
    {
        other.m_pointer = nullptr;
    }

    constexpr PImplPointer& operator=(PImplPointer&& other) noexcept
    {
        auto tmp{m_pointer};
        m_pointer       = other.m_pointer;
        other.m_pointer = move(tmp);
        return *this;
    }

    constexpr T* operator->() noexcept { return m_pointer; }
    constexpr T const* operator->() const noexcept { return m_pointer; }

    constexpr T* get() noexcept { return m_pointer; }
    constexpr T const* get() const noexcept { return m_pointer; }

    constexpr T& operator*() noexcept { return *m_pointer; }
    constexpr T const& operator*() const noexcept { return *m_pointer; }

    constexpr bool operator==(PImplPointer const&) const noexcept = default;

    ~PImplPointer() noexcept
    {
        if (m_pointer != nullptr)
        {
            delete m_pointer;
            m_pointer = nullptr;
        }
    }

private:
    rptr<T> m_pointer;
};

export template <typename T, typename... Args>
constexpr PImplPointer<T> make_pimplp(Args&&... args) noexcept
{
    T* p = new T(forward<Args>(args)...);
    return PImplPointer<T>{p};
}

export template <typename T, typename... Args>
constexpr PImplPointer<T> make_pimplp_forward(Args&&... args) noexcept
{
    T* p = new T{forward<Args>(args)...};
    return PImplPointer<T>{p};
}

}  // namespace htps

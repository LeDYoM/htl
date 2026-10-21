export module htypes:pimplp;

import :btypes;

namespace htps
{
export template <typename T>
class PImplPointer final
{
public:
    explicit constexpr PImplPointer(rptr<T> pointer) noexcept :
        pointer_{pointer}
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
        pointer_{other.pointer_}
    {
        other.pointer_ = nullptr;
    }

    constexpr PImplPointer& operator=(PImplPointer&& other) noexcept
    {
        auto tmp{pointer_};
        pointer_       = other.pointer_;
        other.pointer_ = htps::move(tmp);
        return *this;
    }

    constexpr T* operator->() noexcept { return pointer_; }
    constexpr T const* operator->() const noexcept { return pointer_; }

    constexpr T* get() noexcept { return pointer_; }
    constexpr T const* get() const noexcept { return pointer_; }

    constexpr T& operator*() noexcept { return *pointer_; }
    constexpr T const& operator*() const noexcept { return *pointer_; }

    constexpr bool operator==(std::nullptr_t) const noexcept
    {
        return pointer_ == nullptr;
    }

    ~PImplPointer() noexcept
    {
        if (pointer_ != nullptr)
        {
            delete pointer_;
            pointer_ = nullptr;
        }
    }

private:
    rptr<T> pointer_;
};

export template <typename T, typename... Args>
constexpr PImplPointer<T> make_pimplp(Args&&... args) noexcept
{
    T* p = new T(htps::forward<Args>(args)...);
    return PImplPointer<T>{p};
}

export template <typename T, typename... Args>
constexpr PImplPointer<T> make_pimplp_forward(Args&&... args) noexcept
{
    T* p = new T{htps::forward<Args>(args)...};
    return PImplPointer<T>{p};
}

}  // namespace htps

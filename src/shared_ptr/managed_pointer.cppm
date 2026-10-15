export module htypes:managed_pointer;

import :counter;
import :allocator;

namespace htps::shptr_detail
{
template <typename T>
struct ManagedPointer final : public counter
{
    using counter::counter;

    explicit constexpr ManagedPointer(T* originalPointer) noexcept :
        m_originalPointer{originalPointer}
    {}

    constexpr void destroy() noexcept override
    {
        if (m_originalPointer != nullptr)
        {
            AllocatorType<T>::delete_one(m_originalPointer);
            m_originalPointer = nullptr;
        }
    }

    T* m_originalPointer{nullptr};
};
}  // namespace htps::shptr_detail

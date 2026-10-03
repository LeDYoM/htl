export module htypes:allocator;

import :placement;
import :alloc_func;

#ifdef USER_MEM_MANAGER
export template <typename T>
using ReserveDestroyT = htps::AllocatorMemManagerRaw<T>;
#else
export template <typename T>
using ReserveDestroyT = htps::AllocatorMallocFree<T>;
#endif

namespace htps
{
export template <typename T, typename ReserveDestroy = ReserveDestroyT<T>>
class AllocatorType : public ReserveDestroy, public BasicConstructDestruct<T>
{
public:
    template <typename... Args>
    static T* make_one(Args&&... args) noexcept(
        noexcept(ReserveDestroy::allocate(1)) &&
        noexcept(
            BasicConstructDestruct<T>::construct(nullptr,
                                                 htps::forward<Args>(args)...)))
    {
        auto t{ReserveDestroy::allocate(1)};
        BasicConstructDestruct<T>::construct(t, htps::forward<Args>(args)...);
        return t;
    }

    static void delete_one(T* pointer) noexcept
    {
        AllocatorType<T>::destruct(pointer);
        AllocatorType<T>::deallocate_with_size(pointer, 1U);
    }
};

}  // namespace htps

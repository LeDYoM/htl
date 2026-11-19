export module htypes:allocator;

import :placement;
import :allocator_malloc_free;

namespace htps
{
export template <typename T,
                 typename ReserveDestroy    = AllocatorMallocFree<T>,
                 typename ConstrucortDestructor = BasicConstructDestruct<T>>
class AllocatorType : public ReserveDestroy, public ConstrucortDestructor
{
public:
    template <typename... Args>
    static T* make_one(Args&&... args) noexcept(
        noexcept(ReserveDestroy::allocate(1)) &&
        noexcept(ConstrucortDestructor::construct(nullptr,
                                              forward<Args>(args)...)))
    {
        auto t{ReserveDestroy::allocate(1)};
        ConstrucortDestructor::construct(t, forward<Args>(args)...);
        return t;
    }

    static void delete_one(T* pointer) noexcept(
        noexcept(noexcept(ConstrucortDestructor::destruct(pointer)) &&
                 noexcept(ReserveDestroy::deallocate(pointer, 1U))))
    {
        ConstrucortDestructor::destruct(pointer);
        ReserveDestroy::deallocate(pointer, 1U);
    }
};

template <typename T>
using DefaultAllocator = AllocatorType<T>;

}  // namespace htps

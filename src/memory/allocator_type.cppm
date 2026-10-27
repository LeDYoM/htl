export module htypes:allocator;

import :placement;
import :allocator_malloc_free;

namespace htps
{
export template <typename T,
                 typename ReserveDestroy    = AllocatorMallocFree<T>,
                 typename ConstructDestruct = BasicConstructDestruct<T>>
class AllocatorType : public ReserveDestroy, public ConstructDestruct
{
public:
    template <typename... Args>
    static T* make_one(Args&&... args) noexcept(
        noexcept(ReserveDestroy::allocate(1)) &&
        noexcept(ConstructDestruct::construct(nullptr,
                                              forward<Args>(args)...)))
    {
        auto t{ReserveDestroy::allocate(1)};
        ConstructDestruct::construct(t, forward<Args>(args)...);
        return t;
    }

    static void delete_one(T* pointer) noexcept(
        noexcept(noexcept(ConstructDestruct::destruct(pointer)) &&
                 noexcept(ReserveDestroy::deallocate(pointer, 1U))))
    {
        ConstructDestruct::destruct(pointer);
        ReserveDestroy::deallocate(pointer, 1U);
    }
};

template <typename T>
using DefaultAllocator = AllocatorType<T>;

}  // namespace htps

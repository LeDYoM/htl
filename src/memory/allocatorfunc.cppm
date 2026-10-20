module;

#include <cassert>

export module htypes:allocatorfunc;

import :btypes;

namespace htps
{

export template <typename T, auto Allocate, auto Deallocate>
class AllocatorFunc
{
public:
    using pointer = T*;

    static pointer allocate(size_type const size)
    {
        assert(size > 0U);
        return static_cast<pointer>(Allocate(sizeof(T) * size));
    }

    static void deallocate(pointer element, size_type const size)
    {
        assert(element != nullptr);
        Deallocate(element, sizeof(T) * size);
    }
};

}  // namespace htps

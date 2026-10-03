module;

#include <cstdlib>

export module htypes:alloc_func;
import :allocatorfunc;

namespace htps
{
export void free_with_size(void* block, std::size_t const)
{
    std::free(block);
}

export template <typename T>
using AllocatorMallocFree =
    AllocatorFunc<T, std::malloc, std::free, free_with_size>;

}  // namespace htps

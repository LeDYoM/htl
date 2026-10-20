module;

#include <cstdlib>

export module htypes:allocator_malloc_free;
import :allocatorfunc;

namespace htps
{
void free_with_size(void* block, std::size_t const)
{
    std::free(block);
}

export template <typename T>
using AllocatorMallocFree = AllocatorFunc<T, std::malloc, free_with_size>;

}  // namespace htps

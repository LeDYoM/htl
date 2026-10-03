module;

export module htypes:alloc_func;
import memmanager;

namespace htps
{
export template <typename T>
using AllocatorMemManagerRaw =
    htps::AllocatorFunc<T, mmalloc, mfree, mfree_with_size>;
}  // namespace htps

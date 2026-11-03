export module htypes:vector;

import :growpolicy;
import :allocator;
import :vector_base;
import :shared_ptr;
import :unique_ptr;
import :weak_ptr;

namespace htps
{
export template <typename T,
          typename Allocator  = AllocatorType<T>,
          typename GrowPolicy = GrowPolicyUnary>
using vector = vector_base<T, Allocator, GrowPolicy>;

export template <typename T,
          typename Allocator  = AllocatorType<sptr<T>>,
          typename GrowPolicy = GrowPolicyUnary>
using vector_shared_pointers = vector_base<sptr<T>, Allocator, GrowPolicy>;

export template <typename T,
          typename Allocator  = AllocatorType<uptr<T>>,
          typename GrowPolicy = GrowPolicyUnary>
using vector_unique_pointers = vector<uptr<T>>;

export template <typename T,
          typename Allocator  = AllocatorType<wptr<T>>,
          typename GrowPolicy = GrowPolicyUnary>
using vector_weak_pointers = vector<wptr<T>>;
}  // namespace htps

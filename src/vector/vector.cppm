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

}  // namespace htps

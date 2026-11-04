export module htypes:vector_stack;

import :placement;
import :vector_stack_base;

namespace htps
{
export template <typename T, size_type N>
using vector_stack = vector_stack_base<T, N, BasicConstructDestruct<T>>;
}  // namespace htps

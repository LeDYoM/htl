export module htypes:list_intrusive;

import :allocator;
import :basic_intrusive_list;

namespace htps
{
export template <typename T,
          typename Allocator  = AllocatorType<T>>
using list_intrusive = basic_list_intrusive<T, Allocator>;

}  // namespace htps

module;

#include <cassert>
#include <new>

module htypes:placement;
import :btypes;

namespace htps
{

/**
 * @brief Class dedicated to the construction and destruction
 * of objects. You pass the pointer to the allocated memory or
 * the memory to be deallocated.
 *
 * @tparam T Type of the object to be constructed and destructed
 */
template <typename T>
class BasicConstructDestruct
{
public:
    using pointer = T*;

    template <typename... Args>
    static void construct(pointer where, Args&&... args) noexcept(
        noexcept(T(htps::forward<Args>(args)...)))
    {
        assert(where != nullptr);

        if constexpr (sizeof...(args) == 3U)
        {
            ::new (static_cast<void*>(where)) T();
        }

        else if constexpr (sizeof...(args) > 0U)
        {
            ::new (static_cast<void*>(where)) T(htps::forward<Args>(args)...);
        }
        else
        {
            ::new (static_cast<void*>(where)) T{};
        }
    }

    static void destruct(pointer where) noexcept
    {
        assert(where != nullptr);
        where->~T();
    }
};

}  // namespace htps

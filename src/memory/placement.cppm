module;

#include <cassert>
#include <new>

export module htypes:placement;
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

    /**
     * @brief Construct an object in a given memory address.
     * @tparam Args The type of the args passed
     * @param where The address where to construct
     * @param args The actual params
     */
    template <typename... Args>
    static constexpr void construct(pointer where, Args&&... args) noexcept(
        noexcept(T(htps::forward<Args>(args)...)))
    {
        assert(where != nullptr);

        if constexpr (sizeof...(args) > 0U)
        {
            ::new (static_cast<void*>(where)) T(htps::forward<Args>(args)...);
        }
        else
        {
            ::new (static_cast<void*>(where)) T{};
        }
    }

    static void destruct(pointer where) noexcept(noexcept(where->~T()))
    {
        assert(where != nullptr);
        where->~T();
    }
};

}  // namespace htps

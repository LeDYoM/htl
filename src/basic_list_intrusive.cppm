module;

#include <initializer_list>
#include <cassert>
#include <concepts>

export module htypes:basic_intrusive_list;

namespace htps
{
template <class T>
concept DoublyLinkedNode = requires {
    requires std::same_as<decltype(&T::prev), T * T::*>;
    requires std::same_as<decltype(&T::next), T * T::*>;
};

/**
 * @brief Class implementing a list of nodes.
 * This basic implementation is the intrusive version. It will serve a a base to
 * implement the non intrusive version.
 * @tparam T Type of the element of the list. It must contain the required
 * members.
 * @tparam Allocator Allocator policy class.
 */
export template <typename T, typename Allocator>
class basic_list_intrusive final
{
    T* m_begin{nullptr};
    T* m_last{nullptr};

public:
    /**
     * @brief Default constructor, the list will contain 0 elements and be
     * empty.
     */
    constexpr basic_list_intrusive() noexcept = default;

    explicit constexpr basic_list_intrusive(T const& data) :
        m_begin{Allocator::make_one(data)}, m_last{m_begin}
    {}

    explicit constexpr basic_list_intrusive(T data) :
        m_begin{Allocator::make_one(move(data))}, m_last{m_begin}
    {}

    explicit constexpr basic_list_intrusive(T*& data) :
        m_begin{data}, m_last{m_begin}
    {
        data = nullptr;
    }
};
}  // namespace htps

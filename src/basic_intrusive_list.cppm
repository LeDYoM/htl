module;

#include <initializer_list>
#include <cassert>
#include <concepts>

export module htypes:basic_list;

namespace htps
{
template <class T>
concept DoublyLinkedNode = requires {
    requires std::same_as<decltype(&T::prev), T * T::*>;
    requires std::same_as<decltype(&T::next), T * T::*>;
};

export template <typename T, typename Allocator>
class basic_intrusive_list final
{
    T* m_root{nullptr};

public:
    constexpr basic_intrusive_list() noexcept = default;

    explicit constexpr basic_intrusive_list(T const& data) :
        m_root{All<T>::allocate(1U)}
    {}
};
}  // namespace htps

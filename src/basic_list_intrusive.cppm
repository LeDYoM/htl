module;

#include <initializer_list>
#include <cassert>
#include <concepts>

export module htypes:basic_intrusive_list;

import :btypes;

namespace htps
{
template <class T>
concept DoublyLinkedNode = requires {
    requires std::same_as<decltype(&T::prev), T * T::*>;
    requires std::same_as<decltype(&T::next), T * T::*>;
};

export template <typename T>
requires DoublyLinkedNode<T>
class basic_list_intrusive_iterator final
{
    T* m_pointer{nullptr};

public:
    using reference       = T&;        //< Reference to member
    using const_reference = const T&;  //< Const reference to members
    using pointer         = T*;        //< Pointer to the contained data type
    using const_pointer =
        T const*;  //< Pointer to const to the contained data type

    explicit constexpr basic_list_intrusive_iterator(T* element) noexcept :
        m_pointer{element}
    {}

    constexpr pointer operator->() noexcept { return m_pointer; }
    constexpr const_pointer operator->() const noexcept { return m_pointer; }

    constexpr pointer get() noexcept { return m_pointer; }
    constexpr const_pointer get() const noexcept { return m_pointer; }

    constexpr reference operator*() noexcept { return *m_pointer; }
    constexpr const_reference operator*() const noexcept { return *m_pointer; }

    // Prefix increment
    basic_list_intrusive_iterator& operator++()
    {
        m_pointer = m_pointer->next;
        return *this;
    }

    // Postfix increment
    basic_list_intrusive_iterator operator++(int)
    {
        basic_list_intrusive_iterator tmp{*this};
        (void)(++(*this));
        return tmp;
    }

    bool operator==(basic_list_intrusive_iterator const& rhs) const noexcept =
        default;
};

export template <typename T>
class basic_list_intrusive_const_iterator final
{
    T const* m_pointer{nullptr};

public:
    using const_reference = const T&;  //< Const reference to members
    using const_pointer =
        T const*;  //< Pointer to const to the contained data type

    explicit constexpr basic_list_intrusive_const_iterator(
        T const* element) noexcept :
        m_pointer{element}
    {}

    constexpr const_pointer operator->() const noexcept { return m_pointer; }
    constexpr const_pointer get() const noexcept { return m_pointer; }
    constexpr const_reference operator*() const noexcept { return *m_pointer; }

    // Prefix increment
    basic_list_intrusive_const_iterator& operator++()
    {
        m_pointer = m_pointer->next;
        return *this;
    }

    // Postfix increment
    basic_list_intrusive_const_iterator operator++(int)
    {
        basic_list_intrusive_const_iterator tmp{*this};
        (void)(++(*this));
        return tmp;
    }

    bool operator==(basic_list_intrusive_const_iterator const& rhs)
        const noexcept = default;
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
    using iterator =
        basic_list_intrusive_iterator<T>;  //< Iterator for the values
    using const_iterator =
        basic_list_intrusive_const_iterator<T>;  //< Iterator pointing to const
                                                 // values
    using reference       = T&;                  //< Reference to member
    using rv_reference    = T&&;                 //< r-value reference to member
    using const_reference = const T&;            //< Const reference to members
    using value_type      = T;   //< Value type of the contained data member
    using pointer         = T*;  //< Pointer to the contained data type
    using const_pointer =
        T const*;  //< Pointer to const to the contained data type
    using size_type = size_type;

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

    explicit constexpr basic_list_intrusive(const_iterator begin,
                                            const_iterator end) :
        m_begin{begin}
    {}

    constexpr bool empty() const noexcept { return m_begin == nullptr; }

    constexpr size_type size() const noexcept
    {
        size_type current{0U};
        const_iterator it{m_begin};

        while (it.get() != nullptr)
        {
            ++it;
            ++current;
        }
        return current;
    }

    void push_back(T* value)
    {
        value->next = nullptr;

        if (m_last == nullptr)
        {
            // List is empty
            value->prev = nullptr;
            m_begin     = value;
            m_last      = value;
        }
        else
        {
            // List is not empty
            value->prev  = m_last;
            m_last->next = value;
            m_last = value;
        }
    }

    void push_back(T const& value)
    {
        T* new_value{Allocator::make_one(value)};
        push_back(new_value);
    }

    void push_back(T&& value)
    {
        T* new_value{Allocator::make_one(move(value))};
        push_back(new_value);
    }

    template <typename... Args>
    constexpr void emplace_back(Args&&... args)
    {
        T* new_value{Allocator::make_one(forward<Args>(args)...)};
        push_back(new_value);
    }

    T const& front() const { return *m_begin; }

    T& front() { return *m_begin; }

    T const& back() const { return *m_last; }

    T& back() { return *m_last; }

    void pop_back() {}
};
}  // namespace htps

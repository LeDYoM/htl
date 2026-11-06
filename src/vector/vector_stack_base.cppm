module;

#include <initializer_list>
#include <cassert>

export module htypes:vector_stack_base;

import :btypes;

namespace htps
{
export template <typename T, size_type N, typename ConstructDestruct>
class vector_stack_base final
{
    size_type m_size{0U};
    alignas(alignof(T)) u8 m_storage[alignof(T) * N];

public:
    using iterator        = T*;        //< Iterator for the values
    using const_iterator  = T const*;  //< Iterator pointing to const values
    using reference       = T&;        //< Reference to member
    using rv_reference    = T&&;       //< r-value reference to member
    using const_reference = const T&;  //< Const reference to members
    using value_type      = T;   //< Value type of the contained data member
    using pointer         = T*;  //< Pointer to the contained data type
    using const_pointer =
        T const*;  //< Pointer to const to the contained data type
    using size_type = size_type;

    /**
     * @brief Default constructor.
     * Constructs an empty object.
     */
    constexpr vector_stack_base() noexcept = default;

    /**
     * @brief Constructor with reserved memory and copy from source.
     * This constructor takes a pointer and a number of elements
     * and constructs a vector from it.
     *
     * @param source Pointer to the first element of the
     * @param count Number of elements to copy.
     */
    constexpr vector_stack_base(const_iterator const source, size_type count)
    {
        auto iterator{source};
        auto const end{source + count};
        while (iterator != end)
        {
            // Construct by copy.
            push_back((*iterator++));
        }
    }

    /**
     * @brief Construct a vector_base from an initializer list of elements
     * @param iList Initializer list of elements
     */
    constexpr vector_stack_base(std::initializer_list<value_type> ilist) :
        vector_stack_base(ilist.begin(), ilist.size())
    {}

    /**
     * @brief Construct a vector_base from two iterators
     * @param _begin Iterator pointing to the first element
     * @param _end Iterator pointing one past the end of the range of elements
     */
    constexpr vector_stack_base(const_iterator const _begin,
                                const_iterator const _end) :
        vector_stack_base{_begin, static_cast<size_type>(_end - _begin)}
    {}

    /**
     * @brief Copy constructor.
     * This constructor constructs a vector from another one.
     * The capacity of the resultant vector might be different
     * from the capacity of the source. The size will be the same.
     *
     * @param other Source vector to copy.
     */
    constexpr vector_stack_base(vector_stack_base const& other) :
        vector_stack_base{other.begin(), other.begin() + other.size()}
    {}

    /**
     * @brief Move constructor
     * @param other Object to move from
     */
    constexpr vector_stack_base(vector_stack_base&& other) noexcept
    {
        for (auto it{other.begin()}; it != other.end(); ++it)
        {
            *(begin()) = move(*it);
        }
    }

    /**
     * @brief Copy assignment
     *
     * @param other Object to copy from
     * @return Reference to the newly assigned object
     */
    constexpr vector_stack_base& operator=(vector_stack_base const& other)
    {
        if (this != &other)
        {
            clear();
            insert(other);
        }
        return *this;
    }

    /// Move assignment
    constexpr vector_stack_base& operator=(vector_stack_base&& other) noexcept
    {
        m_storage.swap(other.m_storage);
        return *this;
    }

    /// Destructor
    ~vector_stack_base() noexcept { clear(); }

    constexpr void clear() noexcept
    {
        while (size() > 0U)
        {
            pop_back();
        }
    }

    constexpr void pop_back() noexcept
    {
        if (m_size > 0U)
        {
            ConstructDestruct::destruct(end() - 1U);
            --m_size;
        }
    }

    constexpr reference operator[](size_type const index)
    {
        assert(index < size());
        return reinterpret_cast<reference>(*(&(m_storage[alignof(T) * index])));
    }

    constexpr const_reference operator[](const size_type index) const
    {
        assert(index < size());
        return reinterpret_cast<const_reference>(
            *(&(m_storage[alignof(T) * index])));
    }

    constexpr size_type capacity() const noexcept { return N; }

    constexpr size_type size() const noexcept { return m_size; }

    constexpr bool empty() const noexcept { return m_size == 0U; }

    constexpr bool full() const noexcept { return m_size == N; }

    constexpr iterator begin() noexcept
    {
        return reinterpret_cast<iterator>(m_storage);
    }

    constexpr const_iterator begin() const noexcept
    {
        return reinterpret_cast<const_iterator>(m_storage);
    }

    constexpr const_iterator cbegin() const noexcept { return begin(); }

    constexpr iterator end() noexcept
    {
        return reinterpret_cast<iterator>(m_storage + (alignof(T) * m_size));
    }

    constexpr const_iterator end() const noexcept
    {
        return reinterpret_cast<const_iterator>(m_storage +
                                                (alignof(T) * m_size));
    }

    constexpr const_iterator cend() const noexcept { return m_storage.cend(); }

    constexpr T& back() noexcept
    {
        assert(!empty());
        return *(m_storage.at(size() - 1U));
    }

    constexpr T const& back() const noexcept
    {
        assert(!empty());
        return *(m_storage.at(size() - 1U));
    }

    constexpr T const& cback() const noexcept
    {
        assert(!empty());
        return *(m_storage.cat(size() - 1U));
    }

    constexpr T& front() noexcept
    {
        assert(!empty());
        return *(m_storage.at(0U));
    }

    constexpr T const& front() const noexcept
    {
        assert(!empty());
        return *(m_storage.at(0U));
    }

    constexpr T const& cfront() const noexcept
    {
        assert(!empty());
        return *(m_storage.at(0U));
    }

    constexpr void push_back(const T& value)
    {
        ConstructDestruct::construct(end(), value);
        ++m_size;
    }

    constexpr void push_back(T&& value)
    {
        ConstructDestruct::construct(end(), move(value));
        ++m_size;
    }

    template <typename... Args>
    constexpr void emplace_back(Args&&... args)
    {
        ConstructDestruct::construct(end(), forward<Args>(args)...);
    }
};

}  // namespace htps

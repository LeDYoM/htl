module;

#include <initializer_list>
#include <cassert>
#include <iterator>
#include <cstddef>

export module htypes:array;

import :btypes;
import :span;
import :weak_ptr;
import :algos;

namespace htps
{
/**
 * @brief An array class.
 * @note The main difference of this class with the standard array class is,
 * that the standard mandates an aggregate and this class is not.
 */
export template <class T, size_t array_size>
class array final
{
public:
    using iterator         = T*;
    using const_iterator   = const T*;
    using reference        = T&;
    using const_reference  = const T&;
    using rvalue_reference = T&&;
    using size_t           = size_type;
    using value_type       = T;
    using const_value_type = T const;

    /**
     * @brief Default constructor
     * Default initializes all elements
     */
    constexpr array() noexcept(noexcept(
        std::is_nothrow_default_constructible_v<value_type>)) = default;

    /**
     * @brief Initialize from an initializer_list<value_type>
     * This constructor copies the data from the initializer_list parameter
     * into the array buffer.
     * If the iList size if less than the size of the array, the missing
     * elements will be filled with the last element of the argument list.
     * @param iList [in] The source list
     * @note std::initializer_list is an inmutable class, nothing will be
     * move from it
     */
    constexpr array(std::initializer_list<value_type> iList) noexcept(
        noexcept(std::is_nothrow_copy_assignable_v<value_type>))
    {
        auto buffer_element{&m_buffer[0]};

        // Copy everything from initializer list
        for (auto&& element : iList)
        {
            (*buffer_element++) = element;
        }

        // If after copying from initializer list there are still free places,
        // expand the last element of the list and fill the free places.
        if (iList.size() < array_size && !iList.empty())
        {
            size_t counter{iList.size()};
            buffer_element = &m_buffer[counter];
            while (counter++ < array_size)
            {
                (*buffer_element++) = m_buffer[iList.size() - 1U];
            }
        }
    }

    constexpr array(const T* const source, const size_type count) noexcept(
        noexcept(std::is_nothrow_copy_assignable_v<value_type>)) :
        array{}
    {
        assert(array_size >= count);
        size_type index{0U};

        auto const iterator_end{source + count};
        for (auto iterator{source}; iterator != iterator_end; ++iterator)
        {
            m_buffer[index++] = *iterator;
        }
    }

    constexpr array(const_iterator const _begin,
                    const_iterator const _end) noexcept(noexcept(array{
        _begin, static_cast<size_type>(std::distance(_begin, _end))})) :
        array{_begin, static_cast<size_type>(std::distance(_begin, _end))}
    {}

    constexpr array(span<T const> const rhs) : array{rhs.cbegin(), rhs.cend()}
    {}

    constexpr array(span<T> const rhs) : array{rhs.cbegin(), rhs.cend()} {}

    constexpr void insert(size_type const index, value_type&& element) noexcept
    {
        if (index < array_size)
        {
            for (size_type i{array_size - 2U};
                 i >= index && (i < static_cast<size_type>(-1)); --i)
            {
                m_buffer[i + 1] = move(m_buffer[i]);
            }
            m_buffer[index] = move(element);
        }
    }

    constexpr reference operator[](size_t const index) noexcept
    {
        return m_buffer[index];
    }

    constexpr const_reference operator[](const size_t index) const noexcept
    {
        return m_buffer[index];
    }

    [[nodiscard]] constexpr size_t size() const noexcept { return array_size; }

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return array_size == 0U;
    }

    [[nodiscard]] constexpr iterator begin() noexcept { return m_buffer; }

    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return m_buffer;
    }

    [[nodiscard]] constexpr iterator end() noexcept
    {
        return m_buffer + array_size;
    }
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return m_buffer + array_size;
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return begin();
    }
    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return end();
    }
    [[nodiscard]] constexpr reference front() noexcept { return *begin(); }

    [[nodiscard]] constexpr reference back() noexcept
    {
        return m_buffer[array_size - 1U];
    }

    [[nodiscard]] constexpr const_reference front() const noexcept
    {
        return *cbegin();
    }

    [[nodiscard]] constexpr const_reference back() const noexcept
    {
        return m_buffer[array_size - 1U];
    }

    [[nodiscard]] constexpr const_reference cfront() const noexcept
    {
        return *cbegin();
    }

    [[nodiscard]] constexpr const_reference cback() const noexcept
    {
        return back();
    }

    constexpr array& operator=(span<T> const& rhs)
    {
        *this = array{rhs};
        return *this;
    }

    constexpr array& operator=(span<T const> const& rhs)
    {
        *this = array{rhs};
        return *this;
    }

    [[nodiscard]] constexpr bool operator==(array const& rhs) const noexcept
    {
        return equal(cbegin(), cend(), rhs.cbegin());
    }

private:
    T m_buffer[array_size];
};

export template <typename T, size_type S>
using array_shared_pointers = array<sptr<T>, S>;

export template <typename T, size_type S>
using array_unique_pointers = array<uptr<T>, S>;

export template <typename T, size_type S>
using array_weak_pointers = array<wptr<T>, S>;

export template <typename Tp, typename... Up>
array(Tp, Up...) -> array<std::enable_if_t<(std::is_same_v<Tp, Up> && ...), Tp>,
                          1 + sizeof...(Up)>;

export template <typename T>
array(T) -> array<T, 1U>;

}  // namespace htps

module;

#include <initializer_list>
#include <cassert>
#include <iterator>
#include <cstddef>

export module htypes:array;

import :btypes;
import :span;
import :weak_ptr;

namespace htps
{
export template <class T, size_t array_size>
class array
{
public:
    using iterator         = T*;
    using const_iterator   = const T*;
    using reference        = T&;
    using const_reference  = const T&;
    using size_t           = size_type;
    using value_type       = T;
    using const_value_type = const T;

    [[nodiscard]] array() = default;

    [[nodiscard]] constexpr array(
        std::initializer_list<value_type> iList) noexcept
    {
        //        static_assert(iList.size() <= array_size);
        auto buffer_element{&buffer_[0]};

        for (auto&& element : iList)
        {
            (*buffer_element++) = htps::move(element);
        }

        if (iList.size() < array_size && iList.size() > 0U)
        {
            size_t counter{iList.size()};
            buffer_element = &buffer_[counter];
            while (counter++ < array_size)
            {
                (*buffer_element++) = buffer_[iList.size() - 1U];
            }
        }
    }

    [[nodiscard]] constexpr array(const T* const source,
                                  const size_type count) :
        array{}
    {
        assert(array_size >= count);
        size_type index{0U};

        auto const iterator_end{source + count};
        for (auto iterator{source}; iterator != iterator_end; ++iterator)
        {
            buffer_[index++] = *iterator;
        }
    }

    [[nodiscard]] constexpr array(const_iterator const _begin,
                                  const_iterator const _end) :
        array{_begin, static_cast<size_type>(std::distance(_begin, _end))}
    {}

    [[nodiscard]] constexpr array(span<T const> const rhs) :
        array{rhs.cbegin(), rhs.cend()}
    {}

    [[nodiscard]] constexpr array(span<T> const rhs) :
        array{rhs.cbegin(), rhs.cend()}
    {}

    constexpr array(array&&)      = default;
    constexpr array(const array&) = default;
    constexpr array& operator=(array&&) = default;
    constexpr array& operator=(const array&) = default;

    constexpr void insert(size_type const index, value_type element) noexcept
    {
        if (index < array_size)
        {
            for (size_type i{array_size - 2U};
                 i >= index && (i < static_cast<size_type>(-1)); --i)
            {
                buffer_[i + 1] = htps::move(buffer_[i]);
            }
            buffer_[index] = htps::move(element);
        }
    }

    [[nodiscard]] constexpr reference operator[](size_t const index) noexcept
    {
        return buffer_[index];
    }

    [[nodiscard]] constexpr const_reference operator[](
        const size_t index) const noexcept
    {
        return buffer_[index];
    }

    [[nodiscard]] constexpr size_t size() const noexcept { return array_size; }
    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return array_size == 0U;
    }
    [[nodiscard]] constexpr iterator begin() noexcept { return buffer_; }
    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return buffer_;
    }
    [[nodiscard]] constexpr iterator end() noexcept
    {
        return buffer_ + array_size;
    }
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return buffer_ + array_size;
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
        return buffer_[array_size - 1U];
    }
    [[nodiscard]] constexpr const_reference front() const noexcept
    {
        return cbegin();
    }
    [[nodiscard]] constexpr const_reference back() const noexcept
    {
        return buffer_[array_size > 0U ? (array_size - 1U) : 0U];
    }
    [[nodiscard]] constexpr const_reference cfront() const noexcept
    {
        return cbegin();
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

private:
    T buffer_[array_size];
};

export template <class T, size_type array_size>
[[nodiscard]] constexpr bool operator==(
    array<T, array_size> const& lhs,
    array<T, array_size> const& rhs) noexcept
{
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

export template <typename T, size_type S>
using array_shared_pointers = array<sptr<T>, S>;

export template <typename T, size_type S>
using array_unique_pointers = array<uptr<T>, S>;

export template <typename T, size_type S>
using array_weak_pointers = array<wptr<T>, S>;

export template <typename _Tp, typename... _Up>
array(_Tp, _Up...)
    -> array<std::enable_if_t<(std::is_same_v<_Tp, _Up> && ...), _Tp>,
             1 + sizeof...(_Up)>;

export template <typename T>
array(T) -> array<T, 1U>;

}  // namespace htps

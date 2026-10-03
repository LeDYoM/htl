module;

#include <compare>

export module htypes:str_view;

import :btypes;
import :str_functions;
import :str_literal;
import :algoutils;

namespace htps
{
export template <typename char_type>
class basic_str_view
{
public:
    using value_type = char_type;  //< Type of the characters used
    using const_reference =
        char_type const&;  //< Type of a const reference to a char_type;
    using const_iterator =
        char_type const*;  //< Const iterator type of the string

    constexpr basic_str_view() noexcept : m_begin{nullptr}, m_size{0U} {}

    constexpr basic_str_view(const_iterator const begin,
                             size_type const size) noexcept :
        m_begin{begin}, m_size{size}
    {}

    constexpr basic_str_view(const_iterator const begin,
                             const_iterator const end) noexcept :
        m_begin{begin}, m_size{static_cast<size_type>(end - begin)}
    {}

    template <size_type N>
    constexpr basic_str_view(basic_str_literal<char_type, N> data) = delete;

    template <size_type N>
    constexpr basic_str_view(char_type const (&data)[N]) noexcept :
        m_begin{&data[0U]}, m_size{N - 1}
    {}

    constexpr explicit basic_str_view(const_iterator const data) noexcept :
        basic_str_view{&data[0U], strnlen(data)}
    {}

    [[nodiscard]] constexpr const_iterator data() const noexcept
    {
        return m_begin;
    }
    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return m_begin;
    }
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return m_begin + m_size;
    }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return begin();
    }
    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return end();
    }
    [[nodiscard]] constexpr char_type operator[](
        size_type const index) const noexcept
    {
        return *(m_begin + index);
    }

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return m_begin == nullptr || m_size == 0U ||
            (m_size > 0U && *m_begin == 0);
    }

    [[nodiscard]] constexpr size_type size() const noexcept { return m_size; }

    [[nodiscard]] constexpr bool operator==(
        basic_str_view const& rhs) const noexcept
    {
        if (size() != rhs.size())
        {
            return false;
        }
        else
        {
            return strnncmp(m_begin, size(), rhs.m_begin, rhs.size()) == 0;
        }
    }

    [[nodiscard]] constexpr bool operator==(
        char_type* const& rhs) const noexcept
    {
        return *this == basic_str_view{rhs};
    }

    template <size_type N>
    [[nodiscard]] constexpr bool operator==(
        char_type const (&rhs)[N]) const noexcept
    {
        return *this == basic_str_view{rhs};
    }

    [[nodiscard]] constexpr auto operator<=>(
        const basic_str_view& rhs) const noexcept
    {
        auto const result{strnncmp(m_begin, size(), rhs.m_begin, rhs.size())};
        return (result < 0 ? std::strong_ordering::less
                           : result > 0 ? std::strong_ordering::greater
                                        : std::strong_ordering::equal);
    }

    [[nodiscard]] constexpr size_type find(
        const_iterator str_to_find) const noexcept
    {
        return strnfind(m_begin, str_to_find);
    }

    [[nodiscard]] constexpr basic_str_view offset(
        size_type index) const noexcept
    {
        index = min({index, m_size});
        return basic_str_view{m_begin + index,
                              static_cast<size_type>(m_size - index)};
    }

    [[nodiscard]] constexpr bool starts_with(
        const char_type* to_find) const noexcept
    {
        return htps::starts_with(m_begin, to_find);
    }

    [[nodiscard]] constexpr bool starts_with(
        basic_str_view to_find) const noexcept
    {
        return starts_with(m_begin, to_find.m_begin);
    }

    const_iterator m_begin;
    size_type m_size;
};

export using str_view = basic_str_view<char>;

}  // namespace htps

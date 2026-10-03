module;

#include <algorithm>

export module htypes:str_literal;

import :btypes;

namespace htps
{
export using str_literal_hash = size_type;

export template <typename char_type, size_type N>
struct basic_str_literal
{
    consteval basic_str_literal(char_type const (&str)[N]) noexcept :
        m_real_size{N}
    {
        std::copy_n(str, N, value);
    }

    constexpr size_type size() const noexcept { return m_real_size - 1U; }

    constexpr bool empty() const noexcept { return m_real_size == 1U; }

    constexpr char_type operator[](size_type const index) const noexcept
    {
        return value[index];
    }

    constexpr char_type* begin() noexcept { return value; }
    constexpr char_type* end() noexcept { return &(value[m_real_size]); }
    constexpr char_type const* begin() const noexcept { return value; }
    constexpr char_type const* end() const noexcept
    {
        return &(value[m_real_size]);
    }

    constexpr char_type const* cbegin() const noexcept { return value; }
    constexpr char_type const* cend() const noexcept
    {
        return &(value[m_real_size]);
    }

    constexpr bool operator==(basic_str_literal const& rhs) const noexcept
    {
        if (size() == rhs.size())
        {
            return std::equal(cbegin(), cend(), rhs.cbegin(), rhs.cend());
        }
        else
        {
            return false;
        }
    }

    template <size_type N2>
    constexpr bool operator==(
        basic_str_literal<char_type, N2> const&) const noexcept
    {
        return false;
    }

    template <size_type N2>
    constexpr bool operator==(char_type const (&rhs)[N2]) const
    {
        if (size() == (N2 - 1U))
        {
            return std::equal(cbegin(), cend(), rhs, (rhs + N2));
        }
        else
        {
            return false;
        }
    }

    constexpr str_literal_hash hash() const noexcept { return value[0]; }

    const size_type m_real_size;
    char_type value[N];
};

export template <size_type N>
using str_literal = basic_str_literal<char, N>;

}  // namespace htps

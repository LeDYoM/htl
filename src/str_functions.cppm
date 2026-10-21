export module htypes:str_functions;

import :btypes;

namespace htps
{
static constexpr size_type npos{static_cast<size_type>(-1)};

export [[nodiscard]] constexpr size_type strnlen(std::nullptr_t,
                                          size_type = npos) noexcept
{
    return 0U;
}

export template <typename CharType>
[[nodiscard]] constexpr size_type strnlen(CharType const* lhs,
                                          size_type max_len = npos) noexcept
{
    size_type len{0UL};
    if (lhs != nullptr)
    {
        while ((*lhs++) != 0 && len < max_len)
        {
            ++len;
        }
    }
    return len;
}

export template <typename CharType>
[[nodiscard]] constexpr int strnncmp(CharType const* const lhs,
                                     size_type lhs_size,
                                     CharType const* const rhs,
                                     size_type rhs_size,
                                     size_type start   = 0U,
                                     size_type max_len = npos) noexcept
{
    if (max_len < lhs_size)
    {
        lhs_size = max_len;
    }

    if (max_len < rhs_size)
    {
        rhs_size = max_len;
    }

    auto const min_size{lhs_size < rhs_size ? lhs_size : rhs_size};
    auto const min_size_total{min_size < max_len ? min_size : max_len};

    for (size_type i{start}; i < min_size_total; ++i)
    {
        if (lhs[i] < rhs[i])
        {
            return -1;
        }
        else if (lhs[i] > rhs[i])
        {
            return 1;
        }
    }

    return (lhs_size < rhs_size ? -1 : lhs_size > rhs_size ? 1 : 0);
}

/**
 * @brief Compare two C-style strings
 * Make a comparison of two C-style strings given a starting point and a maximum
 * length
 * @tparam CharType Type of the char for the strings
 * @param lhs Left hand side parameter
 * @param rhs Right hand side parameter
 * @param start First index to compare
 * @param max_index Last index to compare
 * @return int Negative value if lhs < rhs, 0 if lhs == rhs and positive value
 * if lhs > rhs. The concept of beeing smaller or greater is given by char
 * comparison and size
 */
export template <typename CharType>
[[nodiscard]] constexpr int strncmp(CharType const* const lhs,
                                    CharType const* const rhs,
                                    size_type start   = 0U,
                                    size_type max_len = npos) noexcept
{
    auto lhs_size{strnlen(lhs)};
    if (max_len < lhs_size)
    {
        lhs_size = max_len;
    }

    auto rhs_size{strnlen(rhs)};
    if (max_len < rhs_size)
    {
        rhs_size = max_len;
    }

    auto const& min_size{lhs_size < rhs_size ? lhs_size : rhs_size};
    auto const& min_size_total{min_size < max_len ? min_size : max_len};

    for (size_type i{start}; i < min_size_total; ++i)
    {
        if (lhs[i] < rhs[i])
        {
            return -1;
        }
        else if (lhs[i] > rhs[i])
        {
            return 1;
        }
    }

    return (lhs_size < rhs_size ? -1 : lhs_size > rhs_size ? 1 : 0);
}

export template <typename CharType>
[[nodiscard]] constexpr bool starts_with(const CharType* str_text,
                                         const CharType* to_find) noexcept
{
    if (str_text == nullptr || to_find == nullptr)
    {
        return false;
    }

    while (*str_text != 0 && *to_find != 0)
    {
        if (*str_text != *to_find)
        {
            return false;
        }
        ++str_text;
        ++to_find;
    }

    return *to_find == 0;
}

export template <typename CharType>
[[nodiscard]] constexpr size_type strnfind(
    const CharType* const str_text,
    const CharType* const to_find) noexcept
{
    if (str_text == nullptr || to_find == nullptr)
    {
        return npos;
    }

    if (*to_find == 0)
    {
        return 0U;
    }

    auto* begin{str_text};
    size_type len{0U};

    while (*begin != 0)
    {
        if (starts_with(begin, to_find))
        {
            return len;
        }
        ++len;
        ++begin;
    }

    return npos;
}

}  // namespace htps

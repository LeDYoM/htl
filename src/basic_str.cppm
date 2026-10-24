module;

#include <sstream>
#include <type_traits>
#include <algorithm>
#include <cctype>
#include <compare>
#include <format>
#include <concepts>

export module htypes:bstr;

import :vector;
import :str_view;
import :str_literal;
import :btypes;
import :pair;

namespace htps
{
/**
 * @brief Template class containing a string like class dependant on the
 * character type. It provides functions for adding stream and making
 * search of characters, substrings and other useful operations.
 *
 * @tparam char_type Character type to be used
 */
export template <typename char_type>
class basic_str
{
public:
    using value_type = char_type;   //< Type of the characters used
    using reference  = char_type&;  //< Type reference of the characters used
    using const_reference =
        char_type const&;         //< Type of a const reference to a char_type;
    using iterator = char_type*;  //< Iterator value of the string;
    using const_iterator =
        char_type const*;  //< Const iterator type of the string
    using size_type = vector<char_type>::size_type;

private:
    vector<char_type> m_data;

public:
    constexpr basic_str() noexcept = default;

    template <size_type N>
    constexpr basic_str(char_type const (&a)[N]) : m_data(a, N)
    {}

    explicit constexpr basic_str(size_type const size) : m_data(size) {}

    constexpr basic_str(const_iterator const n, size_type const N) :
        m_data(n, N + 1)
    {}

    constexpr explicit basic_str(basic_str_view<char_type> other) :
        basic_str{other.m_begin, other.m_size}
    {}

    template <size_type N>
    constexpr explicit basic_str(basic_str_literal<char_type, N> other) :
        basic_str{other.value, N - 1U}
    {}

    constexpr explicit basic_str(char_type const* const n) :
        basic_str(n, strnlen(n))
    {}

    constexpr basic_str(const_iterator _begin, const_iterator _end) :
        basic_str(_begin, static_cast<size_type>(_end - _begin) + 1U)
    {}

    constexpr basic_str(basic_str&&) noexcept  = default;
    basic_str& operator=(basic_str&&) noexcept = default;

    constexpr basic_str(basic_str const& n)              = default;
    constexpr basic_str& operator=(basic_str const& rhs) = default;

    basic_str& operator=(char_type const* const n)
    {
        auto const len{strnlen(n)};
        if (len < m_data.capacity())
        {
            m_data.resize(len + 1);
            for (decltype(strnlen(n)) i{0U}; i < len; ++i)
            {
                m_data[i] = n[i];
            }
            m_data[len] = static_cast<char_type>(0);
        }
        else
        {
            *this = basic_str(n, len);
        }
        return *this;
    }

    constexpr void reserve(size_type const capacity)
    {
        m_data.reserve(capacity);
    }

    [[nodiscard]] basic_str_view<char_type> to_view() const noexcept
    {
        return basic_str_view<char_type>{cbegin(), size()};
    }

    void swap(basic_str& other) { m_data.swap(other.m_data); }

    static basic_str fromCharAndSize(char_type const ch, size_type const size)
    {
        basic_str result(size + 1U);
        for (size_type i{0U}; i < size; ++i)
        {
            result.push_back(ch);
        }
        return result;
    }

    template <typename T, size_type N = 32U>
    static basic_str convert_to_imp(T const n)
    {
        char buffer[N];
        std::format_to_n_result<char*> r{std::format_to_n(buffer, N, "{}", n)};
        *(r.out) = 0;
        return basic_str{buffer, static_cast<size_type>(r.size)};
    }

    static basic_str to_str(u64 const n) { return convert_to_imp(n); }

    static basic_str to_str(s64 const n) { return convert_to_imp(n); }

    static basic_str to_str(u32 const n) { return convert_to_imp(n); }

    static basic_str to_str(s32 const n) { return convert_to_imp(n); }

    static basic_str to_str(f32 const n) { return convert_to_imp(n); }

    static basic_str to_str(f64 const n) { return convert_to_imp(n); }

    template <typename T>
        requires std::is_default_constructible_v<T>
    bool is() const
    {
        T temp;
        return convert(temp);
    }

    vector<basic_str> split(char_type const& separator) const
    {
        basic_str temp;
        temp.push_back(separator);
        return split(temp);
    }

    vector<basic_str> split(basic_str const& separator) const
    {
        vector<basic_str> result;
        auto _this{*this};

        if (!separator.empty())
        {
            do
            {
                size_type position{_this.cfind(separator)};
                if (position == npos)
                {
                    result.push_back(htps::move(_this));
                    _this.clear();
                }
                else
                {
                    result.push_back(_this.substr(0U, position));
                    _this = _this.substr(position + separator.size());
                    if (_this.empty())
                    {
                        result.push_back("");
                    }
                }

            } while (!_this.empty());
        }
        else
        {
            result.push_back(htps::move(_this));
        }
        return result;
    }

    bool starts_with(basic_str const& prefix) const noexcept
    {
        return has_in(prefix, 0U);
    }

    bool ends_with(basic_str const& prefix) const noexcept
    {
        return has_in(prefix, size() - prefix.size());
    }

    bool has_in(basic_str const& prefix,
                size_type const start_position) const noexcept
    {
        if (size() < prefix.size() || start_position > size())
        {
            return false;
        }

        return (substr(start_position, prefix.size()) == prefix);
    }

    size_type has(basic_str const& sub_str) const noexcept
    {
        if (size() < sub_str.size())
        {
            return npos;
        }

        if (sub_str.empty())
        {
            return 0U;
        }

        auto const size_to_check{size() - (sub_str.size() - 1U)};

        for (size_type i{0U}; i < size_to_check; ++i)
        {
            if (has_in(sub_str, i))
            {
                return i;
            }
        }
        return npos;
    }

    basic_str substr(size_type start, size_type len = npos) const
    {
        if (start >= size() || len < 1U)
        {
            return "";
        }

        basic_str temp;
        while (start < size() && len > 0U)
        {
            if (m_data[start] != 0)
            {
                temp.m_data.push_back(m_data[start]);
            }
            ++start;
            --len;
        }

        temp.m_data.push_back(0);
        return temp;
    }

    constexpr basic_str& append() { return *this; }

    basic_str& append(basic_str const& n)
    {
        m_data.pop_back();
        m_data.insert(n.m_data);
        return *this;
    }

    basic_str& append(basic_str&& n)
    {
        m_data.pop_back();
        m_data.insert(htps::move(n.m_data));
        return *this;
    }

    template <size_type N>
    constexpr basic_str& append(char_type const (&n)[N])
    {
        append(str(htps::forward<char_type const(&)[N]>(n)));
        return *this;
    }

    basic_str& append(char_type const* const n)
    {
        append(basic_str{n});
        return *this;
    }

    basic_str& append(u64 const n)
    {
        append(to_str(n));
        return *this;
    }

    basic_str& append(s64 const n)
    {
        append(to_str(n));
        return *this;
    }

    basic_str& append(u32 const n)
    {
        append(to_str(n));
        return *this;
    }

    basic_str& append(s32 const n)
    {
        append(to_str(n));
        return *this;
    }

    basic_str& append(f32 const n)
    {
        append(to_str(n));
        return *this;
    }

    basic_str& append(f64 const n)
    {
        append(to_str(n));
        return *this;
    }

    basic_str& append(basic_str_view<char_type> const n)
    {
        append(n.cbegin());
        return *this;
    }

    basic_str& push_back(char_type const n)
    {
        char_type const temp[2U] = {n, 0U};
        return append(htps::move(temp));
    }

    template <typename T>
    bool convert(T& n) const
    {
        std::istringstream iss(c_str());
        iss >> n;
        return (iss.eof() && !iss.fail());
    }

    template <typename T>
    [[nodiscard]] pair<bool, T> convert() const
    {
        T value;
        return {convert(value), value};
    }

    template <typename T>
    [[nodiscard]] T convertOrDefault() const
    {
        T value;
        convert(value);
        return value;
    }

    template <typename T>
    basic_str& operator+=(T&& source)
    {
        return append(htps::forward<T>(source));
    }

    template <size_type N>
    basic_str& operator+=(char_type const (&a)[N])
    {
        return append(a);
    }

    basic_str& operator+=(basic_str const& source) { return append(source); }

    basic_str operator+(basic_str<value_type> const& rhs) const
    {
        return basic_str<value_type>(*this).append(rhs);
    }

    basic_str operator+(char_type const* const rhs) const
    {
        return basic_str<value_type>(*this).append(rhs);
    }

    constexpr size_type size() const noexcept
    {
        return m_data.empty() ? 0 : (m_data.size() - 1U);
    }

    void ltrim()
    {
        if (!empty())
        {
            for (size_type index{0U}; index < size(); ++index)
            {
                if (!std::isspace(static_cast<char_type>(m_data[index])))
                {
                    *this = substr(index);
                    return;
                }
            }
            m_data.clear();
        }
    }

    constexpr void clear() noexcept
    {
        if (!m_data.empty())
        {
            m_data[0] = static_cast<char_type>(0);
        }
        m_data.clear();
    }

    constexpr size_type cfind(char_type const ch) const noexcept
    {
        const auto it(m_data.cfind(ch));
        return ((it == m_data.cend()) ? npos : std::distance(cbegin(), it));
    }

    constexpr size_type cfind(basic_str const& other) const noexcept
    {
        if (!other.empty() && !empty() && other.size() <= size())
        {
            auto next_start{cbegin()};
            while (next_start != cend())
            {
                auto const it_start{
                    m_data.cfind(next_start, m_data.cend(), *other.begin())};
                bool continue_inner{it_start != m_data.cend()};
                if (continue_inner)
                {
                    auto it_end{it_start + 1};
                    for (size_type i{1U}; i < other.size() && continue_inner;
                         ++i, ++it_end)
                    {
                        if (it_end == cend())
                        {
                            return npos;
                        }
                        else if (*it_end != other[i])
                        {
                            continue_inner = false;
                            ++next_start;
                        }
                    }
                }
                else
                {
                    next_start = cend();
                }
                if (continue_inner)
                {
                    return static_cast<size_type>(
                        std::distance(cbegin(), it_start));
                }
            }
        }
        return npos;
    }

    constexpr size_type find(char_type const ch) const noexcept
    {
        if (m_data.empty())
        {
            return npos;
        }
        else
        {
            auto const it{m_data.cfind(m_data.cbegin(), &(m_data.back()), ch)};
            return ((it == &m_data.back()) ? npos : std::distance(cbegin(), it));
        }
    }

    constexpr size_type find(basic_str const& other) const noexcept
    {
        return cfind(other);
    }

    // trim from end (in place)
    void rtrim()
    {
        if (!empty())
        {
            for (size_type index{size() - 1U}; index > 0U; --index)
            {
                if (!std::isspace(static_cast<unsigned char>(m_data[index])))
                {
                    *this = substr(0U, index + 1U);
                    return;
                }
            }

            *this = (std::isspace(static_cast<unsigned char>(m_data[0U])))
                ? ""
                : substr(0U, 1U);
        }
    }

    // trim from both ends (in place)
    void trim()
    {
        ltrim();
        rtrim();
    }

    // TO DO: Change them by using, eg.
    // using vector<T>::begin();
    [[nodiscard]] constexpr reference operator[](size_type const index) noexcept
    {
        return m_data[index];
    }

    [[nodiscard]] constexpr const_reference operator[](
        size_type const index) const noexcept
    {
        return m_data[index];
    }

    [[nodiscard]] constexpr size_type capacity() const noexcept
    {
        return m_data.capacity();
    }
    [[nodiscard]] constexpr iterator begin() noexcept { return m_data.begin(); }
    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return m_data.begin();
    }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return m_data.cbegin();
    }
    [[nodiscard]] constexpr iterator end() noexcept
    {
        return m_data.begin() + size();
    }
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return m_data.begin() + size();
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return m_data.cbegin() + size();
    }

    [[nodiscard]] constexpr char_type const* c_str() const noexcept
    {
        return m_data.empty() ? "" : m_data.cbegin();
    }

    [[nodiscard]] constexpr bool empty() const noexcept { return size() == 0U; }

    [[nodiscard]] constexpr auto find_first_of(
        vector<char_type> const& chValue) const noexcept
    {
        auto const it{m_data.cfind_first_of(chValue)};
        return (it == m_data.cend()) ? basic_str::npos
                                    : std::distance(cbegin(), it);
    }

    [[nodiscard]] constexpr auto find_first_of(
        char_type const chValue) const noexcept
    {
        auto const it{m_data.find_first_of(chValue)};
        return (it == m_data.cend()) ? basic_str::npos
                                    : std::distance(cbegin(), it);
    }

    [[nodiscard]] constexpr auto find_first_of(
        basic_str const value) const noexcept
    {
        auto value_data_copy{value.m_data};
        value_data_copy.pop_back();
        auto const it{m_data.find_first_of(value_data_copy)};
        return (it == m_data.cend()) ? basic_str::npos
                                    : std::distance(cbegin(), it);
    }

    constexpr auto find_last_of(vector<char_type> const& chValue) const noexcept
    {
        auto const it{m_data.find_last_of(chValue)};
        return (it == m_data.cend()) ? basic_str::npos
                                    : std::distance(cbegin(), it);
    }

    [[nodiscard]] constexpr auto find_last_of(
        char_type const chValue) const noexcept
    {
        auto const it{m_data.find_last_of(chValue)};
        return (it == m_data.cend()) ? basic_str::npos
                                    : std::distance(cbegin(), it);
    }

    [[nodiscard]] constexpr auto find_last_of(
        basic_str const value) const noexcept
    {
        auto value_data_copy{value.m_data};
        value_data_copy.pop_back();
        auto const it{m_data.find_last_of(value_data_copy)};
        return (it == m_data.cend()) ? basic_str::npos
                                    : std::distance(cbegin(), it);
    }

    template <typename T>
    constexpr bool operator>>(T& n) const
    {
        if constexpr (std::is_enum_v<T>)
        {
            std::underlying_type_t<T> tmp{};
            const bool result{convert(tmp)};
            n = static_cast<T>(tmp);
            return result;
        }
        else if constexpr (std::is_same_v<T, basic_str>)
        {
            n = *this;
            return true;
        }
        else
        {
            return convert(n);
        }
    }

    static constexpr size_type npos{static_cast<size_type>(-1)};

    [[nodiscard]] constexpr bool operator==(
        basic_str const& rhs) const noexcept = default;

    [[nodiscard]] constexpr bool operator==(
        char_type const* const rhs) const noexcept
    {
        size_type counter{0U};
        for (; counter < size() && rhs[counter] != 0U; ++counter)
        {
            if (m_data[counter] != rhs[counter])
            {
                return false;
            }
        }

        return (counter == size() && rhs[counter] == 0);
    }

    [[nodiscard]] constexpr bool operator==(
        basic_str_view<char_type> const& rhs) const noexcept
    {
        return *this == rhs.begin_;
    }

    [[nodiscard]] constexpr auto operator<=>(
        const basic_str& rhs) const noexcept
    {
        for (size_type i{0U}; i < size() && i < rhs.size(); ++i)
        {
            if (m_data[i] < rhs[i])
            {
                return std::strong_ordering::less;
            }
            else if (m_data[i] > rhs[i])
            {
                return std::strong_ordering::greater;
            }
        }

        return (size() < rhs.size()       ? std::strong_ordering::less
                    : size() < rhs.size() ? std::strong_ordering::greater
                                          : std::strong_ordering::equal);
    }
};

export template <typename value_type, typename T>
constexpr basic_str<value_type>& operator<<(basic_str<value_type>& lhs,
                                            T const& n)
{
    return lhs.append(n);
}

export template <typename T, typename value_type>
auto operator+(T&& lhs, basic_str<value_type> const& rhs)
{
    return basic_str<value_type>(htps::forward<T>(lhs)).append(rhs);
}

export template <typename char_value, typename T, typename... Args>
constexpr void make_basic_str_internal(basic_str<char_value>& buffer,
                                       T&& arg,
                                       Args&&... args)
{
    make_basic_str_internal(buffer, htps::forward<T>(arg));
    make_basic_str_internal(buffer, htps::forward<Args>(args)...);
}

export template <typename char_value, typename T>
constexpr void make_basic_str_internal(basic_str<char_value>& buffer, T&& arg)
{
    buffer << htps::forward<T>(arg);
}

export template <typename char_value, typename... Args>
constexpr basic_str<char_value> make_basic_str(Args&&... args)
{
    basic_str<char_value> t;
    make_basic_str_internal(t, htps::forward<Args>(args)...);
    return t;
}

export template <typename char_value, typename T>
constexpr basic_str<char_value> make_basic_str(T&& arg)
{
    basic_str<char_value> t;
    make_basic_str_internal(t, htps::forward<T>(arg));
    return t;
}

export template <typename char_value>
inline basic_str<char_value> make_basic_str(u64&& n)
{
    return basic_str<char_value>::to_str(htps::move(n));
}

export template <typename char_value>
inline basic_str<char_value> make_basic_str(s64&& n)
{
    return basic_str<char_value>::to_str(htps::move(n));
}

export template <typename char_value>
inline basic_str<char_value> make_basic_str(u32&& n)
{
    return basic_str<char_value>::to_str(htps::move(n));
}

export template <typename char_value>
inline basic_str<char_value> make_basic_str(s32&& n)
{
    return basic_str<char_value>::to_str(htps::move(n));
}

export template <typename char_value>
inline basic_str<char_value> make_basic_str(f32&& n)
{
    return basic_str<char_value>::to_str(htps::move(n));
}

export template <typename char_value>
inline basic_str<char_value> make_basic_str(f64&& n)
{
    return basic_str<char_value>::to_str(htps::move(n));
}

}  // namespace htps

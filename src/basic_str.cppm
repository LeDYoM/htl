module;

#include <sstream>
#include <type_traits>
#include <algorithm>
#include <cctype>
#include <compare>

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
    vector<char_type> data_;

public:
    constexpr basic_str() noexcept = default;

    template <size_type N>
    constexpr basic_str(char_type const (&a)[N]) : data_(a, N)
    {}

    explicit constexpr basic_str(size_type const size) : data_(size) {}

    constexpr basic_str(const_iterator const n, size_type const N) :
        data_(n, N + 1)
    {}

    constexpr explicit basic_str(basic_str_view<char_type> other) :
        basic_str{other.m_begin, other.m_size}
    {}

    template<size_type N>
    constexpr explicit basic_str(basic_str_literal<char_type, N> other) :
        basic_str{other.value, N - 1U}
    {}

    constexpr explicit basic_str(char_type const* const n) :
        basic_str(n, strnlen(n))
    {}

    constexpr basic_str(const_iterator _begin, const_iterator _end) :
        basic_str(_begin, (_end - _begin) + 1U)
    {}

    constexpr basic_str(basic_str&&) noexcept = default;
    basic_str& operator=(basic_str&&) noexcept = default;

    constexpr basic_str(basic_str const& n) = default;
    constexpr basic_str& operator=(basic_str const& rhs) = default;

    basic_str& operator=(char_type const* const n)
    {
        auto const len{strnlen(n)};
        if (len < data_.capacity())
        {
            data_.resize(len + 1);
            for (decltype(strnlen(n)) i{0U}; i < len; ++i)
            {
                data_[i] = n[i];
            }
            data_[len] = static_cast<char_type>(0);
        }
        else
        {
            *this = basic_str(n, len);
        }
        return *this;
    }

    constexpr void reserve(size_type const capacity)
    {
        data_.reserve(capacity);
    }

    basic_str_view<char_type> to_view() const noexcept
    {
        return basic_str_view<char_type>{cbegin(), size()};
    }

    void swap(basic_str& other) { data_.swap(other.data_); }

    static basic_str fromCharAndSize(char_type const ch, size_type const size)
    {
        basic_str result(size + 1U);
        for (size_type i{0U}; i < size; ++i)
        {
            result.push_back(ch);
        }
        return result;
    }

    template <size_type b_size, typename T, size_type N>
    static int convert_to_imp_imp(char_type (&buffer)[b_size],
                                  T const n,
                                  char_type const (&fmt_str)[N])
    {
        return static_cast<int>(std::snprintf(buffer, b_size, fmt_str, n));
    }

    template <typename T, size_type N>
    static basic_str convert_to_imp(T const n, char_type const (&fmt_str)[N])
    {
        char buffer[32U];
        auto const size{convert_to_imp_imp(buffer, n, fmt_str)};
        return basic_str{buffer, static_cast<size_type>(size)};
    }

    static basic_str to_str(u64 const n) { return convert_to_imp(n, "%llu"); }

    static basic_str to_str(s64 const n) { return convert_to_imp(n, "%lld"); }

    static basic_str to_str(u32 const n) { return convert_to_imp(n, "%u"); }

    static basic_str to_str(s32 const n) { return convert_to_imp(n, "%d"); }

    static basic_str to_str(f32 const n) { return convert_to_imp(n, "%f"); }

    static basic_str to_str(f64 const n) { return convert_to_imp(n, "%F"); }

    template <typename T>
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
            if (data_[start] != 0)
            {
                temp.data_.push_back(data_[start]);
            }
            ++start;
            --len;
        }

        temp.data_.push_back(0);
        return temp;
    }

    constexpr basic_str& append() { return *this; }

    basic_str& append(basic_str const& n)
    {
        data_.pop_back();
        data_.insert(n.data_);
        return *this;
    }

    basic_str& append(basic_str&& n)
    {
        data_.pop_back();
        data_.insert(htps::move(n.data_));
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
        return data_.empty() ? 0 : (data_.size() - 1U);
    }

    void ltrim()
    {
        if (!empty())
        {
            for (size_type index{0U}; index < size(); ++index)
            {
                if (!std::isspace(static_cast<char_type>(data_[index])))
                {
                    *this = substr(index);
                    return;
                }
            }
            data_.clear();
        }
    }

    constexpr void clear() noexcept
    {
        if (!data_.empty())
        {
            data_[0] = static_cast<char_type>(0);
        }
        data_.clear();
    }

    constexpr size_type cfind(char_type const ch) const noexcept
    {
        const auto it(data_.cfind(ch));
        return ((it == data_.cend()) ? npos : std::distance(cbegin(), it));
    }

    constexpr size_type cfind(basic_str const& other) const noexcept
    {
        if (!other.empty() && !empty() && other.size() <= size())
        {
            auto next_start{cbegin()};
            while (next_start != cend())
            {
                auto const it_start{
                    data_.cfind(next_start, data_.cend(), *other.begin())};
                bool continue_inner{it_start != data_.cend()};
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
                    return std::distance(cbegin(), it_start);
                }
            }
        }
        return npos;
    }

    constexpr size_type find(char_type const ch) const noexcept
    {
        if (data_.empty())
        {
            return npos;
        }
        else
        {
            auto const it{data_.cfind(data_.cbegin(), &(data_.back()), ch)};
            return ((it == &data_.back()) ? npos : std::distance(cbegin(), it));
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
                if (!std::isspace(static_cast<unsigned char>(data_[index])))
                {
                    *this = substr(0U, index + 1U);
                    return;
                }
            }

            *this = (std::isspace(static_cast<unsigned char>(data_[0U])))
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
        return data_[index];
    }

    [[nodiscard]] constexpr const_reference operator[](
        size_type const index) const noexcept
    {
        return data_[index];
    }

    [[nodiscard]] constexpr size_type capacity() const noexcept
    {
        return data_.capacity();
    }
    [[nodiscard]] constexpr iterator begin() noexcept { return data_.begin(); }
    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return data_.begin();
    }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return data_.cbegin();
    }
    [[nodiscard]] constexpr iterator end() noexcept
    {
        return data_.begin() + size();
    }
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return data_.begin() + size();
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return data_.cbegin() + size();
    }

    [[nodiscard]] constexpr char_type const* c_str() const noexcept
    {
        return data_.empty() ? "" : data_.cbegin();
    }

    [[nodiscard]] constexpr bool empty() const noexcept { return size() == 0U; }

    [[nodiscard]] constexpr auto find_first_of(
        vector<char_type> const& chValue) const noexcept
    {
        auto const it{data_.cfind_first_of(chValue)};
        return (it == data_.cend()) ? basic_str::npos
                                    : std::distance(cbegin(), it);
    }

    [[nodiscard]] constexpr auto find_first_of(
        char_type const chValue) const noexcept
    {
        auto const it{data_.find_first_of(chValue)};
        return (it == data_.cend()) ? basic_str::npos
                                    : std::distance(cbegin(), it);
    }

    [[nodiscard]] constexpr auto find_first_of(
        basic_str const value) const noexcept
    {
        auto value_data_copy{value.data_};
        value_data_copy.pop_back();
        auto const it{data_.find_first_of(value_data_copy)};
        return (it == data_.cend()) ? basic_str::npos
                                    : std::distance(cbegin(), it);
    }

    constexpr auto find_last_of(vector<char_type> const& chValue) const noexcept
    {
        auto const it{data_.find_last_of(chValue)};
        return (it == data_.cend()) ? basic_str::npos
                                    : std::distance(cbegin(), it);
    }

    [[nodiscard]] constexpr auto find_last_of(
        char_type const chValue) const noexcept
    {
        auto const it{data_.find_last_of(chValue)};
        return (it == data_.cend()) ? basic_str::npos
                                    : std::distance(cbegin(), it);
    }

    [[nodiscard]] constexpr auto find_last_of(
        basic_str const value) const noexcept
    {
        auto value_data_copy{value.data_};
        value_data_copy.pop_back();
        auto const it{data_.find_last_of(value_data_copy)};
        return (it == data_.cend()) ? basic_str::npos
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

    [[nodiscard]] constexpr bool operator==(basic_str const& rhs) const noexcept
    {
        return data_ == rhs.data_;
    }

    [[nodiscard]] constexpr bool operator==(
        char_type const* const rhs) const noexcept
    {
        size_type counter{0U};
        for (; counter < size() && rhs[counter] != 0U; ++counter)
        {
            if (data_[counter] != rhs[counter])
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
            if (data_[i] < rhs[i])
            {
                return std::strong_ordering::less;
            }
            else if (data_[i] > rhs[i])
            {
                return std::strong_ordering::greater;
            }
        }

        return (size() < rhs.size()
                    ? std::strong_ordering::less
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


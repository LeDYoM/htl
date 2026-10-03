module;

#include <cstring>

export module htypes:raw_memory;

import :btypes;
import :str;

#pragma warning(push)
#pragma warning(disable : 4514)  // Unused functions in MSVC

namespace htps
{
/**
 * @brief Class wrapping a unique pointer to an array of bytes.
 * This class wraps and owns an array of bytes. Instances are
 * copyable and cheaply movable. A size is maintained.
 */
export class RawMemory
{
public:
    using inner_type = char;

    /// Default constructor.
    constexpr RawMemory() noexcept = default;

    explicit RawMemory(vector<inner_type>&& data) noexcept :
        m_data{htps::move(data)}
    {}

    RawMemory(inner_type const* data, size_type const size) noexcept :
        m_data{data, size}
    {}

    RawMemory(htps::str const& data) :
        RawMemory{static_cast<inner_type const*>(data.c_str()),
                  data.size() + 1U}
    {}

    RawMemory(htps::str_view const& data) :
        RawMemory{data.begin(), data.size() + 1U}
    {
        m_data[data.size()] = static_cast<inner_type>(0);
    }

    RawMemory(RawMemory const& rhs)            = default;
    RawMemory& operator=(RawMemory const& rhs) = default;
    RawMemory(RawMemory&&) noexcept            = default;
    RawMemory& operator=(RawMemory&&) noexcept = default;

    inner_type const* cbegin() const noexcept { return m_data.cbegin(); }
    inner_type const* cend() const noexcept { return m_data.cend(); }

    size_type size() const noexcept { return m_data.size(); }

    bool empty() const noexcept { return m_data.empty(); }

    inner_type const* data() const noexcept { return m_data.cbegin(); }

    str to_str() const
    {
        if (!empty())
        {
            str result{reinterpret_cast<str::const_iterator>(cbegin()),
                       reinterpret_cast<str::const_iterator>(cend() - 1)};
            result.push_back(0);
            return result;
        }
        return str{};
    }

    str_view to_str_view() const
    {
        if (!empty())
        {
            str_view result(
                reinterpret_cast<str_view::const_iterator>(cbegin()),
                reinterpret_cast<str_view::const_iterator>(cend() - 1));
        }
        return str_view{};
    }

private:
    vector<inner_type> m_data;
};

}  // namespace htps

#pragma warning(pop)

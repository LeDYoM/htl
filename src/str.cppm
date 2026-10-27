export module htypes:str;

import :bstr;

namespace htps
{
export using str = basic_str<char>;

export template <typename... Args>
constexpr str make_str(Args&&... args)
{
    return make_basic_str<typename str::value_type>(
        forward<Args>(args)...);
}

export using string_vector = vector<str>;

}  // namespace htps

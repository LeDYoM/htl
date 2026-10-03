module;

#include <type_traits>

export module htypes:str;

import :bstr;

namespace htps
{
export using str = basic_str<char>;

export template <typename... Args>
constexpr str make_str(Args&&... args)
{
    return make_basic_str<typename str::value_type>(
        htps::forward<Args>(args)...);
}

static_assert(std::is_move_constructible_v<str>, "str must be movable");
static_assert(std::is_move_assignable_v<str>, "str must be movable assignable");
static_assert(std::is_default_constructible_v<str>,
              "str must be default constructible");
static_assert(std::is_copy_constructible_v<str>,
              "str must be copy constructible");
export using string_vector = vector<str>;

}  // namespace htps

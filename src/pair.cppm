module;

#include <compare>

export module htypes:pair;

namespace htps
{
// template <typename T, typename V>
// using pair = std::pair<T, V>;

export template <typename T1, typename T2>
class pair
{
public:
    using first_type  = T1;
    using second_type = T2;

    T1 first;
    T2 second;

    [[nodiscard]] constexpr bool operator==(const pair& rhs) const noexcept =
        default;

    [[nodiscard]] constexpr auto operator<=>(const pair& rhs) const noexcept
    {
        if (first < rhs.first)
        {
            return std::strong_ordering::less;
        }
        else if (first > rhs.first)
        {
            return std::strong_ordering::greater;
        }
        else
        {
            // first and rhs.first compared equal
            if (second < rhs.second)
            {
                return std::strong_ordering::less;
            }
            else if (second > rhs.second)
            {
                return std::strong_ordering::greater;
            }
        }
        return std::strong_ordering::equal;
    }
};
}  // namespace htps

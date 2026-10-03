module;

#include <type_traits>
#include <limits>

export module htypes:menum;

namespace htps
{

export template <typename T, T ValueMin = T::min, T ValueMax = T::max>
class MEnum
{
public:
    static_assert(std::is_enum_v<T>, "MEnum only works with enums");
    // requires T::min and T::max to be enum values.
    static constexpr auto min{ValueMin};
    static constexpr auto max{ValueMax};

    using numeric_type = std::underlying_type_t<T>;
    static constexpr auto min_numeric{static_cast<numeric_type>(min)};
    static constexpr auto max_numeric{static_cast<numeric_type>(max)};

    static constexpr auto size{max_numeric - min_numeric};

    static constexpr auto min_str{to_str(min)};
    static constexpr auto max_str{to_str(max)};

    constexpr auto toStr()
    {
        // Requires to_str(T) overload
        return to_str(value);
    }

    constexpr MEnum() noexcept = default;
    constexpr explicit MEnum(T val) noexcept : value{val} {}

    constexpr auto begin() noexcept { return MEnum{min}; }
    constexpr auto begin() const noexcept { return MEnum{min}; }
    constexpr auto cbegin() const noexcept { return begin(); }

    constexpr auto end() noexcept { return ++MEnum{max}; }
    constexpr auto end() const noexcept { return ++MEnum{max}; }
    constexpr auto cend() const noexcept { return end(); }

    constexpr auto operator==(const MEnum& rhs) const noexcept
    {
        return value == rhs.value;
    }

    constexpr auto getNumeric() const noexcept
    {
        return static_cast<std::underlying_type_t<T>>(value);
    }

    constexpr T getValue() const noexcept { return value; }

    constexpr MEnum& operator++() noexcept
    {
        value =
            static_cast<T>(static_cast<std::underlying_type_t<T>>(value) + 1);
        return *this;
    }

    constexpr MEnum& operator--() noexcept
    {
        value =
            static_cast<T>(static_cast<std::underlying_type_t<T>>(value) - 1);
        return *this;
    }

    constexpr bool isValid() const noexcept
    {
        return (getNumeric() >= min_numeric) && (getNumeric() <= max_numeric);
    }

    constexpr bool nextIsValid() const noexcept
    {
        MEnum temp{value};
        return (++temp).isValid();
    }

    constexpr bool previousIsValid() const noexcept
    {
        MEnum temp{value};
        return (--temp).isValid();
    }

private:
    T value;
};

}  // namespace htps


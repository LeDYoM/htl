module;

#include <cstdint>
#include <memory>
#include <map>
#include <type_traits>

export module htypes:btypes;

namespace htps
{
export template <class _Ty>
[[nodiscard]] constexpr _Ty&& forward(
    std::remove_reference_t<_Ty>& _Arg) noexcept
{  // forward an lvalue as either an lvalue or an rvalue
    return static_cast<_Ty&&>(_Arg);
}

export template <class _Ty>
[[nodiscard]] constexpr _Ty&& forward(
    std::remove_reference_t<_Ty>&& _Arg) noexcept
{  // forward an rvalue as an rvalue
    static_assert(!std::is_lvalue_reference_v<_Ty>, "bad forward call");
    return static_cast<_Ty&&>(_Arg);
}

export template <class _Ty>
[[nodiscard]] constexpr std::remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept
{  // forward _Arg as movable
    return static_cast<std::remove_reference_t<_Ty>&&>(_Arg);
}

export template <typename T>
using rptr = std::add_pointer_t<T>;

export using u64        = uint64_t;
export using s64        = int64_t;
export using u32        = uint32_t;
export using s32        = int32_t;
export using u16        = uint16_t;
export using s16        = int16_t;
export using u8         = uint8_t;
export using s8         = int8_t;
export using f32        = float;
export using f64        = double;
export using size_type  = std::size_t;
export using ssize_type = std::ptrdiff_t;
export using fast_u64   = uint_fast64_t;
export using fast_u32   = uint_fast32_t;
export using fast_u16   = uint_fast16_t;
export using fast_u8    = uint_fast8_t;
export using fast_s64   = int_fast64_t;
export using fast_s32   = int_fast32_t;
export using fast_s16   = int_fast16_t;
export using fast_s8    = int_fast8_t;

export template <typename T, typename V>
using map = std::map<T, V>;

}  // namespace htps

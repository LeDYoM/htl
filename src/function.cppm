module;

#include <cassert>

export module htypes:function;

import :shared_ptr;

namespace htps
{
/**
 * @brief Unused general template
 * @tparam typename
 */
export template <typename>
class function;

/**
 * @brief Wrapper class to store a pointer to a function. It can store any
 * pointer to function, lambda or pointer to member with object
 *
 * @tparam ReturnValue Return type of the stored function
 * @tparam Args Arguments type for the stored function
 */
template <typename ReturnValue, typename... Args>
class function<ReturnValue(Args...)> final
{
public:
    /**
     * @brief Default constructor.
     * Construct an empty object
     */
    constexpr function() noexcept : callable_{nullptr} {}

    constexpr function(std::nullptr_t) noexcept : function{} {}

    /**
     * @brief Construct from a callable object. Normally, a lambda or a pointer
     * to function.
     *
     * @tparam T Type of the object passed
     * @param t Object to construct from
     */
    template <typename T>
    constexpr function(T t) : callable_{msptr<CallableT<T>>(htps::move(t))}
    {}

    /**
     * @brief Construct from a pointer to member and object.
     *
     * @tparam T Type of the object
     * @param t Object of the type t
     * @param f Pointer to the member function
     */
    template <typename T>
    constexpr function(T* const t, ReturnValue (T::*f)(Args...)) :
        callable_{msptr<CallableMethodPointerT<T>>(t, f)}
    {}

    template <typename T>
    constexpr function(T const* const t, ReturnValue (T::*f)(Args...) const) :
        callable_{msptr<ConstCallableMethodPointerT<T>>(t, f)}
    {}

    /**
     * @brief Implicit conversion to bool
     *
     * @return true The object contains data.
     * @return false The object is empty
     */
    constexpr operator bool() const noexcept { return callable_ != nullptr; }

    /**
     * @brief Operator invoke to perform the actual call.
     *
     * @tparam Args2 Types of the actual call. They should be forwardable
     * to the function call.
     * @param args Arguments to be forwarded to the function.
     * @return ReturnValue Return value of the function.
     */
    template <typename... Args2>
    constexpr ReturnValue operator()(Args2&&... args) const
    {
        assert(callable_);
        return callable_->Invoke(htps::forward<Args2>(args)...);
    }

    /**
     * @brief Equality comparison operator. Compares if two objects points
     * to the same internal object.
     * @note Returns false if they are not copy constructed or copy assigned.
     *
     * @param other Other object to compare to.
     * @return true The two objects point to the same internal object.
     * @return false The two object point to different internal objects.
     */
    constexpr bool operator==(const function& other) const noexcept = default;

    /**
     * @brief Compare two objects if they point to the same callable object.
     * @note Returns true in case of pointers to members to the same object
     * and method.
     *
     * @param other Other object to compare to.
     * @return true If the two objects are equal.
     * @return false If the two objects are different.
     */
    constexpr bool equals(function const& other) const noexcept
    {
        return callable_->equals(*(other.callable_));
    }

private:
    class ICallable
    {
    public:
        virtual ~ICallable() {}
        virtual ReturnValue Invoke(Args...)                  = 0;
        virtual bool equals(ICallable const&) const noexcept = 0;
    };

    template <typename T>
    class CallableT final : public ICallable
    {
    public:
        template <typename Y>
        constexpr CallableT(Y&& t) noexcept : t_{htps::forward<Y>(t)}
        {}

        constexpr ReturnValue Invoke(Args... args) override
        {
            return t_(htps::forward<Args>(args)...);
        }

        constexpr bool equals(CallableT const& other) const noexcept
        {
            return &t_ == &other.t_;
        }

        constexpr bool equals(ICallable const& other) const noexcept override
        {
            CallableT const* const tmp{dynamic_cast<CallableT const*>(&other)};
            return tmp ? equals(*tmp) : false;
        }

    private:
        T t_;
    };

    template <class T>
    class CallableMethodPointerT final : public ICallable
    {
    public:
        using HandlerFunctionPtr = ReturnValue (T::*)(Args...);

        constexpr CallableMethodPointerT(
            T* const receiver,
            const HandlerFunctionPtr function) noexcept :
            obj{receiver}, function_{function}
        {}

        constexpr ReturnValue Invoke(Args... args) override
        {
            return (obj->*function_)(htps::forward<Args>(args)...);
        }

        constexpr bool equals(
            CallableMethodPointerT const& other) const noexcept
        {
            return obj == other.obj && function_ == other.function_;
        }

        constexpr bool equals(ICallable const& other) const noexcept override
        {
            function::CallableMethodPointerT<T> const* const tmp{
                dynamic_cast<CallableMethodPointerT<T> const*>(&other)};
            return tmp ? equals(*tmp) : false;
        }

    private:
        T* obj;
        HandlerFunctionPtr function_;
    };

    template <class T>
    class ConstCallableMethodPointerT final : public ICallable
    {
    public:
        using HandlerFunctionPtr = ReturnValue (T::*)(Args...) const;

        constexpr ConstCallableMethodPointerT(
            T const* const receiver,
            const HandlerFunctionPtr function) :
            obj{receiver}, function_{function}
        {}

        constexpr ReturnValue Invoke(Args... args) override
        {
            return (obj->*function_)(htps::forward<Args>(args)...);
        }

        constexpr bool equals(
            ConstCallableMethodPointerT const& other) const noexcept
        {
            return obj == other.obj && function_ == other.function_;
        }

        constexpr bool equals(ICallable const& other) const noexcept override
        {
            function::ConstCallableMethodPointerT<T> const* const tmp{
                dynamic_cast<ConstCallableMethodPointerT<T> const*>(&other)};
            return tmp ? equals(*tmp) : false;
        }

    private:
        T const* obj;
        HandlerFunctionPtr function_;
    };

    sptr<ICallable> callable_;
};

/**
 * @brief Helper function to create a @b function object deducing the type
 * automatically. The template parameters will be autodeduced.
 *
 * @tparam ReturnType Return type of the function
 * @tparam Args Parameter types of the arguments
 * @param p Callable object
 * @return function with the same return type and arguments that the object
 * passed.
 */
export template <typename ReturnType, typename... Args>
constexpr auto make_function(ReturnType (*p)(Args...))
{
    return function<ReturnType(Args...)>(p);
}

/**
 * @brief Helper function to create a @b function object deducing the type
 * automatically. Overload for objects and member pointers.
 *
 * @tparam ReturnType Return type of the function
 * @tparam T Type of the class with the member function
 * @tparam Args Parameter types of the arguments
 * @param obj Objcect of type T
 * @param p Member pointer
 * @return function with the same return type and arguments that the object
 * and member function passed.
 */
export template <typename ReturnType, typename T, typename... Args>
constexpr auto make_function(T* obj, ReturnType (T::*p)(Args...))
{
    return function<ReturnType(Args...)>(obj, p);
}

export template <typename ReturnType, typename T, typename... Args>
constexpr auto make_function(T const* obj, ReturnType (T::*p)(Args...) const)
{
    return function<ReturnType(Args...)>(obj, p);
}

}  // namespace htps

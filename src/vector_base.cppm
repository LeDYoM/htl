module;

#include <initializer_list>
#include <utility>
#include <iterator>
#include <cassert>

export module htypes:vector_base;

import :function;
import :growpolicy;
import :vector_storage;
import :allocator;

namespace htps
{
/**
 * @brief Vector class to store a sequence of elements.
 * @details This class is a container to store sequences of Ts. It can be
 * resized. Other use cases include search, replacement, etc...
 *
 * @tparam T Type of the contained element.
 * @tparam Allocator Allocator to be used by the vector
 * @tparam GrowPolicy Policy class to dinamically increment the capacity
 */
export template <typename T, typename Allocator, typename GrowPolicy>
class vector_base final
{
    vector_storage<T, Allocator, GrowPolicy> storage_;

public:
    using iterator        = T*;        //< Iterator for the values
    using const_iterator  = T const*;  //< Iterator pointing to const values
    using reference       = T&;        //< Reference to member
    using rv_reference    = T&&;       //< r-value reference to member
    using const_reference = const T&;  //< Const reference to members
    using value_type      = T;   //< Value type of the contained data member
    using pointer         = T*;  //< Pointer to the contained data type
    using const_pointer =
        T const*;  //< Pointer to const to the contained data type
    using size_type = htps::size_type;

    /**
     * @brief Default constructor.
     * Sets all members to 0, nullptr or empty.
     */
    constexpr vector_base() noexcept = default;

    /**
     * @brief Construct a new vector object empty with reserved memory.
     * The memory for the vector is allocated, but no construction
     * of the elements is performed.
     *
     * @param size Expected size of the inner container
     */
    explicit constexpr vector_base(size_type const size) : storage_{size} {}

    /**
     * @brief Constructor with reserved memory and copy from source.
     * This constructor takes a pointer and a number of elements
     * and constructs a vector from it.
     *
     * @param source Pointer to the first element of the
     * @param count Number of elements to copy.
     */
    constexpr vector_base(const_iterator const source, size_type const count) :
        storage_{count}
    {
        auto iterator{source};
        auto const end{source + count};
        while (iterator != end)
        {
            // Construct by copy.
            push_back((*iterator++));
        }
    }

    /**
     * @brief Construct a vector_base from an initializer list of elements
     * @param iList Initializer list of elements
     */
    constexpr vector_base(std::initializer_list<value_type> ilist) :
        vector_base(ilist.begin(), ilist.size())
    {}

    /**
     * @brief Construct a vector_base from two iterators
     * @param _begin Iterator pointing to the first element
     * @param _end Iterator pointing one past the end of the range of elements
     */
    constexpr vector_base(const_iterator const _begin,
                          const_iterator const _end) :
        vector_base{_begin, static_cast<size_type>(std::distance(_begin, _end))}
    {}

    /**
     * @brief Copy constructor.
     * This constructor constructs a vector from another one.
     * The capacity of the resultant vector might be different
     * from the capacity of the source. The size will be the same.
     *
     * @param other Source vector to copy.
     */
    constexpr vector_base(vector_base const& other) :
        vector_base{other.begin(), other.begin() + other.size()}
    {}

    /**
     * @brief Move constructor
     * @param other Object to move from
     */
    constexpr vector_base(vector_base&& other) noexcept :
        storage_{htps::move(other.storage_)}
    {}

    /**
     * @brief Copy assignment
     *
     * @param other Object to copy from
     * @return Reference to the newly assigned object
     */
    constexpr vector_base& operator=(vector_base const& other)
    {
        if (this != &other)
        {
            clear();
            insert(other);
        }
        return *this;
    }

    /// Move assignment
    constexpr vector_base& operator=(vector_base&& other) noexcept
    {
        storage_.swap(other.storage_);
        return *this;
    }

    /// Destructor.
    inline ~vector_base() noexcept { clear(); }

    [[nodiscard]] constexpr reference operator[](const size_type index)
    {
        assert(index < size());
        return *(storage_.at(index));
    }

    [[nodiscard]] constexpr const_reference operator[](
        const size_type index) const
    {
        assert(index < size());
        return *(storage_.at(index));
    }

    [[nodiscard]] constexpr size_type capacity() const noexcept
    {
        return storage_.capacity();
    }

    [[nodiscard]] constexpr size_type size() const noexcept
    {
        return storage_.size();
    }
    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return storage_.empty();
    }
    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return storage_.begin();
    }
    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return storage_.begin();
    }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return storage_.cbegin();
    }
    [[nodiscard]] constexpr iterator end() noexcept { return storage_.end(); }
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return storage_.end();
    }
    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return storage_.cend();
    }

    [[nodiscard]] constexpr T& back() noexcept
    {
        assert(size() > 0U);
        return *(storage_.at(size() - 1U));
    }

    constexpr T const& back() const noexcept
    {
        assert(size() > 0U);
        return *(storage_.at(size() - 1U));
    }

    constexpr T const& cback() const noexcept
    {
        assert(size() > 0U);
        return *(storage_.cat(size() - 1U));
    }

    constexpr T& front() noexcept
    {
        assert(!empty());
        return *(storage_.at(0U));
    }

    constexpr T const& front() const noexcept
    {
        assert(!empty());
        return *(storage_.at(0U));
    }

    constexpr T const& cfront() const noexcept
    {
        assert(!empty());
        return *(storage_.at(0U));
    }

    template <typename F>
    constexpr void for_each(F f)
    {
        if (!empty())
        {
            auto current{begin()};
            do
            {
                f(*current);
            } while (++current != end());
        }
    }

    template <typename F>
    constexpr void for_each_backwards(F f) const
    {
        if (!empty())
        {
            auto current{end()};
            while (current-- != begin())
            {
                f(*current);
            }
        }
    }

    template <typename F>
    constexpr void cfor_each(F f) const
    {
        if (!empty())
        {
            auto current{cbegin()};
            do
            {
                f(*current);
            } while (++current != cend());
        }
    }

    template <typename F>
    constexpr void cfor_each_backwards(F f)
    {
        if (!empty())
        {
            auto current{cend()};
            while (--current != cbegin())
            {
                f(*current);
            }
        }
    }

    constexpr void swap(vector_base& other) noexcept
    {
        storage_.swap(other.storage_);
    }

    template <bool discard_order = true>
    constexpr iterator erase_values(const T& value, iterator start)
    {
        iterator result{start};
        checkRange(result);

        do
        {
            result = start;
            start  = erase_one<discard_order>(value, start);
        } while (start != end());

        return result;
    }

    template <bool discard_order = true>
    constexpr iterator erase_values(const T& value)
    {
        return erase_values<discard_order>(value, begin());
    }

    template <typename U, bool discard_order = true>
    constexpr iterator erase_one_imp(U&& v, iterator const start) noexcept
    {
        checkRange(start);

        if (cbegin() != cend())
        {
            // Find a node with the specified value
            iterator where_it_was{find(start, end(), htps::forward<U>(v))};

            // If such a node is found erase it, if not,
            // return end() (result from find(...)).
            if (where_it_was != end())
            {
                // If the element to delete is not the last one
                if (where_it_was < end() - 1U)
                {
                    remove_iterator<discard_order>(where_it_was, end());
                }
                pop_back();
            }
            return where_it_was;
        }
        return end();
    }

    template <bool discard_order = true>
    constexpr iterator erase_iterator(iterator const where,
                                      iterator const _end) noexcept
    {
        // If such a node is found erase it, if not,
        // return end() (result from find(...)).
        if (where != _end)
        {
            // If the element to delete is not the last one
            if (std::distance(where, _end) > 0)
            {
                remove_element<discard_order>(where, _end);
            }
            pop_back();
        }
        return where;
    }

    using Predicate_t = function<bool(const T&)>;

    template <bool discard_order = true>
    constexpr iterator erase_if(Predicate_t condition, iterator start)
    {
        checkRange(start);

        if (cbegin() != cend())
        {
            // Find a node with the specified value
            return erase_iterator<discard_order>(
                find_if(start, end(), htps::move(condition)), end());
        }
        return end();
    }

    /**
     * @brief Erase one element (the first one containing a specified)
     * value.
     *
     * @param value Value to search for in the vector.
     * @param start start iterator pointing to the first element
     * to look for.
     * @param discard_order If true, relative order after deleting will not
     * be taken into accound. If false, the order will remain the same.
     * Note that using true will keep iterators pointing to other elements,
     * except the deleted one and to the last one valid. Using false, only
     * the iterators to the one previous the one deleted will still be valid.
     * @return iterator Pointing to the element in the position where the
     * deleted element was. If the element was the last one or no element
     * with this value found, the iterator will be end().
     * @warning This method does not allocate or deallocate memory. And it
     * des not preserve the order. Actually, the deleted element will now
     * contain the previous last element.
     */
    template <bool discard_order = true>
    constexpr iterator erase_one(const T& value, iterator const start) noexcept
    {
        checkRange(start);

        if (begin() != end())
        {
            // Find a node with the specified value and erase it
            return erase_iterator<discard_order>(find(start, end(), value),
                                                 end());
        }
        return end();
    }

    template <bool discard_order = true>
    constexpr void remove_element(iterator const element,
                                  iterator const end) noexcept
    {
        if constexpr (discard_order)
        {
            // swap the element to delete with the last one
            std::swap(*element, *std::prev(end));
        }
        else
        {
            for (iterator it{element}; it != std::prev(end); ++it)
            {
                std::swap(*it, *(std::next(it)));
            }
        }
    }

    template <bool discard_order = true>
    constexpr iterator erase_one_index(size_type const index) noexcept
    {
        if (index < size())
        {
            return erase_one<discard_order>(*(begin() + index),
                                            (begin() + index));
        }

        return end();
    }

    template <bool discard_order = true>
    constexpr iterator erase_one(const T& value)
    {
        return erase_one<discard_order>(value, begin());
    }

    template <bool discard_order = true>
    constexpr iterator erase_if(Predicate_t condition)
    {
        return erase_if<discard_order>(htps::move(condition), begin());
    }

    template <bool discard_order = true>
    constexpr iterator erase_all_if(Predicate_t condition, iterator start)
    {
        iterator result{start};
        do
        {
            result = start;
            start  = erase_if<discard_order>(condition, start);
        } while (start != end());

        return result;
    }

    template <bool discard_order = true>
    constexpr iterator erase_all_if(Predicate_t condition)
    {
        return erase_all_if<discard_order>(htps::move(condition), begin());
    }

    [[nodiscard]] constexpr const_iterator find_first_of(
        T const& other) const noexcept
    {
        return cfind(other);
    }

    [[nodiscard]] constexpr const_iterator find_first_of(
        vector_base const& other) const noexcept
    {
        auto result{cend()};
        for (auto&& it : other)
        {
            if (auto const inner_it{cfind(it)}; inner_it != cend())
            {
                if (inner_it < result)
                {
                    result = inner_it;
                }
            }
        }
        return result;
    }

    [[nodiscard]] constexpr const_iterator find_last_of(
        T const& other) const noexcept
    {
        return cfind_backwards(other);
    }

    [[nodiscard]] constexpr const_iterator find_last_of(
        vector_base const& other) const noexcept
    {
        auto result{cbegin() - 1};
        for (auto&& it : other)
        {
            if (auto const inner_it{cfind_backwards(it)}; inner_it != cend())
            {
                if (inner_it > result)
                {
                    result = inner_it;
                }
            }
        }
        return ((result == cbegin() - 1) ? cend() : result);
    }

    [[nodiscard]] constexpr iterator find(iterator begin,
                                          const iterator end,
                                          const T& element) const noexcept
    {
        checkRange(begin);
        checkRange(end);

        for (; (begin != end && !(*begin == element)); ++begin)
            ;
        return begin;
    }

    template <typename F>
    [[nodiscard]] constexpr const_iterator cfind_if(const_iterator begin,
                                                    const_iterator const end,
                                                    F&& f) const noexcept
    {
        checkRange(begin);
        checkRange(end);

        for (; (begin != end && !(htps::forward<F>(f)(*begin))); ++begin)
            ;
        return begin;
    }

    template <typename F>
    [[nodiscard]] constexpr const_iterator cfind_if(F&& f) const noexcept
    {
        return cfind_if(cbegin(), cend(), htps::forward<F>(f));
    }

    template <typename F>
    [[nodiscard]] constexpr iterator find_if(iterator begin,
                                             const const_iterator end,
                                             F&& f) noexcept
    {
        checkRange(begin);
        checkRange(end);

        for (; (begin != end && !(htps::forward<F>(f)(*begin))); ++begin)
            ;
        return begin;
    }

    template <typename F>
    [[nodiscard]] constexpr ssize_type cfind_index_if(F&& f) const noexcept
    {
        auto begin_{cbegin()};
        ssize_type counter{0};

        for (; (begin_ != cend() && !(htps::forward<F>(f)(*begin_)));
             ++begin_, ++counter)
            ;
        return ((begin_ == cend()) ? -1 : counter);
    }

    template <typename F>
    [[nodiscard]] constexpr ssize_type find_index_if(F&& f) const noexcept
    {
        return cfind_index_if(htps::forward<F>(f));
    }

    [[nodiscard]] constexpr ssize_type cfind_index(
        T const& other) const noexcept
    {
        auto begin_{cbegin()};
        ssize_type counter{-1};

        for (; (begin_ != cend() && !(*begin_ == other)); ++begin_, ++counter)
            ;
        return ((begin_ == cend()) ? -1 : counter);
    }

    [[nodiscard]] constexpr ssize_type find_index(T const& other) const noexcept
    {
        return cfind_index(other);
    }

    template <typename F>
    [[nodiscard]] constexpr iterator find_if(F&& f) noexcept
    {
        return find_if(begin(), cend(), htps::forward<F>(f));
    }

    template <typename F>
    [[nodiscard]] constexpr const_iterator find_if(const const_iterator begin,
                                                   const const_iterator end,
                                                   F&& f) const noexcept
    {
        return cfind_if(begin, end, htps::forward<F>(f));
    }

    template <typename F>
    [[nodiscard]] constexpr const_iterator find_if(F&& f) const noexcept
    {
        return cfind_if(cbegin(), cend(), htps::forward<F>(f));
    }

    [[nodiscard]] constexpr const_iterator cfind(
        const_iterator begin,
        const_iterator const end,
        T const& element) const noexcept
    {
        checkRange(begin);
        checkRange(end);

        for (; (begin != end && !(*begin == element)); ++begin)
            ;
        return begin;
    }

    [[nodiscard]] constexpr iterator find(iterator begin,
                                          iterator const end,
                                          T const& element) noexcept
    {
        checkRange(begin);
        checkRange(end);

        for (; (begin != end && !(*begin == element)); ++begin)
            ;
        return begin;
    }

    [[nodiscard]] constexpr iterator find(T const& element) noexcept
    {
        return find(begin(), end(), element);
    }

    [[nodiscard]] constexpr const_iterator cfind(
        T const& element) const noexcept
    {
        return cfind(cbegin(), cend(), element);
    }

    [[nodiscard]] constexpr const_iterator find(T const& element) const noexcept
    {
        return cfind(element);
    }

    [[nodiscard]] constexpr const_iterator cfind_backwards(
        const_iterator const begin,
        const_iterator end,
        T const& element) const noexcept
    {
        checkRange(begin);
        --end;
        checkRange(end);

        for (; (end >= begin && !(*end == element)); --end)
            ;
        return (end == begin - 1) ? cend() : end;
    }

    [[nodiscard]] constexpr iterator find_backwards(iterator const begin,
                                                    iterator end,
                                                    T const& element) noexcept
    {
        checkRange(begin);
        --end;
        checkRange(end);

        for (; (end >= begin && !(*end == element)); --end)
            ;
        return (end == begin - 1) ? this->end() : end;
    }

    [[nodiscard]] constexpr iterator find_backwards(
        iterator const begin,
        iterator end,
        T const& element) const noexcept
    {
        return cfind_backwards(begin, end, element);
    }

    [[nodiscard]] constexpr iterator find_backwards(T const& element) noexcept
    {
        return find_backwards(begin(), end(), element);
    }

    [[nodiscard]] constexpr const_iterator cfind_backwards(
        T const& element) const noexcept
    {
        return cfind_backwards(cbegin(), cend(), element);
    }

    [[nodiscard]] constexpr const_iterator find_backwards(
        T const& element) const noexcept
    {
        return cfind_backwards(element);
    }

    constexpr void shrink_to_fit() { storage_.shrink_to_fit(); }

    constexpr void push_back(const T& value) { storage_.push_back(value); }

    constexpr void push_back(rv_reference value)
    {
        storage_.push_back(htps::move(value));
    }

    template <typename... Args>
    constexpr void emplace_back(Args&&... args)
    {
        storage_.emplace_back(htps::forward<Args>(args)...);
    }

    constexpr void insert(iterator const where, const T& element)
    {
        checkRange(where);
        storage_.insert(where, element);
    }

    constexpr void insert(vector_base const& other)
    {
        if (!other.empty())
        {
            reserve(size() + other.size());
            for (auto&& element : other)
            {
                push_back(htps::forward<decltype(element)>(element));
            }
        }
    }

    constexpr void insert(vector_base&& other)
    {
        if (!other.empty())
        {
            reserve(size() + other.size());
            for (auto&& element : htps::move(other))
            {
                emplace_back(htps::move(element));
            }
        }
    }

    constexpr vector_base& operator+=(vector_base const& other)
    {
        insert(other);
        return *this;
    }

    constexpr vector_base& operator+=(vector_base&& other)
    {
        insert(htps::move(other));
        return *this;
    }

    [[nodiscard]] constexpr bool operator==(
        vector_base const& rhs) const noexcept
    {
        // If the vectors have different sizes, they are different
        if (size() != rhs.size())
        {
            return false;
        }
        else
        {
            for (auto lhs_iterator{cbegin()}, rhs_iterator{rhs.cbegin()};
                 lhs_iterator != cend(); ++lhs_iterator, ++rhs_iterator)
            {
                if (!(*lhs_iterator == *rhs_iterator))
                {
                    return false;
                }
            }
            return true;
        }
    }

    constexpr void pop_back() noexcept { storage_.pop_back(); }

    constexpr void reserve(size_type const capacity)
    {
        storage_.reserve(capacity);
    }

    /**
     * @brief Resize the inner container of this vector
     * @note This function affects the size of the vector it might
     * leave the inner reserved memory untouched.
     * @note If the size of the vector is equal to the parameter, this
     * function will do nothing..
     *
     * @param new_size New expected size of the vector.
     */
    constexpr void resize(size_type const new_size)
    {
        // Delete to shrink
        while (size() > new_size)
        {
            pop_back();
        }

        if (size() < new_size)
        {
            reserve(new_size);
            // Append the necessary default constructed elements
            while (size() < new_size)
            {
                emplace_back();
            }
        }
    }

    /**
     * @brief Set an element at a given index.
     * If the vector is smaller than the index, it will be grown to fit
     * @param index Index element where to put the element
     * @param element r-value reference to the element to be added
     */
    constexpr void set_at_index(size_type const index, rv_reference element)
    {
        if (index >= size())
        {
            resize(index + 1U);
        }
        (*this)[index] = htps::move(element);
    }

    /**
     * @brief Set an element at a given index.
     * If the vector is smaller than the index, it will be grown to fit
     * @param index Index element where to put the element
     * @param element const l-value reference to the element to be added
     */
    constexpr void set_at_index(size_type const index, const T& element)
    {
        if (index >= size())
        {
            resize(index + 1U);
        }
        (*this)[index] = element;
    }

    /**
     * @brief Delete all elements in the vector. but maintains the inner
     * reserved memory unmodified.
     */
    constexpr void clear() noexcept
    {
        while (size() > 0U)
        {
            pop_back();
        }
    }

    /**
     * @brief Check that this iterator is valid. This function is for debug
     * purposes
     * @param it Const iterator to check
     */
    constexpr void checkRange([[maybe_unused]] const_iterator const it) const
    {
        assert(it >= begin());
        assert(it <= end());
    }
};

}  // namespace htps

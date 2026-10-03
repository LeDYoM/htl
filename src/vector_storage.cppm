module;

#include <utility>
#include <iterator>

export module htypes:vector_storage;

import :allocator;
import :function;
import :growpolicy;

namespace htps
{
/** Vector class to store a sequence of elements
 * This class is a container to store sequences of Ts. It can be resized.
 * Other use cases include search, replacement, etc...
 */
export template <typename T, typename Allocator, typename GrowPolicy>
class vector_storage
{
private:
    size_type capacity_{0U};
    size_type size_{0U};
    T* buffer_{nullptr};

public:
    using iterator        = T*;
    using const_iterator  = T const*;
    using reference       = T&;
    using const_reference = T const&;
    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = T const*;

    /**
     * @brief Default constructor
     */
    constexpr vector_storage() noexcept = default;

    /**
     * @brief Construct a new vector storage object with reserved memory.
     * The memory for the vector is allocated, but no construction is done
     * @param size Number of elements to reserve memory for
     */
    explicit vector_storage(size_type const size) :
        capacity_{size},
        buffer_{size > 0U ? Allocator::allocate(size) : nullptr}
    {}

    /**
     * @brief Deleted copy constructor
     */
    vector_storage(vector_storage const&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    vector_storage& operator=(vector_storage const&) = delete;

    /**
     * @brief Move constructor.
     * @param other r-value reference to an object
     * @return this vector
     */
    constexpr vector_storage(vector_storage&& other) noexcept :
        capacity_{std::exchange(other.capacity_, 0U)},
        size_{std::exchange(other.size_, 0U)},
        buffer_{std::exchange(other.buffer_, nullptr)}
    {}

    /**
     * @brief Move assignment
     * @param other r-value reference to an object
     * @return This vector
     */
    constexpr vector_storage& operator=(vector_storage&& other) noexcept
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Destroy the vector storage object
     */
    ~vector_storage() noexcept
    {
        if (buffer_ != nullptr)
        {
            Allocator::deallocate_with_size(buffer_, capacity_);
            buffer_   = nullptr;
            size_     = 0U;
            capacity_ = 0U;
        }
    }

    constexpr pointer at(const size_type index) noexcept
    {
        return (buffer_ + index);
    }

    constexpr const_pointer at(const size_type index) const noexcept
    {
        return (buffer_ + index);
    }

    constexpr const_pointer cat(const size_type index) const noexcept
    {
        return (buffer_ + index);
    }

    constexpr size_type capacity() const noexcept { return capacity_; }
    constexpr size_type size() const noexcept { return size_; }
    constexpr bool empty() const noexcept { return size_ == 0U; }
    constexpr iterator begin() noexcept { return buffer_; }
    constexpr const_iterator begin() const noexcept { return buffer_; }
    constexpr const_iterator cbegin() const noexcept { return buffer_; }
    constexpr iterator end() noexcept { return buffer_ + size_; }
    constexpr const_iterator end() const noexcept { return buffer_ + size_; }
    constexpr const_iterator cend() const noexcept { return buffer_ + size_; }

    constexpr void swap(vector_storage& other) noexcept
    {
        std::swap(buffer_, other.buffer_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    constexpr void push_back(const T& value)
    {
        reserve(GrowPolicy::growSize(size_));
        Allocator::construct((buffer_ + size_++), value);
    }

    constexpr void push_back(T&& value)
    {
        reserve(GrowPolicy::growSize(size_));
        Allocator::construct(static_cast<T*>(buffer_ + size_++),
                             htps::move(value));
    }

    template <typename... Args>
    constexpr void emplace_back(Args&&... args)
    {
        reserve(GrowPolicy::growSize(size_));
        Allocator::construct(static_cast<T*>(buffer_ + size_++),
                             htps::forward<Args>(args)...);
    }

    constexpr void pop_back() noexcept
    {
        if (size_ > 0U)
        {
            Allocator::destruct(end() - 1U);
            --size_;
        }
    }

    constexpr void reserve(const size_type capacity)
    {
        if (capacity_ < capacity)
        {
            vector_storage new_vector{
                vector_storage{static_cast<size_type>(capacity)}};

            for (auto&& elements_old_vector : *this)
            {
                new_vector.push_back(htps::move(elements_old_vector));
            }

            *this = htps::move(new_vector);
        }
    }

    constexpr void insert(iterator const where, const T& newElement)
    {
        auto const newSize{size() + 1U};
        bool const isResizing{capacity_ < newSize};
        if (isResizing)
        {
            vector_storage new_vector{
                vector_storage{static_cast<size_type>(newSize)}};

            for (auto it{begin()}; it < where; ++it)
            {
                new_vector.push_back(htps::move(*it));
            }

            new_vector.push_back(newElement);

            for (auto it{where}; it < end(); ++it)
            {
                new_vector.push_back(htps::move(*it));
            }
            *this = htps::move(new_vector);
        }
        else
        {
            if (where >= end())
            {
                Allocator::construct(where, newElement);
            }
            else
            {
                auto const prev_end{std::prev(end())};
                Allocator::construct(end(), *prev_end);

                if (prev_end > begin())
                {
                    for (auto it{prev_end}; it > where && it > begin(); --it)
                    {
                        *it = std::move(*(std::prev(it)));
                    }
                }
                *where = newElement;
            }

            ++size_;
        }
    }

    constexpr void shrink_to_fit()
    {
        if (size_ < capacity_)
        {
            vector_storage new_vector{
                vector_storage{static_cast<size_type>(size_)}};

            for (auto&& elements_old_vector : *this)
            {
                new_vector.push_back(htps::move(elements_old_vector));
            }

            *this = htps::move(new_vector);
        }
    }
};

}  // namespace htps

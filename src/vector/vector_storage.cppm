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
    size_type m_capacity{0U};
    size_type m_size{0U};
    T* m_buffer{nullptr};

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
        m_capacity{size},
        m_buffer{size > 0U ? Allocator::allocate(size) : nullptr}
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
        m_capacity{std::exchange(other.m_capacity, 0U)},
        m_size{std::exchange(other.m_size, 0U)},
        m_buffer{std::exchange(other.m_buffer, nullptr)}
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
        if (m_buffer != nullptr)
        {
            Allocator::deallocate(m_buffer, m_capacity);
            m_buffer   = nullptr;
            m_size     = 0U;
            m_capacity = 0U;
        }
    }

    constexpr pointer at(const size_type index) noexcept
    {
        return (m_buffer + index);
    }

    constexpr const_pointer at(const size_type index) const noexcept
    {
        return (m_buffer + index);
    }

    constexpr const_pointer cat(const size_type index) const noexcept
    {
        return (m_buffer + index);
    }

    constexpr size_type capacity() const noexcept { return m_capacity; }
    constexpr size_type size() const noexcept { return m_size; }
    constexpr bool empty() const noexcept { return m_size == 0U; }
    constexpr iterator begin() noexcept { return m_buffer; }
    constexpr const_iterator begin() const noexcept { return m_buffer; }
    constexpr const_iterator cbegin() const noexcept { return m_buffer; }
    constexpr iterator end() noexcept { return m_buffer + m_size; }
    constexpr const_iterator end() const noexcept { return m_buffer + m_size; }
    constexpr const_iterator cend() const noexcept { return m_buffer + m_size; }

    constexpr void swap(vector_storage& other) noexcept
    {
        std::swap(m_buffer, other.m_buffer);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }

    constexpr void push_back(const T& value)
    {
        reserve(GrowPolicy::growSize(m_size));
        Allocator::construct((m_buffer + m_size++), value);
    }

    constexpr void push_back(T&& value)
    {
        reserve(GrowPolicy::growSize(m_size));
        Allocator::construct(static_cast<T*>(m_buffer + m_size++), move(value));
    }

    template <typename... Args>
    constexpr void emplace_back(Args&&... args)
    {
        reserve(GrowPolicy::growSize(m_size));
        Allocator::construct(static_cast<T*>(m_buffer + m_size++),
                             forward<Args>(args)...);
    }

    constexpr void pop_back() noexcept
    {
        if (m_size > 0U)
        {
            Allocator::destruct(end() - 1U);
            --m_size;
        }
    }

    constexpr void reserve(const size_type capacity)
    {
        if (m_capacity < capacity)
        {
            vector_storage new_vector{
                vector_storage{static_cast<size_type>(capacity)}};

            for (auto&& elements_old_vector : *this)
            {
                new_vector.push_back(move(elements_old_vector));
            }

            *this = move(new_vector);
        }
    }

    constexpr void insert(iterator const where, const T& newElement)
    {
        auto const newSize{size() + 1U};
        bool const isResizing{m_capacity < newSize};
        if (isResizing)
        {
            vector_storage new_vector{
                vector_storage{static_cast<size_type>(newSize)}};

            for (auto it{begin()}; it < where; ++it)
            {
                new_vector.push_back(move(*it));
            }

            new_vector.push_back(newElement);

            for (auto it{where}; it < end(); ++it)
            {
                new_vector.push_back(move(*it));
            }
            *this = move(new_vector);
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
                        *it = move(*(std::prev(it)));
                    }
                }
                *where = newElement;
            }

            ++m_size;
        }
    }

    constexpr void shrink_to_fit()
    {
        if (m_size < m_capacity)
        {
            vector_storage new_vector{
                vector_storage{static_cast<size_type>(m_size)}};

            for (auto&& elements_old_vector : *this)
            {
                new_vector.push_back(move(elements_old_vector));
            }

            *this = move(new_vector);
        }
    }
};

}  // namespace htps

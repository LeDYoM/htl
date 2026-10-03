module;

#include <initializer_list>

export module htypes:bdictionary;

import :btypes;
import :vector;
import :pair;

namespace htps
{
/**
 * @brief This class contains a group of key_types and their associated values.
 * @tparam T Type of the values stored
 */
export template <typename Key, typename T>
class BasicDictionary final
{
public:
    using value_type     = T;
    using key_type       = Key;
    using element        = pair<key_type, value_type>;
    using content        = vector<element>;
    using iterator       = typename content::iterator;
    using const_iterator = typename content::const_iterator;

    /**
     * @brief  Default constructor
     *
     */
    [[nodiscard]] constexpr BasicDictionary() noexcept = default;

    [[nodiscard]] constexpr BasicDictionary(
        std::initializer_list<element> eList) :
        data_{htps::move(eList)}
    {}

    [[nodiscard]] constexpr BasicDictionary(const content& eList) :
        data_{htps::move(eList)}
    {}

    [[nodiscard]] constexpr iterator begin() noexcept { return data_.begin(); }
    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return data_.begin();
    }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return data_.begin();
    }
    [[nodiscard]] constexpr iterator end() noexcept { return data_.end(); }
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return data_.end();
    }
    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return data_.end();
    }

    [[nodiscard]] constexpr bool add(content const& eList,
                                     bool const overwrite = true)
    {
        bool result{true};
        for (const element& elems : eList)
        {
            result &= add(elems.first, elems.second, overwrite);
        }
        return result;
    }

    [[nodiscard]] constexpr bool add(std::initializer_list<element> eList,
                                     const bool overwrite = true)
    {
        bool result{true};
        for (const element& elems : eList)
        {
            result &= add(elems.first, elems.second, overwrite);
        }
        return result;
    }

    [[nodiscard]] constexpr bool add(key_type key,
                                     T value              = {},
                                     const bool overwrite = true)
    {
        auto const it{find(key)};

        if (it == data_.end())
        {
            data_.emplace_back(htps::move(key), htps::move(value));
            return true;
        }
        else if (overwrite)
        {
            (*it) = element(htps::move(key), htps::move(value));
            return true;
        }
        return false;
    }

    constexpr bool put(key_type const& key,
                       T value,
                       const bool overwrite = true)
    {
        (void)(add(htps::move(key), htps::move(value), overwrite));
        return *this;
    }

    constexpr void clear() { data_.clear(); }

    constexpr content const& data() const noexcept { return data_; }

    constexpr const_iterator find(key_type const& key) const noexcept
    {
        return data_.find_if(
            [&key](auto const& element) { return element.first == key; });
    }

    constexpr const_iterator cfind(key_type const& key) const noexcept
    {
        return find(key);
    }

    constexpr iterator find(key_type const& key) noexcept
    {
        return data_.find_if(
            [&key](auto const& element) { return element.first == key; });
    }

    constexpr bool exists(key_type const& key) const noexcept
    {
        return cfind(key) != data_.cend();
    }

    constexpr pair<bool, iterator> find_checked(key_type const& key) noexcept
    {
        auto const iterator(find(key));
        return {iterator != data_.cend(), iterator};
    }

    constexpr pair<bool, const_iterator> find_checked(
        key_type const& key) const noexcept
    {
        auto const it(cfind(key));
        return {it != data_.cend(), it};
    }

    constexpr pair<bool, const_iterator> cfind_checked(
        key_type const& key) const noexcept
    {
        return find_checked(key);
    }

    constexpr pair<bool, ssize_type> cfind_index(
        key_type const& key) const noexcept
    {
        auto const index{data_.find_index_if(
            [&key](auto const& element) { return element.first == key; })};
        return {index > -1 ? true : false, static_cast<ssize_type>(index)};
    }

    constexpr pair<bool, ssize_type> find_index(
        key_type const& key) const noexcept
    {
        return cfind_index(key);
    }

    constexpr size_type size() const noexcept { return data_.size(); }

    constexpr bool empty() const noexcept { return data_.empty(); }

    constexpr value_type const& index(size_type const index) const
    {
        return data_[index].second;
    }

    constexpr value_type& index(size_type const index)
    {
        return data_[index].second;
    }

    constexpr size_type erase(key_type const& key)
    {
        size_type previous_size{size()};

        data_.template erase_if<false>(
            [&key](element const& current) { return current.first == key; });

        return (previous_size - size());
    }

    constexpr bool operator==(BasicDictionary const& rhs) const noexcept =
        default;

private:
    content data_;
};

}  // namespace htps

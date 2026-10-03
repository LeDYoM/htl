export module htypes:hash_table;

import :bdictionary;

namespace htps
{
/**
 * @brief This class contains a group of keys and their associated values.
 * @tparam T Type of the values stored
 */
export template <typename T>
using HashTable = BasicDictionary<u32, T>;

}  // namespace htps


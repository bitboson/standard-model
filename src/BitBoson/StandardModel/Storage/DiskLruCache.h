/* This file is part of standard-model.
 *
 * Copyright (c) BitBoson
 *
 * standard-model is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * standard-model is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with standard-model.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Written by:
 *     - Tyler Parcell <OriginLegend>
 */

#ifndef BITBOSON_STANDARDMODEL_DISKLRUCACHE_H
#define BITBOSON_STANDARDMODEL_DISKLRUCACHE_H

#include <memory>
#include <BitBoson/StandardModel/Storage/DiskCache.h>
#include <BitBoson/StandardModel/Primitives/BigInt.hpp>

namespace BitBoson::StandardModel
{

    class DiskLruCache
    {

        // Public member class
        public:

            class DiskLruCacheSupplier
            {

                // Public member functions
                public:

                    /**
                     * Virtual function used to add an item to the supplier
                     *
                     * @param key String representing the key for the item to add
                     * @param item String item to add to the data store
                     * @return Boolean indicating whether the item was added or not
                     */
                    virtual bool addItem(const std::string& key, const std::string& item) = 0;

                    /**
                     * Virtual function used to get the value for the given key from the supplier
                     *
                     * @param key String representing the key for the item to get
                     * @return String representing the value for the given key
                     */
                    virtual std::string getItem(const std::string& key) = 0;

                    /**
                     * Virtual function used to remove the value for the given key from the supplier
                     *
                     * @param key String representing the key for the item to remove
                     * @return Boolean indicating whether the item was removed or not
                     */
                    virtual bool deleteItem(const std::string& key) = 0;

                    /**
                     * Destructor used to cleanup the instance
                     */
                    virtual ~DiskLruCacheSupplier() = default;
            };

            // Private structures
        private:
            struct CacheItem
            {
                bool isDirty;
                std::string key;
                long dataSize;
                BigInt lastUsedIndex;
            };

        // Private member variables
        private:
            long _maxOnDiskCacheSize;
            BigInt _lastUsedIndex;
            std::shared_ptr<DiskCache> _diskCache;
            std::vector<std::shared_ptr<CacheItem>> _cacheMd;
            std::shared_ptr<DiskLruCacheSupplier> _cacheSupplier;

        // Public member functions
        public:

            /**
             * Constructor used to setup the cache with the given specifications
             *
             * @param cacheSupplier Disk Cache Supplier used to read and write cache data to and from
             * @param onDiskCacheSizeInBytes Long representing the on-disk cache size (in bytes)
             * @param memoryCacheSizeInBytes Long representing the in-memory cache size (in bytes)
             */
            explicit DiskLruCache(std::shared_ptr<DiskLruCacheSupplier> cacheSupplier,
                    long onDiskCacheSizeInBytes, long memoryCacheSizeInBytes=DataStore::DEFAULT_CACHE_SIZE);

            /**
             * Function used to add an item to the disk-lru-cache
             *
             * @param key String representing the key for the item to add
             * @param item String item to add to the data store
             * @param writeBack Boolean indicating to write back the item to the supplier now
             * @return Boolean indicating whether the item was added or not
             */
            bool addItem(const std::string& key, const std::string& item, bool writeBack=false);

            /**
             * Function used to get the value for the given key from the disk-lru-cache
             *
             * @param key String representing the key for the item to get
             * @return String representing the value for the given key (or default if it doesn't exist)
             */
            std::string getItem(const std::string& key);

            /**
             * Function used to remove an item from the disk-lru-cache
             *
             * @param key String representing the key for the item to remove
             * @return Boolean indicating whether the item was removed or not
             */
            bool deleteItem(const std::string& key);

            /**
             * Function used to write all in-cache changes to the supplier now
             * NOTE: This is useful for when you want to delete/remove the cache
             *
             * @return Boolean indicating whether the items were all written back
             */
            bool writeAllBackNow();

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~DiskLruCache() = default;

        // Private member functions
        private:

            /**
             * Internal function used to get the current on-disk cache size
             *
             * @return Long representing the on-disk current cache size in bytes
             */
            long getCurrentDiskCacheSize();

            /**
             * Internal function used to get the current least-recently used item
             *
             * @return Long representing the index (in the MD-vector) of the least
             *         recently used cache item
             */
            long getCurrentLeastRecentlyUsedItem();

            /**
             * Internal function used to remove the least recently used item
             *
             * @return Boolean indicating whether an item was actually removed or not
             */
            bool removeLeastRecentlyUsedItem();

            /**
             * Internal function used to remove the given item from the cache
             * NOTE: This will write data back to the supplier if needed
             *
             * @param itemIndex Long representing te MD-vector index to remove
             * @return Boolean indicating whether an item was actually removed or not
             */
            bool removeItemByIndex(long itemIndex);

            /**
             * Internal function used to get the desired item's index in the MD-vector
             * NOTE: Will return -1 if the item is not in the MD-vector
             *
             * @param key String representing the key of the item to search for
             * @return Long representing the index of he item  in the MD-vector
             */
            long getCacheItemIndex(const std::string& key);
    };
}

#endif //BITBOSON_STANDARDMODEL_DISKLRUCACHE_H

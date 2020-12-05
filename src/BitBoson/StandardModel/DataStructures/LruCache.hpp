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

#ifndef BITBOSON_STANDARDMODEL_LRUCACHE_HPP
#define BITBOSON_STANDARDMODEL_LRUCACHE_HPP

#include <vector>
#include <memory>
#include <utility>

namespace BitBoson::StandardModel
{

    template <class T> class LruCache
    {

        // Public member class
        public:

            class LruCacheSupplier
            {

                // Public member functions
                public:

                    /**
                     * Virtual function used to add an item to the supplier
                     *
                     * @param key String representing the key for the item to add
                     * @param item Generic (T) Data item to add to the data store
                     * @return Boolean indicating whether the item was added or not
                     */
                    virtual bool addItem(const std::string& key, std::shared_ptr<T> item) = 0;

                    /**
                     * Virtual function used to get the value for the given key from the supplier
                     *
                     * @param key String representing the key for the item to get
                     * @return Generic (T) Data representing the value for the given key
                     */
                    virtual std::shared_ptr<T> getItem(const std::string& key) = 0;

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
                    virtual ~LruCacheSupplier() = default;
            };

        // Private member structure
        public:

            struct CacheNode
            {
                std::string key;
                std::shared_ptr<T> val;
            };

        // Private member variables
        private:
            unsigned long _cacheSize;
            std::list<std::string> _cacheList;
            std::shared_ptr<LruCacheSupplier> _cacheSupplier;
            std::unordered_map<std::string, CacheNode*> _cacheMap;

        // Public member functions
        public:

            /**
             * Constructor used to setup the cache with the given specifications
             *
             * @param cacheSupplier Cache Supplier used to read and write cache data to and from
             * @param cacheSize Long representing the in-memory cache size (in number of items)
             */
            explicit LruCache(std::shared_ptr<LruCacheSupplier> cacheSupplier,
                    long cacheSize=1024)
            {

                // Initialize relevant member variables
                _cacheSize = cacheSize;
                _cacheSupplier = std::move(cacheSupplier);
            }

            /**
             * Function used to add an item to the lru-cache
             *
             * @param key String representing the key for the item to add
             * @param item Generic (T) Data item to add to the data store
             * @param writeBack Boolean indicating to write back the item to the supplier now
             * @return Boolean indicating whether the item was added or not
             */
            bool addItem(const std::string& key, std::shared_ptr<T> item, bool writeBack=false)
            {

                // Create a return flag
                bool retFlag = false;

                // Only continue if the key is valid
                if (!key.empty())
                {

                    // Start by attempting to get the node from the map
                    CacheNode* mapVal = nullptr;
                    if (_cacheMap.find(key) != _cacheMap.end())
                        mapVal = _cacheMap[key];

                    // If the node already existed in the cache then
                    // update its value accordingly
                    if (mapVal != nullptr)
                    {

                        // Update the node's internal cache value
                        mapVal->val = item;

                        // Reset the node's position in the LRU-cache
                        _cacheList.remove(mapVal->key);
                        _cacheList.push_front(mapVal->key);

                        // Setup the return value as true
                        retFlag = true;
                    }

                    // If the node didn't already exist in the cache
                    // then go ahead and add it accordingly
                    else
                    {

                        // If we're already at capacity we'll need to
                        // remove the last item and write it back
                        if (_cacheMap.size() >= _cacheSize)
                        {

                            // Get the least-recently-used item
                            auto lruItemKey = _cacheList.back();
                            auto lruItem = _cacheMap[lruItemKey];

                            // Write the node value back to the supplier
                            _cacheSupplier->addItem(lruItem->key, lruItem->val);

                            // Remove the least-recently-used item from
                            // both the map and the linked-list
                            _cacheMap.erase(lruItem->key);
                            _cacheList.remove(lruItem->key);
                        }

                        // Create the new node to add for the new data
                        auto newNode = new CacheNode();
                        newNode->key = key;
                        newNode->val = item;

                        // Add the new node we just created to the map
                        // and the linked-list for cache-use
                        _cacheMap[key] = newNode;
                        _cacheList.push_front(newNode->key);

                        // Setup the return value as true
                        retFlag = true;
                    }

                    // If we are performing a write-back operation then
                    // go ahead and write the item back to the supplier
                    if (writeBack)
                        retFlag &= _cacheSupplier->addItem(key, item);
                }

                // Return the return flag
                return retFlag;
            }

            /**
             * Function used to get the value for the given key from the lru-cache
             *
             * @param key String representing the key for the item to get
             * @return Generic (T) Data representing the value for the given key
             */
            std::shared_ptr<T> getItem(const std::string& key)
            {

                // Create a return value
                std::shared_ptr<T> retVal = nullptr;

                // Only continue if the key is valid
                if (!key.empty())
                {

                    // Start by attempting to get the node from the map
                    CacheNode* mapVal = nullptr;
                    if (_cacheMap.find(key) != _cacheMap.end())
                        mapVal = _cacheMap[key];

                    // If the node exists in the map then return its value
                    // otherwise get it from the supplier
                    if (mapVal != nullptr)
                        retVal = mapVal->val;
                    else
                        retVal = _cacheSupplier->getItem(key);

                    // If the map contained the item then update its position
                    // in the LRU-list since it is now the most recently used
                    if (mapVal != nullptr)
                    {

                        // Move the item to the front of the linked-list
                        _cacheList.remove(mapVal->key);
                        _cacheList.push_front(mapVal->key);
                    }

                    // If we had to get the node from the supplier then
                    // add it back to the cache and update it accordingly
                    if ((mapVal == nullptr) && (retVal != nullptr))
                        addItem(key, retVal);
                }

                // Return the return value
                return retVal;
            }

            /**
             * Function used to remove an item from the lru-cache
             *
             * @param key String representing the key for the item to remove
             * @return Boolean indicating whether the item was removed or not
             */
            bool deleteItem(const std::string& key)
            {

                // Create a return flag
                bool retFlag = false;

                // Only continue if the key is valid
                if (!key.empty())
                {

                    // Start by attempting to get the node from the map
                    CacheNode* mapVal = nullptr;
                    if (_cacheMap.find(key) != _cacheMap.end())
                        mapVal = _cacheMap[key];

                    // Delete the item from the cache if needed
                    if (mapVal != nullptr)
                    {

                        // Remove the item from the map and linked-list
                        _cacheMap.erase(mapVal->key);
                        _cacheList.remove(mapVal->key);
                    }

                    // Perform the corresponding delete on the supplier
                    retFlag = _cacheSupplier->deleteItem(key);
                }

                // Return the return flag
                return retFlag;
            }

            /**
             * Function used to write all in-cache changes to the supplier now
             * NOTE: This is useful for when you want to delete/remove the cache
             *
             * @return Boolean indicating whether the items were all written back
             */
            bool writeAllBackNow()
            {

                // Create a return flag
                bool retFlag = true;

                // Write all of the cache items back to the supplier
                for (auto cacheItem : _cacheMap)
                    retFlag &= _cacheSupplier->addItem(
                            cacheItem.first, cacheItem.second->val);

                // Return the return flag
                return retFlag;
            }

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~LruCache()
            {

                // Flush/write-back the cache items
                writeAllBackNow();
            }
    };
}

#endif //BITBOSON_STANDARDMODEL_LRUCACHE_HPP

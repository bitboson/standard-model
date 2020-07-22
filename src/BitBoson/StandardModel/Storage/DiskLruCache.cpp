/* This file is part of BitBoson.
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

#include <utility>
#include <BitBoson/StandardModel/Storage/DiskLruCache.h>

using namespace BitBoson::StandardModel;

/**
 * Constructor used to setup the cache with the given specifications
 *
 * @param cacheSupplier Disk Cache Supplier used to read and write cache data to and from
 * @param onDiskCacheSizeInBytes Long representing the on-disk cache size (in bytes)
 * @param memoryCacheSizeInBytes Long representing the in-memory cache size (in bytes)
 */
DiskLruCache::DiskLruCache(std::shared_ptr<DiskLruCacheSupplier> cacheSupplier,
        long onDiskCacheSizeInBytes, long memoryCacheSizeInBytes)
{

    // Initialize relevant member variables
    _lastUsedIndex = 0;
    _cacheSupplier = std::move(cacheSupplier);
    _maxOnDiskCacheSize = onDiskCacheSizeInBytes;
    _diskCache = std::make_shared<DiskCache>(memoryCacheSizeInBytes);
}

/**
 * Function used to add an item to the disk-lru-cache
 *
 * @param key String representing the key for the item to add
 * @param item String item to add to the data store
 * @param writeBack Boolean indicating to write back the item to the supplier now
 * @return Boolean indicating whether the item was added or not
 */
bool DiskLruCache::addItem(const std::string& key, const std::string& item, bool writeBack)
{

    // Create a return flag
    bool retFlag = false;

    // First attempt to get the item from the cache itself
    auto possibleItem = getItem(key);

    // If the item already exists, start by removing the item
    // from the local cache
    retFlag = true;
    if (!possibleItem.empty())
        retFlag = removeItemByIndex(getCacheItemIndex(key));

    // Continuously remove items from the cache until enough free space is available
    // for the new item (it may be bigger than the original version of the item)
    while ((getCurrentDiskCacheSize() + item.size()) > _maxOnDiskCacheSize)
        if (!removeLeastRecentlyUsedItem())
            break;

    // At this point, there should be room in the on-disk cache for the new item
    // so store it in the on-disk cache
    _diskCache->addItem(key, item);

    // If wer are supposed to rite the item back, do so now
    if (writeBack)
        writeBack = _cacheSupplier->addItem(key, item);

    // Finally, add the the item to the MD-vector
    auto newCacheItem = std::make_shared<CacheItem>();
    newCacheItem->isDirty = !writeBack;
    newCacheItem->key = key;
    newCacheItem->lastUsedIndex = _lastUsedIndex++;
    newCacheItem->dataSize = item.size();
    _cacheMd.push_back(newCacheItem);

    // Return the return flag
    return retFlag;
}

/**
 * Function used to get the value for the given key from the disk-lru-cache
 *
 * @param key String representing the key for the item to get
 * @return String representing the value for the given key (or default if it doesn't exist)
 */
std::string DiskLruCache::getItem(const std::string& key)
{

    // Create a return value
    std::string retVal;

    // First check if the item is currently in our cache or not
    auto currItemIndex = getCacheItemIndex(key);

    // Only proceed to the next part if the item isn't in the cache
    if (currItemIndex < 0)
    {

        // Before trying to move the item from the supplier into the disk-cache
        // we need to ensure there is enough room for it, so start by getting it
        auto newItem = _cacheSupplier->getItem(key);
        if (!newItem.empty())
        {

            // Continuously remove items from the cache until enough free space is available
            while ((getCurrentDiskCacheSize() + newItem.size()) > _maxOnDiskCacheSize)
                if (!removeLeastRecentlyUsedItem())
                    break;

            // At this point, there should be room in the on-disk cache for the new item
            // so store it in the on-disk cache
            _diskCache->addItem(key, newItem);

            // Finally, add the the item to the MD-vector
            auto newCacheItem = std::make_shared<CacheItem>();
            newCacheItem->isDirty = false;
            newCacheItem->key = key;
            newCacheItem->lastUsedIndex = _lastUsedIndex++;
            newCacheItem->dataSize = newItem.size();
            _cacheMd.push_back(newCacheItem);
        }
    }

    // Extract the item from the cache
    retVal = _diskCache->getItem(key);

    // Return the return value
    return retVal;
}

/**
 * Function used to remove an item from the disk-lru-cache
 *
 * @param key String representing the key for the item to remove
 * @return Boolean indicating whether the item was removed or not
 */
bool DiskLruCache::deleteItem(const std::string& key)
{

    // Determine if the item is in the MD-vector and
    // remove it if it is
    bool deletedLocally = false;
    auto mdVectIndex = getCacheItemIndex(key);
    if (mdVectIndex >= 0)
        deletedLocally = removeItemByIndex(mdVectIndex);

    // Remove the item from the local disk cache
    auto deletedRemotely = _diskCache->deleteItem(key);

    // Remove the item from the cache supplier
    auto deletedInSupplier = _cacheSupplier->deleteItem(key);

    // Return the return flag
    return (deletedLocally || deletedRemotely || deletedInSupplier);
}

/**
 * Function used to write all in-cache changes to the supplier now
 * NOTE: This is useful for when you want to delete/remove the cache
 *
 * @return Boolean indicating whether the items were all written back
 */
bool DiskLruCache::writeAllBackNow()
{

    // Create a return flag
    bool retFlag = true;

    // Write all dirty items back to the cache supplier
    for (const auto& cacheItem : _cacheMd)
        if (cacheItem->isDirty)
            retFlag &= _cacheSupplier->addItem(cacheItem->key,
                    _diskCache->getItem(cacheItem->key));

    // Return the return flag
    return retFlag;
}

/**
 * Internal function used to get the current on-disk cache size
 *
 * @return Long representing the on-disk current cache size in bytes
 */
long DiskLruCache::getCurrentDiskCacheSize()
{

    // Create a return value
    long retVal = 0;

    // Loop through all of the items in the MD-vector summing up
    // the total size of the on-disk items
    for (const auto& cacheItem : _cacheMd)
        retVal += cacheItem->dataSize;

    // Return the return value
    return retVal;
}

/**
 * Internal function used to get the current least-recently used item
 *
 * @return Long representing the index (in the MD-vector) of the least
 *         recently used cache item
 */
long DiskLruCache::getCurrentLeastRecentlyUsedItem()
{

    // Create a return value
    long retVal = 0;

    // Loop through all of the items in the MD-vector looking for
    // the least-recently used item
    for (long ii = 1; ii < _cacheMd.size(); ii++)
        if (_cacheMd[ii]->lastUsedIndex < _cacheMd[retVal]->lastUsedIndex)
            retVal = ii;

    // If there are no iems in the cache, return -1
    if (_cacheMd.empty())
        retVal = -1;

    // Return the return value
    return retVal;
}

/**
 * Internal function used to remove the least recently used item
 *
 * @return Boolean indicating whether an item was actually removed or not
 */
bool DiskLruCache::removeLeastRecentlyUsedItem()
{

    // Remove the lest-recently-used item from the cache
    return removeItemByIndex(getCurrentLeastRecentlyUsedItem());
}

/**
 * Internal function used to remove the given item from the cache
 * NOTE: This will write data back to the supplier if needed
 *
 * @param itemIndex Long representing te MD-vector index to remove
 * @return Boolean indicating whether an item was actually removed or not
 */
bool DiskLruCache::removeItemByIndex(long itemIndex)
{

    // Create a return flag
    bool retFlag = false;

    // Only continue if the index is valid
    if ((itemIndex >= 0) && (itemIndex < _cacheMd.size()))
    {

        // Move the item to the back of the vector
        if (_cacheMd.size() > 1)
        {
            auto tmpVal = _cacheMd[itemIndex];
            _cacheMd[itemIndex] = _cacheMd.back();
            _cacheMd[_cacheMd.size() - 1] = tmpVal;
        }

        // Remove the last item in the vector
        auto itemToRemove = _cacheMd.back();
        _cacheMd.pop_back();

        // Extract the content from the on-disk cache
        auto keyToRemove = itemToRemove->key;
        auto contentToRemove = _diskCache->getItem(keyToRemove);

        // If the item is "dirty" then write it back to the supplier and save the result
        retFlag = true;
        if (itemToRemove->isDirty)
            retFlag = _cacheSupplier->addItem(keyToRemove, contentToRemove);

        // Now simply remove the item from the local cache (if successful so far)
        if (retFlag)
            _diskCache->deleteItem(keyToRemove);
    }

    // Return the return flag
    return retFlag;
}

/**
 * Internal function used to get the desired item's index in the MD-vector
 * NOTE: Will return -1 if the item is not in the MD-vector
 *
 * @param key String representing the key of the item to search for
 * @return Long representing the index of he item  in the MD-vector
 */
long DiskLruCache::getCacheItemIndex(const std::string& key)
{

    // Create a return value
    long retVal = -1;

    // Loop through all of the items in the MD-vector looking for
    // the desired item by the  given key
    for (long ii = 0; ii < _cacheMd.size(); ii++)
    {
        if (_cacheMd[ii]->key == key)
        {
            retVal = ii;
            break;
        }
    }

    // Return the return value
    return retVal;
}

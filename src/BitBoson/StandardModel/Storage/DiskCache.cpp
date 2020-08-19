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

#include <BitBoson/StandardModel/Storage/DiskCache.h>
#include <BitBoson/StandardModel/FileSystem/FileSystem.h>

using namespace BitBoson::StandardModel;

/**
 * Constructor used to setup the disk-cache instance
 *
 * @param cacheSizeInBytes Long representing the cache size (in bytes)
 * @param directory String representing the directory to store information in
 */
DiskCache::DiskCache(long cacheSizeInBytes, const std::string& directory)
{

    // Initialize the cache with the given parameters
    initializeCache(cacheSizeInBytes, directory);
}

/**
* Constructor used to setup the disk-cache instance
*
* @param directory String representing the directory to store information in
* @param cacheSizeInBytes Long representing the cache size (in bytes)
*/
DiskCache::DiskCache(const std::string& directory, long cacheSizeInBytes)
{

    // Initialize the cache with the given parameters
    initializeCache(cacheSizeInBytes, directory);
}

/**
 * Function used to get the current cache directory being used
 *
 * @return String representing the cache directory for the cache
 */
std::string DiskCache::getCacheDirectory()
{

    // Get and return the cache-directory
    return _dataStore->getDataStoreDirectory();
}

/**
 * Function used to indicate that the cache should not be deleted
 * upon destruction of the object's instance
 *
 * @param persist Boolean indicating whether the cache should persist
 */
void DiskCache::setPersistOnDestruction(bool persist)
{

    // Set the persist flag accordingly
    _shouldPersist = persist;
}

/**
 * Function used to get the underlying DataStore reference
 *
 * @return DataStore reference representing the underlying DataStore
 */
std::shared_ptr<DataStore> DiskCache::getUnderlyingDataStoreRef()
{

    // Get and return the underlying DataStore
    return _dataStore;
}

/**
 * Function used to add an item to the disk-cache
 *
 * @param key String representing the key for the item to add
 * @param item String item to add to the data store
 * @return Boolean indicating whether the item was added or not
 */
bool DiskCache::addItem(const std::string& key, const std::string& item)
{

    // Add the item and return the result
    return _dataStore->addItem(key, item, true);
}

/**
 * Function used to get the value for the given key from the disk-cache
 *
 * @param key String representing the key for the item to get
 * @return String representing the value for the given key (or default if it doesn't exist)
 */
std::string DiskCache::getItem(const std::string& key)
{

    // Get the item and return the result
    return _dataStore->getItem(key);
}

/**
 * Function used to delete the given item from the key-value disk-cache
 *
 * @param key String representing the key for the item to delete
 * @return Boolean indicating whether the item was deleted or not
 */
bool DiskCache::deleteItem(const std::string& key)
{

    // Remove the item and return the result
    return _dataStore->deleteItem(key);
}

/**
 * Internal function used to setup the disk-cache instance
 *
 * @param cacheSizeInBytes Long representing the cache size (in bytes)
 * @param directory String representing the directory to store information in
 */
void DiskCache::initializeCache(long cacheSizeInBytes, const std::string& directory)
{

    // Determine the directory to store information in
    std::string cacheDirectory;
    if (directory.empty())
        cacheDirectory = FileSystem::getTemporaryDir("BitBoson_").getFullPath();
    else
        cacheDirectory = directory;

    // Initialize the underlying data-store
    _dataStore = std::make_shared<DataStore>(cacheDirectory,
            false, cacheSizeInBytes);

    // Assume we will not be persisting the cache by default
    _shouldPersist = false;
}

/**
 * Destructor used to cleanup the cache entirely
 */
DiskCache::~DiskCache()
{

    // Completely cleanup the underlying data-store
    // unless indicated otherwise by the persist flag
    if (!_shouldPersist)
        _dataStore->deleteEntireDataStore(false);
}

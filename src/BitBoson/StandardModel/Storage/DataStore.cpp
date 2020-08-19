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

#include <leveldb/cache.h>
#include <leveldb/write_batch.h>
#include <leveldb/filter_policy.h>
#include <BitBoson/StandardModel/Utils/Utils.h>
#include <BitBoson/StandardModel/Storage/DataStore.h>
#include <BitBoson/StandardModel/FileSystem/FileSystem.h>

using namespace BitBoson::StandardModel;

// Setup/define static constants
const long DataStore::DEFAULT_CACHE_SIZE = 100*1048576;

/**
 * Constructor used to setup the data-store instance
 *
 * @param filePath String representing the full file path for
 *                 the key-value data store directory file
 * @param reCreate Boolean indicating whether to re-create the data-store or not
 *                 This will delete everything in the containing directory
 * @param cacheSizeInBytes Long representing the cache size for
 *                         the data-store (in bytes)
 */
DataStore::DataStore(const std::string& dataDir, bool recreate, long cacheSizeInBytes)
{

    // Get a random directory to use as disk-cache
    _dataStoreDir = dataDir;
    auto fileSystem = FileSystem(_dataStoreDir);
    if (recreate && fileSystem.exists())
    {
        fileSystem.removeDir();
        fileSystem.createDir();
    }

    // Setup the LevelDB instance on the temporary directory
    _keyValueDb = nullptr;
    _options.create_if_missing = true;
    _options.block_cache = leveldb::NewLRUCache(cacheSizeInBytes);
    _options.filter_policy = leveldb::NewBloomFilterPolicy(10);
    leveldb::Status status = leveldb::DB::Open(_options, _dataStoreDir, &_keyValueDb);

    // Verify that the LevelDB instance is setup properly
    assert(status.ok());  // TODO - Replace with custom exception
}

/**
 * Function used to get the current directory being used for the data-store
 *
 * @return String representing the current directory for the data-store
 */
std::string DataStore::getDataStoreDirectory()
{

    // Return the data-store directory
    return _dataStoreDir;
}

/**
 * Function used to add an item to the data-store
 *
 * @param key String representing the key for the item to add
 * @param item String item to add to the data store
 * @param overwrite Boolean indicating whether to overwrite the item if it exists
 * @return Boolean indicating whether the item was added or not
 */
bool DataStore::addItem(const std::string& key, const std::string& item, bool overwrite)
{

    // Lock the synchronous function mutex
    std::unique_lock<std::recursive_mutex> lock(_mutex);

    // Create a return flag
    bool wasAdded = false;

    // Only process if the key isn't empty
    if (!key.empty())
    {

        // Check if the item already exists
        std::string dummyValue;
        leveldb::Status s = _keyValueDb->Get(leveldb::ReadOptions(), key, &dummyValue);
        bool doesExist = s.ok();

        // Add the item to the key-value store if everything checks out
        if (!doesExist || overwrite)
        {

            // Create the write batch for atomic update
            leveldb::WriteBatch batch;

            // Delete the original item if it exists already
            if (doesExist)
                batch.Delete(key);

            // Add in the new write operation for the item
            batch.Put(key, item);

            // Commit the change to the database
            s = _keyValueDb->Write(leveldb::WriteOptions(), &batch);
            wasAdded = s.ok();
        }
    }

    // Return the return flag
    return wasAdded;
}

/**
 * Function used to get the value for the given key (or default if it doesn't exist)
 *
 * @param key String representing the key for the item to get
 * @param defaultValue String representing the default value to return if the item doesn't exist
 * @return String representing the value for the given key (or default if it doesn't exist)
 */
std::string DataStore::getItem(const std::string& key, const std::string& defaultValue)
{

    // Lock the synchronous function mutex
    std::unique_lock<std::recursive_mutex> lock(_mutex);

    // Create the return string/value
    std::string retValue = defaultValue;

    // Only process if the key isn't empty
    if (!key.empty())
    {

        // Attempt to read the item from the key-value store
        std::string dummyValue;
        leveldb::Status s = _keyValueDb->Get(leveldb::ReadOptions(), key, &dummyValue);
        bool doesExist = s.ok();

        // If the item was successfully read, set the return value accordingly
        if (doesExist)
            retValue = dummyValue;
    }

    // Return the return value
    return retValue;
}

/**
 * Function used to delete the given item from the key-value data-store
 *
 * @param key String representing the key for the item to delete
 * @return Boolean indicating whether the item was deleted or not
 */
bool DataStore::deleteItem(const std::string& key)
{

    // Lock the synchronous function mutex
    std::unique_lock<std::recursive_mutex> lock(_mutex);

    // Create a return flag
    bool wasDeleted = false;

    // Only process if the key isn't empty
    if (!key.empty())
    {

        // Check if the item already exists
        std::string dummyValue;
        leveldb::Status s = _keyValueDb->Get(leveldb::ReadOptions(), key, &dummyValue);
        bool doesExist = s.ok();

        // Delete the item from the key-value store if everything checks out
        if (doesExist)
        {

            // Create the write batch for atomic update
            leveldb::WriteBatch batch;

            // Delete the old key
            batch.Delete(key);

            // Commit the change to the database
            s = _keyValueDb->Write(leveldb::WriteOptions(), &batch);
            wasDeleted = s.ok();
        }
    }

    // Return the return flag
    return wasDeleted;
}

/**
 * Function used to delete the entire data-store directory
 *
 * @param reCreate Boolean indicating whether to recreate the data-store
 */
void DataStore::deleteEntireDataStore(bool reCreate)
{

    // Remove the data-store object
    delete _keyValueDb;
    _keyValueDb = nullptr;

    // Delete the data-store directory (if it exists)
    auto fileSystem = FileSystem(_dataStoreDir);
    if (fileSystem.exists() && fileSystem.isDirectory())
        fileSystem.removeDir();

    // Re-create the data-store if desired
    if (reCreate)
    {

        // First re-create the data-store directory
        fileSystem.createDir();

        // Next re-create the data-store object itself
        leveldb::Status status = leveldb::DB::Open(_options, _dataStoreDir, &_keyValueDb);
        assert(status.ok());  // TODO - Replace with custom exception
    }
}

/**
 * Function used to get a next-item iterator based on the given key
 * NOTE: The first item returned is the reference key, if it exists
 *
 * @param refKey String representing the key to use as reference for the next item
 * @return Generator representing the next-key iterator for the given key
 */
std::shared_ptr<Generator<std::string>> DataStore::getNextIterator(const std::string& refKey)
{

    // Create and return a generator for getting the chunked data
    auto keyValueDb = this->_keyValueDb;
    return std::make_shared<Generator<std::string>>(
            [keyValueDb, refKey]
            (std::shared_ptr<Yieldable<std::string>> yielder)
    {

        // Get a database iterator
        leveldb::ReadOptions options;
        options.fill_cache = false;
        std::shared_ptr<leveldb::Iterator> dbIter(keyValueDb->NewIterator(options));

        // Loop through all of the keys for the data-store starting at the reference one
        for (dbIter->Seek(refKey); dbIter->Valid(); dbIter->Next())
            yielder->yield(dbIter->key().ToString());

        // Complete the generator
        yielder->complete();
    });
}

/**
 * Function used to get a previous-item iterator based on the given key
 * NOTE: The first item returned is the reference key, if it exists
 *
 * @param refKey String representing the key to use as reference for the previous item
 * @return Generator representing the previous-key iterator for the given key
 */
std::shared_ptr<Generator<std::string>> DataStore::getPreviousIterator(const std::string& refKey)
{

    // Create and return a generator for getting the chunked data
    auto keyValueDb = this->_keyValueDb;
    return std::make_shared<Generator<std::string>>(
            [keyValueDb, refKey]
            (std::shared_ptr<Yieldable<std::string>> yielder)
    {

        // Get a database iterator
        leveldb::ReadOptions options;
        options.fill_cache = false;
        std::shared_ptr<leveldb::Iterator> dbIter(keyValueDb->NewIterator(options));

        // Loop through all of the keys for the data-store starting at the reference one
        // NOTE: This is done in reverse order to keep getting the previous keys
        for (dbIter->Seek(refKey); dbIter->Valid(); dbIter->Prev())
            yielder->yield(dbIter->key().ToString());

        // Complete the generator
        yielder->complete();
    });
}

/**
 * Function used to set the data-chunks representing the data-store's internal data
 * NOTE: This will overwrite any conflicting values already in the data-store
 *
 * @param chunkGenerator Generator representing the chunked data-store data
 * @return Boolean indicating whether the operation was successful or not
 */
bool DataStore::setChunkedData(const std::shared_ptr<Generator<std::string>>& chunkGenerator)
{

    // Create a return value
    bool retVal = true;

    // Loop over all chunks for the iterator while there are still some left
    while (chunkGenerator->hasMoreItems())
    {

        // Extract the generator's next item
        auto nextChunk = chunkGenerator->getNextItem();

        // Split the chunk up into parts based on the '~' character
        auto splitString = Utils::splitStringByDelimiter(nextChunk, "~");
        for (const auto& item : splitString)
        {

            // Only proceed if the item is non-empty
            if (!item.empty())
            {

                // Parse the file-string and extract the key-value pair
                auto packedVect = Utils::parseFileString(item);
                auto key = packedVect[0];
                auto value = packedVect[1];

                // Add the key-value pair to the data-store
                retVal &= addItem(key, value, true);
            }
        }
    }

    // Return the return value
    return retVal;
}

/**
 * Function used to get the data-chunks representing the data-store's internal data
 * NOTE: This can be used to re-create the data-store in the future
 *
 * @param chunkSizeInBytes Long representing the chunk size to get in bytes
 * @return Generator representing the chunked data-store data
 */
std::shared_ptr<Generator<std::string>> DataStore::getChunkedData(long chunkSizeInBytes)
{

    // Create and return a generator for getting the chunked data
    auto keyValueDb = this->_keyValueDb;
    return std::make_shared<Generator<std::string>>(
            [keyValueDb, chunkSizeInBytes]
            (std::shared_ptr<Yieldable<std::string>> yielder)
    {

        // Create an empty character array to hold the data
        long currentIndex = 0;
        char* dataChunk = new char[chunkSizeInBytes];

        // Get a database iterator
        leveldb::ReadOptions options;
        options.fill_cache = false;
        std::shared_ptr<leveldb::Iterator> dbIter(keyValueDb->NewIterator(options));

        // Loop through all of the keys for the data-store
        // extracting up to 1MB worth of data at a time
        for (dbIter->SeekToFirst(); dbIter->Valid(); dbIter->Next())
        {

            // Extract the key-value pair from the item
            std::vector<std::string> keyValuePair;
            keyValuePair.push_back(dbIter->key().ToString());
            keyValuePair.push_back(dbIter->value().ToString());

            // Create a file-string for the key-value pair
            auto fileString = Utils::getFileString(keyValuePair);

            // Check if the object is simply too big for our buffered chunk
            // in which case we will return it as-is
            bool hasBeenYielded = false;
            if (fileString.size() > chunkSizeInBytes)
            {
                yielder->yield(fileString);
                hasBeenYielded = true;
            }

            // If the current key-value pair won't fit, yield the chunk now
            else if ((currentIndex + fileString.size()) >= chunkSizeInBytes)
            {
                yielder->yield(std::string(dataChunk, currentIndex));
                currentIndex = 0;
            }

            // If the object hasn't been yielded, add it to the current chunk
            if (!hasBeenYielded)
            {
                for(char & character : fileString)
                    dataChunk[currentIndex++] = character;
                dataChunk[currentIndex++] = '~';
            }
        }

        // If we get the the end, and still have data in the chunk, yield it
        if (currentIndex > 0)
            yielder->yield(std::string(dataChunk, currentIndex));

        // Complete the generator
        yielder->complete();

        // Clear the dynamically allocated memory
        delete[] dataChunk;
    });
}

/**
 * Destructor used to cleanup the instance
 */
DataStore::~DataStore()
{

    // Delete the LevelDB instance pointer
    delete _keyValueDb;
    delete _options.block_cache;
    delete _options.filter_policy;
}

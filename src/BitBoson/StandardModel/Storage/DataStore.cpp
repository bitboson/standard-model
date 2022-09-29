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

#include <BitBoson/StandardModel/Utils/Utils.h>
#include <BitBoson/StandardModel/Storage/DataStore.h>
#include <BitBoson/StandardModel/FileSystem/FileSystem.h>

using namespace BitBoson::StandardModel;

/**
 * Constructor used to setup the data-store instance
 *
 * @param filePath String representing the full file path for
 *                 the key-value data store directory file
 * @param reCreate Boolean indicating whether to re-create the data-store or not
 *                 This will delete everything in the containing directory
 */
DataStore::DataStore(const std::string& dataDir, bool recreate)
{

    // Setup the instance on the provided directory
    _dataStoreDir = dataDir;
    _fileSystem = std::make_shared<FileSystem>(_dataStoreDir);
    if (recreate && _fileSystem->exists())
    {
        _fileSystem->removeDir();
        _fileSystem->createDir();
    }
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
        bool doesExist = _fileSystem->getChild(key).exists();

        // Add the item to the key-value store if everything checks out
        if (!doesExist || overwrite)
        {

            // Remove the existing file (if applicable)
            if (doesExist && overwrite)
                _fileSystem->getChild(key).removeFile();

            // Write the new file content to the disk
            wasAdded = _fileSystem->getChild(key).writeSimpleFile(item);
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
        bool doesExist = _fileSystem->getChild(key).exists();

        // If the item was successfully read, set the return value accordingly
        if (doesExist)
            retValue = _fileSystem->getChild(key).readSimpleFile();
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
        bool doesExist = _fileSystem->getChild(key).exists();

        // Delete the item from the key-value store if everything checks out
        if (doesExist)
            wasDeleted = _fileSystem->getChild(key).removeFile();
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

    // Delete the data-store directory (if it exists)
    if (_fileSystem->exists() && _fileSystem->isDirectory())
        _fileSystem->removeDir();

    // Re-create the data-store if desired
    if (reCreate)
        _fileSystem->createDir();
}

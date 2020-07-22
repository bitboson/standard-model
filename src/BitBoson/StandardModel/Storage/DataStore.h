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

#ifndef BITBOSON_STANDARDMODEL_DATASTORE_H
#define BITBOSON_STANDARDMODEL_DATASTORE_H

#include <string>
#include <mutex>
#include <memory>
#include <leveldb/db.h>
#include <BitBoson/StandardModel/Primitives/Generator.hpp>

namespace BitBoson::StandardModel
{

    class DataStore
    {

        // Public constants
        public:
            static const long DEFAULT_CACHE_SIZE;

        // Private member variables
        private:
            std::string _dataStoreDir;
            leveldb::DB* _keyValueDb;
            leveldb::Options _options;
            std::recursive_mutex _mutex;

        // Public member functions
        public:

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
            explicit DataStore(const std::string& dataDir, bool recreate=false,
                    long cacheSizeInBytes=DEFAULT_CACHE_SIZE);

            /**
             * Function used to get the current directory being used for the data-store
             *
             * @return String representing the current directory for the data-store
             */
            std::string getDataStoreDirectory();

            /**
             * Function used to add an item to the data-store
             *
             * @param key String representing the key for the item to add
             * @param item String item to add to the data store
             * @param overwrite Boolean indicating whether to overwrite the item if it exists
             * @return Boolean indicating whether the item was added or not
             */
            bool addItem(const std::string& key, const std::string& item, bool overwrite=false);

            /**
             * Function used to get the value for the given key (or default if it doesn't exist)
             *
             * @param key String representing the key for the item to get
             * @param defaultValue String representing the default value to return if the item doesn't exist
             * @return String representing the value for the given key (or default if it doesn't exist)
             */
            std::string getItem(const std::string& key, const std::string& defaultValue="");

            /**
             * Function used to delete the given item from the key-value data-store
             *
             * @param key String representing the key for the item to delete
             * @return Boolean indicating whether the item was deleted or not
             */
            bool deleteItem(const std::string& key);

            /**
             * Function used to delete the entire data-store directory
             *
             * @param reCreate Boolean indicating whether to recreate the data-store
             */
            void deleteEntireDataStore(bool reCreate=false);

            /**
             * Function used to set the data-chunks representing the data-store's internal data
             * NOTE: This will overwrite any conflicting values already in the data-store
             *
             * @param chunkGenerator Generator representing the chunked data-store data
             * @return Boolean indicating whether the operation was successful or not
             */
            bool setChunkedData(const std::shared_ptr<Generator<std::string>>& chunkGenerator);

            /**
             * Function used to get the data-chunks representing the data-store's internal data
             * NOTE: This can be used to re-create the data-store in the future
             *
             * @param chunkSizeInBytes Long representing the chunk size to get in bytes
             * @return Generator representing the chunked data-store data
             */
            std::shared_ptr<Generator<std::string>> getChunkedData(long chunkSizeInBytes=1000000);

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~DataStore();
    };
}

#endif //BITBOSON_STANDARDMODEL_DATASTORE_H

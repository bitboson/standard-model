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

#ifndef BITBOSON_STANDARDMODEL_DATASTORE_H
#define BITBOSON_STANDARDMODEL_DATASTORE_H

#include <string>
#include <mutex>
#include <memory>
#include <BitBoson/StandardModel/FileSystem/FileSystem.h>

namespace BitBoson::StandardModel
{

    class DataStore
    {

        // Private member variables
        private:
            std::string _dataStoreDir;
            std::recursive_mutex _mutex;
            std::shared_ptr<FileSystem> _fileSystem;

        // Public member functions
        public:

            /**
             * Constructor used to setup the data-store instance
             *
             * @param filePath String representing the full file path for
             *                 the key-value data store directory file
             * @param reCreate Boolean indicating whether to re-create the data-store or not
             *                 This will delete everything in the containing directory
             */
            explicit DataStore(const std::string& dataDir, bool recreate=false);

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
             * Destructor used to cleanup the instance
             */
            virtual ~DataStore() = default;
    };
}

#endif //BITBOSON_STANDARDMODEL_DATASTORE_H

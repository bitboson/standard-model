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

#ifndef BITBOSON_STANDARDMODEL_DISKCACHE_H
#define BITBOSON_STANDARDMODEL_DISKCACHE_H

#include <memory>
#include <BitBoson/StandardModel/Storage/DataStore.h>

namespace BitBoson::StandardModel
{

    class DiskCache
    {

        // Private member variables
        private:
            std::shared_ptr<DataStore> _dataStore;

        // Public member functions
        public:

            /**
             * Constructor used to setup the disk-cache instance
             *
             * @param cacheSizeInBytes Long representing the cache size (in bytes)
             * @param directory String representing the directory to store information in
             */
            explicit DiskCache(long cacheSizeInBytes=DataStore::DEFAULT_CACHE_SIZE,
                               const std::string& directory="");

            /**
             * Function used to get the current cache directory being used
             *
             * @return String representing the cache directory for the cache
             */
            std::string getCacheDirectory();

            /**
             * Function used to add an item to the disk-cache
             *
             * @param key String representing the key for the item to add
             * @param item String item to add to the data store
             * @return Boolean indicating whether the item was added or not
             */
            bool addItem(const std::string& key, const std::string& item);

            /**
             * Function used to get the value for the given key from the disk-cache
             *
             * @param key String representing the key for the item to get
             * @return String representing the value for the given key
             */
            std::string getItem(const std::string& key);

            /**
             * Function used to delete the given item from the key-value disk-cache
             *
             * @param key String representing the key for the item to delete
             * @return Boolean indicating whether the item was deleted or not
             */
            bool deleteItem(const std::string& key);

            /**
             * Destructor used to cleanup the cache entirely
             */
            virtual ~DiskCache();
    };
}

#endif //BITBOSON_STANDARDMODEL_DISKCACHE_H

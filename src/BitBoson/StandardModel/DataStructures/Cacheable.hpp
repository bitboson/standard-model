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

#ifndef BITBOSON_STANDARDMODEL_ICACHEABLE_HPP
#define BITBOSON_STANDARDMODEL_ICACHEABLE_HPP

#include <string>

namespace BitBoson::StandardModel
{

    class Cacheable
    {

        // Public member functions
        public:

            /**
             * Virtual function used to get the file-string-representation of the object
             *
             * @return String representing the cache-able data for the object
             */
            virtual std::string getFileString() const = 0;

            /**
             * Virtual function used to setup the object based on the given file-string-representation
             *
             * @param fileString String representing the cache-able data for the object to create from
             * @return Boolean indicating whether the given string was parsed and processed properly
             */
            virtual bool setFileString(const std::string& fileString) = 0;

            /**
             * Virtual function used to get the unique hash (SHA256) of the underlying object
             *
             * @return String representing the unique (SHA256) hash of the underlying object
             */
            virtual std::string getUniqueHash() const = 0;

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~Cacheable() = default;
    };
}

#endif //BITBOSON_STANDARDMODEL_ICACHEABLE_HPP

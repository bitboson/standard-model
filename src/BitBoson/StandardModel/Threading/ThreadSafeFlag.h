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

#ifndef BITBOSON_STANDARDMODEL_THREADSAFEFLAG_H
#define BITBOSON_STANDARDMODEL_THREADSAFEFLAG_H

#include <mutex>

namespace BitBoson::StandardModel
{

    class ThreadSafeFlag
    {

        // Private member variables
        private:
            bool _flagValue;
            std::mutex _lock;

        // Public member functions
        public:

            /**
             * Constructor used to setup the flag instance
             *
             * @param value Boolean indicating the value of the flag
             */
            explicit ThreadSafeFlag(bool value=false);

            /**
             * Function used to set the value of the flag instance
             *
             * @param value Boolean indicating the value of the flag
             */
            void setValue(bool value);

            /**
             * Function used to get the value of the flag instance
             *
             * @return Boolean indicating the value of the flag
             */
            bool getValue();

            /**
             * Destructor used to clean up the instance
             */
            virtual ~ThreadSafeFlag() = default;
    };
}

#endif //BITBOSON_STANDARDMODEL_THREADSAFEFLAG_H

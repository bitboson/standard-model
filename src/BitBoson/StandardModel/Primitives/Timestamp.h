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

#ifndef BITBOSON_STANDARDMODEL_TIMESTAMP_H
#define BITBOSON_STANDARDMODEL_TIMESTAMP_H

#include <BitBoson/StandardModel/Primitives/BigInt.hpp>

namespace BitBoson::StandardModel
{

    class Timestamp
    {

        // Private member variables
        private:
            BigInt _currTimestamp;

        // Public member functions
        public:

            /**
             * Constructor used to setup the timestamp instance
             *
             * @param timestampValue String Value used to initialize the timestamp
             */
            explicit Timestamp(const std::string& timestampValue="");

            /**
             * Function used to return the current timestamp value
             *
             * @return BigInt representing the current timestamp value
             */
            BigInt getCurrentValue() const;

            /**
             * Function used to convert the current internal timestamp value to a string
             *
             * @return String representation of the internally set timestamp value
             */
            std::string toString() const;

            /**
             * Static function used to get the current timestamp for the system
             *
             * @return Timestamp object representing the current timestamp value for the system
             */
            static Timestamp getCurrentTimestamp();

            /**
             * Function used to implement the less-than operator for timestamps
             *
             * @param rhs Timestamp representing the right-hand side to compare this instance to
             * @return Boolean indicating whether this instance is less than the right-hand-side one
             */
            bool operator<(const Timestamp& rhs) const;

            /**
             * Function used to implement the greater-than operator for timestamps
             *
             * @param rhs Timestamp representing the right-hand side to compare this instance to
             * @return Boolean indicating whether this instance is greater than the right-hand-side one
             */
            bool operator>(const Timestamp& rhs) const;

            /**
             * Function used to implement the less-than or equal-to operator for timestamps
             *
             * @param rhs Timestamp representing the right-hand side to compare this instance to
             * @return Boolean indicating whether this instance is less-than or equal-to the right-hand-side one
             */
            bool operator<=(const Timestamp& rhs) const;

            /**
             * Function used to implement the greater-than or equal-to operator for timestamps
             *
             * @param rhs Timestamp representing the right-hand side to compare this instance to
             * @return Boolean indicating whether this instance is greater-than or equal-to the right-hand-side one
             */
            bool operator>=(const Timestamp& rhs) const;

            /**
             * Function used to implement the equal-to operator for timestamps
             *
             * @param rhs Timestamp representing the right-hand side to compare this instance to
             * @return Boolean indicating whether this instance is equal-to right-hand-side one
             */
            bool operator==(const Timestamp& rhs) const;

            /**
             * Function used to implement the not-equal-to operator for timestamps
             *
             * @param rhs Timestamp representing the right-hand side to compare this instance to
             * @return Boolean indicating whether this instance is not-equal-to right-hand-side one
             */
            bool operator!=(const Timestamp& rhs) const;

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~Timestamp() = default;
    };
}

#endif //BITBOSON_STANDARDMODEL_TIMESTAMP_H

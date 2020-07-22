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

#include <chrono>
#include <BitBoson/StandardModel/Utils/Utils.h>
#include <BitBoson/StandardModel/Primitives/Timestamp.h>

using namespace BitBoson::StandardModel;

/**
 * Constructor used to setup the timestamp instance
 *
 * @param timestampValue String Value used to initialize the timestamp
 */
Timestamp::Timestamp(const std::string& timestampValue)
{

    // Setup the value of the timestamp based on the supplied one
    if (timestampValue.empty())
        _currTimestamp = 0;
    else
        _currTimestamp = BigInt{timestampValue};

    // Correct negative values if necessary
    if (_currTimestamp < 0)
        _currTimestamp = 0;
}

/**
 * Function used to return the current timestamp value
 *
 * @return BigInt representing the current timestamp value
 */
BigInt Timestamp::getCurrentValue() const
{

    // Return the current internal timestamp BigInt value
    return _currTimestamp;
}

/**
 * Function used to convert the current internal timestamp value to a string
 *
 * @return String representation of the internally set timestamp value
 */
std::string Timestamp::toString() const
{

    // Convert the internal BigInt representation to a string and return it
    return Utils::getBigIntString(_currTimestamp);
}

/**
 * Static function used to get the current timestamp for the system
 *
 * @return Timestamp object representing the current timestamp value for the system
 */
Timestamp Timestamp::getCurrentTimestamp()
{

    // Get the current time-stamp of the system (in milliseconds)
    std::chrono::milliseconds currTimestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    std::string currMillis = std::to_string(currTimestamp.count());

    // Convert the given string into a new timestamp and return it
    return Timestamp(currMillis);
}

/**
 * Function used to implement the less-than operator for timestamps
 *
 * @param rhs Timestamp representing the right-hand side to compare this instance to
 * @return Boolean indicating whether this instance is less than the right-hand-side one
 */
bool Timestamp::operator<(const Timestamp& rhs) const
{
    return _currTimestamp<rhs._currTimestamp;
}

/**
 * Function used to implement the greater-than operator for timestamps
 *
 * @param rhs Timestamp representing the right-hand side to compare this instance to
 * @return Boolean indicating whether this instance is greater than the right-hand-side one
 */
bool Timestamp::operator>(const Timestamp& rhs) const
{
    return rhs<*this;
}

/**
 * Function used to implement the less-than or equal-to operator for timestamps
 *
 * @param rhs Timestamp representing the right-hand side to compare this instance to
 * @return Boolean indicating whether this instance is less-than or equal-to the right-hand-side one
 */
bool Timestamp::operator<=(const Timestamp& rhs) const
{
    return !(rhs<*this);
}

/**
 * Function used to implement the greater-than or equal-to operator for timestamps
 *
 * @param rhs Timestamp representing the right-hand side to compare this instance to
 * @return Boolean indicating whether this instance is greater-than or equal-to the right-hand-side one
 */
bool Timestamp::operator>=(const Timestamp& rhs) const
{
    return !(*this<rhs);
}

/**
 * Function used to implement the equal-to operator for timestamps
 *
 * @param rhs Timestamp representing the right-hand side to compare this instance to
 * @return Boolean indicating whether this instance is equal-to right-hand-side one
 */
bool Timestamp::operator==(const Timestamp& rhs) const
{
    return _currTimestamp==rhs._currTimestamp;
}

/**
 * Function used to implement the not-equal-to operator for timestamps
 *
 * @param rhs Timestamp representing the right-hand side to compare this instance to
 * @return Boolean indicating whether this instance is not-equal-to right-hand-side one
 */
bool Timestamp::operator!=(const Timestamp& rhs) const
{
    return !(*this==rhs);
}

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

#include <BitBoson/StandardModel/Threading/ThreadSafeFlag.h>

using namespace BitBoson::StandardModel;

/**
 * Constructor used to setup the flag instance
 *
 * @param value Boolean indicating the value of the flag
 */
ThreadSafeFlag::ThreadSafeFlag(bool value)
{

    // Setup the initial value of the flag instance
    _flagValue = value;
}

/**
 * Function used to set the value of the flag instance
 *
 * @param value Boolean indicating the value of the flag
 */
void ThreadSafeFlag::setValue(bool value)
{

    // Lock the thread for safe operation
    std::unique_lock<std::mutex> lock(_lock);

    // Set the value of the flag instance
    _flagValue = value;
}

/**
 * Function used to get the value of the flag instance
 *
 * @return Boolean indicating the value of the flag
 */
bool ThreadSafeFlag::getValue()
{

    // Lock the thread for safe operation
    std::unique_lock<std::mutex> lock(_lock);

    // Get the value of the flag instance
    return _flagValue;
}

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

#ifndef BITBOSON_STANDARDMODEL_ASYNCEVENTLOOP_TEST_HPP
#define BITBOSON_STANDARDMODEL_ASYNCEVENTLOOP_TEST_HPP

#include <BitBoson/StandardModel/Threading/AsyncEventLoop.hpp>

using namespace BitBoson::StandardModel;

TEST_CASE ("Async Event Loop Test", "[AsyncEventLoopTest]")
{

    // Create a mutex for handling shared data
    std::mutex mutex;

    // Create a global count to represent the event loop being called
    volatile long globalCount = 0;

    // Create an async-event loop to add to the sum every second
    auto eventLoop = AsyncEventLoop(
        [&globalCount, &mutex]() {

            // Sleep for 1 second
            std::this_thread::sleep_for(std::chrono::seconds(1));

            // Update the global count (in a separate context)
            {

                // Lock the thread for safe operation
                std::unique_lock<std::mutex> lock(mutex);

                // Update the global count
                globalCount += 1;
            }
        });

    // Let the event loop run for 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Lock the thread for safe operation
    std::unique_lock<std::mutex> lock(mutex);

    // Validate that the sum is "close" to 10
    REQUIRE (globalCount > 0);
    REQUIRE (globalCount < 20);
}

#endif //BITBOSON_STANDARDMODEL_ASYNCEVENTLOOP_TEST_HPP

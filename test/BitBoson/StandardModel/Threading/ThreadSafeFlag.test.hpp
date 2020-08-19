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

#ifndef BITBOSON_STANDARDMODEL_THREADSAFEFLAG_TEST_HPP
#define BITBOSON_STANDARDMODEL_THREADSAFEFLAG_TEST_HPP

#include <thread>
#include <BitBoson/StandardModel/Threading/ThreadSafeFlag.h>

using namespace BitBoson::StandardModel;

TEST_CASE ("Stop Infinite Loop With Flag", "[ThreadSafeFlag]")
{

    // Create a thread-safe flag to manage an "inifinite" loop
    auto threadSafeFlag = ThreadSafeFlag(true);

    // Create an "infinite" loop in a background thread
    // Essentially the loop will run for a minute
    auto globalCount = 60;
    std::thread inifiniteLoopThread;
    inifiniteLoopThread = std::thread(
        [&threadSafeFlag, &globalCount]() {
            while ((globalCount > 0) && threadSafeFlag.getValue())
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                globalCount--;
            }
        });

    // Stop the loop after 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(10));
    threadSafeFlag.setValue(false);
    inifiniteLoopThread.join();

    // Verify that the global count is less than 60 and greater than 0
    REQUIRE(globalCount < 60);
    REQUIRE(globalCount > 0);
}

#endif //BITBOSON_STANDARDMODEL_THREADSAFEFLAG_TEST_HPP

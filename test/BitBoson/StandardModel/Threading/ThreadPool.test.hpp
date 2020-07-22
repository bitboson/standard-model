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

#ifndef BITBOSON_STANDARDMODEL_THREADPOOL_TEST_HPP
#define BITBOSON_STANDARDMODEL_THREADPOOL_TEST_HPP

#include <BitBoson/StandardModel/Threading/ThreadPool.hpp>

using namespace BitBoson::StandardModel;

TEST_CASE("Single-Threaded Integer Sum", "[ThreadPoolTest]")
{

    // Create a global count to verify the sum later
    long globalCount = 0;

    // Create a thread pool with only one thread
    auto threadPool = ThreadPool<int>(
        [&globalCount](std::shared_ptr<int> val) {
            globalCount += *val;
        }, 1);

    // Enqueue values to add to the global sum
    for (int ii = 0; ii < 1000; ii++)
        threadPool.enqueue(std::make_shared<int>(ii));

    // Wait for the thread pool to stop processing data
    while (!threadPool.isQueueEmpty())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    // Verify that the global count comes out as expected
    REQUIRE(globalCount == 499500);
}

TEST_CASE("Multi-Threaded Integer Sum", "[ThreadPoolTest]")
{

    // Create a global count to verify the sum later
    long globalCount = 0;

    // Add-in a mutex to protext the global sum
    std::mutex mutex;

    // Create a thread pool with only one thread
    auto threadPool = ThreadPool<int>(
        [&globalCount, &mutex](std::shared_ptr<int> val) {
            std::unique_lock<std::mutex> lock(mutex);
            globalCount += *val;
        }, 10);

    // Enqueue values to add to the global sum
    for (int ii = 0; ii < 1000; ii++)
        threadPool.enqueue(std::make_shared<int>(ii));

    // Wait for the thread pool to stop processing data
    while (!threadPool.isQueueEmpty())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    // Verify that the global count comes out as expected
    REQUIRE(globalCount == 499500);
}

TEST_CASE("Default-Threaded Integer Sum", "[ThreadPoolTest]")
{

    // Create a global count to verify the sum later
    long globalCount = 0;

    // Add-in a mutex to protext the global sum
    std::mutex mutex;

    // Create a thread pool with only one thread
    auto threadPool = ThreadPool<int>(
        [&globalCount, &mutex](std::shared_ptr<int> val) {
            std::unique_lock<std::mutex> lock(mutex);
            globalCount += *val;
        });

    // Enqueue values to add to the global sum
    for (int ii = 0; ii < 1000; ii++)
        threadPool.enqueue(std::make_shared<int>(ii));

    // Wait for the thread pool to stop processing data
    while (!threadPool.isQueueEmpty())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    // Verify that the global count comes out as expected
    REQUIRE(globalCount == 499500);
}

TEST_CASE("Single-Threaded String Concatenation", "[ThreadPoolTest]")
{

    // Create a global count to verify the sum later
    std::string globalString;

    // Create a thread pool with only one thread
    auto threadPool = ThreadPool<std::string>(
        [&globalString](std::shared_ptr<std::string> val) {
            globalString += *val;
        }, 1);

    // Enqueue values to add to the global sum
    for (int ii = 0; ii < 1000; ii++)
        threadPool.enqueue(std::make_shared<std::string>(std::to_string(ii)));

    // Wait for the thread pool to stop processing data
    while (!threadPool.isQueueEmpty())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    // Verify that the global count comes out as expected
    REQUIRE(globalString.length() == 2890);
}

TEST_CASE("Multi-Threaded String Concatenation", "[ThreadPoolTest]")
{

    // Create a global count to verify the sum later
    std::string globalString;

    // Add-in a mutex to protext the global sum
    std::mutex mutex;

    // Create a thread pool with only one thread
    auto threadPool = ThreadPool<std::string>(
        [&globalString, &mutex](std::shared_ptr<std::string> val) {
            std::unique_lock<std::mutex> lock(mutex);
            globalString += *val;
        }, 10);

    // Enqueue values to add to the global sum
    for (int ii = 0; ii < 1000; ii++)
        threadPool.enqueue(std::make_shared<std::string>(std::to_string(ii)));

    // Wait for the thread pool to stop processing data
    while (!threadPool.isQueueEmpty())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    // Verify that the global count comes out as expected
    REQUIRE(globalString.length() == 2890);
}

TEST_CASE("Default-Threaded String Concatenation", "[ThreadPoolTest]")
{

    // Create a global count to verify the sum later
    std::string globalString;

    // Add-in a mutex to protext the global sum
    std::mutex mutex;

    // Create a thread pool with only one thread
    auto threadPool = ThreadPool<std::string>(
        [&globalString, &mutex](std::shared_ptr<std::string> val) {
            std::unique_lock<std::mutex> lock(mutex);
            globalString += *val;
        });

    // Enqueue values to add to the global sum
    for (int ii = 0; ii < 1000; ii++)
        threadPool.enqueue(std::make_shared<std::string>(std::to_string(ii)));

    // Wait for the thread pool to stop processing data
    while (!threadPool.isQueueEmpty())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    // Verify that the global count comes out as expected
    REQUIRE(globalString.length() == 2890);
}

#endif //BITBOSON_STANDARDMODEL_THREADPOOL_TEST_HPP

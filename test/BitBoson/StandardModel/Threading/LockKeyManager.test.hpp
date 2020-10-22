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

#ifndef BITBOSON_STANDARDMODEL_LOCKKEYMANAGER_TEST_HPP
#define BITBOSON_STANDARDMODEL_LOCKKEYMANAGER_TEST_HPP

#include <BitBoson/StandardModel/Threading/ThreadPool.hpp>
#include <BitBoson/StandardModel/Threading/LockKeyManager.hpp>

using namespace BitBoson::StandardModel;

TEST_CASE ("General Lock-Key Manager Test", "[LockKeyManagerTest]")
{

    // Create a global count to verify the sum later
    long globalCount = 0;

    // Create a thread pool with several threads
    auto threadPool = ThreadPool<int>(
        [&globalCount](std::shared_ptr<int> val) {

            // Use the Lock-Key Manager to protect the global count
            auto lock = LockKeyManager::getLock("globalCount");
            std::cout << *val << std::endl;

            // Update the global count value
            globalCount += *val;

            // Unlock/release the lock
            lock->unlock();
        }, 10);

    // Enqueue values to add to the global sum
    for (int ii = 0; ii < 10000; ii++)
        threadPool.enqueue(std::make_shared<int>(ii));

    // Wait for the thread pool to stop processing data
    while (!threadPool.isQueueEmpty())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    // Verify that the global count comes out as expected
    std::this_thread::sleep_for(std::chrono::seconds(2));
    REQUIRE(globalCount == 49995000);
}

TEST_CASE ("Forced-No-Collission General Lock-Key Manager Test", "[LockKeyManagerTest]")
{

    // Create a global count to verify the sum later
    long globalCount = 0;

    // Create a thread pool with several threads
    auto threadPool = ThreadPool<int>(
        [&globalCount](std::shared_ptr<int> val) {

            // Use the Lock-Key Manager to protect the global count
            auto lock = LockKeyManager::getLock("globalCount");

            // Update the global count value
            globalCount += *val;

            // Unlock/release the lock
            lock->unlock();
        }, 10);

    // Enqueue values to add to the global sum
    for (int ii = 0; ii < 1000; ii++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        threadPool.enqueue(std::make_shared<int>(ii));
    }

    // Wait for the thread pool to stop processing data
    while (!threadPool.isQueueEmpty())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    // Verify that the global count comes out as expected
    std::this_thread::sleep_for(std::chrono::seconds(2));
    REQUIRE(globalCount == 499500);
}

TEST_CASE ("Forced-Collission General Lock-Key Manager Test", "[LockKeyManagerTest]")
{

    // Create a global count to verify the sum later
    long globalCount = 0;

    // Create a thread pool with several threads
    auto threadPool = ThreadPool<int>(
        [&globalCount](std::shared_ptr<int> val) {

            // Use the Lock-Key Manager to protect the global count
            auto lock = LockKeyManager::getLock("globalCount");
            std::cout << *val << std::endl;

            // Ensure the thread sleeps to force wait conditions
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Update the global count value
            globalCount += *val;

            // Unlock/release the lock
            lock->unlock();
        }, 10);

    // Enqueue values to add to the global sum
    for (int ii = 0; ii < 10000; ii++)
        threadPool.enqueue(std::make_shared<int>(ii));

    // Wait for the thread pool to stop processing data
    while (!threadPool.isQueueEmpty())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    // Verify that the global count comes out as expected
    std::this_thread::sleep_for(std::chrono::seconds(2));
    REQUIRE(globalCount == 49995000);
}

TEST_CASE ("Multi-Lock General Lock-Key Manager Test", "[LockKeyManagerTest]")
{

    // Create a global count to verify the sum later
    long globalCount1 = 0;
    long globalCount2 = 0;
    long globalCount3 = 0;
    long globalCount4 = 0;

    // Create a thread pool with several threads
    auto threadPool = ThreadPool<int>(
        [&globalCount1, &globalCount2, &globalCount3, &globalCount4]
        (std::shared_ptr<int> val) {

            // Use the Lock-Key Manager to protect the global count
            auto currCount = ((*val) % 4);
            auto lock = LockKeyManager::getLock("globalCount" + std::to_string(currCount));
            std::cout << *val << std::endl;

            // Update the global count value
            if (currCount == 0)
                globalCount1 += 1;
            if (currCount == 1)
                globalCount2 += 1;
            if (currCount == 2)
                globalCount3 += 1;
            if (currCount == 3)
                globalCount4 += 1;

            // Unlock/release the lock
            lock->unlock();
        }, 10);

    // Enqueue values to add to the global sum
    for (int ii = 0; ii < 10000; ii++)
        threadPool.enqueue(std::make_shared<int>(ii));

    // Wait for the thread pool to stop processing data
    while (!threadPool.isQueueEmpty())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    // Verify that the global count comes out as expected
    std::this_thread::sleep_for(std::chrono::seconds(2));
    REQUIRE(globalCount1 == 2500);
    REQUIRE(globalCount2 == 2500);
    REQUIRE(globalCount3 == 2500);
    REQUIRE(globalCount4 == 2500);
}

TEST_CASE ("Multi-Lock Forced-No-Collission General Lock-Key Manager Test", "[LockKeyManagerTest]")
{

    // Create a global count to verify the sum later
    long globalCount1 = 0;
    long globalCount2 = 0;
    long globalCount3 = 0;
    long globalCount4 = 0;

    // Create a thread pool with several threads
    auto threadPool = ThreadPool<int>(
        [&globalCount1, &globalCount2, &globalCount3, &globalCount4]
        (std::shared_ptr<int> val) {

            // Use the Lock-Key Manager to protect the global count
            auto currCount = ((*val) % 4);
            auto lock = LockKeyManager::getLock("globalCount" + std::to_string(currCount));

            // Update the global count value
            if (currCount == 0)
                globalCount1 += 1;
            if (currCount == 1)
                globalCount2 += 1;
            if (currCount == 2)
                globalCount3 += 1;
            if (currCount == 3)
                globalCount4 += 1;

            // Unlock/release the lock
            lock->unlock();
        }, 10);

    // Enqueue values to add to the global sum
    for (int ii = 0; ii < 1000; ii+=4)
    {
        threadPool.enqueue(std::make_shared<int>(ii));
        threadPool.enqueue(std::make_shared<int>(ii + 1));
        threadPool.enqueue(std::make_shared<int>(ii + 2));
        threadPool.enqueue(std::make_shared<int>(ii + 3));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Wait for the thread pool to stop processing data
    while (!threadPool.isQueueEmpty())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    // Verify that the global count comes out as expected
    std::this_thread::sleep_for(std::chrono::seconds(2));
    REQUIRE(globalCount1 == 250);
    REQUIRE(globalCount2 == 250);
    REQUIRE(globalCount3 == 250);
    REQUIRE(globalCount4 == 250);
}

TEST_CASE ("Forced-Collission Multi-Lock General Lock-Key Manager Test", "[LockKeyManagerTest]")
{

    // Create a global count to verify the sum later
    long globalCount1 = 0;
    long globalCount2 = 0;
    long globalCount3 = 0;
    long globalCount4 = 0;

    // Create a thread pool with several threads
    auto threadPool = ThreadPool<int>(
        [&globalCount1, &globalCount2, &globalCount3, &globalCount4]
        (std::shared_ptr<int> val) {

            // Use the Lock-Key Manager to protect the global count
            auto currCount = ((*val) % 4);
            auto lock = LockKeyManager::getLock("globalCount" + std::to_string(currCount));
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << *val << std::endl;

            // Update the global count value
            if (currCount == 0)
                globalCount1 += 1;
            if (currCount == 1)
                globalCount2 += 1;
            if (currCount == 2)
                globalCount3 += 1;
            if (currCount == 3)
                globalCount4 += 1;

            // Unlock/release the lock
            lock->unlock();
        }, 10);

    // Enqueue values to add to the global sum
    for (int ii = 0; ii < 10000; ii++)
        threadPool.enqueue(std::make_shared<int>(ii));

    // Wait for the thread pool to stop processing data
    while (!threadPool.isQueueEmpty())
        std::this_thread::sleep_for(std::chrono::seconds(1));

    // Verify that the global count comes out as expected
    std::this_thread::sleep_for(std::chrono::seconds(2));
    REQUIRE(globalCount1 == 2500);
    REQUIRE(globalCount2 == 2500);
    REQUIRE(globalCount3 == 2500);
    REQUIRE(globalCount4 == 2500);
}

#endif //BITBOSON_STANDARDMODEL_LOCKKEYMANAGER_TEST_HPP

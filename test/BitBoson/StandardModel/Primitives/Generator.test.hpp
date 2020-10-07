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

#ifndef BITBOSON_STANDARDMODEL_GENERATOR_TEST_HPP
#define BITBOSON_STANDARDMODEL_GENERATOR_TEST_HPP

#include <BitBoson/StandardModel/Primitives/Generator.hpp>

using namespace BitBoson::StandardModel;

TEST_CASE ("Quit Remaining Items Test", "[GeneratorTest]")
{

    // Setup a typical generator
    auto generator = std::make_shared<Generator<int>>([](std::shared_ptr<Yieldable<int>> yielder){
        for (int ii = 0; ii < 10000; ii++)
        {

            // Create a slow generator
            yielder->yield(ii);

            // If the item is done pre-maturely, exit the loop
            if (yielder->isTerminated())
                break;
        }
    });

    // Execute the generator process for only the first chunk of items
    int sum = 0;
    while (generator->hasMoreItems() && (sum < 500))
        sum += generator->getNextItem();

    // Quit the remaining items in the generator
    generator->quitRemainingItems();

    // Verify the results
    REQUIRE(sum == 528);
}

TEST_CASE ("Quit Remaining Items Destructor Test", "[GeneratorTest]")
{

    // Setup a typical generator
    auto generator = std::make_shared<Generator<int>>([](std::shared_ptr<Yieldable<int>> yielder){
        for (int ii = 0; ii < 10000; ii++)
        {

            // Create a slow generator
            yielder->yield(ii);

            // If the item is done pre-maturely, exit the loop
            if (yielder->isTerminated())
                break;
        }
    });

    // Execute the generator process for only the first chunk of items
    int sum = 0;
    while (generator->hasMoreItems() && (sum < 500))
        sum += generator->getNextItem();

    // Force the cleanup (destruction) of the generator and yielder
    // Which means items will be cleaned up
    generator = nullptr;

    // Verify the results
    REQUIRE(sum == 528);
}

TEST_CASE ("Quit with Guranteed Reamaining Yielded Item Test", "[GeneratorTest]")
{

    // Setup a typical generator
    auto generator = std::make_shared<Generator<int>>([](std::shared_ptr<Yieldable<int>> yielder){
        for (int ii = 0; ii < 10000; ii++)
        {

            // Create a slow generator
            yielder->yield(ii);

            // If the item is done pre-maturely, exit the loop
            if (yielder->isTerminated())
                break;
        }
    });

    // Execute the generator process for only the first chunk of items
    int sum = 0;
    while (generator->hasMoreItems() && (sum < 500))
        sum += generator->getNextItem();

    // Wait a few seconds to guarantee an item is "yielded"
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Quit the remaining items in the generator
    generator->quitRemainingItems();

    // Verify the results
    REQUIRE(sum == 528);
}

TEST_CASE ("Quit with Guranteed No Reamaining Yielded Item Test", "[GeneratorTest]")
{

    // Setup a typical (slow) generator
    auto generator = std::make_shared<Generator<int>>([](std::shared_ptr<Yieldable<int>> yielder){
        for (int ii = 0; ii < 10000; ii++)
        {

            // Create a slow generator
            std::this_thread::sleep_for(std::chrono::seconds(2));
            yielder->yield(ii);

            // If the item is done pre-maturely, exit the loop
            if (yielder->isTerminated())
                break;
        }
    });

    // Execute the generator process for only the first chunk of items
    int sum = 0;
    while (generator->hasMoreItems() && (sum < 500))
        sum += generator->getNextItem();

    // Quit the remaining items in the generator
    // Since it's a slow generator, not item will be "yielded" yet
    generator->quitRemainingItems();

    // Verify the results
    REQUIRE(sum == 528);
}

TEST_CASE ("General Generator Use Test", "[GeneratorTest]")
{

    // Setup a typical generator
    auto generator = std::make_shared<Generator<int>>([](std::shared_ptr<Yieldable<int>> yielder){
        for (int ii = 0; ii < 10000; ii++)
        {
            yielder->yield(ii);
            if (yielder->isTerminated())
                break;
        }
        yielder->complete();
    });

    // Execute the generator process
    int sum = 0;
    while (generator->hasMoreItems())
        sum += generator->getNextItem();

    // Verify the results
    REQUIRE(sum == 49995000);
}

TEST_CASE ("Quick Yields Generator Use Test", "[GeneratorTest]")
{

    // Setup a typical generator
    auto generator = std::make_shared<Generator<int>>([](std::shared_ptr<Yieldable<int>> yielder){
        for (int ii = 0; ii < 20; ii++)
            yielder->yield(ii);
        yielder->complete();
    });

    // Execute the generator process (in a very delayed fashion)
    int sum = 0;
    while (generator->hasMoreItems())
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        sum += generator->getNextItem();
    }

    // Verify the results
    REQUIRE(sum == 190);
}

TEST_CASE ("Slow Yields Generator Use Test", "[GeneratorTest]")
{

    // Setup a typical generator (with delayed yields)
    auto generator = std::make_shared<Generator<int>>([](std::shared_ptr<Yieldable<int>> yielder){
        for (int ii = 0; ii < 20; ii++)
        {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            yielder->yield(ii);
        }
        yielder->complete();
    });

    // Execute the generator process
    int sum = 0;
    while (generator->hasMoreItems())
        sum += generator->getNextItem();

    // Verify the results
    REQUIRE(sum == 190);
}

TEST_CASE ("Delayed-Complete Generator Use Test", "[GeneratorTest]")
{

    // Setup a typical generator with a delay after yields but before complete
    auto generator = std::make_shared<Generator<int>>([](std::shared_ptr<Yieldable<int>> yielder){
        for (int ii = 0; ii < 10000; ii++)
            yielder->yield(ii);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        yielder->complete();
    });

    // Execute the generator process
    int sum = 0;
    while (generator->hasMoreItems())
        sum += generator->getNextItem();

    // Verify the results
    REQUIRE(sum == 49995000);
}

TEST_CASE ("Delayed-Start Generator Use Test", "[GeneratorTest]")
{

    // Setup a typical generator
    auto generator = std::make_shared<Generator<int>>([](std::shared_ptr<Yieldable<int>> yielder){
        for (int ii = 0; ii < 10000; ii++)
            yielder->yield(ii);
        yielder->complete();
    });

    // Delay the start of the generator gets
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Execute the generator process
    int sum = 0;
    while (generator->hasMoreItems())
        sum += generator->getNextItem();

    // Verify the results
    REQUIRE(sum == 49995000);
}

TEST_CASE ("Zero-Item Generator Use Test", "[GeneratorTest]")
{

    // Setup a typical generator with no items yielded
    auto generator = std::make_shared<Generator<int>>([](std::shared_ptr<Yieldable<int>> yielder){
        std::this_thread::sleep_for(std::chrono::seconds(5));
        yielder->complete();
    });

    // Execute the generator process
    int sum = 0;
    while (generator->hasMoreItems())
        sum += generator->getNextItem();

    // Verify the results
    REQUIRE(sum == 0);
}

TEST_CASE ("Single-Item Generator Use Test", "[GeneratorTest]")
{

    // Setup a typical generator with one item yielded
    auto generator = std::make_shared<Generator<int>>([](std::shared_ptr<Yieldable<int>> yielder){
        yielder->yield(1);
        yielder->complete();
    });

    // Execute the generator process
    int sum = 0;
    while (generator->hasMoreItems())
        sum += generator->getNextItem();

    // Verify the results
    REQUIRE(sum == 1);
}

TEST_CASE ("Double-Item Generator Use Test", "[GeneratorTest]")
{

    // Setup a typical generator with two items yielded
    auto generator = std::make_shared<Generator<int>>([](std::shared_ptr<Yieldable<int>> yielder){
        yielder->yield(1);
        yielder->yield(2);
        yielder->complete();
    });

    // Execute the generator process
    int sum = 0;
    while (generator->hasMoreItems())
        sum += generator->getNextItem();

    // Verify the results
    REQUIRE(sum == 3);
}

#endif //BITBOSON_STANDARDMODEL_GENERATOR_TEST_HPP

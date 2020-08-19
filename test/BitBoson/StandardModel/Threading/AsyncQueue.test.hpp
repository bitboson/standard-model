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

#ifndef BITBOSON_STANDARDMODEL_ASYNCQUEUE_TEST_HPP
#define BITBOSON_STANDARDMODEL_ASYNCQUEUE_TEST_HPP

#include <BitBoson/StandardModel/Threading/AsyncQueue.hpp>

using namespace BitBoson::StandardModel;

class PriorityItem : public IComparable
{
    private:
        double _value;
    public:
        explicit PriorityItem(double value)
        {
            _value = value;
        }
        double getComparableValue() const override
        {
            return _value;
        }
        virtual ~PriorityItem() = default;
};

TEST_CASE ("Standard Queue Test", "[AsyncQueueTest]")
{

    // Create a new queue
    auto queue = std::make_shared<AsyncQueue<std::string>>();

    // Add some items to the queue
    queue->enqueue("Hello");
    queue->enqueue("There");
    queue->enqueue("World");
    queue->enqueue("How");
    queue->enqueue("Are");
    queue->enqueue("You");
    queue->enqueue("Doing");

    // Verify the Queue's emptiness and dequeue order
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 7);
    REQUIRE(queue->dequeue() == "Hello");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 6);
    REQUIRE(queue->dequeue() == "There");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 5);
    REQUIRE(queue->dequeue() == "World");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 4);
    REQUIRE(queue->dequeue() == "How");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 3);
    REQUIRE(queue->dequeue() == "Are");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 2);
    REQUIRE(queue->dequeue() == "You");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 1);
    REQUIRE(queue->dequeue() == "Doing");
    REQUIRE(queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 0);
}

TEST_CASE ("Destruct Non-Empty Queue Test", "[AsyncQueueTest]")
{

    // Create a new queue
    auto queue = std::make_shared<AsyncQueue<std::string>>();

    // Add some items to the queue
    queue->enqueue("Hello");
    queue->enqueue("There");
    queue->enqueue("World");
    queue->enqueue("How");
    queue->enqueue("Are");
    queue->enqueue("You");
    queue->enqueue("Doing");

    // Verify the Queue's emptiness and dequeue order
    // but do not dequeue everything
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 7);
    REQUIRE(queue->dequeue() == "Hello");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 6);
    REQUIRE(queue->dequeue() == "There");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 5);
    REQUIRE(queue->dequeue() == "World");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 4);
    REQUIRE(queue->dequeue() == "How");
    REQUIRE(!queue->isQueueEmpty());
}

TEST_CASE ("Priority Queue Test", "[AsyncQueueTest]")
{

    // Create a new queue
    auto queue = std::make_shared<AsyncQueue<std::string>>();

    // Add some items to the queue
    queue->enqueue("Hello", std::make_shared<PriorityItem>(1));
    queue->enqueue("There", std::make_shared<PriorityItem>(1));
    queue->enqueue("World", std::make_shared<PriorityItem>(10));
    queue->enqueue("How", std::make_shared<PriorityItem>(7));
    queue->enqueue("Are", std::make_shared<PriorityItem>(5));
    queue->enqueue("You", std::make_shared<PriorityItem>(8));
    queue->enqueue("Doing", std::make_shared<PriorityItem>(5));

    // Verify the Queue's emptiness and dequeue order
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 7);
    REQUIRE(queue->dequeue() == "World");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 6);
    REQUIRE(queue->dequeue() == "You");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 5);
    REQUIRE(queue->dequeue() == "How");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 4);
    REQUIRE(queue->dequeue() == "Are");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 3);
    REQUIRE(queue->dequeue() == "Doing");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 2);
    REQUIRE(queue->dequeue() == "Hello");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 1);
    REQUIRE(queue->dequeue() == "There");
    REQUIRE(queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 0);
}

TEST_CASE ("Mixed Priority Queue Test", "[AsyncQueueTest]")
{

    // Create a new queue
    auto queue = std::make_shared<AsyncQueue<std::string>>();

    // Add some items to the queue
    queue->enqueue("Hello");
    queue->enqueue("There");
    queue->enqueue("World", std::make_shared<PriorityItem>(10));
    queue->enqueue("How", std::make_shared<PriorityItem>(7));
    queue->enqueue("Are", std::make_shared<PriorityItem>(5));
    queue->enqueue("You");
    queue->enqueue("Doing", std::make_shared<PriorityItem>(5));

    // Verify the Queue's emptiness and dequeue order
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 7);
    REQUIRE(queue->dequeue() == "World");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 6);
    REQUIRE(queue->dequeue() == "How");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 5);
    REQUIRE(queue->dequeue() == "Are");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 4);
    REQUIRE(queue->dequeue() == "Doing");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 3);
    REQUIRE(queue->dequeue() == "Hello");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 2);
    REQUIRE(queue->dequeue() == "There");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 1);
    REQUIRE(queue->dequeue() == "You");
    REQUIRE(queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 0);
}

TEST_CASE ("Standard Windowed Queue Test", "[AsyncQueueTest]")
{

    // Create a new queue
    auto queue = std::make_shared<AsyncQueue<std::string>>(5);

    // Add some items to the queue
    queue->enqueue("Hello");
    queue->enqueue("There");
    queue->enqueue("World");
    queue->enqueue("How");
    queue->enqueue("Are");
    queue->enqueue("You"); // Won't be added
    queue->enqueue("Doing"); // Won't be added

    // Verify the Queue's emptiness and dequeue order
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 5);
    REQUIRE(queue->dequeue() == "Hello");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 4);
    REQUIRE(queue->dequeue() == "There");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 3);
    REQUIRE(queue->dequeue() == "World");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 2);
    REQUIRE(queue->dequeue() == "How");
    REQUIRE(!queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 1);
    REQUIRE(queue->dequeue() == "Are");
    REQUIRE(queue->isQueueEmpty());
    REQUIRE(queue->getQueueSize() == 0);
}

#endif //BITBOSON_STANDARDMODEL_ASYNCQUEUE_TEST_HPP

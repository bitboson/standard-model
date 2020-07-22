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

#ifndef BITBOSON_STANDARDMODEL_ASYNCQUEUE_H
#define BITBOSON_STANDARDMODEL_ASYNCQUEUE_H

#include <mutex>
#include <queue>
#include <memory>
#include <BitBoson/StandardModel/Primitives/BigInt.hpp>

namespace BitBoson::StandardModel
{

    class IComparable
    {

        // Public member functions
        public:

            /**
             * Virtual function used to define the value to be used in comparisons
             *
             * @return Double representing the comparable value for the interface
             */
            virtual double getComparableValue() const = 0;
    };

    template <class T> class AsyncQueue
    {

        // Private member variables
        private:
            unsigned int _queueSize;
            std::mutex _lock;
            std::vector<std::pair<T, std::shared_ptr<IComparable>>> _queue;

        // Public member functions
        public:

            /**
             * Constructor used to setup the instance
             *
             * @param queueSize Unsigned Integer representing the max queue size
             *                  Keeps higher priority items (or oldest if all null)
             *                  A Queue Size of zero (0) means there is no size-constraint
             */
            explicit AsyncQueue(unsigned int queueSize = 0)
            {

                // Setup the queue size
                _queueSize = queueSize;
            }

            /**
             * Function used to enqueue data into the thread-safe queue
             *
             * @param data Data reference to enqueue
             * @param priority IComparable reference representing the item's priority
             */
            void enqueue(T data, std::shared_ptr<IComparable> priority = nullptr)
            {

                // Lock the thread for safe enqueue
                std::unique_lock<std::mutex> lock(_lock);

                // Enqueue the data
                _queue.emplace_back(data, priority);

                // Sort the vector based on the priorities upon insertion
                std::sort (_queue.begin(), _queue.end(), isPriorityGreaterThan);

                // Remove all items that are outside of the windowed area
                if ((_queueSize > 0) && (_queue.size() > _queueSize))
                    for (auto ii = _queueSize; ii < _queue.size(); ii++)
                        _queue.pop_back();
            }

            /**
             * Function used to dequeue data from the thread-safe queue
             *
             * @return Data reference at the end of the thread-safe queue
             *         Returns nullptr if the queue is empty
             */
            T dequeue()
            {

                // Lock the thread for safe dequeue
                std::unique_lock<std::mutex> lock(_lock);

                // Create the return value
                T retVal;

                // Verify the queue isn't empty
                if (!_queue.empty())
                {

                    // Get the front item (highest priority)
                    retVal = _queue.front().first;

                    // Delete the front entry from the queue and free up the priority
                    //delete _queue.front().second;
                    _queue.erase(_queue.begin());
                }

                // Return the return value
                return retVal;
            }

            /**
             * Function used to check if the queue is currently empty
             *
             * @return Boolean indicating if the queue is empty
             */
            bool isQueueEmpty()
            {

                // Lock the thread for safe operations
                std::unique_lock<std::mutex> lock(_lock);

                // Return if the queue is empty or not
                return _queue.empty();
            }

            /**
             * Function used to get the size of the queue
             *
             * @return Integer representing the size of the queue
             */
            int getQueueSize()
            {

                // Lock the thread for safe operations
                std::unique_lock<std::mutex> lock(_lock);

                // Return the queue size
                return _queue.size();
            }

            /**
             * Function used to flush the data in the queue
             */
            void flushQueue()
            {

                // Lock the thread for safe enqueue
                std::unique_lock<std::mutex> lock(_lock);

                // Flush all of the data in the queue
                _queue.clear();
            }

            /**
             * Destructor used to cleanup the queue instance
             */
            virtual ~AsyncQueue()
            {

                // Empty the queue and remove all priorities
                while (!isQueueEmpty())
                    dequeue();
            }

        // Private member functions
        private:

            /**
             * Internal static function used to compare priorities
             * It is capable of comparing nulls and non-nulls as well
             *
             * @param item1 IComparable item representing the lhs of the comparison
             * @param item2 IComparable item representing the rhs of the comparison
             * @return Boolean indicating if the lhs is greater then the lhs
             */
            static bool isPriorityGreaterThan(std::pair<T, std::shared_ptr<IComparable>> item1,
                    std::pair<T, std::shared_ptr<IComparable>> item2)
            {
                if ((item1.second == nullptr) && (item2.second == nullptr))
                    return false;
                if (item1.second == nullptr)
                    return false;
                if (item2.second == nullptr)
                    return true;
                return item1.second->getComparableValue() > item2.second->getComparableValue();
            }
    };
}

#endif //BITBOSON_STANDARDMODEL_ASYNCQUEUE_H

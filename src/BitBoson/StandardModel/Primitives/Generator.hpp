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

#ifndef BITBOSON_STANDARDMODEL_GENERATOR_H
#define BITBOSON_STANDARDMODEL_GENERATOR_H

#include <mutex>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>

namespace BitBoson::StandardModel
{

    template <class T> class Generator;

    template <class T> class Yieldable
    {

        // Friend the generator class
        friend class Generator<T>;

        // Private member functions
        private:
            bool _isItemDone;
            bool _isTerminated;
            bool _hasYieldedOnce;
            std::queue<T> _queue;
            std::mutex _mutex;
            std::condition_variable _yieldConditional;
            std::condition_variable _getConditional;

        // Public member functions
        public:

            Yieldable()
            {

                // Initialize state variables
                _isItemDone = false;
                _hasYieldedOnce = false;
                _isTerminated = false;
            }

            void yield(T currentItem)
            {

                // Lock the current function call
                std::unique_lock<std::mutex> lock(_mutex);

                // Indicate that we have yielded at least once
                if (!_hasYieldedOnce)
                    _hasYieldedOnce = true;

                // Only continue the call if we haven't called complete yet
                if (!_isItemDone)
                {

                    // Set the current item (critical section)
                    _queue.push(currentItem);

                    // Flip control back to the getting portion
                    // Only wait if the queue has two items in it
                    if (_queue.size() >= 2)
                    {
                        _getConditional.notify_one();
                        _yieldConditional.wait(lock);
                    }
                }
            }

            /**
             * Function used to check whether the Yieldable
             * has terminated
             *
             * @return Boolean indicating whether the Yieldable
             *         has terminated
             */
            bool isTerminated()
            {

                // Lock the current function call
                std::unique_lock<std::mutex> lock(_mutex);

                // Return the status of the yieldable
                return _isItemDone;
            }

            void complete()
            {

                // Lock the current function call
                std::unique_lock<std::mutex> lock(_mutex);

                // Only handle the complete if we didn't
                // already do so (ensure it's called once)
                if (!_isItemDone)
                {

                    // Indicate that no more items are pending
                    _isItemDone = true;

                    // Notify both waiting portions to complete
                    lock.unlock();
                    _getConditional.notify_all();
                    _yieldConditional.notify_all();
                }
            }

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~Yieldable()
            {

                // Ensure that the yieldable is completed
                complete();
            }

        // Protected member functions
        protected:

            bool isItemDone()
            {

                // Lock the current function call
                std::unique_lock<std::mutex> lock(_mutex);

                // We must wait for a yield or complete if the
                // internal queue is currently empty
                if (!_isItemDone && _queue.empty())
                    _getConditional.wait(lock);

                // Return whether the yieldable has any more values
                return _isItemDone && _queue.empty();
            }

            T getCurrentItem()
            {

                // Create a return value
                T retVal = T();

                // Lock the mutex for the current function call
                std::unique_lock<std::mutex> lock(_mutex);

                // Only proceed if the yieldable has more items
                if (!_isItemDone || !_queue.empty())
                {

                    // Flip control back to the yielding portion
                    // NOTE: Only do this when we are still yielding items
                    if (!_isItemDone && (_queue.size() <= 2))
                    {
                        _yieldConditional.notify_one();
                        _getConditional.wait(lock);
                    }

                    // Extract the current item from the critical section
                    retVal = _queue.front();
                    _queue.pop();
                }

                // Return the return value
                return retVal;
            }
    };

    template <class T> class Generator
    {

        // Private member variables
        private:
            bool _hasAskedIfMoreItems;
            bool _previousItemAskAnswer;
            std::thread _thread;
            std::shared_ptr<Yieldable<T>> _yieldable;

        // Public member functions
        public:

            explicit Generator(std::function<void (std::shared_ptr<Yieldable<T>>)> generatorFunction)
            {

                // Setup the internal yieldable thread process
                _yieldable = std::make_shared<Yieldable<T>>();
                _thread = std::thread(generatorFunction, _yieldable);

                // Setup status/state variables for the generator
                _hasAskedIfMoreItems = false;
                _previousItemAskAnswer = false;
            }

            bool hasMoreItems()
            {

                // Create a return flag based on the previous answer
                bool retFlag = _previousItemAskAnswer;

                // Only continue if we haven't queried without a state change
                // In which case we will actually request the status
                if (!_hasAskedIfMoreItems)
                    retFlag = !_yieldable->isItemDone();

                // Indicate that we have recently queried the status
                // and save the response of such query
                _hasAskedIfMoreItems = true;
                _previousItemAskAnswer = retFlag;

                // Return the return flag
                return retFlag;
            }

            T getNextItem()
            {

                // Indicate that there has been a state change and
                // that new queries on item status can be made
                _hasAskedIfMoreItems = false;

                // Get and return the next item of the yielder
                return _yieldable->getCurrentItem();
            }

            /**
             * Function used to quit the remaining items
             */
            void quitRemainingItems()
            {

                // Simply call "complete" on the yieldable
                _yieldable->complete();
            }

            virtual ~Generator()
            {

                // Quit any remaining items
                quitRemainingItems();

                // Wait for the thread to exit
                _thread.join();
            }
    };
}

#endif //BITBOSON_STANDARDMODEL_GENERATOR_H

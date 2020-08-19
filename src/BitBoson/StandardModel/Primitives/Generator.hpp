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

            void complete()
            {

                // Lock the current function call
                std::unique_lock<std::mutex> lock(_mutex);

                // Indicate that no more items are pending
                _isItemDone = true;

                // Notify both waiting portions to complete
                lock.unlock();
                _getConditional.notify_all();
                _yieldConditional.notify_all();
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
            std::thread _thread;
            std::shared_ptr<Yieldable<T>> _yieldable;

        // Public member functions
        public:

            explicit Generator(std::function<void (std::shared_ptr<Yieldable<T>>)> generatorFunction)
            {
                _yieldable = std::make_shared<Yieldable<T>>();
                _thread = std::thread(generatorFunction, _yieldable);
            }

            bool hasMoreItems()
            {

                // Get and return whether the yielder has more items
                return !_yieldable->isItemDone();
            }

            T getNextItem()
            {

                // Get and return the next item of the yielder
                return _yieldable->getCurrentItem();
            }

            virtual ~Generator()
            {

                // Wait for the thread to exit
                _thread.join();
            }
    };
}

#endif //BITBOSON_STANDARDMODEL_GENERATOR_H

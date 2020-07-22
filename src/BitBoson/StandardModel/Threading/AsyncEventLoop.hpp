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

#ifndef BITBOSON_STANDARDMODEL_ASYNCEVENTLOOP_H
#define BITBOSON_STANDARDMODEL_ASYNCEVENTLOOP_H

#include <memory>
#include <BitBoson/StandardModel/Threading/ThreadPool.hpp>
#include <BitBoson/StandardModel/Threading/ThreadSafeFlag.h>

namespace BitBoson::StandardModel
{

    class AsyncEventLoop
    {

        // Private member variables
        private:
            std::function<void ()> _eventLoop;
            std::shared_ptr<ThreadSafeFlag> _isRunning;
            std::shared_ptr<ThreadPool<int>> _threadPool;

        // Public member functions
        public:

            /**
             * Constructor used to setup the instance
             */
            AsyncEventLoop(std::function<void ()> eventLoop)
            {

                // Setup the thread-safe flag as running, since the
                // default state is to run the event loop
                _isRunning = std::make_shared<ThreadSafeFlag>();
                _isRunning->setValue(true);

                // Setup the thread-pool with a single worker thread
                _eventLoop = eventLoop;
                _threadPool = std::make_shared<ThreadPool<int>>(
                        std::bind(&AsyncEventLoop::internalEventLoop, this, std::placeholders::_1), 1);
                _threadPool->enqueue(std::make_shared<int>(1));
            }

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~AsyncEventLoop()
            {

                // Stop the event loop by setting the flag
                _isRunning->setValue(false);
            }

        // Private member functions
        private:

            /**
             * Internal function used to run the event loop
             *
             * @param throwaway Integer representing an unused value
             */
            void internalEventLoop(std::shared_ptr<int> throwaway)
            {

                // Run a while-loop until we receive the signal to stop
                // continuously running the callback
                while (_isRunning->getValue())
                    _eventLoop();
            }
    };
}

#endif //BITBOSON_STANDARDMODEL_ASYNCEVENTLOOP_H

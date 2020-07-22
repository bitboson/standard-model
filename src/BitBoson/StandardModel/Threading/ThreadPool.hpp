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

#ifndef BITBOSON_STANDARDMODEL_THREADPOOL_H
#define BITBOSON_STANDARDMODEL_THREADPOOL_H

#include <thread>
#include <vector>
#include <chrono>
#include <signal.h>
#include <pthread.h>
#include <functional>
#include <BitBoson/StandardModel/Threading/AsyncQueue.hpp>
#include <BitBoson/StandardModel/Threading/ThreadSafeFlag.h>

namespace BitBoson::StandardModel
{

    template <class T> class ThreadPool : public AsyncQueue<std::shared_ptr<T>>
    {

        // Private member variables
        private:
            std::mutex _lock;
            std::vector<pthread_t> _threadPool;
            std::shared_ptr<ThreadSafeFlag> _isRunning;
            std::function<void (std::shared_ptr<T>)> _callback;

        // Public member functions
        public:

            /**
             * Constructor used to setup the thread-pool instance
             *
             * @param callback Function used to execute thread-pool jobs on
             * @param threadCount Integer representing the number of threads to use
             */
            ThreadPool(std::function<void (std::shared_ptr<T>)> callback, int threadCount=0)
            {

                // Lock the thread for safe operation
                std::unique_lock<std::mutex> lock(_lock);

                // Setup the is-running flag as true
                _isRunning = std::make_shared<ThreadSafeFlag>();
                _isRunning->setValue(true);

                // Save the callback for later user
                _callback = callback;

                // Determine the number of threads to use
                unsigned int numThreads = 4;
                unsigned int hardwareConcurrency = std::thread::hardware_concurrency();
                if (threadCount > 0)
                    numThreads = threadCount;
                else if (hardwareConcurrency > 0)
                    numThreads = hardwareConcurrency;

                // Setup the thread pool for processing background tasks
                sigset_t omask, mask;
                sigfillset(&mask);
                pthread_sigmask(SIG_SETMASK, &mask, &omask);
                for (unsigned int ii = 0; ii < numThreads; ii++)
                {
                    pthread_t pThread;
                    pthread_create(&pThread, NULL, (void* (*)(void*)) &ThreadPool::processEventLoop, static_cast<void*>(this));
                    _threadPool.push_back(pThread);
                }
            }

            /**
             * Destructor used to clean up the instance
             */
            virtual ~ThreadPool()
            {

                // Indicate that the processes should stop
                _isRunning->setValue(false);

                // Join all of the running threads
                void* result;
                for (const auto& threadItem : _threadPool)
                    pthread_join(threadItem, &result);
            }

        // Private member functions
        private:

            /**
             * Internal function used to get the next item from the queue
             * NOTE: Returns nullptr if there are no items currently
             *
             * @return T Data reference for the next item in the queue
             */
            std::shared_ptr<T> getNextQueueItemIfExists()
            {

                // Lock the thread for safe operation
                std::unique_lock<std::mutex> lock(_lock);

                // Create a return value
                std::shared_ptr<T> retVal = nullptr;

                // Get the next item (if it exists)
                if (!this->isQueueEmpty())
                    retVal = this->dequeue();

                // Return the return value
                return retVal;
            }

            /**
             * Internal function used to safely call the setup callback
             *
             * @param dataToUse T Data reference to provide to the callback
             */
            void safelyCallCallback(std::shared_ptr<T> dataToUse)
            {

                // Lock the thread for safe operation
                std::unique_lock<std::mutex> lock(_lock);

                // Safely call the callback now (due to mutex)
                _callback(dataToUse);
            }

            /**
             * Internal static function used to run the processing event-loop
             */
            static void processEventLoop(void* instancePtr)
            {

                // Continuously process events as long as the
                // thread pool is still running
                auto instance = static_cast<ThreadPool<T>*>(instancePtr);
                while (instance->_isRunning->getValue())
                {

                    // If the queue has items in it, get the next one
                    auto nextQueueItem = instance->getNextQueueItemIfExists();

                    // If the item is not null, execute the callback
                    // otherwise wait a short while before trying again
                    if (nextQueueItem != nullptr)
                        instance->safelyCallCallback(nextQueueItem);
                    else
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }

    };
}

#endif //BITBOSON_STANDARDMODEL_THREADPOOL_H

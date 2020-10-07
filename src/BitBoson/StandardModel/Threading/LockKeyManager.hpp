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

#ifndef BITBOSON_STANDARDMODEL_LOCKKEYMANAGER_HPP
#define BITBOSON_STANDARDMODEL_LOCKKEYMANAGER_HPP

#include <mutex>
#include <memory>
#include <string>

namespace BitBoson::StandardModel
{

    class LockKeyManager
    {

        // Friend the Lock class
        friend class Lock;

        // Private member class
        private:

            class Lock
            {

                // Private member variables
                private:
                    bool _isLocked;
                    std::mutex _mutex;
                    std::string _resource;

                // Public member functions
                public:

                    /**
                     * Constructor used to setup the instance
                     *
                     * @param resource String representing the resource
                     */
                    Lock(const std::string& resource)
                    {

                        // Lock the internally stored mutex
                        _mutex.lock();
                        _isLocked = true;
                        _resource = resource;
                    }

                    /**
                     * Function used to unlock the lock (if it was locked)
                     */
                    void unlock()
                    {

                        // If the lock is still locked then unlock it
                        if (_isLocked)
                        {

                            // Mark the lock as unlocked
                            _isLocked = false;

                            // Actually unlock the mutex itself
                            _mutex.unlock();

                            // Inform the singleton LockKeyManager that
                            // the resource has been free for cleanup
                            getInstance().informUnlocked(_resource);
                        }
                    }

                    /**
                     * Function used to block the caller until the lock
                     * represented by this instance is released
                     */
                    void wait()
                    {

                        // Attempt to get the lock to force blocking
                        _mutex.lock();
                        _isLocked = true;
                    }

                    /**
                     * Destructor used to cleanup the instance
                     */
                    virtual ~Lock() = default;
            };

        // Private member variables
        private:
            std::shared_ptr<std::mutex> _threadSafeMutex;
            std::unordered_map<std::string, int> _numWaiting;
            std::unordered_map<std::string, std::shared_ptr<Lock>> _locks;

        // Public member functions
        public:

            // Delete functions contrary to Singleton use
            LockKeyManager(LockKeyManager const&) = delete;
            void operator=(LockKeyManager const&) = delete;

            /**
             * Static function used to get a lock for the provided key
             *
             * @param resource String representing the resource key to lock
             * @return Lock representing the lock on the provided key to manage
             */
            static std::shared_ptr<Lock> getLock(const std::string& resource)
            {

                // Lock the synchronous function/instance mutex
                getInstance().getMutex()->lock();

                // Determine if the resource is already locked or not
                bool isLocked = getInstance().isLocked(resource);

                // If the resource is already locked indicate we are waiting for it
                // otherwise setup the wait-list count as zero since we'll create it
                if (isLocked)
                    getInstance().incrementWaiting(resource);
                else
                    getInstance().zeroizeWaitingCount(resource);

                // If the lock already exists then we'll need to wait until it
                // is unlocked before proceeding so we'll do that here
                if (isLocked)
                {

                    // Since the resource is locked and we've already updated
                    // the wait-list count we can safely unlock this function
                    // call and start waiting on the resource itself
                    auto currLock = getInstance().getLockObject(resource);
                    getInstance().getMutex()->unlock();
                    currLock->wait();

                    // Once we've obtained the lock on the object itself we
                    // can re-gain the lock for this function call to modify
                    // the wait-list count since we are no longer waiting
                    getInstance().getMutex()->lock();
                    getInstance().decrementWaiting(resource);
                }

                // If the lock didn't previously exist we can simply create it
                if (!isLocked)
                    getInstance().initializeLockObject(resource);

                // Return the lock resource which now holds the lock instance
                auto retValue = getInstance().getLockObject(resource);
                getInstance().getMutex()->unlock();
                return retValue;
            }

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~LockKeyManager() = default;

        // Protected member functions
        protected:

            /**
             * Internal static function used to inform the instance that the
             * corresponding lock object has been unl0cked and should be
             * cleaned-up if no other instance is currently waiting on it
             *
             * @param resource String representing the unlocked resource
             */
            static void informUnlocked(const std::string& resource)
            {

                // Lock the synchronous function/instance mutex
                getInstance().getMutex()->lock();

                // If no one is currently waiting for the locked
                // resource we can safely delete it locally
                if (getInstance().getWaitingCount(resource) == 0)
                {

                    // Remove both the wait-list count and lock
                    getInstance().deleteResources(resource);
                }

                // Unlock the synchronous function/instance mutex
                getInstance().getMutex()->unlock();
            }

        // Private member functions
        private:

            /**
             * Hidden onstructor used to setup the instance
             */
            LockKeyManager()
            {

                // Initialize any required member variables
                _threadSafeMutex = std::make_shared<std::mutex>();
            }

            /**
             * Static singleton accessor function used to get the instance
             *
             * @return Singleton instance of the object/class
             */
            static LockKeyManager& getInstance()
            {

                // Create the instance statically
                static LockKeyManager instance;

                // Return the newly created instance
                return instance;
            }

            /**
             * Internal function used to get the mutex for the instance
             *
             * @return Mutex for the singleton instance
             */
            std::shared_ptr<std::mutex> getMutex()
            {

                // Simply return the mutex pointer for the instance
                return _threadSafeMutex;
            }

            /**
             * Internal function used to get whether the given resource is
             * currently locked by another thread or not
             *
             * @param resource String representing the resource
             * @return Boolean indicating if the resource is locked
             */
            bool isLocked(const std::string& resource)
            {

                // Get and return if the resource is locked or not
                return (_locks.find(resource) != _locks.end());
            }

            /**
             * Internal function used to increment the waiting count for
             * the given resource
             *
             * @param resource String representing the resource
             */
            void incrementWaiting(const std::string& resource)
            {

                // Simply increment the resource count
                _numWaiting[resource]++;
            }

            /**
             * Internal function used to decrement the waiting count for
             * the given resource
             *
             * @param resource String representing the resource
             */
            void decrementWaiting(const std::string& resource)
            {

                // Simply decrement the resource count
                _numWaiting[resource]--;
            }

            /**
             * Internal function used to zero-ize the waiting count for
             * the given resource
             *
             * @param resource String representing the resource
             */
            void zeroizeWaitingCount(const std::string& resource)
            {

                // Simply zero-ize the resource count
                _numWaiting[resource] = 0;
            }

            /**
             * Internal function used to get the current waiting count
             * for the given resource
             *
             * @param resource String representing the resource
             * @return Integer representing the waiting count
             */
            int getWaitingCount(const std::string& resource)
            {

                // Simply get and return the resource count
                return _numWaiting[resource];
            }

            /**
             * Internal function used to get the current lock-object
             * for the given resource
             *
             * @param resource String representing the resource
             * @return Lock pointer representing the lock
             */
            std::shared_ptr<Lock> getLockObject(const std::string& resource)
            {

                // Simply get and return the resource lock
                return _locks[resource];
            }

            /**
             * Internal function used to create the specified lock-object
             * for the given resource
             *
             * @param resource String representing the resource
             * @return Lock pointer representing the lock
             */
            std::shared_ptr<Lock> initializeLockObject(const std::string& resource)
            {

                // Simply create and return the resource count
                _locks[resource] = std::make_shared<Lock>(resource);
                return _locks[resource];
            }

            /**
             * Internal function used to delete the waiting count and lock
             * object references for the given resource
             *
             * @param resource String representing the resource
             */
            void deleteResources(const std::string& resource)
            {

                // Remove both the wait-list count and lock
                _numWaiting.erase(resource);
                _locks.erase(resource);
            }
    };
}

#endif //BITBOSON_STANDARDMODEL_LOCKKEYMANAGER_HPP

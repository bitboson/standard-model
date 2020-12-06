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

#ifndef BITBOSON_STANDARDMODEL_DISKNODE_HPP
#define BITBOSON_STANDARDMODEL_DISKNODE_HPP

#include <string>
#include <memory>
#include <boost/lexical_cast.hpp>
#include <BitBoson/StandardModel/Utils/Utils.h>
#include <BitBoson/StandardModel/Storage/DiskCache.h>
#include <BitBoson/StandardModel/DataStructures/Containers/BaseNode.hpp>

namespace BitBoson::StandardModel
{

    template <class T> class DiskNode : public BaseNode<T>
    {

        // Public sub-classes
        public:
            class DiskNodeAllocator : public BinarySearchTree<T, DiskNode>::Allocator
            {

                // Private member variables
                private:
                    std::shared_ptr<DiskCache> _internalDiskCache;

                // Public functions
                public:

                    /**
                     * Constructor used to setup the allocator and its underlying disk-cache
                     *
                     * @param cacheSizeInBytes Long representing the cache size (in bytes)
                     * @param directory String representing the directory to store information in
                     */
                    explicit DiskNodeAllocator(long cacheSizeInBytes=DataStore::DEFAULT_CACHE_SIZE,
                                      const std::string& directory="")
                    {

                        // Setup the disk-cache instance with the provided arguments
                        _internalDiskCache = std::make_shared<DiskCache>(cacheSizeInBytes, directory);
                    }

                    /**
                     * Function used to get the underlying disk-cache reference
                     * NOTE: Since this is a reference (pointer) you can manipulate
                     *       cache settings here or even replace the cache completely
                     *
                     * @return Disk-Cache reference for the underlying disk-nodes
                     */
                    std::shared_ptr<DiskCache> getDiskCacheReference()
                    {

                        // Return the disk-cache reference
                        return _internalDiskCache;
                    }

                    /**
                     * Overridden function used to allocate the given node
                     *
                     * @return Shared Pointer reference to the allocated node
                     */
                    std::shared_ptr<DiskNode<T>> allocateNode() override
                    {

                        // Create a return value
                        std::shared_ptr<DiskNode> diskNode;

                        // Create a disk node with initialized disk-cache
                        diskNode = std::make_shared<DiskNode<T>>();
                        diskNode->setInternalDiskCache(_internalDiskCache);

                        // Return the newly allocated disk node
                        return diskNode;
                    }

                    /**
                     * Destructor used to cleanup the instance
                     */
                    virtual ~DiskNodeAllocator() = default;
            };

        // Private member variables
        private:
            bool _isLoadingNode;
            std::string _leftChild;
            std::string _rightChild;
            std::shared_ptr<DiskCache> _internalDiskCache;

        // Public member functions
        public:

            /**
             * Constructor used to setup the Disk Node instance
             */
            DiskNode() : BaseNode<T>::BaseNode()
            {

                // Setup the default values
                _isLoadingNode = false;
                _leftChild = "";
                _rightChild = "";
                _internalDiskCache = nullptr;
            }

            /**
             * Function used to set the internal disk cache for use in the node
             *
             * @param internalDiskCache Disk Cache for node use in loading/saving data/children
             */
            void setInternalDiskCache(std::shared_ptr<DiskCache> internalDiskCache)
            {

                // Set the internal disk cache
                _internalDiskCache = std::move(internalDiskCache);
            }

            /**
             * Function used to force the construction of this node from a reference
             * value of a node found in the Singleton Node Cache
             * NOTE: This performs no checking or cleanup and is intended for use on an
             *       empty, un-used (or new) node reference
             *
             * @param valueToReference Generic (T) Value to load the node information from
             */
            void overrideLoadFromCache(T valueToReference)
            {

                // Get the referenced node from the cache
                auto refNode = loadDiskNode(getStringFromTemplateArg(valueToReference));

                // Only continue if the reference node is not null
                if (refNode != nullptr)
                {

                    // Extract the children of the referenced node
                    auto leftChild = refNode->getLeftChild();
                    auto rightChild = refNode->getRightChild();

                    // Get the children data
                    auto leftData = (leftChild != nullptr) ?
                            getStringFromTemplateArg(leftChild->getData()) : "";
                    auto rightData = (rightChild != nullptr) ?
                                      getStringFromTemplateArg(rightChild->getData()) : "";

                    // Load/set the referenced node information to this instance
                    _isLoadingNode = true;
                    _leftChild = leftData;
                    _rightChild = rightData;
                    this->setHeight(refNode->getHeight());
                    this->setData(refNode->getData());
                    _isLoadingNode = false;
                }
            }

            /**
             * Overridden function used to set the Node's data
             *
             * @param data Generic Data (T) representing the Node's data
             */
            void setData(T data) override
            {

                // Call the superclass method
                BaseNode<T>::setData(data);

                // Save node to the cache
                if (!_isLoadingNode)
                    saveDiskNode();
            }

            /**
             * Overridden function used to set the Node's left child
             *
             * @param leftChild Shared Pointer representing the Node's left child
             */
            void setLeftChild(std::shared_ptr<BaseNode<T>> leftChild) override
            {

                // Call the superclass method
                BaseNode<T>::setLeftChild(leftChild);

                // Store value of the left-child as the next-item "pointer"
                if (leftChild != nullptr)
                    _leftChild = getStringFromTemplateArg(leftChild->getData());
                else
                    _leftChild = "";

                // Save node to the cache
                if (!_isLoadingNode)
                    saveDiskNode();
            }

            /**
             * Overridden function used to get the Node's left child
             *
             * @return Shared Pointer representing the Node's left child
             */
            std::shared_ptr<BaseNode<T>> getLeftChild() override
            {

                // Get and return the Disk Node for the left-child
                return loadDiskNode(_leftChild);
            }

            /**
             * Overridden function used to get the Node's right child
             *
             * @param rightChild Shared Pointer representing the Node's right child
             */
            void setRightChild(std::shared_ptr<BaseNode<T>> rightChild) override
            {

                // Call the superclass method
                BaseNode<T>::setRightChild(rightChild);

                // Store value of the right-child as the next-item "pointer"
                if (rightChild != nullptr)
                    _rightChild = getStringFromTemplateArg(rightChild->getData());
                else
                    _rightChild = "";

                // Save node to the cache
                if (!_isLoadingNode)
                    saveDiskNode();
            }

            /**
             * Overridden function used to get the Node's right child
             *
             * @return Shared Pointer representing the Node's right child
             */
            std::shared_ptr<BaseNode<T>> getRightChild() override
            {

                // Get and return the Disk Node for the right-child
                return loadDiskNode(_rightChild);
            }

            /**
             * Overridden function used to delete the underlying Node and its data
             */
            void deleteNode() override
            {
                _internalDiskCache->deleteItem(getStringFromTemplateArg(this->getData()));
            };

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~DiskNode() = default;

        // Private member functions
        private:

            /**
             * Internal function used to get a string from the provided template argument
             *
             * @param templateArg Generic (T) template argument to convert
             * @return String representing the Boost-Lexical-Cast of the template argument
             */
            static std::string getStringFromTemplateArg(T templateArg)
            {

                // Use Boost to lexically-case the template argument to a string
                return boost::lexical_cast<std::string>(templateArg);
            }

            /**
             * Internal function used to get a template argument from a string
             *
             * @param stringArg String representing the lexically-cases template argument
             * @return Generic (T) template argument representing the lexically-un-casted value
             */
            static T getTemplateArgFromString(const std::string& stringArg)
            {

                // Use Boost to lexically-case the string to a template argument
                return boost::lexical_cast<T>(stringArg);
            }

            /**
             * Internal function used to get Disk Node for the given node address
             *
             * @param nodeAddress String representing the unique address of the node
             * @return Shared Pointer representing the Disk Node at the given address
             */
            std::shared_ptr<DiskNode<T>> loadDiskNode(const std::string& nodeAddress)
            {

                // Create the Disk Node to return
                std::shared_ptr<DiskNode<T>> retNode = nullptr;

                // Only continue if the provided address isn't empty
                if (!nodeAddress.empty())
                {

                    // Get the node data (and children) from the cache
                    auto nodeData = _internalDiskCache->getItem(nodeAddress);
                    if (!nodeData.empty())
                    {

                        // Extract the packed vector of data in the file-string
                        auto packedVect = Utils::parseFileString(nodeData);
                        if ((packedVect != nullptr) && (packedVect->size >= 4))
                        {

                            // Build up the Disk Node from the cache data
                            retNode = std::make_shared<DiskNode<T>>();
                            retNode->setInternalDiskCache(_internalDiskCache);
                            retNode->_isLoadingNode = true;
                            retNode->setData(getTemplateArgFromString(Utils::getNextFileStringValue(packedVect)));
                            retNode->setHeight(std::stol(Utils::getNextFileStringValue(packedVect)));
                            retNode->_leftChild = Utils::getNextFileStringValue(packedVect);
                            retNode->_rightChild = Utils::getNextFileStringValue(packedVect);
                            retNode->_isLoadingNode = false;
                        }
                    }
                }

                // Return the Disk Node
                return retNode;
            }

            /**
             * Internal function used to save this (self) Disk ode instance to the cache
             */
            void saveDiskNode()
            {

                // Extract the data from the provided node (if not null)
                std::string providedData = getStringFromTemplateArg(this->getData());

                // Only continue if the node isn't null or has empty data
                if (!providedData.empty())
                {

                    // Extract both children from the supplied node
                    auto leftChild = this->getLeftChild();
                    auto rightChild = this->getRightChild();

                    // Extract the packed-vector form of the Disk Node
                    std::vector<std::string> packedVect;
                    packedVect.push_back(getStringFromTemplateArg(this->getData()));
                    packedVect.push_back(std::to_string(this->getHeight()));
                    packedVect.push_back(_leftChild);
                    packedVect.push_back(_rightChild);

                    // Convert the packed-vector to a file-string and write it to the cache
                    auto fileString = Utils::getFileString(packedVect);
                    _internalDiskCache->addItem(getStringFromTemplateArg(this->getData()), fileString);
                }
            }
    };
}

#endif //BITBOSON_STANDARDMODEL_DISKNODE_HPP

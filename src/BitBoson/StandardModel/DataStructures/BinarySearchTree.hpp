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

#ifndef BITBOSON_STANDARDMODEL_BINARYSEARCHTREE_HPP
#define BITBOSON_STANDARDMODEL_BINARYSEARCHTREE_HPP

#include <stack>
#include <memory>
#include <boost/lexical_cast.hpp>
#include <BitBoson/StandardModel/DataStructures/Containers/BaseNode.hpp>

namespace BitBoson::StandardModel
{

    template <class T, template <class> class S> class BinarySearchTree
    {

        // Public sub-classes
        public:
            class Allocator
            {

                // Public functions
                public:

                    /**
                     * Virtual function used to allocate the given node
                     *
                     * @return Shared Pointer reference to the allocated node
                     */
                    virtual std::shared_ptr<S<T>> allocateNode()
                    {

                        // Return the default shared-pointer allocation
                        return std::make_shared<S<T>>();
                    }

                    /**
                     * Destructor used to cleanup the instance
                     */
                    virtual ~Allocator() = default;
            };

        // Private member variables
        private:
            std::shared_ptr<Allocator> _allocator;
            std::shared_ptr<BaseNode<T>> _rootNode;

        // Public member functions
        public:

            /**
             * Constructor used to setup the default tree instance
             */
            BinarySearchTree()
            {

                // Setup the default values
                _rootNode = nullptr;
                _allocator = std::make_shared<Allocator>();
            }

            /**
             * Function used to setup (override) the default allocator for the tree
             *
             * @param allocator Allocator reference to replace the default on with
             */
            void overrideDefaultAllocator(std::shared_ptr<Allocator> allocator)
            {

                // Set the new (overriding) node allocator
                _allocator = allocator;
            }

            /**
             * Function used to override the existing tree with the given root node
             * NOTE: This does NOT perform any checks or cleanup, so only use with
             *       a new (empty) tree and ensure the given root is valid
             *
             * @param rootNode Base Node representing the new root node to use
             */
            void overrideSetRootNode(std::shared_ptr<BaseNode<T>> rootNode)
            {

                // Set the new root node (no questions asked)
                _rootNode = rootNode;
            }

            /**
             * Function used to get the root element of the tree
             *
             * @return Generic Data (T) representing the root element
             */
            T getRootElement()
            {

                // Create a return value
                T  retVal = T();

                // Get the root element if the root is not null
                if (_rootNode != nullptr)
                    retVal = getRootNode()->getData();

                // Return the return value
                return retVal;
            }

            /**
             * Function used to get the root node of the tree
             *
             * @return Generic Data (T) representing the root node
             */
            std::shared_ptr<BaseNode<T>> getRootNode()
            {

                // Return the root node
                return _rootNode;
            }

            /**
             * Function used to get the height of the tree
             *
             * @return Long representing the tree's height
             */
            long height()
            {

                // Create a return value
                long retVal = 0;

                // Set the height from the root node if it isn't null
                if (_rootNode != nullptr)
                    retVal = _rootNode->getHeight() + 1;

                // Return the return value
                return retVal;
            }

            /**
             * Overridable function used to insert an element into the tree
             * NOTE: Will return false if the element already exists
             * NOTE: This will keep a history in the affected nodes stack
             *
             * @param elementToAdd Generic Data (T) representing the data to add
             * @return Boolean indicating whether the element was added or not
             */
            virtual bool insert(T elementToAdd)
            {

                // Create a return flag
                bool retFlag = false;

                // Call the insert-helper function with the root node
                _rootNode = insertHelper(_rootNode, elementToAdd, retFlag);

                // Return the return flag
                return retFlag;
            }

            /**
             * Function used to get the closest element to the provided reference one
             * NOTE: Calling this function on an empty tree produces undefined results
             * NOTE: If the value sits exactly between two others in the tree, it is
             *       not guaranteed which value will be returned
             *
             * @param elementToSearchFor Generic Data (T) representing the reference data
             * @return Generic Data (T) representing the data that is closest to the reference
             */
            T closest(T elementToSearchFor)
            {

                // Create a return value
                T retVal = T();

                // Only continue if the root node is not null
                if (_rootNode != nullptr)
                {

                    // Get the closes element in the tree
                    bool gotReturnValue = false;
                    retVal = closestHelper(_rootNode, elementToSearchFor,
                            _rootNode->getData(), gotReturnValue);
                }

                // Return the return value
                return retVal;
            }

            /**
             * Function used to determine if the provided element exists in the tree or not
             *
             * @param elementToSearchFor Generic Data (T) representing the item to search for
             * @return Boolean indicating if the provided item exists in the tree or not
             */
            bool exists(T elementToSearchFor)
            {

                // Create a return flag
                bool retFlag = false;

                // Get the closest node and check if it is the same
                if (_rootNode != nullptr)
                    retFlag = closest(elementToSearchFor) == elementToSearchFor;

                // Return the return flag
                return retFlag;
            }

            /**
             * Overridable function used to remove an element from the tree
             * NOTE: Will return false if the element didn't exist
             * NOTE: This will keep a history in the affected nodes stack
             *
             * @param elementToRemove Generic Data (T) representing the data to remove
             * @return Boolean indicating whether the element was removed or not
             */
            virtual bool remove(T elementToRemove)
            {

                // Create a return flag
                bool retFlag = false;

                // Call the remove-helper function with the root node
                if (_rootNode != nullptr)
                {
                    bool justRemoved = false;
                    _rootNode = removeHelper(_rootNode, elementToRemove, retFlag, justRemoved);
                }

                // If no nodes were deleted, check to see if we are
                // actually trying to delete the root node and do so
                if (!retFlag && (_rootNode != nullptr) && (elementToRemove == _rootNode->getData()))
                {
                    _rootNode = removeNode(_rootNode);
                    retFlag = true;
                }

                // Return the return flag
                return retFlag;
            }

            /**
             * Function used to perform an in-order traversal and return the results
             *
             * @return Generator on the Generic (T) type for the in-order traversal
             */
            std::shared_ptr<Generator<T>> traverse()
            {

                // Create and return a generator for getting the tree elements
                auto rootNode = this->_rootNode;
                return std::make_shared<Generator<T>>(
                        [rootNode](std::shared_ptr<Yieldable<T>> yielder)
                    {

                        // Recursively traverse the tree using the helper function
                        traversalHelper(rootNode, yielder);
                        yielder->complete();
                    });
            }

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~BinarySearchTree() = default;

        // Protected member functions
        protected:

            /**
             * Internal (virtual) function used to handle post-insertion operations for the tree
             * NOTE: This is called for each node due to a recursive insertion operation
             *
             * @param currNode Shared Pointer representing the current Node in the
             *                 recursive call of the insert operation
             * @return Shared Pointer representing the new Node to replace the current Node
             */
            virtual std::shared_ptr<BaseNode<T>> onPostInsertNode(std::shared_ptr<BaseNode<T>> currNode)
            {
                return currNode;
            }

            /**
             * Internal (virtual) function used to handle post-removal operations for the tree
             * NOTE: This is called for each node due to a recursive removal operation
             *
             * @param currNode Shared Pointer representing the current Node in the
             *                 recursive call of the remove operation
             * @return Shared Pointer representing the new Node to replace the current Node
             */
            virtual std::shared_ptr<BaseNode<T>> onPostRemoveNode(std::shared_ptr<BaseNode<T>> currNode)
            {
                return currNode;
            }

        // Private member functions
        private:

            /**
             * Internal helper function used to insert an element into the tree
             *
             * @param currNode Shared Pointer representing the current Node in the
             *                 recursive call of the insert operation
             * @param elementToAdd Generic Data (T) representing the data to add
             * @param wasAdded Boolean (by reference) indicating whether the item was added
             * @return Shared Pointer representing the new Node to replace the current Node
             */
            std::shared_ptr<BaseNode<T>> insertHelper(std::shared_ptr<BaseNode<T>> currNode,
                    T elementToAdd, bool &wasAdded)
            {

                // If the current node is empty, setup this as the new element
                if(currNode == nullptr)
                {

                    // Setup the current node as the new element
                    currNode = _allocator->allocateNode();
                    currNode->setData(elementToAdd);

                    // Indicate that the item was added
                    wasAdded = true;
                }

                // If the current node is not empty, try the insertion
                else
                {

                    // Determine which branch to search down for insertion
                    if(elementToAdd < currNode->getData())
                        currNode->setLeftChild(insertHelper(currNode->getLeftChild(),
                                elementToAdd, wasAdded));
                    else if (elementToAdd > currNode->getData())
                        currNode->setRightChild(insertHelper(currNode->getRightChild(),
                                elementToAdd, wasAdded));
                }

                // Call the post-insertion function for any overriding classes
                currNode = onPostInsertNode(currNode);

                // Return the current node Shared Pointer
                return currNode;
            }

            /**
             * Function used to get the closest element to the provided reference one
             *
             * @param currNode Shared Pointer representing the current Node in the
             *                 recursive call of the closest search operation
             * @param elementToSearchFor Generic Data (T) representing the reference data
             * @param closestVal Boolean Generic Data (T) representing best data found so far
             * @param gotValue Boolean indicating whether a return value was gotten in a lower stage
             * @param gotValue Boolean indicating whether to use the "closest value" in the search
             * @return Generic Data (T) representing the data that is closest to the reference
             */
            T closestHelper(std::shared_ptr<BaseNode<T>> currNode, T elementToSearchFor,
                    T closestVal, bool &gotValue, bool useClosestVal = true)
            {

                // Create a return value
                T retValue = T();

                // Only continue if the current node is not null
                if (currNode != nullptr)
                {

                    // Determine which branch to search down for searching
                    if (elementToSearchFor < currNode->getData())
                        retValue = closestHelper(currNode->getLeftChild(), elementToSearchFor,
                                                 useClosestVal ? getClosestValue(closestVal, currNode->getData(),
                                                         elementToSearchFor) : elementToSearchFor, gotValue,
                                                                 useClosestVal);
                    else if (elementToSearchFor > currNode->getData())
                        retValue = closestHelper(currNode->getRightChild(), elementToSearchFor,
                                                 useClosestVal ? getClosestValue(closestVal, currNode->getData(),
                                                         elementToSearchFor) : elementToSearchFor, gotValue,
                                                                 useClosestVal);
                }

                // If we get here, and haven't set a return value,
                // set it from the current node (if it isn't nullptr)
                if (!gotValue)
                {

                    // Setup the prospective return value from the current node
                    // and verify it against the best so far (replacing if needed)
                    retValue = closestVal;
                    if (currNode != nullptr)
                        retValue = currNode->getData();
                    if (useClosestVal)
                        retValue = getClosestValue(closestVal, retValue, elementToSearchFor);

                    // Indicate that we have set and found the return value
                    // Only if we just referenced a valid node
                    if (currNode != nullptr)
                        gotValue = true;
                }

                // Return the return value
                return retValue;
            }

            /**
             * Internal helper function used to remove an element from the tree
             *
             * @param currNode Shared Pointer representing the current Node in the
             *                 recursive call of the insert operation
             * @param elementToRemove Generic Data (T) representing the data to remove
             * @param wasRemoved Boolean (by reference) indicating whether the item was removed
             * @param justRemoved Boolean (by reference) indicating whether the item was just removed
             * @param destructive Boolean indicating whether to call the Node's delete operation
             * @return Shared Pointer representing the new Node to replace the current Node
             */
            std::shared_ptr<BaseNode<T>> removeHelper(std::shared_ptr<BaseNode<T>> currNode,
                                                      T elementToRemove, bool &wasRemoved,
                                                      bool &justRemoved, bool destructive = true)
            {

                // Only continue if the current node is not null
                bool isCurrentItem = false;
                if (currNode != nullptr)
                {

                    // Determine which branch to search down for searching
                    if (elementToRemove < currNode->getData())
                        currNode->setLeftChild(removeHelper(currNode->getLeftChild(), elementToRemove,
                                                            wasRemoved, justRemoved, destructive));
                    else if (elementToRemove > currNode->getData())
                        currNode->setRightChild(removeHelper(currNode->getRightChild(), elementToRemove,
                                                             wasRemoved, justRemoved, destructive));

                    // If the value was hit head-on, indicate that we found it
                    // but don't remove it or anything, that has to be done by
                    // the parent node, for child re-arrangement
                    if (elementToRemove == currNode->getData())
                    {
                        justRemoved = true;
                        isCurrentItem = true;
                    }
                }

                // If the node was just removed (from the previous call)
                // then remove the node and re-organize the children
                if (!wasRemoved && justRemoved && !isCurrentItem)
                {

                    // Get both children of the current (parent) node
                    auto leftChild = currNode->getLeftChild();
                    auto rightChild = currNode->getRightChild();

                    // Delete the node and assign the parent accordingly
                    if ((leftChild != nullptr) && (elementToRemove == leftChild->getData()))
                        currNode->setLeftChild(removeNode(leftChild));
                    else
                        currNode->setRightChild(removeNode(rightChild));

                    // Indicate that the item was actually removed
                    wasRemoved = true;
                }

                // Call the post-deletion function for any overriding classes
                currNode = onPostRemoveNode(currNode);

                // Return the current node Shared Pointer
                return currNode;
            }

            /**
             * Internal function used to remove an individual node from the tree
             *
             * @param nodeToRemove Shared Pointer representing the current Node to remove
             * @param destructive Boolean indicating whether to call the Node's delete operation
             * @return Shared Pointer representing the current Node after removal
             */
            std::shared_ptr<BaseNode<T>> removeNode(std::shared_ptr<BaseNode<T>> nodeToRemove,
                                                    bool destructive = true)
            {

                // Create a copy of the node
                auto nodeCopy = nodeToRemove;

                // Get both children of the node
                auto leftChild = nodeToRemove->getLeftChild();
                auto rightChild = nodeToRemove->getRightChild();

                // Handle each case depending on how many children the
                // node we are tying to remove has
                int numOfChildren = (leftChild != nullptr) ? 1 : 0;
                numOfChildren += (rightChild != nullptr) ? 1 : 0;
                switch (numOfChildren)
                {
                    case 0:
                        nodeToRemove = nullptr;
                        break;
                    case 1:
                        nodeToRemove = removeNodeWithOneChild(nodeToRemove);
                        break;
                    default:
                        nodeToRemove = removeNodeWithTwoChildren(nodeToRemove);
                        break;
                }

                // Actually delete the old node
                if (destructive)
                    nodeCopy->deleteNode();

                // Call the post-removal function for any overriding classes
                nodeToRemove = onPostRemoveNode(nodeToRemove);

                // Return the newly assigned/setup node
                return nodeToRemove;
            }

            /**
             * Internal function used to remove a Node with one child from the tree
             *
             * @param nodeToRemove Shared Pointer representing the Node to remove
             * @return Shared Pointer representing the Node to replace the removed one
             */
            std::shared_ptr<BaseNode<T>> removeNodeWithOneChild(std::shared_ptr<BaseNode<T>> nodeToRemove)
            {

                // Create a return value
                std::shared_ptr<BaseNode<T>> retNode = nullptr;

                // Get the children of the node to remove
                auto leftChild = nodeToRemove->getLeftChild();
                auto rightChild = nodeToRemove->getRightChild();

                // Get the child node and simply re-assign it as the
                // return node (cutting out our node to remove)
                if (leftChild != nullptr)
                    retNode = nodeToRemove->getLeftChild();
                else
                    retNode = nodeToRemove->getRightChild();

                // Return the return value
                return retNode;
            }

            /**
             * Internal function used to remove a Node with two children from the tree
             *
             * @param nodeToRemove Shared Pointer representing the Node to remove
             * @return Shared Pointer representing the Node to replace the removed one
             */
            std::shared_ptr<BaseNode<T>> removeNodeWithTwoChildren(std::shared_ptr<BaseNode<T>> nodeToRemove)
            {

                // Create a return value
                std::shared_ptr<BaseNode<T>> retNode = nullptr;

                // Get the children of the node to remove
                auto leftChild = nodeToRemove->getLeftChild();
                auto rightChild = nodeToRemove->getRightChild();

                // Perform re-organization on the left-side of the
                // sub-tree of the node if it is taller
                T replacementData;
                if (leftChild->getHeight() > rightChild->getHeight())
                {

                    // Get the previous largest element to replace the
                    // node we are trying to delete with (by value only)
                    bool gotVal = false;
                    replacementData = closestHelper(leftChild, nodeToRemove->getData(),
                                                    nodeToRemove->getData(), gotVal, false);

                    // Recursively (using the generic delete operation)
                    // re-organize all children in the sub-tree by
                    // deleting the now duplicate which exists there
                    if (leftChild->getData() != replacementData)
                    {
                        bool wasRemoved = false;
                        bool justRemoved = false;
                        leftChild = removeHelper(leftChild, replacementData,
                                wasRemoved, justRemoved, false);
                    }

                    // If the node's current replacement is the immediate child
                    // of the node we are trying to remove, just remove it directly
                    else
                    {
                        leftChild = removeNode(leftChild);
                    }
                }

                // Perform re-organization on the right-side of the
                // sub-tree of the node if it is taller
                else
                {

                    // Get the next smallest element to replace the
                    // node we are trying to delete with (by value only)
                    bool gotVal = false;
                    replacementData = closestHelper(rightChild, nodeToRemove->getData(),
                                                    nodeToRemove->getData(), gotVal, false);

                    // Recursively (using the generic delete operation)
                    // re-organize all children in the sub-tree by
                    // deleting the now duplicate which exists there
                    if (rightChild->getData() != replacementData)
                    {
                        bool wasRemoved = false;
                        bool justRemoved = false;
                        rightChild = removeHelper(rightChild, replacementData,
                                wasRemoved, justRemoved, false);
                    }

                    // If the node's current replacement is the immediate child
                    // of the node we are trying to remove, just remove it directly
                    else
                    {
                        rightChild = removeNode(rightChild);
                    }
                }

                // Re-construct the new return node
                retNode = _allocator->allocateNode();
                retNode->setData(replacementData);
                retNode->setLeftChild(leftChild);
                retNode->setRightChild(rightChild);

                // Return the return value
                return retNode;
            }

            /**
             * Internal function used to get the new (absolute) closest value to
             * the provided target value from the given candidates
             *
             * @param closestValue Generic Data (T) representing the current best value
             * @param candidateValue Generic Data (T) representing the candidate value
             * @param targetValue Generic Data (T) representing the target value
             * @return Generic Data (T) representing the new closest value
             */
            T getClosestValue(T closestValue, T candidateValue, T targetValue)
            {

                // Create a return value
                T retVal = T();

                // Get the absolute difference between the target and current best value
                auto currDiff = closestValue - targetValue;
                if (currDiff < 0)
                    currDiff = targetValue - closestValue;

                // Get the absolute difference between the target and candidate value
                auto candidateDiff = candidateValue - targetValue;
                if (candidateDiff < 0)
                    candidateDiff = targetValue - candidateValue;

                auto currDiffVal = boost::lexical_cast<std::string>(currDiff);
                auto candidateDiffVal = boost::lexical_cast<std::string>(candidateDiff);

                // Determine which value is smaller and set it as the return one
                if (currDiff < candidateDiff)
                    retVal = closestValue;
                else
                    retVal = candidateValue;

                // Return the return value
                return retVal;
            }

            /**
             * Internal recursive helper function used to perform an in-order
             * traversal on the tree elements, yielding the results as we go
             *
             * @param currNode Shared Pointer representing the current Node
             *                 being used in the traversal process
             * @param yielder Yielder on the Generic (T) data type to put the
             *                results into as we go
             */
            static void traversalHelper(std::shared_ptr<BaseNode<T>> currNode,
                    std::shared_ptr<Yieldable<T>> yielder)
            {

                // Only proceed if the current node is not null
                if (currNode != nullptr)
                {

                    // Get the children from the node
                    auto leftChild = currNode->getLeftChild();
                    auto rightChild = currNode->getRightChild();

                    // Recursively perform the in-order traversal
                    // reporting the current node to the yielder
                    traversalHelper(leftChild, yielder);
                    yielder->yield(currNode->getData());
                    traversalHelper(rightChild, yielder);
                }
            }
    };
}

#endif //BITBOSON_STANDARDMODEL_BINARYSEARCHTREE_HPP

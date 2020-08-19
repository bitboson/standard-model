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

#ifndef BITBOSON_STANDARDMODEL_AVLTREE_HPP
#define BITBOSON_STANDARDMODEL_AVLTREE_HPP

#include <BitBoson/StandardModel/DataStructures/BinarySearchTree.hpp>

namespace BitBoson::StandardModel
{

    template <class T, template <class> class S> class AvlTree : public BinarySearchTree<T, S>
    {

        // Public member functions
        public:

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~AvlTree() = default;

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
            std::shared_ptr<BaseNode<T>> onPostInsertNode(std::shared_ptr<BaseNode<T>> currNode) override
            {

                // Create a return value
                std::shared_ptr<BaseNode<T>> retVal = nullptr;

                // Call the balance function on the provided node (if not null)
                if (currNode != nullptr)
                    retVal = balance(currNode);

                // Return the return value
                return retVal;
            }

            /**
             * Internal (virtual) function used to handle post-removal operations for the tree
             * NOTE: This is called for each node due to a recursive removal operation
             *
             * @param currNode Shared Pointer representing the current Node in the
             *                 recursive call of the remove operation
             * @return Shared Pointer representing the new Node to replace the current Node
             */
            std::shared_ptr<BaseNode<T>> onPostRemoveNode(std::shared_ptr<BaseNode<T>> currNode) override
            {

                // Create a return value
                std::shared_ptr<BaseNode<T>> retVal = nullptr;

                // Call the balance function on the provided node (if not null)
                if (currNode != nullptr)
                    retVal = balance(currNode);

                // Return the return value
                return retVal;
            }

        // Private member functions
        private:

            /**
             * Internal function used to balance and enforce AVL property on the supplied node
             *
             * @param currNode Shared Pointer representing the current node to balance
             * @return Shared Pointer representing the current node after being balanced
             */
            std::shared_ptr<BaseNode<T>> balance(std::shared_ptr<BaseNode<T>> currNode)
            {

                // Extract both children from the provided node
                auto leftChild = currNode->getLeftChild();
                auto rightChild = currNode->getRightChild();

                // Only attempt a rotation if the AVL property is violated
                auto leftHeight = (leftChild != nullptr) ? leftChild->getHeight() : -1;
                auto rightHeight = (rightChild != nullptr) ? rightChild->getHeight() : -1;
                if (std::abs(leftHeight - rightHeight) > 1)
                {

                    // Extract all grand-children from the children
                    auto leftChildLeftTree = (leftChild != nullptr) ? leftChild->getLeftChild() : nullptr;
                    auto leftChildRightTree = (leftChild != nullptr) ? leftChild->getRightChild() : nullptr;
                    auto rightChildLeftTree = (rightChild != nullptr) ? rightChild->getLeftChild() : nullptr;
                    auto rightChildRightTree = (rightChild != nullptr) ? rightChild->getRightChild() : nullptr;

                    // Extract the left-child heights
                    auto leftChildLeftHeight = (leftChildLeftTree != nullptr) ? leftChildLeftTree->getHeight() : -1;
                    auto leftChildRightHeight = (leftChildRightTree != nullptr) ? leftChildRightTree->getHeight() : -1;
                    auto rightChildLeftHeight = (rightChildLeftTree != nullptr) ? rightChildLeftTree->getHeight() : -1;
                    auto rightChildRightHeight = (rightChildRightTree != nullptr) ? rightChildRightTree->getHeight() : -1;

                    // If the left child is left-heavy or balanced, perform a left-rotation
                    if (leftChildLeftHeight > leftChildRightHeight)
                    {
                        currNode = rightRotation(currNode);
                    }

                    // If the right child is right-heavy or balanced, perform a right-rotation
                    else if (rightChildRightHeight > rightChildLeftHeight)
                    {
                        currNode = leftRotation(currNode);
                    }

                    // If the left child is right-heavy, perform a double rotation (left then right)
                    else if (leftChildLeftHeight < leftChildRightHeight)
                    {
                        currNode->setLeftChild(leftRotation(currNode->getLeftChild()));
                        currNode = rightRotation(currNode);
                    }

                    // If the left child is right-heavy, perform a double rotation (right then left)
                    else if (rightChildRightHeight < rightChildLeftHeight)
                    {
                        currNode->setRightChild(rightRotation(currNode->getRightChild()));
                        currNode = leftRotation(currNode);
                    }
                }

                // Return the newly balanced node
                return currNode;
            }

            /**
             * Internal function used to perform a left-rotation on the provided node
             *
             * @param currNode Shared Pointer representing the current node to rotate
             * @return Shared Pointer representing the current node after being rotated
             */
            std::shared_ptr<BaseNode<T>> leftRotation(std::shared_ptr<BaseNode<T>> currNode)
            {

                // Extract the input node's right child
                std::shared_ptr<BaseNode<T>> currNodeRightChild = currNode->getRightChild();

                // Extract the input node's right child's left child (if present)
                std::shared_ptr<BaseNode<T>> currNodeRightChildLeftChild = nullptr;
                if (currNodeRightChild != nullptr)
                    currNodeRightChildLeftChild = currNodeRightChild->getLeftChild();

                // Assign the input node's right child to be the right child's left child
                currNode->setRightChild(currNodeRightChildLeftChild);

                // Assign the input node's right child to have a left child of the input node
                currNodeRightChild->setLeftChild(currNode);

                // Return the input node's original right child as the sub-tree root
                return currNodeRightChild;
            }

            /**
             * Internal function used to perform a right-rotation on the provided node
             *
             * @param currNode Shared Pointer representing the current node to rotate
             * @return Shared Pointer representing the current node after being rotated
             */
            std::shared_ptr<BaseNode<T>> rightRotation(std::shared_ptr<BaseNode<T>> currNode)
            {

                // Extract the input node's left child
                std::shared_ptr<BaseNode<T>> currNodeLeftChild = currNode->getLeftChild();

                // Extract the input node's left child's right child (if present)
                std::shared_ptr<BaseNode<T>> currNodeLeftChildRightChild = nullptr;
                if (currNodeLeftChild != nullptr)
                    currNodeLeftChildRightChild = currNodeLeftChild->getRightChild();

                // Assign the input node's left child to be the left child's right child
                currNode->setLeftChild(currNodeLeftChildRightChild);

                // Assign the input node's left child to have a right child of the input node
                currNodeLeftChild->setRightChild(currNode);

                // Return the input node's original left child as the sub-tree root
                return currNodeLeftChild;
            }
    };
}

#endif //BITBOSON_STANDARDMODEL_AVLTREE_HPP

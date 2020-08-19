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

#ifndef BITBOSON_STANDARDMODEL_MEMORYNODE_HPP
#define BITBOSON_STANDARDMODEL_MEMORYNODE_HPP

#include <BitBoson/StandardModel/DataStructures/Containers/BaseNode.hpp>

namespace BitBoson::StandardModel
{

    template <class T> class MemoryNode : public BaseNode<T>
    {

        // private member variables
        private:
            std::shared_ptr<BaseNode<T>> _leftChild = nullptr;
            std::shared_ptr<BaseNode<T>> _rightChild = nullptr;

        // Public member functions
        public:

            /**
             * Overridden function used to set the Node's left child
             *
             * @param leftChild Shared Pointer representing the Node's left child
             */
            void setLeftChild(std::shared_ptr<BaseNode<T>> leftChild) override
            {

                // Call the super-class function
                BaseNode<T>::setLeftChild(leftChild);

                // Set the node's left child
                _leftChild = leftChild;
            };

            /**
             * Overridden function used to get the Node's left child
             *
             * @return Shared Pointer representing the Node's left child
             */
            std::shared_ptr<BaseNode<T>> getLeftChild() override
            {

                // Return the node's left child
                return _leftChild;
            };;

            /**
             * Overridden function used to get the Node's right child
             *
             * @param rightChild Shared Pointer representing the Node's right child
             */
            void setRightChild(std::shared_ptr<BaseNode<T>> rightChild) override
            {

                // Call the super-class function
                BaseNode<T>::setRightChild(rightChild);

                // Set the node's right child
                _rightChild = rightChild;
            };

            /**
             * Overridden function used to get the Node's right child
             *
             * @return Shared Pointer representing the Node's right child
             */
            std::shared_ptr<BaseNode<T>> getRightChild() override
            {

                // Return the node's right child
                return _rightChild;
            };

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~MemoryNode() = default;
    };
}

#endif //BITBOSON_STANDARDMODEL_MEMORYNODE_HPP

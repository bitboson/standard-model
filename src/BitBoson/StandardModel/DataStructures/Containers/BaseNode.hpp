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

#ifndef BITBOSON_STANDARDMODEL_BASENODE_HPP
#define BITBOSON_STANDARDMODEL_BASENODE_HPP

#include <memory>

namespace BitBoson::StandardModel
{

    template <class T> class BaseNode
    {

        // Private member variables
        private:
            T _data;
            long _height;

        // Public member functions
        public:

            /**
             * Constructor used to setup the Base Node instance
             */
            BaseNode()
            {

                // Setup the default values
                _height = 0;
            }

            /**
             * Virtual function used to set the Node's data
             *
             * @param data Generic Data (T) representing the Node's data
             */
            virtual void setData(T data)
            {
                _data = data;
            };

            /**
             * Virtual function used to get the Node's data
             *
             * @return Generic Data (T) representing the Node's data
             */
            virtual T getData()
            {
                return _data;
            }

            /**
             * Virtual function used to get the Node's height
             *
             * @return Long representing the Node's height
             */
            virtual long getHeight()
            {
                return _height;
            }

            /**
             * Virtual function used to set the Node's left child
             *
             * @param leftChild Shared Pointer representing the Node's left child
             */
            virtual void setLeftChild(std::shared_ptr<BaseNode> leftChild)
            {

                // Get the other child node's reference
                auto rightChild = getRightChild();

                // Get the newly added child's height as well as the old one's
                auto newChildHeight = (leftChild != nullptr) ? leftChild->getHeight(): -1;
                auto otherChildHeight = (rightChild != nullptr) ? rightChild->getHeight() : -1;

                // Set the height to the max of the two heights (plus one)
                if (newChildHeight > otherChildHeight)
                    setHeight(newChildHeight + 1);
                else
                    setHeight(otherChildHeight + 1);
            }

            /**
             * Virtual function used to get the Node's left child
             *
             * @return Shared Pointer representing the Node's left child
             */
            virtual std::shared_ptr<BaseNode> getLeftChild() = 0;

            /**
             * Virtual function used to get the Node's right child
             *
             * @param rightChild Shared Pointer representing the Node's right child
             */
            virtual void setRightChild(std::shared_ptr<BaseNode> rightChild)
            {

                // Get the other child node's reference
                auto leftChild = getLeftChild();

                // Get the newly added child's height as well as the old one's
                auto newChildHeight = (rightChild != nullptr) ? rightChild->getHeight() : -1;
                auto otherChildHeight = (leftChild != nullptr) ? leftChild->getHeight() : -1;

                // Set the height to the max of the two heights (plus one)
                if (newChildHeight > otherChildHeight)
                    setHeight(newChildHeight + 1);
                else
                    setHeight(otherChildHeight + 1);
            }

            /**
             * Virtual function used to get the Node's right child
             *
             * @return Shared Pointer representing the Node's right child
             */
            virtual std::shared_ptr<BaseNode> getRightChild() = 0;

            /**
             * Virtual function used to delete the underlying Node and its data
             */
            virtual void deleteNode() {};

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~BaseNode() = default;

        // Protected member fuctions
        protected:

            /**
             * Internal function used to manually set the Node's height
             *
             * @param height Long representing the Node's height
             */
            void setHeight(long height)
            {

                // Set the Node's height
                _height = height;
            }
    };
}

#endif //BITBOSON_STANDARDMODEL_BASENODE_HPP

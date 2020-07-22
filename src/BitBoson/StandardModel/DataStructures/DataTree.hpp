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

#ifndef BITBOSON_STANDARDMODEL_DATATREE_HPP
#define BITBOSON_STANDARDMODEL_DATATREE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <BitBoson/StandardModel/Crypto/Crypto.h>
#include <BitBoson/StandardModel/Primitives/Timestamp.h>

namespace BitBoson::StandardModel
{

    template<class T> class DataTree
    {

        // Private structures
        public:

            struct Node
            {
                T data;
                bool isLeaf;
                std::string hash;
                std::string parentHash;
                Timestamp timestamp;
            };

        // Private member variables
        private:
            std::string _headHash;
            std::unordered_map<std::string, Node*>* _treeDictionary;

        // Public member functions
        public:

            /**
             * Constructor used to setup the data tree instance
             *
             * @param rootHash String representing an optional root hash
             */
            explicit DataTree(const std::string& rootHash="")
            {

                // Setup default values
                _headHash = rootHash.empty() ? Crypto::getRandomSha256() : rootHash;
                _treeDictionary = new std::unordered_map<std::string, Node*>();
            }

            /**
             * Function used to get the hash of the head item in the tree for reference in adding new items
             *
             * @return String representing the hash of the head item so it can be a parent
             */
            std::string getHeadHash() const
            {

                // Return the hash of the hash for the head item
                return _headHash;
            }

            /**
             * Function used to add an item to the tree at the given parent (via hash)
             *
             * @param dataToAdd Data to store in the tree
             * @param parentHash Hash of the parent to store the data at
             * @param itemHash Hash used to represent the item in the tree (will be random if not assigned)
             * @return String representing the hash of this item (once added) so it can be a parent
             */
            std::string addItem(T dataToAdd, const std::string& parentHash, const std::string& itemHash="")
            {

                // Create return string (for hash)
                std::string retHash;

                // Create the new node to add
                auto* newNode = new Node();
                newNode->isLeaf = true;
                newNode->data = dataToAdd;
                newNode->hash = (itemHash.empty() ? Crypto::getRandomSha256() : itemHash);
                newNode->parentHash = parentHash;
                newNode->timestamp = Timestamp::getCurrentTimestamp();

                // Attempt to add the item to the tree and extract the hash if it was added
                // Otherwise delete the newly created node here (since it wasn't added)
                if (addItem(newNode))
                    retHash = newNode->hash;
                else
                    delete newNode;

                // Return the hash for the (potentially) newly added item
                return retHash;
            }

            /**
             * Function used to add an item to the tree (based on its Node representation)
             *
             * @param nodeToAdd Node representation of the item to add
             * @return Boolean indicating whether the item was added or no
             */
            bool addItem(Node* nodeToAdd)
            {

                // Create a return flag
                bool wasAdded = false;

                // Verify that the parent actually exists
                bool foundParent = false;
                if (isItemInTree(nodeToAdd->parentHash))
                    foundParent = true;

                // Only continue to change the empty hash return hash if the parent was found (or it is the head hash)
                // and the item doesn't already exist in the tree
                if ((foundParent || (nodeToAdd->parentHash == _headHash)) && !isItemInTree(nodeToAdd->hash))
                {

                    // Add the item to the dictionary
                    _treeDictionary->emplace(nodeToAdd->hash, nodeToAdd);

                    // Indicate that the item was added
                    wasAdded = true;
                }

                // If we added the item, ensure that the parent's leaf flag is set to false
                if (wasAdded)
                    if (isItemInTree(nodeToAdd->parentHash))
                        _treeDictionary->at(nodeToAdd->parentHash)->isLeaf = false;

                // Return the return flag
                return wasAdded;
            }

            /**
             * Function used to get the parent hash for the given child item
             *
             * @param itemHash Item to find the parent of in the tree
             * @return Sting representing the hash of the parent for the given child
             */
            std::string getParentForItem(const std::string& itemHash)
            {

                // Create a return item to hold the hash in
                std::string retData;

                // Attempt to gt the parent item and set it to the return
                if (isItemInTree(itemHash))
                    retData = _treeDictionary->at(itemHash)->parentHash;

                // Return the return data
                return retData;
            }

            /**
             * Function used to get the item represented by the given item hash
             *
             * @param itemHash Hash String representing the hash for the item to get
             * @return Data for the given hash (or nullptr if it doesn't exist)
             */
            T getItem(const std::string& itemHash) const
            {

                // Create a return item to hold the data in (nullptr by default)
                T retData;

                // Attempt to get the parent item and set it to the return
                if (isItemInTree(itemHash))
                    retData = _treeDictionary->find(itemHash)->second->data;

                // Return the return data
                return retData;
            }

            /**
             * Function used to get all siblings of the given item (by hash)
             *
             * @param itemHash Hash String representing the item to get the siblings of
             * @return Vector of Strings representing the sibling items (as hashes)
             */
            std::vector<std::string> getSiblingsOfItem(const std::string& itemHash)
            {

                // Create a return vector
                std::vector<std::string> retVect;

                // Start by getting the parent of the given item
                auto parentHash = getParentForItem(itemHash);

                // Now list all children for the parent and add them to the
                // return vector excluding the provided item hash
                for (auto* child : getChildrenOfItem(parentHash))
                    if (child->hash != itemHash)
                        retVect.push_back(child->hash);

                // Return the return vector
                return retVect;
            }

            /**
             * Function used to delete the given item (and its children) from the tree
             *
             * @param itemHash String representing the item in the tree to delete
             * @param deleteChildren Boolean indicating whether to delete the children or not
             *                       If they aren't deleted, their parent will be re-assigned
             */
            void deleteItem(const std::string& itemHash, bool deleteChildren=true)
            {

                // Only attempt to delete the item if it exists in the tree
                if (isItemInTree(itemHash))
                {

                    // Get all children of this item and delete/re-assign them
                    auto* parentNode = _treeDictionary->at(itemHash);
                    for (Node* child : getChildrenOfItem(itemHash, false))
                        if (deleteChildren)
                            deleteItem(child->hash, deleteChildren);
                        else
                            child->parentHash = std::string(parentNode->parentHash);

                    // Extract the node from the dictionary
                    auto* node = _treeDictionary->at(itemHash);

                    // Extract the parent hash (if it exists)
                    std::string parentHash;
                    if (node != nullptr)
                        parentHash = std::string(node->parentHash);

                    // Remove the entry from the dictionary
                    _treeDictionary->erase(itemHash);

                    // Delete it (if it exists)
                    if (node != nullptr)
                        delete node;

                    // Only continue if the parent was found and is valid
                    if (!parentHash.empty())
                    {

                        // Determine if the parent item still has any leaves/children
                        bool foundAnotherLeaf = false;
                        for (const auto &item : *_treeDictionary)
                        {
                            if (item.second->parentHash == parentHash)
                            {
                                foundAnotherLeaf = true;
                                break;
                            }
                        }

                        // If no other leaves were found, update the flag
                        if (!foundAnotherLeaf)
                            if (isItemInTree(parentHash))
                                 _treeDictionary->at(parentHash)->isLeaf = true;
                    }
                }
            }

            /**
             * Function used to get the "deepest" or "tallest" leaf in the tree
             * NOTE: If two nodes are of equal depth, then the "oldest" leaf will
             *       be accepted over the newer (more recent) one
             *
             * @return String representing the hash of the "deepest" leaf node
             */
            std::string getDeepestNode() const
            {

                // Create a return value
                std::string retVal;

                // Loop through all current leaves so that we can search
                // for the current deepest or tallest node
                long currentTallestVal = -1;
                Timestamp currentTallestTimestamp = Timestamp("0");
                for (const auto* item : getAllLeaves())
                {

                    // Get the height of the current node
                    auto currHeight = getNodeDepth(item->hash);

                    // Store the current node/hash if it is the tallest
                    // or the oldest (if there is a tie)
                    if ((currHeight > currentTallestVal)
                            || ((currHeight == currentTallestVal)
                                && (item->timestamp < currentTallestTimestamp)))
                    {

                        // Store the current best values
                        currentTallestVal = currHeight;
                        currentTallestTimestamp = item->timestamp;

                        // Also set the return value according to
                        // the current deepest/tallest node
                        retVal = item->hash;
                    }
                }

                // Return the return value
                return retVal;
            }

            /**
             * Function used to get whether the provided item (by hash) is a leaf or not
             *
             * @param itemHash String representing the item in the tree to check leaf status of
             * @return Boolean indicating whether the provided item (by hash) is a leaf or not
             */
            bool isItemALeaf(const std::string& itemHash) const
            {

                // Create a return flag
                bool retFlag = false;

                // Determine whether the item is a leaf or not (if it exists)
                if (isItemInTree(itemHash))
                    retFlag = _treeDictionary->at(itemHash)->isLeaf;

                // Return the return flag
                return retFlag;
            }

            /**
             * Function used to get the list of all leaf nodes in the tree
             *
             * @return Vector of Node pointers representing the leaves of the tree
             */
            std::vector<Node*> getAllLeaves() const
            {

                // Create a return vector to copy the data into
                std::vector<Node*> retItems;

                // Loop through all items in the tree, adding them to the return vector
                for (auto item : *_treeDictionary)
                    if (item.second->isLeaf)
                        retItems.push_back(item.second);

                // Return the vector of items
                return retItems;
            }

            /**
             * Function used to get all items in the tree (internal representation)
             * NOTE: Since the Nodes contain the parent and current hash, the complete tree can be
             *       reconstructed from this representation/vector
             *
             * @return Vector of Node pointers representing the items of the tree
             */
            std::vector<Node*> getAllItems() const
            {

                // Create a return vector to copy the data into
                std::vector<Node*> retItems;

                // Loop through all items in the tree, adding them to the return vector
                for (auto item : *_treeDictionary)
                    retItems.push_back(item.second);

                // Return the vector of items
                return retItems;
            }

            /**
             * Function used to get all items under the given parent item (internal representation)
             * NOTE: Since the Nodes contain the parent and current hash, the complete tree can be
             *       reconstructed from this representation/vector
             *
             * @param parentHash String hash representing the parent of the items to get
             * @param recursive Boolean indicating whether to recursively get all children or not
             * @return Vector of Node pointers representing the items of the tree for the parent
             */
            std::vector<Node*> getChildrenOfItem(const std::string& parentHash, bool recursive=false) const
            {

                // Create a return vector to copy the data into
                std::vector<Node*> retItems;

                // Loop through all items, checking if the parent is the correct item or not
                for (auto item : *_treeDictionary)
                    if (item.second->parentHash == parentHash)
                        retItems.push_back(item.second);

                // If we desire to get all children recursively, perform the recursive call and merge the results
                if (recursive)
                {
                    std::vector<Node*> subItems;
                    for (Node* item : retItems)
                        for (Node* subItem : getChildrenOfItem(item->hash, true))
                            subItems.push_back(subItem);
                    for (Node* subItem : subItems)
                        retItems.push_back(subItem);
                }

                // Return the vector of items
                return retItems;
            }

            /**
             * Function used to determine if the given item (by value) is already in the tree
             *
             * @param hash Hash to check for in the tree
             * @return Boolean indicating whether the given item (by value) is in the tree
             */
            bool isItemInTree(const std::string& hash) const
            {

                // Create a return flag
                bool wasFound = false;

                // Check if the item is in the tree
                if (_treeDictionary != nullptr)
                    if (_treeDictionary->find(hash) != _treeDictionary->end())
                        wasFound = true;

                // Return the return flag (whether the item was found or not)
                return wasFound;
            }

            /**
             * Destructor used to cleanup the tree instance and its dependent items
             */
            virtual ~DataTree()
            {

                 // Loop through and delete all pointers in the array
                 for (auto item : *_treeDictionary)
                 {
                     delete item.second;
                 }

                 // Clear the array
                _treeDictionary->clear();
                 delete _treeDictionary;
                _treeDictionary = nullptr;
            }

        // Private member functions
        private:

            /**
             * Internal function used to get the depth of the given node
             *
             * @param nodeHash String representing the node to use
             * @return Long representing the depth/height of the node
             */
            long getNodeDepth(const std::string& nodeHash) const
            {

                // Create a return value
                long retVal = -1;

                // Only continue if the provided node hash exists
                if (isItemInTree(nodeHash))
                {

                    // Continuously get the parent item/node
                    // until we reach the current root hash
                    auto headHash = getHeadHash();
                    const auto* currentNode = _treeDictionary->at(nodeHash);
                    while ((currentNode->hash != headHash)
                        && (currentNode->parentHash != headHash))
                    {

                        // Re-assign the current node to the parent
                        currentNode = _treeDictionary->at(currentNode->parentHash);

                        // Increment the counter
                        retVal++;
                    }

                    // If we reached the node right below the head-hash,
                    // increment the counter again
                    if (currentNode->parentHash == headHash)
                        retVal++;
                }

                // Return the return value
                return retVal;
            }
    };
}

#endif //BITBOSON_STANDARDMODEL_DATATREE_HPP

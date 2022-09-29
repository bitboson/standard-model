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

#ifndef BITBOSON_STANDARDMODEL_DISKNODE_TEST_HPP
#define BITBOSON_STANDARDMODEL_DISKNODE_TEST_HPP

#include <BitBoson/StandardModel/FileSystem/FileSystem.h>
#include <BitBoson/StandardModel/DataStructures/AvlTree.hpp>
#include <BitBoson/StandardModel/DataStructures/Containers/DiskNode.hpp>

using namespace BitBoson::StandardModel;

TEST_CASE ("Create and Load Disk Node Test", "[DiskNodeTest]")
{

    // Create a Disk Node
    auto diskCache = std::make_shared<DiskCache>();
    auto diskNodeRoot = std::make_shared<DiskNode<int>>();
    diskNodeRoot->setInternalDiskCache(diskCache);
    diskNodeRoot->setData(5);

    // Create a new scope so that shared pointers are deleted
    {

        // Create two more Disk Nodes
        auto diskNodeLeft = std::make_shared<DiskNode<int>>();
        diskNodeLeft->setInternalDiskCache(diskCache);
        diskNodeLeft->setData(1);
        auto diskNodeRight = std::make_shared<DiskNode<int>>();
        diskNodeRight->setInternalDiskCache(diskCache);
        diskNodeRight->setData(9);

        // Add the two children to the root
        diskNodeRoot->setLeftChild(diskNodeLeft);
        diskNodeRoot->setRightChild(diskNodeRight);
    }

    // Load the child nodes
    auto leftChild = diskNodeRoot->getLeftChild();
    auto rightChild = diskNodeRoot->getRightChild();

    // Verify that the children were loaded
    REQUIRE(leftChild->getData() == 1);
    REQUIRE(rightChild->getData() == 9);
}

TEST_CASE ("Left-Insertion Only Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(6));
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(4));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(2));
    REQUIRE(integerAvlTree->insert(1));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Right-Insertion Only Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(1));
    REQUIRE(integerAvlTree->insert(2));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(4));
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(6));
    REQUIRE(integerAvlTree->insert(7));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Left-Line Root Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(1));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 2);
}

TEST_CASE ("Left-Zig-Zag Root Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(1));
    REQUIRE(integerAvlTree->insert(3));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 2);
}

TEST_CASE ("Right-Line Root Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(9));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 2);
}

TEST_CASE ("Right-Zig-Zag Root Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(9));
    REQUIRE(integerAvlTree->insert(7));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 2);
}

TEST_CASE ("Left-Line Node Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(2));
    REQUIRE(integerAvlTree->insert(1));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Left-Zig-Zag Node Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(1));
    REQUIRE(integerAvlTree->insert(2));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Right-Line Node Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(8));
    REQUIRE(integerAvlTree->insert(9));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Right-Zig-Zag Node Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(9));
    REQUIRE(integerAvlTree->insert(8));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Left-Line Deletion Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(2));
    REQUIRE(integerAvlTree->insert(4));
    REQUIRE(integerAvlTree->insert(6));
    REQUIRE(integerAvlTree->insert(8));
    REQUIRE(integerAvlTree->insert(1));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 4);

    // Delete the value 4 to force a left-line adjustment
    REQUIRE(integerAvlTree->remove(4));

    // Verify that the tree re-balanced
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Left-Zig-Zag Deletion Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(1));
    REQUIRE(integerAvlTree->insert(4));
    REQUIRE(integerAvlTree->insert(6));
    REQUIRE(integerAvlTree->insert(8));
    REQUIRE(integerAvlTree->insert(2));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 4);

    // Delete the value 4 to force a left-zig-zag adjustment
    REQUIRE(integerAvlTree->remove(4));

    // Verify that the tree re-balanced
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Right-Line Deletion Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(2));
    REQUIRE(integerAvlTree->insert(6));
    REQUIRE(integerAvlTree->insert(8));
    REQUIRE(integerAvlTree->insert(1));
    REQUIRE(integerAvlTree->insert(9));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 4);

    // Delete the value 6 to force a right-line adjustment
    REQUIRE(integerAvlTree->remove(6));

    // Verify that the tree re-balanced
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Right-Zig-Zag Deletion Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(2));
    REQUIRE(integerAvlTree->insert(6));
    REQUIRE(integerAvlTree->insert(9));
    REQUIRE(integerAvlTree->insert(1));
    REQUIRE(integerAvlTree->insert(8));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 4);

    // Get the disk-allocator's cache reference
    auto allocatorCacheRef = diskNodeAllocator->getDiskCacheReference();

    // Ensure the 6 value exists in the cache
    REQUIRE(!allocatorCacheRef->getItem("6").empty());

    // Delete the value 6 to force a right-zig-zag adjustment
    REQUIRE(integerAvlTree->remove(6));

    // Ensure the 6 value no longer exists in the cache
    REQUIRE(allocatorCacheRef->getItem("6").empty());

    // Verify that the tree re-balanced
    REQUIRE(integerAvlTree->height() == 3);

}

TEST_CASE ("Root Deletion Disk-Node AVL Tree Balance", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(2));
    REQUIRE(integerAvlTree->insert(6));
    REQUIRE(integerAvlTree->insert(9));
    REQUIRE(integerAvlTree->insert(1));
    REQUIRE(integerAvlTree->insert(8));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 4);

    // Get the disk-allocator's cache reference
    auto allocatorCacheRef = diskNodeAllocator->getDiskCacheReference();

    // Ensure the 5 value exists in the cache
    REQUIRE(!allocatorCacheRef->getItem("5").empty());

    // Delete the value 5 to force a root deletion
    REQUIRE(integerAvlTree->remove(5));

    // Ensure the 6 value no longer exists in the cache
    REQUIRE(allocatorCacheRef->getItem("5").empty());

    // Verify that the tree re-balanced
    REQUIRE(integerAvlTree->height() == 3);

}

TEST_CASE ("Setup a new Disk-Node AVL Tree on an existing cache", "[DiskNodeTest]")
{

    // Create the integer BST instance
    auto cacheDir = FileSystem::getTemporaryDir("BitBoson").getFullPath();
    auto integerAvlTree = std::make_shared<AvlTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>(cacheDir);
    integerAvlTree->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(2));
    REQUIRE(integerAvlTree->insert(6));
    REQUIRE(integerAvlTree->insert(9));
    REQUIRE(integerAvlTree->insert(1));
    REQUIRE(integerAvlTree->insert(8));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 4);

    // Get the disk-allocator's cache reference
    auto allocatorCacheRef = diskNodeAllocator->getDiskCacheReference();

    // Save the root node information in the singleton cache
    allocatorCacheRef->addItem("RootNode",
            std::to_string(integerAvlTree->getRootElement()));

    // Extract the directory information from the
    auto cacheDir2 = allocatorCacheRef->getCacheDirectory();
    REQUIRE(cacheDir == cacheDir2);

    // Construct a new root node from the cached information
    auto newRootVal = allocatorCacheRef->getItem("RootNode");
    auto newRoot = std::make_shared<DiskNode<int>>();
    newRoot->setInternalDiskCache(allocatorCacheRef);
    newRoot->overrideLoadFromCache(std::stoi(newRootVal));

    // Try to load a left-node as well (for testing sake)
    auto newLeaf = std::make_shared<DiskNode<int>>();
    newLeaf->setInternalDiskCache(allocatorCacheRef);
    newLeaf->overrideLoadFromCache(8);

    // Setup a new integer BST on the current cache-directory
    auto integerAvlTree2 = std::make_shared<AvlTree<int, DiskNode>>();
    integerAvlTree2->overrideSetRootNode(newRoot);

    // Ensure that the tree loaded the state properly
    REQUIRE(integerAvlTree2->exists(5));
    REQUIRE(integerAvlTree2->exists(3));
    REQUIRE(integerAvlTree2->exists(7));
    REQUIRE(integerAvlTree2->exists(2));
    REQUIRE(integerAvlTree2->exists(6));
    REQUIRE(integerAvlTree2->exists(9));
    REQUIRE(integerAvlTree2->exists(1));
    REQUIRE(integerAvlTree2->exists(8));
    REQUIRE(integerAvlTree2->height() == 4);
}



#endif //BITBOSON_STANDARDMODEL_DISKNODE_TEST_HPP

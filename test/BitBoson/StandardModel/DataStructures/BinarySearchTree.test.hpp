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

#ifndef BITBOSON_STANDARDMODEL_BINARYSEARCHTREE_TEST_HPP
#define BITBOSON_STANDARDMODEL_BINARYSEARCHTREE_TEST_HPP

#include <BitBoson/StandardModel/DataStructures/BinarySearchTree.hpp>
#include <BitBoson/StandardModel/DataStructures/Containers/DiskNode.hpp>
#include <BitBoson/StandardModel/DataStructures/Containers/MemoryNode.hpp>

using namespace BitBoson::StandardModel;

TEST_CASE ("MemoryNode Insertion and Existence BST Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Verify a value doesn't exist until after we add it
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->exists(5));

    // Verify that a different number doesn't exist in the tree
    REQUIRE(!integerBst->exists(10));
}

TEST_CASE ("MemoryNode Multiple Value Insertion and Existence BST Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->insert(7));

    // Verify that all of the expected values exist
    // and that others (not added) don't
    REQUIRE(!integerBst->exists(0));
    REQUIRE(integerBst->exists(1));
    REQUIRE(!integerBst->exists(2));
    REQUIRE(integerBst->exists(3));
    REQUIRE(!integerBst->exists(4));
    REQUIRE(integerBst->exists(5));
    REQUIRE(!integerBst->exists(6));
    REQUIRE(integerBst->exists(7));
    REQUIRE(!integerBst->exists(8));
    REQUIRE(integerBst->exists(9));

    // Verify tree state with an in-order traversal
    int index = 0;
    const int retItems[] = {1, 3, 5, 7, 9};
    auto treeTraversal = integerBst->traverse();
    while (treeTraversal->hasMoreItems())
        REQUIRE(treeTraversal->getNextItem() == retItems[index++]);
}

TEST_CASE ("MemoryNode Inserting Duplicates BST Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add a new value to the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->exists(5));

    // Verify that adding it again doesn't work
    REQUIRE(!integerBst->insert(5));
    REQUIRE(integerBst->exists(5));
}

TEST_CASE ("MemoryNode Get Closest Items BST Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->insert(7));

    // Get the closest value to the provided ones
    REQUIRE(integerBst->closest(-100) == 1);
    REQUIRE(integerBst->closest(0) == 1);
    REQUIRE(integerBst->closest(1) == 1);
    REQUIRE(integerBst->closest(2) == 3);
    REQUIRE(integerBst->closest(3) == 3);
    REQUIRE(integerBst->closest(4) == 3);
    REQUIRE(integerBst->closest(5) == 5);
    REQUIRE(integerBst->closest(6) == 7);
    REQUIRE(integerBst->closest(7) == 7);
    REQUIRE(integerBst->closest(8) == 7);
    REQUIRE(integerBst->closest(9) == 9);
    REQUIRE(integerBst->closest(10) == 9);
    REQUIRE(integerBst->closest(100) == 9);
}

TEST_CASE ("MemoryNode Deleting Elements BST Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->insert(7));

    // Delete a value from the tree
    REQUIRE(integerBst->remove(3));

    // Verify the state of the tree
    REQUIRE(integerBst->exists(1));
    REQUIRE(!integerBst->exists(3));
    REQUIRE(integerBst->exists(5));
    REQUIRE(integerBst->exists(7));
    REQUIRE(integerBst->exists(9));

    // Delete another value from the tree
    REQUIRE(integerBst->remove(5));

    // Verify the state of the tree
    REQUIRE(integerBst->exists(1));
    REQUIRE(!integerBst->exists(3));
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->exists(7));
    REQUIRE(integerBst->exists(9));

    // Verify that we can't delete an already-deleted item
    REQUIRE(!integerBst->remove(5));

    // Verify the state of the tree
    REQUIRE(integerBst->exists(1));
    REQUIRE(!integerBst->exists(3));
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->exists(7));
    REQUIRE(integerBst->exists(9));

    // Verify tree state with an in-order traversal
    int index = 0;
    const int retItems[] = {1, 7, 9};
    auto treeTraversal = integerBst->traverse();
    while (treeTraversal->hasMoreItems())
        REQUIRE(treeTraversal->getNextItem() == retItems[index++]);
}

TEST_CASE ("MemoryNode Perform Search After Deletion BST Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->insert(7));

    // Delete a couple items
    REQUIRE(integerBst->remove(5));
    REQUIRE(integerBst->remove(9));

    // Get the closest value to the provided ones
    REQUIRE(integerBst->closest(-100) == 1);
    REQUIRE(integerBst->closest(0) == 1);
    REQUIRE(integerBst->closest(1) == 1);
    REQUIRE(integerBst->closest(2) == 3);
    REQUIRE(integerBst->closest(3) == 3);
    REQUIRE(integerBst->closest(4) == 3);
    REQUIRE(integerBst->closest(5) == 3);
    REQUIRE(integerBst->closest(6) == 7);
    REQUIRE(integerBst->closest(7) == 7);
    REQUIRE(integerBst->closest(8) == 7);
    REQUIRE(integerBst->closest(9) == 7);
    REQUIRE(integerBst->closest(10) == 7);
    REQUIRE(integerBst->closest(100) == 7);
}

TEST_CASE ("MemoryNode Left-Only Insertion BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->insert(8));
    REQUIRE(integerBst->insert(7));
    REQUIRE(integerBst->insert(6));
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(4));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->insert(2));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(0));

    // Verify the height of the tree
    REQUIRE(integerBst->height() == 10);
}

TEST_CASE ("MemoryNode Right-Only Insertion BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerBst->insert(0));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(2));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->insert(4));
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(6));
    REQUIRE(integerBst->insert(7));
    REQUIRE(integerBst->insert(8));
    REQUIRE(integerBst->insert(9));

    // Verify the height of the tree
    REQUIRE(integerBst->height() == 10);
}

TEST_CASE ("MemoryNode Delete No-Child Root Node BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->height() == 1);

    // Delete the root node
    REQUIRE(integerBst->remove(5));

    // Verify that the root was removed
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->height() == 0);
}

TEST_CASE ("MemoryNode Delete Left One-Child Root Node BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(2));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->height() == 3);

    // Delete the root node
    REQUIRE(integerBst->remove(5));

    // Verify that the root was removed
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->height() == 2);
}

TEST_CASE ("MemoryNode Delete Right One-Child Root Node BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(7));
    REQUIRE(integerBst->insert(6));
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->height() == 3);

    // Delete the root node
    REQUIRE(integerBst->remove(5));

    // Verify that the root was removed
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->height() == 2);
}

TEST_CASE ("MemoryNode Delete Two-Child Root Node BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(7));
    REQUIRE(integerBst->insert(2));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->height() == 3);

    // Delete the root node
    REQUIRE(integerBst->remove(5));

    // Verify that the root was removed
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->height() == 3);
}

TEST_CASE ("MemoryNode Delete Two-Child Node Left-Edge-Case BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(7));
    REQUIRE(integerBst->insert(2));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(4));
    REQUIRE(integerBst->insert(0));
    REQUIRE(integerBst->height() == 4);

    // Delete the the two-child node
    REQUIRE(integerBst->remove(2));

    // Verify that the child was removed
    REQUIRE(!integerBst->exists(2));
    REQUIRE(integerBst->height() == 3);

    // Verify tree state with an in-order traversal
    int index = 0;
    const int retItems[] = {0, 1, 4, 5, 7};
    auto treeTraversal = integerBst->traverse();
    while (treeTraversal->hasMoreItems())
        REQUIRE(treeTraversal->getNextItem() == retItems[index++]);
}

TEST_CASE ("MemoryNode Delete Two-Child Node Right-Edge-Case BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(2));
    REQUIRE(integerBst->insert(7));
    REQUIRE(integerBst->insert(6));
    REQUIRE(integerBst->insert(8));
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->height() == 4);

    // Delete the the two-child node
    REQUIRE(integerBst->remove(7));

    // Verify that the child was removed
    REQUIRE(!integerBst->exists(7));
    REQUIRE(integerBst->height() == 3);

    // Verify tree state with an in-order traversal
    int index = 0;
    const int retItems[] = {2, 5, 6, 8, 9};
    auto treeTraversal = integerBst->traverse();
    while (treeTraversal->hasMoreItems())
        REQUIRE(treeTraversal->getNextItem() == retItems[index++]);
}

TEST_CASE ("DiskNode Insertion and Existence BST Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Verify a value doesn't exist until after we add it
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->exists(5));

    // Verify that a different number doesn't exist in the tree
    REQUIRE(!integerBst->exists(10));
}

TEST_CASE ("DiskNode Multiple Value Insertion and Existence BST Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->insert(7));

    // Verify that all of the expected values exist
    // and that others (not added) don't
    REQUIRE(!integerBst->exists(0));
    REQUIRE(integerBst->exists(1));
    REQUIRE(!integerBst->exists(2));
    REQUIRE(integerBst->exists(3));
    REQUIRE(!integerBst->exists(4));
    REQUIRE(integerBst->exists(5));
    REQUIRE(!integerBst->exists(6));
    REQUIRE(integerBst->exists(7));
    REQUIRE(!integerBst->exists(8));
    REQUIRE(integerBst->exists(9));

    // Verify tree state with an in-order traversal
    int index = 0;
    const int retItems[] = {1, 3, 5, 7, 9};
    auto treeTraversal = integerBst->traverse();
    while (treeTraversal->hasMoreItems())
        REQUIRE(treeTraversal->getNextItem() == retItems[index++]);
}

TEST_CASE ("DiskNode Inserting Duplicates BST Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add a new value to the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->exists(5));

    // Verify that adding it again doesn't work
    REQUIRE(!integerBst->insert(5));
    REQUIRE(integerBst->exists(5));
}

TEST_CASE ("DiskNode Get Closest Items BST Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->insert(7));

    // Get the closest value to the provided ones
    REQUIRE(integerBst->closest(-100) == 1);
    REQUIRE(integerBst->closest(0) == 1);
    REQUIRE(integerBst->closest(1) == 1);
    REQUIRE(integerBst->closest(2) == 3);
    REQUIRE(integerBst->closest(3) == 3);
    REQUIRE(integerBst->closest(4) == 3);
    REQUIRE(integerBst->closest(5) == 5);
    REQUIRE(integerBst->closest(6) == 7);
    REQUIRE(integerBst->closest(7) == 7);
    REQUIRE(integerBst->closest(8) == 7);
    REQUIRE(integerBst->closest(9) == 9);
    REQUIRE(integerBst->closest(10) == 9);
    REQUIRE(integerBst->closest(100) == 9);
}

TEST_CASE ("DiskNode Deleting Elements BST Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->insert(7));

    // Delete a value from the tree
    REQUIRE(integerBst->remove(3));

    // Verify the state of the tree
    REQUIRE(integerBst->exists(1));
    REQUIRE(!integerBst->exists(3));
    REQUIRE(integerBst->exists(5));
    REQUIRE(integerBst->exists(7));
    REQUIRE(integerBst->exists(9));

    // Delete another value from the tree
    REQUIRE(integerBst->remove(5));

    // Verify the state of the tree
    REQUIRE(integerBst->exists(1));
    REQUIRE(!integerBst->exists(3));
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->exists(7));
    REQUIRE(integerBst->exists(9));

    // Verify that we can't delete an already-deleted item
    REQUIRE(!integerBst->remove(5));

    // Verify the state of the tree
    REQUIRE(integerBst->exists(1));
    REQUIRE(!integerBst->exists(3));
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->exists(7));
    REQUIRE(integerBst->exists(9));

    // Verify tree state with an in-order traversal
    int index = 0;
    const int retItems[] = {1, 7, 9};
    auto treeTraversal = integerBst->traverse();
    while (treeTraversal->hasMoreItems())
        REQUIRE(treeTraversal->getNextItem() == retItems[index++]);
}

TEST_CASE ("DiskNode Perform Search After Deletion BST Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->insert(7));

    // Delete a couple items
    REQUIRE(integerBst->remove(5));
    REQUIRE(integerBst->remove(9));

    // Get the closest value to the provided ones
    REQUIRE(integerBst->closest(-100) == 1);
    REQUIRE(integerBst->closest(0) == 1);
    REQUIRE(integerBst->closest(1) == 1);
    REQUIRE(integerBst->closest(2) == 3);
    REQUIRE(integerBst->closest(3) == 3);
    REQUIRE(integerBst->closest(4) == 3);
    REQUIRE(integerBst->closest(5) == 3);
    REQUIRE(integerBst->closest(6) == 7);
    REQUIRE(integerBst->closest(7) == 7);
    REQUIRE(integerBst->closest(8) == 7);
    REQUIRE(integerBst->closest(9) == 7);
    REQUIRE(integerBst->closest(10) == 7);
    REQUIRE(integerBst->closest(100) == 7);
}

TEST_CASE ("DiskNode Left-Only Insertion BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->insert(8));
    REQUIRE(integerBst->insert(7));
    REQUIRE(integerBst->insert(6));
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(4));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->insert(2));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(0));

    // Verify the height of the tree
    REQUIRE(integerBst->height() == 10);
}

TEST_CASE ("DiskNode Right-Only Insertion BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerBst->insert(0));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(2));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->insert(4));
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(6));
    REQUIRE(integerBst->insert(7));
    REQUIRE(integerBst->insert(8));
    REQUIRE(integerBst->insert(9));

    // Verify the height of the tree
    REQUIRE(integerBst->height() == 10);
}

TEST_CASE ("DiskNode Delete No-Child Root Node BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->height() == 1);

    // Delete the root node
    REQUIRE(integerBst->remove(5));

    // Verify that the root was removed
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->height() == 0);
}

TEST_CASE ("DiskNode Delete Left One-Child Root Node BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(2));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->height() == 3);

    // Delete the root node
    REQUIRE(integerBst->remove(5));

    // Verify that the root was removed
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->height() == 2);
}

TEST_CASE ("DiskNode Delete Right One-Child Root Node BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(7));
    REQUIRE(integerBst->insert(6));
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->height() == 3);

    // Delete the root node
    REQUIRE(integerBst->remove(5));

    // Verify that the root was removed
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->height() == 2);
}

TEST_CASE ("DiskNode Delete Two-Child Root Node BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(7));
    REQUIRE(integerBst->insert(2));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(3));
    REQUIRE(integerBst->height() == 3);

    // Delete the root node
    REQUIRE(integerBst->remove(5));

    // Verify that the root was removed
    REQUIRE(!integerBst->exists(5));
    REQUIRE(integerBst->height() == 3);
}

TEST_CASE ("DiskNode Delete Two-Child Node Left-Edge-Case BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(7));
    REQUIRE(integerBst->insert(2));
    REQUIRE(integerBst->insert(1));
    REQUIRE(integerBst->insert(4));
    REQUIRE(integerBst->insert(0));
    REQUIRE(integerBst->height() == 4);

    // Delete the the two-child node
    REQUIRE(integerBst->remove(2));

    // Verify that the child was removed
    REQUIRE(!integerBst->exists(2));
    REQUIRE(integerBst->height() == 3);

    // Verify tree state with an in-order traversal
    int index = 0;
    const int retItems[] = {0, 1, 4, 5, 7};
    auto treeTraversal = integerBst->traverse();
    while (treeTraversal->hasMoreItems())
        REQUIRE(treeTraversal->getNextItem() == retItems[index++]);
}

TEST_CASE ("DiskNode Delete Two-Child Node Right-Edge-Case BST Height Test", "[BinarySearchTreeTest]")
{

    // Create the integer BST instance
    auto integerBst = std::make_shared<BinarySearchTree<int, DiskNode>>();
    auto diskNodeAllocator = std::make_shared<DiskNode<int>::DiskNodeAllocator>();
    integerBst->overrideDefaultAllocator(diskNodeAllocator);

    // Add some new values into the tree
    REQUIRE(integerBst->insert(5));
    REQUIRE(integerBst->insert(2));
    REQUIRE(integerBst->insert(7));
    REQUIRE(integerBst->insert(6));
    REQUIRE(integerBst->insert(8));
    REQUIRE(integerBst->insert(9));
    REQUIRE(integerBst->height() == 4);

    // Delete the the two-child node
    REQUIRE(integerBst->remove(7));

    // Verify that the child was removed
    REQUIRE(!integerBst->exists(7));
    REQUIRE(integerBst->height() == 3);

    // Verify tree state with an in-order traversal
    int index = 0;
    const int retItems[] = {2, 5, 6, 8, 9};
    auto treeTraversal = integerBst->traverse();
    while (treeTraversal->hasMoreItems())
        REQUIRE(treeTraversal->getNextItem() == retItems[index++]);
}


#endif //BITBOSON_STANDARDMODEL_BINARYSEARCHTREE_TEST_HPP

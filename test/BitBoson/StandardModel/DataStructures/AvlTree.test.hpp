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

#ifndef BITBOSON_STANDARDMODEL_AVLTREE_TEST_HPP
#define BITBOSON_STANDARDMODEL_AVLTREE_TEST_HPP

#include <BitBoson/StandardModel/DataStructures/AvlTree.hpp>
#include <BitBoson/StandardModel/DataStructures/Containers/MemoryNode.hpp>

using namespace BitBoson::StandardModel;

TEST_CASE ("Left-Insertion Only AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

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

TEST_CASE ("Right-Insertion Only AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

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

TEST_CASE ("Left-Line Root AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(1));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 2);
}

TEST_CASE ("Left-Zig-Zag Root AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(1));
    REQUIRE(integerAvlTree->insert(3));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 2);
}

TEST_CASE ("Right-Line Root AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(9));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 2);
}

TEST_CASE ("Right-Zig-Zag Root AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(9));
    REQUIRE(integerAvlTree->insert(7));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 2);
}

TEST_CASE ("Left-Line Node AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(2));
    REQUIRE(integerAvlTree->insert(1));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Left-Zig-Zag Node AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(1));
    REQUIRE(integerAvlTree->insert(2));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Right-Line Node AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(8));
    REQUIRE(integerAvlTree->insert(9));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Right-Zig-Zag Node AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

    // Add some new values into the tree
    REQUIRE(integerAvlTree->insert(5));
    REQUIRE(integerAvlTree->insert(3));
    REQUIRE(integerAvlTree->insert(7));
    REQUIRE(integerAvlTree->insert(9));
    REQUIRE(integerAvlTree->insert(8));

    // Verify the height of the AVL tree
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Left-Line Deletion AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

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

TEST_CASE ("Left-Zig-Zag Deletion AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

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

TEST_CASE ("Right-Line Deletion AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

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

TEST_CASE ("Right-Zig-Zag Deletion AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

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

    // Delete the value 6 to force a right-zig-zag adjustment
    REQUIRE(integerAvlTree->remove(6));

    // Verify that the tree re-balanced
    REQUIRE(integerAvlTree->height() == 3);
}

TEST_CASE ("Root Deletion AVL Tree Balance", "[AvlTreeTest]")
{

    // Create the integer BST instance
    auto integerAvlTree = std::make_shared<AvlTree<int, MemoryNode>>();

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

    // Delete the value 5 to force a root deletion
    REQUIRE(integerAvlTree->remove(5));

    // Verify that the tree re-balanced
    REQUIRE(integerAvlTree->height() == 3);
}

#endif //BITBOSON_STANDARDMODEL_AVLTREE_TEST_HPP

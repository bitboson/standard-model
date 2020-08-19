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

#ifndef BITBOSON_STANDARDMODEL_DATATREE_TEST_HPP
#define BITBOSON_STANDARDMODEL_DATATREE_TEST_HPP

#include <string>
#include <boost/thread/thread_only.hpp>
#include <BitBoson/StandardModel/DataStructures/DataTree.hpp>

using namespace BitBoson::StandardModel;

TEST_CASE ("Auto-Generated Hashes Functionality Test", "[DataTreeTest]")
{

    // Create the actual data tree
    DataTree<std::string> dataTree;

    // Add nodes to the tree (with auto-generated hash)
    auto itemHash1 = dataTree.addItem("A", dataTree.getHeadHash());
    auto itemHash2 = dataTree.addItem("B", itemHash1);
    auto itemHash3 = dataTree.addItem("C", itemHash1);

    // Verify tree structure
    auto children = dataTree.getChildrenOfItem(itemHash1);
    REQUIRE(children.size() == 2);
    REQUIRE(children[0]->data == "C");
    REQUIRE(children[1]->data == "B");
}
TEST_CASE ("Adding an Invalid Item Test", "[DataTreeTest]")
{

    // Create the actual data tree
    DataTree<std::string> dataTree;

    // Add nodes to the tree (with auto-generated hash)
    auto itemHash = dataTree.addItem("A", "");
    REQUIRE(itemHash.empty());

    // Verify tree structure
    auto children = dataTree.getChildrenOfItem(dataTree.getHeadHash());
    REQUIRE(children.empty());
}

TEST_CASE ("Basic Tree Functionality Test", "[DataTreeTest]")
{

    // Create the actual data tree
    DataTree<std::string> dataTree;

    // Build up the data-tree structure:
    //
    //              A
    //             / \
    //            B   C
    //           /   / \
    //          D   H   I
    //         / \
    //        E   F
    //        |
    //        G
    //
    dataTree.addItem("A", dataTree.getHeadHash(), "A");
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    dataTree.addItem("B", "A", "B");
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    dataTree.addItem("C", "A", "C");
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    dataTree.addItem("D", "B", "D");
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    dataTree.addItem("E", "D", "E");
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    dataTree.addItem("F", "D", "F");
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    dataTree.addItem("G", "E", "G");
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    dataTree.addItem("H", "C", "H");
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    dataTree.addItem("I", "C", "I");

    // Verify that G is the "deepest" item
    REQUIRE(dataTree.getDeepestNode() == "G");

    // Check the data-tree's structure
    auto children = dataTree.getChildrenOfItem(dataTree.getHeadHash());
    REQUIRE(children.size() == 1);
    REQUIRE(children[0]->data == "A");
    children = dataTree.getChildrenOfItem("A");
    REQUIRE(children.size() == 2);
    REQUIRE(children[0]->data == "B");
    REQUIRE(children[1]->data == "C");
    children = dataTree.getChildrenOfItem("B");
    REQUIRE(children.size() == 1);
    REQUIRE(children[0]->data == "D");
    children = dataTree.getChildrenOfItem("C");
    REQUIRE(children.size() == 2);
    REQUIRE(children[0]->data == "H");
    REQUIRE(children[1]->data == "I");
    children = dataTree.getChildrenOfItem("D");
    REQUIRE(children.size() == 2);
    REQUIRE(children[0]->data == "E");
    REQUIRE(children[1]->data == "F");
    children = dataTree.getChildrenOfItem("E");
    REQUIRE(children.size() == 1);
    REQUIRE(children[0]->data == "G");
    children = dataTree.getChildrenOfItem("G");
    REQUIRE(children.size() == 0);
    children = dataTree.getChildrenOfItem("F");
    REQUIRE(children.size() == 0);
    children = dataTree.getChildrenOfItem("H");
    REQUIRE(children.size() == 0);
    children = dataTree.getChildrenOfItem("I");
    REQUIRE(children.size() == 0);
    REQUIRE(dataTree.isItemInTree("A"));
    REQUIRE(dataTree.isItemInTree("B"));
    REQUIRE(dataTree.isItemInTree("C"));
    REQUIRE(dataTree.isItemInTree("D"));
    REQUIRE(dataTree.isItemInTree("E"));
    REQUIRE(dataTree.isItemInTree("F"));
    REQUIRE(dataTree.isItemInTree("G"));
    REQUIRE(dataTree.isItemInTree("H"));
    REQUIRE(dataTree.isItemInTree("I"));
    REQUIRE(!dataTree.isItemALeaf("A"));
    REQUIRE(!dataTree.isItemALeaf("B"));
    REQUIRE(!dataTree.isItemALeaf("C"));
    REQUIRE(!dataTree.isItemALeaf("D"));
    REQUIRE(!dataTree.isItemALeaf("E"));
    REQUIRE(dataTree.isItemALeaf("F"));
    REQUIRE(dataTree.isItemALeaf("G"));
    REQUIRE(dataTree.isItemALeaf("H"));
    REQUIRE(dataTree.isItemALeaf("I"));
    auto siblings = dataTree.getSiblingsOfItem("A");
    REQUIRE(siblings.size() == 0);
    siblings = dataTree.getSiblingsOfItem("B");
    REQUIRE(siblings.size() == 1);
    REQUIRE(siblings[0] == "C");
    siblings = dataTree.getSiblingsOfItem("C");
    REQUIRE(siblings.size() == 1);
    REQUIRE(siblings[0] == "B");
    siblings = dataTree.getSiblingsOfItem("D");
    REQUIRE(siblings.size() == 0);
    siblings = dataTree.getSiblingsOfItem("E");
    REQUIRE(siblings.size() == 1);
    REQUIRE(siblings[0] == "F");
    siblings = dataTree.getSiblingsOfItem("F");
    REQUIRE(siblings.size() == 1);
    REQUIRE(siblings[0] == "E");
    siblings = dataTree.getSiblingsOfItem("G");
    REQUIRE(siblings.size() == 0);
    siblings = dataTree.getSiblingsOfItem("H");
    REQUIRE(siblings.size() == 1);
    REQUIRE(siblings[0] == "I");
    siblings = dataTree.getSiblingsOfItem("I");
    REQUIRE(siblings.size() == 1);
    REQUIRE(siblings[0] == "H");

    // Remove the "D" node
    //
    //              A
    //             / \
    //            B   C
    //               / \
    //              H   I
    //
    dataTree.deleteItem("D");

    // Verify that H is the "deepest" and "oldest" item
    REQUIRE(dataTree.getDeepestNode() == "H");

    // Verify that the tree is in the correct structure
    children = dataTree.getChildrenOfItem(dataTree.getHeadHash());
    REQUIRE(children.size() == 1);
    REQUIRE(children[0]->data == "A");
    children = dataTree.getChildrenOfItem("A");
    REQUIRE(children.size() == 2);
    REQUIRE(children[0]->data == "B");
    REQUIRE(children[1]->data == "C");
    children = dataTree.getChildrenOfItem("B");
    REQUIRE(children.size() == 0);
    children = dataTree.getChildrenOfItem("C");
    REQUIRE(children.size() == 2);
    REQUIRE(children[0]->data == "H");
    REQUIRE(children[1]->data == "I");
    children = dataTree.getChildrenOfItem("H");
    REQUIRE(children.size() == 0);
    children = dataTree.getChildrenOfItem("I");
    REQUIRE(children.size() == 0);
    REQUIRE(dataTree.isItemInTree("A"));
    REQUIRE(dataTree.isItemInTree("B"));
    REQUIRE(dataTree.isItemInTree("C"));
    REQUIRE(!dataTree.isItemInTree("D"));
    REQUIRE(!dataTree.isItemInTree("E"));
    REQUIRE(!dataTree.isItemInTree("F"));
    REQUIRE(!dataTree.isItemInTree("G"));
    REQUIRE(dataTree.isItemInTree("H"));
    REQUIRE(dataTree.isItemInTree("I"));
    REQUIRE(!dataTree.isItemALeaf("A"));
    REQUIRE(dataTree.isItemALeaf("B"));
    REQUIRE(!dataTree.isItemALeaf("C"));
    REQUIRE(dataTree.isItemALeaf("H"));
    REQUIRE(dataTree.isItemALeaf("I"));
    siblings = dataTree.getSiblingsOfItem("A");
    REQUIRE(siblings.size() == 0);
    siblings = dataTree.getSiblingsOfItem("B");
    REQUIRE(siblings.size() == 1);
    REQUIRE(siblings[0] == "C");
    siblings = dataTree.getSiblingsOfItem("C");
    REQUIRE(siblings.size() == 1);
    REQUIRE(siblings[0] == "B");
    siblings = dataTree.getSiblingsOfItem("H");
    REQUIRE(siblings.size() == 1);
    REQUIRE(siblings[0] == "I");
    siblings = dataTree.getSiblingsOfItem("I");
    REQUIRE(siblings.size() == 1);
    REQUIRE(siblings[0] == "H");

    // Remove the "C" node, leaving "H" and "I"
    //
    //              A
    //             /|\
    //            / | \
    //           B  H  I
    //
    dataTree.deleteItem("C", false);

    // Verify that B is the "deepest" and "oldest" item
    REQUIRE(dataTree.getDeepestNode() == "B");

    // Verify that the tree is in the correct structure
    children = dataTree.getChildrenOfItem(dataTree.getHeadHash());
    REQUIRE(children.size() == 1);
    REQUIRE(children[0]->data == "A");
    children = dataTree.getChildrenOfItem("A");
    REQUIRE(children.size() == 3);
    REQUIRE(children[0]->data == "H");
    REQUIRE(children[1]->data == "B");
    REQUIRE(children[2]->data == "I");
    children = dataTree.getChildrenOfItem("B");
    REQUIRE(children.size() == 0);
    children = dataTree.getChildrenOfItem("H");
    REQUIRE(children.size() == 0);
    children = dataTree.getChildrenOfItem("I");
    REQUIRE(children.size() == 0);
    REQUIRE(dataTree.isItemInTree("A"));
    REQUIRE(dataTree.isItemInTree("B"));
    REQUIRE(!dataTree.isItemInTree("C"));
    REQUIRE(!dataTree.isItemInTree("D"));
    REQUIRE(!dataTree.isItemInTree("E"));
    REQUIRE(!dataTree.isItemInTree("F"));
    REQUIRE(!dataTree.isItemInTree("G"));
    REQUIRE(dataTree.isItemInTree("H"));
    REQUIRE(dataTree.isItemInTree("I"));
    REQUIRE(!dataTree.isItemALeaf("A"));
    REQUIRE(dataTree.isItemALeaf("B"));
    REQUIRE(dataTree.isItemALeaf("H"));
    REQUIRE(dataTree.isItemALeaf("I"));
    siblings = dataTree.getSiblingsOfItem("A");
    REQUIRE(siblings.size() == 0);
    siblings = dataTree.getSiblingsOfItem("B");
    REQUIRE(siblings.size() == 2);
    REQUIRE(siblings[0] == "H");
    REQUIRE(siblings[1] == "I");
    siblings = dataTree.getSiblingsOfItem("H");
    REQUIRE(siblings.size() == 2);
    REQUIRE(siblings[0] == "B");
    REQUIRE(siblings[1] == "I");
    siblings = dataTree.getSiblingsOfItem("I");
    REQUIRE(siblings.size() == 2);
    REQUIRE(siblings[0] == "H");
    REQUIRE(siblings[1] == "B");
}

TEST_CASE ("Recursive Child Tree Listing Test", "[DataTreeTest]")
{

    // Create the actual data tree
    DataTree<std::string> dataTree("0");

    // Build up the data-tree structure:
    //
    //              A
    //             / \
    //            B   C
    //           /   / \
    //          D   H   I
    //         / \
    //        E   F
    //        |
    //        G
    //
    dataTree.addItem("A", "0", "A");
    dataTree.addItem("B", "A", "B");
    dataTree.addItem("C", "A", "C");
    dataTree.addItem("D", "B", "D");
    dataTree.addItem("E", "D", "E");
    dataTree.addItem("F", "D", "F");
    dataTree.addItem("G", "E", "G");
    dataTree.addItem("H", "C", "H");
    dataTree.addItem("I", "C", "I");

    // Verify that G is the "deepest" item
    REQUIRE(dataTree.getDeepestNode() == "G");

    // Recursively list all children in the tree
    // and verify the results are correct
    auto childListing = dataTree.getChildrenOfItem("A", true);
    REQUIRE (childListing.size() == 8);
    REQUIRE (childListing[0]->data == "B");
    REQUIRE (childListing[1]->data == "C");
    REQUIRE (childListing[2]->data == "D");
    REQUIRE (childListing[3]->data == "E");
    REQUIRE (childListing[4]->data == "F");
    REQUIRE (childListing[5]->data == "G");
    REQUIRE (childListing[6]->data == "H");
    REQUIRE (childListing[7]->data == "I");

    // Recursively list all children of B
    // and verify the results are correct
    childListing = dataTree.getChildrenOfItem("B", true);
    REQUIRE (childListing.size() == 4);
    REQUIRE (childListing[0]->data == "D");
    REQUIRE (childListing[1]->data == "E");
    REQUIRE (childListing[2]->data == "F");
    REQUIRE (childListing[3]->data == "G");

    // Recursively list all children of C
    // and verify the results are correct
    childListing = dataTree.getChildrenOfItem("C", true);
    REQUIRE (childListing.size() == 2);
    REQUIRE (childListing[0]->data == "H");
    REQUIRE (childListing[1]->data == "I");

    // Recursively list all children of D
    // and verify the results are correct
    childListing = dataTree.getChildrenOfItem("D", true);
    REQUIRE (childListing.size() == 3);
    REQUIRE (childListing[0]->data == "E");
    REQUIRE (childListing[1]->data == "F");
    REQUIRE (childListing[2]->data == "G");

    // Recursively list all children of E
    // and verify the results are correct
    childListing = dataTree.getChildrenOfItem("E", true);
    REQUIRE (childListing.size() == 1);
    REQUIRE (childListing[0]->data == "G");

    // Recursively list all children of G
    // and verify the results are correct
    childListing = dataTree.getChildrenOfItem("G", true);
    REQUIRE (childListing.size() == 0);

    // Recursively list all children of F
    // and verify the results are correct
    childListing = dataTree.getChildrenOfItem("F", true);
    REQUIRE (childListing.size() == 0);

    // Recursively list all children of H
    // and verify the results are correct
    childListing = dataTree.getChildrenOfItem("H", true);
    REQUIRE (childListing.size() == 0);

    // Recursively list all children of I
    // and verify the results are correct
    childListing = dataTree.getChildrenOfItem("I", true);
    REQUIRE (childListing.size() == 0);
}

#endif //BITBOSON_STANDARDMODEL_DATATREE_TEST_HPP

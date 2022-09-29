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

#ifndef BITBOSON_STANDARDMODEL_DATASTORE_TEST_HPP
#define BITBOSON_STANDARDMODEL_DATASTORE_TEST_HPP

#include <memory>
#include <BitBoson/StandardModel/Crypto/Crypto.h>
#include <BitBoson/StandardModel/Storage/DataStore.h>
#include <BitBoson/StandardModel/FileSystem/FileSystem.h>

using namespace BitBoson::StandardModel;

/**
 * Helper function used to get a string of a particular size
 * NOTE: The returned string is a random one
 *
 * @param size Integer representing the size of the string
 * @return String representing the desired sized-string
 */
std::string getSizedString(int size)
{

    // Create a return character array
    std::string retString;

    // Get a random hash
    auto randomHash = Crypto::getRandomSha256();

    // Create the string as desired
    for (int ii = 0; ii < size; ii++)
        retString += randomHash[ii % (randomHash.size())];

    // Return the return string
    return retString;
}

TEST_CASE ("General Data-Store with Existing Directory Test", "[DataStoreTest]")
{

    // Create a new temporary directory to use
    auto tempDir = FileSystem::getTemporaryDir("BitBoson");

    // Create a data-store on the temporary directory
    auto dataStore = DataStore(tempDir.getFullPath());

    // Insert some data in the data-store
    REQUIRE(dataStore.addItem("Key1", "Value1"));
    REQUIRE(dataStore.addItem("Key2", "Value2"));
    REQUIRE(dataStore.addItem("Key3", "Value3"));

    // Retrieve the data from the data-store
    REQUIRE(dataStore.getItem("Key1") == "Value1");
    REQUIRE(dataStore.getItem("Key2") == "Value2");
    REQUIRE(dataStore.getItem("Key3") == "Value3");

    // Remove the temporary directory (cleanup)
    tempDir.removeDir();
}

TEST_CASE ("General Data-Store with Non-Existing Directory Test", "[DataStoreTest]")
{

    // Create a new temporary directory to use
    auto tempDir = FileSystem::getTemporaryDir("BitBoson");

    // Create a data-store on the temporary directory
    auto dataStore = DataStore(tempDir.getFullPath(), true);

    // Insert some data in the data-store
    REQUIRE(dataStore.addItem("Key1", "Value1"));
    REQUIRE(dataStore.addItem("Key2", "Value2"));
    REQUIRE(dataStore.addItem("Key3", "Value3"));

    // Retrieve the data from the data-store
    REQUIRE(dataStore.getItem("Key1") == "Value1");
    REQUIRE(dataStore.getItem("Key2") == "Value2");
    REQUIRE(dataStore.getItem("Key3") == "Value3");

    // Remove the temporary directory (cleanup)
    tempDir.removeDir();
}

TEST_CASE ("Completely Delete Data-Store Test", "[DataStoreTest]")
{

    // Create a new temporary directory to use
    auto tempDir = FileSystem::getTemporaryDir("BitBoson");

    // Create a data-store on the temporary directory
    auto dataStore = DataStore(tempDir.getFullPath(), true);

    // Insert some data in the data-store
    REQUIRE(dataStore.addItem("Key1", "Value1"));
    REQUIRE(dataStore.addItem("Key2", "Value2"));
    REQUIRE(dataStore.addItem("Key3", "Value3"));

    // Retrieve the data from the data-store
    REQUIRE(dataStore.getItem("Key1") == "Value1");
    REQUIRE(dataStore.getItem("Key2") == "Value2");
    REQUIRE(dataStore.getItem("Key3") == "Value3");

    // Remove the temporary directory (cleanup)
    dataStore.deleteEntireDataStore();
    REQUIRE(!tempDir.exists());
}

TEST_CASE ("Completely Delete Data-Store (and re-create Directory) Test", "[DataStoreTest]")
{

    // Create a new temporary directory to use
    auto tempDir = FileSystem::getTemporaryDir("BitBoson");

    // Create a data-store on the temporary directory
    auto dataStore = DataStore(tempDir.getFullPath(), true);

    // Insert some data in the data-store
    REQUIRE(dataStore.addItem("Key1", "Value1"));
    REQUIRE(dataStore.addItem("Key2", "Value2"));
    REQUIRE(dataStore.addItem("Key3", "Value3"));

    // Retrieve the data from the data-store
    REQUIRE(dataStore.getItem("Key1") == "Value1");
    REQUIRE(dataStore.getItem("Key2") == "Value2");
    REQUIRE(dataStore.getItem("Key3") == "Value3");

    // Recreate the data-store
    dataStore.deleteEntireDataStore(true);
    REQUIRE(tempDir.exists());

    // Remove the temporary directory (cleanup)
    dataStore.deleteEntireDataStore();
    REQUIRE(!tempDir.exists());
}

TEST_CASE ("Duplicate Inserts and Overwrites Data-Store Test", "[DataStoreTest]")
{

    // Create a new temporary directory to use
    auto tempDir = FileSystem::getTemporaryDir("BitBoson");

    // Create a data-store on the temporary directory
    auto dataStore = DataStore(tempDir.getFullPath(), true);

    // Insert a value and try to overwrite it
    REQUIRE(dataStore.addItem("Key1", "Value1"));
    REQUIRE(!dataStore.addItem("Key1", "Value2"));

    // Verify that the original value entered is still present
    REQUIRE(dataStore.getItem("Key1") == "Value1");

    // Overwrite the value and verify that it is overwritten
    REQUIRE(dataStore.addItem("Key1", "Value2", true));
    REQUIRE(dataStore.getItem("Key1") == "Value2");

    // Remove the temporary directory (cleanup)
    tempDir.removeDir();
}

TEST_CASE ("Add and Delete Items Data-Store Test", "[DataStoreTest]")
{

    // Create a new temporary directory to use
    auto tempDir = FileSystem::getTemporaryDir("BitBoson");

    // Create a data-store on the temporary directory
    auto dataStore = DataStore(tempDir.getFullPath(), true);

    // Insert some data in the data-store
    REQUIRE(dataStore.addItem("Key1", "Value1"));
    REQUIRE(dataStore.addItem("Key2", "Value2"));
    REQUIRE(dataStore.addItem("Key3", "Value3"));

    // Retrieve the data from the data-store
    REQUIRE(dataStore.getItem("Key1") == "Value1");
    REQUIRE(dataStore.getItem("Key2") == "Value2");
    REQUIRE(dataStore.getItem("Key3") == "Value3");

    // Delete a few items
    REQUIRE(dataStore.deleteItem("Key1"));
    REQUIRE(dataStore.deleteItem("Key2"));

    // Verify that the items are deleted
    REQUIRE(dataStore.getItem("Key1").empty());
    REQUIRE(dataStore.getItem("Key2").empty());
    REQUIRE(dataStore.getItem("Key3") == "Value3");

    // Remove the temporary directory (cleanup)
    tempDir.removeDir();
}

TEST_CASE ("Default Return Values Data-Store Test", "[DataStoreTest]")
{

    // Create a new temporary directory to use
    auto tempDir = FileSystem::getTemporaryDir("BitBoson");

    // Create a data-store on the temporary directory
    auto dataStore = DataStore(tempDir.getFullPath(), true);

    // Verify that bad-gets (non-existing items) returns default value
    REQUIRE(dataStore.getItem("Key1").empty());
    REQUIRE(dataStore.getItem("Key1", "") == "");
    REQUIRE(dataStore.getItem("Key1", "Value1") == "Value1");
    REQUIRE(dataStore.getItem("Key1", "Key1") == "Key1");

    // Remove the temporary directory (cleanup)
    tempDir.removeDir();
}

#endif //BITBOSON_STANDARDMODEL_DATASTORE_TEST_HPP

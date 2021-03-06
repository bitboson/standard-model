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

TEST_CASE ("Data-Store Get/Set Chunks", "[DataStoreTest]")
{

    // Create a new temporary directory to use
    auto tempDir = FileSystem::getTemporaryDir("BitBoson");

    // Create a data-store on the temporary directory
    auto dataStore = DataStore(tempDir.getFullPath(), true);

    // Create a bunch or randomly-sized strings
    auto value0 = getSizedString(100);
    auto value1 = getSizedString(100);
    auto value2 = getSizedString(100);
    auto value3 = getSizedString(999967);
    auto value4 = getSizedString(999968);
    auto value5 = getSizedString(999969);
    auto value6 = getSizedString(999935);
    auto value7 = getSizedString(999936);
    auto value8 = getSizedString(999937);
    auto value9 = getSizedString(9999999);
    auto value10 = getSizedString(1000000);
    auto value11 = getSizedString(1000001);

    // Add the variously-sized items to the data-store
    REQUIRE(dataStore.addItem("Value0", value0));
    REQUIRE(dataStore.addItem("Value1", value1));
    REQUIRE(dataStore.addItem("Value2", value2));
    REQUIRE(dataStore.addItem("Value3", value3));
    REQUIRE(dataStore.addItem("Value4", value4));
    REQUIRE(dataStore.addItem("Value5", value5));
    REQUIRE(dataStore.addItem("Value6", value6));
    REQUIRE(dataStore.addItem("Value7", value7));
    REQUIRE(dataStore.addItem("Value8", value8));
    REQUIRE(dataStore.addItem("Value9", value9));
    REQUIRE(dataStore.addItem("Value10", value10));
    REQUIRE(dataStore.addItem("Value11", value11));

    // Create a second data-store instance
    auto tempDir2 = FileSystem::getTemporaryDir("BitBoson");
    auto dataStore2 = DataStore(tempDir2.getFullPath(), true);

    // Chunk-up the data-store and re-create it into a second one
    dataStore2.setChunkedData(dataStore.getChunkedData(250));

    // Verify that the second data-store has the same data as the first
    REQUIRE(dataStore2.getItem("Value0") == value0);
    REQUIRE(dataStore2.getItem("Value1") == value1);
    REQUIRE(dataStore2.getItem("Value2") == value2);
    REQUIRE(dataStore2.getItem("Value3") == value3);
    REQUIRE(dataStore2.getItem("Value4") == value4);
    REQUIRE(dataStore2.getItem("Value5") == value5);
    REQUIRE(dataStore2.getItem("Value6") == value6);
    REQUIRE(dataStore2.getItem("Value7") == value7);
    REQUIRE(dataStore2.getItem("Value8") == value8);
    REQUIRE(dataStore2.getItem("Value9") == value9);
    REQUIRE(dataStore2.getItem("Value10") == value10);
    REQUIRE(dataStore2.getItem("Value11") == value11);

    // Remove the temporary directory (cleanup)
    tempDir.removeDir();
    tempDir2.removeDir();
}

TEST_CASE ("Next/Prev-Iterator Key Data-Store Test", "[DataStoreTest]")
{

    // Create a new temporary directory to use
    auto tempDir = FileSystem::getTemporaryDir("BitBoson");

    // Create a data-store on the temporary directory
    auto dataStore = DataStore(tempDir.getFullPath());

    // Insert some data in the data-store
    REQUIRE(dataStore.addItem("Key1", "Value1"));
    REQUIRE(dataStore.addItem("Key2", "Value2"));
    REQUIRE(dataStore.addItem("Key3", "Value3"));
    REQUIRE(dataStore.addItem("Key4", "Value4"));
    REQUIRE(dataStore.addItem("Key5", "Value5"));
    REQUIRE(dataStore.addItem("Key6", "Value6"));
    REQUIRE(dataStore.addItem("Key7", "Value7"));

    // Iterate over all of the next-item keys starting
    // at the first item in the data-store
    auto nextIter = dataStore.getNextIterator("Key1");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key1");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key2");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key3");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key4");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key5");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key6");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key7");

    // Iterate over some of the next-item keys starting
    // at the fourth item in the data-store
    nextIter = dataStore.getNextIterator("Key4");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key4");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key5");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key6");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key7");

    // Iterate over all of the previous-item keys starting
    // at the last item in the data-store
    auto prevIter = dataStore.getPreviousIterator("Key7");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key7");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key6");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key5");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key4");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key3");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key2");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key1");

    // Iterate over some of the previous-item keys starting
    // at the fourth item in the data-store
    prevIter = dataStore.getPreviousIterator("Key4");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key4");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key3");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key2");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key1");

    // Remove the temporary directory (cleanup)
    tempDir.removeDir();
}

TEST_CASE ("Incomplete Next/Prev-Iterator Key Data-Store Test", "[DataStoreTest]")
{

    // Create a new temporary directory to use
    auto tempDir = FileSystem::getTemporaryDir("BitBoson");

    // Create a data-store on the temporary directory
    auto dataStore = DataStore(tempDir.getFullPath());

    // Insert some data in the data-store
    REQUIRE(dataStore.addItem("Key1", "Value1"));
    REQUIRE(dataStore.addItem("Key2", "Value2"));
    REQUIRE(dataStore.addItem("Key3", "Value3"));
    REQUIRE(dataStore.addItem("Key4", "Value4"));
    REQUIRE(dataStore.addItem("Key5", "Value5"));
    REQUIRE(dataStore.addItem("Key6", "Value6"));
    REQUIRE(dataStore.addItem("Key7", "Value7"));

    // Iterate over all of the next-item keys starting
    // at the last item to see that the iterator behaves
    auto nextIter = dataStore.getNextIterator("Key7");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key7");
    REQUIRE(!nextIter->hasMoreItems());

    // Iterate over all of the next-item keys starting
    // at the first item and exiting early
    nextIter = dataStore.getNextIterator("Key1");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key1");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key2");
    REQUIRE(nextIter->hasMoreItems());
    REQUIRE(nextIter->getNextItem() == "Key3");
    REQUIRE(nextIter->hasMoreItems());
    nextIter->quitRemainingItems();

    // Iterate over all of the previous-item keys starting
    // at the first item to see that the iterator behaves
    auto prevIter = dataStore.getPreviousIterator("Key1");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key1");
    REQUIRE(!prevIter->hasMoreItems());

    // Iterate over all of the next-item keys starting
    // at the first item and exiting early
    prevIter = dataStore.getPreviousIterator("Key7");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key7");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key6");
    REQUIRE(prevIter->hasMoreItems());
    REQUIRE(prevIter->getNextItem() == "Key5");
    REQUIRE(prevIter->hasMoreItems());
    prevIter->quitRemainingItems();

    // Remove the temporary directory (cleanup)
    tempDir.removeDir();
}

TEST_CASE ("Next/Previous Key Data-Store Test", "[DataStoreTest]")
{

    // Create a new temporary directory to use
    auto tempDir = FileSystem::getTemporaryDir("BitBoson");

    // Create a data-store on the temporary directory
    auto dataStore = DataStore(tempDir.getFullPath());

    // Insert some data in the data-store
    REQUIRE(dataStore.addItem("Key1", "Value1"));
    REQUIRE(dataStore.addItem("Key2", "Value2"));
    REQUIRE(dataStore.addItem("Key3", "Value3"));
    REQUIRE(dataStore.addItem("Key4", "Value4"));
    REQUIRE(dataStore.addItem("Key5", "Value5"));
    REQUIRE(dataStore.addItem("Key6", "Value6"));
    REQUIRE(dataStore.addItem("Key7", "Value7"));

    // Get the next key for each given key and validate
    REQUIRE(dataStore.getNextKey("Key0").empty());
    REQUIRE(dataStore.getNextKey("Key1") == "Key2");
    REQUIRE(dataStore.getNextKey("Key2") == "Key3");
    REQUIRE(dataStore.getNextKey("Key3") == "Key4");
    REQUIRE(dataStore.getNextKey("Key4") == "Key5");
    REQUIRE(dataStore.getNextKey("Key5") == "Key6");
    REQUIRE(dataStore.getNextKey("Key6") == "Key7");
    REQUIRE(dataStore.getNextKey("Key7").empty());

    // Get the previous key for each given key and validate
    REQUIRE(dataStore.getPreviousKey("Key1").empty());
    REQUIRE(dataStore.getPreviousKey("Key2") == "Key1");
    REQUIRE(dataStore.getPreviousKey("Key3") == "Key2");
    REQUIRE(dataStore.getPreviousKey("Key4") == "Key3");
    REQUIRE(dataStore.getPreviousKey("Key5") == "Key4");
    REQUIRE(dataStore.getPreviousKey("Key6") == "Key5");
    REQUIRE(dataStore.getPreviousKey("Key7") == "Key6");
    REQUIRE(dataStore.getPreviousKey("Key8").empty());

    // Remove the temporary directory (cleanup)
    tempDir.removeDir();
}

#endif //BITBOSON_STANDARDMODEL_DATASTORE_TEST_HPP

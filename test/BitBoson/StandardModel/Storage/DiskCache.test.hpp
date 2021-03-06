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

#ifndef BITBOSON_STANDARDMODEL_DISKCACHE_TEST_HPP
#define BITBOSON_STANDARDMODEL_DISKCACHE_TEST_HPP

#include <memory>
#include <BitBoson/StandardModel/Storage/DiskCache.h>

using namespace BitBoson::StandardModel;

TEST_CASE ("Add Items to the Disk Cache", "[DiskCacheTest]")
{

    // Create a disk-cache
    auto diskCache = DiskCache();

    // Insert items into the disk cache
    REQUIRE(diskCache.addItem("Key1", "Value1"));
    REQUIRE(diskCache.addItem("Key2", "Value2"));
    REQUIRE(diskCache.addItem("Key3", "Value3"));

    // Verify the items are in the cache
    REQUIRE(diskCache.getItem("Key1") == "Value1");
    REQUIRE(diskCache.getItem("Key2") == "Value2");
    REQUIRE(diskCache.getItem("Key3") == "Value3");
}

TEST_CASE ("Add and Remove Items From the Disk Cache", "[DiskCacheTest]")
{

    // Create a disk-cache
    auto diskCache = DiskCache();

    // Insert items into the disk cache
    REQUIRE(diskCache.addItem("Key1", "Value1"));
    REQUIRE(diskCache.addItem("Key2", "Value2"));
    REQUIRE(diskCache.addItem("Key3", "Value3"));

    // Verify the items are in the cache
    REQUIRE(diskCache.getItem("Key1") == "Value1");
    REQUIRE(diskCache.getItem("Key2") == "Value2");
    REQUIRE(diskCache.getItem("Key3") == "Value3");

    // Remove a few items from the cache
    REQUIRE(diskCache.deleteItem("Key1"));
    REQUIRE(diskCache.deleteItem("Key2"));

    // Verify the items were deleted from the cache
    REQUIRE(diskCache.getItem("Key1").empty());
    REQUIRE(diskCache.getItem("Key2").empty());
    REQUIRE(diskCache.getItem("Key3") == "Value3");
}

TEST_CASE ("Replace Items to the Disk Cache", "[DiskCacheTest]")
{

    // Create a disk-cache
    auto diskCache = DiskCache();

    // Insert an item in the cache and verify it is there
    REQUIRE(diskCache.addItem("Key1", "Value1"));
    REQUIRE(diskCache.getItem("Key1") == "Value1");

    // Put a new item in the cache and verify it has changed
    REQUIRE(diskCache.addItem("Key1", "Value2"));
    REQUIRE(diskCache.getItem("Key1") == "Value2");
}

TEST_CASE ("Get the Underlying DataStore for the Disk Cache", "[DiskCacheTest]")
{

    // Create a disk-cache
    auto diskCache = DiskCache();

    // Insert an item in the cache and verify it is there
    REQUIRE(diskCache.addItem("Key1", "Value1"));
    REQUIRE(diskCache.getItem("Key1") == "Value1");
    REQUIRE(diskCache.getUnderlyingDataStoreRef()->getItem("Key1") == "Value1");

    // Put a new item in the cache and verify it has changed
    REQUIRE(diskCache.addItem("Key1", "Value2"));
    REQUIRE(diskCache.getItem("Key1") == "Value2");
    REQUIRE(diskCache.getUnderlyingDataStoreRef()->getItem("Key1") == "Value2");
}

TEST_CASE ("Re-Initialize Persistent Disk Cache", "[DiskCacheTest]")
{

    // Create a disk-cache
    auto diskCache = std::make_shared<DiskCache>();

    // Setup the disk-cache as persistent
    diskCache->setPersistOnDestruction(true);
    auto originalDir = diskCache->getCacheDirectory();

    // Insert items into the disk cache
    REQUIRE(diskCache->addItem("Key1", "Value1"));
    REQUIRE(diskCache->addItem("Key2", "Value2"));
    REQUIRE(diskCache->addItem("Key3", "Value3"));

    // Verify the items are in the cache
    REQUIRE(diskCache->getItem("Key1") == "Value1");
    REQUIRE(diskCache->getItem("Key2") == "Value2");
    REQUIRE(diskCache->getItem("Key3") == "Value3");

    // Destroy the disk-cache and re-create it on the old directory
    diskCache = nullptr;
    auto diskCache2 = std::make_shared<DiskCache>(originalDir);

    // Verify the items are in the cache
    REQUIRE(diskCache2->getItem("Key1") == "Value1");
    REQUIRE(diskCache2->getItem("Key2") == "Value2");
    REQUIRE(diskCache2->getItem("Key3") == "Value3");
}

#endif //BITBOSON_STANDARDMODEL_DISKCACHE_TEST_HPP

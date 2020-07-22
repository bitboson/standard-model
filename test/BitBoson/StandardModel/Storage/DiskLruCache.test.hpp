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

#ifndef BITBOSON_STANDARDMODEL_DISKLRUCACHE_TEST_HPP
#define BITBOSON_STANDARDMODEL_DISKLRUCACHE_TEST_HPP

#include <BitBoson/StandardModel/Storage/DiskLruCache.h>

using namespace BitBoson::StandardModel;

class DummyCacheSupplier : public DiskLruCache::DiskLruCacheSupplier
{
    private:
        std::unordered_map<std::string, std::string> _dataMap;
    public:
        bool addItem(const std::string& key, const std::string& item) override
        { _dataMap[key] = item; return true; }
        std::string getItem(const std::string& key) override
        { return _dataMap[key]; }
        bool deleteItem(const std::string& key) override
        { bool retFlag = !_dataMap[key].empty(); _dataMap.erase(key); return retFlag; }
        virtual ~DummyCacheSupplier() = default;
};

TEST_CASE ("General LRU Cache Operation", "[DiskLruCacheTest]")
{

    // Create the LRU Cache instance
    auto cacheSupplier = std::make_shared<DummyCacheSupplier>();
    auto diskLruCache = std::make_shared<DiskLruCache>(cacheSupplier, 50, 25);

    // Add some items to the cache
    REQUIRE(diskLruCache->addItem("Key0", "Value0"));
    REQUIRE(diskLruCache->addItem("Key1", "Value1"));
    REQUIRE(diskLruCache->addItem("Key2", "Value2"));
    REQUIRE(diskLruCache->addItem("Key3", "Value3"));
    REQUIRE(diskLruCache->addItem("Key4", "Value4"));
    REQUIRE(diskLruCache->addItem("Key5", "Value5"));
    REQUIRE(diskLruCache->addItem("Key6", "Value6"));
    REQUIRE(diskLruCache->addItem("Key7", "Value7"));
    REQUIRE(diskLruCache->addItem("Key8", "Value8"));
    REQUIRE(diskLruCache->addItem("Key9", "Value9"));

    // Get all items from the cache
    REQUIRE(diskLruCache->getItem("Key0") == "Value0");
    REQUIRE(diskLruCache->getItem("Key1") == "Value1");
    REQUIRE(diskLruCache->getItem("Key2") == "Value2");
    REQUIRE(diskLruCache->getItem("Key3") == "Value3");
    REQUIRE(diskLruCache->getItem("Key4") == "Value4");
    REQUIRE(diskLruCache->getItem("Key5") == "Value5");
    REQUIRE(diskLruCache->getItem("Key6") == "Value6");
    REQUIRE(diskLruCache->getItem("Key7") == "Value7");
    REQUIRE(diskLruCache->getItem("Key8") == "Value8");
    REQUIRE(diskLruCache->getItem("Key9") == "Value9");

    // Delete some items from the cache
    REQUIRE(diskLruCache->deleteItem("Key0"));
    REQUIRE(diskLruCache->deleteItem("Key1"));
    REQUIRE(diskLruCache->deleteItem("Key5"));
    REQUIRE(diskLruCache->deleteItem("Key7"));
    REQUIRE(diskLruCache->deleteItem("Key8"));

    // Verify that we cannot re-delete already deleted items
    REQUIRE(!diskLruCache->deleteItem("Key0"));
    REQUIRE(!diskLruCache->deleteItem("Key1"));
    REQUIRE(!diskLruCache->deleteItem("Key8"));

    // Verify that the correct items were deleted
    REQUIRE(diskLruCache->getItem("Key0").empty());
    REQUIRE(diskLruCache->getItem("Key1").empty());
    REQUIRE(diskLruCache->getItem("Key2") == "Value2");
    REQUIRE(diskLruCache->getItem("Key3") == "Value3");
    REQUIRE(diskLruCache->getItem("Key4") == "Value4");
    REQUIRE(diskLruCache->getItem("Key5").empty());
    REQUIRE(diskLruCache->getItem("Key6") == "Value6");
    REQUIRE(diskLruCache->getItem("Key7").empty());
    REQUIRE(diskLruCache->getItem("Key8").empty());
    REQUIRE(diskLruCache->getItem("Key9") == "Value9");
}

TEST_CASE ("LRU Cache Write-Back Test", "[DiskLruCacheTest]")
{

    // Create the LRU Cache instance
    auto cacheSupplier = std::make_shared<DummyCacheSupplier>();
    auto diskLruCache = std::make_shared<DiskLruCache>(cacheSupplier, 50, 25);

    // Add some items to the cache
    REQUIRE(diskLruCache->addItem("Key0", "Value0"));
    REQUIRE(diskLruCache->addItem("Key1", "Value1", false));
    REQUIRE(diskLruCache->addItem("Key2", "Value2", true));
    REQUIRE(diskLruCache->addItem("Key3", "Value3", true));

    // Check the expected write-back items
    REQUIRE(cacheSupplier->getItem("Key0").empty());
    REQUIRE(cacheSupplier->getItem("Key1").empty());
    REQUIRE(cacheSupplier->getItem("Key2") == "Value2");
    REQUIRE(cacheSupplier->getItem("Key3") == "Value3");

    // Add enough items to force the first two values to be written back
    REQUIRE(diskLruCache->addItem("Key4", "Value4"));
    REQUIRE(diskLruCache->addItem("Key5", "Value5"));
    REQUIRE(diskLruCache->addItem("Key6", "Value6"));
    REQUIRE(diskLruCache->addItem("Key7", "Value7"));
    REQUIRE(diskLruCache->addItem("Key8", "Value8"));
    REQUIRE(diskLruCache->addItem("Key9", "Value9"));

    // Check the expected write-back items
    REQUIRE(cacheSupplier->getItem("Key0") == "Value0");
    REQUIRE(cacheSupplier->getItem("Key1") == "Value1");
    REQUIRE(cacheSupplier->getItem("Key2") == "Value2");
    REQUIRE(cacheSupplier->getItem("Key3") == "Value3");
    REQUIRE(cacheSupplier->getItem("Key4").empty());
    REQUIRE(cacheSupplier->getItem("Key5").empty());
    REQUIRE(cacheSupplier->getItem("Key6").empty());
    REQUIRE(cacheSupplier->getItem("Key7").empty());
    REQUIRE(cacheSupplier->getItem("Key8").empty());
    REQUIRE(cacheSupplier->getItem("Key9").empty());

    // Write the remaining items back now
    REQUIRE(diskLruCache->writeAllBackNow());

    // Check the expected write-back items
    REQUIRE(cacheSupplier->getItem("Key0") == "Value0");
    REQUIRE(cacheSupplier->getItem("Key1") == "Value1");
    REQUIRE(cacheSupplier->getItem("Key2") == "Value2");
    REQUIRE(cacheSupplier->getItem("Key3") == "Value3");
    REQUIRE(cacheSupplier->getItem("Key4") == "Value4");
    REQUIRE(cacheSupplier->getItem("Key5") == "Value5");
    REQUIRE(cacheSupplier->getItem("Key6") == "Value6");
    REQUIRE(cacheSupplier->getItem("Key7") == "Value7");
    REQUIRE(cacheSupplier->getItem("Key8") == "Value8");
    REQUIRE(cacheSupplier->getItem("Key9") == "Value9");

    // Delete some items from the cache
    REQUIRE(diskLruCache->deleteItem("Key0"));
    REQUIRE(diskLruCache->deleteItem("Key1"));
    REQUIRE(diskLruCache->deleteItem("Key5"));
    REQUIRE(diskLruCache->deleteItem("Key7"));
    REQUIRE(diskLruCache->deleteItem("Key8"));

    // Verify that we cannot re-delete already deleted items
    REQUIRE(!diskLruCache->deleteItem("Key0"));
    REQUIRE(!diskLruCache->deleteItem("Key1"));
    REQUIRE(!diskLruCache->deleteItem("Key8"));

    // Verify that the correct items were deleted
    REQUIRE(diskLruCache->getItem("Key0").empty());
    REQUIRE(diskLruCache->getItem("Key1").empty());
    REQUIRE(diskLruCache->getItem("Key2") == "Value2");
    REQUIRE(diskLruCache->getItem("Key3") == "Value3");
    REQUIRE(diskLruCache->getItem("Key4") == "Value4");
    REQUIRE(diskLruCache->getItem("Key5").empty());
    REQUIRE(diskLruCache->getItem("Key6") == "Value6");
    REQUIRE(diskLruCache->getItem("Key7").empty());
    REQUIRE(diskLruCache->getItem("Key8").empty());
    REQUIRE(diskLruCache->getItem("Key9") == "Value9");

    // Write the remaining items back now
    REQUIRE(diskLruCache->writeAllBackNow());

    // Verify the same items are deleted after write-back
    REQUIRE(diskLruCache->getItem("Key0").empty());
    REQUIRE(diskLruCache->getItem("Key1").empty());
    REQUIRE(diskLruCache->getItem("Key2") == "Value2");
    REQUIRE(diskLruCache->getItem("Key3") == "Value3");
    REQUIRE(diskLruCache->getItem("Key4") == "Value4");
    REQUIRE(diskLruCache->getItem("Key5").empty());
    REQUIRE(diskLruCache->getItem("Key6") == "Value6");
    REQUIRE(diskLruCache->getItem("Key7").empty());
    REQUIRE(diskLruCache->getItem("Key8").empty());
    REQUIRE(diskLruCache->getItem("Key9") == "Value9");
}

TEST_CASE ("Add Duplicate Items LRU Cache Operation", "[DiskLruCacheTest]")
{

    // Create the LRU Cache instance
    auto cacheSupplier = std::make_shared<DummyCacheSupplier>();
    auto diskLruCache = std::make_shared<DiskLruCache>(cacheSupplier, 50, 25);

    // Add some items to the cache (enough to force write-back)
    REQUIRE(diskLruCache->addItem("Key0", "Value0"));
    REQUIRE(diskLruCache->addItem("Key1", "Value1"));
    REQUIRE(diskLruCache->addItem("Key2", "Value2"));
    REQUIRE(diskLruCache->addItem("Key3", "Value3"));
    REQUIRE(diskLruCache->addItem("Key4", "Value4"));
    REQUIRE(diskLruCache->addItem("Key5", "Value5"));
    REQUIRE(diskLruCache->addItem("Key6", "Value6"));
    REQUIRE(diskLruCache->addItem("Key7", "Value7"));
    REQUIRE(diskLruCache->addItem("Key8", "Value8"));
    REQUIRE(diskLruCache->addItem("Key9", "Value9"));

    // Overwrite some items
    REQUIRE(diskLruCache->addItem("Key9", "NewValue9"));
    REQUIRE(diskLruCache->addItem("Key0", "NewValue0"));
    REQUIRE(diskLruCache->addItem("Key5", "NewValue5", true));

    // Get all items from the cache
    REQUIRE(diskLruCache->getItem("Key0") == "NewValue0");
    REQUIRE(diskLruCache->getItem("Key1") == "Value1");
    REQUIRE(diskLruCache->getItem("Key2") == "Value2");
    REQUIRE(diskLruCache->getItem("Key3") == "Value3");
    REQUIRE(diskLruCache->getItem("Key4") == "Value4");
    REQUIRE(diskLruCache->getItem("Key5") == "NewValue5");
    REQUIRE(diskLruCache->getItem("Key6") == "Value6");
    REQUIRE(diskLruCache->getItem("Key7") == "Value7");
    REQUIRE(diskLruCache->getItem("Key8") == "Value8");
    REQUIRE(diskLruCache->getItem("Key9") == "NewValue9");
}

TEST_CASE ("Too-Small LRU Cache Operation", "[DiskLruCacheTest]")
{

    // Create the LRU Cache instance
    auto cacheSupplier = std::make_shared<DummyCacheSupplier>();
    auto diskLruCache = std::make_shared<DiskLruCache>(cacheSupplier, 0, 0);

    // Add some items to the cache
    REQUIRE(diskLruCache->addItem("Key0", "Value0"));
    REQUIRE(diskLruCache->addItem("Key1", "Value1"));
    REQUIRE(diskLruCache->addItem("Key2", "Value2"));
    REQUIRE(diskLruCache->addItem("Key3", "Value3"));
    REQUIRE(diskLruCache->addItem("Key4", "Value4"));
    REQUIRE(diskLruCache->addItem("Key5", "Value5"));
    REQUIRE(diskLruCache->addItem("Key6", "Value6"));
    REQUIRE(diskLruCache->addItem("Key7", "Value7"));
    REQUIRE(diskLruCache->addItem("Key8", "Value8"));
    REQUIRE(diskLruCache->addItem("Key9", "Value9"));

    // Get all items from the cache
    REQUIRE(diskLruCache->getItem("Key0") == "Value0");
    REQUIRE(diskLruCache->getItem("Key1") == "Value1");
    REQUIRE(diskLruCache->getItem("Key2") == "Value2");
    REQUIRE(diskLruCache->getItem("Key3") == "Value3");
    REQUIRE(diskLruCache->getItem("Key4") == "Value4");
    REQUIRE(diskLruCache->getItem("Key5") == "Value5");
    REQUIRE(diskLruCache->getItem("Key6") == "Value6");
    REQUIRE(diskLruCache->getItem("Key7") == "Value7");
    REQUIRE(diskLruCache->getItem("Key8") == "Value8");
    REQUIRE(diskLruCache->getItem("Key9") == "Value9");
}

#endif //BITBOSON_STANDARDMODEL_DISKLRUCACHE_TEST_HPP

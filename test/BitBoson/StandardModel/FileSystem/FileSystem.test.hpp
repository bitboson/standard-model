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

#ifndef BITBOSON_STANDARDMODEL_FILESYSTEM_TEST_HPP
#define BITBOSON_STANDARDMODEL_FILESYSTEM_TEST_HPP

#include <BitBoson/StandardModel/FileSystem/FileSystem.h>

using namespace BitBoson::StandardModel;

TEST_CASE ("Writing/Reading Simple Data to File Test", "[FileSystemTest]")
{

    // Create a temporary directory
    auto tempDir = FileSystem::getTemporaryDir("BitBosonTest_");
    auto tempFile = tempDir.getChild("TestSmallFile");

    // Write the content to the file
    REQUIRE (tempFile.writeSimpleFile("Hello World!"));

    // Read back the content from the file
    REQUIRE (tempFile.readSimpleFile() == "Hello World!");

    // Delete the temporary directory
    tempFile.removeFile();
    tempDir.removeDir();
}

TEST_CASE ("Writing/Reading Small Data to File Test", "[FileSystemTest]")
{

    // Create a temporary directory
    auto tempDir = FileSystem::getTemporaryDir("BitBosonTest_");
    auto tempFile = tempDir.getChild("TestSmallFile");

    // Create a generator for writing file content
    long index = 0;
    auto writeGenerator = std::make_shared<Generator<std::string>>(
            [&index](std::shared_ptr<Yieldable<std::string>> yielder)
    {
        for (auto ii = 0; ii < 1; ii++)
        {
            yielder->yield("Hello World!");
            index++;
        }

        // Complete the yielder
        yielder->complete();
    });

    // Write the content to the file
    REQUIRE (tempFile.writeFile(writeGenerator));
    REQUIRE (index == 1);

    // Read back the content from the file
    auto readGenerator = tempFile.readFile();
    REQUIRE (readGenerator != nullptr);

    // Verify that it is the same content that was written
    while (readGenerator->hasMoreItems())
    {
        REQUIRE (readGenerator->getNextItem() == "Hello World!");
        index--;
    }
    REQUIRE (index == 0);

    // Delete the temporary directory
    tempFile.removeFile();
    tempDir.removeDir();
}

TEST_CASE ("Writing/Reading Mis-aligned Buffer File Test", "[FileSystemTest]")
{

    // Create a temporary directory
    auto tempDir = FileSystem::getTemporaryDir("BitBosonTest_");
    auto tempFile = tempDir.getChild("TestMisAlignedFile");

    // Create a generator for writing file content
    long index = 0;
    auto writeGenerator = std::make_shared<Generator<std::string>>(
            [&index](std::shared_ptr<Yieldable<std::string>> yielder)
    {
        for (auto ii = 0; ii < 2; ii++)
        {
            yielder->yield("Hello World!");
            index++;
        }

        // Complete the yielder
        yielder->complete();
    });

    // Write the content to the file
    REQUIRE (tempFile.writeFile(writeGenerator));
    REQUIRE (index == 2);

    // Read back the content from the file
    auto readGenerator = tempFile.readFile(17);
    REQUIRE (readGenerator != nullptr);

    // Verify that it is the same content that was written
    REQUIRE (readGenerator->getNextItem() == "Hello World!Hello");
    index--;
    REQUIRE (readGenerator->getNextItem() == " World!");
    index--;
    REQUIRE (index == 0);

    // Delete the temporary directory
    tempFile.removeFile();
    tempDir.removeDir();
}

TEST_CASE ("Writing/Reading Large Data to File Test", "[FileSystemTest]")
{

    // Create a temporary directory
    auto tempDir = FileSystem::getTemporaryDir("BitBosonTest_");
    auto tempFile = tempDir.getChild("TestLargeFile");

    // Create a generator for writing file content
    long index = 0;
    auto writeGenerator = std::make_shared<Generator<std::string>>(
            [&index](std::shared_ptr<Yieldable<std::string>> yielder)
    {
        for (auto ii = 0; ii < 100000; ii++)
        {
            yielder->yield("Hello World!");
            index++;
        }

        // Complete the yielder
        yielder->complete();
    });

    // Write the content to the file
    REQUIRE (tempFile.writeFile(writeGenerator));
    REQUIRE (index == 100000);

    // Read back the content from the file
    auto readGenerator = tempFile.readFile(12);
    REQUIRE (readGenerator != nullptr);

    // Verify that it is the same content that was written
    while (readGenerator->hasMoreItems())
    {
        REQUIRE (readGenerator->getNextItem() == "Hello World!");
        index--;
    }
    REQUIRE (index == 0);

    // Delete the temporary directory
    tempFile.removeFile();
    tempDir.removeDir();
}

TEST_CASE ("Renaming a small File Test", "[FileSystemTest]")
{

    // Create a temporary directory
    auto tempDir = FileSystem::getTemporaryDir("BitBosonTest_");
    auto tempFile = tempDir.getChild("TestSmallFile");

    // Create a generator for writing file content
    long index = 0;
    auto writeGenerator = std::make_shared<Generator<std::string>>(
            [&index](std::shared_ptr<Yieldable<std::string>> yielder)
    {
        for (auto ii = 0; ii < 1; ii++)
        {
            yielder->yield("Hello World!");
            index++;
        }

        // Complete the yielder
        yielder->complete();
    });

    // Write the content to the file
    REQUIRE (tempFile.writeFile(writeGenerator));
    REQUIRE (index == 1);

    // Rename the file
    tempFile.rename("TestSmallFile2");

    // Read back the content from the file
    auto readGenerator = tempDir.getChild("TestSmallFile2").readFile();
    REQUIRE (readGenerator != nullptr);

    // Verify that it is the same content that was written
    while (readGenerator->hasMoreItems())
    {
        REQUIRE (readGenerator->getNextItem() == "Hello World!");
        index--;
    }
    REQUIRE (index == 0);

    // Delete the temporary directory
    tempFile.removeFile();
    tempDir.removeDir();
}

#endif //BITBOSON_STANDARDMODEL_FILESYSTEM_TEST_HPP

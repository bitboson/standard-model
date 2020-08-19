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

#ifndef BITBOSON_STANDARDMODEL_UTILS_TEST_HPP
#define BITBOSON_STANDARDMODEL_UTILS_TEST_HPP

#include <BitBoson/StandardModel/Utils/Utils.h>
#include <BitBoson/StandardModel/Primitives/BigInt.hpp>

using namespace BitBoson::StandardModel;

/**
 * Helper function used to compare the contents of two vectors
 *
 * @param vect1 Vector of Strings representing one of the vectors for comparison
 * @param vect2 Vector of Strings representing one of the vectors for comparison
 * @return Boolean indicating whether the two vectors are equal (the same) or not
 */
bool compareVectors(std::vector<std::string> vect1, std::vector<std::string> vect2)
{

    // Create a return flag
    bool retFlag = false;

    // Only continue if both vectors are the same size
    if (vect1.size() == vect2.size())
    {

        // Verify that all of the items are the same
        retFlag = true;
        for (auto ii = 0; ii < vect1.size(); ii++)
            if (vect1[ii] != vect2[ii])
                retFlag = false;
    }

    // Return the return flag
    return retFlag;
}

TEST_CASE ("Get BigInt String Test", "[UtilsTest]")
{

    // Test various BigInt to String conversions
    REQUIRE (Utils::getBigIntString(BigInt{"1"}) == "1");
    REQUIRE (Utils::getBigIntString(BigInt{"-1"}) == "-1");
    REQUIRE (Utils::getBigIntString(BigInt{"0"}) == "0");
    REQUIRE (Utils::getBigIntString(BigInt{"10000000000000000"}) == "10000000000000000");
    REQUIRE (Utils::getBigIntString(BigInt{"-10000000000000000"}) == "-10000000000000000");
}

TEST_CASE ("Get BigFloat String Test", "[UtilsTest]")
{

    // Test various BigFloat to String conversions
    REQUIRE (Utils::getBigFloatString(BigFloat{"1.0"}) == "1");
    REQUIRE (Utils::getBigFloatString(BigFloat{"-1.123"}) == "-1.123");
    REQUIRE (Utils::getBigFloatString(BigFloat{"0"}) == "0");
    REQUIRE (Utils::getBigFloatString(BigFloat{"0.0001"}) == "0.0001");
    REQUIRE (Utils::getBigFloatString(BigFloat{"100000000000.000001"}) == "1e+11");
    REQUIRE (Utils::getBigFloatString(BigFloat{"-100000000000.000001"}) == "-1e+11");
}

TEST_CASE ("Random UUID Generation Test", "[UtilsTest]")
{

    // Generate random UUIDs and verify they are different
    REQUIRE (Utils::getUUID() != Utils::getUUID());
    REQUIRE (Utils::getUUID() != Utils::getUUID());
    REQUIRE (Utils::getUUID() != Utils::getUUID());
    REQUIRE (Utils::getUUID() != Utils::getUUID());
    REQUIRE (Utils::getUUID() != Utils::getUUID());
    REQUIRE (Utils::getUUID() != Utils::getUUID());
    REQUIRE (Utils::getUUID() != Utils::getUUID());
}

TEST_CASE ("Get String Between Sub-Strings Test", "[UtilsTest]")
{

    // Create a test string to extract the sub-string from
    auto testString = "a<b<cd>y>z";

    // Test splitting up strings to verify sub-string getting
    REQUIRE (Utils::getStringBetweenSubStrings(testString, "<", ">", Utils::ParseType::Inner) == "cd");
    REQUIRE (Utils::getStringBetweenSubStrings(testString, "<", ">", Utils::ParseType::Outer) == "b<cd>y");
    REQUIRE (Utils::getStringBetweenSubStrings(testString, "<", ">", Utils::ParseType::First) == "b<cd");
    REQUIRE (Utils::getStringBetweenSubStrings(testString, "<", ">", Utils::ParseType::Last) == "cd>y");
}

TEST_CASE ("Split String By Delimiter Test", "[UtilsTest]")
{

    // Create the test string to split
    auto stringToSplit = "Hello,World,This,Is,Awesome!";

    // Split the string into its parts
    auto splitString = Utils::splitStringByDelimiter(stringToSplit, ",");

    // Verify the string parts were split properly
    REQUIRE (splitString.size() == 5);
    REQUIRE (splitString[0] == "Hello");
    REQUIRE (splitString[1] == "World");
    REQUIRE (splitString[2] == "This");
    REQUIRE (splitString[3] == "Is");
    REQUIRE (splitString[4] == "Awesome!");

    // Split the string into its parts (not really)
    auto splitString2 = Utils::splitStringByDelimiter(stringToSplit, "~");

    // Verify the string wasn't split into parts
    REQUIRE (splitString2.size() == 1);
    REQUIRE (splitString2[0] == "Hello,World,This,Is,Awesome!");
}

TEST_CASE ("Create/Parse File-String Test", "[UtilsTest]")
{

    // Create a new random packed vector
    std::vector<std::string> packedVect;
    packedVect.emplace_back("Hello");
    packedVect.emplace_back("World");
    packedVect.emplace_back("");
    packedVect.emplace_back("How");
    packedVect.emplace_back("Are");
    packedVect.emplace_back("You");
    packedVect.emplace_back("?");

    // Create a file-string from the packed vector
    REQUIRE (Utils::getFileString(std::vector<std::string>()) == "e317fQ==");
    REQUIRE (Utils::getFileString(std::vector<std::string>(), "") == "e317fQ==");
    REQUIRE (Utils::getFileString(std::vector<std::string>(), "Signature") == "e317VTJsbmJtRjBkWEpsfQ==");
    REQUIRE (Utils::getFileString(packedVect) == "e1NHVnNiRzg9LFYyOXliR1E9LCxTRzkzLFFYSmwsV1c5MSxQdz09LH17fQ==");
    REQUIRE (Utils::getFileString(packedVect, "") == "e1NHVnNiRzg9LFYyOXliR1E9LCxTRzkzLFFYSmwsV1c5MSxQdz09LH17fQ==");
    REQUIRE (Utils::getFileString(packedVect, "Signature") == "e1NHVnNiRzg9LFYyOXliR1E9LCxTRzkzLFFYSmwsV1c5MSxQdz09"
                                                             "LH17VTJsbmJtRjBkWEpsfQ==");

    // Parse a completely empty file string into a packed vector
    auto packedVect2 = Utils::parseFileString("", true);
    REQUIRE (packedVect2.empty());

    // Parse a completely empty file string into a packed vector
    packedVect2 = Utils::parseFileString("", false);
    REQUIRE (packedVect2.empty());

    // Parse the empty file-string with the empty signature back into a packed vector
    packedVect2 = Utils::parseFileString("e317fQ==", true);
    REQUIRE (packedVect2.empty());

    // Parse the empty file-string without the empty signature back into a packed vector
    packedVect2 = Utils::parseFileString("e317fQ==", false);
    REQUIRE (packedVect2.empty());

    // Parse the empty file-string (plus signature) with the signature back into a packed vector
    packedVect2 = Utils::parseFileString("e317VTJsbmJtRjBkWEpsfQ==", true);
    REQUIRE (packedVect2.size() == 1);
    REQUIRE (packedVect2[0] == "Signature");

    // Parse the empty file-string (plus signature) without the signature back into a packed vector
    packedVect2 = Utils::parseFileString("e317VTJsbmJtRjBkWEpsfQ==", false);
    REQUIRE (packedVect2.empty());

    // Parse the file-string with the empty signature back into a packed vector
    packedVect2 = Utils::parseFileString("e1NHVnNiRzg9LFYyOXliR1E9LCxTRzkzLFFYSmwsV1c5MSxQdz09LH17fQ==", true);
    REQUIRE (packedVect2.size() == 8);
    REQUIRE (packedVect2[0] == "Hello");
    REQUIRE (packedVect2[1] == "World");
    REQUIRE (packedVect2[2].empty());
    REQUIRE (packedVect2[3] == "How");
    REQUIRE (packedVect2[4] == "Are");
    REQUIRE (packedVect2[5] == "You");
    REQUIRE (packedVect2[6] == "?");
    REQUIRE (packedVect2[2].empty());

    // Parse the file-string without the empty signature back into a packed vector
    packedVect2 = Utils::parseFileString("e1NHVnNiRzg9LFYyOXliR1E9LCxTRzkzLFFYSmwsV1c5MSxQdz09LH17fQ==", false);
    REQUIRE (packedVect2.size() == 7);
    REQUIRE (packedVect2[0] == "Hello");
    REQUIRE (packedVect2[1] == "World");
    REQUIRE (packedVect2[2].empty());
    REQUIRE (packedVect2[3] == "How");
    REQUIRE (packedVect2[4] == "Are");
    REQUIRE (packedVect2[5] == "You");
    REQUIRE (packedVect2[6] == "?");

    // Parse the file-string with the signature back into a packed vector
    packedVect2 = Utils::parseFileString("e1NHVnNiRzg9LFYyOXliR1E9LCxTRzkzLFFYSmwsV1c5MSxQdz09LH17"
                                              "VTJsbmJtRjBkWEpsfQ==", true);
    REQUIRE (packedVect2.size() == 8);
    REQUIRE (packedVect2[0] == "Hello");
    REQUIRE (packedVect2[1] == "World");
    REQUIRE (packedVect2[2].empty());
    REQUIRE (packedVect2[3] == "How");
    REQUIRE (packedVect2[4] == "Are");
    REQUIRE (packedVect2[5] == "You");
    REQUIRE (packedVect2[6] == "?");
    REQUIRE (packedVect2[7] == "Signature");

    // Parse the file-string without the signature back into a packed vector
    packedVect2 = Utils::parseFileString("e1NHVnNiRzg9LFYyOXliR1E9LCxTRzkzLFFYSmwsV1c5MSxQdz09LH17"
                                         "VTJsbmJtRjBkWEpsfQ==", false);
    REQUIRE (packedVect2.size() == 7);
    REQUIRE (packedVect2[0] == "Hello");
    REQUIRE (packedVect2[1] == "World");
    REQUIRE (packedVect2[2].empty());
    REQUIRE (packedVect2[3] == "How");
    REQUIRE (packedVect2[4] == "Are");
    REQUIRE (packedVect2[5] == "You");
    REQUIRE (packedVect2[6] == "?");
}

TEST_CASE ("Combine String Parts Test", "[UtilsTest]")
{

    // Test various String Combinations
    REQUIRE (Utils::combineStringParts({"Hello", "World", "How", "Are", "You"}) == "HelloWorldHowAreYou");
    REQUIRE (Utils::combineStringParts({"", "", "", "", ""}) == "");
    REQUIRE (Utils::combineStringParts({}) == "");
    REQUIRE (Utils::combineStringParts({" ", " ", " ", " "}) == "    ");
    REQUIRE (Utils::combineStringParts({"", "0", "", "", "1", "", "", "2", ""}) == "012");
}

TEST_CASE ("Split String into Parts Test", "[UtilsTest]")
{

    // Test various String Splitting Combinations
    REQUIRE (Utils::splitStringIntoParts("1234567890", -1).empty());
    REQUIRE (Utils::splitStringIntoParts("1234567890", 0).empty());
    REQUIRE (compareVectors(Utils::splitStringIntoParts("1234567890", 1), {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"}));
    REQUIRE (compareVectors(Utils::splitStringIntoParts("1234567890", 2), {"12", "34", "56", "78", "90"}));
    REQUIRE (Utils::splitStringIntoParts("1234567890", 3).empty());
    REQUIRE (Utils::splitStringIntoParts("1234567890", 4).empty());
    REQUIRE (compareVectors(Utils::splitStringIntoParts("1234567890", 5), {"12345", "67890"}));
    REQUIRE (Utils::splitStringIntoParts("1234567890", 6).empty());
    REQUIRE (Utils::splitStringIntoParts("1234567890", 7).empty());
    REQUIRE (Utils::splitStringIntoParts("1234567890", 8).empty());
    REQUIRE (Utils::splitStringIntoParts("1234567890", 9).empty());
    REQUIRE (compareVectors(Utils::splitStringIntoParts("1234567890", 10), {"1234567890"}));
    REQUIRE (Utils::splitStringIntoParts("1234567890", 11).empty());
    REQUIRE (Utils::splitStringIntoParts("1234567890", 10000).empty());
}

#endif //BITBOSON_STANDARDMODEL_UTILS_TEST_HPP

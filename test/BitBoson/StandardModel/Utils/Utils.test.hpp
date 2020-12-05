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
        for (unsigned long ii = 0; ii < vect1.size(); ii++)
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
    //REQUIRE (Utils::getFileString(std::vector<std::string>()) == "");
    //REQUIRE (Utils::getFileString(packedVect) == "[\"Hello\",\"World\",null,\"How\",\"Are\",\"You\",\"?\"]");

    // Parse a completely empty file string into a packed vector
    auto packedVectOut = Utils::parseFileString("");
    REQUIRE (packedVectOut.empty());

    // Parse the file-string back into a packed vector
    //packedVectOut = Utils::parseFileString("[\"Hello\",\"World\",null,\"How\",\"Are\",\"You\",\"?\"]");
    packedVectOut = Utils::parseFileString(Utils::getFileString(packedVect));
    REQUIRE (packedVectOut.size() == 7);
    REQUIRE (packedVectOut[0] == "Hello");
    REQUIRE (packedVectOut[1] == "World");
    REQUIRE (packedVectOut[2].empty());
    REQUIRE (packedVectOut[3] == "How");
    REQUIRE (packedVectOut[4] == "Are");
    REQUIRE (packedVectOut[5] == "You");
    REQUIRE (packedVectOut[6] == "?");

    // Create a file-string from the given information
    auto fileString = Utils::getFileString(packedVect);
    //REQUIRE (fileString == "[\"Hello\",\"World\",null,\"How\",\"Are\",\"You\",\"?\"]");

    // Create a new random packed vector including the previous
    std::vector<std::string> packedVect2;
    packedVect2.emplace_back("1");
    packedVect2.emplace_back("2");
    packedVect2.emplace_back(fileString);
    packedVect2.emplace_back("3");
    packedVect2.emplace_back("4");
    packedVect2.emplace_back("5");
    packedVect2.emplace_back("6");

    // Create a file-string from the given information
    auto fileString2 = Utils::getFileString(packedVect2);
    //REQUIRE (fileString2 == "[\"1\",\"2\",\"[\\\"Hello\\\",\\\"World\\\",null,\\\"How\\\","
    //                        "\\\"Are\\\",\\\"You\\\",\\\"?\\\"]\",\"3\",\"4\",\"5\",\"6\"]");

    // Create a new random packed vector including the previous two
    std::vector<std::string> packedVect3;
    packedVect3.emplace_back("A");
    packedVect3.emplace_back("B");
    packedVect3.emplace_back(fileString2);
    packedVect3.emplace_back("C");
    packedVect3.emplace_back(fileString);

    // Create a file-string from the given information
    auto fileString3 = Utils::getFileString(packedVect3);

    // Parse the file-string back into a readable state for the top-level
    auto packedVectParsed = Utils::parseFileString(fileString3);
    REQUIRE (packedVectParsed.size() == 5);
    REQUIRE (packedVectParsed[0] == "A");
    REQUIRE (packedVectParsed[1] == "B");
    REQUIRE (packedVectParsed[2] == fileString2);
    REQUIRE (packedVectParsed[3] == "C");
    REQUIRE (packedVectParsed[4] == fileString);

    // Parse the file-string back into a readable state for the middle-level
    packedVectParsed = Utils::parseFileString(fileString2);
    REQUIRE (packedVectParsed.size() == 7);
    REQUIRE (packedVectParsed[0] == "1");
    REQUIRE (packedVectParsed[1] == "2");
    REQUIRE (packedVectParsed[2] == fileString);
    REQUIRE (packedVectParsed[3] == "3");
    REQUIRE (packedVectParsed[4] == "4");
    REQUIRE (packedVectParsed[5] == "5");
    REQUIRE (packedVectParsed[6] == "6");

    // Parse the file-string back into a readable state for the bottom-level
    packedVectParsed = Utils::parseFileString(fileString);
    REQUIRE (packedVectParsed.size() == 7);
    REQUIRE (packedVectParsed[0] == "Hello");
    REQUIRE (packedVectParsed[1] == "World");
    REQUIRE (packedVectParsed[2].empty());
    REQUIRE (packedVectParsed[3] == "How");
    REQUIRE (packedVectParsed[4] == "Are");
    REQUIRE (packedVectParsed[5] == "You");
    REQUIRE (packedVectParsed[6] == "?");

    std::cout << "START" << std::endl;
    long val = 0;
    for (int ii = 0; ii < 1000000; ii++)
        val += Utils::parseFileString(fileString).size();
    std::cout << "END: " << val << std::endl;
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

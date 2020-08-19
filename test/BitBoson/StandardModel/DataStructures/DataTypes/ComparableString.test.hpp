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

#ifndef BITBOSON_STANDARDMODEL_COMPARABLESTRING_TEST_HPP
#define BITBOSON_STANDARDMODEL_COMPARABLESTRING_TEST_HPP

#include <string>
#include <sstream>
#include <BitBoson/StandardModel/Utils/Utils.h>
#include <BitBoson/StandardModel/DataStructures/DataTypes/ComparableString.hpp>

using namespace BitBoson::StandardModel;

TEST_CASE ("General Comparable String Test", "[ComparableStringTest]")
{

    // Create a few comparable strings
    ComparableString string1("AAAAA");
    ComparableString string2("BBBBB");
    ComparableString string3("BBBBB");
    ComparableString string4("AABBB");
    ComparableString string5("11ZZZ");
    ComparableString string6("ZZZ111");

    // Verify ordering
    REQUIRE(string1 != string2);
    REQUIRE(string1 < string2);
    REQUIRE(string2 > string1);
    REQUIRE(string1 <= string2);
    REQUIRE(string2 <= string3);
    REQUIRE(string3 >= string2);
    REQUIRE(string2 >= string3);
    REQUIRE(string2 == string3);
    REQUIRE(string3 > string4);
    REQUIRE(string3 >= string4);
    REQUIRE(string4 > string5);
    REQUIRE(string4 >= string5);
    REQUIRE(string5 < string6);
    REQUIRE(string5 <= string6);
    REQUIRE(string6 > string5);
    REQUIRE(string6 >= string5);
}

TEST_CASE ("Large Comparable String Test", "[ComparableStringTest]")
{

    // Create a few comparable strings
    ComparableString string1("SYDF8Y8WRG27WE6TGR823F77RFY1368GEO8WFGUIQG3E8GQFG97TWGPFUQGEOV283FGRWERW");
    ComparableString string2("DF89YG934T4T7NY0QVV6TN0Q7N4W4RT0Y2397YR78378CNR3Y37R238TRG8WG38GW48RG487");

    // Verify ordering
    REQUIRE(Utils::getBigIntString(string1.getComparableValue()) >= Utils::getBigIntString(string2.getComparableValue()));
}

TEST_CASE ("Get Comparable String Difference Test", "[ComparableStringTest]")
{

    // Create a few comparable strings
    ComparableString string1("AAAA");
    ComparableString string2("AAAB");
    ComparableString string3("AAAZ");

    // Verify ordering
    REQUIRE(Utils::getBigIntString(string2 - string1) == "1");
}

TEST_CASE ("Accessing and Setting Comparable String Test", "[ComparableStringTest]")
{

    // Create a comparable string to use for testing
    ComparableString compString;

    // Set the value using the >> operator
    // and verify it's value has been set
    std::stringstream strStream1;
    strStream1 << "HelloWorld";
    strStream1 >> compString;
    REQUIRE (compString.getString() == "HelloWorld");

    // Get the value using the << operator
    // and verify the value is as expected
    std::stringstream strStream2;
    strStream2 << compString;
    REQUIRE (strStream2.str() == "HelloWorld");
}

#endif //BITBOSON_STANDARDMODEL_COMPARABLESTRING_TEST_HPP

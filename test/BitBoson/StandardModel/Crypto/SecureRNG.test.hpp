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

#ifndef BITBOSON_STANDARDMODEL_SECURERNG_TEST_HPP
#define BITBOSON_STANDARDMODEL_SECURERNG_TEST_HPP

#include <BitBoson/StandardModel/Crypto/SecureRNG.h>

using namespace BitBoson::StandardModel;

TEST_CASE ("Generate Random Length Strings Test", "[SecureRNGTest]")
{

    // Create a SecureRNG for testing
    SecureRNG secureRng;

    // Generate various sized random strings and confirm their size
    REQUIRE (secureRng.generateRandomString(0).empty());
    REQUIRE (secureRng.generateRandomString(1).size() == 1);
    REQUIRE (secureRng.generateRandomString(2).size() == 2);
    REQUIRE (secureRng.generateRandomString(3).size() == 3);
    REQUIRE (secureRng.generateRandomString(13).size() == 13);
    REQUIRE (secureRng.generateRandomString(256).size() == 256);
    REQUIRE (secureRng.generateRandomString(257).size() == 257);
    REQUIRE (secureRng.generateRandomString(300).size() == 300);
    REQUIRE (secureRng.generateRandomString(1022).size() == 1022);
    REQUIRE (secureRng.generateRandomString(1023).size() == 1023);
    REQUIRE (secureRng.generateRandomString(1024).size() == 1024);
    REQUIRE (secureRng.generateRandomString(1025).size() == 1025);
    REQUIRE (secureRng.generateRandomString(1026).size() == 1026);
    REQUIRE (secureRng.generateRandomString(10000).size() == 10000);
}

TEST_CASE ("Unique Separate Random Sequences Test", "[SecureRNGTest]")
{

    // Create two SecureRNG for testing
    SecureRNG secureRng1;
    SecureRNG secureRng2;

    // Verify that the numbers are not the same for both
    REQUIRE (secureRng1.generateRandomString(8) != secureRng2.generateRandomString(8));
    REQUIRE (secureRng1.generateRandomString(16) != secureRng2.generateRandomString(16));
    REQUIRE (secureRng1.generateRandomString(32) != secureRng2.generateRandomString(32));
    REQUIRE (secureRng1.generateRandomString(64) != secureRng2.generateRandomString(64));
    REQUIRE (secureRng1.generateRandomString(128) != secureRng2.generateRandomString(128));
    REQUIRE (secureRng1.generateRandomString(256) != secureRng2.generateRandomString(256));
    REQUIRE (secureRng1.generateRandomString(1024) != secureRng2.generateRandomString(1024));
}


TEST_CASE ("Unique Random Sequences Test", "[SecureRNGTest]")
{

    // Create a SecureRNG for testing
    SecureRNG secureRng;

    // Verify that consecutive runs do not produce the same strings
    REQUIRE (secureRng.generateRandomString(8) != secureRng.generateRandomString(8));
    REQUIRE (secureRng.generateRandomString(16) != secureRng.generateRandomString(16));
    REQUIRE (secureRng.generateRandomString(32) != secureRng.generateRandomString(32));
    REQUIRE (secureRng.generateRandomString(64) != secureRng.generateRandomString(64));
    REQUIRE (secureRng.generateRandomString(128) != secureRng.generateRandomString(128));
    REQUIRE (secureRng.generateRandomString(256) != secureRng.generateRandomString(256));
    REQUIRE (secureRng.generateRandomString(1024) != secureRng.generateRandomString(1024));
}

#endif //BITBOSON_STANDARDMODEL_SECURERNG_TEST_HPP

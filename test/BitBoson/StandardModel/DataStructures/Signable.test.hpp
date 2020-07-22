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

#ifndef BITBOSON_STANDARDMODEL_SIGNABLE_TEST_HPP
#define BITBOSON_STANDARDMODEL_SIGNABLE_TEST_HPP

#include <BitBoson/StandardModel/DataStructures/Signable.hpp>

using namespace BitBoson::StandardModel;

// Create a dummy signable
class DummySignable : public Signable
{

    /**
     * Overridden function used to get the file-string-representation of the object
     *
     * @return String representing the cache-able data for the object
     */
    std::string getFileString() const override
    {

        // Simply return dummy-text for testing
        return "FileString";
    };

    /**
     * Overridden function used to setup the object based on the given file-string-representation
     *
     * @param fileString String representing the cache-able data for the object to create from
     * @return Boolean indicating whether the given string was parsed and processed properly
     */
    bool setFileString(const std::string& fileString) override
    {

        // Always return true for testing
        return true;
    };

    /**
     * Overridden function used to get the unique hash (SHA256) of the underlying object
     *
     * @return String representing the unique (SHA256) hash of the underlying object
     */
    std::string getUniqueHash() const override
    {

        // Simply return dummy-text for testing
        return "UniqueHash";
    };
};

TEST_CASE ("Generic Signable Test", "[SignableTest]")
{
    // Create a new key-pair
    auto keyPair = Crypto::getWinternitzKeyPair();

    // Create a new dummy-signable instance and sign it
    DummySignable newSignable;
    newSignable.sign(keyPair.privateKey);

    // Verify the signature
    REQUIRE(newSignable.isValid(keyPair.publicKey));

    // Verify we can copy the signable
    auto signable2 = newSignable;
    REQUIRE(signable2.isValid(keyPair.publicKey));

    // Verify we can copy the signable
    DummySignable signable3(newSignable);
    REQUIRE(signable3.isValid(keyPair.publicKey));
}

TEST_CASE ("Compare Signables Test", "[SignableTest]")
{
    // Create two new key-pair
    auto keyPair1 = Crypto::getWinternitzKeyPair();
    auto keyPair2 = Crypto::getWinternitzKeyPair();

    // Create a new dummy-signable instance and sign it
    DummySignable newSignable1;
    newSignable1.sign(keyPair1.privateKey);

    // Create another new dummy-signable instance and sign it
    // with the second key-pair this time
    DummySignable newSignable2;
    newSignable2.sign(keyPair2.privateKey);

    // Verify that the two signatures are different
    REQUIRE (newSignable1.getSignature() != newSignable2.getSignature());

    // Create another new dummy-signable instance and sign it
    // with the first key-pair this time
    DummySignable newSignable3;
    newSignable3.sign(keyPair1.privateKey);

    // Verify that the signatures match up as expected
    REQUIRE (newSignable1.getSignature() == newSignable3.getSignature());
    REQUIRE (newSignable2.getSignature() != newSignable3.getSignature());
    REQUIRE (newSignable1.getSignature() != newSignable2.getSignature());
}

#endif //BITBOSON_STANDARDMODEL_SIGNABLE_TEST_HPP

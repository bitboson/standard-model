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

#ifndef BITBOSON_STANDARDMODEL_WINTERNITZKEYPAIR
#define BITBOSON_STANDARDMODEL_WINTERNITZKEYPAIR

#include <BitBoson/StandardModel/Utils/Utils.h>
#include <BitBoson/StandardModel/Crypto/Crypto.h>
#include <BitBoson/StandardModel/Crypto/DigitalSignatures/DigitalSignatureKeyPair.hpp>

namespace BitBoson::StandardModel
{

    class WinternitzKeyPair : public DigitalSignatureKeyPair
    {

        // Public member functions
        public:

            /**
             * Constructor used to setup the instance
             */
            WinternitzKeyPair() = default;

            /**
             * Overridden function used to generate a new key-pair
             */
            void generateNewKeyPair() override
            {

                // Generate 32 random hashes for the SHA256 Winternitz one-time
                // signature scheme (private key)
                std::vector<std::string> randomHashes;
                for (auto ii = 0; ii < 32; ii++)
                    randomHashes.push_back(Crypto::getRandomSha256(true));

                // Construct the private key from the random hashes
                setPrivateKey(Utils::combineStringParts(randomHashes));

                // Hash the 32 random hashes 256 times
                for (auto ii = 0; ii < 32; ii++)
                    for (auto jj = 0; jj < 256; jj++)
                        randomHashes[ii] = Crypto::sha256(randomHashes[ii]);

                // Construct the public key from the hashes random hashes
                setPublicKey(Utils::combineStringParts(randomHashes));
            }

            /**
             * Overridden function used to get the key-pair type
             *
             * @return KeyTypes representing the key-pair type
             */
            KeyTypes getKeyType() const override
            {

                // Simply return the hard-coded key type
                return DigitalSignatureKeyPair::KeyTypes::WINTERNITZ;
            }

            /**
             * Overridden function used to sign the given message
             *
             * @param message String representing the message to sign
             * @return String representing the signed message (signature)
             */
            std::string sign(const std::string& message) const override
            {

                // Start by hashing the message using the SHA256 hash scheme
                auto messageHash = Crypto::sha256(message);

                // Split the message hash up into 32 8-bit parts
                auto splitHashParts = Utils::splitStringIntoParts(messageHash, 2);

                // Loop through each part of the message hash and extract the integer for it
                // TODO - This is done REALLY poorly - so fix it
                std::vector<int> partInt;
                for (const auto& item : splitHashParts)
                    partInt.push_back(boost::lexical_cast<int>(Utils::getBigIntString(
                            Crypto::getBigIntFromHash(item))));

                // Split the Winternitz key into its component parts
                auto privateKeyParts = Utils::splitStringIntoParts(getPrivateKey(), 64);

                // For each of the new integer representations for the hash parts,
                // hash the private key part 256 - the generated integer amount
                for (unsigned long ii = 0; ii < partInt.size(); ii++)
                    for (unsigned long jj = 0; jj < (256 - partInt[ii]); jj++)
                        privateKeyParts[ii] = Crypto::sha256(privateKeyParts[ii]);

                // Return the signature (combined hash parts);
                return Utils::combineStringParts(privateKeyParts);
            }

            /**
             * Overridden function used to verify the message signature
             *
             * @param message String representing the message to use
             * @param signature String representing the signature to verify
             * @return Boolean indicating if the signature is valid or not
             */
            bool isValid(const std::string& message, const std::string& signature) const override
            {

                // Create the return flag
                bool result;

                // Start by hashing the message using the SHA256 hash scheme
                auto messageHash = Crypto::sha256(message);

                // Split the message hash up into 32 parts
                auto splitHashParts = Utils::splitStringIntoParts(messageHash, 2);

                // Loop through each part of the message hash and extract the integer for it
                std::vector<int> partInt;
                for (const auto& item : splitHashParts)
                    partInt.push_back(boost::lexical_cast<int>(Utils::getBigIntString(
                            Crypto::getBigIntFromHash(item))));

                // Split the signature up into its component parts
                auto splitSignatureParts = Utils::splitStringIntoParts(signature, 64);

                // For each of the new integer representations for the hash parts,
                // hash the private key part the generated integer amount
                if (splitSignatureParts.size() >= partInt.size())
                    for (unsigned long ii = 0; ii < partInt.size(); ii++)
                        for (unsigned long jj = 0; jj < partInt[ii]; jj++)
                            splitSignatureParts[ii] = Crypto::sha256(splitSignatureParts[ii]);

                // Re-combined the individual hash parts and compare with the public key
                result = (Utils::combineStringParts(splitSignatureParts) == getPublicKey());

                // Return the results
                return result;
            }

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~WinternitzKeyPair() = default;
    };
}

#endif //BITBOSON_STANDARDMODEL_WINTERNITZKEYPAIR

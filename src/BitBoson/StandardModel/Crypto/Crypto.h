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

#ifndef BITBOSON_STANDARDMODEL_CRYPTO_H
#define BITBOSON_STANDARDMODEL_CRYPTO_H

#include <string>
#include <BitBoson/StandardModel/Primitives/BigInt.hpp>
#include <BitBoson/StandardModel/Crypto/Encryption/EncryptionKey.hpp>
#include <BitBoson/StandardModel/Crypto/DigitalSignatures/DigitalSignatureKeyPair.hpp>

namespace BitBoson::StandardModel
{

    namespace Crypto
    {

        /**
         * Function used to get the number of leading zeros a hash begins with
         *
         * @param hash String representing the hash to get the number of leading zeros from
         * @return Unsigned Int representing the number of leading zeros from the hash
         */
        unsigned long getNumberOfLeadingZerosInHash(const std::string& hash);

        /**
         * Function used to get a public-private key-pair for the provided type
         *
         * @param keyType KeyTypes representing the key-pair type
         * @return KeyPair representing the private-public key-pair
         */
        std::shared_ptr<DigitalSignatureKeyPair> getKeyPair(
                DigitalSignatureKeyPair::KeyTypes keyType);

        /**
         * Function used to get a public key-pair object for the provided type
         * NOTE: This returns a key-pair with NO private key (just the public one)
         *
         * @param keyType KeyTypes representing the key-pair type
         * @param publicKey String representing the public key to use
         * @return KeyPair representing the private-public key-pair
         */
        std::shared_ptr<DigitalSignatureKeyPair> getPublicKey(
                DigitalSignatureKeyPair::KeyTypes keyType, const std::string& publicKey);

        /**
         * Function used to get a new encryption key (and IV) for symmetric encryption
         *
         * @param keyType KeyTypes representing the encryption-key type
         * @return EncryptionKey representing the private key and IV for encryption
         */
        std::shared_ptr<EncryptionKey> getEncryptionKey(EncryptionKey::KeyTypes keyType);

        /**
         * Function used to get the integer (BigInt) representation of the given hexadecimal hash value
         *
         * @param hash String representing the hexadecimal hash value
         * @return BigInteger representation of the given hash
         */
        BigInt getBigIntFromHash(const std::string& hash);

        /**
         * Function used to get the argon2d hash of the given string in a base64 format
         *
         * @param data String to get the hash of
         * @return String representing the hashed value (in base64 format) of the given data
         */
        std::string argon2d(const std::string& data);

        /**
         * Function used to get the SHA256 hash of the given string in a hex format
         *
         * @param data String to get the hash of
         * @param toUpper Boolean indicating whether the output should be upper-case
         * @param getBytes Boolean indicating whether to return bytes instead of hex
         * @return String representing the hashed value (in hex format) of the given data
         */
        std::string sha256(const std::string& data, bool toUpper=true, bool getBytes=false);

        /**
         * Function used to get a random SHA256 hash
         *
         * @param secure Boolean indicating whether this needs to be a secure random hash
         * @return String representing the random SHA256 hash
         */
        std::string getRandomSha256(bool secure=false);

        /**
         * Function used to get a PoW hash based on the supplied string
         * This function will produce a "fudge value" which was used to get the PoW hash
         *
         * @param paddedZeros Unsigned Long representing the number of zeros to find a hash for
         * @param initString String representing the base string to add the fudge value to to get the PoW hash
         * @param fudgeValue String representing the fudge value required to obtain the PoW hash
         * @return String representing the PoW hash for the given string and deduced "fudge value"
         */

        std::string getPowHash(unsigned long paddedZeros, const std::string& initString, std::string& fudgeValue);

        /**
         * Function used to encode the supplied string into its base-64 representation
         *
         * @param stringToEncode String representing the raw string to encode
         * @param urlEncode Boolean used to indicate if the string should be URL encoded
         * @return String representing the encoded string
         */
        std::string base64Encode(const std::string& stringToEncode, bool urlEncode=true);

        /**
         * Function used to decode the supplied string from its base-64 representation
         *
         * @param stringToDecode String representing the base-64 string to decode
         * @return String representing the decoded string
         */
        std::string base64Decode(const std::string& stringToDecode);

        /**
         * Function used to convert the supplied string from hexidecimal to binary
         *
         * @param stringToDecode String representing the base-64 string to decode
         * @return String representing the decoded string
         */
        std::string hexToBinary(const std::string& hexString);
    };
}

#endif //BITBOSON_STANDARDMODEL_CRYPTO_H

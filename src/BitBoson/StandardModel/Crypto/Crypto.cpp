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

#include <algorithm>
#include <argon2.h>
#include <picosha2/picosha2.h>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <BitBoson/StandardModel/Utils/Utils.h>
#include <BitBoson/StandardModel/Crypto/Crypto.h>
#include <BitBoson/StandardModel/Crypto/SecureRNG.h>
#include <BitBoson/StandardModel/Crypto/Encryption/AesEncryptionKey.hpp>
#include <BitBoson/StandardModel/Crypto/DigitalSignatures/EcdsaKeyPair.hpp>
#include <BitBoson/StandardModel/Crypto/DigitalSignatures/WinternitzKeyPair.hpp>

using namespace BitBoson::StandardModel;

// Base64 characters available for use
static const std::string base64Chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

/**
 * Function used to get the number of leading zeros a hash begins with
 *
 * @param hash String representing the hash to get the number of leading zeros from
 * @return Unsigned Int representing the number of leading zeros from the hash
 */
unsigned long Crypto::getNumberOfLeadingZerosInHash(const std::string& hash)
{

    // Create the return value
    unsigned long leadingZeros = 0;

    // Loop through the characters of the hash (left to right) until we run out of zeros
    // keeping track of the number of zeros we find along the way
    while (leadingZeros < hash.size() && hash[leadingZeros] == '0')
        leadingZeros++;

    // Return the number of leading zeros
    return leadingZeros;
}

/**
 * Function used to get a public-private key-pair for the provided type
 *
 * @param keyType KeyTypes representing the key-pair type
 * @return KeyPair representing the private-public key-pair
 */
std::shared_ptr<DigitalSignatureKeyPair> Crypto::getKeyPair(
        DigitalSignatureKeyPair::KeyTypes keyType)
{

    // Create a return key-pair object
    std::shared_ptr<DigitalSignatureKeyPair> retObj = nullptr;

    // Setup/Initialize the key-pair based on the provided type
    switch (keyType)
    {

        // Handle the Winternitz key-type
        case DigitalSignatureKeyPair::KeyTypes::WINTERNITZ:
            retObj = std::make_shared<WinternitzKeyPair>();
            break;

        // Handle the ECDSA key-type
        case DigitalSignatureKeyPair::KeyTypes::ECDSA:
            retObj = std::make_shared<EcdsaKeyPair>();
            break;

        // Handle the NONE key-type (and thus default)
        case DigitalSignatureKeyPair::KeyTypes::NONE:
            retObj = nullptr;
        default:
            retObj = nullptr;
    }

    // If the key-type was valid generate the new key-pair
    if (retObj != nullptr)
        retObj->generateNewKeyPair();

    // Return the key-pair object
    return retObj;
}

/**
 * Function used to get a public key-pair object for the provided type
 * NOTE: This returns a key-pair with NO private key (just the public one)
 *
 * @param keyType KeyTypes representing the key-pair type
 * @param publicKey String representing the public key to use
 * @return KeyPair representing the private-public key-pair
 */
std::shared_ptr<DigitalSignatureKeyPair> Crypto::getPublicKey(
        DigitalSignatureKeyPair::KeyTypes keyType, const std::string& publicKey)
{

    // Create a return key-pair object
    std::shared_ptr<DigitalSignatureKeyPair> retObj = nullptr;

    // Setup/Initialize the key-pair based on the provided type
    switch (keyType)
    {

        // Handle the Winternitz key-type
        case DigitalSignatureKeyPair::KeyTypes::WINTERNITZ:
            retObj = std::make_shared<WinternitzKeyPair>();
            break;

        // Handle the ECDSA key-type
        case DigitalSignatureKeyPair::KeyTypes::ECDSA:
            retObj = std::make_shared<EcdsaKeyPair>();
            break;

        // Handle the NONE key-type (and thus default)
        case DigitalSignatureKeyPair::KeyTypes::NONE:
            retObj = nullptr;
        default:
            retObj = nullptr;
    }

    // If the key-type was valid populate the public key portion
    if (retObj != nullptr)
        retObj->setPublicKey(publicKey);

    // Return the key-pair object
    return retObj;
}

/**
 * Function used to get a new encryption key (and IV) for symmetric encryption
 *
 * @param keyType KeyTypes representing the encryption-key type
 * @return EncryptionKey representing the private key and IV for encryption
 */
std::shared_ptr<EncryptionKey> Crypto::getEncryptionKey(EncryptionKey::KeyTypes keyType)
{

    // Create a return encryption-key object
    std::shared_ptr<EncryptionKey> retObj = nullptr;

    // Setup/Initialize the encryption-key based on the provided type
    switch (keyType)
    {

        // Handle the AES key-type
        case EncryptionKey::KeyTypes::AES:
            retObj = std::make_shared<AesEncryptionKey>();
            break;

        // Handle the NONE key-type (and thus default)
        case EncryptionKey::KeyTypes::NONE:
            retObj = nullptr;
        default:
            retObj = nullptr;
    }

    // If the key-type was valid generate the new encryption-key
    if (retObj != nullptr)
        retObj->generateNewKey();

    // Return the encryption-key object
    return retObj;
}

/**
 * Function used to get the integer (BigInt) representation of the given hexadecimal hash value
 *
 * @param hash String representing the hexadecimal hash value
 * @return BigInteger representation of the given hash
 */
BigInt Crypto::getBigIntFromHash(const std::string& hash)
{

    // Create the BigInt to return
    BigInt retVal = 0;

    // Set the conversion string for the function
    std::string hexDigits = "0123456789ABCDEF";

    // Ensure that the hash is all upper-case
    auto upperHash = boost::to_upper_copy(hash);

    // Convert each character to decimal as a BigInt and sum the results
    BigInt currPowVal = 1;
    for (long ii = upperHash.size(); ii > 0; ii--)
    {

        // Update the return value
        auto digit = upperHash[ii - 1];
        retVal += hexDigits.find(digit) * currPowVal;

        // Update the current power value
        currPowVal *= 16;
    }

    // Create a BigInteger from the given hash and return it
    return retVal;
}

/**
 * Function used to get the argon2d hash of the given string in a base64 format
 *
 * @param data String to get the hash of
 * @return String representing the hashed value (in base64 format) of the given data
 */
std::string Crypto::argon2d(const std::string& data)
{

    // Create a return string
    std::string retString;

    // Define the lengths of the different values
    uint32_t HASHLEN = 32;
    uint32_t SALTLEN = 16;

    // Ensure we do not use any salt (a zeroed salt)
    uint8_t salt[SALTLEN];
    memset(salt, 0x00, SALTLEN);

    // Setup the default const metrics for use in hashing
    uint32_t t_cost = 2;            // 1-pass computation
    uint32_t m_cost = (1<<16);      // 64 mebibytes memory usage
    uint32_t parallelism = 1;       // number of threads and lanes

    // Perform the argon2d hash on the provided data with the default cost metrics
    uint8_t hash1[HASHLEN];
    argon2d_hash_raw(t_cost, m_cost, parallelism, data.c_str(), data.size(), salt, SALTLEN, hash1, HASHLEN);

    // Copy the output hash into the output string and convert to base64 format
    for (uint32_t ii = 0; ii < HASHLEN; ii++)
        retString += ((char) hash1[ii]);
    retString = base64Encode(retString);

    // Return the return string
    return retString;
}

/**
 * Function used to get the SHA256 hash of the given string in a hex format
 *
 * @param data String to get the hash of
 * @param toUpper Boolean indicating whether the output should be upper-case
 * @param getBytes Boolean indicating whether to return bytes instead of hex
 * @return String representing the hashed value of the given data
 */
std::string Crypto::sha256(const std::string& data, bool toUpper, bool getBytes)
{

    // Create the destination vector and calculate the hash
    std::vector<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(data.begin(), data.end(), hash.begin(), hash.end());

    // Calculate the hex-form of the SHA256 hash
    std::string retHash;
    if (getBytes)
        retHash = std::string((char*) &hash[0], hash.size());
    else
        retHash = picosha2::bytes_to_hex_string(hash.begin(), hash.end());

    // Convert the hash to upper-case (if desired) and return
    if (toUpper)
        for (auto & c: retHash)
            c = toupper(c);
    return retHash;
}

/**
 * Function used to get a random SHA256 hash
 *
 * @param secure Boolean indicating whether this needs to be a secure random hash
 * @return String representing the random SHA256 hash
 */
std::string Crypto::getRandomSha256(bool secure)
{

    // Generate the hash seed based on the desired security level
    std::string seed;
    if (secure)
        seed = SecureRNG().generateRandomString(32);
    else
        seed = Utils::getUUID();

    // Return the SHA256 hash of a random UUID
    return sha256(seed);
}

/**
 * Function used to get a PoW hash based on the supplied string
 * This function will produce a "fudge value" which was used to get the PoW hash
 *
 * @param paddedZeros Unsigned Long representing the number of zeros to find a hash for
 * @param initString String representing the base string to add the fudge value to to get the PoW hash
 * @param fudgeValue String representing the fudge value required to obtain the PoW hash
 * @return String representing the PoW hash for the given string and deduced "fudge value"
 */
std::string Crypto::getPowHash(unsigned long paddedZeros, const std::string& initString,
                               std::string& fudgeValue)
{

    // Create the return string for the PoW hash
    std::string retString;

    // Continuously get a random hash (from random SHA256 of UUID) until the PoW hash criteria is met
    // When this exits, the fudge value will already be setup to include the required value to meet the criteria
    do fudgeValue = getRandomSha256();
    while (getNumberOfLeadingZerosInHash(sha256(argon2d(initString + fudgeValue))) < paddedZeros);
    retString = sha256(argon2d(initString + fudgeValue));

    // Return the return string
    return retString;
}

/**
 * Function used to encode the supplied string into its base-64 representation
 *
 * @param stringToEncode String representing the raw string to encode
 * @param urlEncode Boolean used to indicate if the string should be URL encoded
 * @return String representing the encoded string
 */
std::string Crypto::base64Encode(const std::string& stringToEncode, bool urlEncode)
{

    // Create the return values
    int index = 0;
    int inputSize = stringToEncode.size();
    char ret[inputSize * 4];

    // Setup input and variables for encode procedure
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    unsigned long in_len = stringToEncode.size();
    auto bytes_to_encode = stringToEncode.c_str();

    // Only continue while there is more valid characters to process
    while (in_len--) {

        // Actually perform the conversion
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret[index++] = base64Chars[char_array_4[i]];
            i = 0;
        }
    }

    // If the index is non-zero, correct the last bit
    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        // Actually perform the conversion
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        // Keep track of offsets during the process
        for (j = 0; (j < i + 1); j++)
            ret[index++] = base64Chars[char_array_4[j]];

        // Add any required padding
        while((i++ < 3))
            ret[index++] = '=';

    }

    // Convert to URL-safe version (if applicable)
    if (urlEncode)
    {

        for (int ii = 0; ii < index; ii++)
        {
            if (ret[ii] == '+')
                ret[ii] = '-';
            else if (ret[ii] == '/')
                ret[ii] = '_';
        }
    }

    // Return the converted string
    return std::string(ret, index);
}

/**
 * Function used to decode the supplied string from its base-64 representation
 *
 * @param stringToDecode String representing the base-64 string to decode
 * @return String representing the decoded string
 */
std::string Crypto::base64Decode(const std::string& stringToDecode)
{

    // Create the return values
    int index = 0;
    char ret[stringToDecode.size()];

    // Setup input and variables for decode procedure
    size_t in_len = stringToDecode.size();
    size_t i = 0;
    size_t j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];

    // Convert from URL-safe version (if applicable)
    std::string tmpStringToDecode = stringToDecode;
    std::replace(tmpStringToDecode.begin(), tmpStringToDecode.end(), '-', '+');
    std::replace(tmpStringToDecode.begin(), tmpStringToDecode.end(), '_', '/');

    // Only continue while there is more valid characters to process
    while (in_len-- && ( tmpStringToDecode[in_] != '=') && (std::isalnum(tmpStringToDecode[in_])
                 || (tmpStringToDecode[in_] == '+') || (tmpStringToDecode[in_] == '/'))) {
        char_array_4[i++] = tmpStringToDecode[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = static_cast<unsigned char>(base64Chars.find(char_array_4[i]));

            // Actually perform the conversion
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            // Keep track of offsets during the process
            for (i = 0; (i < 3); i++)
                ret[index++] = char_array_3[i];
            i = 0;
        }
    }

    // If the index is non-zero, correct the last bit
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        for (j = 0; j <4; j++)
            char_array_4[j] = static_cast<unsigned char>(base64Chars.find(char_array_4[j]));

        // Actually perform the conversion
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        // Keep track of offsets during the process
        for (j = 0; (j < i - 1); j++)
            ret[index++] = char_array_3[j];
    }

    // Return the converted string
    return std::string(ret, index);
}

/**
 * Function used to convert the supplied string from hexidecimal to binary
 *
 * @param stringToDecode String representing the base-64 string to decode
 * @return String representing the decoded string
 */
std::string Crypto::hexToBinary(const std::string& hexString)
{

    // Create a return string
    std::string retString = "";

    // Convert the hex-string to binary iteratively
    for (size_t ii = 0; ii < hexString.size(); ii += 2)
    {

        // Extract two characters from hex string
        std::string part = hexString.substr(ii, 2);

        // Convert it to base 16 then to binary
        // and append it to the output string
        char ch = stoul(part, nullptr, 16);
        retString += ch;
    }

    // Return the return string
    return retString;
}

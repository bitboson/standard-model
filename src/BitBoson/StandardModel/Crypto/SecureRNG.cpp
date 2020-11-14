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

#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <BitBoson/StandardModel/Crypto/Crypto.h>
#include <BitBoson/StandardModel/Crypto/SecureRNG.h>

using namespace BitBoson::StandardModel;

/**
 * Constructor used to setup the Secure Random Number Generator
 */
SecureRNG::SecureRNG()
{

    // Generate a Secure-Byte-Block for scratch area for the
    // secure random number generator to work with
    const unsigned int BLOCKSIZE = 16 * 8;
    _scratch = CryptoPP::SecByteBlock(BLOCKSIZE);
}

/**
 * Function used to generate a random String using the Secure
 * Random Number Generator
 *
 * @param length Integer representing the length of the string
 * @return String representing the random byte-string
 */
std::string SecureRNG::generateRandomString(int length)
{

    // Generate the corresponding random byte-block
    auto randByteBlock = generateRandomByteBlock(length);

    // Convert the byte-block to a string and return it
    std::string randString(reinterpret_cast<const char*>(&randByteBlock[0]), randByteBlock.size());
    return randString;
}

/**
 * Static function used to generate a random BigInt using the seed
 * NOTE: The same seed will yield the same results every time
 *
 * @param seed String representing the seed to use
 * @param bound BigInt representing the upper-bound
 * @return BigInt representing the random BigInt
 */
BigInt SecureRNG::generateRandomBigIntSeeded(const std::string& seed, BigInt bound)
{

    // Setup the secure byte-block for the given seed
    auto seedHash = Crypto::sha256(seed);
    CryptoPP::SecByteBlock seedBlock((CryptoPP::byte*) (&seedHash[0]), seedHash.size());

    // Create the secure random-number-generator based on the seed
    CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption prng;
    prng.SetKeyWithIV(seedBlock, 32, seedBlock + 32, 16);

    // Generate the random number usig the secure random-number-generator
    CryptoPP::SecByteBlock randomBlock(16);
    prng.GenerateBlock(randomBlock, randomBlock.size());

    // Extract the random block as a string for conversion
    std::string randomBlockString;
    CryptoPP::HexEncoder hex(new CryptoPP::StringSink(randomBlockString));
    hex.Put(randomBlock, randomBlock.size());
    hex.MessageEnd();

    // Convert the hex-string into a BigInt and return it
    // NOTE: We'll also apply the bound if it is non-zero
    auto randomInt = Crypto::getBigIntFromHash(randomBlockString);
    if (bound > 0)
        randomInt = (randomInt % bound);
    return randomInt;
}

/**
 * Function used to generate a random string using the Secure
 * Random Number Generator
 *
 * @param length Integer representing the length of the string
 * @return SecByteBlock representing the random byte-block
 */
CryptoPP::SecByteBlock SecureRNG::generateRandomByteBlock(int length)
{

    // Create a return SecByteBlock
    CryptoPP::SecByteBlock retString;

    // Continually generate and add to the return string until
    // the desired length is reached
    while (retString.size() < length)
    {

        // Generate a new block of random bytes
        _rng.GenerateBlock(_scratch, _scratch.size());
        CryptoPP::SecByteBlock tmpBlock(_scratch);

        // Resize the current block to append if desired/required
        if ((length - retString.size()) < tmpBlock.size())
            tmpBlock.resize(length - retString.size());

        // Add the current random byte block to the output block
        retString += CryptoPP::SecByteBlock(tmpBlock);
    }

    // Return the return SecByteBlock
    return retString;
}

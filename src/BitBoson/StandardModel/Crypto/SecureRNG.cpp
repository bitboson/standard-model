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

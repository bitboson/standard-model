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

#ifndef BITBOSON_STANDARDMODEL_SECURERNG_H
#define BITBOSON_STANDARDMODEL_SECURERNG_H

#include <string>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include <cryptopp/cryptlib.h>

namespace BitBoson::StandardModel
{

    class SecureRNG
    {

        // Private member variables
        private:
            CryptoPP::SecByteBlock _scratch;
            CryptoPP::AutoSeededRandomPool _rng;

        // Public member functions
        public:

            /**
             * Constructor used to setup the Secure Random Number Generator
             */
            SecureRNG();

            /**
             * Function used to generate a random String using the Secure
             * Random Number Generator
             *
             * @param length Integer representing the length of the string
             * @return String representing the random byte-string
             */
            std::string generateRandomString(int length);

            /**
             * Static function used to generate a random BigInt using the seed
             * NOTE: The same seed will yield the same results every time
             *
             * @param seed String representing the seed to use
             * @param bound BigInt representing the upper-bound
             * @return BigInt representing the random BigInt
             */
            static BigInt generateRandomBigIntSeeded(const std::string& seed, BigInt bound=0);

            /**
             * Function used to generate a random SecBlock using the Secure
             * Random Number Generator
             *
             * @param length Integer representing the length of the string
             * @return SecByteBlock representing the random byte-string
             */
            CryptoPP::SecByteBlock generateRandomByteBlock(int length);

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~SecureRNG() = default;
    };
}

#endif //BITBOSON_STANDARDMODEL_SECURERNG_H

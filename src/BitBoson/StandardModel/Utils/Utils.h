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

#ifndef BITBOSON_STANDARDMODEL_UTILS_H
#define BITBOSON_STANDARDMODEL_UTILS_H

#include <string>
#include <vector>
#include <BitBoson/StandardModel/Primitives/BigInt.hpp>
#include <BitBoson/StandardModel/Primitives/BigFloat.hpp>

namespace BitBoson::StandardModel
{

    namespace Utils
    {

        // Enum used to specify parsing types with string manipulations
        enum ParseType
        {
            First,
            Last,
            Outer,
            Inner
        };

        /**
         * Function used to get the string representation of a BigInt
         *
         * @param bigInt BigInt to get the string representation of
         * @return String representation of the given BigInt
         */
        std::string getBigIntString(const BigInt& bigInt);

        /**
         * Function used to get the string representation of a BigFloat
         *
         * @param bigInt BigFloat to get the string representation of
         * @return String representation of the given BigFloat
         */
        std::string getBigFloatString(const BigFloat& bigFloat);

        /**
         * Function used to get a random UUID string
         *
         * @return String representing the UUID
         */
        std::string getUUID();

        /**
         * Function used to get the file-string representation of a given list/vector of items and a signature
         *
         * @param itemsToPack Vector of strings to pack into the file string
         * @return String representing the file string for the given items
         */
        std::string getFileString(const std::vector<std::string>& itemsToPack);

        /**
         * Function used to parse a given file string into its component items
         *
         * @param fileString FileString representing the object to parse
         * @return Vector fo strings representing the individual items from the file string
         */
        std::vector<std::string> parseFileString(const std::string& fileString);

        /**
         * Function used to simply concatenate the given string vector parts
         * into a single string representation
         *
         * @param stringParts Vector of strings to concatenate
         * @return Concatenated string from the vector parts
         */
        std::string combineStringParts(std::vector<std::string> stringParts);

        /**
         * Function used to split the string into equal parts given the part size
         * NOTE: Will return empty vector if the string length isn't a multiple of part-size
         *
         * @param stringToSplit String to split into the equal parts
         * @param partSize Unsigned Long representing the number of parts to split it into
         * @return Vector of String parts from the provided string
         */
        std::vector<std::string> splitStringIntoParts(const std::string& stringToSplit, unsigned long partSize);

        /**
         * Function used to get the string between two delimiters (first/last occurrence of each)
         * NOTE: Will return empty string if invalid
         *
         * @param originalString The String to extract the substring from
         * @param startDelimiter The String representing the start delimiter for the splitting process
         * @param stopDelimiter The String representing the stop delimiter for the splitting process
         * @param parseType ParseType indicating how to parse the string based on the delimiters (and order)
         * @return String representing the substring between the two given delimiters or empty if invalid
         */
        std::string getStringBetweenSubStrings(const std::string& originalString,
                                               const std::string& startDelimiter,
                                               const std::string& stopDelimiter, ParseType parseType);

        /**
         * Function used to split the given string into a vector of strings based on the delimiter given
         *
         * @param stringToSplit String to split on the delimiter
         * @param delimiters String representing the delimiter(s) (each character) to split the original string on
         * @return Vector of String representing the parts of the original string split on the delimiter
         */
        std::vector<std::string> splitStringByDelimiter(const std::string& stringToSplit,
                                                        const std::string& delimiters);
    };
}

#endif //BITBOSON_STANDARDMODEL_UTILS_H

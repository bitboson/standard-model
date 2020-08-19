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

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <BitBoson/StandardModel/Utils/Utils.h>
#include <BitBoson/StandardModel/Crypto/Crypto.h>

using namespace BitBoson::StandardModel;

/**
 * Function used to get the string representation of a BigInt
 *
 * @param bigInt BigInt to get the string representation of
 * @return String representation of the given BigInt
 */
std::string Utils::getBigIntString(const BigInt& bigInt)
{
    return boost::lexical_cast<std::string>(bigInt);
}

/**
 * Function used to get the string representation of a BigFloat
 *
 * @param bigInt BigFloat to get the string representation of
 * @return String representation of the given BigFloat
 */
std::string Utils::getBigFloatString(const BigFloat& bigFloat)
{
    return boost::lexical_cast<std::string>(bigFloat);
}

/**
 * Function used to get a random UUID string
 *
 * @return String representing the UUID
 */
std::string Utils::getUUID()
{

    // Generate the actual UUID
    boost::uuids::random_generator generator;

    // Return the string
    return boost::lexical_cast<std::string>(generator());
}

/**
 * Function used to get the file-string representation of a given list/vector of items and a signature
 *
 * @param itemsToPack Vector of strings to pack into the file string
 * @param signature String representing the signature for the object
 * @return String representing the file string for the given items
 */
std::string Utils::getFileString(const std::vector<std::string>& itemsToPack,
                                 const std::string& signature)
{

    // Create the String to return
    std::string fileString = "{";

    // Add the given items to pack (base 64 encoded) to the file string
    for (const std::string& item : itemsToPack)
        fileString += Crypto::base64Encode(item) + ",";
    fileString += "}";

    // Add the signature to the end of the file String
    fileString += "{" + Crypto::base64Encode(signature) + "}";

    // Return the file String (in base 64 encoded format)
    return Crypto::base64Encode(fileString);
}

/**
 * Function used to parse a given file string into its component items
 * NOTE: The last item represents the signature item (it will be blank if there was no signature)
 *
 * @param fileString FileString representing the object to parse
 * @param includeSignature Boolean indicating whether to include the signature item or not
 * @return Vector fo strings representing the individual items from the file string
 */
std::vector<std::string> Utils::parseFileString(const std::string& fileString, bool includeSignature)
{

    // Decode the file string (they are base 64 encoded)
    std::string decodedString = Crypto::base64Decode(fileString);

    // Extract the file data from the file string
    std::string fileData = Utils::getStringBetweenSubStrings(decodedString, "{", "}", ParseType::First);
    std::vector<std::string> fileDataItems = Utils::splitStringByDelimiter(fileData, ",");

    // Decode all of the items in the vector
    for (auto& fileDataItem : fileDataItems)
        fileDataItem = Crypto::base64Decode(fileDataItem);

    // Extract the signature portion from the file string and add to the return vector
    std::string signature = Utils::getStringBetweenSubStrings(decodedString, "{", "}", ParseType::Last);
    fileDataItems[fileDataItems.size() - 1] = Crypto::base64Decode(signature);

    // If the only item is an empty one, clear the vector
    // This is essentially a correction in the way we process empty strings
    // due to signature addition on the end
    if ((fileDataItems.size() == 1) && (fileDataItems[0].empty()))
        fileDataItems.clear();

    // If the return vector is non-zero in length, strip the signature if desired
    if (!fileDataItems.empty() && !includeSignature)
        fileDataItems.pop_back();

    // Return the vector of file string components
    return fileDataItems;
}

/**
 * Function used to simply concatenate the given string vector parts
 * into a single string representation
 *
 * @param stringParts Vector of strings to concatenate
 * @return Concatenated string from the vector parts
 */
std::string Utils::combineStringParts(std::vector<std::string> stringParts)
{

    // Create a return string
    std::string retString;

    // Concatenate the vector parts
    for (const auto& item : stringParts)
        retString += item;

    // Return the return string
    return retString;
}

/**
 * Function used to split the string into equal parts given the part size
 * NOTE: Will return empty vector if the string length isn't a multiple of part-size
 *
 * @param stringToSplit String to split into the equal parts
 * @param partSize Unsigned Long representing the number of parts to split it into
 * @return Vector of String parts from the provided string
 */
std::vector<std::string> Utils::splitStringIntoParts(const std::string& stringToSplit,
                                                     unsigned long partSize)
{

    // Create the return vector
    std::vector<std::string> retVect;

    // Verify the length is a multiple of the part-size
    // and split the string into the parts
    if (partSize > 0)
        if (stringToSplit.length() % partSize == 0)
            for (unsigned long ii = 0; ii < (stringToSplit.length() / partSize); ii++)
                retVect.push_back(stringToSplit.substr(partSize * ii, partSize));

    // Return the return vector
    return retVect;
}

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
std::string Utils::getStringBetweenSubStrings(const std::string& originalString,
                                              const std::string& startDelimiter, const std::string& stopDelimiter, ParseType parseType)
{

    // Create the string to return
    std::string subString;

    // Create the variables to hold the indices of the delimiters
    unsigned long firstIndx = 0;
    unsigned long lastIndx = 0;

    // Extract the indices of the start and stop delimiters based on the parse type
    switch (parseType)
    {

        // Handle the First case
        case ParseType::First:
            firstIndx = originalString.find(startDelimiter) + 1;
            lastIndx = originalString.find(stopDelimiter);
            break;

            // Handle the Last case
        case ParseType::Last:
            firstIndx = originalString.find_last_of(startDelimiter) + 1;
            lastIndx = originalString.find_last_of(stopDelimiter);
            break;

            // Handle the Outer case
        case ParseType::Outer:
            firstIndx = originalString.find(startDelimiter) + 1;
            lastIndx = originalString.find_last_of(stopDelimiter);
            break;

            // Handle the Inner case
        default:
            firstIndx = originalString.find_last_of(startDelimiter) + 1;
            lastIndx = originalString.find(stopDelimiter);
    }

    // Only attempt to extract the string if the indices make sense
    if ((firstIndx >= 0) && (firstIndx < lastIndx) && (lastIndx < originalString.size()))
        subString = originalString.substr(firstIndx, lastIndx - firstIndx);

    // Return the new substring
    return subString;
}

/**
 * Function used to split the given string into a vector of strings based on the delimiter given
 *
 * @param stringToSplit String to split on the delimiter
 * @param delimiters String representing the delimiter(s) (each character) to split the original string on
 * @return Vector of String representing the parts of the original string split on the delimiter
 */
std::vector<std::string> Utils::splitStringByDelimiter(const std::string& stringToSplit,
                                                       const std::string& delimiters)
{

    // Create a vector to return
    std::vector<std::string> stringItems;

    // Split the given string on the given delimiter
    boost::split(stringItems, stringToSplit, boost::is_any_of(delimiters));

    // Return the vector of items
    return stringItems;
}

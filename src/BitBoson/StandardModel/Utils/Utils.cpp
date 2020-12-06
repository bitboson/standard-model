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

#include <regex>
#include <yas/mem_streams.hpp>
#include <yas/binary_iarchive.hpp>
#include <yas/binary_oarchive.hpp>
#include <yas/std_types.hpp>
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
 * @return String representing the file string for the given items
 */
std::string Utils::getFileString(const std::vector<std::string>& itemsToPack)
{

    // Create a return string
    std::string retString;

    // Only continue if the packed-vector is valid
    if (!itemsToPack.empty())
    {

        // Perform operations in a try-catch for error-handling
        try
        {

            // Un-pack the vector into the return string
            yas::mem_ostream os;
            constexpr size_t opts = yas::mem | yas::binary | yas::no_header;
            yas::binary_oarchive<yas::mem_ostream, opts> oa(os);
            oa & itemsToPack;
            auto buf = os.get_intrusive_buffer();
            retString.assign(buf.data, buf.size);
        }

        // Catch any and all exceptions here
        catch (...)
        {

            // Clear the return string on exception
            retString = "";
        }
    }

    // Return the return string
    return retString;
}

/**
 * Function used to parse a given file string into its component items
 *
 * @param fileString FileString representing the object to parse
 * @return FileStringVect representing the individual items from the file string
 */
std::shared_ptr<Utils::FileStringVect> Utils::parseFileString(const std::string& fileString)
{

    // Create a return object
    std::shared_ptr<FileStringVect> retObj = nullptr;

    // Only continue if the file-string is valid
    if (!fileString.empty())
    {

        // Perform operations in a try-catch for error-handling
        try
        {

            // Parse the input file-string into the return vector
            retObj = std::make_shared<FileStringVect>();
            constexpr size_t opts = yas::mem | yas::binary | yas::no_header;
            yas::mem_istream is(fileString.c_str(), fileString.size());
            yas::binary_iarchive<yas::mem_istream, opts> ia(is);
            ia & retObj->rawVect;
            retObj->size = retObj->rawVect.size();
            retObj->index = 0;
        }

        // Catch any and all exceptions here
        catch (...)
        {

            // Clear the return vector on exception
            retObj = nullptr;
        }
    }

    // Return the return object
    return retObj;
}

/**
 * Function used to get the next individual file-string vector value
 * NOTE: This can also be used to enforce regex checks (should be used)
 * NOTE: This operation will increment the index regardless of matching or not
 *
 * @param fileStringVect FileStringVect representing the file-string vector
 * @param regexCriteria String representing the regex-criteria to use
 * @param requiredSize Unsigned-Long representing the required size for the string
 * @return String representing the value from the vector (if valid/matched)
 */
std::string Utils::getNextFileStringValue(std::shared_ptr<FileStringVect> fileStringVect,
        const std::string& regexCriteria, unsigned long requiredSize)
{

    // Create a return string
    std::string retString;

    // Only continue if the provided information is valid
    if (fileStringVect->index < fileStringVect->rawVect.size())
    {

        // Get the return value from the vector if the regex checks-out
        auto tmpString = fileStringVect->rawVect[fileStringVect->index];
        if (regexCriteria.empty() || (std::regex_match(tmpString, std::regex(regexCriteria))
                && ((requiredSize == 0) || (tmpString.size() == requiredSize))))
            retString = tmpString;
    }

    // Increment the provided index regardless of the return value
    fileStringVect->index++;

    // Return the return string
    return retString;
}

/**
* Overloaded function used to get the next individual file-string vector value
* NOTE: This can also be used to enforce regex checks (should be used)
* NOTE: This operation will increment the index regardless of matching or not
*
* @param fileStringVect FileStringVect representing the file-string vector
* @param regexCriteria RegexType representing the regex-criteria to use
* @return String representing the value from the vector (if valid/matched)
*/
std::string Utils::getNextFileStringValue(std::shared_ptr<FileStringVect> fileStringVect,
        RegexType regexType)
{

    // Create a return string
    std::string retString;

    // Perform the lookup based on the provided regex definition
    switch (regexType)
    {

        // Handle the SHA256 regex case
        case RegexType::Sha256:
            retString = getNextFileStringValue(fileStringVect, "[A-Za-z0-9]*", 64);
            break;

        // Handle the Base64 regex case
        case RegexType::Base64:
            retString = getNextFileStringValue(fileStringVect, "[A-Za-z0-9+/]*");
            break;

        // Handle the DecimalNumber regex case
        case RegexType::DecimalNumber:
            retString = getNextFileStringValue(fileStringVect, "[0-9]*.[0-9]*");
            break;

        // Handle the IntegerNumber regex case
        case RegexType::IntegerNumber:
            retString = getNextFileStringValue(fileStringVect, "[0-9]*");
            break;

        // Handle the AlphaNumeric regex case
        case RegexType::AlphaNumeric:
            retString = getNextFileStringValue(fileStringVect, "[A-Za-z0-9]*");
            break;
    }

    // Return the return string
    return retString;
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

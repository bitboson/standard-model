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

#ifndef BITBOSON_STANDARDMODEL_COMPARABLESTRING_HPP
#define BITBOSON_STANDARDMODEL_COMPARABLESTRING_HPP

#include <boost/algorithm/string.hpp>
#include <BitBoson/StandardModel/Crypto/Crypto.h>

namespace BitBoson::StandardModel
{

    class ComparableString
    {

        // Private member variables
        private:
            //BigInt _comparableValue;
            std::string _internalString;

        // Public member functions
        public:

            /**
             * Constructor used to setup the default instance
             */
            ComparableString() = default;

            /**
             * Constructor used to setup the comparable string on the given message
             *
             * @param message String representing the message to store internally
             */
            explicit ComparableString(const std::string& message)
            {

                // Store the upper-case alpha-numeric string of the input message
                _internalString = message;
                boost::to_upper(_internalString);

                // Setup the internal value for comparisons
                //_comparableValue = getBigIntFromText(_internalString);
            }

            /**
             * Function used to get the stored string value of the input message
             *
             * @return String representing the string value of the message
             */
            std::string getString() const
            {

                // Return the internal hash value
                return _internalString;
            }

            /**
             * Function used to get the Big Integer comparable value of the instance
             *
             * @return Big Integer representing the underlying comparable value
             */
            BigInt getComparableValue() const
            {

                // Return the comparable value
                return getBigIntFromText(_internalString);
            }

            /**
             * Operator overloading used to check for equality
             *
             * @param rhs Right-Hand-Size object used in the comparison
             * @return Boolean indicating if the comparison is true
             */
            bool operator==(const ComparableString &rhs) const
            {
                return getBigIntFromText(_internalString) == getBigIntFromText(rhs._internalString);
            }

            /**
             * Operator overloading used to check for inequality
             *
             * @param rhs Right-Hand-Size object used in the comparison
             * @return Boolean indicating if the comparison is true
             */
            bool operator!=(const ComparableString &rhs) const
            {
                return !(rhs == *this);
            }

            /**
             * Operator overloading used to check for less-than property
             *
             * @param rhs Right-Hand-Size object used in the comparison
             * @return Boolean indicating if the comparison is true
             */
            bool operator<(const ComparableString &rhs) const
            {
                return getBigIntFromText(_internalString) < getBigIntFromText(rhs._internalString);
            }

            /**
             * Operator overloading used to check for greater-than property
             *
             * @param rhs Right-Hand-Size object used in the comparison
             * @return Boolean indicating if the comparison is true
             */
            bool operator>(const ComparableString &rhs) const
            {
                return rhs < *this;
            }

            /**
             * Operator overloading used to check for less-than or equal-to property
             *
             * @param rhs Right-Hand-Size object used in the comparison
             * @return Boolean indicating if the comparison is true
             */
            bool operator<=(const ComparableString &rhs) const
            {
                return !(rhs < *this);
            }

            /**
             * Operator overloading used to check for greater-than or equal-to property
             *
             * @param rhs Right-Hand-Size object used in the comparison
             * @return Boolean indicating if the comparison is true
             */
            bool operator>=(const ComparableString &rhs) const
            {
                return !(*this < rhs);
            }

            /**
             * Operator overloading used to get the difference between two comparable types
             *
             * @param rhs Right-Hand-Size object used in the subtraction/difference
             * @return Big Integer representing the difference
             */
            BigInt operator-(ComparableString &rhs)
            {
                return getBigIntFromText(_internalString) - getBigIntFromText(rhs._internalString);
            }

            /**
             * Friend function used to get the string-stream representation of the instance
             *
             * @param os Object Stream used to put instance string data into
             * @param obj Comparable String used to get the string data from
             * @return Object Stream after being processed by the given object's instance
             */
            friend std::ostream& operator<<(std::ostream& os, const ComparableString& obj){
                return os << obj.getStringRepresentation();
            }

            /**
             * Friend function used to set the string-stream representation for the instance
             *
             * @param input Object Stream used to get the string instance data from
             * @param obj Comparable String representing the object to put string data into
             * @return Object Stream after being processed by the given object's instance
             */
            friend std::istream& operator>>(std::istream &input, ComparableString& obj)
            {
                std::string temporaryInput;
                input >> temporaryInput;
                obj.setStringRepresentation(temporaryInput);
                return input;
            }

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~ComparableString() = default;

        // Private member functions
        private:

            /**
             * Internal static function used to get the integer (BigInt) representation of
             * the given alpha-numeric string value
             *
             * @param hash String representing the hexadecimal hash value
             * @return BigInteger representation of the given hash
             */
            static BigInt getBigIntFromText(const std::string& text)
            {

                // Create the BigInt to return
                BigInt retVal = 0;

                // Set the conversion string for the function
                std::string charDigits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

                // Convert each character to decimal as a BigInt and sum the results
                BigInt currPowVal = 1;
                for (long ii = text.size(); ii > 0; ii--)
                {

                    // Update the return value
                    auto digit = text[ii - 1];
                    retVal += charDigits.find(digit) * currPowVal;

                    // Update the current power value
                    currPowVal *= charDigits.size();
                }

                // Create a BigInteger from the given hash and return it
                return retVal;
            }

            /**
             * Overridden internal function used to get the string representation of the object
             * NOTE: This is to be used in boost::lexical_cast support
             *
             * @return String representing the object instance
             */
            std::string getStringRepresentation() const
            {

                // Get and return the internal string value
                return _internalString;
            }

            /**
             * Overridden internal function used to set the string representation for the object
             * NOTE: This is to be used in boost::lexical_cast support
             *
             * @param stringRep String representing the object instance
             */
            void setStringRepresentation(const std::string& stringRep)
            {

                // Set the internal string value
                _internalString = stringRep;
            }
    };
}

#endif //BITBOSON_STANDARDMODEL_COMPARABLESTRING_HPP

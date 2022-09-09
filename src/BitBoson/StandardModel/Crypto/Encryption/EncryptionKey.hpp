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

#ifndef BITBOSON_STANDARDMODEL_ENCRYPTIONKEY
#define BITBOSON_STANDARDMODEL_ENCRYPTIONKEY

#include <string>

namespace BitBoson::StandardModel
{

    class EncryptionKey
    {

        // Public enumerations
        public:
            enum KeyTypes
            {
                AES,
                NONE
            };


        // private member variables
        private:
            std::string _encryptionKey;
            std::string _initializationVector;

        // Public member functions
        public:

            /**
             * Constructor used to setup the instance
             */
            EncryptionKey() = default;

            /**
             * Function used to get the encryption key
             *
             * @return String representing the encryption key
             */
            std::string getEncryptionKey() const
            {

                // Simply return the encryption key
                return _encryptionKey;
            }

            /**
             * Function used to set the encryption key
             *
             * @param encryptionKey String representing the encryption key
             */
            void setEncryptionKey(const std::string& encryptionKey)
            {

                // Simply set the encryption key
                _encryptionKey = encryptionKey;
            }

            /**
             * Virtual function used to generate a new encryption key
             */
            virtual void generateNewKey() = 0;

            /**
             * Virtual function used to get the encryption key type
             *
             * @return KeyTypes representing the encryption key type
             */
            virtual KeyTypes getKeyType() const = 0;

            /**
             * Virtual function used to encrypt the provided clear-text message using the encryption key
             *
             * @param plainText String representing the plain-text message to encrypt
             * @return String representing the cipher-text representation of the plain-text message
             */
            virtual std::string encrypt(const std::string& plainText) = 0;

            /**
             * Virtual function used to decrypt the provided cipher-text message using the encryption key
             *
             * @param plainText String representing the cipher-text message to decrypt
             * @return String representing the plain-text representation of the cipher-text message
             */
            virtual std::string decrypt(const std::string& cipherText) = 0;

            /**
             * Static function used to get the string representation of a key-type
             *
             * @param keyType KeyTypes representing the key type to get the string for
             * @return String representation of the provided key-type
             */
            static std::string getKeyTypeString(KeyTypes keyType)
            {

                // Create a return string
                std::string retString;

                // Convert the string based on the key-type
                switch (keyType)
                {

                    // Handle the AES key-type
                    case KeyTypes::AES:
                        retString = "AES";
                        break;

                    // Handle the NONE key-type (and thus default)
                    case KeyTypes::NONE:
                        retString = "NONE";
                    default:
                        retString = "NONE";
                }

                // Return the return string
                return retString;
            }

            /**
             * Static function used to get the key-type for the given string representation
             *
             * @param keyTypeString String representation of the key-type to convert
             * @return KeyTypes representing the key-type for the provided string
             */
            static KeyTypes getKeyTypeFromString(const std::string& keyTypeString)
            {

                // Create a return key-type
                KeyTypes retKeyType = KeyTypes::NONE;

                // Convert the string to the corresponding key-type
                if (keyTypeString == "AES")
                    retKeyType = KeyTypes::AES;

                // Return the return key-type
                return retKeyType;
            }

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~EncryptionKey() = default;
    };
}

#endif //BITBOSON_STANDARDMODEL_ENCRYPTIONKEY

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

#ifndef BITBOSON_STANDARDMODEL_DIGITALSIGNATUREKEYPAIR
#define BITBOSON_STANDARDMODEL_DIGITALSIGNATUREKEYPAIR

namespace BitBoson::StandardModel
{

    class DigitalSignatureKeyPair
    {

        // Public enumerations
        public:
            enum KeyTypes
            {
                WINTERNITZ,
                ECDSA,
                NONE
            };

        // private member variables
        private:
            std::string _publicKey;
            std::string _privateKey;

        // Public member functions
        public:

            /**
             * Constructor used to setup the instance
             */
            DigitalSignatureKeyPair() = default;

            /**
             * Function used to get the public key
             *
             * @return String representing the public key
             */
            std::string getPublicKey() const
            {

                // Simply return the public key
                return _publicKey;
            }

            /**
             * Virtual function used to set the public key
             *
             * @param publicKey String representing the public key
             */
            void setPublicKey(const std::string& publicKey)
            {

                // Simply set the public key
                _publicKey = publicKey;
            }

            /**
             * Function used to get the private key
             *
             * @return String representing the private key
             */
            std::string getPrivateKey() const
            {

                // Simply return the private key
                return _privateKey;
            }

            /**
             * Function used to set the private key
             *
             * @param privateKey String representing the private key
             */
            void setPrivateKey(const std::string& privateKey)
            {

                // Simply set the private key
                _privateKey = privateKey;
            }

            /**
             * Virtual function used to generate a new key-pair
             */
            virtual void generateNewKeyPair() = 0;

            /**
             * Virtual function used to get the key-pair type
             *
             * @return KeyTypes representing the key-pair type
             */
            virtual KeyTypes getKeyType() const = 0;

            /**
             * Virtual function used to sign the given message
             *
             * @param message String representing the message to sign
             * @return String representing the signed message (signature)
             */
            virtual std::string sign(const std::string& message) const = 0;

            /**
             * Virtual function used to verify the message signature
             *
             * @param message String representing the message to use
             * @param signature String representing the signature to verify
             * @return Boolean indicating if the signature is valid or not
             */
            virtual bool isValid(const std::string& message, const std::string& signature) const = 0;

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

                    // Handle the Winternitz key-type
                    case KeyTypes::WINTERNITZ:
                        retString = "WINTERNITZ";
                        break;

                    // Handle the ECDSA key-type
                    case KeyTypes::ECDSA:
                        retString = "ECDSA";
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
            static KeyTypes getKeyTypeString(const std::string& keyTypeString)
            {

                // Create a return key-type
                KeyTypes retKeyType = KeyTypes::NONE;

                // Convert the string to the corresponding key-type
                if (keyTypeString == "WINTERNITZ")
                    retKeyType = KeyTypes::WINTERNITZ;
                else if (keyTypeString == "ECDSA")
                    retKeyType = KeyTypes::ECDSA;

                // Return the return key-type
                return retKeyType;
            }

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~DigitalSignatureKeyPair() = default;
    };
}

#endif //BITBOSON_STANDARDMODEL_DIGITALSIGNATUREKEYPAIR

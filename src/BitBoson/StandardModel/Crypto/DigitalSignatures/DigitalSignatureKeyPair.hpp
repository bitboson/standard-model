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
             * Destructor used to cleanup the instance
             */
            virtual ~DigitalSignatureKeyPair() = default;

        // Protected member functions
        protected:

            /**
             * Internal function used to get the private key
             *
             * @return String representing the private key
             */
            std::string getPrivateKey() const
            {

                // Simply return the private key
                return _privateKey;
            }

            /**
             * Internal function used to set the private key
             *
             * @param privateKey String representing the private key
             */
            void setPrivateKey(const std::string& privateKey)
            {

                // Simply set the private key
                _privateKey = privateKey;
            }

    };
}

#endif //BITBOSON_STANDARDMODEL_DIGITALSIGNATUREKEYPAIR

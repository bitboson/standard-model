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

#ifndef BITBOSON_STANDARDMODEL_ISIGNABLE_HPP
#define BITBOSON_STANDARDMODEL_ISIGNABLE_HPP

#include <BitBoson/StandardModel/Crypto/Crypto.h>
#include <BitBoson/StandardModel/DataStructures/Cacheable.hpp>
#include <BitBoson/StandardModel/Crypto/DigitalSignatures/DigitalSignatureKeyPair.hpp>

namespace BitBoson::StandardModel
{

    class Signable : public Cacheable
    {

        // Private member variables
        private:
            DigitalSignatureKeyPair::KeyTypes _keyType;
            std::string _signature;

        // Public member functions
        public:

            /**
             * Default constructor used to setup the class
             */
             Signable()
             {

                // Setup default member variable values
                _keyType = DigitalSignatureKeyPair::KeyTypes::NONE;
             }

            /**
             * Copy constructor used to setup the class from another
             *
             * @param signable Signable reference used to setup the class from
             */
            Signable(const Signable& signable)
            {
                _keyType = signable._keyType;
                _signature = signable._signature;
            }

            /**
             * Virtual function used to set the signature for the underlyig instance
             *
             * @param signature String representing the signature to set
             */
            virtual void setSignature(const std::string& signature)
            {
                _signature = signature;
            }

            /**
             * Virtual function used to sign the object using the given key-pair information
             *
             * @param keyPair DigitalSignatureKeyPair representing the signing key-pair
             */
            virtual void sign(std::shared_ptr<DigitalSignatureKeyPair> keyPair)
            {
                _keyType = keyPair->getKeyType();
                setSignature(keyPair->sign(getUniqueHash()));
            }

            /**
             * Virtual function used to verify the object's signature (internal) using the given key
             *
             * @param publicKey Base64 String representation of the public key
             * @return Boolean indicating whether the internal signature is valid or not
             */
            virtual bool isValid(const std::string& publicKey) const
            {
                auto pubKey = Crypto::getPublicKey(_keyType, publicKey);
                return ((pubKey == nullptr) ? false : pubKey->isValid(getUniqueHash(), _signature));
            }

            /**
             * Virtual function used to get the string representation of the actual signature for the object
             *
             * @return String representation of the signature for the object
             */
            virtual std::string getSignature() const
            {
                return _signature;
            }

            /**
             * Virtual function used to get the key-type for the signed object
             *
             * @return KeyTypes representing the key-type for the signed object
             */
            virtual DigitalSignatureKeyPair::KeyTypes getKeyType() const
            {
                return _keyType;
            }

            /**
             * Virtual function used to set the key-type for the signed object
             *
             * @param keyType KeyTypes representing the key-type for the signed object
             */
            virtual void setKeyType(DigitalSignatureKeyPair::KeyTypes keyType)
            {
                _keyType = keyType;
            }

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~Signable() = default;

    };
}

#endif //BITBOSON_STANDARDMODEL_ISIGNABLE_HPP

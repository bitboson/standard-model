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

#ifndef BITBOSON_STANDARDMODEL_ECSDAKEYPAIR
#define BITBOSON_STANDARDMODEL_ECSDAKEYPAIR

#include <cryptopp/asn.h>
#include <cryptopp/ecp.h>
#include <cryptopp/sha.h>
#include <cryptopp/rng.h>
#include <cryptopp/hex.h>
#include <cryptopp/dsa.h>
#include <cryptopp/misc.h>
#include <cryptopp/oids.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/cryptlib.h>
#include <BitBoson/StandardModel/Utils/Utils.h>
#include <BitBoson/StandardModel/Crypto/Crypto.h>
#include <BitBoson/StandardModel/Crypto/DigitalSignatures/DigitalSignatureKeyPair.hpp>

namespace BitBoson::StandardModel
{

    class EcdsaKeyPair : public DigitalSignatureKeyPair
    {

        // Public member functions
        public:

            /**
             * Constructor used to setup the instance
             */
            EcdsaKeyPair() = default;

            /**
             * Overridden function used to generate a new key-pair
             */
            void generateNewKeyPair() override
            {

                // Create the new ECDSA private key
                CryptoPP::AutoSeededRandomPool prng;
                CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
                privateKey.Initialize(prng, CryptoPP::ASN1::secp256k1());

                // Convert the Secret Integer component to a hex-string
                // and save it to the instance as the private key
                std::string privateKeyRaw;
                CryptoPP::StringSink stringSinkPriv(privateKeyRaw);
                privateKey.Save(stringSinkPriv);
                setPrivateKey(Crypto::base64Encode(privateKeyRaw));

                // Create the public key from the private key
                CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;
                privateKey.MakePublicKey(publicKey);

                // Convert the Public Point component to a hex-string
                // and save it to the instance as the public key
                std::string publicKeyRaw;
                CryptoPP::StringSink stringSinkPub(publicKeyRaw);
                publicKey.Save(stringSinkPub);
                setPublicKey(Crypto::base64Encode(publicKeyRaw));
            }

            /**
             * Overridden function used to get the key-pair type
             *
             * @return KeyTypes representing the key-pair type
             */
            KeyTypes getKeyType() const override
            {

                // Simply return the hard-coded key type
                return DigitalSignatureKeyPair::KeyTypes::ECDSA;
            }

            /**
             * Overridden function used to sign the given message
             *
             * @param message String representing the message to sign
             * @return String representing the signed message (signature)
             */
            std::string sign(const std::string& message) const override
            {

                // Initialize the private key using the Integer private exponent
                CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
                auto privKeyRaw = Crypto::base64Decode(getPrivateKey());
                CryptoPP::StringSource stringSource(privKeyRaw, true);
                privateKey.Load(stringSource);

                // Use the private key to sign the message
                CryptoPP::AutoSeededRandomPool prng;
                CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer signer(privateKey);
                size_t siglen = signer.MaxSignatureLength();
                std::string p1363Signature(siglen, 0x00);
                siglen = signer.SignMessage(prng, (const CryptoPP::byte*) &message[0],
                        message.size(), (CryptoPP::byte*) &p1363Signature[0]);
                p1363Signature.resize(siglen);

                // Convert the P1363 signature to ASN.1/DER encoding
                std::string derSignature;
                derSignature.resize(3 + 3 + 3 + 2 + p1363Signature.size());
                size_t signatureSize = CryptoPP::DSAConvertSignatureFormat(
                        (CryptoPP::byte*) (&derSignature[0]), derSignature.size(), CryptoPP::DSA_DER,
                        (const CryptoPP::byte*) (p1363Signature.data()), p1363Signature.size(), CryptoPP::DSA_P1363);
                derSignature.resize(signatureSize);

                // Return the generated signature (in base64 format)
                return Crypto::base64Encode(derSignature);
            }

            /**
             * Overridden function used to verify the message signature
             *
             * @param message String representing the message to use
             * @param signature String representing the signature to verify
             * @return Boolean indicating if the signature is valid or not
             */
            bool isValid(const std::string& message, const std::string& signature) const override
            {

                // Start by hashing the message using the SHA256 hash scheme
                // and decoding the base64 signature
                auto signatureDecoded = Crypto::base64Decode(signature);

                // Initialize the private key using the public point
                CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;
                auto pubKeyRaw = Crypto::base64Decode(getPublicKey());
                CryptoPP::StringSource stringSource(pubKeyRaw, true);
                publicKey.Load(stringSource);

                // Convert the ASN.1/DER signature to P1363 encoding
                std::string signatureConverted;
                CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier verifier(publicKey);
                signatureConverted.resize(verifier.SignatureLength());
                size_t signatureSize = CryptoPP::DSAConvertSignatureFormat(
                        (CryptoPP::byte*) (&signatureConverted[0]), signatureConverted.size(), CryptoPP::DSA_P1363,
                        (const CryptoPP::byte*) (signatureDecoded.data()), signatureDecoded.size(), CryptoPP::DSA_DER);
                signatureConverted.resize(signatureSize);

                // Use the public key to verify the message signature
                // and return the results accordingly
                return verifier.VerifyMessage((const CryptoPP::byte*) &message[0], message.size(),
                        (CryptoPP::byte*) (&signatureConverted[0]), signatureConverted.size());
            }

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~EcdsaKeyPair() = default;
    };
}

#endif //BITBOSON_STANDARDMODEL_ECSDAKEYPAIR

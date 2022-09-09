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

#ifndef BITBOSON_STANDARDMODEL_AESENCRYPTIONKEY
#define BITBOSON_STANDARDMODEL_AESENCRYPTIONKEY

#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/modes.h>
#include <cryptopp/rijndael.h>
#include <cryptopp/cryptlib.h>
#include <BitBoson/StandardModel/Utils/Utils.h>
#include <BitBoson/StandardModel/Crypto/Crypto.h>
#include <BitBoson/StandardModel/Crypto/Encryption/EncryptionKey.hpp>

namespace BitBoson::StandardModel
{

    class AesEncryptionKey : public EncryptionKey
    {

        // Public member functions
        public:

            /**
             * Constructor used to setup the instance
             */
            AesEncryptionKey() = default;

            /**
             * Overridden function used to generate a new encryption key
             */
            void generateNewKey()
            {

                // Generate the secure-byte-blocks for AES
                CryptoPP::AutoSeededRandomPool prng;
                CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
                prng.GenerateBlock(key, key.size());

                // Save the private key the internal variable
                std::string keyRaw;
                CryptoPP::HexEncoder keyEncoder(new CryptoPP::StringSink(keyRaw));
                keyEncoder.Put(key, key.size());
                keyEncoder.MessageEnd();
                setEncryptionKey(keyRaw);
            }

            /**
             * Overridden function used to get the encryption key type
             *
             * @return KeyTypes representing the encryption key type
             */
            KeyTypes getKeyType() const
            {

                // Simply return the hard-coded key type
                return EncryptionKey::KeyTypes::AES;
            }

            /**
             * Overridden function used to encrypt the provided clear-text message using the encryption key
             *
             * @param plainText String representing the plain-text message to encrypt
             * @return String representing the base64 cipher-text representation of the plain-text message
             */
            std::string encrypt(const std::string& plainText)
            {

                // Encode the value to base64 and pad it with a SHA256 hash
                std::string plainTextEncoded = Crypto::base64Encode(plainText);
                plainTextEncoded = Crypto::getRandomSha256(true) + plainTextEncoded;

                // Re-create the secure byte-block key for the encryptor to use
                std::string keyRawDecoded;
                std::string keyRaw = getEncryptionKey();
                CryptoPP::HexDecoder keyDecoder;
                keyDecoder.Put((CryptoPP::byte*) keyRaw.data(), keyRaw.size());
                keyDecoder.MessageEnd();
                keyRawDecoded.resize(keyDecoder.MaxRetrievable());
                keyDecoder.Get((CryptoPP::byte*) &keyRawDecoded[0], keyRawDecoded.size());
                CryptoPP::SecByteBlock key((CryptoPP::byte*) (&keyRawDecoded[0]), keyRawDecoded.size());

                // Create the Initialization vector for the operation
                CryptoPP::AutoSeededRandomPool prng;
                CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
                prng.GenerateBlock(iv, iv.size());

                // Setup the encryptor with the internal key and random IV
                CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
                CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

                // Encrypt the provided plain-text and save it into the cipher-text
                std::string cipherText;
                CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(cipherText));
                stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plainTextEncoded.c_str()), plainTextEncoded.length());
                stfEncryptor.MessageEnd();

                // Return the cipher-text representation
                return Crypto::base64Encode(cipherText);
            }

            /**
             * Overridden function used to decrypt the provided cipher-text message using the encryption key
             *
             * @param plainText String representing the base64 cipher-text message to decrypt
             * @return String representing the plain-text representation of the cipher-text message
             */
            std::string decrypt(const std::string& cipherText)
            {

                // Re-create the secure byte-block key for the decryptor to use
                std::string keyRawDecoded;
                std::string keyRaw = getEncryptionKey();
                CryptoPP::HexDecoder keyDecoder;
                keyDecoder.Put((CryptoPP::byte*) keyRaw.data(), keyRaw.size());
                keyDecoder.MessageEnd();
                keyRawDecoded.resize(keyDecoder.MaxRetrievable());
                keyDecoder.Get((CryptoPP::byte*) &keyRawDecoded[0], keyRawDecoded.size());
                CryptoPP::SecByteBlock key((CryptoPP::byte*) (&keyRawDecoded[0]), keyRawDecoded.size());

                // Create the Initialization vector for the operation
                CryptoPP::AutoSeededRandomPool prng;
                CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
                prng.GenerateBlock(iv, iv.size());

                // Catch any exceptions and return an empty string
                std::string plainTextEncoded;
                try
                {

                    // Setup the decryptor with the internal key and random IV
                    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
                    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

                    // Decrypt the provided cipher-text and save it into the plain-text
                    std::string cipherTextDecoded = Crypto::base64Decode(cipherText);
                    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(plainTextEncoded));
                    stfDecryptor.Put(reinterpret_cast<const unsigned char*>(cipherTextDecoded.c_str()), cipherTextDecoded.size());
                    stfDecryptor.MessageEnd();
                } catch (...) {
                    return "";
                }

                // Trim the padded plain-text and decode the results
                plainTextEncoded = plainTextEncoded.substr(64);
                std::string plainText = Crypto::base64Decode(plainTextEncoded);
                return plainText;
            }

            /**
             * Destructor used to cleanup the instance
             */
            virtual ~AesEncryptionKey() = default;
    };
}

#endif //BITBOSON_STANDARDMODEL_AESENCRYPTIONKEY

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

#ifndef BITBOSON_STANDARDMODEL_CRYPTO_TEST_HPP
#define BITBOSON_STANDARDMODEL_CRYPTO_TEST_HPP

#include <BitBoson/StandardModel/Crypto/Crypto.h>
#include <BitBoson/StandardModel/Crypto/DigitalSignatures/EcdsaKeyPair.hpp>
#include <BitBoson/StandardModel/Crypto/DigitalSignatures/DigitalSignatureKeyPair.hpp>

using namespace BitBoson::StandardModel;

TEST_CASE ("BCrypt Hash Test", "[CryptoTest]")
{

    // Test some SHA256 combinations and verify they check-out
    REQUIRE (Crypto::argon2d("hello") == "wEeoH62Xr65VY8RDauBSsHPP8qJOhAEHuAOgZrYQ1Lo=");
    REQUIRE (Crypto::argon2d("world") == "vz_Z_3K2zg-upWYioX-fp4_pZ8VhZRkXUCo6HbLIl0M=");
    REQUIRE (Crypto::argon2d("") == "1Enx8i-HOWtH6UAN6I3yCoGjxxHeq7UUL6Z3Gi6nQDU=");
    REQUIRE (Crypto::argon2d("1234567890") == "pAOl97IKntdyDBNmp8Ca4PFxmLhlDwidYuuf4S2aZsw=");
}

TEST_CASE ("SHA256 Test", "[CryptoTest]")
{

    // Test some SHA256 combinations and verify they check-out
    REQUIRE (Crypto::sha256("hello") == "2CF24DBA5FB0A30E26E83B2AC5B9E29E1B161E5C1FA7425E73043362938B9824");
    REQUIRE (Crypto::sha256("world") == "486EA46224D1BB4FB680F34F7C9AD96A8F24EC88BE73EA8E5A6C65260E9CB8A7");
    REQUIRE (Crypto::sha256("") == "E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855");
    REQUIRE (Crypto::sha256("1234567890") == "C775E7B757EDE630CD0AA1113BD102661AB38829CA52A6422AB782862F268646");
}

TEST_CASE ("Random SHA256 Test", "[CryptoTest]")
{

    // Generate random UUIDs and verify they are different
    REQUIRE (Crypto::getRandomSha256() != Crypto::getRandomSha256());
    REQUIRE (Crypto::getRandomSha256(false) != Crypto::getRandomSha256(false));
    REQUIRE (Crypto::getRandomSha256(true) != Crypto::getRandomSha256(true));
    REQUIRE (Crypto::getRandomSha256() != Crypto::getRandomSha256(true));
    REQUIRE (Crypto::getRandomSha256() != Crypto::getRandomSha256(false));
    REQUIRE (Crypto::getRandomSha256(false) != Crypto::getRandomSha256(true));
}

TEST_CASE ("Get Number of Leading Zeros in SHA256 Hash", "[CryptoTest]")
{

    // Test some SHA256 combinations and verify they check-out
    REQUIRE (Crypto::getNumberOfLeadingZerosInHash("2CF24DBA5FB0A30E26E83B2AC5B9E29E1B161E5C1FA7425E73043362938B9824") == 0);
    REQUIRE (Crypto::getNumberOfLeadingZerosInHash("0CF24DBA5FB0A30E26E83B2AC5B9E29E1B161E5C1FA7425E73043362938B9824") == 1);
    REQUIRE (Crypto::getNumberOfLeadingZerosInHash("00F24DBA5FB0A30E26E83B2AC5B9E29E1B161E5C1FA7425E73043362938B9824") == 2);
    REQUIRE (Crypto::getNumberOfLeadingZerosInHash("00024DBA5FB0A30E26E83B2AC5B9E29E1B161E5C1FA7425E73043362938B9824") == 3);
    REQUIRE (Crypto::getNumberOfLeadingZerosInHash("00004DBA5FB0A30E26E83B2AC5B9E29E1B161E5C1FA7425E73043362938B9824") == 4);
    REQUIRE (Crypto::getNumberOfLeadingZerosInHash("A000000000000000000000000000000000000000000000000000000000000000") == 0);
    REQUIRE (Crypto::getNumberOfLeadingZerosInHash("0000000000000000000000000000000000000000000000000000000000000000") == 64);
}

TEST_CASE ("Test Winternitz Signatures", "[CryptoTest]")
{

    // Generate two key-value pairs to use for testing
    auto kvPair1 = Crypto::getKeyPair(DigitalSignatureKeyPair::KeyTypes::WINTERNITZ);
    auto kvPair2 = Crypto::getKeyPair(DigitalSignatureKeyPair::KeyTypes::WINTERNITZ);

    // Verify that the first key-value pair works properly
    auto signature1 = kvPair1->sign("Hello World!");
    auto signature2 = kvPair1->sign("Oh what a Beautiful Morning!");
    REQUIRE (signature1.size() == 2048);
    REQUIRE (signature2.size() == 2048);
    REQUIRE (signature1 != signature2);
    REQUIRE (kvPair1->isValid("Hello World!", signature1));
    REQUIRE (!kvPair1->isValid("Hello World!", signature2));
    REQUIRE (kvPair1->isValid("Oh what a Beautiful Morning!", signature2));
    REQUIRE (!kvPair1->isValid("Oh what a Beautiful Morning!", signature1));

    // Verify that a signatures with different keys are different
    auto signature3 = kvPair2->sign("Hello World!");
    REQUIRE (signature1 != signature3);
    REQUIRE (kvPair1->isValid("Hello World!", signature1));
    REQUIRE (kvPair2->isValid("Hello World!", signature3));
    REQUIRE (!kvPair1->isValid("Hello World!", signature3));
    REQUIRE (!kvPair2->isValid("Hello World!", signature1));
}

TEST_CASE ("Test ECDSA Signatures", "[CryptoTest]")
{

    // Generate two key-value pairs to use for testing
    auto kvPair1 = Crypto::getKeyPair(DigitalSignatureKeyPair::KeyTypes::ECDSA);
    auto kvPair2 = Crypto::getKeyPair(DigitalSignatureKeyPair::KeyTypes::ECDSA);

    // Verify that the first key-value pair works properly
    auto signature1 = kvPair1->sign("Hello World!");
    auto signature2 = kvPair1->sign("Oh what a Beautiful Morning!");
    REQUIRE (signature1.size() == 96);
    REQUIRE (signature2.size() == 96);
    REQUIRE (signature1 != signature2);
    REQUIRE (kvPair1->isValid("Hello World!", signature1));
    REQUIRE (!kvPair1->isValid("Hello World!", signature2));
    REQUIRE (kvPair1->isValid("Oh what a Beautiful Morning!", signature2));
    REQUIRE (!kvPair1->isValid("Oh what a Beautiful Morning!", signature1));

    // Verify that a signatures with different keys are different
    auto signature3 = kvPair2->sign("Hello World!");
    REQUIRE (signature1 != signature3);
    REQUIRE (kvPair1->isValid("Hello World!", signature1));
    REQUIRE (kvPair2->isValid("Hello World!", signature3));
    REQUIRE (!kvPair1->isValid("Hello World!", signature3));
    REQUIRE (!kvPair2->isValid("Hello World!", signature1));

    // Ensure we can verify an AWS signed message using the AWS public key
    std::string awsMessage = "Hello";
    std::string awsSignature = "MEUCIQDDUK3dJZRHW8ILRTyN6qRVGUIxfnbK2CippG8kIEusUAIgGa9kLezENZSjAkdIQ9N5BFIDkKlbaO5qCBh03EY5nrE=";
    std::string awsPublicKey = "MFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEdTLxUdH2C6dlDyupHsL6IteufHiLvGMalqDt4ExVin7qUPiRvgkxaWEYsOFDv1vdZh4uS6PKPYRU2TbYyXalXA==";
    auto awsPublicKeyObj = std::make_shared<EcdsaKeyPair>();
    awsPublicKeyObj->setPublicKey(awsPublicKey);
    REQUIRE (awsPublicKeyObj->isValid(awsMessage, awsSignature));
}

TEST_CASE ("Test Base 64 Encoding/Decoding", "[CryptoTest]")
{

    // Encode and decode some strings
    auto testString1 = "Hello World";
    auto testString2 = "Hello~World";
    auto testString3 = "Hello\x7fWorld";
    auto testString4 = "";
    auto testString5 = "1234567890123456";
    auto testString6 = "12345678901234567";
    auto testString7 = "123456789012345678";
    auto testString8 = "1234567890123456789";
    auto testString9 = "12345678901234567890";

    // Perform all assertions on the encoded/decoded strings
    REQUIRE (Crypto::base64Encode(testString1, false) == "SGVsbG8gV29ybGQ=");
    REQUIRE (Crypto::base64Encode(testString1, true) == "SGVsbG8gV29ybGQ=");
    REQUIRE (Crypto::base64Encode(testString2, false) == "SGVsbG9+V29ybGQ=");
    REQUIRE (Crypto::base64Encode(testString2, true) == "SGVsbG9-V29ybGQ=");
    REQUIRE (Crypto::base64Encode(testString3, false) == "SGVsbG9/V29ybGQ=");
    REQUIRE (Crypto::base64Encode(testString3, true) == "SGVsbG9_V29ybGQ=");
    REQUIRE (Crypto::base64Encode(testString4, false).empty());
    REQUIRE (Crypto::base64Encode(testString4, true).empty());
    REQUIRE (Crypto::base64Encode(testString5, false) == "MTIzNDU2Nzg5MDEyMzQ1Ng==");
    REQUIRE (Crypto::base64Encode(testString5, true) == "MTIzNDU2Nzg5MDEyMzQ1Ng==");
    REQUIRE (Crypto::base64Encode(testString6, false) == "MTIzNDU2Nzg5MDEyMzQ1Njc=");
    REQUIRE (Crypto::base64Encode(testString6, true) == "MTIzNDU2Nzg5MDEyMzQ1Njc=");
    REQUIRE (Crypto::base64Encode(testString7, false) == "MTIzNDU2Nzg5MDEyMzQ1Njc4");
    REQUIRE (Crypto::base64Encode(testString7, true) == "MTIzNDU2Nzg5MDEyMzQ1Njc4");
    REQUIRE (Crypto::base64Encode(testString8, false) == "MTIzNDU2Nzg5MDEyMzQ1Njc4OQ==");
    REQUIRE (Crypto::base64Encode(testString8, true) == "MTIzNDU2Nzg5MDEyMzQ1Njc4OQ==");
    REQUIRE (Crypto::base64Encode(testString9, false) == "MTIzNDU2Nzg5MDEyMzQ1Njc4OTA=");
    REQUIRE (Crypto::base64Encode(testString9, true) == "MTIzNDU2Nzg5MDEyMzQ1Njc4OTA=");
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString1, false)) == testString1);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString1, true)) == testString1);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString2, false)) == testString2);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString2, true)) == testString2);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString3, false)) == testString3);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString3, true)) == testString3);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString4, false)) == testString4);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString4, true)) == testString4);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString5, false)) == testString5);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString5, true)) == testString5);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString6, false)) == testString6);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString6, true)) == testString6);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString7, false)) == testString7);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString7, true)) == testString7);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString8, false)) == testString8);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString8, true)) == testString8);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString9, false)) == testString9);
    REQUIRE (Crypto::base64Decode(Crypto::base64Encode(testString9, true)) == testString9);
}

TEST_CASE ("Get BigInt from Hexideciamal Hash", "[CryptoTest]")
{

    // Verify that the converted hash (hexidecimal) values are correct
    REQUIRE (Crypto::getBigIntFromHash("0000000") == 0);
    REQUIRE (Crypto::getBigIntFromHash("F") == 15);
    REQUIRE (Crypto::getBigIntFromHash("FF") == 255);
    REQUIRE (Crypto::getBigIntFromHash("486EA46224D1B") == BigInt{"1274240663964955"});
    REQUIRE (Crypto::getBigIntFromHash("E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855") == BigInt{"102987336249554097029535212322581322789799900648198034993379397001115665086549"});
}

TEST_CASE ("Get Proof-of-Work Hash", "[CryptoTest]")
{

    // Validate that the PoW hash abd fudge-value checks-out
    std::string fudgeString;
    auto powHash = Crypto::getPowHash(0, "BLAH", fudgeString);
    REQUIRE (Crypto::getNumberOfLeadingZerosInHash(powHash) >= 0);
    REQUIRE (Crypto::sha256(Crypto::argon2d("BLAH" + fudgeString)) == powHash);

    // Validate that the PoW hash has at least 1 zero and
    // that the fudge-value checks-out
    powHash = Crypto::getPowHash(1, "BLAH", fudgeString);
    REQUIRE (Crypto::getNumberOfLeadingZerosInHash(powHash) >= 1);
    REQUIRE (Crypto::sha256(Crypto::argon2d("BLAH" + fudgeString)) == powHash);

    // Validate that the PoW hash has at least 2 zeros and
    // that the fudge-value checks-out
    powHash = Crypto::getPowHash(2, "BLAH", fudgeString);
    REQUIRE (Crypto::getNumberOfLeadingZerosInHash(powHash) >= 2);
    REQUIRE (Crypto::sha256(Crypto::argon2d("BLAH" + fudgeString)) == powHash);
}

#endif //BITBOSON_STANDARDMODEL_CRYPTO_TEST_HPP

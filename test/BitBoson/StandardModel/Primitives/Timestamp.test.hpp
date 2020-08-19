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

#ifndef BITBOSON_STANDARDMODEL_TIMESTAMP_TEST_HPP
#define BITBOSON_STANDARDMODEL_TIMESTAMP_TEST_HPP

#include <boost/thread/thread_only.hpp>
#include <BitBoson/StandardModel/Primitives/Timestamp.h>

using namespace BitBoson::StandardModel;

TEST_CASE ("Timestamp Initialization Test", "[TimestampTest]")
{

    // Create a few different timestamps with different values
    auto ts1 = Timestamp("-10000");
    auto ts2 = Timestamp("-1");
    auto ts3 = Timestamp("0");
    auto ts4 = Timestamp("1");
    auto ts5 = Timestamp("10000");

    // Verify the BigInt values are as expected
    REQUIRE (ts1.getCurrentValue() == 0);
    REQUIRE (ts2.getCurrentValue() == 0);
    REQUIRE (ts3.getCurrentValue() == 0);
    REQUIRE (ts4.getCurrentValue() == 1);
    REQUIRE (ts5.getCurrentValue() == 10000);

    // Verify the string values are as expected
    REQUIRE (ts1.toString() == "0");
    REQUIRE (ts2.toString() == "0");
    REQUIRE (ts3.toString() == "0");
    REQUIRE (ts4.toString() == "1");
    REQUIRE (ts5.toString() == "10000");
}

TEST_CASE ("Sequential Timestamp Creation Test", "[TimestampTest]")
{

    // Create three in "sequence" timestamps to verify they work
    auto ts1 = Timestamp::getCurrentTimestamp();
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    auto ts2 = Timestamp::getCurrentTimestamp();
    boost::this_thread::sleep(boost::posix_time::seconds(5));
    auto ts3 = Timestamp::getCurrentTimestamp();

    // Verify that their values are in fact sequential
    REQUIRE (ts1 <= ts2);
    REQUIRE (ts1 < ts3);
    REQUIRE (ts2 < ts3);

    // Compare their value differences to ensure they are as expected
    REQUIRE ((ts2.getCurrentValue() - ts1.getCurrentValue()) >= 0);
    REQUIRE ((ts2.getCurrentValue() - ts1.getCurrentValue()) <= 2000);
    REQUIRE ((ts3.getCurrentValue() - ts2.getCurrentValue()) >= 4000);
    REQUIRE ((ts3.getCurrentValue() - ts2.getCurrentValue()) <= 6000);
    REQUIRE ((ts3.getCurrentValue() - ts1.getCurrentValue()) >= 5000);
    REQUIRE ((ts3.getCurrentValue() - ts1.getCurrentValue()) <= 7000);
}

TEST_CASE ("Timestamp Comparisons Test", "[TimestampTest]")
{

    // Create a few different timestamps with different values
    auto ts1 = Timestamp("128");
    auto ts2 = Timestamp("256");
    auto ts3 = Timestamp("256");
    auto ts4 = Timestamp("1024");

    // Perform various comparisons to ensure they check-out
    REQUIRE (ts1 < ts2);
    REQUIRE (ts1 <= ts2);
    REQUIRE (ts2 <= ts3);
    REQUIRE (ts2 == ts3);
    REQUIRE (ts2 >= ts3);
    REQUIRE (ts4 > ts3);
    REQUIRE (ts3 < ts4);
    REQUIRE (ts1 != ts2);
    REQUIRE (ts3 != ts4);
}

#endif //BITBOSON_STANDARDMODEL_TIMESTAMP_TEST_HPP

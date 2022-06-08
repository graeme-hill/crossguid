#include <crossguid/guid.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <unordered_map>

TEST(RandomGuidTest, randomGuidGeneratesCorrectly)
{
    auto r1 = xg::newGuid();
	auto r2 = xg::newGuid();
	auto r3 = xg::newGuid();

    EXPECT_NE(r1, r2);
    EXPECT_NE(r1, r3);
    EXPECT_NE(r2, r3);
}

TEST(ProvidedStringTest, matchingStringsShouldGenerateSameGuid)
{
    xg::Guid a1("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
    xg::Guid a2("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");

    EXPECT_EQ(a1, a2);
}

TEST(ProvidedStringTest, differentStringsShouldGenerateDifferentGuids)
{
    xg::Guid a1("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
    xg::Guid a2("16d1bd03-09a5-47d3-944b-5e326fd52d27");

    EXPECT_NE(a1, a2);
}

TEST(ProvidedStringTest, higherGuidShouldBeGreaterThanLowerGuid)
{
    xg::Guid a1("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
    xg::Guid a2("7bcd757f-5b10-4f9b-af69-1a1f226f3b31");

    bool result = a1 < a2;

    EXPECT_FALSE(result);
}

TEST(GuidToStringTest, GuidToStringShouldMatch)
{
    xg::Guid a1("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
    std::string expected = "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e";

    std::string actual = a1.str();

    EXPECT_EQ(expected, actual);
}

TEST(GuidToStringTest, GuidToStringStreamShouldMatch)
{
    xg::Guid a1("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
    std::string expected = "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e";

    std::stringstream actual;
	actual << a1;

    EXPECT_EQ(expected, actual.str());
}

TEST(SwapGuidTest, SwapGuidsShouldHaveGoodInitialState)
{
    auto swap1 = xg::newGuid();
	auto swap2 = xg::newGuid();
	auto swap3 = swap1;
	auto swap4 = swap2;

    EXPECT_EQ(swap1, swap3);
    EXPECT_EQ(swap2, swap4);
    EXPECT_NE(swap1, swap2);
}

TEST(SwapGuidTest, GuidsShouldSwapWhenInvoked)
{
    auto swap1 = xg::newGuid();
	auto swap2 = xg::newGuid();
	auto swap3 = swap1;
	auto swap4 = swap2;
    swap1.swap(swap2);

    EXPECT_EQ(swap1, swap4);
    EXPECT_EQ(swap2, swap3);
    EXPECT_NE(swap1, swap2);
}

TEST(GuidMapTest, GuidCanBeUsedAsKey)
{
    xg::Guid s1("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
	xg::Guid s2("16d1bd03-09a5-47d3-944b-5e326fd52d27");

    std::unordered_map<xg::Guid, int> m = {{s1, 1}, {s2, 2}};
	auto it1 = m.find(s1);
	auto it2 = m.find(s2);
	
    EXPECT_TRUE(it1 != m.end());
    EXPECT_EQ(it1->first, s1);
    EXPECT_EQ(it1->second, 1);

    EXPECT_TRUE(it2 != m.end());
    EXPECT_EQ(it2->first, s2);
    EXPECT_EQ(it2->second, 2);
}

TEST(ByteGuidTest, GuidFromBytesShouldMatchGuidFromString)
{
    std::array<unsigned char, 16> bytes =
	{{
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xdd
	}};
	xg::Guid guidFromBytes(bytes);
	xg::Guid guidFromString("0102030405060708090a0b0c0d0e0fdd");

    EXPECT_EQ(guidFromBytes, guidFromString);
}

TEST(ByteGuidTest, ArrayFromByteGuidShouldMatch)
{
    std::array<unsigned char, 16> bytes =
	{{
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xdd
	}};
	xg::Guid guidFromBytes(bytes);

    EXPECT_TRUE(std::equal(guidFromBytes.bytes().begin(), guidFromBytes.bytes().end(), bytes.begin()));
}

TEST(ErrorHandlingTest, GuidWithOneMissingValueShouldBeEmpty)
{
    xg::Guid empty;
    xg::Guid oneTooFew("16d1bd03-09a5-47d3-944b-5e326fd52d2");
    EXPECT_EQ(oneTooFew, empty);
    EXPECT_FALSE(oneTooFew.isValid())
}

TEST(ErrorHandlingTest, GuidWithTwoMissingValuesShouldBeEmpty)
{
    xg::Guid empty;
	xg::Guid twoTooFew("7bcd757f-5b10-4f9b-af69-1a1f226f3b");
    EXPECT_EQ(twoTooFew, empty);
    EXPECT_FALSE(twoTooFew.isValid())
}

TEST(ErrorHandlingTest, GuidWithOneExtraValueShouldBeEmpty)
{
    xg::Guid empty;
    xg::Guid oneExtraValue("16d1bd03-09a5-47d3-944b-5e326fd52d27a");
    EXPECT_EQ(oneExtraValue, empty);
    EXPECT_FALSE(oneExtraValue.isValid())
}

TEST(ErrorHandlingTest, GuidWithTwoExtraValuesShouldBeEmpty)
{
    xg::Guid empty;
	xg::Guid twoExtraValues("7bcd757f-5b10-4f9b-af69-1a1f226f3beeff");
    EXPECT_EQ(twoExtraValues, empty);
    EXPECT_FALSE(twoExtraValues.isValid())
}

TEST(ErrorHandlingTest, GuidWithBadStringShouldBeEmpty)
{
    xg::Guid empty;
	xg::Guid badString("!!bad-guid-string!!");
    EXPECT_EQ(badString, empty);
    EXPECT_FALSE(badString.isValid())
}


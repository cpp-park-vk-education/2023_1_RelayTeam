#include <gtest/gtest.h>

#include "networkTools.h"
#include "variables.h"

TEST(getLocalIPv4Test, HandlesZeroInput) {
	EXPECT_EQ(getLocalIPv4().protocol() == QAbstractSocket::IPv4Protocol, 1);
}

TEST(getLocalIPv6Test, HandlesZeroInput) {
	EXPECT_EQ(getLocalIPv6().protocol() == QAbstractSocket::IPv6Protocol, 1);
}

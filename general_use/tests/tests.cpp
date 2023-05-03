#include <gtest/gtest.h>

#include "networkTools.h"
#include "variables.h"

TEST(getLocalIPv4Test, HandlesZeroInput) {
	EXPECT_TRUE(getLocalIPv4().protocol() == QAbstractSocket::IPv4Protocol);
	EXPECT_FALSE(getLocalIPv4().isLoopback());
}

TEST(getLocalIPv6Test, HandlesZeroInput) {
	EXPECT_TRUE(getLocalIPv6().protocol() == QAbstractSocket::IPv6Protocol || getLocalIPv6().isNull());
	EXPECT_FALSE(getLocalIPv6().isLoopback());
}

TEST(resourceDirCheck, HandlesZeroInput) {
	EXPECT_TRUE(Q_RESOURCE_DIR.exists());
}

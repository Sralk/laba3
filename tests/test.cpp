// Copyright 2018 Your Name <your_email>

#include <gtest/gtest.h>
#include <header.hpp>

TEST(Account, SimpleTest1) {
	Account acc = Account { "00001312" };
	EXPECT_EQ(acc.count, 1);
}

class BrokerImplTest: public ::testing::Test
{
protected:
    BrokerImpl resolver;
    AccountsMap acc_map;

    void SetUp() override
    {
        resolver.resolve("../tests/data");
        acc_map = resolver.get_result();
    }
};

TEST_F(BrokerImplTest, FindOne) {
	auto it = acc_map.find("new_dir");
	EXPECT_NE(it, acc_map.end());
}

TEST_F(BrokerImplTest, FindTwo) {
	auto it = acc_map.find("new_dir/nested_dir");
	EXPECT_NE(it, acc_map.end());
}

TEST_F(BrokerImplTest, AccountCheckFound) {
	bool is_found = false;
	for (auto iter : acc_map)
	{
		for (auto item : iter.second)
			if (item.first == "00001234")
			{
				is_found = true;
				break;
			}
		if (is_found) break;
	}

	EXPECT_EQ(is_found, true);
}

TEST_F(BrokerImplTest, AccountCheckNotFound) {
	bool is_found = false;
	for (auto iter : acc_map)
	{
		for (auto item : iter.second)
			if (item.first == "00009999")
			{
				is_found = true;
				break;
			}
		if (is_found) break;
	}

	EXPECT_EQ(is_found, false);
}

TEST_F(BrokerImplTest, CheckFullAccount) {
	auto it = acc_map.find("dir");
	EXPECT_NE(it, acc_map.end());

	auto acc = acc_map["dir"].find("00001234");
	EXPECT_NE(acc, acc_map["dir"].end());

	EXPECT_EQ(acc_map["dir"]["00001234"].count, 3);

	EXPECT_EQ(acc_map["dir"]["00001234"].date, "20181010");
}

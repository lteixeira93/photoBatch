#include <gtest/gtest.h>
#include <array>
#include "../ArgumentParser.h"

#define TEST_FLAG   "MyFlag"
#define TEST_OPTION "MyOption"

TEST(TestArgumentParser, RegisterFlag)
{
	/* Create object */
	ArgumentParser argParser;

	/* Verify state before operation */
	EXPECT_FALSE(argParser.IsFlagRegistered(TEST_FLAG));

	/* Verify state after operation */
	argParser.RegisterFlag(TEST_FLAG);
	EXPECT_TRUE(argParser.IsFlagRegistered(TEST_FLAG));	
}

TEST(TestArgumentParser, RegisterFlag_Whitespace)
{
	/* Create object */
	ArgumentParser argParser;

	argParser.RegisterFlag("My Flag");
	EXPECT_FALSE(argParser.IsFlagRegistered("My Flag"));
}

TEST(TestArgumentParser, RegisterOption)
{
	/* Create object */
	ArgumentParser argParser;

	argParser.RegisterOption(TEST_OPTION);
	EXPECT_TRUE(argParser.IsOptionRegistered(TEST_OPTION));
}

TEST(TestArgumentParser, RegisterOption_Whitespace)
{
	/* Create object */
	ArgumentParser argParser;

	argParser.RegisterOption("My Option");
	EXPECT_FALSE(argParser.IsOptionRegistered("My Option"));
}

TEST(TestArgumentParser, Parse) 
{
	/* Create object */
	ArgumentParser argParser;

	std::array<char*, 5> args =
	{
		"TestArgumentParser",
		"--flag",
		"--string=AaBbCcDd",
		"--number=42",
		"--float=4.2"
	};

	argParser.RegisterFlag("flag");
	argParser.RegisterOption("string");
	argParser.RegisterOption("number");
	argParser.RegisterOption("float");

	argParser.Parse(static_cast<int>(args.size()), args.data());

	EXPECT_FALSE(argParser.GetFlag("UnregisteredFlag"));
	EXPECT_TRUE(argParser.GetFlag("flag"));
	EXPECT_EQ(argParser.GetOptionAs<std::string>("string"), "aabbccdd");
	EXPECT_EQ(argParser.GetOptionAs<int>("number"), 42);
	EXPECT_EQ(argParser.GetOptionAs<float>("float"), 4.2f);
}

TEST(TestArgumentParser, Parse_InvalidArguments)
{
	/* Create object */
	ArgumentParser argParser;
	
	argParser.Parse(0, nullptr);
	argParser.Parse(42, nullptr);
	argParser.Parse(-1, nullptr);
}
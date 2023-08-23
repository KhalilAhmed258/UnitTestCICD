// Reverse.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
//#include <gtest/gtest.h>
#include "logger.h"
#include "utils.h"



/*
// Tests for uint32_t reverse(const uint32_t val)
TEST(ReverseUint32Test, Zero) {
   EXPECT_EQ(utils::reverse(0), 0);
}

TEST(ReverseUint32Test, SingleDigit) {
   EXPECT_EQ(utils::reverse(5), 5);
}

TEST(ReverseUint32Test, MultipleDigits) {
   EXPECT_EQ(utils::reverse(12345), 54321);
}

TEST(ReverseUint32Test, Overflow) {
   EXPECT_THROW(utils::reverse(UINT32_MAX), std::domain_error);
}

// Tests for std::string reverse(const std::string& src)
TEST(ReverseStringTest, NonEmptyString) {
   EXPECT_EQ(utils::reverse("hello"), "olleh");
   EXPECT_EQ(utils::reverse("world"), "dlrow");
   EXPECT_EQ(utils::reverse("testing"), "gnitset");
}

TEST(ReverseStringTest, EmptyString) {
   EXPECT_THROW(utils::reverse(""), std::invalid_argument);
}

TEST(ReverseStringTest, WhiteSpaceString) {
   EXPECT_THROW(utils::reverse("    "), std::invalid_argument);
}

int main(int argc, char** argv) {
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}

*/


int main(int argc, char** argv)
{
    std::cout << "Hello World!\n";

    uint32_t num1 = 12345;
    uint32_t reversed1 = utils::reverse(num1);
    std::cout << num1 << " reversed: " << reversed1 << std::endl;

    uint32_t num2 = 0;
    uint32_t reversed2 = utils::reverse(num2);
    std::cout << num2 << " reversed: " << reversed2 << std::endl;

    uint32_t num3 = 191;
    uint32_t reversed3 = utils::reverse(num3);
    std::cout << num3 << " reversed: " << reversed3 << std::endl;


    std::string str1 = "bob";
    std::string reversedStr1 = utils::reverse(str1);
    std::cout << str1 << " reversed: " << reversedStr1 << std::endl;

    std::string str2 = "book";
    std::string reversedStr2 = utils::reverse(str2);
    std::cout << str2 << " reversed: " << reversedStr2 << std::endl;

    std::string str3 = "hello world";
    std::string reversedStr3 = utils::reverse(str3);
    std::cout << str3 << " reversed: " << reversedStr3 << std::endl;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

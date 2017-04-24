//
// Created by pairs on 4/18/17.
//

#include <utils/singleton.h>
#include <gtest/gtest.h>

class test
{
public:
    test(){}
    test(int x, int y):x_(x), y_(y) {

    }

    int math()
    {
        return x_ + y_;
    }
private:
    int x_;
    int y_;
};

using namespace  utils;

TEST(single, single)
{
    auto a = utils::SingleTon<test>::Instance(1,2);
    EXPECT_EQ(3, a->math());

    auto b = utils::SingleTon<test>::Instance(3,4);
    EXPECT_EQ(3, b->math());

    auto c = utils::SingleTon<test>::Instance();
    EXPECT_EQ(3, c->math());
}

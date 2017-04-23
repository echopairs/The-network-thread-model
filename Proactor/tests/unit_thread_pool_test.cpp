#include <gtest/gtest.h>
#include <unit/thread_pool.h>
#include <proactor/event_handle.h>

class ThreadPoolTest : public testing::Test
{
protected:
    ThreadPoolTest():thread_pool_(4) {

    }
    virtual void SetUp() {

    }

    virtual void TearDown() {

    }

    utils::ThreadPool thread_pool_;
};

int c_function(int a, int b) {
    return a + b;
}

auto lambda = [](int a, int b)->int { return a + b; };

auto bind_f = std::bind(c_function, std::placeholders::_1, std::placeholders::_2);

class Base
{
public:
    Base() {
        member_function_ = [](int a, int b)->int {
            return a + b;
        };
    }

    std::function<int (int a, int b) > get_member_function() {
        return member_function_;
    }

    static int StaticMember(int a, int b) {
        return a+b;
    }

private:
    std::function<int(int a, int b) > member_function_;
};


TEST_F(ThreadPoolTest, Function)
{
    auto r = thread_pool_.enqueue(c_function, 1, 2);
    EXPECT_EQ(3, r.get());
}

TEST_F(ThreadPoolTest, Lambda)
{
    auto r = thread_pool_.enqueue(lambda, 1, 2);
    EXPECT_EQ(3, r.get());
}

TEST_F(ThreadPoolTest, Bind)
{
    auto r = thread_pool_.enqueue(bind_f, 1, 2);
    EXPECT_EQ(3, r.get());
}

TEST_F(ThreadPoolTest, ClassMember)
{
    Base cm;
    auto r = thread_pool_.enqueue(cm.get_member_function(), 1, 2);
    EXPECT_EQ(3, r.get());
}



TEST_F(ThreadPoolTest, ClassStaticMember)
{
    auto r = thread_pool_.enqueue(Base::StaticMember, 1, 2);
    EXPECT_EQ(3, r.get());
}
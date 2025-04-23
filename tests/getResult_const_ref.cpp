#include <scheduler.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>


TEST(getResultConstRef, Get_Ref) {
    TTaskScheduler scheduler;

    float b = 2;
    float c = 3;

    auto id1 = scheduler.add([](float b, float c) {return b + c;}, b, c);

    ASSERT_FLOAT_EQ( scheduler.getResult<float&>(id1), 5.0);
}


TEST(getResultConstRef, Get_Const) {
    TTaskScheduler scheduler;

    float b = 2;
    float c = 3;

    auto id1 = scheduler.add([](float b, float c) {return b + c;}, b, c);

    ASSERT_FLOAT_EQ( scheduler.getResult<const float>(id1), 5.0);
}


TEST(getResultConstRef, Get_Const_Ref) {
    TTaskScheduler scheduler;

    float b = 2;
    float c = 3;

    auto id1 = scheduler.add([](float b, float c) {return b + c;}, b, c);

    ASSERT_FLOAT_EQ( scheduler.getResult<const float&>(id1), 5.0);
}
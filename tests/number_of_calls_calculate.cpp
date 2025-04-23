#include <scheduler.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>





TEST(NumberOfCallsCalculate, Correct_Number_Of_Calls_1) {
    TTaskScheduler scheduler;

    float a = 1;
    float c = 3;


    int call_count = 0;

    auto func = [&call_count](float a, float c) {
        ++call_count;
        return a + c;
    };


    auto id1 = scheduler.add(func, a, c);
    auto id2 = scheduler.add(func, a, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add(func, a, scheduler.getFutureResult<float>(id2));


    scheduler.executeAll();
    float result1 = scheduler.getResult<float>(id3);
    float result2 = scheduler.getResult<float>(id2);
    float result3 = scheduler.getResult<float>(id1);


    EXPECT_EQ(call_count, 3);
    EXPECT_FLOAT_EQ(result1, 6.0);
    EXPECT_FLOAT_EQ(result2, 5.0);
    EXPECT_FLOAT_EQ(result3, 4.0);
}



TEST(NumberOfCallsCalculate, Correct_Number_Of_Calls_2) {
    TTaskScheduler scheduler;

    float a = 1;
    float c = 3;


    int call_count = 0;

    auto func = [&call_count](float a, float c) {
        ++call_count;
        return a + c;
    };


    auto id1 = scheduler.add(func, a, c);
    auto id2 = scheduler.add(func, a, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add(func, a, scheduler.getFutureResult<float>(id2));
    auto id4 = scheduler.add(func, scheduler.getFutureResult<float>(id3), scheduler.getFutureResult<float>(id2));
    auto id5 = scheduler.add(func, a, scheduler.getFutureResult<float>(id2));
    auto id6 = scheduler.add(func, scheduler.getFutureResult<float>(id5), scheduler.getFutureResult<float>(id4));


    scheduler.executeAll();
    float result = scheduler.getResult<float>(id6);

    EXPECT_EQ(call_count, 6);
    EXPECT_FLOAT_EQ(result, 17.0);

}
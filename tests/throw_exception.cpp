#include "my_exceptions.h"
#include <cstddef>
#include <scheduler.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

struct Foo {
    static void Print(float x) {
        std::cout << x << std::endl;
    }

};


TEST(ThrowException, Exception_In_GetResult_WrongCast) {
    TTaskScheduler scheduler;

    float a = 1;
    float c = 3;

    auto id1 = scheduler.add([](float a, float c) {return a + c;}, a, c);

    EXPECT_THROW({scheduler.getResult<int>(id1);}, my_exceptions::WrongCast); 
}


TEST(ThrowException, Exception_In_GetFututreResult_WrongId_1) {
    TTaskScheduler scheduler;

    float a = 1;
    float c = 3;

    size_t wrong_id = 0;

    EXPECT_THROW(scheduler.add([](float a, float c) {return a + c;}, scheduler.getFutureResult<float>(wrong_id), c),
     my_exceptions::WrongIdException); 
}


TEST(ThrowException, Exception_In_GetFututreResult_WrongId_2) {
    TTaskScheduler scheduler;

    float a = 1;
    float c = 3;

    size_t wrong_id = 3;

    auto id1 = scheduler.add([](float a, float c) {return a + c;}, a, c);
    auto id2 = scheduler.add([](float a, float c) {return a + c;}, a, c);
    auto id3 = scheduler.add([](float a, float c) {return a + c;}, a, c);

    EXPECT_THROW(scheduler.add([](float a, float c) {return a + c;}, scheduler.getFutureResult<float>(wrong_id), c),
    my_exceptions::WrongIdException); 
}


TEST(ThrowException, Exception_Void_1) {
    TTaskScheduler scheduler;

    float a = 1;
    float c = 3;

    auto id1 = scheduler.add(&Foo::Print, a);

    EXPECT_THROW(scheduler.getResult<float>(id1), my_exceptions::WrongReturnVoid); 
}


TEST(ThrowException, Exception_Void_2) {
    TTaskScheduler scheduler;

    float a = 1;
    float b = 2;
    float c = 3;

    auto id1 = scheduler.add([](float a, float c) {return -4 * a * c;}, a, c);

    auto id2 = scheduler.add([](float b, float v) {return b * b + v;}, b, scheduler.getFutureResult<float>(id1));

    auto id3 = scheduler.add([](float b, float d) {return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));

    auto id4 = scheduler.add([](float b, float d) {return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));

    auto id5 = scheduler.add([](float a, float v) {return v / (2 * a);}, a, scheduler.getFutureResult<float>(id3));

    auto id6 = scheduler.add([](float a, float v) {return v / (2 * a);}, a, scheduler.getFutureResult<float>(id4));

    auto id7 = scheduler.add(&Foo::Print, scheduler.getFutureResult<float>(id6));
    
    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id1), -12.0);
    EXPECT_THROW(scheduler.getResult<float>(id7), my_exceptions::WrongReturnVoid); 
}

TEST(ThrowException, Exception_Result_Id) {
    TTaskScheduler scheduler;

    float a = 1;
    float b = 2;
    float c = 3;

    auto id1 = scheduler.add([](float a, float c) {return -4 * a * c;}, a, c);

    auto id2 = scheduler.add([](float b, float v) {return b * b + v;}, b, scheduler.getFutureResult<float>(id1));

    auto id3 = scheduler.add([](float b, float d) {return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));

    auto id4 = scheduler.add([](float b, float d) {return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));

    auto id5 = scheduler.add([](float a, float v) {return v / (2 * a);}, a, scheduler.getFutureResult<float>(id3));

    auto id6 = scheduler.add([](float a, float v) {return v / (2 * a);}, a, scheduler.getFutureResult<float>(id4));

    auto id7 = scheduler.add(&Foo::Print, scheduler.getFutureResult<float>(id6));

    EXPECT_THROW(scheduler.getResult<float>(8), my_exceptions::WrongIdException); 
}
#include <functional>
#include <scheduler.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>


struct Functor {
    float operator()(float num1, float num2) const {  
        return (num1 * num2);
    }
};

struct Boo {
    float Mult_three(float num) {
        return (num * 3);
    }

    float value = 0.0;
};


struct Foo {
    static float Plus_seven(float a) { 
        return a + 7;
    }

    const float& ReturnArg() const {
        return arg;
    }

    float& ReturnMultArg(float x) {
        arg += x;
        return arg;
    }


    float arg = 77;
};


float function(float num) {
    return (num + 6);
}


TEST(DifferentCallableObjects, Call_Functor) {
    TTaskScheduler scheduler;

    float a = 1;
    float b = 2;
    float c = 3;

    Functor f;

    auto id1 = scheduler.add([](float a, float c) {return a + c;}, a, c);
    auto id2 = scheduler.add([](float b, float c) {return b + c;}, b, c);
    auto id3 = scheduler.add(f, scheduler.getFutureResult<float>(id1), scheduler.getFutureResult<float>(id2));

    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id3), 20.0);
}


TEST(DifferentCallableObjects, Call_Method_RefClass) {
    TTaskScheduler scheduler;

    float b = 2;
    float c = 3;

    Boo boo;
    
    auto id1 = scheduler.add([](float b, float c) {return b + c;}, b, c);
    auto id2 = scheduler.add(&Boo::Mult_three, boo, scheduler.getFutureResult<float>(id1));

    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id2), 15.0);
}


TEST(DifferentCallableObjects, Call_Method_PtrClass) {
    TTaskScheduler scheduler;

    float b = 2;
    float c = 3;

    Boo boo;
    
    auto id1 = scheduler.add([](float b, float c) {return b + c;}, b, c);
    auto id2 = scheduler.add(&Boo::Mult_three, &boo, scheduler.getFutureResult<float>(id1));

    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id2), 15.0);
}


TEST(DifferentCallableObjects, Call_PtrFunction) {
    TTaskScheduler scheduler;

    float b = 2;
    float c = 3;
    
    auto id1 = scheduler.add([](float b, float c) {return b + c;}, b, c);
    auto id2 = scheduler.add(&function, scheduler.getFutureResult<float>(id1));

    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id2), 11.0);
}


TEST(DifferentCallableObjects, Call_Function) {
    TTaskScheduler scheduler;

    float b = 2;
    float c = 3;
    
    auto id1 = scheduler.add([](float b, float c) {return b + c;}, b, c);
    auto id2 = scheduler.add(function, scheduler.getFutureResult<float>(id1));

    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id2), 11.0);
}


TEST(DifferentCallableObjects, Call_RefFunction) {
    TTaskScheduler scheduler;

    float b = 2;
    float c = 3;

    auto id1 = scheduler.add([](float b, float c) {return b + c;}, b, c);
    auto id2 = scheduler.add(*function, scheduler.getFutureResult<float>(id1));

    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id2), 11.0);
}


TEST(DifferentCallableObjects, Call_PtrClassMember) {
    TTaskScheduler scheduler;

    float b = 2;
    float c = 3;

    Boo boo {
        .value = 5.0
    };

    auto id1 = scheduler.add(&Boo::value, &boo);

    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id1), 5.0);
}


TEST(DifferentCallableObjects, Call_ClassMember) {
    TTaskScheduler scheduler;

    float b = 2;
    float c = 3;

    Boo boo {
        .value = 5.0
    };

    auto id1 = scheduler.add(&Boo::value, boo);

    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id1), 5.0);
}


TEST(DifferentCallableObjects, Call_StdFunction) {
    TTaskScheduler scheduler;

    float b = 2;
    float c = 3;

    std::function<float(float, float)> f = [](float a, float b) { return a + b; };

    auto id1 = scheduler.add(f, b, c);

    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id1), 5.0);
}


TEST(DifferentCallableObjects, Call_StaticFunction) {
    TTaskScheduler scheduler;

    float b = 2;

    auto id1 = scheduler.add(&Foo::Plus_seven, b);

    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id1), 9.0);
}


TEST(DifferentCallableObjects, Call_Function_ReturnCosntRef) {
    TTaskScheduler scheduler;

    Foo foo;

    auto id1 = scheduler.add(&Foo::ReturnArg, foo);

    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id1), 77.0);
}


TEST(DifferentCallableObjects, Call_Function_ReturnRef) {
    TTaskScheduler scheduler;

    Foo foo;

    auto id1 = scheduler.add(&Foo::ReturnMultArg, foo, 8);

    ASSERT_FLOAT_EQ( scheduler.getResult<float>(id1), 85.0);
}


TEST(DifferentCallableObjects, Func_With_ConstRefArg) {
    TTaskScheduler scheduler;

    float value = 10.0;

    auto id1 = scheduler.add([](float& x) {x *= 2; return x;}, value);

    scheduler.executeAll();
    EXPECT_FLOAT_EQ(scheduler.getResult<float>(id1), 20.0);
    EXPECT_FLOAT_EQ(value, 20.0); 
}

TEST(DifferentCallableObjects, Func_With_RValueReference) {
    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](float&& x) {return x + 5.0f;}, std::move(10.0f)); 

    scheduler.executeAll();
    EXPECT_FLOAT_EQ(scheduler.getResult<float>(id1), 15.0f);
}

#include <scheduler.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>


TEST(GraphTests, Large_Graph) {
    TTaskScheduler scheduler;

    std::vector<int> tasks(100);

    tasks[0] = scheduler.add([](int x) { return x + 1; }, 0);

    for (int i = 1; i < 100; ++i) {
        tasks[i] = scheduler.add(
            [](int x) { return x + 1; },
            scheduler.getFutureResult<int>(tasks[i - 1])
        );
    }

    scheduler.executeAll();

    EXPECT_EQ(scheduler.getResult<int>(tasks.back()), 100);
}
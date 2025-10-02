#pragma once

#include <cstddef>
#include <cstdio>
#include <queue>
#include <memory>
#include <utility>
#include <vector>

#include "my_stl_invoke.h"
#include "my_exceptions.h"
#include "my_stl_any.h"
#include "my_stl_utilities.h"


class TTaskScheduler {
    public:
        template<typename Predicate>
        size_t add(Predicate&& predicate) {
            using Wrapper = CallableWrapperZeroArg<Predicate>;
            auto new_ptr = std::make_shared<Wrapper>(std::forward<Predicate>(predicate));
            all_tasks.push_back(new_ptr);

            return tasks_amount++;
        }

        template<typename Predicate, typename Arg1>
        size_t add(Predicate&& predicate, Arg1&& arg1) {
            using Wrapper = CallableWrapperOneArg<Predicate, Arg1>;
            auto new_ptr = std::make_shared<Wrapper>(std::forward<Predicate>(predicate), std::forward<Arg1>(arg1));
            all_tasks.push_back(new_ptr);

            return tasks_amount++;
        }

        template<typename Predicate, typename Arg1, typename Arg2>
        size_t add(Predicate&& predicate, Arg1&& arg1, Arg2&& arg2) {
            using Wrapper = CallableWrapperTwoArgs<Predicate, Arg1, Arg2>;
            auto new_ptr = std::make_shared<Wrapper>(std::forward<Predicate>(predicate), std::forward<Arg1>(arg1), std::forward<Arg2>(arg2));
            all_tasks.push_back(new_ptr);

            return tasks_amount++;
        }

        template<typename T>
        auto getFutureResult(const size_t& id) {
            if (!all_tasks.size() || id > (all_tasks.size() - 1)) {
                throw my_exceptions::WrongIdException(id);
            }

            FillGraph();

            must_be_completed_before_[tasks_amount].push_back(id);
            return CastWrapper<T>(all_tasks[id]);
        }

        void executeAll() {
            for (const auto& task: all_tasks) {
                if (!task->IsCalculated()) {
                    task->Calculate();
                }
            }
        }

        template<typename T>
        auto getResult(const size_t& id) {
            if (!all_tasks.size() || id > (all_tasks.size() - 1)) {
                throw my_exceptions::WrongIdException(id);
            }

            static_assert(!my_stl::IsVoidV<T>, "Void in getResult");

            FillGraph();

            std::queue<size_t> parents;
            std::vector<bool> visited(tasks_amount, false);
            DFS(id, parents, visited);

            while (!parents.empty()) {
                size_t currend_id = parents.front();
                if (!all_tasks[currend_id]->IsCalculated()) {
                    all_tasks[currend_id]->Calculate();
                }
                parents.pop();
            }
            return all_tasks[id]->GetResult().SameTypeCast<T>();
        }
                
    private:
        void FillGraph() {
            while (tasks_amount != (must_be_completed_before_.size() - 1)) {
                must_be_completed_before_.push_back({});
            }
        }

        class BaseCallableWrapper {
            public:
                ~BaseCallableWrapper() = default;
                virtual bool IsCalculated() = 0;
                virtual void Calculate() = 0;
                virtual my_stl::Any GetResult() = 0;


            protected:
                bool is_calculated_;
                my_stl::Any result_;
        };

        template<typename Predicate>
        class CallableWrapperZeroArg : public BaseCallableWrapper {
            public:
                CallableWrapperZeroArg(Predicate&& predicate) 
                    : predicate_(std::forward<Predicate>(predicate))
                    {is_calculated_ = false;}

                bool IsCalculated() override {
                    return is_calculated_;
                }

                void Calculate() override {
                    is_calculated_ = true;

                    using ResultType = decltype(my_stl::Invoke(std::forward<Predicate>(predicate_)));

                    if constexpr (my_stl::IsVoidV<ResultType>) {
                        throw my_exceptions::WrongReturnVoid();
                    } else {
                        result_ = my_stl::Invoke(std::forward<Predicate>(predicate_));
                    }
                }

                my_stl::Any GetResult() override {
                    return result_;
                }

            private:
                Predicate predicate_;
        };

        template<typename Predicate, typename Arg1>
        class CallableWrapperOneArg : public BaseCallableWrapper {
            public:
                CallableWrapperOneArg(Predicate&& predicate, Arg1&& arg1) 
                    : predicate_(std::forward<Predicate>(predicate)),
                    arg1_(std::forward<Arg1>(arg1))
                    {is_calculated_ = false;}

                bool IsCalculated() override {
                    return is_calculated_;
                }

                void Calculate() override {
                    is_calculated_ = true;

                    using ResultType = decltype(my_stl::Invoke(std::forward<Predicate>(predicate_), std::forward<Arg1>(arg1_)));

                    if constexpr (my_stl::IsVoidV<ResultType>) {
                        throw my_exceptions::WrongReturnVoid();
                    } else {
                        result_ = my_stl::Invoke(std::forward<Predicate>(predicate_), std::forward<Arg1>(arg1_));
                    }
                }

                my_stl::Any GetResult() override {
                    return result_;
                }

            private:
                Predicate predicate_;
                Arg1 arg1_;
        };

        template<typename Predicate, typename Arg1, typename Arg2>
        class CallableWrapperTwoArgs : public BaseCallableWrapper {
            public:
                CallableWrapperTwoArgs(Predicate&& predicate, Arg1&& arg1, Arg2&& arg2) 
                    : predicate_(std::forward<Predicate>(predicate)),
                    arg1_(std::forward<Arg1>(arg1)),
                    arg2_(std::forward<Arg2>(arg2))
                    {is_calculated_ = false;}

                bool IsCalculated() override {
                    return is_calculated_;
                }

                void Calculate() override {
                    is_calculated_ = true;

                    using ResultType = decltype(my_stl::Invoke(std::forward<Predicate>(predicate_), std::forward<Arg1>(arg1_), std::forward<Arg2>(arg2_)));

                    if constexpr (my_stl::IsVoidV<ResultType>) {
                        throw my_exceptions::WrongReturnVoid();
                    } else {
                        result_ = my_stl::Invoke(std::forward<Predicate>(predicate_), std::forward<Arg1>(arg1_), std::forward<Arg2>(arg2_));
                    }
                }

                my_stl::Any GetResult() override {
                    return result_;
                }

            private:
                Predicate predicate_;
                Arg1 arg1_;
                Arg2 arg2_;
        };
        
        void DFS(size_t currend_id, std::queue<size_t>& parents, std::vector<bool>& visited) {
            visited[currend_id] = true;

            for (const auto v: must_be_completed_before_[currend_id]) {
                if (!visited[v]) {
                    DFS(v, parents, visited);
                }
            }
            parents.push(currend_id);
        }

        template<typename T>
        class CastWrapper {
            public:
                CastWrapper(std::shared_ptr<BaseCallableWrapper> task) : task_(task.get()) {};

                operator T() {
                    return task_->GetResult().SameTypeCast<T>();
                }

            private:    
                BaseCallableWrapper* task_ = nullptr;
        };

        std::vector<std::shared_ptr<BaseCallableWrapper>> all_tasks;
        std::vector<std::vector<size_t>> must_be_completed_before_;
        size_t tasks_amount = 0;
};

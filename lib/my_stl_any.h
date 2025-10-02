#pragma once

#include "my_exceptions.h"
#include "my_stl_utilities.h"
#include <memory>

namespace my_stl {
    class Any {
        private:
            struct BaseValueHolder {
                virtual ~BaseValueHolder() = default;
                virtual std::unique_ptr<BaseValueHolder> Clone() const = 0;
            };

            template<typename T>
            struct ValueHolder : public BaseValueHolder {

                ValueHolder(T value) : value_(value) {}

                std::unique_ptr<BaseValueHolder> Clone() const override {
                    return std::make_unique<ValueHolder<T>>(value_);
                }

                T value_;
            };


        public:
            Any() = default;

            template<typename T>
            Any(const T& value) : value_(std::make_unique<ValueHolder<T>>(value)) {}

            Any(const Any& other) : value_(other.value_ ? other.value_->Clone() : nullptr) {}

            Any(Any&& other) = default;

            Any& operator=(Any&& other) = default;

            Any& operator=(const Any& other) {
                if (this == &other) {
                    return *this;
                }
                value_ = other.value_ ? other.value_->Clone() : nullptr;
                return *this;
            }

            void Reset() {
                value_.reset();
            }

            bool HasValue() {
                return (value_ != nullptr);
            }

            template<typename T>
            T SameTypeCast() {
                if (HasValue()) {
                    using CastType = my_stl::RemoveCvT<my_stl::RemoveReferenceT<T>>;

                    auto* ptr = dynamic_cast<ValueHolder<CastType>*>(value_.get());
                    if (ptr) {     
                        return static_cast<T>(ptr->value_);
                    }
                }
                throw my_exceptions::WrongCast();
            }

        private:
            std::unique_ptr<BaseValueHolder> value_ = nullptr;
    };
}
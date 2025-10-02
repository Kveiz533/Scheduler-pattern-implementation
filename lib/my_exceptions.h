#pragma once 

#include <exception>
#include <string>



namespace my_exceptions {
    class WrongIdException : public std::exception {
        public:
            WrongIdException(size_t wrong_id) : wrong_id_(wrong_id) {message_ = "Wrong id :" + std::to_string(wrong_id_);}

            const char* what() const noexcept override {
                return message_.c_str();
            }

        private:
            size_t wrong_id_;
            std::string message_;
    };


    class WrongCast : public std::exception {
        public:
            const char* what() const noexcept override {
                return message_.c_str();
            }

        private:
            std::string message_ = "Wrong Cast Type";
    };


    class WrongReturnVoid : public std::exception {
        public:
            const char* what() const noexcept override {
                return message_.c_str();
            }

        private:
            std::string message_ = "Try to return void";

    };
};
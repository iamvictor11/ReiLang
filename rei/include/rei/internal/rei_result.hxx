#pragma once
#include "rei/internal/rei_error.hxx"
#include <variant>
#include <cassert>

namespace rei
{
    template<typename T>
    class Result final
    {
    private:
        std::variant<T, Error> data_;
    public:
        Result(T value) : data_(std::move(value)) {}
        Result(Error error) : data_(std::move(error)) {}
    public:
        operator bool() const { return okey(); }
    public:
        bool okey() const { return std::holds_alternative<T>(data_); }
    public:
        auto value() & -> T&
        {
            assert(okey() && "rei::Result has error, not have value!");
            return std::get<T>(data_);
        }
        auto value() const& -> const T&
        {
            assert(okey() && "rei::Result has error, not have value!");
            return std::get<T>(data_);
        }
        auto value() && -> T
        {
            assert(okey() && "rei::Result has error, not have value!");
            return std::move(std::get<T>(data_));
        }
    public:
        auto error() & -> Error&
        {
            assert(!okey() && "rei::Result has value, not have error!");
            return std::get<Error>(data_);
        }
        auto error() const& -> const Error&
        {
            assert(!okey() && "rei::Result has value, not have error!");
            return std::get<Error>(data_);
        }
        auto error() && -> Error
        {
            assert(!okey() && "rei::Result has value, not have error!");
            return std::move(std::get<Error>(data_));
        }
    public:
        [[nodiscard]]
        auto unwrap() && -> T
        {
            if (!okey())
            {
                error().report();
                return {};
            }
            return std::move(std::get<T>(data_));
        }
        void report()
        {
            assert(!okey() && "rei::Result not have error!");
            error().report();
        }
        auto check() -> Result
        {
            if (!okey())
                error().report();
            return *this;
        }
    };
    template<>
    class Result<void> final
    {
    private:
        Error error_;
    public:
        Result() = default;
        Result(Error error) : error_(std::move(error)) {};
    public:
        operator bool() const { return okey(); }
    public:
        bool okey() const { return error_.empty(); }
    public:
        auto error() & -> Error&
        {
            assert(!okey() && "rei::Result not have error!");
            return error_;
        }
        auto error() const& -> const Error&
        {
            assert(!okey() && "rei::Result not have error!");
            return error_;
        }
        auto error() && -> Error
        {
            assert(!okey() && "rei::Result not have error!");
            return std::move(error_);
        }
    public:
        void report()
        {
            assert(!okey() && "rei::Result not have error!");
            error().report();
        }
        auto check() -> Result<void>
        {
            if (!okey())
                error().report();
            return *this;
        }
    };
}

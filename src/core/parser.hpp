#pragma once
#include "base/token.hpp"
#include "util/error.hpp"
#include "ast/node.hpp"

namespace luna
{
    class Parser final
    {
    private:
        Token::List _tokens;
        struct
        {
            uint64_t current = 0;
        } _cursor;
        Error::Reporter* _error_reporter;
    public:
        Parser(Token::List&& ts, Error::Reporter* er) : _tokens(std::move(ts)), _error_reporter(er) {};
        ~Parser() = default;
    public:
        void start();
    private:
        std::unique_ptr<Node> _expression();
        std::unique_ptr<Node> _equality();
        std::unique_ptr<Node> _term();
        std::unique_ptr<Node> _factor();
        std::unique_ptr<Node> _unary();
        std::unique_ptr<Node> _primary();
    };
}

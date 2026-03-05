#pragma once
#include "node.hpp"

namespace luna::ast
{
    template <class... Ts>
    struct overloaded : Ts...
    {
        using Ts::operator()...;
    };
    template <class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    struct Evaluator final
    {
    private:
        template<typename L, typename R>
        static Value::Data _binaryDist(L&& left, R&& right, Token::Type op);
        template<typename L, typename R>
        static Value::Data _numberBinary(L&& left, R&& right, Token::Type op);
        template<typename L, typename R>
        static Value::Data _stringBinary(L&& left, R&& right, Token::Type op);
        template<typename L, typename R>
        static Value::Data _referenceBinary(L&& left, R&& right, Token::Type op);
        template<typename L, typename R>
        static Value::Data _mixedBinary(L&& left, R&& right, Token::Type op);
    public:
        Value::Data operator()(const ast::Node& node);
    };
    struct Printer final
    {
    private:
        int _indent_level = 0;
        char _indent_char = '\t';
    public:
        Value::Data operator()(const ast::Node& node);
    private:
        void _printIndent() const;
    };
}
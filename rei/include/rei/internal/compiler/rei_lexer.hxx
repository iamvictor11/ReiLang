#pragma once
#include "rei/internal/compiler/rei_token.hxx"
#include "rei/internal/rei_error.hxx"
#include <vector>

namespace rei
{
class LexerState final
{
    friend class Lexer;
private:
    const std::string& source_;
    Error& error_;
    std::vector<Token> tokens_ {};
    uint64_t start_ = 0;
    uint64_t curr_ = 0;
    uint64_t line_ = 0;
public:
    LexerState(const std::string& source, Error& error);
private:
    void scan_();
    void moveCursor_();
    void moveCursorln_();
    void smartMoveCursor_(char c);
private:
    bool isAtEnd_() const;
    bool isAtEnd_(size_t offset) const;
    auto advance_() -> char;
    void pass_();
    void pass_(size_t step);
    bool match_(char c);
    bool match_(const char* s, size_t omit = 1);
    auto peek_() const -> char;
    auto peek_(int offset) const -> char;
private:
    void skipNote_();
    void skipWhite_();
private:
    void lexOther_(char c);
    void lexNumber_();
    void lexChar_();
    void lexString_();
    void lexIdentifier_();
private:
    void addToken_(TokenKind::E kind);
    void addToken_(TokenKind::E kind, uint64_t i);
    void addToken_(TokenKind::E kind, double f);
    void addToken_(TokenKind::E kind, const std::string& str);
};
class Lexer final
{
private:
    Error error_;
public:
    auto error() const -> const Error& { return error_; }
public:
    auto compile(const std::string& source) -> std::vector<Token>
    {
        LexerState state {source, error_};
        return state.tokens_;
    }
};
}

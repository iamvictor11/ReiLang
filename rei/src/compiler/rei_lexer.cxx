#include "rei/internal/compiler/rei_lexer.hxx"
#include "rei/internal/utils/rei_str2num.hxx"
#include <format>

namespace rei
{
LexerState::LexerState(const std::string& source, Error& error) : source_(source), error_(error)
{
    while (!isAtEnd_()) scan_();
    Token token {};
    token.kind = TK_EOF;
    token.line = line_;
    tokens_.push_back(token);
}
#pragma region Scan
void LexerState::scan_()
{
    skipWhite_();
    start_ = curr_;
    char c = advance_();
    switch (c)
    {
    case '\0': break;
    case '\\': break;
    case '(': addToken_(TK_LEFT_PAREN); break;
    case ')': addToken_(TK_RIGHT_PAREN); break;
    case '[': addToken_(match_('[') ? TK_LEFT_BRACKET_BRACKET : TK_LEFT_BRACKET); break;
    case ']': addToken_(match_(']') ? TK_RIGHT_BRACKET_BRACKET : TK_RIGHT_BRACKET); break;
    case '{': addToken_(TK_LEFT_BRACE); break;
    case '}': addToken_(TK_RIGHT_BRACE); break;
    case ',': addToken_(TK_COMMA); break;
    case '?': addToken_(TK_QUESTION); break;
    case '.': addToken_(match_('.') ? (match_('.') ? TK_DOT_DOT_DOT : TK_DOT_DOT) : TK_DOT); break;
    case ':': addToken_(match_(':') ? TK_COLON_COLON : (match_('=') ? TK_WALRUS : TK_COLON)); break;
    case ';': addToken_(TK_SEMICOLON); break;
    case '+': addToken_(match_('+') ? TK_PLUS_PLUS : (match_('=') ? TK_PLUS_EQUAL : TK_PLUS)); break;
    case '-': addToken_(match_('-') ? TK_MINUS_MINUS : (match_('=') ? TK_MINUS_EQUAL : (match_('>') ? TK_RIGHT_ARROW : TK_MINUS))); break;
    case '*': addToken_(match_('=') ? TK_STAR_EQUAL : TK_STAR); break;
    case '/': addToken_(match_('=') ? TK_SLASH_EQUAL : TK_SLASH); break;
    case '%': addToken_(match_('=') ? TK_MODULO_EQUAL : TK_MODULO); break;
    case '&': addToken_(match_('&') ? TK_AND_AND : (match_('=') ? TK_AND_EQUAL : TK_AND)); break;
    case '|': addToken_(match_('|') ? TK_PIPE_PIPE : (match_('=') ? TK_PIPE_EQUAL : TK_PIPE)); break;
    case '^': addToken_(match_('=') ? TK_CARET_EQUAL : TK_CARET); break;
    case '~': addToken_(match_('=') ? TK_TILDE_EQUAL : TK_TILDE); break;
    case '<': addToken_(match_('<') ? (match_('=') ? (match_('>') ? TK_COMPARE : TK_LESS_LESS_EQUAL) : TK_LESS_LESS) : (match_('-') ? TK_LEFT_ARROW : TK_LESS)); break;
    case '>': addToken_(match_('>') ? (match_('=') ? TK_GREATER_GREATER_EQUAL : TK_GREATER_GREATER) : TK_GREATER); break;
    case '=': addToken_(match_('=') ? TK_EQUAL_EQUAL : TK_EQUAL); break;
    case '!': addToken_(match_('=') ? TK_BANG_EQUAL : TK_BANG); break;
    case '#': addToken_(TK_POUND); break;
    case '@': addToken_(TK_AT); break;
    case '"': lexString_(); break;
    case '\'': lexChar_(); break;
    default: lexOther_(c); break;
    }
}
#pragma endregion
#pragma region Move
void LexerState::moveCursor_()
{
    curr_++;
}
void LexerState::moveCursorln_()
{
    curr_++;
    line_++;
}
void LexerState::smartMoveCursor_(char c)
{
    if (c == '\n') moveCursorln_();
    else moveCursor_();
}
#pragma endregion
#pragma region Look
bool LexerState::isAtEnd_() const
{
    return curr_ >= source_.length();
}
bool LexerState::isAtEnd_(size_t offset) const
{
    return curr_ + offset >= source_.length();
}
auto LexerState::advance_() -> char
{
    if (isAtEnd_()) return '\0';
    char c = source_.at(curr_);
    smartMoveCursor_(c);
    return c;
}
void LexerState::pass_()
{
    if (isAtEnd_()) return;
    smartMoveCursor_(source_.at(curr_));
}
void LexerState::pass_(size_t step)
{
    while (step > 0)
    {
        if (isAtEnd_()) return;
        smartMoveCursor_(source_.at(curr_));
        step--;
    }
}
bool LexerState::match_(char c)
{
    if (isAtEnd_() || source_.at(curr_) != c)
        return false;
    smartMoveCursor_(c);
    return true;
}
bool LexerState::match_(const char* s, size_t omit)
{
    size_t i = 0;
    const char* p = s + omit;
    for (; *p != '\0'; p++)
    {
        if (isAtEnd_(i))
            return false;
        if (peek_(i) != *p)
            return false;
        i++;
    }
    pass_(i);
    return true;
}
auto LexerState::peek_() const -> char
{
    if (isAtEnd_()) return '\0';
    return source_.at(curr_);
}
auto LexerState::peek_(int offset) const -> char
{
    if (isAtEnd_(offset)) return '\0';
    return source_.at(curr_ + offset);
}
#pragma endregion
#pragma region Skip
void LexerState::skipNote_()
{
    while (peek_() != '\n' && !isAtEnd_())
        moveCursor_();
}
void LexerState::skipWhite_()
{
    while (!isAtEnd_())
    {
        char c = peek_();
        switch (c)
        {
        case ' ':
        case '\t':
        case '\r':
            moveCursor_();
            break;
        case '\n':
            moveCursorln_();
            break;
        default:
            return;
        }
    }
}
#pragma endregion
#pragma region Lex
void LexerState::lexOther_(char c)
{
    if (isdigit(c))
    {
        lexNumber_();
        return;
    }
    else if (isalpha(c) || c == '_')
    {
        lexIdentifier_();
        return;
    }
    error_.title = "LexerState::lexOther_";
    error_.message = std::format("unknown char '{}'(HEX: 0x{:02x}) [{}]!", c, (unsigned char)c, line_);
    error_.code = REI_ERROR_LEXER;
}
void LexerState::lexNumber_()
{
    bool is_hex = false;
    bool is_float = false;
    while (!isAtEnd_())
    {
        char c = peek_();
        if (isdigit(c) || c == 'b' || c == 'B' || c == 'o' || c == 'O' || c == '\'')
        { advance_(); continue; }
        if (c == 'x' || c == 'X')
        { is_hex = true; advance_(); continue; }
        if (is_hex && (c == 'a' || c == 'A' || c == 'b' || c == 'B' || c == 'c' || c == 'C' || c == 'd' || c == 'D' || c == 'e' || c == 'E' || c == 'f' || c == 'F'))
        { advance_(); continue; }
        break;
    }
    if (!isAtEnd_() && peek_() == '.' && peek_(1) != '.')
    {
        is_float = false;
        advance_();
        while (!isAtEnd_())
        {
            char c = peek_();
            if (isdigit(c) || c == '\'')
            { advance_(); continue; }
            break;
        }
    }
    if (!isAtEnd_() && (peek_() == 'e' || peek_() == 'E'))
    {
        is_float = true;
        advance_();
        if (peek_() == '+' || peek_() == '-') advance_();
        while (!isAtEnd_())
        {
            char c = peek_();
            if (isdigit(c) || c == '\'')
            { advance_(); continue; }
            break;
        }
    }
    if (is_float) addToken_(TK_LIT_FLOAT, stringToFloat({source_.data() + start_, curr_ - start_}));
    else addToken_(TK_LIT_INT, stringToInt({source_.data() + start_, curr_ - start_}));
}
void LexerState::lexChar_()
{
    Token token {};
    token.kind = TK_CHAR;
    char c = advance_();
    if (c == '\\')
    {
        if (isAtEnd_())
        {
            error_.title = "LexerState::lexChar_";
            error_.message = std::format("unclosed char [{}]", line_);
            error_.code = REI_ERROR_LEXER;
        }
        char next = advance_();
        switch (next)
        {
            case 'n':  token.literal.i = static_cast<uint64_t>('\n'); break;
            case 'r':  token.literal.i = static_cast<uint64_t>('\r'); break;
            case 't':  token.literal.i = static_cast<uint64_t>('\t'); break;
            case '\\': token.literal.i = static_cast<uint64_t>('\\'); break;
            case '"':  token.literal.i = static_cast<uint64_t>('"');  break;
            case '\'': token.literal.i = static_cast<uint64_t>('\''); break;
        }
    }
    if (isAtEnd_() || !match_('\''))
    {
        error_.title = "LexerState::lexChar_";
        error_.message = std::format("unclosed char [{}]", line_);
        error_.code = REI_ERROR_LEXER;
    }
    token.literal.i = static_cast<uint64_t>(c);
}
void LexerState::lexString_()
{
    std::string value;
    while (peek_() != '"' && !isAtEnd_())
    {
        char c = advance_();
        if (c == '\\')
        {
            if (isAtEnd_()) break;
            char next = advance_();
            switch (next)
            {
                case 'n':  value += '\n'; break;
                case 'r':  value += '\r'; break;
                case 't':  value += '\t'; break;
                case '\\': value += '\\'; break;
                case '"':  value += '"';  break;
                case '\'': value += '\''; break;
                default: value += c; value += next; break;
            }
        }
        else
        {
            value += c;
        }
    }
    if (isAtEnd_())
    {
        error_.title = "LexerState::lexString_";
        error_.message = std::format("unclosed string [{}]!", line_);
        error_.code = REI_ERROR_LEXER;
        return;
    }
    pass_();
    addToken_(TK_LIT_STRING, value);
}
void LexerState::lexIdentifier_()
{
    auto isidentifier = [](char c) -> bool
    { return isalnum(c) || c == '_'; };
    while (isidentifier(peek_())) pass_();
    std::string_view lexeme {source_.data() + start_, curr_ - start_};
    TokenKind kind = keywordToTokenKind(lexeme);
    addToken_(kind);
}
#pragma endregion
#pragma region Add
void LexerState::addToken_(TokenKind kind)
{
    Token token {};
    token.kind = kind;
    token.lexeme = {source_.data() + start_, curr_ - start_};
    token.line = line_;
    tokens_.push_back(token);
}
void LexerState::addToken_(TokenKind kind, uint64_t i)
{
    Token token {};
    token.kind = kind;
    token.lexeme = {source_.data() + start_, curr_ - start_};
    token.literal.i = i;
    token.line = line_;
    tokens_.push_back(token);
}
void LexerState::addToken_(TokenKind kind, double f)
{
    Token token {};
    token.kind = kind;
    token.lexeme = {source_.data() + start_, curr_ - start_};
    token.literal.f = f;
    token.line = line_;
    tokens_.push_back(token);
}
void LexerState::addToken_(TokenKind kind, const std::string& str)
{
    Token token {};
    token.kind = kind;
    token.lexeme = {source_.data() + start_, curr_ - start_};
    token.str = str;
    token.line = line_;
    tokens_.push_back(token);
}
#pragma endregion
}

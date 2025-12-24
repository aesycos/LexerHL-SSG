/*
    lexer.h  
*/

#pragma once

#include <cstdlib>
#include <string>
#include <vector>

#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class TokenType : std::uint8_t {
    Whitespace,
    Newline,
    Comment,
    PreprocessorDirective,
    HeaderName,
    String,
    Number,
    Operator,
    Punctuator,
    StandardKeyword,
    StandardIdentifier,
    SpecialIdentifier,
    CharLiteral,
    Unknown
};

struct Token {
    TokenType type;
    std::string text;
};

inline const char* tokenTypeName(TokenType t)
{
    switch (t) {
        case TokenType::Whitespace:             return "Whitespace";
        case TokenType::Newline:                return "Newline";
        case TokenType::Comment:                return "Comment";
        case TokenType::PreprocessorDirective:  return "PreprocessorDirective";
        case TokenType::HeaderName:              return "HeaderName";
        case TokenType::String:                  return "String";
        case TokenType::Number:                  return "Number";
        case TokenType::Operator:                return "Operator";
        case TokenType::Punctuator:              return "Punctuator";
        case TokenType::StandardKeyword:         return "StandardKeyword";
        case TokenType::StandardIdentifier:      return "StandardIdentifier";
        case TokenType::SpecialIdentifier:       return "SpecialIdentifier";
        default:                                 return "Unknown";
    }
}

class Lexer {
public:
    using StateId = std::uint16_t;

    virtual ~Lexer() = default;
    /*
    */
    template <typename Derived>
    std::vector<Token> lex()
    {
        auto& self = static_cast<Derived&>(*this);
        const auto& table = self.handlers();

        while (!eof()) {
            (self.*table[self.state])();
        }

        flush();
        return tokens;
    }

protected:
    explicit Lexer(const std::string& source) : src(source) {}

    const std::string& src;
    std::size_t pos = 0;
    StateId state = 0;

    std::string buffer;
    std::vector<Token> tokens;

    char peek() const;
    char peekNext() const;
    char advance();
    bool eof() const;

    void emit(TokenType type);
    void emit(TokenType type, const std::string& text);

    void flush();

    bool match(char expected);
};
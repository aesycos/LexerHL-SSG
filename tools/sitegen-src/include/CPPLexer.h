/*
    CPPLexer.h
*/

#pragma once
#include <unordered_set>
#include "lexer.h"

static const std::unordered_set<std::string> cppKeywords = {
    "int", "char", "float", "double", "void",
    "if", "else", "for", "while", "do",
    "return", "break", "continue",
    "class", "struct", "public", "private", "protected",
    "const", "constexpr", "static",
    "switch", "case", "default",
    "try", "catch", "throw",
    "namespace", "using",
    "new", "delete",
    "true", "false", "nullptr"
};

class CPPLexer : public Lexer {
public:
    using Handler = void (CPPLexer::*)();

    enum State : StateId {
        Normal,
        Identifier,
        Number,
        String,
        LineComment,
        BlockComment,
        Preprocessor,
        Whitespace,
        CharLiteral
    };

    explicit CPPLexer(const std::string& source)
        : Lexer(source) {};

    const std::vector<Handler>& handlers() const {
        static const std::vector<Handler> table = {
            &CPPLexer::handleNormal,
            &CPPLexer::handleIdentifier,
            &CPPLexer::handleNumber,
            &CPPLexer::handleString,
            &CPPLexer::handleLineComment,
            &CPPLexer::handleBlockComment,
            &CPPLexer::handlePreprocessor,
            &CPPLexer::handleWhitespace,
            &CPPLexer::handleCharLiteral
        };
        return table;
    };

protected:
    void handleNormal();
    void handleIdentifier();
    void handleNumber();
    void handleString();
    void handleLineComment();
    void handleBlockComment();
    void handlePreprocessor();
    void handleWhitespace();
    void handleCharLiteral();
    bool isKeyword(const std::string& s);
};

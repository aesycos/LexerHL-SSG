/*
    JSLexer.h
*/

#pragma once
#include "lexer.h"

#include <string>
#include <vector>

class JSLexer : public Lexer {
public:
    using Handler = void (JSLexer::*)();

    enum State : StateId {
        Normal,
        Identifier,
        Number,
        String,
        TemplateString,
        RegexLiteral,
        LineComment,
        BlockComment
    };

    JSLexer(const std::string& src) : Lexer(src) {
        state = Normal;
    }

protected:
    const std::vector<Handler>& handlers() const {
        static const std::vector<Handler> table = {
            //&JSLexer::handleNormal,
            //&JSLexer::handleIdentifier,
            //&JSLexer::handleNumber,
            //&JSLexer::handleString,
            &JSLexer::handleTemplateString,
            &JSLexer::handleRegexLiteral,
            //&JSLexer::handleLineComment,
            //&JSLexer::handleBlockComment
        };
        return table;
    }

    void handleTemplateString();
    void handleRegexLiteral();
};

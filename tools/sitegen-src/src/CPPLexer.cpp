/*
    CPPLexer.cpp
*/

#include "CPPLexer.h"
#include <cctype>
#include <unordered_set>

/*
 * Normal scanning state
 */
void CPPLexer::handleNormal()
{
    char c = peek();   // look, do NOT consume yet

    // ---- whitespace ----
    if (c == ' ' || c == '\t') {
        state = Whitespace;
        return;
    }

    // ---- newline ----
    if (c == '\n') {
        advance();
        emit(TokenType::Newline, "\n");
        return;
    }

    // ---- identifier / keyword ----
    if (std::isalpha(c) || c == '_') {
        state = Identifier;
        return;
    }

    // ---- number ----
    if (std::isdigit(c)) {
        state = Number;
        return;
    }

    // ---- preprocessor ----
    if (c == '#') {
        state = Preprocessor;
        return;
    }

    // ---- comments or '/' operator ----
    if (c == '/') {
        if (peekNext() == '/') {      // you need peekNext() (or equivalent)
            state = LineComment;
            return;
        }
        if (peekNext() == '*') {
            state = BlockComment;
            return;
        }
        advance();
        emit(TokenType::Operator, "/");
        return;
    }

    if (c == '\'') {
        state = CharLiteral;
        return;
    }


    if (c == '"') {
        state = String;
        return;
    }


    // ---- operators & punctuators ----
    advance(); // NOW consume exactly one character

    switch (c) {

        case '<':
            if (match('<')) emit(TokenType::Operator, "<<");
            else if (match('=')) emit(TokenType::Operator, "<=");
            else emit(TokenType::Operator, "<");
            break;

        case '>':
            if (match('>')) emit(TokenType::Operator, ">>");
            else if (match('=')) emit(TokenType::Operator, ">=");
            else emit(TokenType::Operator, ">");
            break;

        case '=':
            emit(TokenType::Operator, match('=') ? "==" : "=");
            break;

        case '!':
            emit(TokenType::Operator, match('=') ? "!=" : "!");
            break;

        case '&':
            emit(TokenType::Operator, match('&') ? "&&" : "&");
            break;

        case '|':
            emit(TokenType::Operator, match('|') ? "||" : "|");
            break;

        case '+':
            emit(TokenType::Operator, match('+') ? "++" : "+");
            break;

        case '-':
            if (match('-')) emit(TokenType::Operator, "--");
            else if (match('>')) emit(TokenType::Operator, "->");
            else emit(TokenType::Operator, "-");
            break;

        case '*': case '%':
            emit(TokenType::Operator, std::string(1, c));
            break;

        case ':':
            if (match(':'))
                emit(TokenType::Operator, "::");
            else
                emit(TokenType::Punctuator, ":");
            break;
        case '.':
            emit(TokenType::Operator, ".");
            break;

        case ';': case ',': case '(':
        case ')': case '{': case '}':
        case '[': case ']':
            emit(TokenType::Punctuator, std::string(1, c));
            break;

        default:
            emit(TokenType::Unknown, std::string(1, c));
            break;
    }
}


/*
 * Identifier scanning
 */
void CPPLexer::handleIdentifier()
{
    while (std::isalnum(peek()) || peek() == '_') {
        buffer += advance();
    }

    if (cppKeywords.contains(buffer))
        emit(TokenType::StandardKeyword);
    else if (std::isupper(buffer[0]))
        emit(TokenType::SpecialIdentifier);
    else
        emit(TokenType::StandardIdentifier);

    state = Normal;
}


/*
 * Number scanning
 */
void CPPLexer::handleNumber()
{
    while (std::isdigit(peek())) {
        buffer += advance();
    }

    emit(TokenType::Number);
    state = Normal;
}


/*
 * String literal scanning
 */
void CPPLexer::handleString()
{
    buffer.clear();

    buffer += advance(); // consume opening " INTO buffer

    while (!eof()) {
        char c = advance();
        buffer += c;

        if (c == '\\') {
            if (!eof()) {
                buffer += advance(); // escaped char
            }
            continue;
        }

        if (c == '"') {
            emit(TokenType::String, buffer); // buffer already includes quotes
            state = Normal;
            return;
        }
    }

    // Unterminated string literal (still emit what we saw)
    emit(TokenType::String, buffer);
    state = Normal;
}

void CPPLexer::handleCharLiteral()
{
    buffer.clear();
    buffer += advance(); // opening '

    while (!eof()) {
        char c = advance();
        buffer += c;

        if (c == '\\') {
            if (!eof())
                buffer += advance(); // escaped char
            continue;
        }

        if (c == '\'') {
            emit(TokenType::CharLiteral, buffer);
            buffer.clear();
            state = Normal;
            return;
        }
    }

    // Unterminated char literal
    emit(TokenType::CharLiteral, buffer);
    buffer.clear();
    state = Normal;
}



/*
 * Line comment scanning
 */
void CPPLexer::handleLineComment()
{
    

    // consume "//"
    advance(); // '/'
    advance(); // '/'
    buffer = "//";

    while (!eof() && peek() != '\n')
        buffer += advance();

    emit(TokenType::Comment);
    state = Normal;
}

void CPPLexer::handleBlockComment()
{
    

    // consume "/*"
    advance(); // '/'
    advance(); // '*'
    buffer = "/*";

    while (!eof()) {
        char c = advance();
        buffer += c;

        if (c == '*' && !eof() && peek() == '/') {
            buffer += advance(); // '/'
            break;
        }
    }

    emit(TokenType::Comment);
    state = Normal;
}


/*
 * Preprocessor directive scanning
 */
void CPPLexer::handlePreprocessor()
{
    while (peek() != '\n' && !eof()) {
        buffer += advance();
    }

    emit(TokenType::PreprocessorDirective);
    
    state = Normal;
}

/*
*/
void CPPLexer::handleWhitespace()
{
    buffer.clear();

    while (peek() == ' ' || peek() == '\t') {
        buffer += advance();
    }

    emit(TokenType::Whitespace);
    state = Normal;
}

/*
*/
bool CPPLexer::isKeyword(const std::string& s)
{
    static const std::unordered_set<std::string> keywords = {
        "if","else","for","while","return","class","struct","namespace"
    };
    return keywords.count(s) != 0;
}

#include <stdexcept>

#include "lexer.h"

/*
 */
char Lexer::peek() const
{
	return eof() ? '\0' : src[pos];
}

/*
*/
char Lexer::peekNext() const
{
    return (pos + 1 < src.size()) ? src[pos + 1] : '\0';
}

/*
 */
char Lexer::advance()
{
	return eof() ? '\0' : src[pos++];
}

/*
 */
bool Lexer::eof() const
{
	return pos >= src.size();
}

/*
 */
void Lexer::emit(TokenType type)
{
	tokens.push_back({ type, buffer });
    buffer.clear();
}

void Lexer::emit(TokenType type, const std::string& text)
{
    tokens.push_back(Token{ type, text });
}


/*
*/
void Lexer::flush()
{
    if (buffer.empty())
        return;

    // Default fallback; language handlers may override behavior earlier
    emit(TokenType::StandardIdentifier);
}

/*
*/
bool Lexer::match(char expected)
{
    if (peek() != expected)
        return false;

    advance();
    return true;
}


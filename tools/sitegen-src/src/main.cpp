
#include "core.h"
#include "lexer.h"
#ifdef LEXER_CPP
#include "CPPLexer.h"
#endif

#include <iostream>
#include <string>

const char* tokenClass(TokenType t);
std::string htmlEscape(std::string_view s);
void emitHTML(std::ostream& out, const std::vector<Token>& tokens);

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: sitegen <source-file>\n";
        return 1;
    }

    try {
        std::string source = readFile(argv[1]);

        #ifdef LEXER_CPP
            CPPLexer lexer(source);

            auto tokens = lexer.lex<CPPLexer>();

            std::ostream & out = std::cout;

            out << "<pre>\n";
            out << "<code data-type=\"c++\">";
            out << "<span class=\"code-filename\" >" << argv[1];
            out << "</span>";
            out << "<button class=\"copy-btn\" data-clipboard-target=\"#content-to-copy\">";
            out << "<i class=\"fa fa-copy\"></i></button>";
            emitHTML(out, tokens);

            out << "</code></pre>\n";

            for (const auto& tok : tokens) {
                std::cerr << tokenTypeName(tok.type)
                          << " : [" << tok.text << "]\n";
            }
        #else
            // Base lexer can't be instantiated directly if abstract
            std::cerr << "No lexer enabled. Build with LEXERS=cpp\n";
            return 1;
        #endif

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

const char* tokenClass(TokenType t)
{
    switch (t) {
        case TokenType::StandardKeyword:      return "standard-keyword";
        case TokenType::StandardIdentifier:   return "standard-identifier";
        case TokenType::SpecialIdentifier:    return "special-identifier";
        case TokenType::Number:               return "number";
        case TokenType::String:               return "string-literal";
        case TokenType::Comment:              return "comment";
        case TokenType::Operator:             return "operator";
        case TokenType::Punctuator:           return "punctuator";
        case TokenType::PreprocessorDirective:return "preprocessor-directive";
        default:                              return nullptr;
    }
}

std::string htmlEscape(std::string_view s)
{
    std::string out;
    out.reserve(s.size());

    for (char c : s) {
        switch (c) {
            case '&':  out += "&amp;";  break;
            case '<':  out += "&lt;";   break;
            case '>':  out += "&gt;";   break;
            case '"':  out += "&quot;"; break;
            default:   out += c;         break;
        }
    }
    return out;
}

void emitHTML(std::ostream& out, const std::vector<Token>& tokens)
{
    for (const Token& tok : tokens) {

        // Preserve formatting EXACTLY
        if (tok.type == TokenType::Whitespace ||
            tok.type == TokenType::Newline) {
            out << tok.text;
            continue;
        }

        const char* cls = tokenClass(tok.type);

        if (!cls) {
            // Unknown or raw text
            out << htmlEscape(tok.text);
            continue;
        }

        out << "<span class=\"" << cls << "\">"
            << htmlEscape(tok.text)
            << "</span>";
    }
}



#include "lexer.hpp"
#include <stdexcept>

void Lexer::getFileContent(const std::string& file)
{
    
}

unsigned long Lexer::getNextSpace(unsigned long index)
{
    
}

Token Lexer::getCurToken()
{
    return {
        TokenType::Invalid, 
        "", 
        {
            curPos.line, 
            curPos.column
        }
    };
}

std::vector<Token> Lexer::applyLexer()
{
    std::vector<Token> tokens;

    Token token;
    while (true)
    {
        token = getCurToken();

        if (token.type == TokenType::Invalid)
        {
            throw std::runtime_error(
                "Token at line: " + std::to_string(token.position.line) + 
                " column:" + std::to_string(token.position.column)
            );
        }

        if (token.type == TokenType::EndOfFile)
        {
            break;
        }

        tokens.push_back(token);
    }

    return tokens;
}

Lexer::Lexer(const std::string& file)
{
    getFileContent(file);
    curPos.column = 1;   
    curPos.line = 1;   
}
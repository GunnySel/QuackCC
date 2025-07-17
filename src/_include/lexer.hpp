#pragma once

#include <string>
#include <vector>

#include "token.h"

class Lexer
{
public:
    Lexer(const std::string& file);

    std::vector<Token> applyLexer();

private:
    void getFileContent(const std::string& file);

    unsigned long getNextSpace(unsigned long index);

    Token getCurToken();

private:
    std::string fileContent;
    FilePosition curPos; 
    unsigned long curIndex;

};

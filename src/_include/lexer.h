#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "token.h"

class Lexer
{
public:
    Lexer(const std::string& filename);

    std::vector<Token> applyLexer();

private:
    void getFileContent(const std::string& file);

    unsigned long getNextSpace(unsigned long index);

    Token getCurToken();

private:
    std::string m_fileContent;
    FilePosition m_curPos; 
    unsigned long m_curIndex;

private:
    static void initKeywords();
    static void initOperators();
    static void initPunctuation();

    static inline bool isKeyword(const std::string& value);
    static inline bool isOperator(const std::string& value);
    static inline bool isPunctuation(const std::string& value);
private:
    static bool s_initialized;
    
    static std::unordered_map<std::string, TokenType> s_keywords;
    static std::unordered_map<std::string, TokenType> s_operator;
    static std::unordered_map<std::string, TokenType> s_punctuation;

};

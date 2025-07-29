#include "lexer.h"
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include <sstream>
#include <string>

bool Lexer::s_initialized = false;
std::unordered_map<std::string, TokenType> Lexer::s_keywords = {};
std::unordered_map<std::string, TokenType> Lexer::s_operator = {};
std::unordered_map<std::string, TokenType> Lexer::s_punctuation = {};
std::unordered_set<char> Lexer::s_operatorChars = {};
std::unordered_set<char> Lexer::s_punctuationChars = {};


Lexer::Lexer(const std::string& file)
{
    if (s_initialized == false)
    {
        initKeywords();
        initOperators();
        initPunctuation();

        s_initialized = true;
    }

    getFileContent(file);
    m_curPos.column = 1;   
    m_curPos.line = 1;   
}

// isdigit, isalpha, '_'
void Lexer::getFileContent(const std::string& filename)
{
    std::ifstream file(filename);
    
    if (!file.is_open()) 
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::ostringstream contentStream;
    contentStream << file.rdbuf(); 
    m_fileContent = contentStream.str();
    
}


void Lexer::moveCursor(unsigned long toPos)
{
    if (toPos >= m_fileContent.size())
    {
        throw std::runtime_error("toPos in moveCursor cannot be reached.");
    }

    while (m_curIndex < toPos)
    {
        if (m_fileContent[m_curIndex] == '\n')
        {
            m_curPos.column = 1;
            m_curPos.line++;
        }
        else 
        {
            m_curPos.column++;
        }

        m_curIndex++; 
    }
}

unsigned long Lexer::findSpace(unsigned long index)
{
    int spaceIndex = index;
    while(spaceIndex < m_fileContent.size() && !(std::isspace(m_fileContent[spaceIndex])))
    {
        spaceIndex++;
    }

    if(spaceIndex == m_fileContent.size())
    {
        return -1;
    }

    return spaceIndex;
}

unsigned long Lexer::findNonSpace(unsigned long index)
{
    int tokenIndex = index;
    while(tokenIndex < m_fileContent.size() && (std::isspace(m_fileContent[tokenIndex])))
    {
        tokenIndex++;
    }

    if(tokenIndex == m_fileContent.size())
    {
        return -1;
    }

    return tokenIndex;
}

Token Lexer::getCurToken()
{
    return {
        TokenType::EndOfFile, 
        "", 
        {
            m_curPos.line, 
            m_curPos.column
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
            tokens.push_back(token);
            break;
        }

        tokens.push_back(token);
    }

    return tokens;
}

bool Lexer::isKeyword(const std::string& value)
{
    return s_keywords.find(value) != s_keywords.end();
}

bool Lexer::isOperator(const std::string& value)
{
    return s_operator.find(value) != s_operator.end();
}

bool Lexer::isPunctuation(const std::string& value)
{
    return s_punctuation.find(value) != s_punctuation.end();
}


void Lexer::initKeywords()
{
    s_keywords["if"] = TokenType::KeywordIf;
    s_keywords["else"] = TokenType::KeywordElse;
    s_keywords["while"] = TokenType::KeywordWhile;
    s_keywords["for"] = TokenType::KeywordFor;
    s_keywords["continue"] = TokenType::KeywordContinue;
    s_keywords["break"] = TokenType::KeywordBreak;
    s_keywords["return"] = TokenType::KeywordReturn;
    s_keywords["do"] = TokenType::KeywordDo;
    s_keywords["case"] = TokenType::KeywordCase;
    s_keywords["switch"] = TokenType::KeywordSwitch;
    s_keywords["default"] = TokenType::KeywordDefault;
    s_keywords["typedef"] = TokenType::KeywordTypedef;

    s_keywords["class"] = TokenType::KeywordClass;
    s_keywords["struct"] = TokenType::KeywordStruct;
    s_keywords["union"] = TokenType::KeywordUnion;
    s_keywords["enum"] = TokenType::KeywordEnum;

    s_keywords["void"] = TokenType::KeywordVoid;
    s_keywords["int"] = TokenType::KeywordInt;
    s_keywords["double"] = TokenType::KeywordDouble;
    s_keywords["float"] = TokenType::KeywordFloat;
    s_keywords["long"] = TokenType::KeywordLong;
    s_keywords["char"] = TokenType::KeywordChar;
    s_keywords["short"] = TokenType::KeywordShort;

    s_keywords["const"] = TokenType::KeywordConst;
    s_keywords["unsigned"] = TokenType::KeywordUnsigned;
    s_keywords["signed"] = TokenType::KeywordSigned;
    s_keywords["extern"] = TokenType::KeywordExtern;
}

void Lexer::initOperators()
{
    s_operator["<"]  = TokenType::OperatorLeftArrow;
    s_operator[">"]  = TokenType::OperatorRightArrow;

    s_operator["*"]  = TokenType::OperatorAsterik;
    s_operator["&"]  = TokenType::OperatorAmpersand;

    s_operator["+"]  = TokenType::OperatorArithmeticAdd;
    s_operator["-"]  = TokenType::OperatorArithmeticSub;
    s_operator["/"]  = TokenType::OperatorArithmeticDiv;
    s_operator["%"]  = TokenType::OperatorArithmeticMod;

    s_operator["=="] = TokenType::OperatorRelationalEqual;
    s_operator["!="] = TokenType::OperatorRelationalNotEqual;
    s_operator[">="] = TokenType::OperatorRelationalBiggerEqual;
    s_operator["<="] = TokenType::OperatorRelationalLowerEqual;

    s_operator["&&"] = TokenType::OperatorLogicalAnd;
    s_operator["||"] = TokenType::OperatorLogicalOr;
    s_operator["!"]  = TokenType::OperatorLogicalNot;

    s_operator["|"]  = TokenType::OperatorBitwiseOr;
    s_operator["^"]  = TokenType::OperatorBitwiseXor;
    s_operator["~"]  = TokenType::OperatorBitwiseNot;
    s_operator["<<"] = TokenType::OperatorBitwiseShiftLeft;
    s_operator[">>"] = TokenType::OperatorBitwiseShiftRight;

    s_operator["="]  = TokenType::OperatorAssign;
    s_operator["+="] = TokenType::OperatorAssignAdd;
    s_operator["-="] = TokenType::OperatorAssignSub;
    s_operator["*="] = TokenType::OperatorAssignMul;
    s_operator["/="] = TokenType::OperatorAssignDiv;
    s_operator["%="] = TokenType::OperatorAssignMod;
    s_operator["&="] = TokenType::OperatorAssignAnd;
    s_operator["|="] = TokenType::OperatorAssignOr;
    s_operator["^="] = TokenType::OperatorAssignXor;
    s_operator["<<="] = TokenType::OperatorAssignShiftLeft;
    s_operator[">>="] = TokenType::OperatorAssignShiftRight;

    s_operator["++"] = TokenType::OperatorIncrement;
    s_operator["--"] = TokenType::OperatorDecrement;

    s_operator["?"]  = TokenType::OperatorTernary;

    for (const auto& [op, _] : s_operator) 
    {
        for (char c : op)
        {
            s_operatorChars.insert(c);
        }
    }
}

void Lexer::initPunctuation()
{
    s_punctuation["{"]   = TokenType::PunctuationBracketLBlock;
    s_punctuation["}"]   = TokenType::PunctuationBracketRBlock;
    s_punctuation["["]   = TokenType::PunctuationBracketLIndex;
    s_punctuation["]"]   = TokenType::PunctuationBracketRIndex;
    s_punctuation["("]   = TokenType::PunctuationBracketLCall;
    s_punctuation[")"]   = TokenType::PunctuationBracketRCall;

    s_punctuation[","]   = TokenType::PunctuationComma;
    s_punctuation[";"]   = TokenType::PunctuationSemicolon;
    s_punctuation[":"]   = TokenType::PunctuationColon;
    s_punctuation["::"]  = TokenType::PunctuationDoubleColon;
    s_punctuation["."]   = TokenType::PunctuationDot;
    s_punctuation["..."] = TokenType::Punctuation3Dots;
    s_punctuation["->"]  = TokenType::PunctuationArrow;

    for (const auto& [op, _] : s_punctuation) 
    {
        for (char c : op)
        {
            s_punctuationChars.insert(c);
        }
    }
}

bool Lexer::isOperatorChar(const char value)
{
    return s_operatorChars.find(value) != s_operatorChars.end();
}

bool Lexer::isPunctuationChar(const char value)
{
    return s_punctuationChars.find(value) != s_punctuationChars.end();
}
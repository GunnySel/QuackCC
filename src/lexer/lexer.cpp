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

    m_lastPos.column = 1;   
    m_lastPos.line = 1;   

    m_lastIndex = 0;
    m_curIndex = 0;
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

    // Move forward
    if (m_lastIndex < toPos)
    {
        while (m_lastIndex < toPos)
        {
            if (m_fileContent[m_lastIndex] == '\n')
            {
                m_lastPos.column = 1;
                m_lastPos.line++;
            }
            else
            {
                m_lastPos.column++;
            }
            m_lastIndex++;
        }
    }
    // Move backward
    else if (m_lastIndex > toPos)
    {
        while (m_lastIndex > toPos)
        {
            m_lastIndex--;
            if (m_fileContent[m_lastIndex] == '\n')
            {
                // Recalculate column when crossing a newline
                m_lastPos.line--;
                m_lastPos.column = 1;

                // Count the columns from the previous line start
                size_t tempIndex = m_lastIndex - 1;
                while (tempIndex > 0 && m_fileContent[tempIndex] != '\n')
                {
                    m_lastPos.column++;
                    tempIndex--;
                }
            }
            else
            {
                m_lastPos.column--;
                if (m_lastPos.column < 1) 
                {
                    m_lastPos.column = 1;
                }
            }
        }
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

Token Lexer::handleNumericLiteral()
{
    bool isHex = false;
    bool isBin = false;
    std::string numberStr;

    if (m_fileContent[m_curIndex] == '0' &&
        (m_curIndex + 1 < m_fileContent.size()) &&
        (m_fileContent[m_curIndex + 1] == 'x' || m_fileContent[m_curIndex + 1] == 'X'))
    {
        isHex = true;
        numberStr = "0x";
        m_curIndex += 2;
    }

    else if (m_fileContent[m_curIndex] == '0' &&
             (m_curIndex + 1 < m_fileContent.size()) &&
             (m_fileContent[m_curIndex + 1] == 'b' || m_fileContent[m_curIndex + 1] == 'B'))
    {
        isBin = true;
        numberStr = "0b";
        m_curIndex += 2;
    }

    std::string digitPartOnly;
    while (m_curIndex < m_fileContent.size())
    {
        char c = m_fileContent[m_curIndex];

        if (isHex)
        {
            if (!std::isxdigit(static_cast<unsigned char>(c))) 
            {
                break;
            }
        }

        else if (isBin)
        {
            if (c != '0' && c != '1')
            {
                break;
            } 
        }
        
        else
        {
            if (!std::isdigit(static_cast<unsigned char>(c)))
            {
                break;
            } 
        }

        numberStr += c;
        digitPartOnly += c;
        m_curIndex++;
    }

    // Invalid if `0x` or `0b` not followed by digits
    if ((isHex || isBin) && digitPartOnly.empty())
    {
        return {
            TokenType::Invalid,
            "",
            m_lastPos
        };
    }

    FilePosition tokenPos = m_lastPos;

    moveCursor(m_curIndex);

    return {
        TokenType::LiteralInteger,
        numberStr,
        tokenPos
    };
}

Token Lexer::handleStringLiteral()
{
    
}

Token Lexer::handleIdentifier()
{
    return {
        TokenType::Invalid, 
        "", 
        m_lastPos
    };
    
}

Token Lexer::handlePunctuation()
{
    // Iterate characters until not punctuation anymore
    while (m_curIndex < m_fileContent.size() && isPunctuationChar(m_fileContent[m_curIndex]))
    {
        m_curIndex++;
    }
    
    // Get the candidate for the punctuation
    std::string puncCandidate = m_fileContent.substr(m_lastIndex, m_curIndex - m_lastIndex);

    // Iterate until m_curIndex == m_lastIndex
    while (m_curIndex > m_lastIndex)
    {
        // Check if the candidate is the result
        if (isPunctuation(puncCandidate))
        {
            moveCursor(m_curIndex);

            Token value;
            value.type = s_operator[puncCandidate];
            value.name = "";
            value.position = m_lastPos;

            return value;
        }

        // Goes back by one
        m_curIndex--;
        puncCandidate.resize(puncCandidate.size()-1);
    }

    m_curIndex = m_lastIndex;

    // No valid operator
    return {
        .type=TokenType::Invalid,
        .name="",
        .position={.line=m_lastPos.line, .column=m_lastPos.column}
    };
}

Token Lexer::handleOperator()
{
    // Iterate characters until not operator anymore
    while (m_curIndex < m_fileContent.size() && isOperatorChar(m_fileContent[m_curIndex]))
    {
        m_curIndex++;
    }

    // Get the candidate for the operator
    std::string opCandidate = m_fileContent.substr(m_lastIndex, m_curIndex - m_lastIndex);

    // Iterate until m_curIndex == m_lastIndex
    while (m_curIndex > m_lastIndex)
    {
        // Check if the candidate is the result
        if (isOperator(opCandidate))
        {
            moveCursor(m_curIndex);

            Token value;
            value.type = s_operator[opCandidate];
            value.name = "";
            value.position = m_lastPos;

            return value;
        }

        // Goes back by one
        m_curIndex--;
        opCandidate.resize(opCandidate.size()-1);
    }

    m_curIndex = m_lastIndex;

    // No valid operator
    return {
        .type=TokenType::Invalid,
        .name="",
        .position={.line=m_lastPos.line, .column=m_lastPos.column}
    };
}

Token Lexer::getCurToken()
{
    unsigned long fromIndex = findNonSpace(m_curIndex);

    if (fromIndex == m_fileContent.size())
    {
        return {
            .type=TokenType::EndOfFile, 
            .name="", 
            .position{
                .line=m_lastPos.line, 
                .column=m_lastPos.column
            }
        };
    }

    moveCursor(fromIndex);
    
    m_curIndex = m_lastIndex;
    char ch = m_fileContent[m_curIndex];

    if (std::isdigit(ch))
    {
        return handleNumericLiteral();
    }
    if (ch == '\"')
    {
        return handleStringLiteral();
    }
    if (std::isalpha(ch) || ch == '_')
    {
        return handleIdentifier();
    }
    if (isPunctuationChar(ch))
    {
        return handlePunctuation();
    }
    if (isOperatorChar(ch))
    {
        return handleOperator();
    }
    
    return {
        TokenType::Invalid, 
        "", 
        {
            m_lastPos.line, 
            m_lastPos.column
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
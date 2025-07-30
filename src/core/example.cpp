#include "lexer.h"

#include <iostream>
#include <string>
#include <unordered_map>

std::string tokenTypeToString(TokenType type) {
    static const std::unordered_map<TokenType, std::string> typeNames = {
        {TokenType::Invalid, "Invalid"},
        {TokenType::Identifier, "Identifier"},

        // Keywords
        {TokenType::KeywordIf, "KeywordIf"},
        {TokenType::KeywordElse, "KeywordElse"},
        {TokenType::KeywordWhile, "KeywordWhile"},
        {TokenType::KeywordFor, "KeywordFor"},
        {TokenType::KeywordContinue, "KeywordContinue"},
        {TokenType::KeywordBreak, "KeywordBreak"},
        {TokenType::KeywordReturn, "KeywordReturn"},
        {TokenType::KeywordDo, "KeywordDo"},
        {TokenType::KeywordCase, "KeywordCase"},
        {TokenType::KeywordSwitch, "KeywordSwitch"},
        {TokenType::KeywordDefault, "KeywordDefault"},
        {TokenType::KeywordTypedef, "KeywordTypedef"},
        {TokenType::KeywordClass, "KeywordClass"},
        {TokenType::KeywordStruct, "KeywordStruct"},
        {TokenType::KeywordUnion, "KeywordUnion"},
        {TokenType::KeywordEnum, "KeywordEnum"},
        {TokenType::KeywordVoid, "KeywordVoid"},
        {TokenType::KeywordInt, "KeywordInt"},
        {TokenType::KeywordDouble, "KeywordDouble"},
        {TokenType::KeywordFloat, "KeywordFloat"},
        {TokenType::KeywordLong, "KeywordLong"},
        {TokenType::KeywordChar, "KeywordChar"},
        {TokenType::KeywordShort, "KeywordShort"},
        {TokenType::KeywordConst, "KeywordConst"},
        {TokenType::KeywordUnsigned, "KeywordUnsigned"},
        {TokenType::KeywordSigned, "KeywordSigned"},
        {TokenType::KeywordExtern, "KeywordExtern"},

        // Literals
        {TokenType::LiteralInteger, "LiteralInteger"},
        {TokenType::LiteralDouble, "LiteralDouble"},
        {TokenType::LiteralFloat, "LiteralFloat"},
        {TokenType::LiteralChar, "LiteralChar"},
        {TokenType::LiteralString, "LiteralString"},

        // Operators
        {TokenType::OperatorLeftArrow, "Operator<"},
        {TokenType::OperatorRightArrow, "Operator>"},
        {TokenType::OperatorAsterik, "Operator*"},
        {TokenType::OperatorAmpersand, "Operator&"},
        {TokenType::OperatorArrow, "Operator->"},
        {TokenType::OperatorArithmeticAdd, "Operator+"},
        {TokenType::OperatorArithmeticSub, "Operator-"},
        {TokenType::OperatorArithmeticDiv, "Operator/"},
        {TokenType::OperatorArithmeticMod, "Operator%"},
        {TokenType::OperatorRelationalEqual, "Operator=="},
        {TokenType::OperatorRelationalNotEqual, "Operator!="},
        {TokenType::OperatorRelationalBiggerEqual, "Operator>="},
        {TokenType::OperatorRelationalLowerEqual, "Operator<="},
        {TokenType::OperatorLogicalAnd, "Operator&&"},
        {TokenType::OperatorLogicalOr, "Operator||"},
        {TokenType::OperatorLogicalNot, "Operator!"},
        {TokenType::OperatorBitwiseOr, "Operator|"},
        {TokenType::OperatorBitwiseXor, "Operator^"},
        {TokenType::OperatorBitwiseNot, "Operator~"},
        {TokenType::OperatorBitwiseShiftLeft, "Operator<<"},
        {TokenType::OperatorBitwiseShiftRight, "Operator>>"},
        {TokenType::OperatorAssign, "Operator="},
        {TokenType::OperatorAssignAdd, "Operator+="},
        {TokenType::OperatorAssignSub, "Operator-="},
        {TokenType::OperatorAssignMul, "Operator*="},
        {TokenType::OperatorAssignDiv, "Operator/="},
        {TokenType::OperatorAssignMod, "Operator%="},
        {TokenType::OperatorAssignAnd, "Operator&="},
        {TokenType::OperatorAssignOr, "Operator|="},
        {TokenType::OperatorAssignXor, "Operator^="},
        {TokenType::OperatorAssignShiftLeft, "Operator<<="},
        {TokenType::OperatorAssignShiftRight, "Operator>>="},
        {TokenType::OperatorIncrement, "Operator++"},
        {TokenType::OperatorDecrement, "Operator--"},
        {TokenType::OperatorTernary, "Operator?"},

        // Punctuation
        {TokenType::PunctuationBracketLBlock, "Punctuation{"},
        {TokenType::PunctuationBracketRBlock, "Punctuation}"},
        {TokenType::PunctuationBracketLIndex, "Punctuation["},
        {TokenType::PunctuationBracketRIndex, "Punctuation]"},
        {TokenType::PunctuationBracketLCall, "Punctuation("},
        {TokenType::PunctuationBracketRCall, "Punctuation)"},
        {TokenType::PunctuationComma, "Punctuation,"},
        {TokenType::PunctuationSemicolon, "Punctuation;"},
        {TokenType::PunctuationColon, "Punctuation:"},
        {TokenType::PunctuationDoubleColon, "Punctuation::"},
        {TokenType::PunctuationDot, "Punctuation."},
        {TokenType::Punctuation3Dots, "Punctuation..."},

        {TokenType::EndOfFile, "EndOfFile"}
    };

    auto it = typeNames.find(type);
    return (it != typeNames.end()) ? it->second : "UnknownToken";
}

std::string tokenToString(const Token& token) {
    return "Token(" + tokenTypeToString(token.type) + 
           ", name=\"" + token.name + 
           "\", line=" + std::to_string(token.position.line) + 
           ", column=" + std::to_string(token.position.column) + ")";
}

int main(int argc, char* argv[]) 
{
    Lexer lexer("examples/template.qc");
    std::vector<Token> resultTokens = lexer.applyLexer(); 
    for (const Token& token : resultTokens)
    {
        std::cout << "Token: " << tokenToString(token) << std::endl;
    }
}

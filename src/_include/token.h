#include <string>

struct FilePosition
{
    unsigned long line;
    unsigned long column;
};

enum class TokenType : unsigned long
{
    Invalid=0,

    Identifier, // function & var names
    
    // Keywords - if, else, while for return etc..
    KeywordBegin,

    KeywordIf = KeywordBegin, 
    KeywordElse, 
    KeywordWhile, 
    KeywordFor, 
    KeywordContinue, 
    KeywordBreak, 
    KeywordReturn, 
    KeywordDo,
    KeywordCase,
    KeywordSwitch,
    KeywordDefault,
    KeywordTypedef,

    KeywordClass, 
    KeywordStruct, 
    KeywordUnion, 
    KeywordEnum, 
    
    KeywordVoid, 
    KeywordInt, 
    KeywordDouble, 
    KeywordFloat, 
    KeywordLong, 
    KeywordChar, 
    KeywordShort, 
    
    KeywordConst, 
    KeywordUnsigned, 
    KeywordSigned, 
    KeywordExtern, 

    KeywordEnd,

    /// Literals    (will not include negative numbers, up to the parser)
    
    LiteralBegin,
    LiteralInteger=LiteralBegin, // 42
    LiteralDouble, // 3.14, -12.75
    LiteralFloat, // 3.14f, -12.75f
    LiteralChar, // '\n' 'a'
    LiteralString, // "str"

    LiteralEnd,

    /// Operators
    OperatorBegin,
    OperatorLeftArrow=OperatorBegin, // <
    OperatorRightArrow, // >

    // Pointers/Arithemtic/Logical
    OperatorAsterik, // *
    OperatorAmpersand, // &

    // Arithmetic (+,- etc)
    OperatorArithmeticAdd,
    OperatorArithmeticSub,
    OperatorArithmeticDiv,
    OperatorArithmeticMod,
    
    // Relational (==, != etc..)
    OperatorRelationalEqual,
    OperatorRelationalNotEqual,
    OperatorRelationalBiggerEqual,
    OperatorRelationalLowerEqual,
    
    // Logical (&&) 
    OperatorLogicalAnd,
    OperatorLogicalOr,
    OperatorLogicalNot,

    // Bitwise (&, |, ^)
    // OperatorBitwiseAnd, (& is seperate)
    OperatorBitwiseOr,
    OperatorBitwiseXor,
    OperatorBitwiseNot,
    OperatorBitwiseShiftLeft,
    OperatorBitwiseShiftRight,
    // Assignment (=, += ...)
    OperatorAssign,  // =
    OperatorAssignAdd, // +=
    OperatorAssignSub, // -=
    OperatorAssignMul, // *=
    OperatorAssignDiv, // /=
    OperatorAssignMod, // %=
    OperatorAssignAnd, // &=
    OperatorAssignOr, // |=
    OperatorAssignXor, // ^=
    OperatorAssignShiftLeft, // <<=
    OperatorAssignShiftRight, // >>=
    
    OperatorIncrement, // ++
    OperatorDecrement, // --

    OperatorTernary, // b ? t : f
    OperatorEnd,

    // Punctuation
    PunctuationBegin,
    PunctuationBracketLBlock=PunctuationBegin, // {
    PunctuationBracketRBlock, // }
    PunctuationBracketLIndex, // [
    PunctuationBracketRIndex, // ]
    PunctuationBracketLCall, // (
    PunctuationBracketRCall, // )
    
    PunctuationComma, // ,
    PunctuationSemicolon, // ;
    PunctuationColon, // switch case
    PunctuationDoubleColon, // used for enum EnumClass::EnumValue
    PunctuationDot, // .
    Punctuation3Dots, // ...
    PunctuationArrow, // ->

    PunctuationEnd,


    EndOfFile=~0UL,
};

struct Token {
    TokenType type;
    std::string name;

    FilePosition position;
};
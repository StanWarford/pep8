#ifndef ASM_H
#define ASM_H

class Asm
{
public:
    // Lexical tokens
    enum ELexicalTokens
    {
        LT_ADDRESSING_MODE, LT_CHAR_CONSTANT, LT_COMMENT, LT_DEC_CONSTANT, LT_DOT_COMMAND,
        LT_EMPTY, LT_ERROR, LT_HEX_CONSTANT, LT_IDENTIFIER, LT_STRING_CONSTANT, LT_SYMBOL
    };

};

#endif // ASM_H

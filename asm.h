#ifndef ASM_H
#define ASM_H

#include <QRegExp>
#include "code.h"

class Asm
{
public:
    // Lexical tokens
    enum ELexicalToken
    {
        LT_ADDRESSING_MODE, LT_CHAR_CONSTANT, LT_COMMENT, LT_DEC_CONSTANT, LT_DOT_COMMAND,
        LT_EMPTY, LT_HEX_CONSTANT, LT_IDENTIFIER, LT_STRING_CONSTANT, LT_SYMBOL_DEF
    };

    enum ParseState
    {
        PS_ADDRESSING_MODE, PS_CLOSE, PS_COMMENT, PS_DOT_ADDRSS, PS_DOT_ASCII, PS_DOT_BLOCK,
        PS_DOT_BURN, PS_DOT_BYTE, PS_DOT_END, PS_DOT_EQUATE, PS_DOT_WORD, PS_FINISH,
        PS_INSTRUCTION, PS_START, PS_STRING, PS_SYMBOL_DEF
    };

    // Regular expressions
    static QRegExp rxAddrMode;
    static QRegExp rxCharConst;
    static QRegExp rxComment;
    static QRegExp rxDecConst;
    static QRegExp rxDotCommand;
    static QRegExp rxHexConst;
    static QRegExp rxIdentifier;
    static QRegExp rxStringConst;

    static bool getToken(QString &sourceLine, ELexicalToken &token, QString &tokenString);
    // Pre: sourceLine has one line of source code.
    // Post: If the next token is valid, the string of characters representing the next token are deleted from the
    // beginning of sourceLine and returned in tokenString, true is returned, and token is set to the token type.
    // Post: If false is returned, then tokenString is set to the lexical error message.

    static bool processSourceLine(QString sourceLine, Code *&code, QString &errorString);
    // Pre: sourceLine has one line of source code.
    // Post: If the source line is valid, true is returned and code is set to the source code for the line.
    // Post: If the source line is not valid, false is returned and errorString is set to the error message.

    static QList<QString> listOfReferencedSymbols;

    static bool startsWithHexPrefix(QString str);
    static int stringToAddrMode(QString str);
    static int charStringToInt(QString str);

};

#endif // ASM_H

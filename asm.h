#ifndef ASM_H
#define ASM_H

#include <QRegExp>
#include <QStringList>


class Asm
{
public:
    // Lexical tokens
    enum ELexicalToken
    {
        LT_ADDRESSING_MODE, LT_CHAR_CONSTANT, LT_COMMENT, LT_DEC_CONSTANT, LT_DOT_COMMAND,
        LT_EMPTY, LT_ERROR, LT_HEX_CONSTANT, LT_IDENTIFIER, LT_STRING_CONSTANT, LT_SYMBOL
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

    static void getToken(QString &sourceLine, ELexicalToken &token, QString &tokenString);
    // Pre: sourceLine has one line of source code.
    // Post: The string of characters representing the next token are deleted from the
    // beginning ofsourceLine and returned in tokenString.
    // Post: token is set to the token type.
    // Post: If token is set to LT_ERROR, then tokenString is set to the lexical error message.

    static bool startsWithHexPrefix(QString str);

};


#endif // ASM_H

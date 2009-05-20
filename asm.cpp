#include "asm.h"

// Regular expressions
QRegExp Asm::rxAddrMode ("^((,)(\\s*)(i|d|x|n|s(?![fx])|sx(?![f])|sf|sxf){1}){1}");
QRegExp Asm::rxCharConst ("^((\')(?![\'])(([^\'|\\\\]){1}|((\\\\)([\'|b|f|n|r|t|v|\"|\\\\]))|((\\\\)(([x|X])([0-9|A-F|a-f]{2}))))(\'))");
QRegExp Asm::rxComment ("^((;{1})(.)*)");
QRegExp Asm::rxDecConst ("^((([+|-]{0,1})([0-9]+))|^(([1-9])([0-9]*)))");
QRegExp Asm::rxDotCommand ("^((.)(([A-Z|a-z]{1})(\\w)*))");
QRegExp Asm::rxHexConst ("^((0(?![x|X]))|((0)([x|X])([0-9|A-F|a-f])+)|((0)([0-9]+)))");
QRegExp Asm::rxIdentifier ("^((([A-Z|a-z]{1})(\\w*))(:){0,1})");
QRegExp Asm::rxStringConst ("^((\")((([^\"|\\\\])|((\\\\)([\'|b|f|n|r|t|v|\"|\\\\]))|((\\\\)(([x|X])([0-9|A-F|a-f]{2}))))*)(\"))");

void Asm::getToken(QString &sourceLine, ELexicalToken &token, QString &tokenString)
{
    sourceLine = sourceLine.trimmed();
    if (sourceLine.length() == 0) {
        token = LT_EMPTY;
        tokenString = "";
        return;
    }
    QChar firstChar = sourceLine[0];
    rxAddrMode.setCaseSensitivity (Qt::CaseInsensitive);  // Make rxAddrMode not case sensitive
    if (firstChar == ',') {
        if (rxAddrMode.indexIn(sourceLine) == -1) {
            token = LT_ERROR;
            tokenString = ";ERROR: Malformed addressing mode.";
            return;
        }
        token = LT_ADDRESSING_MODE;
        tokenString = rxAddrMode.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return;
    }
    if (firstChar == '\'') {
        if (rxCharConst.indexIn(sourceLine) == -1) {
            token = LT_ERROR;
            tokenString = ";ERROR: Malformed character constant.";
            return;
        }
        token = LT_CHAR_CONSTANT;
        tokenString = rxCharConst.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return;
    }
    if (firstChar == ';') {
        if (rxComment.indexIn(sourceLine) == -1) {
            // This error should not occur, as any characters are allowed in a comment.
            token = LT_ERROR;
            tokenString = ";ERROR: Malformed comment";
            return;
        }
        token = LT_COMMENT;
        tokenString = rxComment.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return;
    }
    if ((firstChar.isDigit() || firstChar == '+' || firstChar == '-') & !startsWithHexPrefix(sourceLine)) {
        if (rxDecConst.indexIn(sourceLine) == -1) {
            token = LT_ERROR;
            tokenString = ";ERROR: Malformed decimal constant.";
            return;
        }
        token = LT_DEC_CONSTANT;
        tokenString = rxDecConst.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return;
    }
    if (firstChar == '.') {
        if (rxDotCommand.indexIn(sourceLine) == -1) {
            token = LT_ERROR;
            tokenString = ";ERROR: Malformed dot command.";
            return;
        }
        token = LT_DOT_COMMAND;
        tokenString = rxDotCommand.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return;
    }
    if (startsWithHexPrefix(sourceLine)) {
        if (rxHexConst.indexIn(sourceLine) == -1) {
            token = LT_ERROR;
            tokenString = ";ERROR: Malformed hex constant.";
            return;
        }
        token = LT_HEX_CONSTANT;
        tokenString = rxHexConst.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return;
    }
    if (firstChar.isLetter() || firstChar == '_') {
        if (rxIdentifier.indexIn(sourceLine) == -1) {
            // This error should not occur, as one-character identifiers are valid.
            token = LT_ERROR;
            tokenString = ";ERROR: Malformed identifier.";
            return;
        }
        tokenString = rxIdentifier.capturedTexts()[0];
        token = tokenString.endsWith(':') ? LT_SYMBOL : LT_IDENTIFIER;
        sourceLine.remove(0, tokenString.length());
        return;
    }
    if (firstChar == '\"') {
        if (rxStringConst.indexIn(sourceLine) == -1) {
            token = LT_ERROR;
            tokenString = ";ERROR: Malformed string constant.";
            return;
        }
        token = LT_STRING_CONSTANT;
        tokenString = rxStringConst.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return;
    }
    token = LT_ERROR;
    tokenString = ";ERROR: Syntax error.";
}

bool Asm::startsWithHexPrefix(QString str)
{
    if (str.length() < 2) return false;
    if (str[0] != '0') return false;
    if (str[1] == 'x' || str[1] == 'X') return true;
    return false;
}

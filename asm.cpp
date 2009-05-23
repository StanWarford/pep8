#include "asm.h"
#include "pep.h"

#include <QDebug>

// Regular expressions for lexical analysis
QRegExp Asm::rxAddrMode ("^((,)(\\s*)(i|d|x|n|s(?![fx])|sx(?![f])|sf|sxf){1}){1}");
QRegExp Asm::rxCharConst ("^((\')(?![\'])(([^\'|\\\\]){1}|((\\\\)([\'|b|f|n|r|t|v|\"|\\\\]))|((\\\\)(([x|X])([0-9|A-F|a-f]{2}))))(\'))");
QRegExp Asm::rxComment ("^((;{1})(.)*)");
QRegExp Asm::rxDecConst ("^((([+|-]{0,1})([0-9]+))|^(([1-9])([0-9]*)))");
QRegExp Asm::rxDotCommand ("^((.)(([A-Z|a-z]{1})(\\w)*))");
QRegExp Asm::rxHexConst ("^((0(?![x|X]))|((0)([x|X])([0-9|A-F|a-f])+)|((0)([0-9]+)))");
QRegExp Asm::rxIdentifier ("^((([A-Z|a-z]{1})(\\w*))(:){0,1})");
QRegExp Asm::rxStringConst ("^((\")((([^\"|\\\\])|((\\\\)([\'|b|f|n|r|t|v|\"|\\\\]))|((\\\\)(([x|X])([0-9|A-F|a-f]{2}))))*)(\"))");

// Regular expressions for trace tag analysis


bool Asm::getToken(QString &sourceLine, ELexicalToken &token, QString &tokenString)
{
    sourceLine = sourceLine.trimmed();
    if (sourceLine.length() == 0) {
        token = LT_EMPTY;
        tokenString = "";
        return true;
    }
    QChar firstChar = sourceLine[0];
    rxAddrMode.setCaseSensitivity (Qt::CaseInsensitive);  // Make rxAddrMode not case sensitive
    if (firstChar == ',') {
        if (rxAddrMode.indexIn(sourceLine) == -1) {
            tokenString = ";ERROR: Malformed addressing mode.";
            return false;
        }
        token = LT_ADDRESSING_MODE;
        tokenString = rxAddrMode.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return true;
    }
    if (firstChar == '\'') {
        if (rxCharConst.indexIn(sourceLine) == -1) {
            tokenString = ";ERROR: Malformed character constant.";
            return false;
        }
        token = LT_CHAR_CONSTANT;
        tokenString = rxCharConst.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return true;
    }
    if (firstChar == ';') {
        if (rxComment.indexIn(sourceLine) == -1) {
            // This error should not occur, as any characters are allowed in a comment.
            tokenString = ";ERROR: Malformed comment";
            return false;
        }
        token = LT_COMMENT;
        tokenString = rxComment.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return true;
    }
    if ((firstChar.isDigit() || firstChar == '+' || firstChar == '-') & !startsWithHexPrefix(sourceLine)) {
        if (rxDecConst.indexIn(sourceLine) == -1) {
            tokenString = ";ERROR: Malformed decimal constant.";
            return false;
        }
        token = LT_DEC_CONSTANT;
        tokenString = rxDecConst.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return true;
    }
    if (firstChar == '.') {
        if (rxDotCommand.indexIn(sourceLine) == -1) {
            tokenString = ";ERROR: Malformed dot command.";
            return false;
        }
        token = LT_DOT_COMMAND;
        tokenString = rxDotCommand.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return true;
    }
    if (startsWithHexPrefix(sourceLine)) {
        if (rxHexConst.indexIn(sourceLine) == -1) {
            tokenString = ";ERROR: Malformed hex constant.";
            return false;
        }
        token = LT_HEX_CONSTANT;
        tokenString = rxHexConst.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return true;
    }
    if (firstChar.isLetter() || firstChar == '_') {
        if (rxIdentifier.indexIn(sourceLine) == -1) {
            // This error should not occur, as one-character identifiers are valid.
            tokenString = ";ERROR: Malformed identifier.";
            return false;
        }
        tokenString = rxIdentifier.capturedTexts()[0];
        token = tokenString.endsWith(':') ? LT_SYMBOL_DEF : LT_IDENTIFIER;
        sourceLine.remove(0, tokenString.length());
        return true;
    }
    if (firstChar == '\"') {
        if (rxStringConst.indexIn(sourceLine) == -1) {
            tokenString = ";ERROR: Malformed string constant.";
            return false;
        }
        token = LT_STRING_CONSTANT;
        tokenString = rxStringConst.capturedTexts()[0];
        sourceLine.remove(0, tokenString.length());
        return true;
    }
    tokenString = ";ERROR: Syntax error.";
    return false;
}

QList<QString> Asm::listOfReferencedSymbols;

bool Asm::startsWithHexPrefix(QString str)
{
    if (str.length() < 2) return false;
    if (str[0] != '0') return false;
    if (str[1] == 'x' || str[1] == 'X') return true;
    return false;
}

int Asm::stringToAddrMode(QString str)
{
    str.chop(1); // Remove the comma.
    str = str.trimmed().toUpper();
    if (str == "I") return Pep::I;
    if (str == "D") return Pep::D;
    if (str == "N") return Pep::N;
    if (str == "S") return Pep::S;
    if (str == "SF") return Pep::SF;
    if (str == "X") return Pep::X;
    if (str == "SX") return Pep::SX;
    if (str == "SXF") return Pep::SXF;
    return Pep::NONE;
}

int Asm::charStringToInt(QString str) {
    return 0;
}

bool Asm::processSourceLine(QString sourceLine, Code *&code, QString &errorString)
{
    Asm::ELexicalToken token; // Passed to getToken.
    QString tokenString; // Passed to getToken.
    QString localSymbolDef = ""; // Saves symbol definition for processing in the following state.
    int byteCount = 0; // Computes symbol value.
    Pep::EMnemonic localEnumMnemonic; // Key to Pep:: table lookups.

    // The concrete code objects asssigned to code.
    UnaryInstruction *unaryInstruction;
    NonUnaryInstruction *nonUnaryInstruction;
    DotAddress *dotAddress;
    DotAscii *dotAscii;
    DotBlock *dotBlock;
    DotBurn *dotBurn;
    DotByte *dotByte;
    DotEnd *dotEnd;
    DotEquate *dotEquate;
    DotWord *dotWord;
    CommentOnly *commentOnly;
    BlankLine *blankLine;

    Pep::memAddrssToAssemblerListing.clear();
    Pep::symbolTable.clear();
    Asm::ParseState state = Asm::PS_START;
    do {
        if (!getToken(sourceLine, token, tokenString)) {
            errorString = tokenString;
            return false;
        }
        switch (state) {
        case Asm::PS_START:
            if (token == Asm::LT_IDENTIFIER){
                if (Pep::mnemonToEnumMap.contains(tokenString.toUpper())) {
                    localEnumMnemonic = Pep::mnemonToEnumMap.value(tokenString);
                    if (Pep::isUnaryMap.value(localEnumMnemonic)) {
                        unaryInstruction = new UnaryInstruction;
                        unaryInstruction->symbolDef = "";
                        unaryInstruction->mnemonic = localEnumMnemonic;
                        code = unaryInstruction;
                        byteCount += 1; // One byte generated for unary instruction.
                        state = Asm::PS_CLOSE;
                    }
                    else {
                        nonUnaryInstruction = new NonUnaryInstruction;
                        nonUnaryInstruction->symbolDef = "";
                        nonUnaryInstruction->mnemonic = localEnumMnemonic;
                        code = nonUnaryInstruction;
                        byteCount += 3; // Three bytes generated for unary instruction.
                        state = Asm::PS_INSTRUCTION;
                    }
                }
                else {
                    errorString = "ERROR: Invalid mnemonic.";
                    return false;
                }
            }
            else if (token == Asm::LT_DOT_COMMAND) {
                tokenString.chop(1); // Remove the period
                tokenString = tokenString.toUpper();
                if (tokenString == "ADDRSS") {
                    dotAddress = new DotAddress;
                    dotAddress->symbolDef = "";
                    code = dotAddress;
                    state = Asm::PS_DOT_ADDRSS;
                }
                else if (tokenString == "ASCII") {
                    dotAscii = new DotAscii;
                    dotAscii->symbolDef = "";
                    code = dotAscii;
                    state = Asm::PS_DOT_ASCII;
                }
                else if (tokenString == "BLOCK") {
                    dotBlock = new DotBlock;
                    dotBlock->symbolDef = "";
                    code = dotBlock;
                    state = Asm::PS_DOT_BLOCK;
                }
                else if (tokenString == "BURN") {
                    dotBurn = new DotBurn;
                    dotBurn->symbolDef = "";
                    code = dotBurn;
                    state = Asm::PS_DOT_BURN;
                }
                else if (tokenString == "BYTE") {
                    dotByte = new DotByte;
                    dotByte->symbolDef = "";
                    code = dotByte;
                    state = Asm::PS_DOT_BYTE;
                }
                else if (tokenString == "END") {
                    dotEnd = new DotEnd;
                    dotEnd->symbolDef = "";
                    code = dotEnd;
                    state = Asm::PS_DOT_END;
                }
                else if (tokenString == "EQUATE") {
                    dotEquate = new DotEquate;
                    dotEquate->symbolDef = "";
                    code = dotEquate;
                    state = Asm::PS_DOT_EQUATE;
                }
                else if (tokenString == "WORD") {
                    dotWord = new DotWord;
                    dotWord->symbolDef = "";
                    code = dotWord;
                    state = Asm::PS_DOT_WORD;
                }
                else {
                    errorString = "ERROR: Invalid dot command.";
                    return false;
                }
            }
            else if (token == Asm::LT_SYMBOL_DEF) {
                tokenString = tokenString.left(tokenString.length() - 1); // Remove the colon
                if (tokenString.length() > 8) {
                    errorString = "ERROR: Symbol " + tokenString + " cannot have more than eight characters.";
                    return false;
                }
                localSymbolDef = tokenString;
                state = Asm::PS_SYMBOL_DEF;
            }
            else if (token == Asm::LT_COMMENT) {
                commentOnly = new CommentOnly;
                commentOnly->comment = tokenString;
                code = commentOnly;
                state = Asm::PS_COMMENT;
            }
            else if (token == Asm::LT_EMPTY) {
                blankLine = new BlankLine;
                code = blankLine;
                state = Asm::PS_FINISH;
            }
            else {
                errorString = "ERROR: Line must start with symbol definition, mnemonic, dot command, or comment.";
                return false;
            }
            break;

        case Asm::PS_SYMBOL_DEF:
            if (token == Asm::LT_IDENTIFIER){
                if (Pep::mnemonToEnumMap.contains(tokenString.toUpper())) {
                    localEnumMnemonic = Pep::mnemonToEnumMap.value(tokenString);
                    if (Pep::isUnaryMap.value(localEnumMnemonic)) {
                        unaryInstruction = new UnaryInstruction;
                        unaryInstruction->symbolDef = localSymbolDef;
                        unaryInstruction->mnemonic = localEnumMnemonic;
                        code = unaryInstruction;
                        byteCount += 1; // One byte generated for unary instruction.
                        state = Asm::PS_CLOSE;
                    }
                    else {
                        nonUnaryInstruction = new NonUnaryInstruction;
                        nonUnaryInstruction->symbolDef = localSymbolDef;
                        nonUnaryInstruction->mnemonic = localEnumMnemonic;
                        code = nonUnaryInstruction;
                        byteCount += 3; // Three bytes generated for unary instruction.
                        state = Asm::PS_INSTRUCTION;
                    }
                }
                else {
                    errorString = "ERROR: Invalid mnemonic.";
                    return false;
                }
            }
            else if (token == Asm::LT_DOT_COMMAND) {
                tokenString.chop(1); // Remove the period
                tokenString = tokenString.toUpper();
                if (tokenString == "ADDRSS") {
                    dotAddress = new DotAddress;
                    dotAddress->symbolDef = localSymbolDef;
                    code = dotAddress;
                    state = Asm::PS_DOT_ADDRSS;
                }
                else if (tokenString == "ASCII") {
                    dotAscii = new DotAscii;
                    dotAscii->symbolDef = localSymbolDef;
                    code = dotAscii;
                    state = Asm::PS_DOT_ASCII;
                }
                else if (tokenString == "BLOCK") {
                    dotBlock = new DotBlock;
                    dotBlock->symbolDef = localSymbolDef;
                    code = dotBlock;
                    state = Asm::PS_DOT_BLOCK;
                }
                else if (tokenString == "BURN") {
                    dotBurn = new DotBurn;
                    dotBurn->symbolDef = localSymbolDef;
                    code = dotBurn;
                    state = Asm::PS_DOT_BURN;
                }
                else if (tokenString == "BYTE") {
                    dotByte = new DotByte;
                    dotByte->symbolDef = localSymbolDef;
                    code = dotByte;
                    state = Asm::PS_DOT_BYTE;
                }
                else if (tokenString == "END") {
                    dotEnd = new DotEnd;
                    dotEnd->symbolDef = localSymbolDef;
                    code = dotEnd;
                    state = Asm::PS_DOT_END;
                }
                else if (tokenString == "EQUATE") {
                    dotEquate = new DotEquate;
                    dotEquate->symbolDef = localSymbolDef;
                    code = dotEquate;
                    state = Asm::PS_DOT_EQUATE;
                }
                else if (tokenString == "WORD") {
                    dotWord = new DotWord;
                    dotWord->symbolDef = localSymbolDef;
                    code = dotWord;
                    state = Asm::PS_DOT_WORD;
                }
                else {
                    errorString = "ERROR: Invalid dot command.";
                    return false;
                }
            }
            else {
                errorString = "ERROR: Must have mnemonic or dot command after symbol definition.";
                return false;
            }
            break;



        case Asm::PS_INSTRUCTION:
            if (token == Asm::LT_IDENTIFIER) {
                if (tokenString.length() > 8) {
                    errorString = "ERROR: Symbol " + tokenString + " cannot have more than eight characters.";
                    return false;
                }
                nonUnaryInstruction->argument = new SymbolRefArgument(tokenString);
                Asm::listOfReferencedSymbols.append(tokenString);
                state = Asm::PS_ADDRESSING_MODE;
            }
            else if (token == Asm::LT_STRING_CONSTANT) {
                if (tokenString.length() > 2) {
                    errorString = "ERROR: String operands must have length at most two.";
                    return false;
                }
                nonUnaryInstruction->argument = new SymbolRefArgument(tokenString);
                state = Asm::PS_ADDRESSING_MODE;
            }
            else if (token == Asm::LT_HEX_CONSTANT) {
                tokenString.chop(2); // Remove "0x" prefix.
                bool ok;
                nonUnaryInstruction->argument = new HexArgument(tokenString.toInt(&ok, 16));
                state = Asm::PS_ADDRESSING_MODE;
            }
            else if (token == Asm::LT_DEC_CONSTANT) {
                bool ok;
                nonUnaryInstruction->argument = new DecArgument(tokenString.toInt(&ok, 10));
                state = Asm::PS_ADDRESSING_MODE;
            }
            else if (token == Asm::LT_CHAR_CONSTANT) {
                nonUnaryInstruction->argument = new CharArgument(tokenString); // Possible problem here. Tricky single quotes.
                state = Asm::PS_ADDRESSING_MODE;
            }
            else {
                errorString = "ERROR: Operand specifier expected after mnemonic.";
                return false;
            }
            break;

        case Asm::PS_ADDRESSING_MODE:
            if (token == Asm::LT_ADDRESSING_MODE) {
                int addrMode = Asm::stringToAddrMode(tokenString);
                if (addrMode & Pep::addrModesMap.value(localEnumMnemonic) == 0) {
                    errorString = "ERROR: Illegal addressing mode for this instruction.";
                    return false;
                }
                nonUnaryInstruction->addressingMode = addrMode;
                state = Asm::PS_CLOSE;
            }
            else if (Pep::addrModeRequiredMap.value(localEnumMnemonic)) {
                errorString = "ERROR: Addressing mode required for this instruction.";
                return false;
            }
            else { // Must be branch type instruction with no addressing mode. Assign default addressing mode.
                nonUnaryInstruction->addressingMode = Pep::I;
                state = Asm::PS_CLOSE;
            }
            break;

        case Asm::PS_DOT_ADDRSS:
            break;
        case Asm::PS_DOT_ASCII:
            break;
        case Asm::PS_DOT_BLOCK:
            break;
        case Asm::PS_DOT_BURN:
            break;
        case Asm::PS_DOT_BYTE:
            break;
        case Asm::PS_DOT_END:
            break;
        case Asm::PS_DOT_EQUATE:
            break;
        case Asm::PS_DOT_WORD:
            break;

        case Asm::PS_CLOSE:
            if (token == Asm::LT_EMPTY) {
                state = Asm::PS_FINISH;
            }
            else if (token == Asm::LT_COMMENT) {
                code->comment = tokenString;
                state = Asm::PS_COMMENT;
            }
            else {
                errorString = "ERROR: Comment expected following instruction.";
                return false;
            }
            break;

        case Asm::PS_COMMENT:
            if (token == Asm::LT_EMPTY) {
                state = Asm::PS_FINISH;
            }
            else {
                // This error should not occur, as all characters are allowed in comments.
                errorString = "ERROR: Problem detected after comment.";
                return false;
            }
            break;

        default:
            break;
        }
    }
    while (state != Asm::PS_FINISH);
    qDebug() << "processSourceLine returning true.";
    return true;
}

// File: code.cpp

#include "code.h"
#include "argument.h"
#include "pep.h"

// appendObjectCode
void UnaryInstruction::appendObjectCode(QList<int> &objectCode)
{
    if ((Pep::burnCount == 0) || ((Pep::burnCount == 1) && (memAddress >= Pep::romStartAddress))) {
        objectCode.append(Pep::opCodeMap.value(mnemonic));
    }
}

void NonUnaryInstruction::appendObjectCode(QList<int> &objectCode)
{
    if ((Pep::burnCount == 0) || ((Pep::burnCount == 1) && (memAddress >= Pep::romStartAddress))) {
        int instructionSpecifier = Pep::opCodeMap.value(mnemonic);
        if (Pep::addrModeRequiredMap.value(mnemonic)) {
            instructionSpecifier += Pep::aaaAddressField(addressingMode);
        }
        else {
            instructionSpecifier += Pep::aAddressField(addressingMode);
        }
        objectCode.append(instructionSpecifier);
        int operandSpecifier = argument->getArgumentValue();
        objectCode.append(operandSpecifier / 256);
        objectCode.append(operandSpecifier % 256);
    }
}

void DotAddrss::appendObjectCode(QList<int> &objectCode)
{
    if ((Pep::burnCount == 0) || ((Pep::burnCount == 1) && (memAddress >= Pep::romStartAddress))) {
        int symbolValue = Pep::symbolTable.value(argument->getArgumentString());
        objectCode.append(symbolValue / 256);
        objectCode.append(symbolValue % 256);
    }
}

void DotAscii::appendObjectCode(QList<int> &objectCode)
{
    if ((Pep::burnCount == 0) || ((Pep::burnCount == 1) && (memAddress >= Pep::romStartAddress))) {
        int value;
        QString str = argument->getArgumentString();
        str.remove(0, 1); // Remove the leftmost double quote.
        str.chop(1); // Remove the rightmost double quote.
        while (str.length() > 0) {
            Asm::unquotedStringToInt(str, value);
            objectCode.append(value);
        }
    }
}

void DotBlock::appendObjectCode(QList<int> &objectCode)
{
    if ((Pep::burnCount == 0) || ((Pep::burnCount == 1) && (memAddress >= Pep::romStartAddress))) {
        for (int i = 0; i < argument->getArgumentValue(); i++) {
            objectCode.append(0);
        }
    }
}

void DotBurn::appendObjectCode(QList<int> &)
{
    // Does not generate code.
}

void DotByte::appendObjectCode(QList<int> &objectCode)
{
    if ((Pep::burnCount == 0) || ((Pep::burnCount == 1) && (memAddress >= Pep::romStartAddress))) {
        objectCode.append(argument->getArgumentValue());
    }
}

void DotEnd::appendObjectCode(QList<int> &)
{
    // Does not generate code.
}

void DotEquate::appendObjectCode(QList<int> &)
{
    // Does not generate code.
}

void DotWord::appendObjectCode(QList<int> &objectCode)
{
    if ((Pep::burnCount == 0) || ((Pep::burnCount == 1) && (memAddress >= Pep::romStartAddress))) {
        int value = argument->getArgumentValue();
        objectCode.append(value / 256);
        objectCode.append(value % 256);
    }
}

void CommentOnly::appendObjectCode(QList<int> &)
{
    // Does not generate code.
}

void BlankLine::appendObjectCode(QList<int> &)
{
    // Does not generate code.
}

void UnaryInstruction::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    QString memStr = QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper();
    QString codeStr = QString("%1").arg(Pep::opCodeMap.value(mnemonic), 2, 16, QLatin1Char('0')).toUpper();;
    if ((Pep::burnCount == 1) && (memAddress < Pep::romStartAddress)) {
        codeStr = "  ";
    }
    QString symbolStr = symbolDef;
    if (symbolStr.length() > 0) {
        symbolStr.append(":");
    }
    QString mnemonStr = Pep::enumToMnemonMap.value(mnemonic);
    QString lineStr = QString("%1%2%3%4%5")
                      .arg(memStr, -6, QLatin1Char(' '))
                      .arg(codeStr, -7, QLatin1Char(' '))
                      .arg(symbolStr, -9, QLatin1Char(' '))
                      .arg(mnemonStr, -8, QLatin1Char(' '))
                      .arg("            " + comment);
    Pep::memAddrssToAssemblerListing.insert(memAddress, assemblerListingList.size());
    Pep::listingRowChecked.insert(assemblerListingList.size(), Qt::Unchecked);
    assemblerListingList.append(lineStr);
    listingTraceList.append(lineStr);
    hasCheckBox.append(true);
}

void NonUnaryInstruction::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    QString memStr = QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper();
    int temp = Pep::opCodeMap.value(mnemonic);
    temp += Pep::addrModeRequiredMap.value(mnemonic) ? Pep::aaaAddressField(addressingMode) : Pep::aAddressField(addressingMode);
    QString codeStr = QString("%1").arg(temp, 2, 16, QLatin1Char('0')).toUpper();
    QString oprndNumStr = QString("%1").arg(argument->getArgumentValue(), 4, 16, QLatin1Char('0')).toUpper();
    if ((Pep::burnCount == 1) && (memAddress < Pep::romStartAddress)) {
        codeStr = "  ";
        oprndNumStr = "    ";
    }
    QString symbolStr = symbolDef;
    if (symbolStr.length() > 0) {
        symbolStr.append(":");
    }
    QString mnemonStr = Pep::enumToMnemonMap.value(mnemonic);
    QString oprndStr = argument->getArgumentString();
    if (Pep::addrModeRequiredMap.value(mnemonic)) {
        oprndStr.append("," + Pep::intToAddrMode(addressingMode));
    }
    else if (addressingMode == Enu::X) {
        oprndStr.append("," + Pep::intToAddrMode(addressingMode));
    }
    QString lineStr = QString("%1%2%3%4%5%6%7")
                      .arg(memStr, -6, QLatin1Char(' '))
                      .arg(codeStr, -2)
                      .arg(oprndNumStr, -5, QLatin1Char(' '))
                      .arg(symbolStr, -9, QLatin1Char(' '))
                      .arg(mnemonStr, -8, QLatin1Char(' '))
                      .arg(oprndStr, -12)
                      .arg(comment);
    Pep::memAddrssToAssemblerListing.insert(memAddress, assemblerListingList.size());
    Pep::listingRowChecked.insert(assemblerListingList.size(), Qt::Unchecked);
    assemblerListingList.append(lineStr);
    listingTraceList.append(lineStr);
    hasCheckBox.append(true);
}

void DotAddrss::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    QString memStr = QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper();
    int symbolValue = Pep::symbolTable.value(argument->getArgumentString());
    QString codeStr = QString("%1").arg(symbolValue, 4, 16, QLatin1Char('0')).toUpper();
    if ((Pep::burnCount == 1) && (memAddress < Pep::romStartAddress)) {
        codeStr = "    ";
    }
    QString symbolStr = symbolDef;
    if (symbolStr.length() > 0) {
        symbolStr.append(":");
    }
    QString dotStr = ".ADDRSS";
    QString oprndStr = argument->getArgumentString();
    QString lineStr = QString("%1%2%3%4%5%6")
                      .arg(memStr, -6, QLatin1Char(' '))
                      .arg(codeStr, -7, QLatin1Char(' '))
                      .arg(symbolStr, -9, QLatin1Char(' '))
                      .arg(dotStr, -8, QLatin1Char(' '))
                      .arg(oprndStr, -12)
                      .arg(comment);
    assemblerListingList.append(lineStr);
    listingTraceList.append(lineStr);
    hasCheckBox.append(false);
}

void DotAscii::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    QString memStr = QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper();
    QString str = argument->getArgumentString();
    str.remove(0, 1); // Remove the leftmost double quote.
    str.chop(1); // Remove the rightmost double quote.
    int value;
    QString codeStr = "";
    while ((str.length() > 0) && (codeStr.length() < 6)) {
        Asm::unquotedStringToInt(str, value);
        codeStr.append(QString("%1").arg(value, 2, 16, QLatin1Char('0')).toUpper());
    }
    if ((Pep::burnCount == 1) && (memAddress < Pep::romStartAddress)) {
        codeStr = "      ";
    }
    QString symbolStr = symbolDef;
    if (symbolStr.length() > 0) {
        symbolStr.append(":");
    }
    QString dotStr = ".ASCII";
    QString oprndStr = argument->getArgumentString();
    QString lineStr = QString("%1%2%3%4%5%6")
                      .arg(memStr, -6, QLatin1Char(' '))
                      .arg(codeStr, -7, QLatin1Char(' '))
                      .arg(symbolStr, -9, QLatin1Char(' '))
                      .arg(dotStr, -8, QLatin1Char(' '))
                      .arg(oprndStr, -12)
                      .arg(comment);
    assemblerListingList.append(lineStr);
    listingTraceList.append(lineStr);
    hasCheckBox.append(false);
    if ((Pep::burnCount == 0) || ((Pep::burnCount == 1) && (memAddress >= Pep::romStartAddress))) {
        while (str.length() > 0) {
            codeStr = "";
            while ((str.length() > 0) && (codeStr.length() < 6)) {
                Asm::unquotedStringToInt(str, value);
                codeStr.append(QString("%1").arg(value, 2, 16, QLatin1Char('0')).toUpper());
            }
            lineStr = QString("      %1").arg(codeStr, -7, QLatin1Char(' '));
            assemblerListingList.append(lineStr);
            listingTraceList.append(lineStr);
            hasCheckBox.append(false);
        }
    }
}

void DotBlock::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    QString memStr = QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper();
    int numBytes = argument->getArgumentValue();
    QString codeStr = "";
    while ((numBytes > 0) && (codeStr.length() < 6)) {
        codeStr.append("00");
        numBytes--;
    }
    if ((Pep::burnCount == 1) && (memAddress < Pep::romStartAddress)) {
        codeStr = "      ";
    }
    QString symbolStr = symbolDef;
    if (symbolStr.length() > 0) {
        symbolStr.append(":");
    }
    QString dotStr = ".BLOCK";
    QString oprndStr = argument->getArgumentString();
    QString lineStr = QString("%1%2%3%4%5%6")
                      .arg(memStr, -6, QLatin1Char(' '))
                      .arg(codeStr, -7, QLatin1Char(' '))
                      .arg(symbolStr, -9, QLatin1Char(' '))
                      .arg(dotStr, -8, QLatin1Char(' '))
                      .arg(oprndStr, -12)
                      .arg(comment);
    assemblerListingList.append(lineStr);
    listingTraceList.append(lineStr);
    hasCheckBox.append(false);
    if ((Pep::burnCount == 0) || ((Pep::burnCount == 1) && (memAddress >= Pep::romStartAddress))) {
        while (numBytes > 0) {
            codeStr = "";
            while ((numBytes > 0) && (codeStr.length() < 6)) {
                codeStr.append("00");
                numBytes--;
            }
            lineStr = QString("      %1").arg(codeStr, -7, QLatin1Char(' '));
            assemblerListingList.append(lineStr);
            listingTraceList.append(lineStr);
            hasCheckBox.append(false);
        }
    }
}

void DotBurn::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    QString memStr = QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper();
    QString symbolStr = symbolDef;
    if (symbolStr.length() > 0) {
        symbolStr.append(":");
    }
    QString dotStr = ".BURN";
    QString oprndStr = argument->getArgumentString();
    QString lineStr = QString("%1       %2%3%4%5")
                      .arg(memStr, -6, QLatin1Char(' '))
                      .arg(symbolStr, -9, QLatin1Char(' '))
                      .arg(dotStr, -8, QLatin1Char(' '))
                      .arg(oprndStr, -12)
                      .arg(comment);
    assemblerListingList.append(lineStr);
    listingTraceList.append(lineStr);
    hasCheckBox.append(false);
}

void DotByte::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    QString memStr = QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper();
    QString codeStr = QString("%1").arg(argument->getArgumentValue(), 2, 16, QLatin1Char('0')).toUpper();
    if ((Pep::burnCount == 1) && (memAddress < Pep::romStartAddress)) {
        codeStr = "  ";
    }
    QString symbolStr = symbolDef;
    if (symbolStr.length() > 0) {
        symbolStr.append(":");
    }
    QString dotStr = ".BYTE";
    QString oprndStr = argument->getArgumentString();
    QString lineStr = QString("%1%2%3%4%5%6")
                      .arg(memStr, -6, QLatin1Char(' '))
                      .arg(codeStr, -7, QLatin1Char(' '))
                      .arg(symbolStr, -9, QLatin1Char(' '))
                      .arg(dotStr, -8, QLatin1Char(' '))
                      .arg(oprndStr, -12)
                      .arg(comment);
    assemblerListingList.append(lineStr);
    listingTraceList.append(lineStr);
    hasCheckBox.append(false);
}

void DotEnd::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    QString memStr = QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper();
    QString symbolStr = symbolDef;
    if (symbolStr.length() > 0) {
        symbolStr.append(":");
    }
    QString dotStr = ".END";
    QString lineStr = QString("%1       %2%3              %4")
                      .arg(memStr, -6, QLatin1Char(' '))
                      .arg(symbolStr, -9, QLatin1Char(' '))
                      .arg(dotStr, -8, QLatin1Char(' '))
                      .arg(comment);
    assemblerListingList.append(lineStr);
    listingTraceList.append(lineStr);
    hasCheckBox.append(false);
}

void DotEquate::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    QString symbolStr = symbolDef;
    if (symbolStr.length() > 0) {
        symbolStr.append(":");
    }
    QString dotStr = ".EQUATE";
    QString oprndStr = argument->getArgumentString();
    QString lineStr = QString("             %1%2%3%4")
                      .arg(symbolStr, -9, QLatin1Char(' '))
                      .arg(dotStr, -8, QLatin1Char(' '))
                      .arg(oprndStr, -12)
                      .arg(comment);
    assemblerListingList.append(lineStr);
    listingTraceList.append(lineStr);
    hasCheckBox.append(false);
}

void DotWord::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    QString memStr = QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper();
    QString codeStr = QString("%1").arg(argument->getArgumentValue(), 4, 16, QLatin1Char('0')).toUpper();
    if ((Pep::burnCount == 1) && (memAddress < Pep::romStartAddress)) {
        codeStr = "    ";
    }
    QString symbolStr = symbolDef;
    if (symbolStr.length() > 0) {
        symbolStr.append(":");
    }
    QString dotStr = ".WORD";
    QString oprndStr = argument->getArgumentString();
    QString lineStr = QString("%1%2%3%4%5%6")
                      .arg(memStr, -6, QLatin1Char(' '))
                      .arg(codeStr, -7, QLatin1Char(' '))
                      .arg(symbolStr, -9, QLatin1Char(' '))
                      .arg(dotStr, -8, QLatin1Char(' '))
                      .arg(oprndStr, -12)
                      .arg(comment);
    assemblerListingList.append(lineStr);
    listingTraceList.append(lineStr);
    hasCheckBox.append(false);
}

void CommentOnly::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    assemblerListingList.append("             " + comment);
    listingTraceList.append("             " + comment);
    hasCheckBox.append(false);
}

void BlankLine::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    assemblerListingList.append("");
    listingTraceList.append("");
    hasCheckBox.append(false);
}

bool DotBlock::processFormatTraceTags(int &sourceLine, QString &errorString) {
    if (symbolDef.size() == 0) {
        return true;
    }
    errorString = ";WARNING: This is a test.";
    sourceLine = sourceCodeLine;
    return true;
}

bool DotEquate::processFormatTraceTags(int &sourceLine, QString &errorString) {
    if (symbolDef.size() == 0) {
        return true;
    }
    errorString = ";WARNING: This is a test.";
    sourceLine = sourceCodeLine;
    return true;
}

bool NonUnaryInstruction::processSymbolTraceTags(int &sourceLine, QString &errorString) {
    switch (mnemonic) {
    case Enu::ADDSP:
        errorString = ";WARNING: This is a test.";
        sourceLine = sourceCodeLine;
        return false;
    case Enu::SUBSP:
        errorString = ";WARNING: This is a test.";
        sourceLine = sourceCodeLine;
        return false;
    case Enu::RET0:
        break;
    case Enu::RET1:
        break;
    case Enu::RET2:
        break;
    case Enu::RET3:
        break;
    case Enu::RET4:
        break;
    case Enu::RET5:
        break;
    case Enu::RET6:
        break;
    case Enu::RET7:
        break;
    default:
        return true;
    }
    return true;
}

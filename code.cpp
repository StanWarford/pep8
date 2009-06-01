// File: code.cpp

#include "code.h"
#include "argument.h"

// appendObjectCode
void UnaryInstruction::appendObjectCode(QList<int> &objectCode)
{
    objectCode.append(Pep::opCodeMap.value(mnemonic));
}

void NonUnaryInstruction::appendObjectCode(QList<int> &objectCode)
{
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

void DotAddress::appendObjectCode(QList<int> &objectCode)
{
    int operandSpecifier = Pep::symbolTable.value(argument->getArgumentString());
    objectCode.append(operandSpecifier / 256);
    objectCode.append(operandSpecifier % 256);
}

void DotAscii::appendObjectCode(QList<int> &objectCode)
{
    int value;
    QString str = argument->getArgumentString();
    str.remove(0, 1); // Remove the leftmost double quote.
    str.chop(1); // Remove the rightmost double quote.
    while (str.length() > 0) {
        Asm::unquotedStringToInt(str, value);
        objectCode.append(value);
    }
}

void DotBlock::appendObjectCode(QList<int> &objectCode)
{
    for (int i = 0; i < argument->getArgumentValue(); i++) {
        objectCode.append(0);
    }
}

void DotBurn::appendObjectCode(QList<int> &)
{
    // Does not generate code.
}

void DotByte::appendObjectCode(QList<int> &objectCode)
{
    objectCode.append(argument->getArgumentValue());
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
    int value = argument->getArgumentValue();
    objectCode.append(value / 256);
    objectCode.append(value % 256);
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
    QString codeStr = QString("%1").arg(Pep::opCodeMap.value(mnemonic), 2, 16, QLatin1Char('0')).toUpper();
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
                      .arg("              " + comment);
    assemblerListingList.append(lineStr);
//    protected:
//    int memAddress;
//    QString symbolDef;
//    QString comment;
//
//    assemblerListingList.append(comment);
//    listingTraceList.append(comment);
//    hasCheckBox.append(false);

}

void NonUnaryInstruction::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    QString memStr = QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper();
    int temp = Pep::opCodeMap.value(mnemonic);
    temp += Pep::addrModeRequiredMap.value(mnemonic) ? Pep::aaaAddressField(addressingMode) : Pep::aAddressField(addressingMode);
    QString codeStr = QString("%1").arg(temp, 2, 16, QLatin1Char('0')).toUpper();
    QString oprndNumStr = QString("%1").arg(argument->getArgumentValue(), 4, 16, QLatin1Char('0')).toUpper();
    QString symbolStr = symbolDef;
    if (symbolStr.length() > 0) {
        symbolStr.append(":");
    }
    QString mnemonStr = Pep::enumToMnemonMap.value(mnemonic);
    QString oprndStr = argument->getArgumentString();
    if (Pep::addrModeRequiredMap.value(mnemonic)) {
        oprndStr.append(Pep::commaPrefixedMode(addressingMode));
    }
    QString lineStr = QString("%1%2%3%4%5%6%7")
                      .arg(memStr, -6, QLatin1Char(' '))
                      .arg(codeStr, -2)
                      .arg(oprndNumStr, -5, QLatin1Char(' '))
                      .arg(symbolStr, -9, QLatin1Char(' '))
                      .arg(mnemonStr, -8, QLatin1Char(' '))
                      .arg(oprndStr, -14)
                      .arg(comment);
    assemblerListingList.append(lineStr);
}

void DotAddress::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{
    QString memStr = QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper();
}

void DotAscii::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{

}

void DotBlock::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{

}

void DotBurn::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{

}

void DotByte::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{

}

void DotEnd::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{

}

void DotEquate::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{

}

void DotWord::appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox)
{

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

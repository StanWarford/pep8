// File: code.cpp

#include "code.h"
#include "argument.h"

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

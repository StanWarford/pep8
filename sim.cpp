#include "sim.h"
#include "pep.h"
#include <QMessageBox>
#include <QDebug>

using namespace Enu;

// The machine
QVector<int> Sim::Mem(65536);
bool Sim::nBit, Sim::zBit, Sim::vBit, Sim::cBit;
int Sim::accumulator;
int Sim::indexRegister;
int Sim::stackPointer;
int Sim::programCounter;
int Sim::instructionSpecifier;
int Sim::operandSpecifier;

QString Sim::inputBuffer;
QString Sim::outputBuffer;

QList<int> Sim::changedMemAddrss;

Enu::EExecState Sim::executionState;

int Sim::toSignedDecimal(int value)
{
    return value > 32767 ? value - 65536 : value;
}

int Sim::fromSignedDecimal(int value)
{
    return value < 0 ? value + 65536 : value;
}

void Sim::loadMem(QList<int> objectCodeList) {
    for (int i = 0; objectCodeList.length() > 0; i++) {
        Mem[i] = objectCodeList.takeAt(0);
    }
}

int Sim::add(int lhs, int rhs)
{
    return (lhs + rhs) % 65536;
}

int Sim::addAndSetNZVC(int lhs, int rhs)
{
    int result = lhs + rhs;    
    if (result >= 65536) {
        Sim::cBit = 1;
        result = result % 65536;
    }
    else {
        Sim::cBit = 0;
    }
    Sim::nBit = result < 32768 ?  false : true;
    Sim::zBit = result == 0 ? true : false;
    Sim::vBit = (lhs < 32768 && rhs < 32768 && result >= 32768) ||
                (lhs >= 32768 && rhs <= 32768 && result < 32768);
    return result;
}

int Sim::readByte(int memAddr)
{
    return Mem[memAddr % 65536];
}

int Sim::readWord(int memAddr)
{
    return 256 * Mem[memAddr % 65536] + Mem[(memAddr + 1) % 65536];
}

int Sim::readByteOprnd(Enu::EAddrMode addrMode)
{
    switch (addrMode) {
    case Enu::NONE:
        break;
    case Enu::I:
        return operandSpecifier;
    case Enu::D:
        return readByte(operandSpecifier);
    case Enu::N:
        return readByte(readWord(operandSpecifier));
    case Enu::S:
        return readByte(add(stackPointer, operandSpecifier));
    case Enu::SF:
        return readByte(readWord(add(stackPointer, operandSpecifier)));
    case Enu::X:
        return readByte(add(operandSpecifier, indexRegister));
    case Enu::SX:
        return readByte(add(add(stackPointer, operandSpecifier), indexRegister));
    case Enu::SXF:
        return readByte(add(readWord(add(stackPointer, operandSpecifier)), indexRegister));
    case Enu::ALL:
        break;
    }
    return 0;
}

int Sim::readWordOprnd(Enu::EAddrMode addrMode)
{
    switch (addrMode) {
    case Enu::NONE:
        break;
    case Enu::I:
        return operandSpecifier;
    case Enu::D:
        return readWord(operandSpecifier);
    case Enu::N:
        return readWord(readWord(operandSpecifier));
    case Enu::S:
        return readWord(add(stackPointer, operandSpecifier));
    case Enu::SF:
        return readWord(readWord(add(stackPointer, operandSpecifier)));
    case Enu::X:
        return readWord(add(operandSpecifier, indexRegister));
    case Enu::SX:
        return readWord(add(add(stackPointer, operandSpecifier), indexRegister));
    case Enu::SXF:
        return readWord(add(readWord(add(stackPointer, operandSpecifier)), indexRegister));
    case Enu::ALL:
        break;
    }
    return 0;
}

void Sim::writeByte(int memAddr, int value)
{
    Mem[memAddr % 65536] = value;
    changedMemAddrss.append(memAddr % 65536);
}

void Sim::writeWord(int memAddr, int value)
{
    Mem[memAddr % 65536] = value / 256;
    changedMemAddrss.append(memAddr % 65536);
    Mem[(memAddr + 1) % 65536] = value % 256;
    changedMemAddrss.append((memAddr + 1) % 65536);
}

void Sim::writeByteOprnd(Enu::EAddrMode addrMode, int value)
{
    switch (addrMode) {
    case Enu::NONE:
        break;
    case Enu::I:
        // illegal
        break;
    case Enu::D:
        writeByte(operandSpecifier, value);
        break;
    case Enu::N:
        writeByte(readWord(operandSpecifier), value);
        break;
    case Enu::S:
        writeByte(add(stackPointer, operandSpecifier), value);
        break;
    case Enu::SF:
        writeByte(readWord(add(stackPointer, operandSpecifier)), value);
        break;
    case Enu::X:
        writeByte(add(operandSpecifier, indexRegister), value);
        break;
    case Enu::SX:
        writeByte(add(add(stackPointer, operandSpecifier), indexRegister), value);
        break;
    case Enu::SXF:
        writeByte(add(readWord(add(stackPointer, operandSpecifier)), indexRegister), value);
        break;
    case Enu::ALL:
        break;
    }
}

void Sim::writeWordOprnd(Enu::EAddrMode addrMode, int value)
{
    switch (addrMode) {
    case Enu::NONE:
        break;
    case Enu::I:
        // illegal
        break;
    case Enu::D:
        writeWord(operandSpecifier, value);
        break;
    case Enu::N:
        writeWord(readWord(operandSpecifier), value);
        break;
    case Enu::S:
        writeWord(add(stackPointer, operandSpecifier), value);
        break;
    case Enu::SF:
        writeWord(readWord(add(stackPointer, operandSpecifier)), value);
        break;
    case Enu::X:
        writeWord(add(operandSpecifier, indexRegister), value);
        break;
    case Enu::SX:
        writeWord(add(add(stackPointer, operandSpecifier), indexRegister), value);
        break;
    case Enu::SXF:
        writeWord(add(readWord(add(stackPointer, operandSpecifier)), indexRegister), value);
        break;
    case Enu::ALL:
        break;
    }
}

bool Sim::vonNeumannStep(QString &errorString)
{
    changedMemAddrss.clear();
    EMnemonic mnemonic;
    int operand;
    EAddrMode addrMode;
//    int result;
    // Fetch
    instructionSpecifier = readByte(programCounter);
    // Increment
    programCounter = add(programCounter, 1);
    // Decode
    mnemonic = Pep::decodeMnemonic[instructionSpecifier];
    addrMode = Pep::decodeAddrMode[Sim::instructionSpecifier];
    if (!Pep::isUnaryMap[mnemonic]) {
        operandSpecifier = readWord(programCounter);
        programCounter = add(programCounter, 2);
    }
    // Execute
    //qDebug() << Pep::enumToMnemonMap[Pep::decodeMnemonic[instructionSpecifier]];
    if (!Pep::isUnaryMap[mnemonic] && !Pep::isTrapMap[mnemonic] && !(Pep::addrModesMap.value(mnemonic) & addrMode )) {
        errorString = "Invalid Addressing Mode.";
        return false;
    }

    switch (Pep::decodeMnemonic[instructionSpecifier]) {
    case ADDA:
        operand = readWordOprnd(addrMode);
        accumulator = addAndSetNZVC(accumulator, operand);
        return true;
    case ADDSP:
        operand = readWordOprnd(addrMode);
        stackPointer = addAndSetNZVC(stackPointer, operand);
        return true;
    case ADDX:
        operand = readWordOprnd(addrMode);
        indexRegister = addAndSetNZVC(indexRegister, operand);
        return true;
    case ANDA:
        accumulator = accumulator & operandSpecifier;
        nBit = accumulator > 32768;
        zBit = accumulator == 0;
        return true;
    case ANDX:
        indexRegister = indexRegister & operandSpecifier;
        nBit = accumulator > 32768;
        zBit = accumulator == 0;
        return true;
    case ASLA:
        vBit = (accumulator >= 0x4000 && accumulator < 0x8000) || // prefix is 01 (bin)
                    (accumulator >= 0x8000 && accumulator < 0xC000); // prefix is 10 (bin)
        accumulator *= 2;
        if (accumulator >= 65536) {
            cBit = 1;
            accumulator = accumulator % 65536;
        }
        else {
            cBit = 0;
        }
        nBit = accumulator >= 32768;
        zBit = accumulator == 0;
        return true;
    case ASLX:
        vBit = (indexRegister >= 0x4000 && indexRegister < 0x8000) || // prefix is 01 (bin)
                    (indexRegister >= 0x8000 && indexRegister < 0xC000); // prefix is 10 (bin)
        indexRegister *= 2;
        if (indexRegister >= 65536) {
            cBit = 1;
            indexRegister = indexRegister % 65536;
        }
        else {
            cBit = 0;
        }
        nBit = indexRegister >= 32768;
        zBit = indexRegister == 0;
        return true;
    case ASRA:
        cBit = (accumulator % 2) == 1;
        if (accumulator < 32768) {
            accumulator /= 2;
        }
        else {
            accumulator = accumulator / 2 + 32768;
        }
        nBit = accumulator >= 32768;
        zBit = accumulator == 0;
        return true;
    case ASRX:
        cBit = (indexRegister % 2) == 1;
        if (indexRegister < 32768) {
            indexRegister /= 2;
        }
        else {
            indexRegister = indexRegister / 2 + 32768;
        }
        nBit = indexRegister >= 32768;
        zBit = indexRegister == 0;
        return true;
    case BR:
        operand = readWordOprnd(addrMode);
        programCounter = operand;
        return true;
    case BRC:
        operand = readWordOprnd(addrMode);
        if (cBit) {
            programCounter = operand;
        }
        return true;
    case BREQ:
        operand = readWordOprnd(addrMode);
        if (zBit) {
            programCounter = operand;
        }
        return true;
    case BRGE:
        operand = readWordOprnd(addrMode);
        if (!nBit) {
            programCounter = operand;
        }
        return true;
    case BRGT:
        operand = readWordOprnd(addrMode);
        if (!nBit && !zBit) {
            programCounter = operand;
        }
        return true;
    case BRLE:
        operand = readWordOprnd(addrMode);
        if (nBit || zBit) {
            programCounter = operand;
        }
        return true;
    case BRLT:
        operand = readWordOprnd(addrMode);
        if (nBit) {
            programCounter = operand;
        }
        return true;
    case BRNE:
        operand = readWordOprnd(addrMode);
        if (!zBit) {
            programCounter = operand;
        }
        return true;
    case BRV:
        operand = readWordOprnd(addrMode);
        if (vBit) {
            programCounter = operand;
        }
        return true;
    case CALL:
        operand = readWordOprnd(addrMode);
        stackPointer = add(stackPointer, 65534); // SP <- SP - 2
        writeWord(stackPointer, programCounter); // Mem[SP] <- PC
        programCounter = operand; // PC <- Oprnd
        return true;
    case CHARI:
        if (Sim::inputBuffer.size() != 0) {
            QString ch = Sim::inputBuffer.left(1);
            Sim::inputBuffer.remove(0, 1);
            Sim::writeByteOprnd(addrMode, QChar(ch[0]).toAscii());
        }
        else {
            errorString = "Error: CHARI executed past end of input.";
            return false;
        }
        return true;
    case CHARO:
        operand = readByteOprnd(addrMode);
        Sim::outputBuffer = QString(operand);
        return true;
    case CPA:
        return true;
    case CPX:
        return true;
    case DECI:
        return true;
    case DECO:
        return true;
    case LDA:
        operand = readWordOprnd(addrMode);
        accumulator = operand % 65536;
        nBit = accumulator >= 32768;
        zBit = accumulator == 0;
        return true;
    case LDBYTEA:
        operand = readByteOprnd(addrMode);
        accumulator = accumulator & 0xFF00;
        accumulator |= operand;
        nBit = accumulator >= 32768;
        zBit = accumulator == 0;
        return true;
    case LDBYTEX:
        operand = readByteOprnd(addrMode);
        indexRegister = indexRegister & 0xFF00;
        indexRegister |= operand;
        nBit = indexRegister >= 32768;
        zBit = indexRegister == 0;
        return true;
    case LDX:
        operand = readWordOprnd(addrMode);
        indexRegister = operand % 65536;
        nBit = indexRegister >= 32768;
        zBit = indexRegister == 0;
        return true;
    case MOVFLGA:
        accumulator = 0;
        accumulator += cBit ? 1 : 0;
        accumulator += vBit ? 2 : 0;
        accumulator += zBit ? 4 : 0;
        accumulator += nBit ? 8 : 0;
        return true;
    case MOVSPA:
        stackPointer = accumulator;
        return true;
    case NEGA:
        accumulator = (~accumulator + 1) & 65535;
        nBit = accumulator >= 32768;
        zBit = accumulator == 0;
        return true;
    case NEGX:
        indexRegister = (~indexRegister + 1) & 65535;
        nBit = indexRegister >= 32768;
        zBit = indexRegister == 0;
        return true;
    case NOP:
    case NOP0:
    case NOP1:
    case NOP2:
    case NOP3:

        return true;
    case NOTA:
        accumulator = ~accumulator & 65535;
        nBit = accumulator >= 32768;
        zBit = accumulator == 0;
        return true;
    case NOTX:
        indexRegister = ~indexRegister & 65535;
        nBit = indexRegister >= 32768;
        zBit = indexRegister == 0;
        return true;
    case ORA:
        accumulator = accumulator | operandSpecifier;
        nBit = accumulator > 32768;
        zBit = accumulator == 0;
        return true;
    case ORX:
        indexRegister = indexRegister | operandSpecifier;
        nBit = indexRegister > 32768;
        zBit = indexRegister == 0;
        return true;
    case RET0:
        programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET1:
        stackPointer = add(stackPointer, 1); // SP <- SP + 1
        programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET2:
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET3:
        stackPointer = add(stackPointer, 3); // SP <- SP + 3
        programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET4:
        stackPointer = add(stackPointer, 4); // SP <- SP + 4
        programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET5:
        stackPointer = add(stackPointer, 5); // SP <- SP + 5
        programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET6:
        stackPointer = add(stackPointer, 6); // SP <- SP + 6
        programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET7:
        stackPointer = add(stackPointer, 7); // SP <- SP + 7
        programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RETTR:
        return true;
    case ROLA:
        return true;
    case ROLX:
        return true;
    case RORA:
        return true;
    case RORX:
        return true;
    case STA:
        writeWordOprnd(addrMode, accumulator);
        return true;
    case STBYTEA:
        operand = readByteOprnd(addrMode);
        accumulator = accumulator & 0xFF00;
        accumulator |= operand;
        return true;
    case STBYTEX:
        operand = readByteOprnd(addrMode);
        indexRegister = indexRegister & 0xFF00;
        indexRegister |= operand;
        return true;
    case STOP:
        return true;
    case STRO:
        return true;
    case STX:
        operand = readWordOprnd(addrMode);
        indexRegister = operand;
        return true;
    case SUBA:
        operand = readWordOprnd(addrMode);
        accumulator = addAndSetNZVC(accumulator, (~operand + 1) & 0xFFFF);
        return true;
    case SUBSP:
        operand = readWordOprnd(addrMode);
        stackPointer = addAndSetNZVC(stackPointer, (~operand + 1) & 0xFFFF);
        return true;
    case SUBX:
        operand = readWordOprnd(addrMode);
        indexRegister = addAndSetNZVC(indexRegister, (~operand + 1) & 0xFFFF);
        return true;
    default:
        qDebug() << "Invalid instruction?!";
    }
    return false;
}





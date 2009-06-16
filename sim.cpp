#include "sim.h"
#include "pep.h"
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
}

void Sim::writeWord(int memAddr, int value)
{
    Mem[memAddr % 65536] = value / 256;
    Mem[(memAddr + 1) % 65536] = value % 256;
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
        writeByte(readWord(operandSpecifier), value);
        break;
    case Enu::N:
        writeByte(readWord(readWord(operandSpecifier)), value);
        break;
    case Enu::S:
        writeByte(readWord(add(stackPointer, operandSpecifier)), value);
        break;
    case Enu::SF:
        writeByte(readWord(readWord(add(stackPointer, operandSpecifier))), value);
        break;
    case Enu::X:
        writeByte(readWord(add(operandSpecifier, indexRegister)), value);
        break;
    case Enu::SX:
        writeByte(readWord(add(add(stackPointer, operandSpecifier), indexRegister)), value);
        break;
    case Enu::SXF:
        writeByte(readWord(add(readWord(add(stackPointer, operandSpecifier)), indexRegister)), value);
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
        writeWord(readWord(operandSpecifier), value);
        break;
    case Enu::N:
        writeWord(readWord(readWord(operandSpecifier)), value);
        break;
    case Enu::S:
        writeWord(readWord(add(stackPointer, operandSpecifier)), value);
        break;
    case Enu::SF:
        writeWord(readWord(readWord(add(stackPointer, operandSpecifier))), value);
        break;
    case Enu::X:
        writeWord(readWord(add(operandSpecifier, indexRegister)), value);
        break;
    case Enu::SX:
        writeWord(readWord(add(add(stackPointer, operandSpecifier), indexRegister)), value);
        break;
    case Enu::SXF:
        writeWord(readWord(add(readWord(add(stackPointer, operandSpecifier)), indexRegister)), value);
        break;
    case Enu::ALL:
        break;
    }
}

bool Sim::vonNeumannStep()
{
    EMnemonic mnemonic;
    int operand;
    EAddrMode addrMode;
    int result;
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
    // qDebug() << Pep::enumToMnemonMap[Pep::decodeMnemonic[instructionSpecifier]];
    if (!(Pep::addrModesMap.value(mnemonic) & addrMode)) {
        return false;
    }

    switch (Pep::decodeMnemonic[instructionSpecifier]) {
    case ADDA:
        operand = Sim::readWordOprnd(addrMode);
        Sim::accumulator = addAndSetNZVC(accumulator, operand);
        return true;
    case ADDSP:
        operand = Sim::readWordOprnd(addrMode);
        Sim::stackPointer = addAndSetNZVC(stackPointer, operand);
        return true;
    case ADDX:
        operand = Sim::readWordOprnd(addrMode);
        Sim::indexRegister = addAndSetNZVC(indexRegister, operand);
        return true;
    case ANDA:
        Sim::accumulator = Sim::accumulator & Sim::operandSpecifier;
        Sim::nBit = Sim::accumulator > 32768;
        Sim::zBit = Sim::accumulator == 0;
        return true;
    case ANDX:
        Sim::indexRegister = Sim::indexRegister & Sim::operandSpecifier;
        Sim::nBit = Sim::accumulator > 32768;
        Sim::zBit = Sim::accumulator == 0;
        return true;
    case ASLA:
        Sim::vBit = (Sim::accumulator >= 16384 && Sim::accumulator < 32768) ||
                    (Sim::accumulator >= 32768 && Sim::accumulator < 49152);
        Sim::accumulator *= 2;
        if (Sim::accumulator >= 65536) {
            Sim::cBit = 1;
            Sim::accumulator = Sim::accumulator % 65536;
        }
        else {
            Sim::cBit = 0;
        }
        Sim::nBit = Sim::accumulator >= 32768;
        Sim::zBit = Sim::accumulator == 0;
        return true;
    case ASLX:
        Sim::vBit = (Sim::indexRegister >= 16384 && Sim::indexRegister < 32768) ||
                    (Sim::indexRegister >= 32768 && Sim::indexRegister < 49152);
        Sim::indexRegister *= 2;
        if (Sim::indexRegister >= 65536) {
            Sim::cBit = 1;
            Sim::indexRegister = Sim::indexRegister % 65536;
        }
        else {
            Sim::cBit = 0;
        }
        Sim::nBit = Sim::indexRegister >= 32768;
        Sim::zBit = Sim::indexRegister == 0;
        return true;
    case ASRA:
        Sim::cBit = (Sim::accumulator % 2) == 1;
        if (Sim::accumulator < 32768) {
            Sim::accumulator /= 2;
        }
        else {
            Sim::accumulator = Sim::accumulator / 2 + 32768;
        }
        Sim::nBit = Sim::accumulator >= 32768;
        Sim::zBit = Sim::accumulator == 0;
        return true;
    case ASRX:
        Sim::cBit = (Sim::indexRegister % 2) == 1;
        if (Sim::indexRegister < 32768) {
            Sim::indexRegister /= 2;
        }
        else {
            Sim::indexRegister = Sim::indexRegister / 2 + 32768;
        }
        Sim::nBit = Sim::indexRegister >= 32768;
        Sim::zBit = Sim::indexRegister == 0;
        return true;
    case BR:
        programCounter = operand;
        return true;
    case BRC:
        if (cBit) {
            programCounter = operand;
        }
        return true;
    case BREQ:
        if (zBit) {
            programCounter = operand;
        }
        return true;
    case BRGE:
        if (!nBit) {
            programCounter = operand;
        }
        return true;
    case BRGT:
        if (!nBit && !zBit) {
            programCounter = operand;
        }
        return true;
    case BRLE:
        if (nBit || zBit) {
            programCounter = operand;
        }
        return true;
    case BRLT:
        if (nBit) {
            programCounter = operand;
        }
        return true;
    case BRNE:
        if (!zBit) {
            programCounter = operand;
        }
        return true;
    case BRV:
        if (vBit) {
            programCounter = operand;
        }
        return true;
    case CALL:
        stackPointer = add(stackPointer, 65534); // SP <- SP - 2
        writeWord(stackPointer, programCounter); // Mem[SP] <- PC
        programCounter = operand; // PC <- Oprnd
        return true;
    case CHARI:
        break;
    case CHARO:
        break;
    case CPA:
        break;
    case CPX:
        break;
    case DECI:
        break;
    case DECO:
        break;
    case LDA:
        Sim::accumulator = Sim::operandSpecifier % 65536;
        Sim::nBit = Sim::accumulator < 0 ? 1 : 0;
        Sim::zBit = Sim::accumulator == 0 ? 1 : 0;
        break;
    case LDBYTEA:
        break;
    case LDBYTEX:
        break;
    case LDX:
        Sim::indexRegister = Sim::operandSpecifier % 65536;
        Sim::nBit = Sim::indexRegister < 0 ? 1 : 0;
        Sim::zBit = Sim::indexRegister == 0 ? 1 : 0;
        break;
    case MOVFLGA:
        break;
    case MOVSPA:
        stackPointer = accumulator;
        break;
    case NEGA:
        break;
    case NEGX:
        break;
    case NOP:
        break;
    case NOP0:
        break;
    case NOP1:
        break;
    case NOP2:
        break;
    case NOP3:
        break;
    case NOTA:
        break;
    case NOTX:
        break;
    case ORA:
        break;
    case ORX:
        break;
    case RET0:
        // SP <- SP + n
        stackPointer = stackPointer + 0;
        // PC <- Mem[SP]
        Sim::programCounter = readWord(stackPointer);
        // SP <- SP + 2
        stackPointer = (stackPointer + 2) % 65536;
        break;
    case RET1:
        // SP <- SP + n
        stackPointer = stackPointer + 1;
        // PC <- Mem[SP]
        Sim::programCounter = readWord(stackPointer);
        // SP <- SP + 2
        stackPointer = (stackPointer + 2) % 65536;
        break;
    case RET2:
        // SP <- SP + n
        stackPointer = stackPointer + 2;
        // PC <- Mem[SP]
        Sim::programCounter = readWord(stackPointer);
        // SP <- SP + 2
        stackPointer = (stackPointer + 2) % 65536;
        break;
    case RET3:
        // SP <- SP + n
        stackPointer = stackPointer + 3;
        // PC <- Mem[SP]
        Sim::programCounter = readWord(stackPointer);
        // SP <- SP + 2
        stackPointer = (stackPointer + 2) % 65536;
        break;
    case RET4:
        // SP <- SP + n
        stackPointer = stackPointer + 4;
        // PC <- Mem[SP]
        Sim::programCounter = readWord(stackPointer);
        // SP <- SP + 2
        stackPointer = (stackPointer + 2) % 65536;
        break;
    case RET5:
        // SP <- SP + n
        stackPointer = stackPointer + 5;
        // PC <- Mem[SP]
        Sim::programCounter = readWord(stackPointer);
        // SP <- SP + 2
        stackPointer = (stackPointer + 2) % 65536;
        break;
    case RET6:
        // SP <- SP + n
        stackPointer = stackPointer + 6;
        // PC <- Mem[SP]
        Sim::programCounter = readWord(stackPointer);
        // SP <- SP + 2
        stackPointer = (stackPointer + 2) % 65536;
        break;
    case RET7:
        // SP <- SP + n
        stackPointer = stackPointer + 7;
        // PC <- Mem[SP]
        Sim::programCounter = readWord(stackPointer);
        // SP <- SP + 2
        stackPointer = (stackPointer + 2) % 65536;
        break;
    case RETTR:
        break;
    case ROLA:
        break;
    case ROLX:
        break;
    case RORA:
        break;
    case RORX:
        break;
    case STA:
        Sim::operandSpecifier = Sim::accumulator % 65536;
        break;
    case STBYTEA:
        break;
    case STBYTEX:
        break;
    case STOP:
        break;
    case STRO:
        break;
    case STX:
        Sim::operandSpecifier = Sim::indexRegister % 65536;
        break;
    case SUBA:
        break;
    case SUBSP:
        break;
    case SUBX:
        break;
    default:
        qDebug() << "Invalid instruction?!";
    }
}





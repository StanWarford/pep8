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
        Sim::vBit = (Sim::accumulator >= 0x4000 && Sim::accumulator < 0x8000) || // prefix is 01 (bin)
                    (Sim::accumulator >= 0x8000 && Sim::accumulator < 0xC000); // prefix is 10 (bin)
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
        Sim::vBit = (Sim::indexRegister >= 0x4000 && Sim::indexRegister < 0x8000) || // prefix is 01 (bin)
                    (Sim::indexRegister >= 0x8000 && Sim::indexRegister < 0xC000); // prefix is 10 (bin)
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
        operand = Sim::readWordOprnd(addrMode);
        programCounter = operand;
        return true;
    case BRC:
        operand = Sim::readWordOprnd(addrMode);
        if (cBit) {
            programCounter = operand;
        }
        return true;
    case BREQ:
        operand = Sim::readWordOprnd(addrMode);
        if (zBit) {
            programCounter = operand;
        }
        return true;
    case BRGE:
        operand = Sim::readWordOprnd(addrMode);
        if (!nBit) {
            programCounter = operand;
        }
        return true;
    case BRGT:
        operand = Sim::readWordOprnd(addrMode);
        if (!nBit && !zBit) {
            programCounter = operand;
        }
        return true;
    case BRLE:
        operand = Sim::readWordOprnd(addrMode);
        if (nBit || zBit) {
            programCounter = operand;
        }
        return true;
    case BRLT:
        operand = Sim::readWordOprnd(addrMode);
        if (nBit) {
            programCounter = operand;
        }
        return true;
    case BRNE:
        operand = Sim::readWordOprnd(addrMode);
        if (!zBit) {
            programCounter = operand;
        }
        return true;
    case BRV:
        operand = Sim::readWordOprnd(addrMode);
        if (vBit) {
            programCounter = operand;
        }
        return true;
    case CALL:
        operand = Sim::readWordOprnd(addrMode);
        stackPointer = add(stackPointer, 65534); // SP <- SP - 2
        writeWord(stackPointer, programCounter); // Mem[SP] <- PC
        programCounter = operand; // PC <- Oprnd
        return true;
    case CHARI:
        return true;
    case CHARO:
        operand = Sim::readByteOprnd(addrMode);
        qDebug() << operand;
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
        Sim::accumulator = Sim::operandSpecifier % 65536;
        Sim::nBit = Sim::accumulator >= 32768;
        Sim::zBit = Sim::accumulator == 0;
        return true;
    case LDBYTEA:
        operand = Sim::readByteOprnd(addrMode);
        Sim::accumulator = Sim::accumulator & 0xFF00;
        Sim::accumulator |= operand;
        Sim::nBit = Sim::accumulator >= 32768;
        Sim::zBit = Sim::accumulator == 0;
        return true;
    case LDBYTEX:
        operand = Sim::readByteOprnd(addrMode);
        Sim::indexRegister = Sim::indexRegister & 0xFF00;
        Sim::indexRegister |= operand;
        Sim::nBit = Sim::indexRegister >= 32768;
        Sim::zBit = Sim::indexRegister == 0;
        return true;
    case LDX:
        Sim::indexRegister = Sim::operandSpecifier % 65536;
        Sim::nBit = Sim::indexRegister >= 32768;
        Sim::zBit = Sim::indexRegister == 0;
        return true;
    case MOVFLGA:
        Sim::accumulator = 0;
        Sim::accumulator += Sim::cBit ? 1 : 0;
        Sim::accumulator += Sim::vBit ? 2 : 0;
        Sim::accumulator += Sim::zBit ? 4 : 0;
        Sim::accumulator += Sim::nBit ? 8 : 0;
        return true;
    case MOVSPA:
        stackPointer = accumulator;
        return true;
    case NEGA:
        accumulator = (~accumulator + 1) & 65535;
        Sim::nBit = Sim::accumulator >= 32768;
        Sim::zBit = Sim::accumulator == 0;
        return true;
    case NEGX:
        indexRegister = (~indexRegister + 1) & 65535;
        Sim::nBit = Sim::indexRegister >= 32768;
        Sim::zBit = Sim::indexRegister == 0;
        return true;
    case NOP:
    case NOP0:
    case NOP1:
    case NOP2:
    case NOP3:

        return true;
    case NOTA:
        accumulator = ~accumulator & 65535;
        Sim::nBit = Sim::accumulator >= 32768;
        Sim::zBit = Sim::accumulator == 0;
        return true;
    case NOTX:
        indexRegister = ~indexRegister & 65535;
        Sim::nBit = Sim::indexRegister >= 32768;
        Sim::zBit = Sim::indexRegister == 0;
        return true;
    case ORA:
        Sim::accumulator = Sim::accumulator | Sim::operandSpecifier;
        Sim::nBit = Sim::accumulator > 32768;
        Sim::zBit = Sim::accumulator == 0;
        return true;
    case ORX:
        Sim::indexRegister = Sim::indexRegister | Sim::operandSpecifier;
        Sim::nBit = Sim::indexRegister > 32768;
        Sim::zBit = Sim::indexRegister == 0;
        return true;
    case RET0:
        Sim::programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET1:
        stackPointer = add(stackPointer, 1); // SP <- SP + 1
        Sim::programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET2:
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        Sim::programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET3:
        stackPointer = add(stackPointer, 3); // SP <- SP + 3
        Sim::programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET4:
        stackPointer = add(stackPointer, 4); // SP <- SP + 4
        Sim::programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET5:
        stackPointer = add(stackPointer, 5); // SP <- SP + 5
        Sim::programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET6:
        stackPointer = add(stackPointer, 6); // SP <- SP + 6
        Sim::programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RET7:
        stackPointer = add(stackPointer, 7); // SP <- SP + 7
        Sim::programCounter = readWord(stackPointer); // PC <- Mem[SP]
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
        operand = Sim::readWordOprnd(addrMode);
        accumulator = operand;
        return true;
    case STBYTEA:
        operand = Sim::readByteOprnd(addrMode);
        accumulator = accumulator & 0xFF00;
        accumulator |= operand;
        return true;
    case STBYTEX:
        operand = Sim::readByteOprnd(addrMode);
        indexRegister = indexRegister & 0xFF00;
        indexRegister |= operand;
        return true;
    case STOP:
        return true;
    case STRO:
        return true;
    case STX:
        operand = Sim::readWordOprnd(addrMode);
        indexRegister = operand;
        return true;
    case SUBA:
        operand = Sim::readWordOprnd(addrMode);
        accumulator = addAndSetNZVC(accumulator, (~operand + 1) & 0xFFFF);
        return true;
    case SUBSP:
        operand = Sim::readWordOprnd(addrMode);
        stackPointer = addAndSetNZVC(stackPointer, (~operand + 1) & 0xFFFF);
        return true;
    case SUBX:
        operand = Sim::readWordOprnd(addrMode);
        indexRegister = addAndSetNZVC(indexRegister, (~operand + 1) & 0xFFFF);
        return true;
    default:
        qDebug() << "Invalid instruction?!";
    }
    return false;
}





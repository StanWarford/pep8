#include "sim.h"
#include "pep.h"
#include <QDebug>

// The machine
QVector<int> Sim::Mem(65536);
bool Sim::nBit, Sim::zBit, Sim::vBit, Sim::cBit;
int Sim::accumulator;
int Sim::indexRegister;
int Sim::stackPointer;
int Sim::programCounter;
int Sim::instructionSpecifier;
int Sim::operandSpecifier;

void Sim::loadMem(QList<int> objectCodeList) {
    for (int i = 0; objectCodeList.length() > 0; i++) {
        Mem[i] = objectCodeList.takeAt(0);
    }
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
        return readByte(stackPointer + operandSpecifier);
    case Enu::SF:
        return readByte(readWord(stackPointer + operandSpecifier));
    case Enu::X:
        return readByte(operandSpecifier + indexRegister);
    case Enu::SX:
        return readByte(stackPointer + operandSpecifier + indexRegister);
    case Enu::SXF:
        return readByte(readWord(stackPointer + operandSpecifier) + indexRegister);
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
        return readWord(stackPointer + operandSpecifier);
    case Enu::SF:
        return readWord(readWord(stackPointer + operandSpecifier));
    case Enu::X:
        return readWord(operandSpecifier + indexRegister);
    case Enu::SX:
        return readWord(stackPointer + operandSpecifier + indexRegister);
    case Enu::SXF:
        return readWord(readWord(stackPointer + operandSpecifier) + indexRegister);
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
        writeByte(readWord(stackPointer + operandSpecifier), value);
        break;
    case Enu::SF:
        writeByte(readWord(readWord(stackPointer + operandSpecifier)), value);
    case Enu::X:
        writeByte(readWord(operandSpecifier + indexRegister), value);
    case Enu::SX:
        writeByte(readWord(stackPointer + operandSpecifier + indexRegister), value);
    case Enu::SXF:
        writeByte(readWord(readWord(stackPointer + operandSpecifier) + indexRegister), value);
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
        writeWord(readWord(stackPointer + operandSpecifier), value);
        break;
    case Enu::SF:
        writeWord(readWord(readWord(stackPointer + operandSpecifier)), value);
    case Enu::X:
        writeWord(readWord(operandSpecifier + indexRegister), value);
    case Enu::SX:
        writeWord(readWord(stackPointer + operandSpecifier + indexRegister), value);
    case Enu::SXF:
        writeWord(readWord(readWord(stackPointer + operandSpecifier) + indexRegister), value);
    case Enu::ALL:
        break;
    }
}

void Sim::vonNeumanStep()
{
    // Fetch
    instructionSpecifier = readByte(programCounter);
    // Increment
    programCounter = (programCounter + 1) % 65536;
    // Decode
    if (!Pep::isUnaryMap[Pep::decodeMnemonic[instructionSpecifier]]) {
        operandSpecifier = readWord(programCounter);
        programCounter = (programCounter + 2) % 65536;
    }
    // Execute
//    switch () {
//
//    }
}





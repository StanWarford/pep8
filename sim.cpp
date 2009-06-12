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

int Sim::readByteOprnd(int addrMode)
{
    switch (addrMode) {
    case 1: // Pep::I, Immediate
        return Sim::operandSpecifier;
    case 2: // Pep::D, Direct
        return readByte(Sim::operandSpecifier);
    case 4: // Pep::N, Indirect
        return readByte(readWord(Sim::operandSpecifier));
    case 8: // Pep::S, Stack-relative
        return readByte(Sim::stackPointer + Sim::operandSpecifier);
    case 16: // Pep::SF, Stack-relative deferred
        return readByte(readWord(Sim::stackPointer + Sim::operandSpecifier));
    case 32: // Pep::X, Indexed
        return readByte(Sim::operandSpecifier + Sim::indexRegister);
    case 64: // Pep::SX, Stack-indexed
        return readByte(Sim::stackPointer + Sim::operandSpecifier + Sim::indexRegister);
    case 128: // Pep:SXF, Stack-indexed deferred
        return readByte(readWord(Sim::stackPointer + Sim::operandSpecifier) + Sim::indexRegister);
    }
    return 0;
}

int Sim::readWordOprnd(int addrMode)
{
    switch (addrMode) {
    case 1: // Pep::I, Immediate
        return Sim::operandSpecifier;
    case 2: // Pep::D, Direct
        return readWord(Sim::operandSpecifier);
    case 4: // Pep::N, Indirect
        return readWord(readWord(Sim::operandSpecifier));
    case 8: // Pep::S, Stack-relative
        return readWord(Sim::stackPointer + Sim::operandSpecifier);
    case 16: // Pep::SF, Stack-relative deferred
        return readWord(readWord(Sim::stackPointer + Sim::operandSpecifier));
    case 32: // Pep::X, Indexed
        return readWord(Sim::operandSpecifier + Sim::indexRegister);
    case 64: // Pep::SX, Stack-indexed
        return readWord(Sim::stackPointer + Sim::operandSpecifier + Sim::indexRegister);
    case 128: // Pep:SXF, Stack-indexed deferred
        return readWord(readWord(Sim::stackPointer + Sim::operandSpecifier) + Sim::indexRegister);
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

void Sim::writeByteOprnd(int addrMode, int value)
{
    switch (addrMode) {
    case 1: // Pep::I, Immediate
        // illegal
        break;
    case 2: // Pep::D, Direct
        writeByte(readWord(Sim::operandSpecifier), value);
        break;
    case 4: // Pep::N, Indirect
        writeByte(readWord(readWord(Sim::operandSpecifier)), value);
        break;
    case 8: // Pep::S, Stack-relative
        writeByte(readWord(Sim::stackPointer + Sim::operandSpecifier), value);
        break;
    case 16: // Pep::SF, Stack-relative deferred
        writeByte(readWord(readWord(Sim::stackPointer + Sim::operandSpecifier)), value);
    case 32: // Pep::X, Indexed
        writeByte(readWord(Sim::operandSpecifier + Sim::indexRegister), value);
    case 64: // Pep::SX, Stack-indexed
        writeByte(readWord(Sim::stackPointer + Sim::operandSpecifier + Sim::indexRegister), value);
    case 128: // Pep:SXF, Stack-indexed deferred
        writeByte(readWord(readWord(Sim::stackPointer + Sim::operandSpecifier) + Sim::indexRegister), value);
    }
}

void Sim::writeWordOprnd(int addrMode, int value)
{
    switch (addrMode) {
    case 1: // Pep::I, Immediate
        // illegal
        break;
    case 2: // Pep::D, Direct
        writeWord(readWord(Sim::operandSpecifier), value);
        break;
    case 4: // Pep::N, Indirect
        writeWord(readWord(readWord(Sim::operandSpecifier)), value);
        break;
    case 8: // Pep::S, Stack-relative
        writeWord(readWord(Sim::stackPointer + Sim::operandSpecifier), value);
        break;
    case 16: // Pep::SF, Stack-relative deferred
        writeWord(readWord(readWord(Sim::stackPointer + Sim::operandSpecifier)), value);
    case 32: // Pep::X, Indexed
        writeWord(readWord(Sim::operandSpecifier + Sim::indexRegister), value);
    case 64: // Pep::SX, Stack-indexed
        writeWord(readWord(Sim::stackPointer + Sim::operandSpecifier + Sim::indexRegister), value);
    case 128: // Pep:SXF, Stack-indexed deferred
        writeWord(readWord(readWord(Sim::stackPointer + Sim::operandSpecifier) + Sim::indexRegister), value);
    }
}

void Sim::vonNeumanStep()
{
    // Fetch
    Sim::instructionSpecifier = Sim::readByte(Sim::programCounter);
    // Increment
    Sim::programCounter = (Sim::programCounter + 1) % 65536;
    // Decode
    if (!Pep::isUnaryMap[Pep::decodeMnemonic[Sim::instructionSpecifier]]) {
        Sim::operandSpecifier = Sim::readWord(Sim::programCounter);
        Sim::programCounter = (Sim::programCounter + 2) % 65536;
    }
    // Execute
//    switch () {
//
//    }
}





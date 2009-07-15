#ifndef SIM_H
#define SIM_H

#include <QVector>
#include "enu.h"

class Sim
{
public:    
    // The machine
    static QVector<int> Mem;
    static bool nBit, zBit, vBit, cBit;
    static int accumulator;
    static int indexRegister;
    static int stackPointer;
    static int programCounter;
    static int instructionSpecifier;
    static int operandSpecifier;

    static QString inputBuffer;
    static QString outputBuffer;

    static Enu::EExecState executionState;
    // State for keeping track of what actions are possible for user and machine

    static int toSignedDecimal(int value);
    // Pre: 0 <= value < 65536
    // Post: -32768 <= value < 32768

    static int fromSignedDecimal(int value);
    // Pre: -32768 <= value < 32768
    // Post: 0 <= value < 65536

    static int add(int lhs, int rhs);

    static int addAndSetNZVC(int lhs, int rhs);

    static void loadMem(QList<int> objectCodeList);

    static int readByte(int memAddr);
    static int readWord(int memAddr);
    static int readByteOprnd(Enu::EAddrMode addrMode);
    static int readWordOprnd(Enu::EAddrMode addrMode);

    static int memAddrOfOprnd(Enu::EAddrMode addrMode);

    static void writeByte(int memAddr, int value);
    // Pre: 0 <= value < 256
    // Post: Value is stored in Mem[memAddr]

    static void writeWord(int memAddr, int value);
    // Pre: 0 <= value < 65536
    // Post: The high-end byte of value is stored in Mem[memAddr]
    // and the low-end byte of value is stored in Mem[memAddr + 1]

    static void writeByteOprnd(Enu::EAddrMode addrMode, int value);
    static void writeWordOprnd(Enu::EAddrMode addrMode, int value);

    static bool vonNeumannStep(QString &errorString);

};

#endif // SIM_H

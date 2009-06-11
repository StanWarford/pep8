#ifndef SIM_H
#define SIM_H

#include <QVector>

class Sim
{
public:    
    // The machine
    static QVector<int> Mem;
    static int nBit, zBit, vBit, cBit;
    static int accumulator;
    static int indexRegister;
    static int stackPointer;
    static int programCounter;
    static int instructionRegister;
    static int operandSpecifier;

    static void loadMem(QList<int> objectCodeList);
};

#endif // SIM_H

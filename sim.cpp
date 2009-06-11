#include "sim.h"
#include <QDebug>

// The machine
QVector<int> Sim::Mem(65536);
int Sim::nBit, Sim::zBit, Sim::vBit, Sim::cBit;
int Sim::accumulator;
int Sim::indexRegister;
int Sim::stackPointer;
int Sim::programCounter;
int Sim::instructionRegister;
int Sim::operandSpecifier;

void Sim::loadMem(QList<int> objectCodeList) {
    int i = 0;
    while (objectCodeList.length() > 0) {
        Mem[i] = objectCodeList.takeAt(0);
        i++;
    }
}

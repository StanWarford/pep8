#include "sim.h"

// The machine
QVector<int> Sim::Mem(65536);
int Sim::nBit, Sim::zBit, Sim::vBit, Sim::cBit;
int Sim::accumulator;
int Sim::indexRegister;
int Sim::stackPointer;
int Sim::programCounter;
int Sim::instructionRegister;
int Sim::operandSpecifier;


// StackFrameFSM.cpp: implementation of the StackFrameFSM class.

#include "StackFrameFSM.h"
#include "sim.h"
#include "pep.h"

#include <QDebug>

StackFrameFSM::StackFrameFSM()
{

}

void StackFrameFSM::reset()
{
    stackState = EStart;
    numCellsFromSubSP = 0;
    numCellsFromCall = 0;
}

// the states: EStart, ESubSP, ECall

int StackFrameFSM::makeTransition(int numCellsToAdd)
{
    Enu::EMnemonic mnemon = Pep::decodeMnemonic[Sim::instructionSpecifier];

    switch(stackState)
    {
    case(EStart):
        if (mnemon == Enu::SUBSP) {
            numCellsFromSubSP = numCellsToAdd;
            stackState = ESubSP;
            qDebug("EStart -> ESubSP");
        }
        else if (mnemon == Enu::CALL) {
            numCellsFromSubSP = 0;
            numCellsFromCall = 1; // = numCellsToAdd; // ECall = 1
            stackState = ECall;
            qDebug() << "EStart -> ECall";
        }
        break;
    case(ESubSP):
        if (mnemon == Enu::CALL) {
            numCellsFromCall = 1; // = numCellsToAdd; // ECall = 1
            stackState = ECall;
            qDebug() << "ESubSP -> ECall";
        }
        else { // not ECall
            stackState = EStart;
            qDebug() << "ESubSP -> EStart, adding frame with " << numCellsFromSubSP << " cells";
            return numCellsFromSubSP; // lone subsp
        }
        break;
    case(ECall):
        if (mnemon == Enu::SUBSP) { // function with 1 or more locals and 0 or more parameters
            stackState = EStart;
            qDebug() << "ECall -> EStart, adding frame with " << numCellsFromSubSP + numCellsFromCall + numCellsToAdd << " cells";
            return (numCellsFromSubSP + numCellsFromCall + numCellsToAdd);
        }
        else { // not ESubSP
            stackState = EStart; // no locals
            qDebug() << "ECall -> EStart, adding frame with " << numCellsFromSubSP + numCellsFromCall << " cells";
            return (numCellsFromSubSP + numCellsFromCall);
        }
        break;
    default:;
    }
    return 0; // don't add a frame yet
}

// StackFrameFSM.cpp: implementation of the StackFrameFSM class.

#include "StackFrameFSM.h"

StackFrameFSM::StackFrameFSM()
{

}

void StackFrameFSM::reset()
{
	stackState = EStart;
	numCellsFromSubSP = 0; 
	numCellsFromCall = 0;
}

//	the states: EStart, ESubSP, ECall

int StackFrameFSM::makeTransition(Enu::EMnemonic mnemon, int nNumCellsToAdd)
{
	switch(stackState)
	{
	case(EStart):
		if (mnemon == Enu::SUBSP) {
			numCellsFromSubSP = nNumCellsToAdd;
			stackState = ESubSP;
			qDebug("EStart -> ESubSP");
		}
			else if (mnemon == Enu::CALL) {
			numCellsFromSubSP = 0;
			numCellsFromCall = nNumCellsToAdd; // m_nCall = 1
			stackState = ECall;
			qDebug("EStart -> ECall");
		}
		break;
	case(ESubSP):
		if (mnemon == Enu::CALL) {
			numCellsFromCall = nNumCellsToAdd; // m_nCall = 1
			stackState = ECall;
			qDebug("ESubSP -> ECall");
		}
		else { // not eM_CALL
			stackState = EStart;
			qDebug("ESubSP -> EStart, adding frame with %d cells", numCellsFromSubSP);
			return numCellsFromSubSP; // lone subsp
		}
		break;
	case(ECall):
		if (mnemon == Enu::SUBSP) { // function with 1 or more locals and 0 or more parameters
			stackState = EStart;
			qDebug("ECall -> EStart, adding frame with %d cells", (numCellsFromSubSP + numCellsFromCall + nNumCellsToAdd));
			return (numCellsFromSubSP + numCellsFromCall + nNumCellsToAdd);
		}
		else { // not ESubSP
			qDebug("ECall -> EStart, adding frame with %d cells", (numCellsFromSubSP + numCellsFromCall));
			stackState = EStart; // no locals
			return (numCellsFromSubSP + numCellsFromCall);
		}
		break;
	default:;
	}
	return 0; // don't add a frame yet
}

// StackFrameFSM.h: interface for the StackFrameFSM class

#ifndef STACK_FRAME_FSM_H 
#define STACK_FRAME_FSM_H 

#include <QObject>
#include "enu.h"

class StackFrameFSM: public QObject  
{
public:
	StackFrameFSM();

	void reset(); // set everything to 0 and start state
        int makeTransition(int numCellsToAdd);
	// if the frame is ready to add, returns the number of cells
	// otherwise returns 0

private:
        enum { EStart, ESubSP, ECall } stackState;

	int numCellsFromSubSP; // number of cells from the first SUBSP (i.e. parameters and retVal)
	int numCellsFromCall;
};

#endif // STACK_FRAME_FSM_H

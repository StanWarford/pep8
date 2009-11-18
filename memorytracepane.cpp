// File: memorytracepane.cpp
/*
    Pep8-1 is a virtual machine for writing machine language and assembly
    language programs.
    
    Copyright (C) 2009  J. Stanley Warford, Pepperdine University

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QFontDialog>
#include <QRgb>
#include "memorytracepane.h"
#include "ui_memorytracepane.h"
#include "pep.h"
#include "sim.h"
#include "asm.h"

#include <QMessageBox>
#include <QDebug>

MemoryTracePane::MemoryTracePane(QWidget *parent) :
        QWidget(parent),
        m_ui(new Ui::MemoryTracePane)
{
    m_ui->setupUi(this);

    m_ui->pepMemoryTraceLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    m_ui->pepStackTraceGraphicsView->setFont(QFont(Pep::codeFont, Pep::codeFontSize));

    connect(m_ui->pepScaleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(zoomFactorChanged(int)));

    scene = new QGraphicsScene(this);

}

MemoryTracePane::~MemoryTracePane()
{
    delete m_ui;
}

void MemoryTracePane::setMemoryTrace()
{
    globalVars.clear();
    runtimeStack.clear();
    heap.clear();
    isRuntimeStackItemAddedStack.clear();
    isHeapItemAddedStack.clear();
    isStackFrameAddedStack.clear();
    isHeapFrameAddedStack.clear();
    stackHeightToStackFrameMap.clear();
    modifiedBytes.clear();
    bytesWrittenLastStep.clear();
    addressToGlobalItemMap.clear();
    addressToStackItemMap.clear();
    addressToHeapItemMap.clear();
    numCellsInStackFrame.clear();
    graphicItemsInStackFrame.clear();
    heapFrameItemStack.clear();
    scene->clear();

    if (Pep::traceTagWarning) {
        hide();
        return;
    }

    stackLocation = QPointF(200, 0);
    globalLocation = QPointF(0, 0);
    heapLocation = QPointF(400, 0 - MemoryCellGraphicsItem::boxHeight);
    QString blockSymbol;
    int multiplier;

    // Globals:
    for (int i = 0; i < Pep::blockSymbols.size(); i++) {
        blockSymbol = Pep::blockSymbols.at(i);
        multiplier = Pep::symbolFormatMultiplier.value(blockSymbol);
        int address = Pep::symbolTable.value(blockSymbol);
        if (multiplier == 1) {
            MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(address,
                                                                      blockSymbol,
                                                                      Pep::symbolFormat.value(blockSymbol),
                                                                      static_cast<int>(globalLocation.x()),
                                                                      static_cast<int>(globalLocation.y()));
            item->updateValue();
            globalLocation = QPointF(globalLocation.x(), globalLocation.y() + MemoryCellGraphicsItem::boxHeight);
            globalVars.push(item);
            addressToGlobalItemMap.insert(address, item);
            scene->addItem(item);
        }
        else {
            int offset = 0;
            int bytesPerCell = Sim::cellSize(Pep::symbolFormat.value(blockSymbol));
            for (int j = 0; j < multiplier; j++) {
                MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(address + offset,
                                                                          blockSymbol + QString("[%1]").arg(j),
                                                                          Pep::symbolFormat.value(blockSymbol),
                                                                          static_cast<int>(globalLocation.x()),
                                                                          static_cast<int>(globalLocation.y()));
                item->updateValue();
                globalLocation = QPointF(globalLocation.x(), globalLocation.y() + MemoryCellGraphicsItem::boxHeight);
                globalVars.push(item);
                addressToGlobalItemMap.insert(address + offset, item);
                scene->addItem(item);
                offset += bytesPerCell;
            }
        }
    }

    // Stack frame:
    stackLocation.setY(globalLocation.y());
    scene->addLine(stackLocation.x() - MemoryCellGraphicsItem::boxWidth * 0.2, stackLocation.y(),
                   stackLocation.x() + MemoryCellGraphicsItem::boxWidth * 1.2, stackLocation.y(),
                   QPen(QBrush(Qt::SolidPattern), 2, Qt::SolidLine));
    int dist = static_cast<int>(MemoryCellGraphicsItem::boxWidth * 1.2 - MemoryCellGraphicsItem::boxWidth * 1.4);
    for (int i = static_cast<int>(MemoryCellGraphicsItem::boxWidth * 1.2); i > dist; i = i - 10) {
        scene->addLine(stackLocation.x() + i - 10, stackLocation.y() + 10,
                       stackLocation.x() + i, stackLocation.y(),
                       QPen(QBrush(Qt::SolidPattern), 1, Qt::SolidLine));
    }
    stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);

    heapLocation.setY(globalLocation.y() - MemoryCellGraphicsItem::boxHeight);

//    int x = -100 - MemoryCellGraphicsItem::addressWidth - 10;
//    int h = globalVars.size() > runtimeStack.size() ?
//    globalVars.size() * MemoryCellGraphicsItem::boxHeight + 25 :
//    runtimeStack.size() * MemoryCellGraphicsItem::boxHeight + 25;
//    int widthOfCell = MemoryCellGraphicsItem::addressWidth + MemoryCellGraphicsItem::bufferWidth * 2 +
//    MemoryCellGraphicsItem::boxWidth + MemoryCellGraphicsItem::symbolWidth;
//    int w = 200 + widthOfCell;
//    m_ui->pepStackTraceGraphicsView->setSceneRect(x, 15, w, -h);
    
    m_ui->pepStackTraceGraphicsView->setScene(scene);
    m_ui->warningLabel->clear();

    stackFrameFSM.reset();
}

void MemoryTracePane::updateMemoryTrace()
{
    // Color all of the cells normally (globals)
    for (int i = 0; i < globalVars.size(); i++) {
        globalVars.at(i)->boxBgColor = Qt::white;
        globalVars.at(i)->boxTextColor = Qt::black;
    }
    // Color all of the cells normally (stack)
    for (int i = 0; i < runtimeStack.size(); i++) {
        runtimeStack.at(i)->boxBgColor = Qt::white;
        runtimeStack.at(i)->boxTextColor = Qt::black;
    }
    // Color all of the cells normally (heap)
    for (int i = 0; i < heap.size(); i++) {
        heap.at(i)->boxBgColor = Qt::white;
        heap.at(i)->boxTextColor = Qt::black;
    }

    // Add cached stack items to the scene
    for (int i = 0; i < runtimeStack.size(); i++) {
        if (!isRuntimeStackItemAddedStack.at(i)) {
            scene->addItem(runtimeStack.at(i));
            isRuntimeStackItemAddedStack[i] = true;
        }
    }
    // Add cached stack FRAME items to the scene
    for (int i = 0; i < isStackFrameAddedStack.size(); i++) {
        if (!isStackFrameAddedStack.at(i)) {
            scene->addItem(graphicItemsInStackFrame.at(i));
            isStackFrameAddedStack[i] = true;
        }
    }

    // Add cached heap items to the scene
    for (int i = 0; i < isHeapItemAddedStack.size(); i++) {
        if (!isHeapItemAddedStack.at(i)) {
            scene->addItem(heap.at(i));
            isHeapItemAddedStack[i] = true;
        }
    }
    for (int i = 0; i < isHeapFrameAddedStack.size(); i++) {
        if (!isHeapFrameAddedStack.at(i)) {
            scene->addItem(heapFrameItemStack.at(i));
            isHeapFrameAddedStack[i] = true;
        }
    }

    // Color global/stack/heap items red if they were modified last step
    QList<int> modifiedBytesToBeUpdated = modifiedBytes.toList();
    for (int i = 0; i < bytesWrittenLastStep.size(); i++) {
        if (addressToGlobalItemMap.contains(bytesWrittenLastStep.at(i))) {
            addressToGlobalItemMap.value(bytesWrittenLastStep.at(i))->boxBgColor = Qt::red;
            addressToGlobalItemMap.value(bytesWrittenLastStep.at(i))->boxTextColor = Qt::white;
        }
        if (addressToStackItemMap.contains(bytesWrittenLastStep.at(i))) {
            addressToStackItemMap.value(bytesWrittenLastStep.at(i))->boxBgColor = Qt::red;
            addressToStackItemMap.value(bytesWrittenLastStep.at(i))->boxTextColor = Qt::white;
        }
        if (addressToHeapItemMap.contains(bytesWrittenLastStep.at(i))) {
            addressToHeapItemMap.value(bytesWrittenLastStep.at(i))->boxBgColor = Qt::red;
            addressToHeapItemMap.value(bytesWrittenLastStep.at(i))->boxTextColor = Qt::white;
        }
    }
    // Update modified cells
    for (int i = 0; i < modifiedBytesToBeUpdated.size(); i++) {
        if (addressToGlobalItemMap.contains(modifiedBytesToBeUpdated.at(i))) {
            addressToGlobalItemMap.value(modifiedBytesToBeUpdated.at(i))->updateValue();
        }
        if (addressToStackItemMap.contains(modifiedBytesToBeUpdated.at(i))) {
            addressToStackItemMap.value(modifiedBytesToBeUpdated.at(i))->updateValue();
        }
        if (addressToHeapItemMap.contains(modifiedBytesToBeUpdated.at(i))) {
            addressToHeapItemMap.value(modifiedBytesToBeUpdated.at(i))->updateValue();
        }
    }

    // Set the scene rect (to remove scroll bars):
    // I thought we got rid of this? commenting it out...
//    int x = -100 - MemoryCellGraphicsItem::addressWidth - 10;
//    int h = globalVars.size() > runtimeStack.size() ?
//            globalVars.size() * MemoryCellGraphicsItem::boxHeight + 25 :
//            runtimeStack.size() * MemoryCellGraphicsItem::boxHeight + 25;
//    int widthOfCell = MemoryCellGraphicsItem::addressWidth + MemoryCellGraphicsItem::bufferWidth * 2 +
//                      MemoryCellGraphicsItem::boxWidth + MemoryCellGraphicsItem::symbolWidth;
//    int w = 200 + widthOfCell;
//    m_ui->pepStackTraceGraphicsView->setSceneRect(x, 15, w, -h);

    scene->invalidate(); // redraw the scene!
    // this is fast, so we do this for the whole scene instead of just certain boxes

    // Scroll to the top item if we have a scrollbar:
    if (!runtimeStack.isEmpty() && m_ui->pepStackTraceGraphicsView->viewport()->height() < scene->height()) {
        m_ui->pepStackTraceGraphicsView->centerOn(runtimeStack.top());
    }

    // Clear modified bytes so for the next update:
    bytesWrittenLastStep.clear();
    modifiedBytes.clear();
}

void MemoryTracePane::cacheChanges()
{
    modifiedBytes.unite(Sim::modifiedBytes);
    if (Sim::tracingTraps) {
        bytesWrittenLastStep.clear();
        bytesWrittenLastStep = Sim::modifiedBytes.toList();
    }
    else if (Sim::trapped) {
        // We delay for a single vonNeumann step so that we preserve the modified bytes until we leave the trap - this allows for
        // recoloring of cells modified by a trap instruction.
        delayLastStepClear = true;
        bytesWrittenLastStep.append(Sim::modifiedBytes.toList());
    }
    else if (delayLastStepClear) {
        // Phew! We can now update (in updateMemoryTrace). If we don't, no harm done - they didn't want to see what happened in the trap
        delayLastStepClear = false;
    }
    else {
        // Clear the bytes written the step before last, and get the new list from the previous step. This is used in our update for coloring.
        bytesWrittenLastStep.clear();
        bytesWrittenLastStep = Sim::modifiedBytes.toList();
    }
}

void MemoryTracePane::cacheStackChanges()
{
    if (Sim::trapped) {
        return;
    }

    // Look ahead for the symbol trace list (needs to be done here because of the possibility of call (can't look behind on a call)
    // so we just do it for them all)
    switch (Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)]) {
    case Enu::SUBSP:
    case Enu::CALL:
    case Enu::RET0:
    case Enu::RET1:
    case Enu::RET2:
    case Enu::RET3:
    case Enu::RET4:
    case Enu::RET5:
    case Enu::RET6:
    case Enu::RET7:
    case Enu::ADDSP:
        if (Pep::symbolTraceList.contains(Sim::programCounter)) {
            lookAheadSymbolList = Pep::symbolTraceList.value(Sim::programCounter);
        }
        break;
    default:
        break;
    }
    // End look ahead

    int multiplier = 0;
    int bytesPerCell = 0;
    int offset = 0;
    int numCellsToAdd = 0;
    int frameSizeToAdd = 0;
    QString stackSymbol;

    switch (Pep::decodeMnemonic[Sim::instructionSpecifier]) {
    case Enu::CALL:
        {
            MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::stackPointer, "retAddr", Enu::F_2H,
                                                                      static_cast<int>(stackLocation.x()), static_cast<int>(stackLocation.y()));
            item->updateValue();
            stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);

            isRuntimeStackItemAddedStack.push(false);
            runtimeStack.push(item);
            addressToStackItemMap.insert(Sim::stackPointer, item);
            frameSizeToAdd = stackFrameFSM.makeTransition(1);
        }
        break;
    case Enu::SUBSP:
        {
            for (int i = 0; i < lookAheadSymbolList.size(); i++) {
                stackSymbol = lookAheadSymbolList.at(i);
                multiplier = Pep::symbolFormatMultiplier.value(stackSymbol);
                if (multiplier == 1) {
                    offset += Sim::cellSize(Pep::symbolFormat.value(stackSymbol));
                    MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::stackPointer - offset + Sim::operandSpecifier, stackSymbol,
                                                                              Pep::symbolFormat.value(stackSymbol),
                                                                              static_cast<int>(stackLocation.x()), static_cast<int>(stackLocation.y()));
                    item->updateValue();
                    stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);
                    isRuntimeStackItemAddedStack.push(false);
                    runtimeStack.push(item);
                    addressToStackItemMap.insert(Sim::stackPointer - offset + Sim::operandSpecifier, item);
                    numCellsToAdd++;
                }
                else { // This is an array!
                    bytesPerCell = Sim::cellSize(Pep::symbolFormat.value(stackSymbol));
                    for (int j = multiplier - 1; j >= 0; j--) {
                        offset += bytesPerCell;
                        MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::stackPointer - offset + Sim::operandSpecifier, stackSymbol + QString("[%1]").arg(j),
                                                                                  Pep::symbolFormat.value(stackSymbol),
                                                                                  static_cast<int>(stackLocation.x()), static_cast<int>(stackLocation.y()));
                        item->updateValue();
                        stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);
                        isRuntimeStackItemAddedStack.push(false);
                        runtimeStack.push(item);
                        addressToStackItemMap.insert(Sim::stackPointer - offset + Sim::operandSpecifier, item);
                        numCellsToAdd++;
                    }
                }
            }
            // qDebug() << "numCellsToAdd before makeTransition in ADDSP: " << numCellsToAdd;
            frameSizeToAdd = stackFrameFSM.makeTransition(numCellsToAdd);
        }
        break;
    case Enu::RET0:
        popBytes(2);
        frameSizeToAdd = stackFrameFSM.makeTransition(0); // makeTransition(0) -> 0 bytes to add to the stack frame FSM.
        break;
    case Enu::RET1:
        popBytes(3);
        frameSizeToAdd = stackFrameFSM.makeTransition(0);
        break;
    case Enu::RET2:
        popBytes(4);
        frameSizeToAdd = stackFrameFSM.makeTransition(0);
        break;
    case Enu::RET3:
        popBytes(5);
        frameSizeToAdd = stackFrameFSM.makeTransition(0);
        break;
    case Enu::RET4:
        popBytes(6);
        frameSizeToAdd = stackFrameFSM.makeTransition(0);
        break;
    case Enu::RET5:
        popBytes(7);
        frameSizeToAdd = stackFrameFSM.makeTransition(0);
        break;
    case Enu::RET6:
        popBytes(8);
        frameSizeToAdd = stackFrameFSM.makeTransition(0);
        break;
    case Enu::RET7:
        popBytes(9);
        frameSizeToAdd = stackFrameFSM.makeTransition(0);
        break;
    case Enu::ADDSP:
        popBytes(Sim::operandSpecifier);
        frameSizeToAdd = stackFrameFSM.makeTransition(0);
        break;
    default:
        frameSizeToAdd = stackFrameFSM.makeTransition(0);
        break;
    }

    if (frameSizeToAdd != 0) {
        addStackFrame(frameSizeToAdd);
        // This map is used to correlate the top of the stack frame with the frame itself, useful for determining when the frame should dissapear
        // IE: The top byte of the frame gets removed, so does the frame
        stackHeightToStackFrameMap.insert(runtimeStack.size() - 1, graphicItemsInStackFrame.top());
    }
}

void MemoryTracePane::cacheHeapChanges()
{
    if (Sim::trapped) {
        return;
    }
    m_ui->warningLabel->clear();

    if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::CALL && Pep::symbolTable.value("new") == Sim::operandSpecifier) {
        qDebug() << "CALL new";
        int numCellsToAdd = 0;
        int multiplier;
        QString heapSymbol;
        int heapPointer;
        if (Pep::symbolTable.contains("hpPtr")) {
            heapPointer = Pep::symbolTable.value("hpPtr");
        }
        else {
            // We have no idea where the heap pointer is. Error!
            m_ui->warningLabel->setText("Warning: hpPtr not found, unable to trace <code>CALL \'new\'</code>.");
            return;
        }
        int listNumBytes = 0;
        // Check and make sure the accumulator matches the number of bytes we're newing:
        // We'll start by adding up the number of bytes...
        for (int i = 0; i < lookAheadSymbolList.size(); i++) {
            heapSymbol = lookAheadSymbolList.at(i);
            if (Pep::equateSymbols.contains(heapSymbol) || Pep::blockSymbols.contains(heapSymbol)) {
                // listNumBytes += number of bytes for that tag * the multiplier (IE, 2d4a is a 4 cell array of 2 byte decimals, where 2 is the multiplier
                // and 4 is the number of cells. Note: the multiplier should always be 1 for new'd cells, but that's checked below, where we'll give a more specific error.
                listNumBytes += Asm::tagNumBytes(Pep::symbolFormat.value(heapSymbol)) * Pep::symbolFormatMultiplier.value(heapSymbol);
            }
        }
        if (listNumBytes != Sim::accumulator) {
            m_ui->warningLabel->setText("Warning: The accumulator doesn't match the number of bytes in the trace tags");
            return;
        }
        for (int i = 0; i < lookAheadSymbolList.size(); i++) {
            heapSymbol = lookAheadSymbolList.at(i);
            if (Pep::equateSymbols.contains(heapSymbol) || Pep::blockSymbols.contains(heapSymbol)) {
                multiplier = Pep::symbolFormatMultiplier.value(heapSymbol);
            }
            else {
                m_ui->warningLabel->setText("Warning: Symbol \"" + heapSymbol + "\" not found in .equates, unknown size.");
                return;
            }
            if (multiplier == 1) { // We can't support arrays on the stack with our current addressing modes.
                // Very good! Have a cookie. Then, work! *cracks whip* (All our prereqs have been met to make an item)
                MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::readWord(heapPointer),
                                                                          heapSymbol,
                                                                          Pep::symbolFormat.value(heapSymbol),
                                                                          static_cast<int>(heapLocation.x()),
                                                                          static_cast<int>(heapLocation.y()));
                item->updateValue();
                heapLocation.setY(heapLocation.y() - MemoryCellGraphicsItem::boxHeight);
                isHeapItemAddedStack.push(false);
                heap.push(item);
                addressToHeapItemMap.insert(Sim::readWord(heapPointer), item);
                numCellsToAdd++;
            }
        }
        if (numCellsToAdd != 0) {
            addHeapFrame(numCellsToAdd);
        }
    }
}

void MemoryTracePane::highlightOnFocus()
{
    if (m_ui->pepStackTraceGraphicsView->hasFocus() || m_ui->pepScaleSpinBox->hasFocus()) {
        m_ui->pepMemoryTraceLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->pepMemoryTraceLabel->setAutoFillBackground(false);
    }
}

bool MemoryTracePane::hasFocus()
{
    return m_ui->pepStackTraceGraphicsView->hasFocus() || m_ui->pepScaleSpinBox->hasFocus();
}

void MemoryTracePane::setFont()
{
    // We might just do away with this in this pane.
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(m_ui->pepStackTraceGraphicsView->font()), this,
                                      "Set Object Code Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        m_ui->pepStackTraceGraphicsView->setFont(font);
    }
}

void MemoryTracePane::setFocus()
{
    m_ui->pepStackTraceGraphicsView->setFocus();
}

void MemoryTracePane::addStackFrame(int numCells)
{
    QPen pen(Qt::black);
    pen.setWidth(4);
    QGraphicsRectItem *item = new QGraphicsRectItem(stackLocation.x() - 2, stackLocation.y() + MemoryCellGraphicsItem::boxHeight, 
                      static_cast<qreal>(MemoryCellGraphicsItem::boxWidth + 4),
                      static_cast<qreal>(MemoryCellGraphicsItem::boxHeight * numCells), 0);
    item->setPen(pen);
    graphicItemsInStackFrame.push(item);
    isStackFrameAddedStack.push(false);
    item->setZValue(1.0); // This moves the stack frame to the front
    numCellsInStackFrame.push(numCells);
}

void MemoryTracePane::addHeapFrame(int numCells)
{
    QPen pen(Qt::black);
    pen.setWidth(4);
    QGraphicsRectItem *item = new QGraphicsRectItem(heapLocation.x() - 2, heapLocation.y() + MemoryCellGraphicsItem::boxHeight,
                      static_cast<qreal>(MemoryCellGraphicsItem::boxWidth + 4),
                      static_cast<qreal>(MemoryCellGraphicsItem::boxHeight * numCells), 0);
    item->setPen(pen);
    heapFrameItemStack.push(item);
    isHeapFrameAddedStack.push(false);
    item->setZValue(1.0); // This moves the heap frame to the front
}

void MemoryTracePane::popBytes(int bytesToPop)
{
    while (bytesToPop > 0 && !runtimeStack.isEmpty()) {
        if (stackHeightToStackFrameMap.contains(runtimeStack.size() - 1)) {
            if (stackHeightToStackFrameMap.value(runtimeStack.size() - 1)->scene() == scene) {
                scene->removeItem(stackHeightToStackFrameMap.value(runtimeStack.size() - 1));
            }
            delete stackHeightToStackFrameMap.value(runtimeStack.size() - 1);
            graphicItemsInStackFrame.pop();
            stackHeightToStackFrameMap.remove(runtimeStack.size() - 1);
            isStackFrameAddedStack.pop();
            numCellsInStackFrame.pop();
        }
        
        if (runtimeStack.top()->scene() == scene) {
            scene->removeItem(runtimeStack.top());
        }
        addressToStackItemMap.remove(runtimeStack.top()->getAddress());
        bytesToPop -= runtimeStack.top()->getNumBytes();
        delete runtimeStack.top();
        runtimeStack.pop();
        isRuntimeStackItemAddedStack.pop();
        stackLocation.setY(stackLocation.y() + MemoryCellGraphicsItem::boxHeight);
    }
}

void MemoryTracePane::mouseReleaseEvent(QMouseEvent *)
{
    m_ui->pepStackTraceGraphicsView->setFocus();
}

void MemoryTracePane::zoomFactorChanged(int factor)
{
    QMatrix matrix;
    matrix.scale(factor * .01, factor * .01);
    m_ui->pepStackTraceGraphicsView->setMatrix(matrix);
}

void MemoryTracePane::mouseDoubleClickEvent(QMouseEvent *)
{
    emit labelDoubleClicked(Enu::EMemoryTrace);
}

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
    isRuntimeStackItemAddedStack.clear();
    isStackFrameAddedMap.clear();
    stackHeightToStackFrameMap.clear();
    modifiedBytes.clear();
    bytesWrittenLastStep.clear();
    addressToGlobalItemMap.clear();
    addressToStackItemMap.clear();
    numCellsInStackFrame.clear();
    graphicItemsInStackFrame.clear();
    scene->clear();

    if (Pep::traceTagWarning) {
        hide();
        return;
    }

    stackLocation = QPointF(100, 0);
    globalLocation = QPointF(-100, 0);
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
            globalLocation = QPointF(-100, globalLocation.y() + MemoryCellGraphicsItem::boxHeight);
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
                globalLocation = QPointF(-100, globalLocation.y() + MemoryCellGraphicsItem::boxHeight);
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

//    int x = -100 - MemoryCellGraphicsItem::addressWidth - 10;
//    int h = globalVars.size() > runtimeStack.size() ?
//    globalVars.size() * MemoryCellGraphicsItem::boxHeight + 25 :
//    runtimeStack.size() * MemoryCellGraphicsItem::boxHeight + 25;
//    int widthOfCell = MemoryCellGraphicsItem::addressWidth + MemoryCellGraphicsItem::bufferWidth * 2 +
//    MemoryCellGraphicsItem::boxWidth + MemoryCellGraphicsItem::symbolWidth;
//    int w = 200 + widthOfCell;
//    m_ui->pepStackTraceGraphicsView->setSceneRect(x, 15, w, -h);
    
    m_ui->pepStackTraceGraphicsView->setScene(scene);

    stackFrameFSM.reset();
}

void MemoryTracePane::updateMemoryTrace()
{
    for (int i = 0; i < globalVars.size(); i++) {
        globalVars.at(i)->boxBgColor = Qt::white;
        globalVars.at(i)->boxTextColor = Qt::black;
    }
    for (int i = 0; i < runtimeStack.size(); i++) {
        runtimeStack.at(i)->boxBgColor = Qt::white;
        runtimeStack.at(i)->boxTextColor = Qt::black;
    }
    for (int i = 0; i < runtimeStack.size(); i++) {
        if (!isRuntimeStackItemAddedStack.at(i)) {
            scene->addItem(runtimeStack.at(i));
            isRuntimeStackItemAddedStack[i] = true;
        }
    }
    for (int i = 0; i < isStackFrameAddedMap.size(); i++) {
        if (!isStackFrameAddedMap.at(i)) {
            scene->addItem(graphicItemsInStackFrame.at(i));
            isStackFrameAddedMap[i] = true;
        }
    }
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
    }
    for (int i = 0; i < modifiedBytesToBeUpdated.size(); i++) {
        if (addressToGlobalItemMap.contains(modifiedBytesToBeUpdated.at(i))) {
            addressToGlobalItemMap.value(modifiedBytesToBeUpdated.at(i))->updateValue();
        }
        if (addressToStackItemMap.contains(modifiedBytesToBeUpdated.at(i))) {
            addressToStackItemMap.value(modifiedBytesToBeUpdated.at(i))->updateValue();
        }
    }

    // Set the scene rect (to remove scroll bars):
    int x = -100 - MemoryCellGraphicsItem::addressWidth - 10;
    int h = globalVars.size() > runtimeStack.size() ?
            globalVars.size() * MemoryCellGraphicsItem::boxHeight + 25 :
            runtimeStack.size() * MemoryCellGraphicsItem::boxHeight + 25;
    int widthOfCell = MemoryCellGraphicsItem::addressWidth + MemoryCellGraphicsItem::bufferWidth * 2 +
                      MemoryCellGraphicsItem::boxWidth + MemoryCellGraphicsItem::symbolWidth;
    int w = 200 + widthOfCell;
    m_ui->pepStackTraceGraphicsView->setSceneRect(x, 15, w, -h);

    scene->invalidate(); // redraw the scene!
    // this is fast, so we do this regardless of things changing in the boxes

    // Scroll to the top item if we have a scrollbar:
    if (!runtimeStack.isEmpty() && m_ui->pepStackTraceGraphicsView->viewport()->height() < scene->height()) {
        m_ui->pepStackTraceGraphicsView->centerOn(runtimeStack.top());
    }

    // Clear modified bytes so for the next update:
    bytesWrittenLastStep.clear();
    modifiedBytes.clear();
}

void MemoryTracePane::cacheStackChanges()
{
    modifiedBytes.unite(Sim::modifiedBytes);
    if (Sim::tracingTraps) {
        bytesWrittenLastStep.clear();
        bytesWrittenLastStep = Sim::modifiedBytes.toList();
    }
    else if (Sim::trapped) {
        delayLastStepClear = true;
        bytesWrittenLastStep.append(Sim::modifiedBytes.toList());
    }
    else if (delayLastStepClear) {
        delayLastStepClear = false;
    }
    else {
        bytesWrittenLastStep.clear();
        bytesWrittenLastStep = Sim::modifiedBytes.toList();
    }

    if (Sim::trapped) {
        return;
    }

    // Look ahead for the symbol trace list (needs to be done here because of call, so we just do it for them all)
    switch (Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)]) {
    case Enu::SUBSP:
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
            MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::stackPointer, "retAddr",
                                                                      Enu::F_2H,
                                                                      static_cast<int>(stackLocation.x()),
                                                                      static_cast<int>(stackLocation.y()));
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
                    MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::stackPointer - offset + Sim::operandSpecifier,
                                                                              stackSymbol,
                                                                              Pep::symbolFormat.value(stackSymbol),
                                                                              static_cast<int>(stackLocation.x()),
                                                                              static_cast<int>(stackLocation.y()));
                    item->updateValue();
                    stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);
                    isRuntimeStackItemAddedStack.push(false);
                    runtimeStack.push(item);
                    addressToStackItemMap.insert(Sim::stackPointer - offset + Sim::operandSpecifier, item);
                    numCellsToAdd++;
                }
                else {
                    bytesPerCell = Sim::cellSize(Pep::symbolFormat.value(stackSymbol));
                    for (int j = multiplier - 1; j >= 0; j--) {
                        offset += bytesPerCell;
                        MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::stackPointer - offset + Sim::operandSpecifier,
                                                                                  stackSymbol + QString("[%1]").arg(j),
                                                                                  Pep::symbolFormat.value(stackSymbol),
                                                                                  static_cast<int>(stackLocation.x()),
                                                                                  static_cast<int>(stackLocation.y()));
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
        frameSizeToAdd = stackFrameFSM.makeTransition(0);
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
        stackHeightToStackFrameMap.insert(runtimeStack.size() - 1, graphicItemsInStackFrame.top());
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
    isStackFrameAddedMap.push(false);
    graphicItemsInStackFrame.top()->setZValue(1.0); // This moves the stack frame to the front
    numCellsInStackFrame.push(numCells);
}

void MemoryTracePane::popBytes(int bytesToPop)
{
    while (bytesToPop > 0 && !runtimeStack.isEmpty()) {
        if (stackHeightToStackFrameMap.contains(runtimeStack.size() - 1)) {
            if (stackHeightToStackFrameMap.value(runtimeStack.size() - 1)->scene() == scene) {
                scene->removeItem(stackHeightToStackFrameMap.value(runtimeStack.size() - 1));
            }
//            delete stackHeightToStackFrameMap.value(runtimeStack.size() - 1);
            graphicItemsInStackFrame.pop();
            stackHeightToStackFrameMap.remove(runtimeStack.size() - 1);
            isStackFrameAddedMap.pop();
            numCellsInStackFrame.pop();
        }
        
        if (runtimeStack.top()->scene() == scene) {
            scene->removeItem(runtimeStack.top());
        }
        addressToStackItemMap.remove(runtimeStack.top()->getAddress());
        bytesToPop -= runtimeStack.top()->getNumBytes();
//        delete runtimeStack.top();
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

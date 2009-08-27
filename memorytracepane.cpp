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
    scene->clear();
    globalVars.clear();
    runtimeStack.clear();
    isStackItemRendered.clear();
    modifiedBytesToBeUpdated.clear();
    modifiedBytes.clear();
    bytesWrittenLastStep.clear();
    addressToGlobalItemMap.clear();
    addressToStackItemMap.clear();

    if (Pep::traceTagWarning) {
        hide();
        return;
    }
    stackLocation = QPointF(100, -100);
    globalLocation = QPointF(-100, -100);
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
                                                                      globalLocation.x(),
                                                                      globalLocation.y());
            item->updateValue();
            globalLocation = QPointF(-100, globalLocation.y() + MemoryCellGraphicsItem::boxHeight);
            globalVars.push(item);
            addressToGlobalItemMap.insert(address, item);
            scene->addItem(item);
        }
        else {
            int offset = 0;
            int bytesPerCell = cellSize(Pep::symbolFormat.value(blockSymbol));
            for (int j = 0; j < multiplier; j++) {
                MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(address + offset,
                                                                          blockSymbol + QString("[%1]").arg(j),
                                                                          Pep::symbolFormat.value(blockSymbol),
                                                                          globalLocation.x(),
                                                                          globalLocation.y());
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
    int dist = MemoryCellGraphicsItem::boxWidth * 1.2 - MemoryCellGraphicsItem::boxWidth * 1.4;
    for (int i = MemoryCellGraphicsItem::boxWidth * 1.2; i > dist; i = i - 10) {
        scene->addLine(stackLocation.x() + i - 10, stackLocation.y() + 10,
                       stackLocation.x() + i, stackLocation.y(),
                       QPen(QBrush(Qt::SolidPattern), 1, Qt::SolidLine));
    }
    stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);

    m_ui->pepStackTraceGraphicsView->setScene(scene);
}

int MemoryTracePane::cellSize(Enu::ESymbolFormat symbolFormat)
{
    switch (symbolFormat) {
    case Enu::F_1C:
        return 1;
    case Enu::F_1D:
        return 1;
    case Enu::F_2D:
        return 2;
    case Enu::F_1H:
        return 1;
    case Enu::F_2H:
        return 2;
    default:
        // Should not occur
        return 0;
    }
}

void MemoryTracePane::setDebugState(bool b)
{

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
    modifiedBytesToBeUpdated = modifiedBytes.toList();
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
    m_ui->pepStackTraceGraphicsView->fitInView(m_ui->pepStackTraceGraphicsView->viewport()->rect());

    bytesWrittenLastStep.clear();
    modifiedBytes.clear();

    for (int i = 0; i < runtimeStack.size(); i++) {
        if (!isStackItemRendered.at(i)) {
            scene->addItem(runtimeStack.at(i));
            isStackItemRendered[i] = true;
        }
    }
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

    // Look ahead
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

    int multiplier;
    int offset = 0;
    int bytesPerCell;
    int bytesToPop;
    QString stackSymbol;

    switch (Pep::decodeMnemonic[Sim::instructionSpecifier]) {
    case Enu::CALL:
        MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::stackPointer, "retAddr",
                                                                  Enu::F_2H, stackLocation.x(), stackLocation.y());
        item->updateValue();
        stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);

        runtimeStack.push(item);
        isStackItemRendered.push(false);
        addressToStackItemMap.insert(Sim::stackPointer, item);
        break;
    case Enu::SUBSP:
        for (int i = 0; i < lookAheadSymbolList.size(); i++) {
            stackSymbol = lookAheadSymbolList.at(i);
            multiplier = Pep::symbolFormatMultiplier.value(stackSymbol);
            if (multiplier == 1) {
                offset += cellSize(Pep::symbolFormat.value(stackSymbol));
                MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::stackPointer - offset + Sim::operandSpecifier,
                                                                          stackSymbol,
                                                                          Pep::symbolFormat.value(stackSymbol),
                                                                          stackLocation.x(), stackLocation.y());
                item->updateValue();
                stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);
                runtimeStack.push(item);
                isStackItemRendered.push(false);
                addressToStackItemMap.insert(Sim::stackPointer - offset + Sim::operandSpecifier, item);
            }
            else {
                bytesPerCell = cellSize(Pep::symbolFormat.value(stackSymbol));
                for (int j = multiplier - 1; j >= 0; j--) {
                    offset += bytesPerCell;
                    MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::stackPointer - offset + Sim::operandSpecifier,
                                                                              stackSymbol + QString("[%1]").arg(j),
                                                                              Pep::symbolFormat.value(stackSymbol),
                                                                              stackLocation.x(), stackLocation.y());
                    item->updateValue();
                    stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);
                    runtimeStack.push(item);
                    isStackItemRendered.push(false);
                    addressToStackItemMap.insert(Sim::stackPointer - offset + Sim::operandSpecifier, item);
                }
            }
        }
        break;
    case Enu::RET0:
        break;
    case Enu::RET1:
        break;
    case Enu::RET2:
        break;
    case Enu::RET3:
        break;
    case Enu::RET4:
        break;
    case Enu::RET5:
        break;
    case Enu::RET6:
        break;
    case Enu::RET7:
        break;
    case Enu::ADDSP:
        bytesToPop = Sim::operandSpecifier;
        for (int i = bytesToPop; i > 0; i = i - 2) {

        }
        break;
    default:
        break;
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

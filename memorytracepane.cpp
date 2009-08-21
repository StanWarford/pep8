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
#include "sim.h"
#include "pep.h"

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

//    MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(31, 65536, "main", -32, -16);
//    scene->addItem(item);
//    m_ui->pepStackTraceGraphicsView->setScene(scene);
}

MemoryTracePane::~MemoryTracePane()
{
    delete m_ui;
}

void MemoryTracePane::setMemoryTrace()
{
//    scene->clear();
    while (!globalVars.isEmpty()) {
        delete globalVars.pop();
    }
    if (Pep::traceTagWarning) {
        hide();
        return;
    }
    stackLocation = QPointF(100, -100);
    globalLocation = QPointF(-100, -100);
    QString blockSymbol;
    int multiplier;
    for (int i = 0; i < Pep::blockSymbols.size(); i++) {
        blockSymbol = Pep::blockSymbols.at(i);
        multiplier = Pep::symbolFormatMultiplier.value(blockSymbol);
        int address = Pep::symbolTable.value(blockSymbol);
        if (multiplier == 1) {
            MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(address, traceValue(blockSymbol), blockSymbol,
                                                                      globalLocation.x(), globalLocation.y());
            globalLocation = QPointF(-100, globalLocation.y() + MemoryCellGraphicsItem::boxHeight);
            globalVars.push(item);
            addressToGlobalItemMap.insert(address, item);
            scene->addItem(item);
        }
        else {
            for (int j = 0; j < multiplier; j++) {
                MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(address + j * cellSize(Pep::symbolFormat.value(blockSymbol)),
                                                                          traceValue(blockSymbol, j * multiplier),
                                                                          blockSymbol + QString("[%1]").arg(j),
                                                                          globalLocation.x(), globalLocation.y());
                globalLocation = QPointF(-100, globalLocation.y() + MemoryCellGraphicsItem::boxHeight);
                globalVars.push(item);
                addressToGlobalItemMap.insert(address, item);
                scene->addItem(item);
            }
        }
        m_ui->pepStackTraceGraphicsView->setScene(scene);
    }

}

void MemoryTracePane::setDebugState(bool b)
{

}

void MemoryTracePane::updateMemoryTrace()
{
    for (int i = 0; i < globalVars.size(); i++) {
        globalVars.at(i)->boxBgColor = Qt::white;
    }
    modifiedBytesToBeUpdated = modifiedBytes.toList();
    for (int i = 0; i < bytesWrittenLastStep.size(); i++) {
        if (addressToGlobalItemMap.contains(bytesWrittenLastStep.at(i))) {
            addressToGlobalItemMap.value(modifiedBytesToBeUpdated.at(i))->boxBgColor = Qt::red;
        }
    }
    for (int i = 0; i < modifiedBytesToBeUpdated.size(); i++) {
        if (addressToGlobalItemMap.contains(modifiedBytesToBeUpdated.at(i))) {
            addressToGlobalItemMap.value(modifiedBytesToBeUpdated.at(i))->value = traceValue(addressToGlobalItemMap.value(modifiedBytesToBeUpdated.at(i))->getSymbol());
        }
    }
    m_ui->pepStackTraceGraphicsView->setScene(scene);
    m_ui->pepStackTraceGraphicsView->fitInView(m_ui->pepStackTraceGraphicsView->viewport()->rect());

    modifiedBytes.clear();
}

void MemoryTracePane::cacheStackChanges()
{
    bytesWrittenLastStep.clear();
    modifiedBytes.unite(Sim::modifiedBytes);
    bytesWrittenLastStep = Sim::modifiedBytes.toList();
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

QString MemoryTracePane::traceValue(QString symbol, int offset)
{
    QString retString;
    switch (Pep::symbolFormat.value(symbol)) {
    case Enu::F_1C:
        retString = QString(QChar(Sim::Mem[Pep::symbolTable.value(symbol) + offset]));
        break;
    case Enu::F_1D:
        retString = QString("%1").arg(Sim::Mem[Pep::symbolTable.value(symbol) + offset]);
        break;
    case Enu::F_2D:
        retString = QString("%1").arg(Sim::toSignedDecimal(
                Sim::Mem[Pep::symbolTable.value(symbol) + offset] * 256 + Sim::Mem[Pep::symbolTable.value(symbol) + offset +1]));
        break;
    case Enu::F_1H:
        retString = QString("%1").arg(Sim::Mem[Pep::symbolTable.value(symbol) + offset],
                                      2, 16, QLatin1Char('0')).toUpper();
        break;
    case Enu::F_2H:
        retString = QString("%1").arg(Sim::Mem[Pep::symbolTable.value(symbol) + offset] * 256 + Sim::Mem[Pep::symbolTable.value(symbol) + offset + 1],
                                      4, 16, QLatin1Char('0')).toUpper();
        break;
    default:
        // Should not occur
        break;
    }
    return retString;
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

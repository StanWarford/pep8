// File: cpupane.cpp
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
#include <QMessageBox>
#include <QKeyEvent>
#include <QSound>
#include "cpupane.h"
#include "ui_cpupane.h"
#include "sim.h"
#include "pep.h"
#include <QtGlobal>

CpuPane::CpuPane(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CpuPane)
{
    ui->setupUi(this);

    connect(ui->cpuSingleStepPushButton, SIGNAL(clicked()), this, SLOT(singleStepButton()));
    connect(ui->cpuResumePushButton, SIGNAL(clicked()), this, SIGNAL(resumeButtonClicked()));

    interruptExecutionFlag = false;
    clearCpu();
    
    if (Pep::getSystem() != "Mac") {
        ui->pepCpuLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
        ui->pepTraceTrapsCheckBox->setFont(QFont(Pep::labelFont));

        ui->cpuALabel->setFont(QFont(Pep::labelFont));
        ui->cpuXLabel->setFont(QFont(Pep::labelFont));
        ui->cpuSPLabel->setFont(QFont(Pep::labelFont));
        ui->cpuPCLabel->setFont(QFont(Pep::labelFont));
        ui->cpuInstrSpecLabel->setFont(QFont(Pep::labelFont));
        ui->cpuOprndSpecLabel->setFont(QFont(Pep::labelFont));
        ui->cpuOprndLabel->setFont(QFont(Pep::labelFont));
        ui->cpuSingleStepPushButton->setFont(QFont(Pep::labelFont));
        ui->cpuResumePushButton->setFont(QFont(Pep::labelFont));

        ui->pepNLabel->setFont(QFont(Pep::labelFont));
        ui->pepZLabel->setFont(QFont(Pep::labelFont));
        ui->pepVLabel->setFont(QFont(Pep::labelFont));
        ui->pepCLabel->setFont(QFont(Pep::labelFont));

        ui->cpuNLabel->setFont(QFont(Pep::labelFont));
        ui->cpuZLabel->setFont(QFont(Pep::labelFont));
        ui->cpuVLabel->setFont(QFont(Pep::labelFont));
        ui->cpuCLabel->setFont(QFont(Pep::labelFont));

        ui->cpuAHexLabel->setFont(QFont(Pep::labelFont));
        ui->cpuADecLabel->setFont(QFont(Pep::labelFont));

        ui->cpuXHexLabel->setFont(QFont(Pep::labelFont));
        ui->cpuXDecLabel->setFont(QFont(Pep::labelFont));

        ui->cpuSPHexLabel->setFont(QFont(Pep::labelFont));
        ui->cpuSPDecLabel->setFont(QFont(Pep::labelFont));

        ui->cpuPCHexLabel->setFont(QFont(Pep::labelFont));
        ui->cpuPCDecLabel->setFont(QFont(Pep::labelFont));

        ui->cpuInstrSpecBinLabel->setFont(QFont(Pep::labelFont));
        ui->cpuInstrSpecMnemonLabel->setFont(QFont(Pep::labelFont));

        ui->cpuOprndSpecHexLabel->setFont(QFont(Pep::labelFont));
        ui->cpuOprndSpecDecLabel->setFont(QFont(Pep::labelFont));
        ui->cpuOprndHexLabel->setFont(QFont(Pep::labelFont));
        ui->cpuOprndDecLabel->setFont(QFont(Pep::labelFont));
    }
}

CpuPane::~CpuPane()
{
    delete ui;
}

void CpuPane::updateCpu() {
    Enu::EAddrMode addrMode = Pep::decodeAddrMode[Sim::instructionSpecifier];

    ui->cpuNLabel->setText(Sim::nBit ? "1" : "0");
    ui->cpuZLabel->setText(Sim::zBit ? "1" : "0");
    ui->cpuVLabel->setText(Sim::vBit ? "1" : "0");
    ui->cpuCLabel->setText(Sim::cBit ? "1" : "0");

    ui->cpuAHexLabel->setText(QString("0x") + QString("%1").arg(Sim::accumulator, 4, 16, QLatin1Char('0')).toUpper());
    ui->cpuADecLabel->setText(QString("%1").arg(Sim::toSignedDecimal(Sim::accumulator)));

    ui->cpuXHexLabel->setText(QString("0x") + QString("%1").arg(Sim::indexRegister, 4, 16, QLatin1Char('0')).toUpper());
    ui->cpuXDecLabel->setText(QString("%1").arg(Sim::toSignedDecimal(Sim::indexRegister)));

    ui->cpuSPHexLabel->setText(QString("0x") + QString("%1").arg(Sim::stackPointer, 4, 16, QLatin1Char('0')).toUpper());
    ui->cpuSPDecLabel->setText(QString("%1").arg(Sim::stackPointer));

    ui->cpuPCHexLabel->setText(QString("0x") + QString("%1").arg(Sim::programCounter, 4, 16, QLatin1Char('0')).toUpper());
    ui->cpuPCDecLabel->setText(QString("%1").arg(Sim::programCounter));

    ui->cpuInstrSpecBinLabel->setText(QString("%1").arg(Sim::instructionSpecifier, 8, 2, QLatin1Char('0')).toUpper());
    ui->cpuInstrSpecMnemonLabel->setText(" " + Pep::enumToMnemonMap.value(Pep::decodeMnemonic[Sim::instructionSpecifier])
                                           + Pep::commaSpaceToAddrMode(addrMode));

    if (Pep::decodeAddrMode.value(Sim::instructionSpecifier) == Enu::NONE) {
        ui->cpuOprndSpecHexLabel->setText("");
        ui->cpuOprndSpecDecLabel->setText("");
        ui->cpuOprndHexLabel->setText("");
        ui->cpuOprndDecLabel->setText("");
    }
    else {
        ui->cpuOprndSpecHexLabel->setText(QString("0x") + QString("%1").arg(Sim::operandSpecifier, 4, 16, QLatin1Char('0')).toUpper());
        ui->cpuOprndSpecDecLabel->setText(QString("%1").arg(Sim::toSignedDecimal(Sim::operandSpecifier)));
        ui->cpuOprndHexLabel->setText(QString("0x") + QString("%1").arg(Sim::operand, Sim::operandDisplayFieldWidth, 16, QLatin1Char('0')).toUpper());
        ui->cpuOprndDecLabel->setText(QString("%1").arg(Sim::toSignedDecimal(Sim::operand)));
    }
}

void CpuPane::clearCpu()
{
    ui->cpuNLabel->setText("");
    ui->cpuZLabel->setText("");
    ui->cpuVLabel->setText("");
    ui->cpuCLabel->setText("");

    ui->cpuAHexLabel->setText("");
    ui->cpuADecLabel->setText("");

    ui->cpuXHexLabel->setText("");
    ui->cpuXDecLabel->setText("");

    ui->cpuSPHexLabel->setText("");
    ui->cpuSPDecLabel->setText("");

    ui->cpuPCHexLabel->setText("");
    ui->cpuPCDecLabel->setText("");

    ui->cpuInstrSpecBinLabel->setText("");
    ui->cpuInstrSpecMnemonLabel->setText("");

    ui->cpuOprndSpecHexLabel->setText("");
    ui->cpuOprndSpecDecLabel->setText("");
    ui->cpuOprndHexLabel->setText("");
    ui->cpuOprndDecLabel->setText("");

    Sim::nBit = false;
    Sim::zBit = false;
    Sim::vBit = false;
    Sim::cBit = false;

    Sim::accumulator = 0;
    Sim::indexRegister = 0;
    Sim::stackPointer = 0; // Sim::readWord(Pep::dotBurnArgument - 7);
    Sim::programCounter = 0;
}

void CpuPane::runClicked() {
    ui->pepTraceTrapsCheckBox->setChecked(false);
}

void CpuPane::setDebugState(bool b)
{
    ui->pepTraceTrapsCheckBox->setDisabled(b);
    Sim::tracingTraps = ui->pepTraceTrapsCheckBox->isChecked();
}

void CpuPane::traceTraps(bool b)
{
    ui->pepTraceTrapsCheckBox->setChecked(b);
}

void CpuPane::setButtonsEnabled(bool b) {
    ui->cpuResumePushButton->setDisabled(!b);
    ui->cpuSingleStepPushButton->setDisabled(!b);
    if (b) {
        ui->cpuSingleStepPushButton->setFocus();
    }
}

void CpuPane::runWithBatch()
{
    interruptExecutionFlag = false;
    QString errorString;
    while (true) {
        qApp->processEvents(); // To make sure that the event filter gets to handle keypresses during the run
        if (Sim::vonNeumannStep(errorString)) {
            emit vonNeumannStepped();
            if (Sim::outputBuffer.length() == 1) {
                emit appendOutput(Sim::outputBuffer);
                Sim::outputBuffer = "";
            }
        }
        else {
            QMessageBox::warning(0, "Pep/8", errorString);
            updateCpu();
            emit executionComplete();
            return;
        }
        if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
            updateCpu();
            emit executionComplete();
            return;
        }
        if (interruptExecutionFlag) {
            updateCpu();
            return;
        }
    }
}

void CpuPane::runWithTerminal()
{
    waiting = Enu::ERunWaiting;
    interruptExecutionFlag = false;
    QString errorString;
    while (true) {
        qApp->processEvents(); // To make sure that the event filter gets to handle keypresses during the run
        if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
            // we are waiting for input
            updateCpu();
            emit waitingForInput();
            return;
        }
        else {
            if (Sim::vonNeumannStep(errorString)) {
                emit vonNeumannStepped();
                if (Sim::outputBuffer.length() == 1) {
                    emit appendOutput(Sim::outputBuffer);
                    Sim::outputBuffer = "";
                }
            }
            else {
                QMessageBox::warning(0, "Pep/8", errorString);
                updateCpu();
                emit executionComplete();
                return;
            }
            if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                updateCpu();
                emit executionComplete();
                return;
            }
            if (interruptExecutionFlag) {
                updateCpu();
                emit updateSimulationView();
                return;
            }
        }
    }
}

void CpuPane::resumeWithBatch()
{
    interruptExecutionFlag = false;
    QString errorString;
    while (true) {
        qApp->processEvents(); // To make sure that the event filter gets to handle keypresses during the run
        if (ui->pepTraceTrapsCheckBox->isChecked()) {
            trapLookahead();
        }
        else if (Pep::isTrapMap[Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)]]) {
            Sim::trapped = true;
        }
        else if (Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::RETTR) {
            Sim::trapped = false;
        }
        if (Sim::vonNeumannStep(errorString)) {
            emit vonNeumannStepped();
            if (Sim::outputBuffer.length() == 1) {
                emit appendOutput(Sim::outputBuffer);
                Sim::outputBuffer = "";
            }
            if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                emit updateSimulationView();
                emit executionComplete();
                return;
            }
            if (Pep::memAddrssToAssemblerListing->contains(Sim::programCounter) &&
                Pep::listingRowChecked->value(Pep::memAddrssToAssemblerListing->value(Sim::programCounter)) == Qt::Checked) {
                updateCpu();
                emit updateSimulationView();
                return;
            }
        }
        else {
            QMessageBox::warning(0, "Pep/8", errorString);
            updateCpu();
            emit updateSimulationView();
            emit executionComplete();
        }
        if (interruptExecutionFlag) {
            emit updateSimulationView();
            return;
        }
    }
}

void CpuPane::resumeWithTerminal()
{
    waiting = Enu::EDebugResumeWaiting;
    interruptExecutionFlag = false;
    QString errorString;
    while (true) {
        qApp->processEvents(); // To make sure that the event filter gets to handle keypresses during the run
        trapLookahead();
        if (Sim::trapped && !ui->pepTraceTrapsCheckBox->isChecked()) {
            updateCpu();
            do {
                trapLookahead();
                qApp->processEvents(); // To make sure that the event filter gets to handle keypresses during the run
                if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
                    // we are waiting for input
                    ui->cpuSingleStepPushButton->setDisabled(true);
                    ui->cpuResumePushButton->setDisabled(true);
                    emit waitingForInput();
                    return;
                }
                else {
                    if (Sim::vonNeumannStep(errorString)) {
                        emit vonNeumannStepped();
                        if (Sim::outputBuffer.length() == 1) {
                            emit appendOutput(Sim::outputBuffer);
                            Sim::outputBuffer = "";
                        }
                        if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                            emit updateSimulationView();
                            emit executionComplete();
                        }
                    }
                    else {
                        QMessageBox::warning(0, "Pep/8", errorString);
                        emit updateSimulationView();
                        emit executionComplete();
                    }
                }
                if (interruptExecutionFlag) {
                    updateCpu();
                    emit updateSimulationView();
                    return;
                }
            } while (Sim::trapped);
        }
        else if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
            // we are waiting for input
            ui->cpuSingleStepPushButton->setDisabled(true);
            ui->cpuResumePushButton->setDisabled(true);
            emit updateSimulationView();
            updateCpu();
            emit waitingForInput();
            return;
        }
        else {
            if (Sim::vonNeumannStep(errorString)) {
                emit vonNeumannStepped();
                if (Sim::outputBuffer.length() == 1) {
                    emit appendOutput(Sim::outputBuffer);
                    Sim::outputBuffer = "";
                }
                if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                    emit updateSimulationView(); // Finish updating the memory before we're done executing
                    emit executionComplete();
                    return;
                }
                if (Pep::memAddrssToAssemblerListing->contains(Sim::programCounter) &&
                    Pep::listingRowChecked->value(Pep::memAddrssToAssemblerListing->value(Sim::programCounter)) == Qt::Checked) {
                    updateCpu();
                    emit updateSimulationView();
                    return;
                }
            }
            else {
                QMessageBox::warning(0, "Pep/8", errorString);
                updateCpu();
                emit updateSimulationView();
                emit executionComplete();
            }
        }
        if (interruptExecutionFlag) {
            return;
        }
    }
}

void CpuPane::singleStepWithBatch()
{
    interruptExecutionFlag = false;
    QString errorString;
    trapLookahead();
    if (Sim::trapped && !ui->pepTraceTrapsCheckBox->isChecked()) {
        QString errorString;
        do {
            trapLookahead();
            qApp->processEvents();
            if (Sim::vonNeumannStep(errorString)) {
                emit vonNeumannStepped();
                if (Sim::outputBuffer.length() == 1) {
                    emit appendOutput(Sim::outputBuffer);
                    Sim::outputBuffer = "";
                }
                if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                    emit updateSimulationView();
                    emit executionComplete();
                }
            }
            else {
                QMessageBox::warning(0, "Pep/8", errorString);
                emit updateSimulationView();
                emit executionComplete();
            }
            if (interruptExecutionFlag) {
                updateCpu();
                emit updateSimulationView();
                return;
            }
        } while (Sim::trapped);
        emit updateSimulationView();
        updateCpu();
    }
    else if (Sim::vonNeumannStep(errorString)) {
        emit vonNeumannStepped();
        emit updateSimulationView();
        if (Sim::outputBuffer.length() == 1) {
            emit appendOutput(Sim::outputBuffer);
            Sim::outputBuffer = "";
        }
        if (Pep::decodeMnemonic[Sim::instructionSpecifier] != Enu::STOP) {
            updateCpu();
        }
        else {
            emit executionComplete();
        }
    }
    else {
        QMessageBox::warning(0, "Pep/8", errorString);
        emit executionComplete();
    }
}

void CpuPane::singleStepWithTerminal()
{
    interruptExecutionFlag = false;
    QString errorString;
    waiting = Enu::EDebugSSWaiting;
    trapLookahead();
    if (Sim::trapped && !ui->pepTraceTrapsCheckBox->isChecked()) {
        updateCpu();
        do {
            trapLookahead();
            qApp->processEvents();
            if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
                // we are waiting for input
                ui->cpuSingleStepPushButton->setDisabled(true);
                ui->cpuResumePushButton->setDisabled(true);
                emit waitingForInput();
                return;
            }
            else {
                if (Sim::vonNeumannStep(errorString)) {
                    emit vonNeumannStepped();
                    if (Sim::outputBuffer.length() == 1) {
                        emit appendOutput(Sim::outputBuffer);
                        Sim::outputBuffer = "";
                    }
                    if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                        emit updateSimulationView();
                        emit executionComplete();
                    }
                }
                else {
                    QMessageBox::warning(0, "Pep/8", errorString);
                    emit updateSimulationView();
                    emit executionComplete();
                }
            }
            if (interruptExecutionFlag) {
                updateCpu();
                emit updateSimulationView();
                return;
            }
        } while (Sim::trapped);
        emit updateSimulationView();
    }
    else if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
        ui->cpuSingleStepPushButton->setDisabled(true);
        ui->cpuResumePushButton->setDisabled(true);
        emit waitingForInput();
    }
    else {
        if (Sim::vonNeumannStep(errorString)) {
            emit vonNeumannStepped();
            emit updateSimulationView();
            if (Sim::outputBuffer.length() == 1) {
                emit appendOutput(Sim::outputBuffer);
                Sim::outputBuffer = "";
            }
            if (Pep::decodeMnemonic[Sim::instructionSpecifier] != Enu::STOP) {
                updateCpu();
            }
            else {
                emit executionComplete();
            }
        }
        else {
            QMessageBox::warning(0, "Pep/8", errorString);
            emit executionComplete();
            return;
        }
        if (Pep::decodeMnemonic[Sim::instructionSpecifier] != Enu::STOP) {
            updateCpu();
        }
        else {
            emit executionComplete();
        }
    }
}

void CpuPane::trapLookahead()
{
    if (Pep::isTrapMap[Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)]]) {
        Sim::trapped = true;
        Pep::memAddrssToAssemblerListing = &Pep::memAddrssToAssemblerListingOS;
        Pep::listingRowChecked = &Pep::listingRowCheckedOS;
    }
    else if (Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::RETTR) {
        Sim::trapped = false;
        Pep::memAddrssToAssemblerListing = &Pep::memAddrssToAssemblerListingProg;
        Pep::listingRowChecked = &Pep::listingRowCheckedProg;
    }
}

void CpuPane::interruptExecution()
{
    interruptExecutionFlag = true;
}

void CpuPane::highlightOnFocus()
{
    if (ui->cpuSingleStepPushButton->hasFocus()) {
        ui->pepCpuLabel->setAutoFillBackground(true);
    }
    else {
        ui->pepCpuLabel->setAutoFillBackground(false);
    }
}

Enu::EWaiting CpuPane::waitingState()
{
    return waiting;
}

bool CpuPane::singleStepHasFocus()
{
    return ui->cpuSingleStepPushButton->isEnabled() && ui->cpuSingleStepPushButton->hasFocus();
}

bool CpuPane::hasFocus()
{
    return ui->cpuSingleStepPushButton->hasFocus();
}

void CpuPane::mousePressEvent(QMouseEvent *)
{
    if (ui->cpuSingleStepPushButton->isEnabled()) {
        ui->cpuSingleStepPushButton->setFocus();
    }
}

void CpuPane::singleStepButton()
{
    ui->cpuSingleStepPushButton->setFocus();
    emit singleStepButtonClicked();
}

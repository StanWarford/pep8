#include <QMessageBox>
#include <QKeyEvent>
#include "cpupane.h"
#include "ui_cpupane.h"
#include "sim.h"
#include "pep.h"
#include <QDebug>

CpuPane::CpuPane(QWidget *parent) :
        QWidget(parent),
        m_ui(new Ui::CpuPane)
{
    m_ui->setupUi(this);

    connect(m_ui->cpuSingleStepPushButton, SIGNAL(clicked()), this, SIGNAL(singleStepButtonClicked()));

    connect(m_ui->cpuResumePushButton, SIGNAL(clicked()), this, SIGNAL(resumeButtonClicked()));

}

CpuPane::~CpuPane()
{
    delete m_ui;
}

void CpuPane::updateCpu() {
    Enu::EAddrMode addrMode = Pep::decodeAddrMode[Sim::instructionSpecifier];

    m_ui->cpuNLabel->setText(Sim::nBit ? "1" : "0");
    m_ui->cpuZLabel->setText(Sim::zBit ? "1" : "0");
    m_ui->cpuVLabel->setText(Sim::vBit ? "1" : "0");
    m_ui->cpuCLabel->setText(Sim::cBit ? "1" : "0");

    m_ui->cpuAHexLabel->setText(QString("0x") + QString("%1").arg(Sim::accumulator, 4, 16, QLatin1Char('0')).toUpper());
    m_ui->cpuADecLabel->setText(QString("%1").arg(Sim::toSignedDecimal(Sim::accumulator)));

    m_ui->cpuXHexLabel->setText(QString("0x") + QString("%1").arg(Sim::indexRegister, 4, 16, QLatin1Char('0')).toUpper());
    m_ui->cpuXDecLabel->setText(QString("%1").arg(Sim::toSignedDecimal(Sim::indexRegister)));

    m_ui->cpuSPHexLabel->setText(QString("0x") + QString("%1").arg(Sim::stackPointer, 4, 16, QLatin1Char('0')).toUpper());
    m_ui->cpuSPDecLabel->setText(QString("%1").arg(Sim::stackPointer));

    m_ui->cpuPCHexLabel->setText(QString("0x") + QString("%1").arg(Sim::programCounter, 4, 16, QLatin1Char('0')).toUpper());
    m_ui->cpuPCDecLabel->setText(QString("%1").arg(Sim::programCounter));

    m_ui->cpuInstrSpecBinLabel->setText(QString("%1").arg(Sim::instructionSpecifier, 8, 2, QLatin1Char('0')).toUpper());
    m_ui->cpuInstrSpecMnemonLabel->setText(" " + Pep::enumToMnemonMap.value(Pep::decodeMnemonic[Sim::instructionSpecifier])
                                           + Pep::commaSpaceToAddrMode(addrMode));

    if (Pep::decodeAddrMode.value(Sim::instructionSpecifier) == Enu::NONE) {
        m_ui->cpuOprndSpecHexLabel->setText("");
        m_ui->cpuOprndSpecDecLabel->setText("");
        m_ui->cpuOprndHexLabel->setText("");
        m_ui->cpuOprndDecLabel->setText("");
    }
    else {
        m_ui->cpuOprndSpecHexLabel->setText(QString("0x") + QString("%1").arg(Sim::operandSpecifier, 4, 16, QLatin1Char('0')).toUpper());
        m_ui->cpuOprndSpecDecLabel->setText(QString("%1").arg(Sim::toSignedDecimal(Sim::operandSpecifier)));
        if (Sim::isByteOperand) {
            m_ui->cpuOprndHexLabel->setText(QString("0x") + QString("%1").arg(Sim::operand, 2, 16, QLatin1Char('0')).toUpper());
        }
        else {
            m_ui->cpuOprndHexLabel->setText(QString("0x") + QString("%1").arg(Sim::operand, 4, 16, QLatin1Char('0')).toUpper());
        }
        m_ui->cpuOprndHexLabel->setText(QString("0x") + QString("%1").arg(Sim::operand, 4, 16, QLatin1Char('0')).toUpper());
        m_ui->cpuOprndDecLabel->setText(QString("%1").arg(Sim::toSignedDecimal(Sim::operand)));
    }
}

void CpuPane::clearCpu()
{
    m_ui->cpuNLabel->setText("");
    m_ui->cpuZLabel->setText("");
    m_ui->cpuVLabel->setText("");
    m_ui->cpuCLabel->setText("");

    m_ui->cpuAHexLabel->setText("");
    m_ui->cpuADecLabel->setText("");

    m_ui->cpuXHexLabel->setText("");
    m_ui->cpuXDecLabel->setText("");

    m_ui->cpuSPHexLabel->setText("");
    m_ui->cpuSPDecLabel->setText("");

    m_ui->cpuPCHexLabel->setText("");
    m_ui->cpuPCDecLabel->setText("");

    m_ui->cpuInstrSpecBinLabel->setText("");
    m_ui->cpuInstrSpecMnemonLabel->setText("");

    m_ui->cpuOprndSpecHexLabel->setText("");
    m_ui->cpuOprndSpecDecLabel->setText("");
    m_ui->cpuOprndHexLabel->setText("");
    m_ui->cpuOprndDecLabel->setText("");
}

void CpuPane::runClicked() {
    m_ui->pepTraceLoadCheckBox->setChecked(false);
    m_ui->pepTraceProgramCheckBox->setChecked(false);
    m_ui->pepTraceTrapsCheckBox->setChecked(false);
}

void CpuPane::startDebuggingClicked() {
    if (m_ui->pepTraceLoadCheckBox->isChecked() || m_ui->pepTraceProgramCheckBox->isChecked() || m_ui->pepTraceTrapsCheckBox->isChecked()) {
        // do nothing
    } else {
        m_ui->pepTraceProgramCheckBox->setChecked(true);
    }
}

void CpuPane::setDebugState(bool b)
{
    m_ui->pepTraceLoadCheckBox->setDisabled(b);
    m_ui->pepTraceProgramCheckBox->setDisabled(b);
    m_ui->pepTraceTrapsCheckBox->setDisabled(b);
}

void CpuPane::setButtonsEnabled(bool b) {
    m_ui->cpuResumePushButton->setDisabled(!b);
    m_ui->cpuSingleStepPushButton->setDisabled(!b);
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
            emit executionComplete();
            return;
        }
        if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
            emit executionComplete();
            return;
        }
        if (interruptExecutionFlag) {
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
                emit executionComplete();
                return;
            }
            if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                emit executionComplete();
                return;
            }
            if (interruptExecutionFlag) {
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
        if (Sim::vonNeumannStep(errorString)) {
            emit vonNeumannStepped();
            if (Sim::outputBuffer.length() == 1) {
                emit appendOutput(Sim::outputBuffer);
                Sim::outputBuffer = "";
            }
            if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                emit executionComplete();
                return;
            }
            if (Pep::memAddrssToAssemblerListing.contains(Sim::programCounter) &&
                Pep::listingRowChecked.value(Pep::memAddrssToAssemblerListing.value(Sim::programCounter)) == Qt::Checked) {
                updateCpu();
                emit updateSimulationView();
                return;
            }
        }
        else {
            QMessageBox::warning(0, "Pep/8", errorString);
            updateCpu();
            emit executionComplete();
        }
        if (interruptExecutionFlag) {
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
        if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
            // we are waiting for input
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
                    emit executionComplete();
                    return;
                }
                if (Pep::memAddrssToAssemblerListing.contains(Sim::programCounter) &&
                    Pep::listingRowChecked.value(Pep::memAddrssToAssemblerListing.value(Sim::programCounter)) == Qt::Checked) {
                    updateCpu();
                    emit updateSimulationView();
                    return;
                }
            }
            else {
                QMessageBox::warning(0, "Pep/8", errorString);
                updateCpu();
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
    QString errorString;
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
    }
}

void CpuPane::singleStepWithTerminal()
{
    waiting = Enu::EDebugSSWaiting;
    QString errorString;
    if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
//  if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::CHARI ) { // We used to do this, hopefully we didn't have
        m_ui->cpuSingleStepPushButton->setDisabled(true);                // we didn't have a good reason to.
        m_ui->cpuResumePushButton->setDisabled(true);
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
//<<<<<<< .mine
//=======
        if (Pep::decodeMnemonic[Sim::instructionSpecifier] != Enu::STOP) {
            updateCpu();
        }
//>>>>>>> .r221
        else {
            QMessageBox::warning(0, "Pep/8", errorString);
            emit executionComplete();
        }
    }
}

void CpuPane::interruptExecution()
{
    interruptExecutionFlag = true;
}

void CpuPane::highlightOnFocus()
{
    if (m_ui->pepCpuLabel->hasFocus()) { // Never has focus, which is fine
        m_ui->pepCpuLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->pepCpuLabel->setAutoFillBackground(false);
    }
}

Enu::EWaiting CpuPane::waitingState()
{
    return waiting;
}

bool CpuPane::hasFocus()
{
    return m_ui->pepCpuLabel->hasFocus();
}

void CpuPane::undo()
{
    // does nothing with our current implementation
}

void CpuPane::redo()
{
    // does nothing with our current implementation
}

void CpuPane::cut()
{
    // does nothing with our current implementation
}

void CpuPane::copy()
{
    // does nothing with our current implementation
}

void CpuPane::paste()
{
    // does nothing with our current implementation
}


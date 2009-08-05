#include <QMessageBox>
#include <QKeyEvent>
#include <QSound>
#include "cpupane.h"
#include "ui_cpupane.h"
#include "sim.h"
#include "pep.h"

CpuPane::CpuPane(QWidget *parent) :
        QWidget(parent),
        m_ui(new Ui::CpuPane)
{
    m_ui->setupUi(this);
    // QSound::play(":/sounds/itsatrap.wav"); // For later! Gonna be implemented as a % chance on tracing traps in the OS

    connect(m_ui->cpuSingleStepPushButton, SIGNAL(clicked()), this, SIGNAL(singleStepButtonClicked()));
    connect(m_ui->cpuResumePushButton, SIGNAL(clicked()), this, SIGNAL(resumeButtonClicked()));

    if (Pep::getSystem() != "Mac") {
        m_ui->pepCpuLabel->setFont(QFont(Pep::labelFont));
        m_ui->pepTraceTrapsCheckBox->setFont(QFont(Pep::labelFont));

        m_ui->cpuALabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuXLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuSPLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuPCLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuInstrSpecLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuOprndSpecLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuOprndLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuSingleStepPushButton->setFont(QFont(Pep::labelFont));
        m_ui->cpuResumePushButton->setFont(QFont(Pep::labelFont));

        m_ui->cpuNLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuZLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuVLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuCLabel->setFont(QFont(Pep::labelFont));

        m_ui->cpuAHexLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuADecLabel->setFont(QFont(Pep::labelFont));

        m_ui->cpuXHexLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuXDecLabel->setFont(QFont(Pep::labelFont));

        m_ui->cpuSPHexLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuSPDecLabel->setFont(QFont(Pep::labelFont));

        m_ui->cpuPCHexLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuPCDecLabel->setFont(QFont(Pep::labelFont));

        m_ui->cpuInstrSpecBinLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuInstrSpecMnemonLabel->setFont(QFont(Pep::labelFont));

        m_ui->cpuOprndSpecHexLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuOprndSpecDecLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuOprndHexLabel->setFont(QFont(Pep::labelFont));
        m_ui->cpuOprndDecLabel->setFont(QFont(Pep::labelFont));
    }
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
        m_ui->cpuOprndHexLabel->setText(QString("0x") + QString("%1").arg(Sim::operand, Sim::operandDisplayFieldWidth, 16, QLatin1Char('0')).toUpper());
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
    m_ui->pepTraceTrapsCheckBox->setChecked(false);
}

void CpuPane::setDebugState(bool b)
{
    m_ui->pepTraceTrapsCheckBox->setDisabled(b);
}

void CpuPane::traceTraps(bool b)
{
    m_ui->pepTraceTrapsCheckBox->setChecked(b);
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
            emit updateSimulationView();
            emit executionComplete();
            return;
        }
        if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
            emit updateSimulationView();
            emit executionComplete();
            return;
        }
        if (interruptExecutionFlag) {
            emit updateSimulationView();
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
                emit updateSimulationView();
                emit executionComplete();
                return;
            }
            if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                emit updateSimulationView();
                emit executionComplete();
                return;
            }
            if (interruptExecutionFlag) {
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
        if (m_ui->pepTraceTrapsCheckBox->isChecked()) {
            trapLookahead();
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
        if (!m_ui->pepTraceTrapsCheckBox->isChecked()) {
            if (Sim::trapped /*&& !m_ui->pepTraceTrapsCheckBox->isChecked()*/ && !m_ui->pepTraceTrapsCheckBox->isChecked()) {
                updateCpu();
                do {
                    trapLookahead();
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
                } while (Sim::trapped);
            }
        }
        else if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
            // we are waiting for input
            m_ui->cpuSingleStepPushButton->setDisabled(true);
            m_ui->cpuResumePushButton->setDisabled(true);
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
    QString errorString;
    trapLookahead();
    if (Sim::trapped && !m_ui->pepTraceTrapsCheckBox->isChecked()) {
        resumeThroughTrapBatch();
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
    QString errorString;
    waiting = Enu::EDebugSSWaiting;
    trapLookahead();
    if (Sim::trapped && !m_ui->pepTraceTrapsCheckBox->isChecked()) {
        updateCpu();
        do {
            trapLookahead();
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
        } while (Sim::trapped);
        emit updateSimulationView();
    }
    else if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
        m_ui->cpuSingleStepPushButton->setDisabled(true);
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

void CpuPane::resumeThroughTrapBatch()
{
    QString errorString;
    do {
        trapLookahead();
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
    } while (Sim::trapped);
    emit updateSimulationView();
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


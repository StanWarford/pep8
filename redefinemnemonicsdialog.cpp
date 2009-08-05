#include "redefinemnemonicsdialog.h"
#include "ui_redefinemnemonicsdialog.h"

using namespace Enu;

RedefineMnemonicsDialog::RedefineMnemonicsDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::RedefineMnemonicsDialog)
{
    m_ui->setupUi(this);
    restoreDefaults();

    if (Pep::getSystem() != "Mac") {
        m_ui->unaryOpCodeLabel->setFont(QFont(Pep::labelFont));
        m_ui->unaryOpCode1Label->setFont(QFont(Pep::codeFont));
        m_ui->unaryOpCode2Label->setFont(QFont(Pep::codeFont));
        m_ui->unaryOpCode3Label->setFont(QFont(Pep::codeFont));
        m_ui->unaryOpCode4Label->setFont(QFont(Pep::codeFont));

        m_ui->unaryMnemonicLabel->setFont(QFont(Pep::labelFont));
        m_ui->unaryMnemonic0LineEdit->setFont(QFont(Pep::codeFont));
        m_ui->unaryMnemonic1LineEdit->setFont(QFont(Pep::codeFont));
        m_ui->unaryMnemonic2LineEdit->setFont(QFont(Pep::codeFont));
        m_ui->unaryMnemonic3LineEdit->setFont(QFont(Pep::codeFont));

        m_ui->nonunaryMnemonicLabel->setFont(QFont(Pep::labelFont));
        m_ui->iLabel->setFont(QFont(Pep::labelFont));
        m_ui->dLabel->setFont(QFont(Pep::labelFont));
        m_ui->nLabel->setFont(QFont(Pep::labelFont));
        m_ui->sLabel->setFont(QFont(Pep::labelFont));
        m_ui->sfLabel->setFont(QFont(Pep::labelFont));
        m_ui->xLabel->setFont(QFont(Pep::labelFont));
        m_ui->sxLabel->setFont(QFont(Pep::labelFont));
        m_ui->sxfLabel->setFont(QFont(Pep::labelFont));

        m_ui->nonunaryOpCode1Label->setFont(QFont(Pep::codeFont));
        m_ui->nonunaryOpCode2Label->setFont(QFont(Pep::codeFont));
        m_ui->nonunaryOpCode3Label->setFont(QFont(Pep::codeFont));
        m_ui->nonunaryOpCode4Label->setFont(QFont(Pep::codeFont));
    }


    connect(m_ui->defaultMnemonicsButton, SIGNAL(clicked()), this, SLOT(restoreDefaults()));

    QRegExp rx("^[A-Za-z][A-Za-z0-9]{0,7}");
    QValidator *validator = new QRegExpValidator(rx, this);

    m_ui->nonUnaryMnemonic0LineEdit->setValidator(validator);
    m_ui->nonUnaryMnemonic1LineEdit->setValidator(validator);
    m_ui->nonUnaryMnemonic2LineEdit->setValidator(validator);
    m_ui->nonUnaryMnemonic3LineEdit->setValidator(validator);

    m_ui->unaryMnemonic0LineEdit->setValidator(validator);
    m_ui->unaryMnemonic1LineEdit->setValidator(validator);
    m_ui->unaryMnemonic2LineEdit->setValidator(validator);
    m_ui->unaryMnemonic3LineEdit->setValidator(validator);

    connect(m_ui->nonUnaryMnemonic0LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineNonUnaryMnemonic0(QString)));
    connect(m_ui->nonUnaryMnemonic1LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineNonUnaryMnemonic1(QString)));
    connect(m_ui->nonUnaryMnemonic2LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineNonUnaryMnemonic2(QString)));
    connect(m_ui->nonUnaryMnemonic3LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineNonUnaryMnemonic3(QString)));

    connect(m_ui->unaryMnemonic0LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineUnaryMnemonic0(QString)));
    connect(m_ui->unaryMnemonic1LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineUnaryMnemonic1(QString)));
    connect(m_ui->unaryMnemonic2LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineUnaryMnemonic2(QString)));
    connect(m_ui->unaryMnemonic3LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineUnaryMnemonic3(QString)));

    connect(m_ui->mnemon0iCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon0dCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon0nCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon0sCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon0sfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon0xCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon0sxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon0sxfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));

    connect(m_ui->mnemon1iCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon1dCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon1nCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon1sCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon1sfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon1xCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon1sxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon1sxfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));

    connect(m_ui->mnemon2iCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon2dCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon2nCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon2sCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon2sfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon2xCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon2sxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon2sxfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));

    connect(m_ui->mnemon3iCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon3dCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon3nCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon3sCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon3sfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon3xCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon3sxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(m_ui->mnemon3sxfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
}

RedefineMnemonicsDialog::~RedefineMnemonicsDialog()
{
    delete m_ui;
}

void RedefineMnemonicsDialog::restoreDefaults()
{
    m_ui->unaryMnemonic0LineEdit->setText(Pep::defaultUnaryMnemonic0);
    m_ui->unaryMnemonic1LineEdit->setText(Pep::defaultUnaryMnemonic1);
    m_ui->unaryMnemonic2LineEdit->setText(Pep::defaultUnaryMnemonic2);
    m_ui->unaryMnemonic3LineEdit->setText(Pep::defaultUnaryMnemonic3);
    m_ui->nonUnaryMnemonic0LineEdit->setText(Pep::defaultNonUnaryMnemonic0);
    m_ui->mnemon0iCheckBox->setChecked(Pep::defaultMnemon0i);
    m_ui->mnemon0dCheckBox->setChecked(Pep::defaultMnemon0d);
    m_ui->mnemon0nCheckBox->setChecked(Pep::defaultMnemon0n);
    m_ui->mnemon0sCheckBox->setChecked(Pep::defaultMnemon0s);
    m_ui->mnemon0sfCheckBox->setChecked(Pep::defaultMnemon0sf);
    m_ui->mnemon0xCheckBox->setChecked(Pep::defaultMnemon0x);
    m_ui->mnemon0sxCheckBox->setChecked(Pep::defaultMnemon0sx);
    m_ui->mnemon0sxfCheckBox->setChecked(Pep::defaultMnemon0sxf);
    m_ui->nonUnaryMnemonic1LineEdit->setText(Pep::defaultNonUnaryMnemonic1);
    m_ui->mnemon1iCheckBox->setChecked(Pep::defaultMnemon1i);
    m_ui->mnemon1dCheckBox->setChecked(Pep::defaultMnemon1d);
    m_ui->mnemon1nCheckBox->setChecked(Pep::defaultMnemon1n);
    m_ui->mnemon1sCheckBox->setChecked(Pep::defaultMnemon1s);
    m_ui->mnemon1sfCheckBox->setChecked(Pep::defaultMnemon1sf);
    m_ui->mnemon1xCheckBox->setChecked(Pep::defaultMnemon1x);
    m_ui->mnemon1sxCheckBox->setChecked(Pep::defaultMnemon1sx);
    m_ui->mnemon1sxfCheckBox->setChecked(Pep::defaultMnemon1sxf);
    m_ui->nonUnaryMnemonic2LineEdit->setText(Pep::defaultNonUnaryMnemonic2);
    m_ui->mnemon2iCheckBox->setChecked(Pep::defaultMnemon2i);
    m_ui->mnemon2dCheckBox->setChecked(Pep::defaultMnemon2d);
    m_ui->mnemon2nCheckBox->setChecked(Pep::defaultMnemon2n);
    m_ui->mnemon2sCheckBox->setChecked(Pep::defaultMnemon2s);
    m_ui->mnemon2sfCheckBox->setChecked(Pep::defaultMnemon2sf);
    m_ui->mnemon2xCheckBox->setChecked(Pep::defaultMnemon2x);
    m_ui->mnemon2sxCheckBox->setChecked(Pep::defaultMnemon2sx);
    m_ui->mnemon2sxfCheckBox->setChecked(Pep::defaultMnemon2sxf);
    m_ui->nonUnaryMnemonic3LineEdit->setText(Pep::defaultNonUnaryMnemonic3);
    m_ui->mnemon3iCheckBox->setChecked(Pep::defaultMnemon3i);
    m_ui->mnemon3dCheckBox->setChecked(Pep::defaultMnemon3d);
    m_ui->mnemon3nCheckBox->setChecked(Pep::defaultMnemon3n);
    m_ui->mnemon3sCheckBox->setChecked(Pep::defaultMnemon3s);
    m_ui->mnemon3sfCheckBox->setChecked(Pep::defaultMnemon3sf);
    m_ui->mnemon3xCheckBox->setChecked(Pep::defaultMnemon3x);
    m_ui->mnemon3sxCheckBox->setChecked(Pep::defaultMnemon3sx);
    m_ui->mnemon3sxfCheckBox->setChecked(Pep::defaultMnemon3sxf);

    int addrMode = 0;
    if (Pep::defaultMnemon0i) addrMode |= I;
    if (Pep::defaultMnemon0d) addrMode |= D;
    if (Pep::defaultMnemon0n) addrMode |= N;
    if (Pep::defaultMnemon0s) addrMode |= S;
    if (Pep::defaultMnemon0sf) addrMode |= SF;
    if (Pep::defaultMnemon0x) addrMode |= X;
    if (Pep::defaultMnemon0sx) addrMode |= SX;
    if (Pep::defaultMnemon0sxf) addrMode |= SXF;
    Pep::addrModesMap.insert(NOP, addrMode);
    addrMode = 0;
    if (Pep::defaultMnemon1i) addrMode |= I;
    if (Pep::defaultMnemon1d) addrMode |= D;
    if (Pep::defaultMnemon1n) addrMode |= N;
    if (Pep::defaultMnemon1s) addrMode |= S;
    if (Pep::defaultMnemon1sf) addrMode |= SF;
    if (Pep::defaultMnemon1x) addrMode |= X;
    if (Pep::defaultMnemon1sx) addrMode |= SX;
    if (Pep::defaultMnemon1sxf) addrMode |= SXF;
    Pep::addrModesMap.insert(DECI, addrMode);
    addrMode = 0;
    if (Pep::defaultMnemon2i) addrMode |= I;
    if (Pep::defaultMnemon2d) addrMode |= D;
    if (Pep::defaultMnemon2n) addrMode |= N;
    if (Pep::defaultMnemon2s) addrMode |= S;
    if (Pep::defaultMnemon2sf) addrMode |= SF;
    if (Pep::defaultMnemon2x) addrMode |= X;
    if (Pep::defaultMnemon2sx) addrMode |= SX;
    if (Pep::defaultMnemon2sxf) addrMode |= SXF;
    Pep::addrModesMap.insert(DECO, addrMode);
    addrMode = 0;
    if (Pep::defaultMnemon3i) addrMode |= I;
    if (Pep::defaultMnemon3d) addrMode |= D;
    if (Pep::defaultMnemon3n) addrMode |= N;
    if (Pep::defaultMnemon3s) addrMode |= S;
    if (Pep::defaultMnemon3sf) addrMode |= SF;
    if (Pep::defaultMnemon3x) addrMode |= X;
    if (Pep::defaultMnemon3sx) addrMode |= SX;
    if (Pep::defaultMnemon3sxf) addrMode |= SXF;
    Pep::addrModesMap.insert(STRO, addrMode);

    Pep::initEnumMnemonMaps();
}

void RedefineMnemonicsDialog::redefineNonUnaryMnemonic0(QString string)
{    
    string = string.toUpper();
    if (string.isEmpty()) {
        m_ui->warningLabel->setText("Empty mnemonic not stored for 00101.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        m_ui->warningLabel->setText("Duplicate not stored for 00101.");
    }
    else {
        m_ui->nonUnaryMnemonic0LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::NOP));
        Pep::enumToMnemonMap.insert(Enu::NOP, string);
        Pep::mnemonToEnumMap.insert(string, Enu::NOP);
        m_ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineNonUnaryMnemonic1(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        m_ui->warningLabel->setText("Empty mnemonic not stored for 00110.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        m_ui->warningLabel->setText("Duplicate not stored for 00110.");
    }
    else {
        m_ui->nonUnaryMnemonic1LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::DECI));
        Pep::enumToMnemonMap.insert(Enu::DECI, string);
        Pep::mnemonToEnumMap.insert(string, Enu::DECI);
        m_ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineNonUnaryMnemonic2(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        m_ui->warningLabel->setText("Empty mnemonic not stored for 00111.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        m_ui->warningLabel->setText("Duplicate not stored for 00111.");
    }
    else {
        m_ui->nonUnaryMnemonic2LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::DECO));
        Pep::enumToMnemonMap.insert(Enu::DECO, string);
        Pep::mnemonToEnumMap.insert(string, Enu::DECO);
        m_ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineNonUnaryMnemonic3(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        m_ui->warningLabel->setText("Empty mnemonic not stored for 01000.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        m_ui->warningLabel->setText("Duplicate not stored for 01000.");
    }
    else {
        m_ui->nonUnaryMnemonic3LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::STRO));
        Pep::enumToMnemonMap.insert(Enu::STRO, string);
        Pep::mnemonToEnumMap.insert(string, Enu::STRO);
        m_ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineUnaryMnemonic0(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        m_ui->warningLabel->setText("Empty mnemonic not stored for 0010 0010.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        m_ui->warningLabel->setText("Duplicate not stored for 0010 0010.");
    }
    else {
        m_ui->unaryMnemonic0LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::NOP0));
        Pep::enumToMnemonMap.insert(Enu::NOP0, string);
        Pep::mnemonToEnumMap.insert(string, Enu::NOP0);
        m_ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineUnaryMnemonic1(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        m_ui->warningLabel->setText("Empty mnemonic not stored for 0010 0101.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        m_ui->warningLabel->setText("Duplicate not stored for 0010 0101.");
    }
    else {
        m_ui->unaryMnemonic1LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::NOP1));
        Pep::enumToMnemonMap.insert(Enu::NOP1, string);
        Pep::mnemonToEnumMap.insert(string, Enu::NOP1);
        m_ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineUnaryMnemonic2(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        m_ui->warningLabel->setText("Empty mnemonic not stored for 0010 0110.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        m_ui->warningLabel->setText("Duplicate not stored for 0010 0110.");
    }
    else {
        m_ui->unaryMnemonic2LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::NOP2));
        Pep::enumToMnemonMap.insert(Enu::NOP2, string);
        Pep::mnemonToEnumMap.insert(string, Enu::NOP2);
        m_ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineUnaryMnemonic3(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        m_ui->warningLabel->setText("Empty mnemonic not stored for 0010 0111.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        m_ui->warningLabel->setText("Duplicate not stored for 0010 0111.");
    }
    else {
        m_ui->unaryMnemonic3LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::NOP3));
        Pep::enumToMnemonMap.insert(Enu::NOP3, string);
        Pep::mnemonToEnumMap.insert(string, Enu::NOP3);
        m_ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::setNonUnaryAllowedModes()
{
    int addrMode = 0;
    if (m_ui->mnemon0iCheckBox->isChecked()) addrMode |= I;
    if (m_ui->mnemon0dCheckBox->isChecked()) addrMode |= D;
    if (m_ui->mnemon0nCheckBox->isChecked()) addrMode |= N;
    if (m_ui->mnemon0sCheckBox->isChecked()) addrMode |= S;
    if (m_ui->mnemon0sfCheckBox->isChecked()) addrMode |= SF;
    if (m_ui->mnemon0xCheckBox->isChecked()) addrMode |= X;
    if (m_ui->mnemon0sxCheckBox->isChecked()) addrMode |= SX;
    if (m_ui->mnemon0sxfCheckBox->isChecked()) addrMode |= SXF;
    Pep::addrModesMap.insert(NOP, addrMode);
    addrMode = 0;
    if (m_ui->mnemon1iCheckBox->isChecked()) addrMode |= I;
    if (m_ui->mnemon1dCheckBox->isChecked()) addrMode |= D;
    if (m_ui->mnemon1nCheckBox->isChecked()) addrMode |= N;
    if (m_ui->mnemon1sCheckBox->isChecked()) addrMode |= S;
    if (m_ui->mnemon1sfCheckBox->isChecked()) addrMode |= SF;
    if (m_ui->mnemon1xCheckBox->isChecked()) addrMode |= X;
    if (m_ui->mnemon1sxCheckBox->isChecked()) addrMode |= SX;
    if (m_ui->mnemon1sxfCheckBox->isChecked()) addrMode |= SXF;
    Pep::addrModesMap.insert(DECI, addrMode);
    addrMode = 0;
    if (m_ui->mnemon2iCheckBox->isChecked()) addrMode |= I;
    if (m_ui->mnemon2dCheckBox->isChecked()) addrMode |= D;
    if (m_ui->mnemon2nCheckBox->isChecked()) addrMode |= N;
    if (m_ui->mnemon2sCheckBox->isChecked()) addrMode |= S;
    if (m_ui->mnemon2sfCheckBox->isChecked()) addrMode |= SF;
    if (m_ui->mnemon2xCheckBox->isChecked()) addrMode |= X;
    if (m_ui->mnemon2sxCheckBox->isChecked()) addrMode |= SX;
    if (m_ui->mnemon2sxfCheckBox->isChecked()) addrMode |= SXF;
    Pep::addrModesMap.insert(DECO, addrMode);
    addrMode = 0;
    if (m_ui->mnemon3iCheckBox->isChecked()) addrMode |= I;
    if (m_ui->mnemon3dCheckBox->isChecked()) addrMode |= D;
    if (m_ui->mnemon3nCheckBox->isChecked()) addrMode |= N;
    if (m_ui->mnemon3sCheckBox->isChecked()) addrMode |= S;
    if (m_ui->mnemon3sfCheckBox->isChecked()) addrMode |= SF;
    if (m_ui->mnemon3xCheckBox->isChecked()) addrMode |= X;
    if (m_ui->mnemon3sxCheckBox->isChecked()) addrMode |= SX;
    if (m_ui->mnemon3sxfCheckBox->isChecked()) addrMode |= SXF;
    Pep::addrModesMap.insert(STRO, addrMode);
}

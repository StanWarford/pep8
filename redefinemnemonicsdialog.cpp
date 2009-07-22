#include "redefinemnemonicsdialog.h"
#include "ui_redefinemnemonicsdialog.h"

using namespace Enu;

RedefineMnemonicsDialog::RedefineMnemonicsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::RedefineMnemonicsDialog)
{
    m_ui->setupUi(this);
    restoreDefaults();

    connect(m_ui->defaultMnemonicsButton, SIGNAL(clicked()), this, SLOT(restoreDefaults()));

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

    setNonUnaryAllowedModes();
}

void RedefineMnemonicsDialog::setNonUnaryAllowedModes()
{
    int addrMode;
    addrMode = 0;
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

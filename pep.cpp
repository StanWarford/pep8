#include "pep.h"

    QMap<Pep::MnemonicOpCode, QString> Pep::mnemonicTable;
    void Pep::initMnemonicTable() {
        mnemonicTable.insert(ADDA, "ADDA");
    }

    const QString Pep::defaultUnaryMnemonic0 = "NOP0";
    const QString Pep::defaultUnaryMnemonic1 = "NOP1";
    const QString Pep::defaultUnaryMnemonic2 = "NOP2";
    const QString Pep::defaultUnaryMnemonic3 = "NOP3";
    const QString Pep::defaultNonUnaryMnemonic0 = "NOP";
    const bool Pep::defaultMnemon0i = true;
    const bool Pep::defaultMnemon0d = false;
    const bool Pep::defaultMnemon0n = false;
    const bool Pep::defaultMnemon0s = false;
    const bool Pep::defaultMnemon0sf = false;
    const bool Pep::defaultMnemon0x = false;
    const bool Pep::defaultMnemon0sx = false;
    const bool Pep::defaultMnemon0sxf = false;
    const QString Pep::defaultNonUnaryMnemonic1 = "DECI";
    const bool Pep::defaultMnemon1i = false;
    const bool Pep::defaultMnemon1d = true;
    const bool Pep::defaultMnemon1n = true;
    const bool Pep::defaultMnemon1s = true;
    const bool Pep::defaultMnemon1sf = true;
    const bool Pep::defaultMnemon1x = true;
    const bool Pep::defaultMnemon1sx = true;
    const bool Pep::defaultMnemon1sxf = true;
    const QString Pep::defaultNonUnaryMnemonic2 = "DECO";
    const bool Pep::defaultMnemon2i = true;
    const bool Pep::defaultMnemon2d = true;
    const bool Pep::defaultMnemon2n = true;
    const bool Pep::defaultMnemon2s = true;
    const bool Pep::defaultMnemon2sf = true;
    const bool Pep::defaultMnemon2x = true;
    const bool Pep::defaultMnemon2sx = true;
    const bool Pep::defaultMnemon2sxf = true;
    const QString Pep::defaultNonUnaryMnemonic3 = "STRO";
    const bool Pep::defaultMnemon3i = false;
    const bool Pep::defaultMnemon3d = true;
    const bool Pep::defaultMnemon3n = true;
    const bool Pep::defaultMnemon3s = false;
    const bool Pep::defaultMnemon3sf = true;
    const bool Pep::defaultMnemon3x = false;
    const bool Pep::defaultMnemon3sx = false;
    const bool Pep::defaultMnemon3sxf = false;

//Pep::Pep()
//{
//
//}

#ifndef PEP_H
#define PEP_H

#include <QString>
#include <QMap>

class Pep
{
//public:
//    Pep();

public:
    enum MnemonicOpCode
    {
        ADDA, ADDSP, ADDX, ANDA, ANDX, ASLA, ASLX, ASRA, ASRX,
        BR, BRC, BREQ, BRGE, BRGT, BRLE, BRLT, BRNE, BRV,
        CALL, CHARI, CHARO, CPA, CPX,
        DECI, DECO,
        LDA, LDBYTEA, LDBYTEX, LDX,
        MOVSPA, MOVFLGA,
        NEGA, NEGX, NOP, NOP0, NOP1, NOP2, NOP3, NOTA, NOTX,
        ORA, ORX,
        RETTR, RET0, RET1, RET2, RET3, RET4, RET5, RET6, RET7, ROLA, ROLX, RORA, RORX,
        STA, STBYTEA, STBYTEX, STOP, STRO, STX, SUBA, SUBSP, SUBX
    };

    static QMap<MnemonicOpCode, QString> mnemonicTable;
    static void initMnemonicTable();

    // Default redefine mnemonics
    static const QString defaultUnaryMnemonic0;
    static const QString defaultUnaryMnemonic1;
    static const QString defaultUnaryMnemonic2;
    static const QString defaultUnaryMnemonic3;
    static const QString defaultNonUnaryMnemonic0;
    static const bool defaultMnemon0i;
    static const bool defaultMnemon0d;
    static const bool defaultMnemon0n;
    static const bool defaultMnemon0s;
    static const bool defaultMnemon0sf;
    static const bool defaultMnemon0x;
    static const bool defaultMnemon0sx;
    static const bool defaultMnemon0sxf;
    static const QString defaultNonUnaryMnemonic1;
    static const bool defaultMnemon1i;
    static const bool defaultMnemon1d;
    static const bool defaultMnemon1n;
    static const bool defaultMnemon1s;
    static const bool defaultMnemon1sf;
    static const bool defaultMnemon1x;
    static const bool defaultMnemon1sx;
    static const bool defaultMnemon1sxf;
    static const QString defaultNonUnaryMnemonic2;
    static const bool defaultMnemon2i;
    static const bool defaultMnemon2d;
    static const bool defaultMnemon2n;
    static const bool defaultMnemon2s;
    static const bool defaultMnemon2sf;
    static const bool defaultMnemon2x;
    static const bool defaultMnemon2sx;
    static const bool defaultMnemon2sxf;
    static const QString defaultNonUnaryMnemonic3;
    static const bool defaultMnemon3i;
    static const bool defaultMnemon3d;
    static const bool defaultMnemon3n;
    static const bool defaultMnemon3s;
    static const bool defaultMnemon3sf;
    static const bool defaultMnemon3x;
    static const bool defaultMnemon3sx;
    static const bool defaultMnemon3sxf;
};

#endif // PEP_H

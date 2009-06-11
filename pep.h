#ifndef PEP_H
#define PEP_H

#include <QMap>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QColor>

class Pep
{
public:

    // Label highlight colors
    static const QColor labelHighlightColor;
    static const QColor labelNoHighlightColor;

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

    // Constants used to define valid addressing modes for instructions
    static const int NONE;
    static const int I;
    static const int D;
    static const int N;
    static const int S;
    static const int SF;
    static const int X;
    static const int SX;
    static const int SXF;
    static const int ALL;

    // Functions for computing instruction specifiers
    static int aaaAddressField(int addressMode);
    static int aAddressField(int addressMode);
    static QString commaPrefixedMode(int addressMode);

    // Function to read text from a resource file
    static QString resToString(QString fileName);

    // Instruction mnemonics
    enum EMnemonic
    {
        ADDA, ADDSP, ADDX, ANDA, ANDX, ASLA, ASLX, ASRA, ASRX,
        BR, BRC, BREQ, BRGE, BRGT, BRLE, BRLT, BRNE, BRV,
        CALL, CHARI, CHARO, CPA, CPX,
        DECI, DECO,
        LDA, LDBYTEA, LDBYTEX, LDX,
        MOVFLGA, MOVSPA,
        NEGA, NEGX, NOP, NOP0, NOP1, NOP2, NOP3, NOTA, NOTX,
        ORA, ORX,
        RET0, RET1, RET2, RET3, RET4, RET5, RET6, RET7, RETTR, ROLA, ROLX, RORA, RORX,
        STA, STBYTEA, STBYTEX, STOP, STRO, STX, SUBA, SUBSP, SUBX
    };

    // Maps between mnemonic enums and strings
    static QMap<EMnemonic, QString> enumToMnemonMap;
    static QMap<QString, EMnemonic> mnemonToEnumMap;
    static void initEnumMnemonMaps();

    // Maps to characterize each instruction
    static QMap<EMnemonic, int> opCodeMap;
    static QMap<EMnemonic, bool> isUnaryMap;
    static QMap<EMnemonic, bool> addrModeRequiredMap;
    static QMap<EMnemonic, bool> isTrapMap;
    static void initMnemonicMaps();

    // Map to specify legal addressing modes for each instruction
    static QMap<EMnemonic, int> addrModesMap;
    static void initAddrModesMap();

    // The symbol table
    static QMap<QString, int> symbolTable;
    static QMap<QString, bool> adjustSymbolValueForBurn;

    // Map from instruction memory address to assembler listing line
    static QMap<int, int> memAddrssToAssemblerListing;

    // Decoder tables
    static QVector<EMnemonic> decodeMnemonic;
    static QVector<int> decodeAddrMode;
    static void initDecoderTables();

    // .BURN and the ROM state
    static int byteCount;
    static int burnCount;
    static int dotBurnArgument;
    static int romStartAddress;
};

#endif // PEP_H

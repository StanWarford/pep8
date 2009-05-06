#include "pep.h"

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

const int Pep::I = 1;
const int Pep::D = 2;
const int Pep::N = 4;
const int Pep::S = 8;
const int Pep::SF = 16;
const int Pep::X = 32;
const int Pep::SX = 64;
const int Pep::SXF = 128;
const int Pep::ALL = 255;

QMap<Pep::EMnemonic, QString> Pep::enumToMnemonMap;
QMap<QString, Pep::EMnemonic> Pep::mnemonToEnumMap;
void Pep::initEnumMnemonMaps()
{
    enumToMnemonMap.insert(ADDA, "ADDA"); mnemonToEnumMap.insert("ADDA", ADDA);
    enumToMnemonMap.insert(ADDSP, "ADDSP"); mnemonToEnumMap.insert("ADDSP", ADDSP);
    enumToMnemonMap.insert(ADDX, "ADDX"); mnemonToEnumMap.insert("ADDX", ADDX);
    enumToMnemonMap.insert(ANDA, "ANDA"); mnemonToEnumMap.insert("ANDA", ANDA);
    enumToMnemonMap.insert(ANDX, "ANDX"); mnemonToEnumMap.insert("ANDX", ANDX);
    enumToMnemonMap.insert(ASLA, "ASLA"); mnemonToEnumMap.insert("ASLA", ASLA);
    enumToMnemonMap.insert(ASLX, "ASLX"); mnemonToEnumMap.insert("ASLX", ASLX);
    enumToMnemonMap.insert(ASRA, "ASRA"); mnemonToEnumMap.insert("ASRA", ASRA);
    enumToMnemonMap.insert(ASRX, "ASRX"); mnemonToEnumMap.insert("ASRX", ASRX);
    enumToMnemonMap.insert(BR, "BR"); mnemonToEnumMap.insert("BR", BR);
    enumToMnemonMap.insert(BRC, "BRC"); mnemonToEnumMap.insert("BRC", BRC);
    enumToMnemonMap.insert(BREQ, "BREQ"); mnemonToEnumMap.insert("BREQ", BREQ);
    enumToMnemonMap.insert(BRGE, "BRGE"); mnemonToEnumMap.insert("BRGE", BRGE);
    enumToMnemonMap.insert(BRGT, "BRGT"); mnemonToEnumMap.insert("BRGT", BRGT);
    enumToMnemonMap.insert(BRLE, "BRLE"); mnemonToEnumMap.insert("BRLE", BRLE);
    enumToMnemonMap.insert(BRLT, "BRLT"); mnemonToEnumMap.insert("BRLT", BRLT);
    enumToMnemonMap.insert(BRNE, "BRNE"); mnemonToEnumMap.insert("BRNE", BRNE);
    enumToMnemonMap.insert(BRV, "BRV"); mnemonToEnumMap.insert("BRV", BRV);
    enumToMnemonMap.insert(CALL, "CALL"); mnemonToEnumMap.insert("CALL", CALL);
    enumToMnemonMap.insert(CHARI, "CHARI"); mnemonToEnumMap.insert("CHARI", CHARI);
    enumToMnemonMap.insert(CHARO, "CHARO"); mnemonToEnumMap.insert("CHARO", CHARO);
    enumToMnemonMap.insert(CPA, "CPA"); mnemonToEnumMap.insert("CPA", CPA);
    enumToMnemonMap.insert(CPX, "CPX"); mnemonToEnumMap.insert("CPX", CPX);
    enumToMnemonMap.insert(DECI, "DECI"); mnemonToEnumMap.insert("DECI", DECI);
    enumToMnemonMap.insert(DECO, "DECO"); mnemonToEnumMap.insert("DECO", DECO);
    enumToMnemonMap.insert(LDA, "LDA"); mnemonToEnumMap.insert("LDA", LDA);
    enumToMnemonMap.insert(LDBYTEA, "LDBYTEA"); mnemonToEnumMap.insert("LDBYTEA", LDBYTEA);
    enumToMnemonMap.insert(LDBYTEX, "LDBYTEX"); mnemonToEnumMap.insert("LDBYTEX", LDBYTEX);
    enumToMnemonMap.insert(LDX, "LDX"); mnemonToEnumMap.insert("LDX", LDX);
    enumToMnemonMap.insert(MOVSPA, "MOVSPA"); mnemonToEnumMap.insert("MOVSPA", MOVSPA);
    enumToMnemonMap.insert(MOVFLGA, "MOVFLGA"); mnemonToEnumMap.insert("MOVFLGA", MOVFLGA);
    enumToMnemonMap.insert(NEGA, "NEGA"); mnemonToEnumMap.insert("NEGA", NEGA);
    enumToMnemonMap.insert(NEGX, "NEGX"); mnemonToEnumMap.insert("NEGX", NEGX);
    enumToMnemonMap.insert(NOP, "NOP"); mnemonToEnumMap.insert("NOP", NOP);
    enumToMnemonMap.insert(NOP0, "NOP0"); mnemonToEnumMap.insert("NOP0", NOP0);
    enumToMnemonMap.insert(NOP1, "NOP1"); mnemonToEnumMap.insert("NOP1", NOP1);
    enumToMnemonMap.insert(NOP2, "NOP2"); mnemonToEnumMap.insert("NOP2", NOP2);
    enumToMnemonMap.insert(NOP3, "NOP3"); mnemonToEnumMap.insert("NOP3", NOP3);
    enumToMnemonMap.insert(NOTA, "NOTA"); mnemonToEnumMap.insert("NOTA", NOTA);
    enumToMnemonMap.insert(NOTX, "NOTX"); mnemonToEnumMap.insert("NOTX", NOTX);
    enumToMnemonMap.insert(ORA, "ORA"); mnemonToEnumMap.insert("ORA", ORA);
    enumToMnemonMap.insert(ORX, "ORX"); mnemonToEnumMap.insert("ORX", ORX);
    enumToMnemonMap.insert(RETTR, "RETTR"); mnemonToEnumMap.insert("RETTR", RETTR);
    enumToMnemonMap.insert(RET0, "RET0"); mnemonToEnumMap.insert("RET0", RET0);
    enumToMnemonMap.insert(RET1, "RET1"); mnemonToEnumMap.insert("RET1", RET1);
    enumToMnemonMap.insert(RET2, "RET2"); mnemonToEnumMap.insert("RET2", RET2);
    enumToMnemonMap.insert(RET3, "RET3"); mnemonToEnumMap.insert("RET3", RET3);
    enumToMnemonMap.insert(RET4, "RET4"); mnemonToEnumMap.insert("RET4", RET4);
    enumToMnemonMap.insert(RET5, "RET5"); mnemonToEnumMap.insert("RET5", RET5);
    enumToMnemonMap.insert(RET6, "RET6"); mnemonToEnumMap.insert("RET6", RET6);
    enumToMnemonMap.insert(RET7, "RET7"); mnemonToEnumMap.insert("RET7", RET7);
    enumToMnemonMap.insert(ROLA, "ROLA"); mnemonToEnumMap.insert("ROLA", ROLA);
    enumToMnemonMap.insert(ROLX, "ROLX"); mnemonToEnumMap.insert("ROLX", ROLX);
    enumToMnemonMap.insert(RORA, "RORA"); mnemonToEnumMap.insert("RORA", RORA);
    enumToMnemonMap.insert(RORX, "RORX"); mnemonToEnumMap.insert("RORX", RORX);
    enumToMnemonMap.insert(STA, "STA"); mnemonToEnumMap.insert("STA", STA);
    enumToMnemonMap.insert(STBYTEA, "STBYTEA"); mnemonToEnumMap.insert("STBYTEA", STBYTEA);
    enumToMnemonMap.insert(STBYTEX, "STBYTEX"); mnemonToEnumMap.insert("STBYTEX", STBYTEX);
    enumToMnemonMap.insert(STOP, "STOP"); mnemonToEnumMap.insert("STOP", STOP);
    enumToMnemonMap.insert(STRO, "STRO"); mnemonToEnumMap.insert("STRO", STRO);
    enumToMnemonMap.insert(STX, "STX"); mnemonToEnumMap.insert("STX", STX);
    enumToMnemonMap.insert(SUBA, "SUBA"); mnemonToEnumMap.insert("SUBA", SUBA);
    enumToMnemonMap.insert(SUBSP, "SUBSP"); mnemonToEnumMap.insert("SUBSP", SUBSP);
    enumToMnemonMap.insert(SUBX, "SUBX"); mnemonToEnumMap.insert("SUBX", SUBX);
}

QMap<Pep::EMnemonic, int> Pep::opCodeMap;
QMap<Pep::EMnemonic, bool> Pep::isUnaryMap;
QMap<Pep::EMnemonic, bool> Pep::addrModeRequiredMap;
QMap<Pep::EMnemonic, bool> Pep::isTrapMap;

void Pep::initMnemonicMaps()
{
    opCodeMap.insert(ADDA, 112); isUnaryMap.insert(ADDA, false); addrModeRequiredMap.insert(ADDA, true); isTrapMap.insert(ADDA, false);
}

//        ADDA, ADDSP, ADDX, ANDA, ANDX, ASLA, ASLX, ASRA, ASRX,
//        BR, BRC, BREQ, BRGE, BRGT, BRLE, BRLT, BRNE, BRV,
//        CALL, CHARI, CHARO, CPA, CPX,
//        DECI, DECO,
//        LDA, LDBYTEA, LDBYTEX, LDX,
//        MOVSPA, MOVFLGA,
//        NEGA, NEGX, NOP, NOP0, NOP1, NOP2, NOP3, NOTA, NOTX,
//        ORA, ORX,
//        RETTR, RET0, RET1, RET2, RET3, RET4, RET5, RET6, RET7, ROLA, ROLX, RORA, RORX,
//        STA, STBYTEA, STBYTEX, STOP, STRO, STX, SUBA, SUBSP, SUBX

QMap<Pep::EMnemonic, int > Pep::addrModesMap;
void Pep::initAddrModesMap()
{
    // Nonunary instructions
    addrModesMap.insert(ADDA, ALL);
    addrModesMap.insert(ADDSP, ALL);
    addrModesMap.insert(ADDX, ALL);
    addrModesMap.insert(ANDA, ALL);
    addrModesMap.insert(ANDX, ALL);
    addrModesMap.insert(BR, I | X);
    addrModesMap.insert(BRC, I | X);
    addrModesMap.insert(BREQ, I | X);
    addrModesMap.insert(BRGE, I | X);
    addrModesMap.insert(BRGT, I | X);
    addrModesMap.insert(BRLE, I | X);
    addrModesMap.insert(BRLT, I | X);
    addrModesMap.insert(BRNE, I | X);
    addrModesMap.insert(BRV, I | X);
    addrModesMap.insert(CALL, I | X);
    addrModesMap.insert(CHARI, D | N | S | SF | X | SX | SXF);
    addrModesMap.insert(CHARO, ALL);
    addrModesMap.insert(CPA, ALL);
    addrModesMap.insert(CPX, ALL);
    addrModesMap.insert(LDA, ALL);
    addrModesMap.insert(LDBYTEA, ALL);
    addrModesMap.insert(LDBYTEX, ALL);
    addrModesMap.insert(LDX, ALL);
    addrModesMap.insert(ORA, ALL);
    addrModesMap.insert(ORX, ALL);
    addrModesMap.insert(STA, D | N | S | SF | X | SX | SXF);
    addrModesMap.insert(STBYTEA, D | N | S | SF | X | SX | SXF);
    addrModesMap.insert(STBYTEX, D | N | S | SF | X | SX | SXF);
    addrModesMap.insert(STX, D | N | S | SF | X | SX | SXF);
    addrModesMap.insert(SUBA, ALL);
    addrModesMap.insert(SUBSP, ALL);
    addrModesMap.insert(SUBX, ALL);
    // Nonunary trap instructions
    int addrMode;
    addrMode = 0;
    if (defaultMnemon0i) addrMode |= I;
    if (defaultMnemon0d) addrMode |= D;
    if (defaultMnemon0n) addrMode |= N;
    if (defaultMnemon0s) addrMode |= S;
    if (defaultMnemon0sf) addrMode |= SF;
    if (defaultMnemon0x) addrMode |= X;
    if (defaultMnemon0sx) addrMode |= SX;
    if (defaultMnemon0sxf) addrMode |= SXF;
    addrModesMap.insert(NOP, addrMode);
    addrMode = 0;
    if (defaultMnemon1i) addrMode |= I;
    if (defaultMnemon1d) addrMode |= D;
    if (defaultMnemon1n) addrMode |= N;
    if (defaultMnemon1s) addrMode |= S;
    if (defaultMnemon1sf) addrMode |= SF;
    if (defaultMnemon1x) addrMode |= X;
    if (defaultMnemon1sx) addrMode |= SX;
    if (defaultMnemon1sxf) addrMode |= SXF;
    addrModesMap.insert(DECI, addrMode);
    addrMode = 0;
    if (defaultMnemon2i) addrMode |= I;
    if (defaultMnemon2d) addrMode |= D;
    if (defaultMnemon2n) addrMode |= N;
    if (defaultMnemon2s) addrMode |= S;
    if (defaultMnemon2sf) addrMode |= SF;
    if (defaultMnemon2x) addrMode |= X;
    if (defaultMnemon2sx) addrMode |= SX;
    if (defaultMnemon2sxf) addrMode |= SXF;
    addrModesMap.insert(DECO, addrMode);
    addrMode = 0;
    if (defaultMnemon3i) addrMode |= I;
    if (defaultMnemon3d) addrMode |= D;
    if (defaultMnemon3n) addrMode |= N;
    if (defaultMnemon3s) addrMode |= S;
    if (defaultMnemon3sf) addrMode |= SF;
    if (defaultMnemon3x) addrMode |= X;
    if (defaultMnemon3sx) addrMode |= SX;
    if (defaultMnemon3sxf) addrMode |= SXF;
    addrModesMap.insert(STRO, addrMode);
}

QMap<QString, int> Pep::symbolTable;
void Pep::initSymbolTable()
{
    symbolTable.clear();
}

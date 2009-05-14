#include "pep.h"

// Default redefine mnemonics
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

// Constants used to define valid addressing modes for instructions
const int Pep::I = 1;
const int Pep::D = 2;
const int Pep::N = 4;
const int Pep::S = 8;
const int Pep::SF = 16;
const int Pep::X = 32;
const int Pep::SX = 64;
const int Pep::SXF = 128;
const int Pep::ALL = 255;

// Maps between mnemonic enums and strings
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

// Maps to characterize each instruction
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

// Map to specify legal addressing modes for each instruction
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

// The symbol table
QMap<QString, int> Pep::symbolTable;

// Map from instruction memory address to assembler listing line
QMap<int, int> Pep::memAddrssToAssemblerListing;

// Decoder tables
QVector<Pep::EMnemonic> Pep::decodeMnemonic(256);
QVector<int> Pep::decodeAddrMode(256);
void Pep::initDecoderTables()
{
    decodeMnemonic[0] = STOP; decodeAddrMode[0] = 0;
    decodeMnemonic[1] = RETTR; decodeAddrMode[1] = 0;
    decodeMnemonic[2] = MOVSPA; decodeAddrMode[2] = 0;
    decodeMnemonic[3] = MOVFLGA; decodeAddrMode[3] = 0;

    decodeMnemonic[4] = BR; decodeAddrMode[4] = I;
    decodeMnemonic[5] = BR; decodeAddrMode[5] = X;
    decodeMnemonic[6] = BRLE; decodeAddrMode[6] = I;
    decodeMnemonic[7] = BRLE; decodeAddrMode[7] = X;
    decodeMnemonic[8] = BRLT; decodeAddrMode[8] = I;
    decodeMnemonic[9] = BRLT; decodeAddrMode[9] = X;
    decodeMnemonic[10] = BREQ; decodeAddrMode[10] = I;
    decodeMnemonic[11] = BREQ; decodeAddrMode[11] = X;
    decodeMnemonic[12] = BRNE; decodeAddrMode[12] = I;
    decodeMnemonic[13] = BRNE; decodeAddrMode[13] = X;
    decodeMnemonic[14] = BRGE; decodeAddrMode[14] = I;
    decodeMnemonic[15] = BRGE; decodeAddrMode[15] = X;
    decodeMnemonic[16] = BRGT; decodeAddrMode[16] = I;
    decodeMnemonic[17] = BRGT; decodeAddrMode[17] = X;
    decodeMnemonic[18] = BRV; decodeAddrMode[18] = I;
    decodeMnemonic[19] = BRV; decodeAddrMode[19] = X;
    decodeMnemonic[20] = BRC; decodeAddrMode[20] = I;
    decodeMnemonic[21] = BRC; decodeAddrMode[21] = X;
    decodeMnemonic[22] = CALL; decodeAddrMode[22] = I;
    decodeMnemonic[23] = CALL; decodeAddrMode[23] = X;

    decodeMnemonic[24] = NOTA; decodeAddrMode[24] = 0;
    decodeMnemonic[25] = NOTX; decodeAddrMode[25] = 0;
    decodeMnemonic[26] = NEGA; decodeAddrMode[26] = 0;
    decodeMnemonic[27] = NEGX; decodeAddrMode[27] = 0;
    decodeMnemonic[28] = ASLA; decodeAddrMode[28] = 0;
    decodeMnemonic[29] = ASLX; decodeAddrMode[29] = 0;
    decodeMnemonic[30] = ASRA; decodeAddrMode[30] = 0;
    decodeMnemonic[31] = ASRX; decodeAddrMode[31] = 0;
    decodeMnemonic[32] = ROLA; decodeAddrMode[32] = 0;
    decodeMnemonic[33] = ROLX; decodeAddrMode[33] = 0;
    decodeMnemonic[34] = RORA; decodeAddrMode[34] = 0;
    decodeMnemonic[35] = RORX; decodeAddrMode[35] = 0;

    // Note that the trap instructions are all unary at the machine level
    decodeMnemonic[36] = NOP0; decodeAddrMode[36] = 0;
    decodeMnemonic[37] = NOP1; decodeAddrMode[37] = 0;
    decodeMnemonic[38] = NOP2; decodeAddrMode[38] = 0;
    decodeMnemonic[39] = NOP3; decodeAddrMode[39] = 0;

    decodeMnemonic[40] = NOP; decodeAddrMode[40] = 0;
    decodeMnemonic[41] = NOP; decodeAddrMode[41] = 0;
    decodeMnemonic[42] = NOP; decodeAddrMode[42] = 0;
    decodeMnemonic[43] = NOP; decodeAddrMode[43] = 0;
    decodeMnemonic[44] = NOP; decodeAddrMode[44] = 0;
    decodeMnemonic[45] = NOP; decodeAddrMode[45] = 0;
    decodeMnemonic[46] = NOP; decodeAddrMode[46] = 0;
    decodeMnemonic[47] = NOP; decodeAddrMode[47] = 0;

    decodeMnemonic[48] = DECI; decodeAddrMode[48] = 0;
    decodeMnemonic[49] = DECI; decodeAddrMode[49] = 0;
    decodeMnemonic[50] = DECI; decodeAddrMode[50] = 0;
    decodeMnemonic[51] = DECI; decodeAddrMode[51] = 0;
    decodeMnemonic[52] = DECI; decodeAddrMode[52] = 0;
    decodeMnemonic[53] = DECI; decodeAddrMode[53] = 0;
    decodeMnemonic[54] = DECI; decodeAddrMode[54] = 0;
    decodeMnemonic[55] = DECI; decodeAddrMode[55] = 0;

    decodeMnemonic[56] = DECO; decodeAddrMode[56] = 0;
    decodeMnemonic[57] = DECO; decodeAddrMode[57] = 0;
    decodeMnemonic[58] = DECO; decodeAddrMode[58] = 0;
    decodeMnemonic[59] = DECO; decodeAddrMode[59] = 0;
    decodeMnemonic[60] = DECO; decodeAddrMode[60] = 0;
    decodeMnemonic[61] = DECO; decodeAddrMode[61] = 0;
    decodeMnemonic[62] = DECO; decodeAddrMode[62] = 0;
    decodeMnemonic[63] = DECO; decodeAddrMode[63] = 0;

    decodeMnemonic[64] = STRO; decodeAddrMode[64] = 0;
    decodeMnemonic[65] = STRO; decodeAddrMode[65] = 0;
    decodeMnemonic[66] = STRO; decodeAddrMode[66] = 0;
    decodeMnemonic[67] = STRO; decodeAddrMode[67] = 0;
    decodeMnemonic[68] = STRO; decodeAddrMode[68] = 0;
    decodeMnemonic[69] = STRO; decodeAddrMode[69] = 0;
    decodeMnemonic[70] = STRO; decodeAddrMode[70] = 0;
    decodeMnemonic[71] = STRO; decodeAddrMode[71] = 0;

    decodeMnemonic[72] = CHARI; decodeAddrMode[72] = I;
    decodeMnemonic[73] = CHARI; decodeAddrMode[73] = D;
    decodeMnemonic[74] = CHARI; decodeAddrMode[74] = N;
    decodeMnemonic[75] = CHARI; decodeAddrMode[75] = S;
    decodeMnemonic[76] = CHARI; decodeAddrMode[76] = SF;
    decodeMnemonic[77] = CHARI; decodeAddrMode[77] = X;
    decodeMnemonic[78] = CHARI; decodeAddrMode[78] = SX;
    decodeMnemonic[79] = CHARI; decodeAddrMode[79] = SXF;

    decodeMnemonic[80] = CHARO; decodeAddrMode[80] = I;
    decodeMnemonic[81] = CHARO; decodeAddrMode[81] = D;
    decodeMnemonic[82] = CHARO; decodeAddrMode[82] = N;
    decodeMnemonic[83] = CHARO; decodeAddrMode[83] = S;
    decodeMnemonic[84] = CHARO; decodeAddrMode[84] = SF;
    decodeMnemonic[85] = CHARO; decodeAddrMode[85] = X;
    decodeMnemonic[86] = CHARO; decodeAddrMode[86] = SX;
    decodeMnemonic[87] = CHARO; decodeAddrMode[87] = SXF;

    decodeMnemonic[88] = RET0; decodeAddrMode[88] = 0;
    decodeMnemonic[89] = RET1; decodeAddrMode[89] = 0;
    decodeMnemonic[90] = RET2; decodeAddrMode[90] = 0;
    decodeMnemonic[91] = RET3; decodeAddrMode[91] = 0;
    decodeMnemonic[92] = RET4; decodeAddrMode[92] = 0;
    decodeMnemonic[93] = RET5; decodeAddrMode[93] = 0;
    decodeMnemonic[94] = RET6; decodeAddrMode[94] = 0;
    decodeMnemonic[95] = RET7; decodeAddrMode[95] = 0;

    decodeMnemonic[96] = ADDSP; decodeAddrMode[96] = I;
    decodeMnemonic[97] = ADDSP; decodeAddrMode[97] = D;
    decodeMnemonic[98] = ADDSP; decodeAddrMode[98] = N;
    decodeMnemonic[99] = ADDSP; decodeAddrMode[99] = S;
    decodeMnemonic[100] = ADDSP; decodeAddrMode[100] = SF;
    decodeMnemonic[101] = ADDSP; decodeAddrMode[101] = X;
    decodeMnemonic[102] = ADDSP; decodeAddrMode[102] = SX;
    decodeMnemonic[103] = ADDSP; decodeAddrMode[103] = SXF;

    decodeMnemonic[104] = SUBSP; decodeAddrMode[104] = I;
    decodeMnemonic[105] = SUBSP; decodeAddrMode[105] = D;
    decodeMnemonic[106] = SUBSP; decodeAddrMode[106] = N;
    decodeMnemonic[107] = SUBSP; decodeAddrMode[107] = S;
    decodeMnemonic[108] = SUBSP; decodeAddrMode[108] = SF;
    decodeMnemonic[109] = SUBSP; decodeAddrMode[109] = X;
    decodeMnemonic[110] = SUBSP; decodeAddrMode[110] = SX;
    decodeMnemonic[111] = SUBSP; decodeAddrMode[111] = SXF;

    decodeMnemonic[112] = ADDA; decodeAddrMode[112] = I;
    decodeMnemonic[113] = ADDA; decodeAddrMode[113] = D;
    decodeMnemonic[114] = ADDA; decodeAddrMode[114] = N;
    decodeMnemonic[115] = ADDA; decodeAddrMode[115] = S;
    decodeMnemonic[116] = ADDA; decodeAddrMode[116] = SF;
    decodeMnemonic[117] = ADDA; decodeAddrMode[117] = X;
    decodeMnemonic[118] = ADDA; decodeAddrMode[118] = SX;
    decodeMnemonic[119] = ADDA; decodeAddrMode[119] = SXF;

    decodeMnemonic[120] = ADDX; decodeAddrMode[120] = I;
    decodeMnemonic[121] = ADDX; decodeAddrMode[121] = D;
    decodeMnemonic[122] = ADDX; decodeAddrMode[122] = N;
    decodeMnemonic[123] = ADDX; decodeAddrMode[123] = S;
    decodeMnemonic[124] = ADDX; decodeAddrMode[124] = SF;
    decodeMnemonic[125] = ADDX; decodeAddrMode[125] = X;
    decodeMnemonic[126] = ADDX; decodeAddrMode[126] = SX;
    decodeMnemonic[127] = ADDX; decodeAddrMode[127] = SXF;

    decodeMnemonic[128] = SUBA; decodeAddrMode[128] = I;
    decodeMnemonic[129] = SUBA; decodeAddrMode[129] = D;
    decodeMnemonic[130] = SUBA; decodeAddrMode[130] = N;
    decodeMnemonic[131] = SUBA; decodeAddrMode[131] = S;
    decodeMnemonic[132] = SUBA; decodeAddrMode[132] = SF;
    decodeMnemonic[133] = SUBA; decodeAddrMode[133] = X;
    decodeMnemonic[134] = SUBA; decodeAddrMode[134] = SX;
    decodeMnemonic[135] = SUBA; decodeAddrMode[135] = SXF;

    decodeMnemonic[136] = SUBX; decodeAddrMode[136] = I;
    decodeMnemonic[137] = SUBX; decodeAddrMode[137] = D;
    decodeMnemonic[138] = SUBX; decodeAddrMode[138] = N;
    decodeMnemonic[139] = SUBX; decodeAddrMode[139] = S;
    decodeMnemonic[140] = SUBX; decodeAddrMode[140] = SF;
    decodeMnemonic[141] = SUBX; decodeAddrMode[141] = X;
    decodeMnemonic[142] = SUBX; decodeAddrMode[142] = SX;
    decodeMnemonic[143] = SUBX; decodeAddrMode[143] = SXF;

    decodeMnemonic[144] = ANDA; decodeAddrMode[144] = I;
    decodeMnemonic[145] = ANDA; decodeAddrMode[145] = D;
    decodeMnemonic[146] = ANDA; decodeAddrMode[146] = N;
    decodeMnemonic[147] = ANDA; decodeAddrMode[147] = S;
    decodeMnemonic[148] = ANDA; decodeAddrMode[148] = SF;
    decodeMnemonic[149] = ANDA; decodeAddrMode[149] = X;
    decodeMnemonic[150] = ANDA; decodeAddrMode[150] = SX;
    decodeMnemonic[151] = ANDA; decodeAddrMode[151] = SXF;

    decodeMnemonic[152] = ANDX; decodeAddrMode[152] = I;
    decodeMnemonic[153] = ANDX; decodeAddrMode[153] = D;
    decodeMnemonic[154] = ANDX; decodeAddrMode[154] = N;
    decodeMnemonic[155] = ANDX; decodeAddrMode[155] = S;
    decodeMnemonic[156] = ANDX; decodeAddrMode[156] = SF;
    decodeMnemonic[157] = ANDX; decodeAddrMode[157] = X;
    decodeMnemonic[158] = ANDX; decodeAddrMode[158] = SX;
    decodeMnemonic[159] = ANDX; decodeAddrMode[159] = SXF;

    decodeMnemonic[160] = ORA; decodeAddrMode[160] = I;
    decodeMnemonic[161] = ORA; decodeAddrMode[161] = D;
    decodeMnemonic[162] = ORA; decodeAddrMode[162] = N;
    decodeMnemonic[163] = ORA; decodeAddrMode[163] = S;
    decodeMnemonic[164] = ORA; decodeAddrMode[164] = SF;
    decodeMnemonic[165] = ORA; decodeAddrMode[165] = X;
    decodeMnemonic[166] = ORA; decodeAddrMode[166] = SX;
    decodeMnemonic[167] = ORA; decodeAddrMode[167] = SXF;

    decodeMnemonic[168] = ORX; decodeAddrMode[168] = I;
    decodeMnemonic[169] = ORX; decodeAddrMode[169] = D;
    decodeMnemonic[170] = ORX; decodeAddrMode[170] = N;
    decodeMnemonic[171] = ORX; decodeAddrMode[171] = S;
    decodeMnemonic[172] = ORX; decodeAddrMode[172] = SF;
    decodeMnemonic[173] = ORX; decodeAddrMode[173] = X;
    decodeMnemonic[174] = ORX; decodeAddrMode[174] = SX;
    decodeMnemonic[175] = ORX; decodeAddrMode[175] = SXF;

    decodeMnemonic[176] = CPA; decodeAddrMode[176] = I;
    decodeMnemonic[177] = CPA; decodeAddrMode[177] = D;
    decodeMnemonic[178] = CPA; decodeAddrMode[178] = N;
    decodeMnemonic[179] = CPA; decodeAddrMode[179] = S;
    decodeMnemonic[180] = CPA; decodeAddrMode[180] = SF;
    decodeMnemonic[181] = CPA; decodeAddrMode[181] = X;
    decodeMnemonic[182] = CPA; decodeAddrMode[182] = SX;
    decodeMnemonic[183] = CPA; decodeAddrMode[183] = SXF;

    decodeMnemonic[184] = CPX; decodeAddrMode[184] = I;
    decodeMnemonic[185] = CPX; decodeAddrMode[185] = D;
    decodeMnemonic[186] = CPX; decodeAddrMode[186] = N;
    decodeMnemonic[187] = CPX; decodeAddrMode[187] = S;
    decodeMnemonic[188] = CPX; decodeAddrMode[188] = SF;
    decodeMnemonic[189] = CPX; decodeAddrMode[189] = X;
    decodeMnemonic[190] = CPX; decodeAddrMode[190] = SX;
    decodeMnemonic[191] = CPX; decodeAddrMode[191] = SXF;

    decodeMnemonic[192] = LDA; decodeAddrMode[192] = I;
    decodeMnemonic[193] = LDA; decodeAddrMode[193] = D;
    decodeMnemonic[194] = LDA; decodeAddrMode[194] = N;
    decodeMnemonic[195] = LDA; decodeAddrMode[195] = S;
    decodeMnemonic[196] = LDA; decodeAddrMode[196] = SF;
    decodeMnemonic[197] = LDA; decodeAddrMode[197] = X;
    decodeMnemonic[198] = LDA; decodeAddrMode[198] = SX;
    decodeMnemonic[199] = LDA; decodeAddrMode[199] = SXF;

    decodeMnemonic[200] = LDX; decodeAddrMode[200] = I;
    decodeMnemonic[201] = LDX; decodeAddrMode[201] = D;
    decodeMnemonic[202] = LDX; decodeAddrMode[202] = N;
    decodeMnemonic[203] = LDX; decodeAddrMode[203] = S;
    decodeMnemonic[204] = LDX; decodeAddrMode[204] = SF;
    decodeMnemonic[205] = LDX; decodeAddrMode[205] = X;
    decodeMnemonic[206] = LDX; decodeAddrMode[206] = SX;
    decodeMnemonic[207] = LDX; decodeAddrMode[207] = SXF;

    decodeMnemonic[208] = LDBYTEA; decodeAddrMode[208] = I;
    decodeMnemonic[209] = LDBYTEA; decodeAddrMode[209] = D;
    decodeMnemonic[210] = LDBYTEA; decodeAddrMode[210] = N;
    decodeMnemonic[211] = LDBYTEA; decodeAddrMode[211] = S;
    decodeMnemonic[212] = LDBYTEA; decodeAddrMode[212] = SF;
    decodeMnemonic[213] = LDBYTEA; decodeAddrMode[213] = X;
    decodeMnemonic[214] = LDBYTEA; decodeAddrMode[214] = SX;
    decodeMnemonic[215] = LDBYTEA; decodeAddrMode[215] = SXF;

    decodeMnemonic[216] = LDBYTEX; decodeAddrMode[216] = I;
    decodeMnemonic[217] = LDBYTEX; decodeAddrMode[217] = D;
    decodeMnemonic[218] = LDBYTEX; decodeAddrMode[218] = N;
    decodeMnemonic[219] = LDBYTEX; decodeAddrMode[219] = S;
    decodeMnemonic[220] = LDBYTEX; decodeAddrMode[220] = SF;
    decodeMnemonic[221] = LDBYTEX; decodeAddrMode[221] = X;
    decodeMnemonic[222] = LDBYTEX; decodeAddrMode[222] = SX;
    decodeMnemonic[223] = LDBYTEX; decodeAddrMode[223] = SXF;

    decodeMnemonic[224] = STA; decodeAddrMode[224] = I;
    decodeMnemonic[225] = STA; decodeAddrMode[225] = D;
    decodeMnemonic[226] = STA; decodeAddrMode[226] = N;
    decodeMnemonic[227] = STA; decodeAddrMode[227] = S;
    decodeMnemonic[228] = STA; decodeAddrMode[228] = SF;
    decodeMnemonic[229] = STA; decodeAddrMode[229] = X;
    decodeMnemonic[230] = STA; decodeAddrMode[230] = SX;
    decodeMnemonic[231] = STA; decodeAddrMode[231] = SXF;

    decodeMnemonic[232] = STX; decodeAddrMode[232] = I;
    decodeMnemonic[233] = STX; decodeAddrMode[233] = D;
    decodeMnemonic[234] = STX; decodeAddrMode[234] = N;
    decodeMnemonic[235] = STX; decodeAddrMode[235] = S;
    decodeMnemonic[236] = STX; decodeAddrMode[236] = SF;
    decodeMnemonic[237] = STX; decodeAddrMode[237] = X;
    decodeMnemonic[238] = STX; decodeAddrMode[238] = SX;
    decodeMnemonic[239] = STX; decodeAddrMode[239] = SXF;

    decodeMnemonic[240] = STBYTEA; decodeAddrMode[240] = I;
    decodeMnemonic[241] = STBYTEA; decodeAddrMode[241] = D;
    decodeMnemonic[242] = STBYTEA; decodeAddrMode[242] = N;
    decodeMnemonic[243] = STBYTEA; decodeAddrMode[243] = S;
    decodeMnemonic[244] = STBYTEA; decodeAddrMode[244] = SF;
    decodeMnemonic[245] = STBYTEA; decodeAddrMode[245] = X;
    decodeMnemonic[246] = STBYTEA; decodeAddrMode[246] = SX;
    decodeMnemonic[247] = STBYTEA; decodeAddrMode[247] = SXF;

    decodeMnemonic[248] = STBYTEX; decodeAddrMode[248] = I;
    decodeMnemonic[249] = STBYTEX; decodeAddrMode[249] = D;
    decodeMnemonic[250] = STBYTEX; decodeAddrMode[250] = N;
    decodeMnemonic[251] = STBYTEX; decodeAddrMode[251] = S;
    decodeMnemonic[252] = STBYTEX; decodeAddrMode[252] = SF;
    decodeMnemonic[253] = STBYTEX; decodeAddrMode[253] = X;
    decodeMnemonic[254] = STBYTEX; decodeAddrMode[254] = SX;
    decodeMnemonic[255] = STBYTEX; decodeAddrMode[255] = SXF;
}

// The machine
QVector<int> Pep::Mem(65536);
int Pep::nBit, Pep::zBit, Pep::vBit, Pep::cBit;
int Pep::accumulator;
int Pep::indexRegister;
int Pep::stackPointer;
int Pep::programCounter;
int Pep::instructionRegister;
int Pep::operandSpecifier;

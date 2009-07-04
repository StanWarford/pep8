#ifndef CODE_H
#define CODE_H

#include "pep.h"
#include "enu.h"

class Argument; // Forward declaration for attributes of code classes.

// Abstract Code class
class Code
{
    friend class Asm;
public:
    virtual ~Code() { }
    virtual void appendObjectCode(QList<int> &objectCode) = 0;
    virtual void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                                  QMap<int, int> &memAddrssToAssemblerListing) = 0;
    void adjustMemAddress(int addressDelta) { memAddress += addressDelta; }

protected:
    int memAddress;
    QString symbolDef;
    QString comment;
};

// Concrete code classes
class UnaryInstruction: public Code
{
    friend class Asm;
private:
    Enu::EMnemonic mnemonic;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                          QMap<int, int> &memAddrssToAssemblerListing);
};

class NonUnaryInstruction: public Code
{
    friend class Asm;
private:
    Enu::EMnemonic mnemonic;
    Enu::EAddrMode addressingMode;
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                          QMap<int, int> &memAddrssToAssemblerListing);
};

class DotAddrss: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                          QMap<int, int> &memAddrssToAssemblerListing);
};

class DotAscii: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                          QMap<int, int> &memAddrssToAssemblerListing);
};

class DotBlock: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                          QMap<int, int> &memAddrssToAssemblerListing);
};

class DotBurn: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                          QMap<int, int> &memAddrssToAssemblerListing);
};

class DotByte: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                          QMap<int, int> &memAddrssToAssemblerListing);
};

class DotEnd: public Code
{
    friend class Asm;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                          QMap<int, int> &memAddrssToAssemblerListing);
};

class DotEquate: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                          QMap<int, int> &memAddrssToAssemblerListing);
};

class DotWord: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                          QMap<int, int> &memAddrssToAssemblerListing);
};

class CommentOnly: public Code
{
    friend class Asm;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                          QMap<int, int> &memAddrssToAssemblerListing);
};

class BlankLine: public Code
{
    friend class Asm;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox,
                          QMap<int, int> &memAddrssToAssemblerListing);
};

#endif // CODE_H

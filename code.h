#ifndef CODE_H
#define CODE_H

#include "pep.h"

class Argument; // Forward declaration for attributes of code classes.

// Abstract Code class
class Code
{
    friend class Asm;
public:
    virtual ~Code() { }
    virtual void appendObjectCode(QList<int> &objectCode) = 0;
    virtual void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox) = 0;
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
    Pep::EMnemonic mnemonic;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox);
};

class NonUnaryInstruction: public Code
{
    friend class Asm;
private:
    Pep::EMnemonic mnemonic;
    int addressingMode;
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox);
};

class DotAddrss: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox);
};

class DotAscii: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox);
};

class DotBlock: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox);
};

class DotBurn: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox);
};

class DotByte: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox);
};

class DotEnd: public Code
{
    friend class Asm;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox);
};

class DotEquate: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox);
};

class DotWord: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox);
};

class CommentOnly: public Code
{
    friend class Asm;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox);
};

class BlankLine: public Code
{
    friend class Asm;
public:
    void appendObjectCode(QList<int> &objectCode);
    void appendSourceLine(QStringList &assemblerListingList, QStringList &listingTraceList, QList<bool> &hasCheckBox);
};

#endif // CODE_H

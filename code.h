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

private:
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
};

class DotAddress: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
};

class DotAscii: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
};

class DotBlock: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
};

class DotBurn: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
};

class DotByte: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
};

class DotEnd: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
};

class DotEquate: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
};

class DotWord: public Code
{
    friend class Asm;
private:
    Argument *argument;
public:
    void appendObjectCode(QList<int> &objectCode);
};

class CommentOnly: public Code
{
    friend class Asm;
public:
    void appendObjectCode(QList<int> &objectCode);
};

class BlankLine: public Code
{
    friend class Asm;
public:
    void appendObjectCode(QList<int> &objectCode);
};

#endif // CODE_H

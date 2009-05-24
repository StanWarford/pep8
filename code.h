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
};

class NonUnaryInstruction: public Code
{
    friend class Asm;
private:
    Pep::EMnemonic mnemonic;
    Argument *argument;
    int addressingMode;
};

class DotAddress: public Code
{
    friend class Asm;
private:
    Argument *argument;
};

class DotAscii: public Code
{
    friend class Asm;
private:
    Argument *argument;
};

class DotBlock: public Code
{
    friend class Asm;
private:
    Argument *argument;
};

class DotBurn: public Code
{
    friend class Asm;
private:
    Argument *argument;
};

class DotByte: public Code
{
    friend class Asm;
private:
    Argument *argument;
};

class DotEnd: public Code
{
    friend class Asm;
private:
    Argument *argument;
};

class DotEquate: public Code
{
    friend class Asm;
private:
    Argument *argument;
};

class DotWord: public Code
{
    friend class Asm;
private:
    Argument *argument;
};

class CommentOnly: public Code
{
    friend class Asm;
};

class BlankLine: public Code
{
    friend class Asm;
};

#endif // CODE_H

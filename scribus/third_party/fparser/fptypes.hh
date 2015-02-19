/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Copyright: Juha Nieminen                                                *|
\***************************************************************************/

// NOTE:
// This file contains only internal types for the function parser library.
// You don't need to include this file in your code. Include "fparser.hh"
// only.

#ifndef ONCE_FPARSER_TYPES_H_
#define ONCE_FPARSER_TYPES_H_

#include "fpconfig.hh"
#include <cmath>

namespace FUNCTIONPARSERTYPES
{
// The functions must be in alphabetical order:
    enum OPCODE
    {
        cAbs,
        cAcos, cAcosh,
        cAsin, cAsinh,
        cAtan, cAtan2, cAtanh,
        cCeil,
        cCos, cCosh, cCot, cCsc,
        cEval,
        cExp, cExp2, cFloor, cIf, cInt, cLog, cLog10, cLog2, cMax, cMin,
        cPow, cSec, cSin, cSinh, cSqrt, cTan, cTanh,

// These do not need any ordering:
        cImmed, cJump,
        cNeg, cAdd, cSub, cMul, cDiv, cMod,
        cEqual, cNEqual, cLess, cLessOrEq, cGreater, cGreaterOrEq,
        cNot, cAnd, cOr,
        cNotNot, /* Protects the double-not sequence from optimizations */

        cDeg, cRad,

        cFCall, cPCall,
        cRPow,

#ifdef FP_SUPPORT_OPTIMIZER
        cVar,   /* Denotes a variable in CodeTree (not used by bytecode) */
        cFetch, /* Same as Dup, except with absolute index
                   (next value is index) */
        cPopNMov, /* cPopNMov(x,y) moves [y] to [x] and deletes anything
                     above [x] */
#endif

        cDup,   /* Duplicates the last value in the stack:
                   Pop A, Push A, Push A */
        cInv,   /* Inverts the last value in the stack (x = 1/x) */
        cSqr,   /* squares the last operand in the stack, no push/pop */
        cRDiv,  /* reverse division (not x/y, but y/x) */
        cRSub,  /* reverse subtraction (not x-y, but y-x) */
        cRSqrt, /* inverse square-root) */

        cNop,
        VarBegin
    };

#ifdef ONCE_FPARSER_H_
    struct FuncDefinition
    {
        const char* name;
        unsigned nameLength;
        OPCODE   opcode;
        unsigned params;
        bool enabled;

        // This is basically strcmp(), but taking 'nameLength' as string
        // length (not ending '\0'):
        bool operator<(const FuncDefinition& rhs) const
        {
            for(unsigned i = 0; i < nameLength; ++i)
            {
                if(i == rhs.nameLength) return false;
                const char c1 = name[i], c2 = rhs.name[i];
                if(c1 < c2) return true;
                if(c2 < c1) return false;
            }
            return nameLength < rhs.nameLength;
        }
    };

#ifndef FP_DISABLE_EVAL
#define FP_EVAL_FUNCTION_ENABLED true
#else
#define FP_EVAL_FUNCTION_ENABLED false
#endif

// This list must be in alphabetical order:
    const FuncDefinition Functions[]=
    {
        { "abs", 3, cAbs, 1, true },
        { "acos", 4, cAcos, 1, true },
        { "acosh", 5, cAcosh, 1, true },
        { "asin", 4, cAsin, 1, true },
        { "asinh", 5, cAsinh, 1, true },
        { "atan", 4, cAtan, 1, true },
        { "atan2", 5, cAtan2, 2, true },
        { "atanh", 5, cAtanh, 1, true },
        { "ceil", 4, cCeil, 1, true },
        { "cos", 3, cCos, 1, true },
        { "cosh", 4, cCosh, 1, true },
        { "cot", 3, cCot, 1, true },
        { "csc", 3, cCsc, 1, true },
        { "eval", 4, cEval, 0, FP_EVAL_FUNCTION_ENABLED },
        { "exp", 3, cExp, 1, true },
        { "exp2", 4, cExp2, 1, true },
        { "floor", 5, cFloor, 1, true },
        { "if", 2, cIf, 0, true },
        { "int", 3, cInt, 1, true },
        { "log", 3, cLog, 1, true },
        { "log10", 5, cLog10, 1, true },
        { "log2", 4, cLog2, 1, true },
        { "max", 3, cMax, 2, true },
        { "min", 3, cMin, 2, true },
        { "pow", 3, cPow, 2, true },
        { "sec", 3, cSec, 1, true },
        { "sin", 3, cSin, 1, true },
        { "sinh", 4, cSinh, 1, true },
        { "sqrt", 4, cSqrt, 1, true },
        { "tan", 3, cTan, 1, true },
        { "tanh", 4, cTanh, 1, true }
    };

    struct NamePtr
    {
        const char* name;
        unsigned nameLength;

        NamePtr(const char* n, unsigned l): name(n), nameLength(l) {}

        inline bool operator<(const NamePtr& rhs) const
        {
            for(unsigned i = 0; i < nameLength; ++i)
            {
                if(i == rhs.nameLength) return false;
                const char c1 = name[i], c2 = rhs.name[i];
                if(c1 < c2) return true;
                if(c2 < c1) return false;
            }
            return nameLength < rhs.nameLength;
        }
    };

    struct NameData
    {
        enum DataType { CONSTANT, UNIT, FUNC_PTR, PARSER_PTR };

        DataType type;
        std::string name;

        union
        {
            unsigned index;
            double value;
        };

        NameData(DataType t, const std::string& n): type(t), name(n) {}

        inline bool operator<(const NameData& rhs) const
        {
            return name < rhs.name;
        }
    };

    const unsigned FUNC_AMOUNT = sizeof(Functions)/sizeof(Functions[0]);

    // -1 = (lhs < rhs); 0 = (lhs == rhs); 1 = (lhs > rhs)
    inline int compare(const FuncDefinition& lhs, const NamePtr& rhs)
    {
        for(unsigned i = 0; i < lhs.nameLength; ++i)
        {
            if(i == rhs.nameLength) return 1;
            const char c1 = lhs.name[i], c2 = rhs.name[i];
            if(c1 < c2) return -1;
            if(c2 < c1) return 1;
        }
        return lhs.nameLength < rhs.nameLength ? -1 : 0;
    }

    inline const FuncDefinition* findFunction(const NamePtr& functionName)
    {
        const FuncDefinition* first = Functions;
        const FuncDefinition* last = Functions + FUNC_AMOUNT;

        while(first < last)
        {
            const FuncDefinition* middle = first+(last-first)/2;
            const int comp = compare(*middle, functionName);
            if(comp == 0) return middle;
            if(comp < 0) first = middle+1;
            else last = middle;
        }
        return 0;
    }

#ifndef FP_SUPPORT_ASINH
    inline double fp_asinh(double x) { return log(x + sqrt(x*x + 1)); }
    inline double fp_acosh(double x) { return log(x + sqrt(x*x - 1)); }
    inline double fp_atanh(double x) { return log( (1+x) / (1-x) ) * 0.5; }
#else
    inline double fp_asinh(double x) { return asinh(x); }
    inline double fp_acosh(double x) { return acosh(x); }
    inline double fp_atanh(double x) { return atanh(x); }
#endif // FP_SUPPORT_ASINH

#ifdef FP_EPSILON
    inline bool FloatEqual(double a, double b)
    { return fabs(a - b) <= FP_EPSILON; }
#else
    inline bool FloatEqual(double a, double b)
    { return a == b; }
#endif // FP_EPSILON

    inline bool IsIntegerConst(double a)
    { return FloatEqual(a, (double)(long)a); }

#endif // ONCE_FPARSER_H_
}

#ifdef ONCE_FPARSER_H_
#include <map>
#include <set>
#include <vector>

struct FunctionParser::Data
{
    unsigned referenceCounter;

    std::string variablesString;
    std::map<FUNCTIONPARSERTYPES::NamePtr, unsigned> variableRefs;

    std::set<FUNCTIONPARSERTYPES::NameData> nameData;
    std::map<FUNCTIONPARSERTYPES::NamePtr,
             const FUNCTIONPARSERTYPES::NameData*> namePtrs;

    struct FuncPtrData
    {
        union { FunctionPtr funcPtr; FunctionParser* parserPtr; };
        unsigned params;
    };

    std::vector<FuncPtrData> FuncPtrs;
    std::vector<FuncPtrData> FuncParsers;

    std::vector<unsigned> ByteCode;
    std::vector<double> Immed;
    std::vector<double> Stack;
    unsigned StackSize;

    Data(): referenceCounter(1),
            variablesString(),
            variableRefs(),
            nameData(),
            namePtrs(),
            FuncPtrs(),
            FuncParsers(),
            ByteCode(),
            Immed(), Stack(), StackSize(0) {}
    Data(const Data&);
    Data& operator=(const Data&); // not implemented on purpose
};
#endif

#endif

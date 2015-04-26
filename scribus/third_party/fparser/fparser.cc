/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Copyright: Juha Nieminen                                                *|
\***************************************************************************/

#include "fpconfig.hh"
#include "fparser.hh"
#include "fptypes.hh"
using namespace FUNCTIONPARSERTYPES;

#include <set>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <cassert>
using namespace std;

#ifdef FP_USE_THREAD_SAFE_EVAL_WITH_ALLOCA
#ifndef FP_USE_THREAD_SAFE_EVAL
#define FP_USE_THREAD_SAFE_EVAL
#endif
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#include "scclocale.h"

//=========================================================================
// Name handling functions
//=========================================================================
namespace
{
    bool addNewNameData(std::set<NameData>& nameData,
                        std::map<NamePtr, const NameData*>& namePtrs,
                        const NameData& newData)
    {
        const FuncDefinition* funcDef =
            findFunction(NamePtr(&(newData.name[0]),
                                 unsigned(newData.name.size())));
        if(funcDef && funcDef->enabled)
            return false;

        std::set<NameData>::iterator dataIter = nameData.find(newData);

        if(dataIter != nameData.end())
        {
            if(dataIter->type != newData.type) return false;
            namePtrs.erase(NamePtr(&(dataIter->name[0]),
                                   unsigned(dataIter->name.size())));
            nameData.erase(dataIter);
        }

        dataIter = nameData.insert(newData).first;
        namePtrs[NamePtr(&(dataIter->name[0]),
                         unsigned(dataIter->name.size()))] = &(*dataIter);
        return true;
    }

    const char* readIdentifier(const char* ptr)
    {
        static const char A=10, B=11;
        /*  ^ define numeric constants for two-digit numbers
         *    so as not to disturb the layout of this neat table
         */
        static const char tab[0x100] =
        {
            0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, //00-0F
            0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, //10-1F
            0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, //20-2F
            9,9,9,9, 9,9,9,9, 9,9,0,0, 0,0,0,0, //30-3F
            0,2,2,2, 2,2,2,2, 2,2,2,2, 2,2,2,2, //40-4F
            2,2,2,2, 2,2,2,2, 2,2,2,0, 0,0,0,2, //50-5F
            0,2,2,2, 2,2,2,2, 2,2,2,2, 2,2,2,2, //60-6F
            2,2,2,2, 2,2,2,2, 2,2,2,0, 0,0,0,0, //70-7F
            8,8,8,8, 8,8,8,8, 8,8,8,8, 8,8,8,8, //80-8F
            A,A,A,A, A,A,A,A, A,A,A,A, A,A,A,A, //90-9F
            B,B,B,B, B,B,B,B, B,B,B,B, B,B,B,B, //A0-AF
            B,B,B,B, B,B,B,B, B,B,B,B, B,B,B,B, //B0-BF
            0,0,4,4, 4,4,4,4, 4,4,4,4, 4,4,4,4, //C0-CF
            4,4,4,4, 4,4,4,4, 4,4,4,4, 4,4,4,4, //D0-DF
            5,3,3,3, 3,3,3,3, 3,3,3,3, 3,0,3,3, //E0-EC, EE-EF
            6,1,1,1, 7,0,0,0, 0,0,0,0, 0,0,0,0  //F0-FF
        };
        /* Classes:
         *   9 = digits    (30-39)
         *   2 = A-Z_a-z   (41-5A, 5F, 61-7A)
         *   8 = 80-8F
         *   A = 90-9F
         *   B = A0-BF
         *   4 = C2-DF
         *   5 = E0
         *   3 = E1-EC, EE-EF
         *   6 = F0
         *   1 = F1-F3
         *   7 = F4
         *
         * Allowed multibyte utf8 sequences consist of these class options:
         *   [4]             [8AB]
         *   [5]         [B] [8AB]
         *   [3]       [8AB] [8AB]
         *   [6] [AB]  [8AB] [8AB]
         *   [1] [8AB] [8AB] [8AB]
         *   [7] [8]   [8AB] [8AB]
         * In addition, the first characters may be
         *   [2]
         * And the following characters may be
         *   [92]
         * These may never begin the character:
         *   [08AB]
         *
         * The numberings are such chosen to optimize the
         * following switch-statements for code generation.
         */

        const unsigned char* uptr = (const unsigned char*) ptr;
        switch(tab[uptr[0]])
        {
            case 2: goto loop_2; // A-Z_a-z
            case 5: goto loop_5; // E0
            case 3: goto loop_3; // E1-EC, EE-EF
            case 4: goto loop_4; // C2-DF

            case 1: goto loop_1; // F0-F4 XXX F1-F3
            case 6: goto loop_6; //       XXX F0
            case 7: goto loop_7; //       XXX F4
        }
        return (const char*) uptr;

    loop:
        switch(tab[uptr[0]])
        {
            case 9: // 0-9
            case 2: // A-Z_a-z
            loop_2:
                uptr += 1;
                goto loop;
            case 6: // F0:
            loop_6:
                if(uptr[1] < 0x90 || uptr[1] > 0xBF) break;
                goto len4pos2;
            case 1: // F1-F3:
            loop_1:
                if(uptr[1] < 0x80 || uptr[1] > 0xBF) break;
            len4pos2:
                if(uptr[2] < 0x80 || uptr[2] > 0xBF) break;
                if(uptr[3] < 0x80 || uptr[3] > 0xBF) break;
                uptr += 4;
                goto loop;
            case 7: // F4:
            loop_7:
                if(tab[uptr[1]] != 8) break;
                goto len4pos2;
            case 5: // E0
            loop_5:
                if(tab[uptr[1]] != B) break;
                goto len3pos2;
            case 3: // E1-EC, EE-EF
            loop_3:
                if(uptr[1] < 0x80 || uptr[1] > 0xBF) break;
            len3pos2:
                if(uptr[2] < 0x80 || uptr[2] > 0xBF) break;
                uptr += 3;
                goto loop;
            case 4: // C2-DF
            loop_4:
                if(uptr[1] < 0x80 || uptr[1] > 0xBF) break;
                uptr += 2;
                goto loop;
        }
        return (const char*) uptr;
    }

    bool containsOnlyValidNameChars(const std::string& name)
    {
        if(name.empty()) return false;
        const char* endPtr = readIdentifier(name.c_str());
        return *endPtr == '\0';
    }

    inline int doubleToInt(double d)
    {
        return d<0 ? -int((-d)+.5) : int(d+.5);
    }

    inline double Min(double d1, double d2)
    {
        return d1<d2 ? d1 : d2;
    }
    inline double Max(double d1, double d2)
    {
        return d1>d2 ? d1 : d2;
    }

    inline double DegreesToRadians(double degrees)
    {
        return degrees*(M_PI/180.0);
    }
    inline double RadiansToDegrees(double radians)
    {
        return radians*(180.0/M_PI);
    }
}


//=========================================================================
// Data struct implementation
//=========================================================================
FunctionParser::Data::Data(const Data& rhs):
    referenceCounter(0),
    variablesString(),
    variableRefs(),
    nameData(rhs.nameData),
    namePtrs(),
    FuncPtrs(),
    FuncParsers(),
    ByteCode(rhs.ByteCode),
    Immed(rhs.Immed),
    Stack(),
    StackSize(rhs.StackSize)
{
    Stack.resize(rhs.Stack.size());

    for(std::set<NameData>::const_iterator iter = nameData.begin();
        iter != nameData.end(); ++iter)
    {
        namePtrs[NamePtr(&(iter->name[0]), unsigned(iter->name.size()))] =
            &(*iter);
    }
}


//=========================================================================
// FunctionParser constructors, destructor and assignment
//=========================================================================
FunctionParser::FunctionParser():
    delimiterChar(0),
    parseErrorType(NO_FUNCTION_PARSED_YET), evalErrorType(0),
    data(new Data),
    useDegreeConversion(false),
    evalRecursionLevel(0),
    StackPtr(0), errorLocation(0)
{
}

FunctionParser::~FunctionParser()
{
    if(--(data->referenceCounter) == 0)
        delete data;
}

FunctionParser::FunctionParser(const FunctionParser& cpy):
    delimiterChar(cpy.delimiterChar),
    parseErrorType(cpy.parseErrorType),
    evalErrorType(cpy.evalErrorType),
    data(cpy.data),
    useDegreeConversion(cpy.useDegreeConversion),
    evalRecursionLevel(0),
    StackPtr(0), errorLocation(0)
{
    ++(data->referenceCounter);
}

FunctionParser& FunctionParser::operator=(const FunctionParser& cpy)
{
    if(data != cpy.data)
    {
        if(--(data->referenceCounter) == 0) delete data;

        delimiterChar = cpy.delimiterChar;
        parseErrorType = cpy.parseErrorType;
        evalErrorType = cpy.evalErrorType;
        data = cpy.data;
        useDegreeConversion = cpy.useDegreeConversion;
        evalRecursionLevel = cpy.evalRecursionLevel;

        ++(data->referenceCounter);
    }

    return *this;
}

void FunctionParser::setDelimiterChar(char c)
{
    delimiterChar = c;
}


//---------------------------------------------------------------------------
// Copy-on-write method
//---------------------------------------------------------------------------
void FunctionParser::CopyOnWrite()
{
    if(data->referenceCounter > 1)
    {
        Data* oldData = data;
        data = new Data(*oldData);
        --(oldData->referenceCounter);
        data->referenceCounter = 1;
    }
}

void FunctionParser::ForceDeepCopy()
{
    CopyOnWrite();
}


//=========================================================================
// User-defined constant and function addition
//=========================================================================
bool FunctionParser::AddConstant(const std::string& name, double value)
{
    if(!containsOnlyValidNameChars(name)) return false;

    CopyOnWrite();
    NameData newData(NameData::CONSTANT, name);
    newData.value = value;
    return addNewNameData(data->nameData, data->namePtrs, newData);
}

bool FunctionParser::AddUnit(const std::string& name, double value)
{
    if(!containsOnlyValidNameChars(name)) return false;

    CopyOnWrite();
    NameData newData(NameData::UNIT, name);
    newData.value = value;
    return addNewNameData(data->nameData, data->namePtrs, newData);
}

bool FunctionParser::AddFunction(const std::string& name,
                                 FunctionPtr ptr, unsigned paramsAmount)
{
    if(!containsOnlyValidNameChars(name)) return false;

    CopyOnWrite();
    NameData newData(NameData::FUNC_PTR, name);
    newData.index = unsigned(data->FuncPtrs.size());

    data->FuncPtrs.push_back(Data::FuncPtrData());
    data->FuncPtrs.back().funcPtr = ptr;
    data->FuncPtrs.back().params = paramsAmount;

    const bool retval = addNewNameData(data->nameData, data->namePtrs, newData);
    if(!retval) data->FuncPtrs.pop_back();
    return retval;
}

bool FunctionParser::CheckRecursiveLinking(const FunctionParser* fp) const
{
    if(fp == this) return true;
    for(unsigned i = 0; i < fp->data->FuncParsers.size(); ++i)
        if(CheckRecursiveLinking(fp->data->FuncParsers[i].parserPtr))
            return true;
    return false;
}

bool FunctionParser::AddFunction(const std::string& name, FunctionParser& fp)
{
    if(!containsOnlyValidNameChars(name) || CheckRecursiveLinking(&fp))
        return false;

    CopyOnWrite();
    NameData newData(NameData::PARSER_PTR, name);
    newData.index = unsigned(data->FuncParsers.size());

    data->FuncParsers.push_back(Data::FuncPtrData());
    data->FuncParsers.back().parserPtr = &fp;
    data->FuncParsers.back().params = unsigned(fp.data->variableRefs.size());

    const bool retval = addNewNameData(data->nameData, data->namePtrs, newData);
    if(!retval) data->FuncParsers.pop_back();
    return retval;
}

bool FunctionParser::RemoveIdentifier(const std::string& name)
{
    CopyOnWrite();

    const NameData dataToRemove(NameData::CONSTANT, name);
    std::set<NameData>::iterator dataIter = data->nameData.find(dataToRemove);

    if(dataIter != data->nameData.end())
    {
        data->namePtrs.erase(NamePtr(&(dataIter->name[0]),
                                     unsigned(dataIter->name.size())));
        data->nameData.erase(dataIter);
        return true;
    }
    return false;
}


//=========================================================================
// Function parsing
//=========================================================================
namespace
{
    // Error messages returned by ErrorMsg():
    const char* const ParseErrorMessage[]=
    {
        "Syntax error",                             // 0
        "Mismatched parenthesis",                   // 1
        "Missing ')'",                              // 2
        "Empty parentheses",                        // 3
        "Syntax error: Operator expected",          // 4
        "Not enough memory",                        // 5
        "An unexpected error occurred. Please make a full bug report "
        "to the author",                            // 6
        "Syntax error in parameter 'Vars' given to "
        "FunctionParser::Parse()",                  // 7
        "Illegal number of parameters to function", // 8
        "Syntax error: Premature end of string",    // 9
        "Syntax error: Expecting ( after function", // 10
        "(No function has been parsed yet)",
        ""
    };
}

// Return parse error message
// --------------------------
const char* FunctionParser::ErrorMsg() const
{
    return ParseErrorMessage[parseErrorType];
}


// Parse variables
// ---------------
bool FunctionParser::ParseVariables(const std::string& inputVarString)
{
    if(data->variablesString == inputVarString) return true;

    data->variableRefs.clear();
    data->variablesString = inputVarString;

    const std::string& vars = data->variablesString;
    const unsigned len = unsigned(vars.size());

    unsigned varNumber = VarBegin;

    const char* beginPtr = vars.c_str();
    const char* finalPtr = beginPtr + len;

    while(beginPtr < finalPtr)
    {
        const char* endPtr = readIdentifier(beginPtr);
        if(endPtr == beginPtr) return false;
        if(endPtr != finalPtr && *endPtr != ',') return false;

        NamePtr namePtr(beginPtr, unsigned(endPtr - beginPtr));

        const FuncDefinition* funcDef = findFunction(namePtr);
        if(funcDef && funcDef->enabled) return false;

        std::map<NamePtr, const NameData*>::iterator nameIter =
            data->namePtrs.find(namePtr);
        if(nameIter != data->namePtrs.end()) return false;

        if(!(data->variableRefs.insert(make_pair(namePtr, varNumber++)).second))
            return false;

        beginPtr = endPtr + 1;
    }
    return true;
}

// Parse interface functions
// -------------------------
int FunctionParser::Parse(const char* Function, const std::string& Vars,
                          bool useDegrees)
{
    CopyOnWrite();

    if(!ParseVariables(Vars))
    {
        parseErrorType = INVALID_VARS;
        return int(strlen(Function));
    }

    return ParseFunction(Function, useDegrees);
}

int FunctionParser::Parse(const std::string& Function, const std::string& Vars,
                          bool useDegrees)
{
    CopyOnWrite();

    if(!ParseVariables(Vars))
    {
        parseErrorType = INVALID_VARS;
        return int(Function.size());
    }

    return ParseFunction(Function.c_str(), useDegrees);
}


// Main parsing function
// ---------------------
int FunctionParser::ParseFunction(const char* function, bool useDegrees)
{
    useDegreeConversion = useDegrees;
    parseErrorType = FP_NO_ERROR;

    data->ByteCode.clear(); data->ByteCode.reserve(128);
    data->Immed.clear(); data->Immed.reserve(128);
    data->StackSize = StackPtr = 0;

    const char* ptr = CompileExpression(function);
    if(parseErrorType != FP_NO_ERROR) return int(errorLocation - function);

    assert(ptr); // Should never be null at this point. It's a bug otherwise.
    if(*ptr)
    {
        if(delimiterChar == 0 || *ptr != delimiterChar)
            parseErrorType = EXPECT_OPERATOR;
        return int(ptr - function);
    }

#ifndef FP_USE_THREAD_SAFE_EVAL
    data->Stack.resize(data->StackSize);
#endif

    return -1;
}


//=========================================================================
// Parsing and bytecode compiling functions
//=========================================================================
inline const char* FunctionParser::SetErrorType(ParseErrorType t,
                                                const char* pos)
{
    parseErrorType = t;
    errorLocation = pos;
    return 0;
}

inline void FunctionParser::incStackPtr()
{
    if(++StackPtr > data->StackSize) ++(data->StackSize);
}

#ifdef FP_SUPPORT_OPTIMIZER
namespace FPoptimizer_ByteCode
{
    extern signed char powi_table[256];
}
#endif
inline bool FunctionParser::CompilePowi(int int_exponent)
{
    int num_muls=0;
    while(int_exponent > 1)
    {
#ifdef FP_SUPPORT_OPTIMIZER
        if(int_exponent < 256)
        {
            int half = FPoptimizer_ByteCode::powi_table[int_exponent];
            if(half != 1 && !(int_exponent % half))
            {
                if(!CompilePowi(half)) return false;
                int_exponent /= half;
                continue;
            }
            else if(half >= 3)
            {
                data->ByteCode.push_back(cDup);
                incStackPtr();
                if(!CompilePowi(half)) return false;
                data->ByteCode.push_back(cMul);
                --StackPtr;
                int_exponent -= half+1;
                continue;
            }
        }
#endif
        if(!(int_exponent & 1))
        {
            int_exponent /= 2;
            data->ByteCode.push_back(cSqr);
        }
        else
        {
            data->ByteCode.push_back(cDup);
            incStackPtr();
            int_exponent -= 1;
            ++num_muls;
        }
    }
    if(num_muls > 0)
    {
        data->ByteCode.resize(data->ByteCode.size()+num_muls,
                              cMul);
        StackPtr -= num_muls;
    }
    return true;
}

namespace
{
    struct MulOp
    {
        enum { opcode = cMul, opposite = cDiv, combined = cMul, defval=1 };
        static inline void action(double& target, double value)
        { target *= value; }
        static inline void combine_action(double& target, double value)
        { target=value/target; }
        static inline bool valid_rvalue(double) { return true; }
        static inline bool valid_opposite_rvalue(double v) { return v != 0.0; }
        static inline bool is_redundant(double v) { return v==1.0; }
        static inline bool opposite_is_preferred() { return false; }
    };
    struct DivOp
    {
        enum { opcode = cDiv, opposite = cMul, combined = cMul, defval=1 };
        static inline void action(double& target, double value)
        { target /= value; }
        static inline void combine_action(double& target, double value)
        { target=target/value; }
        static inline bool valid_rvalue(double v) { return v != 0.0; }
        static inline bool valid_opposite_rvalue(double) { return true; }
        static inline bool is_redundant(double v) { return v==1.0; }
        static inline bool opposite_is_preferred() { return true; }
    };
    struct AddOp
    {
        enum { opcode = cAdd, opposite = cSub, combined = cAdd, defval=0 };
        static inline void action(double& target, double value)
        { target += value; }
        static inline void combine_action(double& target, double value)
        { target=value-target; }
        static inline bool valid_rvalue(double) { return true; }
        static inline bool valid_opposite_rvalue(double) { return true; }
        static inline bool is_redundant(double v) { return v==0.0; }
        static inline bool opposite_is_preferred() { return false; }
    };
    struct SubOp
    {
        enum { opcode = cSub, opposite = cAdd, combined = cAdd, defval=0 };
        static inline void action(double& target, double value)
        { target -= value; }
        static inline void combine_action(double& target, double value)
        { target=target-value; }
        static inline bool valid_rvalue(double) { return true; }
        static inline bool valid_opposite_rvalue(double) { return true; }
        static inline bool is_redundant(double v) { return v==0.0; }
        static inline bool opposite_is_preferred() { return true; }
    };
    struct ModOp
    {
        enum { opcode = cMod, opposite = cMod, combined = cMod, defval=1 };
        static inline void action(double& target, double value)
        { target = fmod(target, value); }
        static inline void combine_action(double& target, double value)
        { target = fmod(target, value); }
        static inline bool valid_rvalue(double v) { return v != 0.0; }
        static inline bool valid_opposite_rvalue(double v) { return v != 0.0; }
        static inline bool is_redundant(double) { return false; }
        static inline bool opposite_is_preferred() { return false; }
    };

    bool IsEligibleIntPowiExponent(int int_exponent)
    {
        int abs_int_exponent = int_exponent;
        if(abs_int_exponent < 0) abs_int_exponent = -abs_int_exponent;

        return (abs_int_exponent >= 1)
            && (abs_int_exponent <= 46 ||
              (abs_int_exponent <= 1024 &&
              (abs_int_exponent & (abs_int_exponent - 1)) == 0));
    }
}

inline void FunctionParser::AddFunctionOpcode(unsigned opcode)
{
    if(data->ByteCode.back() == cImmed)
    {
        switch(opcode)
        {
          case cAbs:
              data->Immed.back() = fabs(data->Immed.back());
              return;
          case cAcos:
              if(data->Immed.back() < -1 || data->Immed.back() > 1) break;
              data->Immed.back() = acos(data->Immed.back());
              return;
          case cAcosh:
              data->Immed.back() = fp_acosh(data->Immed.back());
              return;
          case cAsin:
              if(data->Immed.back() < -1 || data->Immed.back() > 1) break;
              data->Immed.back() = asin(data->Immed.back());
              return;
          case cAsinh:
              data->Immed.back() = fp_asinh(data->Immed.back());
              return;
          case cAtan:
              data->Immed.back() = atan(data->Immed.back());
              return;
          case cAtanh:
              data->Immed.back() = fp_atanh(data->Immed.back());
              return;
          case cCeil:
              data->Immed.back() = ceil(data->Immed.back());
              return;
          case cCos:
              data->Immed.back() = cos(data->Immed.back());
              return;
          case cCosh:
              data->Immed.back() = cosh(data->Immed.back());
              return;
          case cExp:
              data->Immed.back() = exp(data->Immed.back());
              return;
          case cExp2:
              data->Immed.back() = pow(2.0, data->Immed.back());
              return;
          case cFloor:
              data->Immed.back() = floor(data->Immed.back());
              return;
          case cInt:
              data->Immed.back() = floor(data->Immed.back() + 0.5);
              return;
          case cLog:
              if(data->Immed.back() <= 0.0) break;
              data->Immed.back() = log(data->Immed.back());
              return;
          case cLog10:
              if(data->Immed.back() <= 0.0) break;
              data->Immed.back() = log10(data->Immed.back());
              return;
          case cLog2:
              if(data->Immed.back() <= 0.0) break;
              data->Immed.back() =
                  log(data->Immed.back()) * 1.4426950408889634074;
              return;
          case cSin:
              data->Immed.back() = sin(data->Immed.back());
              return;
          case cSinh:
              data->Immed.back() = sinh(data->Immed.back());
              return;
          case cSqrt:
              if(data->Immed.back() < 0.0) break;
              data->Immed.back() = sqrt(data->Immed.back());
              return;
          case cTan:
              data->Immed.back() = tan(data->Immed.back());
              return;
          case cTanh:
              data->Immed.back() = tanh(data->Immed.back());
              return;
          case cDeg:
              data->Immed.back() = RadiansToDegrees(data->Immed.back());
              return;
          case cRad:
              data->Immed.back() = DegreesToRadians(data->Immed.back());
              return;
          case cPow:
          {
              unsigned before_immed_opcode = data->ByteCode[data->ByteCode.size()-2];
              if(before_immed_opcode == cExp
              || before_immed_opcode == cExp2
              || before_immed_opcode == cPow)
              {
                  /* Change  ... Exp immed (Pow)
                   *      to ... immed mul Exp
                   * Similarly with Exp, Exp2 and Pow
                   *
                   * This helps change exp(x*3)^5 into exp(x*15)
                   */
                  data->ByteCode.pop_back();
                  data->ByteCode.back() = cImmed;
                  AddBinaryOperationByConst<MulOp> ();
                  data->ByteCode.push_back(before_immed_opcode);
                  return;
              }
              /*if(before_immed_opcode == cImmed)
              {
                  data->Immed[data->Immed.size()-2] = pow(data->Immed[data->Immed.size()-2, data->Immed.back());
                  data->Immed.pop_back();
                  return;
              }*/
              // if the exponent is a special constant value
              if(data->Immed.back() == 0.5)
              {
                  data->Immed.pop_back(); data->ByteCode.pop_back();
                  opcode = cSqrt;
              }
              else if(data->Immed.back() == -0.5)
              {
                  data->Immed.pop_back(); data->ByteCode.pop_back();
                  opcode = (cRSqrt);
              }
              else if(data->Immed.back() == -1.0)
              {
                  data->Immed.pop_back(); data->ByteCode.pop_back();
                  opcode = (cInv);
              }
              else
              {
                  double original_immed = data->Immed.back();
                  int int_exponent = (int)original_immed;

                  if(original_immed != (double)int_exponent)
                  {
                      for(int sqrt_count=1; sqrt_count<=4; ++sqrt_count)
                      {
                          int factor = 1 << sqrt_count;
                          double changed_exponent =
                              original_immed * (double)factor;
                          if(IsIntegerConst(changed_exponent) &&
                             IsEligibleIntPowiExponent
                             ( (int)changed_exponent ) )
                          {
                              while(sqrt_count > 0)
                              {
                                  data->ByteCode.insert(data->ByteCode.end()-1,
                                                        cSqrt);
                                  --sqrt_count;
                              }
                              original_immed = changed_exponent;
                              int_exponent   = (int)changed_exponent;
                              goto do_powi;
                          }
                      }
                  }
                  else if(IsEligibleIntPowiExponent(int_exponent))
                  {
                  do_powi:;
                      int abs_int_exponent = int_exponent;
                      if(abs_int_exponent < 0)
                          abs_int_exponent = -abs_int_exponent;

                      data->Immed.pop_back(); data->ByteCode.pop_back();
                      /*size_t bytecode_size = data->ByteCode.size();*/
                      if(int_exponent < 0) AddFunctionOpcode(cInv);
                      if(CompilePowi(abs_int_exponent))
                           return;
                      /*powi_failed:;
                      data->ByteCode.resize(bytecode_size);
                      data->Immed.push_back(original_immed);
                      data->ByteCode.push_back(cImmed);*/
                  }
                  // x^y can be safely converted into exp(y * log(x))
                  // when y is _not_ integer, because we know that x >= 0.
                  // Otherwise either expression will give a NaN.
                  if(original_immed != (double)int_exponent)
                  {
                      data->Immed.pop_back(); data->ByteCode.pop_back();
                      AddFunctionOpcode(cLog);
                      AddMultiplicationByConst(original_immed);
                      opcode = cExp;
                  }
              }
          } // pow
        }
    }
    switch(opcode)
    {
        #define eliminate_redundant_sequence(first, then) \
            case then: \
                if(data->ByteCode.back() == first) \
                { \
                    data->ByteCode.pop_back(); \
                    return; \
                } \
                break
        eliminate_redundant_sequence(cLog, cExp);
        eliminate_redundant_sequence(cLog2, cExp2);
        eliminate_redundant_sequence(cAsin, cSin);
        eliminate_redundant_sequence(cAcos, cCos);
        eliminate_redundant_sequence(cInv, cInv);
        #undef eliminate_redundant_sequence
        case cAbs:
            // cAbs is redundant after any opcode that never
            // returns a negative value
            switch(data->ByteCode.back())
            {
                case cSqrt: case cRSqrt:
                case cAbs:
                case cAnd: case cOr:
                case cEqual: case cNEqual:
                case cLess: case cLessOrEq:
                case cGreater: case cGreaterOrEq:
                case cNot: case cNotNot:
                case cLog: case cLog2: case cLog10:
                case cAcos: case cCosh:
                    return;
            }
    }
    switch(opcode)
    {
        case cExp:
            /* Change "immed Add Exp" to "Exp exp(immed) Mul" */
            if(data->ByteCode.back() == cAdd
            && data->ByteCode[data->ByteCode.size()-2] == cImmed)
            {
                data->ByteCode[data->ByteCode.size()-2] = cExp;
                data->ByteCode.back() = cImmed;
                data->Immed.back() = exp(data->Immed.back());
                opcode = cMul;
            }
            break;
        case cExp2:
            /* Change "immed Add Exp2" to "Exp2 exp2(immed) Mul" */
            if(data->ByteCode.back() == cAdd
            && data->ByteCode[data->ByteCode.size()-2] == cImmed)
            {
                data->ByteCode[data->ByteCode.size()-2] = cExp2;
                data->ByteCode.back() = cImmed;
                data->Immed.back() = pow(2.0, data->Immed.back());
                opcode = cMul;
            }
            break;
    }
    data->ByteCode.push_back(opcode);
}

inline void FunctionParser::AddFunctionOpcode_CheckDegreesConversion
(unsigned opcode)
{
    if(useDegreeConversion)
        switch(opcode)
        {
          case cCos:
          case cCosh:
          case cCot:
          case cCsc:
          case cSec:
          case cSin:
          case cSinh:
          case cTan:
          case cTanh:
              AddFunctionOpcode(cRad);
        }

    switch(opcode)
    {
      case cMin:
          if(data->ByteCode.back() == cImmed
          && data->ByteCode[data->ByteCode.size()-2] == cImmed)
          {
              data->Immed[data->Immed.size()-2] =
                Min(data->Immed[data->Immed.size()-2], data->Immed.back());
              data->ByteCode.pop_back();
              data->Immed.pop_back();
              goto skip_op;
          }
          break;
      case cMax:
          if(data->ByteCode.back() == cImmed
          && data->ByteCode[data->ByteCode.size()-2] == cImmed)
          {
              data->Immed[data->Immed.size()-2] =
                Max(data->Immed[data->Immed.size()-2], data->Immed.back());
              data->ByteCode.pop_back();
              data->Immed.pop_back();
              goto skip_op;
          }
          break;
      case cAtan2:
          if(data->ByteCode.back() == cImmed
          && data->ByteCode[data->ByteCode.size()-2] == cImmed)
          {
              data->Immed[data->Immed.size()-2] =
                atan2(data->Immed[data->Immed.size()-2], data->Immed.back());
              data->ByteCode.pop_back();
              data->Immed.pop_back();
              goto skip_op;
          }
          break;
      case cPow:
          if(data->ByteCode.back() == cImmed
          && data->ByteCode[data->ByteCode.size()-2] == cImmed)
          {
              data->Immed[data->Immed.size()-2] =
                pow(data->Immed[data->Immed.size()-2], data->Immed.back());
              data->ByteCode.pop_back();
              data->Immed.pop_back();
              goto skip_op;
          }
          break;
    }
    AddFunctionOpcode(opcode);
 skip_op:;

    if(useDegreeConversion)
        switch(opcode)
        {
          case cAcos:
          case cAcosh:
          case cAsinh:
          case cAtanh:
          case cAsin:
          case cAtan:
          case cAtan2:
              AddFunctionOpcode(cDeg);
        }
}

inline void FunctionParser::AddMultiplicationByConst(double value)
{
    if(data->ByteCode.back() == cImmed)
    {
        data->Immed.back() *= value;
    }
    else if(data->ByteCode.back() == cMul &&
            data->ByteCode[data->ByteCode.size()-2] == cImmed)
    {
        data->Immed.back() *= value;
        if(data->Immed.back() == 1.0)
            { data->Immed.pop_back();
              data->ByteCode.pop_back();
              data->ByteCode.pop_back(); }
    }
    else
    {
        data->Immed.push_back(value);
        data->ByteCode.push_back(cImmed);
        incStackPtr();
        AddBinaryOperationByConst<MulOp> ();
        --StackPtr;
    }
}

template<typename Operation>
inline void FunctionParser::AddBinaryOperationByConst()
{
    // data->ByteCode.back() is assumed to be cImmed here
    // that is, data->ByteCode[data->ByteCode.size()-1]
    if(!Operation::valid_rvalue(data->Immed.back()))
    {
        /* If the function has something like x/0, don't try optimizing it. */
        data->ByteCode.push_back( unsigned(Operation::opcode) );
    }
    else if(Operation::is_redundant(data->Immed.back()))
    {
        /* If the function has x*1 or x/1, just keep x. */
        data->Immed.pop_back();
        data->ByteCode.pop_back();
    }
    else if(data->ByteCode[data->ByteCode.size()-2] == cImmed)
    {
        // bytecode top:       ... immed immed <to be cMul>
        Operation::action(data->Immed[data->Immed.size()-2],
                          data->Immed.back());
        data->Immed.pop_back();
        data->ByteCode.pop_back();
    }
    else if(data->ByteCode[data->ByteCode.size()-2] == Operation::opcode
         && data->ByteCode[data->ByteCode.size()-3] == cImmed)
    {
        // bytecode top:  ... immed cMul immed <to be cMul>
        Operation::action(data->Immed[data->Immed.size()-2],
                          data->Immed.back());
        data->Immed.pop_back();
        data->ByteCode.pop_back();
        // bytecode top:  ... immed cMul
        if(Operation::is_redundant(data->Immed.back()))
        {
            data->Immed.pop_back();
            data->ByteCode.pop_back();
            data->ByteCode.pop_back();
            // bytecode top:  ...
        }
    }
    /* x*2/4 = x*(2/4)
     * x/2*4 = x*(4/2)
     * x+2-4 = x+(2-4)
     * x-2+4 = x+(4-2)
     */
    else if(data->ByteCode[data->ByteCode.size()-2] == Operation::opposite
         && data->ByteCode[data->ByteCode.size()-3] == cImmed
         && Operation::valid_opposite_rvalue(data->Immed[data->Immed.size()-2]))
    {
        // bytecode top:  ... immed cDiv immed <to be cMul>
        data->ByteCode[data->ByteCode.size()-2] = Operation::combined;
        Operation::combine_action(data->Immed[data->Immed.size()-2],
                                  data->Immed.back());
        data->Immed.pop_back();
        data->ByteCode.pop_back();
        // bytecode top:  ... immed cMul
        if(Operation::is_redundant(data->Immed.back()))
        {
            data->Immed.pop_back();
            data->ByteCode.pop_back();
            data->ByteCode.pop_back();
            // bytecode top:  ...
        }
    }
    else if(Operation::opposite_is_preferred())
    {
        double p = (double) Operation::defval;
        Operation::combine_action(p, data->Immed.back());
        data->Immed.back() = p;
        data->ByteCode.push_back(unsigned(Operation::opposite));
    }
    else
    {
        /* Possibilities:
         *  Change "Exp immed Mul" into "log(immed) Add Exp"
         *         "Exp2 immed Mul" into "log2(immed) Add Exp2"
         * Or the opposite:
         *         "immed Add Exp" to "Exp exp(immed) Mul"
         * this is now actually done in AddFunctionOpcode(),
         * because it allows optimizing exp(y+1)*2 into exp(y)*5.437.
         */
        data->ByteCode.push_back(unsigned(Operation::opcode));
    }
}

namespace
{
    inline FunctionParser::ParseErrorType noCommaError(char c)
    {
        return c == ')' ?
            FunctionParser::ILL_PARAMS_AMOUNT : FunctionParser::SYNTAX_ERROR;
    }

    inline FunctionParser::ParseErrorType noParenthError(char c)
    {
        return c == ',' ?
            FunctionParser::ILL_PARAMS_AMOUNT : FunctionParser::MISSING_PARENTH;
    }
}

const char* FunctionParser::CompileIf(const char* function)
{
    if(*function != '(') return SetErrorType(EXPECT_PARENTH_FUNC, function);

    function = CompileExpression(function+1);
    if(!function) return 0;
    if(*function != ',') return SetErrorType(noCommaError(*function), function);

    data->ByteCode.push_back(cIf);
    const unsigned curByteCodeSize = unsigned(data->ByteCode.size());
    data->ByteCode.push_back(0); // Jump index; to be set later
    data->ByteCode.push_back(0); // Immed jump index; to be set later

    --StackPtr;

    function = CompileExpression(function + 1);
    if(!function) return 0;
    if(*function != ',') return SetErrorType(noCommaError(*function), function);

    data->ByteCode.push_back(cJump);
    const unsigned curByteCodeSize2 = unsigned(data->ByteCode.size());
    const unsigned curImmedSize2 = unsigned(data->Immed.size());
    data->ByteCode.push_back(0); // Jump index; to be set later
    data->ByteCode.push_back(0); // Immed jump index; to be set later

    --StackPtr;

    function = CompileExpression(function + 1);
    if(!function) return 0;
    if(*function != ')')
        return SetErrorType(noParenthError(*function), function);

    data->ByteCode.push_back(cNop);

    // Set jump indices
    data->ByteCode[curByteCodeSize] = curByteCodeSize2+1;
    data->ByteCode[curByteCodeSize+1] = curImmedSize2;
    data->ByteCode[curByteCodeSize2] = unsigned(data->ByteCode.size())-1;
    data->ByteCode[curByteCodeSize2+1] = unsigned(data->Immed.size());

    ++function;
    while(isspace(*function)) ++function;
    return function;
}

const char* FunctionParser::CompileFunctionParams(const char* function,
                                                  unsigned requiredParams)
{
    if(*function != '(') return SetErrorType(EXPECT_PARENTH_FUNC, function);

    if(requiredParams > 0)
    {
        function = CompileExpression(function+1);
        if(!function) return 0;

        for(unsigned i = 1; i < requiredParams; ++i)
        {
            if(*function != ',')
                return SetErrorType(noCommaError(*function), function);

            function = CompileExpression(function+1);
            if(!function) return 0;
        }
        // No need for incStackPtr() because each parse parameter calls it
        StackPtr -= requiredParams-1;
    }
    else
    {
        incStackPtr(); // return value of function is pushed onto the stack
        ++function;
        while(isspace(*function)) ++function;
    }

    if(*function != ')')
        return SetErrorType(noParenthError(*function), function);
    ++function;
    while(isspace(*function)) ++function;
    return function;
}

const char* FunctionParser::CompileElement(const char* function)
{
    const char c = *function;

    if(c == '(') // Expression in parentheses
    {
        ++function;
        while(isspace(*function)) ++function;
        if(*function == ')') return SetErrorType(EMPTY_PARENTH, function);

        function = CompileExpression(function);
        if(!function) return 0;

        if(*function != ')') return SetErrorType(MISSING_PARENTH, function);

        ++function;
        while(isspace(*function)) ++function;
        return function;
    }

    if(isdigit(c) || c=='.') // Number
    {
        char* endPtr;
        const double val = ScCLocale::strtod(function, &endPtr);
        if(endPtr == function) return SetErrorType(SYNTAX_ERROR, function);

        data->Immed.push_back(val);
        data->ByteCode.push_back(cImmed);
        incStackPtr();

        while(isspace(*endPtr)) ++endPtr;
        return endPtr;
    }

    const char* endPtr = readIdentifier(function);
    if(endPtr != function) // Function, variable or constant
    {
        NamePtr name(function, unsigned(endPtr - function));
        while(isspace(*endPtr)) ++endPtr;

        const FuncDefinition* funcDef = findFunction(name);
        if(funcDef && funcDef->enabled) // is function
        {
            if(funcDef->opcode == cIf) // "if" is a special case
                return CompileIf(endPtr);

#ifndef FP_DISABLE_EVAL
            const unsigned requiredParams =
                funcDef->opcode == cEval ?
                unsigned(data->variableRefs.size()) :
                funcDef->params;
#else
            const unsigned requiredParams = funcDef->params;
#endif

            function = CompileFunctionParams(endPtr, requiredParams);
            if(!function) return 0;
            AddFunctionOpcode_CheckDegreesConversion(funcDef->opcode);
            return function;
        }

        std::map<NamePtr, unsigned>::iterator varIter =
            data->variableRefs.find(name);
        if(varIter != data->variableRefs.end()) // is variable
        {
            data->ByteCode.push_back(varIter->second);
            incStackPtr();
            return endPtr;
        }

        std::map<NamePtr, const NameData*>::iterator nameIter =
            data->namePtrs.find(name);
        if(nameIter != data->namePtrs.end())
        {
            const NameData* nameData = nameIter->second;
            switch(nameData->type)
            {
              case NameData::CONSTANT:
                  data->Immed.push_back(nameData->value);
                  data->ByteCode.push_back(cImmed);
                  incStackPtr();
                  return endPtr;

              case NameData::UNIT: break;

              case NameData::FUNC_PTR:
                  function = CompileFunctionParams
                      (endPtr, data->FuncPtrs[nameData->index].params);
                  data->ByteCode.push_back(cFCall);
                  data->ByteCode.push_back(nameData->index);
                  data->ByteCode.push_back(cNop);
                  return function;

              case NameData::PARSER_PTR:
                  function = CompileFunctionParams
                      (endPtr, data->FuncParsers[nameData->index].params);
                  data->ByteCode.push_back(cPCall);
                  data->ByteCode.push_back(nameData->index);
                  data->ByteCode.push_back(cNop);
                  return function;
            }
        }
    }

    if(c == ')') return SetErrorType(MISM_PARENTH, function);
    return SetErrorType(SYNTAX_ERROR, function);
}

const char* FunctionParser::CompilePossibleUnit(const char* function)
{
    const char* endPtr = readIdentifier(function);

    if(endPtr != function)
    {
        NamePtr name(function, unsigned(endPtr - function));
        while(isspace(*endPtr)) ++endPtr;

        std::map<NamePtr, const NameData*>::iterator nameIter =
            data->namePtrs.find(name);
        if(nameIter != data->namePtrs.end())
        {
            const NameData* nameData = nameIter->second;
            if(nameData->type == NameData::UNIT)
            {
                AddMultiplicationByConst(nameData->value);
                return endPtr;
            }
        }
    }

    return function;
}

const char* FunctionParser::CompilePow(const char* function)
{
    function = CompileElement(function);
    if(!function) return 0;
    function = CompilePossibleUnit(function);

    if(*function == '^')
    {
        ++function;
        while(isspace(*function)) ++function;

        bool base_is_immed = false;
        double base_immed = 0;
        if(data->ByteCode.back() == cImmed)
        {
            base_is_immed = true;
            base_immed = data->Immed.back();
            data->Immed.pop_back();
            data->ByteCode.pop_back();
        }

        function = CompileUnaryMinus(function);
        if(!function) return 0;

        // Check if the exponent is a literal
        if(data->ByteCode.back() == cImmed)
        {
            // If operator is applied to two literals, calculate it now:
            if(base_is_immed)
                data->Immed.back() = pow(base_immed, data->Immed.back());
            else
                AddFunctionOpcode(cPow);
        }
        else if(base_is_immed)
        {
            if(base_immed > 0.0)
            {
                double mulvalue = std::log(base_immed);
                if(mulvalue != 1.0)
                    AddMultiplicationByConst(mulvalue);
                AddFunctionOpcode(cExp);
            }
            else /* uh-oh, we've got e.g. (-5)^x, and we already deleted
                    -5 from the stack */
            {
                data->Immed.push_back(base_immed);
                data->ByteCode.push_back(cImmed);
                incStackPtr();
                AddFunctionOpcode(cRPow);
            }
        }
        else // add opcode
            AddFunctionOpcode(cPow);

        --StackPtr;
    }
    return function;
}

const char* FunctionParser::CompileUnaryMinus(const char* function)
{
    const char op = *function;
    if(op == '-' || op == '!')
    {
        ++function;
        while(isspace(*function)) ++function;
        function = CompileUnaryMinus(function);
        if(!function) return 0;

        if(op == '-')
        {
            // if we are negating a negation, we can remove both:
            if(data->ByteCode.back() == cNeg)
                data->ByteCode.pop_back();

            // if we are negating a constant, negate the constant itself:
            else if(data->ByteCode.back() == cImmed)
                data->Immed.back() = -data->Immed.back();

            else data->ByteCode.push_back(cNeg);
        }
        else
        {
            // if notting a constant, change the constant itself:
            if(data->ByteCode.back() == cImmed)
                data->Immed.back() = !doubleToInt(data->Immed.back());

            // !!x is a common paradigm: instead of x cNot cNot,
            // we produce x cNotNot.
            else if(data->ByteCode.back() == cNot)
                data->ByteCode.back() = cNotNot;

            // !!!x is simply x cNot. The cNotNot in the middle is redundant.
            else if(data->ByteCode.back() == cNotNot)
                data->ByteCode.back() = cNot;

            else
                data->ByteCode.push_back(cNot);
        }
    }
    else
        function = CompilePow(function);

    return function;
}

inline const char* FunctionParser::CompileMult(const char* function)
{
    function = CompileUnaryMinus(function);
    if(!function) return 0;

    char op;
    while((op = *function) == '*' || op == '/' || op == '%')
    {
        ++function;
        while(isspace(*function)) ++function;

        bool is_unary = false;
        if(op != '%'
        && data->ByteCode.back() == cImmed
        && data->Immed.back() == 1.0)
        {
            is_unary = true;
            data->Immed.pop_back();
            data->ByteCode.pop_back();
        }

        function = CompileUnaryMinus(function);
        if(!function) return 0;

    op_changed:
        switch(data->ByteCode.back())
        {
          case cImmed:
              // If operator is applied to two literals, calculate it now:
              switch(op)
              {
                case '%':
                    AddBinaryOperationByConst<ModOp>();
                    break;
                default:
                case '*':
                    if(is_unary) break;
                    AddBinaryOperationByConst<MulOp>();
                    break;
                case '/':
                    if(is_unary)
                    {
                        if(data->Immed.back() == 0.0)
                            // avoid dividing by zero.
                            data->ByteCode.push_back(cInv);
                        else
                            data->Immed.back() = 1.0 / data->Immed.back();
                        break;
                    }
                    AddBinaryOperationByConst<DivOp>();
                    break;
              }
              break;
          case cInv: // x * y^-1 = x * (1/y) = x/y
              switch(op)
              {
                case '*':
                    data->ByteCode.pop_back(); op = '/'; goto op_changed;
                case '/':
                    data->ByteCode.pop_back(); op = '*'; goto op_changed;
                default: break;
              }
              // passthru
          default:
              // add opcode
              switch(op)
              {
                case '%':
                    data->ByteCode.push_back(cMod);
                    break;
                case '/':
                    if(is_unary)
                        data->ByteCode.push_back(cInv);
                    else
                    {
                    /* Change x / exp(log(y)*1.1)   -  x y Log  1.1 Mul Exp Div
                     *   into x * exp(log(y)*-1.1)  -  x y Log -1.1 Mul Exp Mul
                     *
                     * Changing x / y^9000          -  x y  9000 Pow Div
                     *    into  x * y^-9000         -  x y -9000 Pow Mul
                     * is also possible, but at least on x86_64
                     * it is detrimental for performance.
                     * In fact, the opposite seems favorable.
                     */
                        if(data->ByteCode.back() == cExp
                           && data->ByteCode[data->ByteCode.size()-2] == cMul
                           && data->ByteCode[data->ByteCode.size()-3] == cImmed)
                        {
                            data->Immed.back() = -data->Immed.back();
                            data->ByteCode.push_back(cMul);
                        }
                        else
                            data->ByteCode.push_back(cDiv);
                    }
                    break;
                default:
                case '*':
                    if(is_unary)
                    { }
                    else
                    {
                        if(data->ByteCode.back() == cPow
                           && data->ByteCode[data->ByteCode.size()-2] == cImmed
                           && data->Immed.back() < 0)
                        {
                            data->Immed.back() = -data->Immed.back();
                            data->ByteCode.push_back(cDiv);
                        }
                        else
                            data->ByteCode.push_back(cMul);
                    }
                    break;
              }
        }
        --StackPtr;
    }
    return function;
}

inline const char* FunctionParser::CompileAddition(const char* function)
{
    function = CompileMult(function);
    if(!function) return 0;

    char op;
    while((op = *function) == '+' || op == '-')
    {
        ++function;
        while(isspace(*function)) ++function;

        bool is_unary = false;
        if(data->ByteCode.back() == cImmed
        && data->Immed.back() == 0.0)
        {
            is_unary = true;
            data->Immed.pop_back();
            data->ByteCode.pop_back();
        }

        function = CompileMult(function);
        if(!function) return 0;

    op_changed:
        switch(data->ByteCode.back())
        {
          case cImmed:
              // If operator is applied to two literals, calculate it now:
              switch(op)
              {
                default:
                case '+':
                    if(is_unary) break;
                    AddBinaryOperationByConst<AddOp>();
                    break;
                case '-':
                    if(is_unary)
                    { data->Immed.back() = -data->Immed.back(); break; }
                    AddBinaryOperationByConst<SubOp>();
              }
              break;
          case cNeg: // x + (-y) = x-y
              switch(op)
              {
                default:
                case '+':
                    data->ByteCode.pop_back(); op = '-'; goto op_changed;
                case '-':
                    data->ByteCode.pop_back(); op = '+'; goto op_changed;
              }
              // passthru (not reached)
          default:
              // add opcode
              switch(op)
              {
                default:
                case '+':
                    if(!is_unary) data->ByteCode.push_back(cAdd); break;
                case '-':
                    data->ByteCode.push_back(is_unary ? cNeg : cSub); break;
              }
        }
        --StackPtr;
    }
    return function;
}

namespace
{
    inline int getComparisonOpcode(const char*& f)
    {
        switch(*f)
        {
          case '=':
              ++f; return cEqual;

          case '!':
              if(f[1] == '=') { f += 2; return cNEqual; }
              return -1; // If '=' does not follow '!', a syntax error will
                         // be generated at the outermost parsing level

          case '<':
              if(f[1] == '=') { f += 2; return cLessOrEq; }
              ++f; return cLess;

          case '>':
              if(f[1] == '=') { f += 2; return cGreaterOrEq; }
              ++f; return cGreater;
        }
        return -1;
    }
}

const char* FunctionParser::CompileComparison(const char* function)
{
    function = CompileAddition(function);
    if(!function) return 0;

    int opCode;
    while((opCode = getComparisonOpcode(function)) >= 0)
    {
        while(isspace(*function)) ++function;
        function = CompileAddition(function);
        if(!function) return 0;
        data->ByteCode.push_back(opCode);
        --StackPtr;
    }
    return function;
}

inline const char* FunctionParser::CompileAnd(const char* function)
{
    function = CompileComparison(function);
    if(!function) return 0;

    while(*function == '&')
    {
        ++function;
        while(isspace(*function)) ++function;
        function = CompileComparison(function);
        if(!function) return 0;
        data->ByteCode.push_back(cAnd);
        --StackPtr;
    }
    return function;
}

const char* FunctionParser::CompileExpression(const char* function)
{
    while(isspace(*function)) ++function;
    function = CompileAnd(function);
    if(!function) return 0;

    while(*function == '|')
    {
        ++function;
        while(isspace(*function)) ++function;
        function = CompileAnd(function);
        if(!function) return 0;
        data->ByteCode.push_back(cOr);
        --StackPtr;
    }
    return function;
}

//===========================================================================
// Function evaluation
//===========================================================================
double FunctionParser::Eval(const double* Vars)
{
    if(parseErrorType != FP_NO_ERROR) return 0.0;

    const unsigned* const ByteCode = &(data->ByteCode[0]);
    const double* const Immed = data->Immed.empty() ? 0 : &(data->Immed[0]);
    const unsigned ByteCodeSize = unsigned(data->ByteCode.size());
    unsigned IP, DP=0;
    int SP=-1;

#ifdef FP_USE_THREAD_SAFE_EVAL
#ifdef FP_USE_THREAD_SAFE_EVAL_WITH_ALLOCA
    double* const Stack = (double*)alloca(data->StackSize*sizeof(double));
#else
    std::vector<double> Stack(data->StackSize);
#endif
#else
    std::vector<double>& Stack = data->Stack;
#endif

    for(IP=0; IP<ByteCodeSize; ++IP)
    {
        switch(ByteCode[IP])
        {
// Functions:
          case   cAbs: Stack[SP] = fabs(Stack[SP]); break;

          case  cAcos:
#                    ifndef FP_NO_EVALUATION_CHECKS
                       if(Stack[SP] < -1 || Stack[SP] > 1)
                       { evalErrorType=4; return 0; }
#                    endif
                       Stack[SP] = acos(Stack[SP]); break;

          case cAcosh: Stack[SP] = fp_acosh(Stack[SP]); break;

          case  cAsin:
#                    ifndef FP_NO_EVALUATION_CHECKS
                       if(Stack[SP] < -1 || Stack[SP] > 1)
                       { evalErrorType=4; return 0; }
#                    endif
                       Stack[SP] = asin(Stack[SP]); break;

          case cAsinh: Stack[SP] = fp_asinh(Stack[SP]); break;

          case  cAtan: Stack[SP] = atan(Stack[SP]); break;

          case cAtan2: Stack[SP-1] = atan2(Stack[SP-1], Stack[SP]);
                       --SP; break;

          case cAtanh: Stack[SP] = fp_atanh(Stack[SP]); break;

          case  cCeil: Stack[SP] = ceil(Stack[SP]); break;

          case   cCos: Stack[SP] = cos(Stack[SP]); break;

          case  cCosh: Stack[SP] = cosh(Stack[SP]); break;

          case   cCot:
              {
                  const double t = tan(Stack[SP]);
#               ifndef FP_NO_EVALUATION_CHECKS
                  if(t == 0) { evalErrorType=1; return 0; }
#               endif
                  Stack[SP] = 1/t; break;
              }

          case   cCsc:
              {
                  const double s = sin(Stack[SP]);
#               ifndef FP_NO_EVALUATION_CHECKS
                  if(s == 0) { evalErrorType=1; return 0; }
#               endif
                  Stack[SP] = 1/s; break;
              }


#       ifndef FP_DISABLE_EVAL
          case  cEval:
              {
                  const unsigned varAmount =
                      unsigned(data->variableRefs.size());
                  double retVal = 0;
                  if(evalRecursionLevel == FP_EVAL_MAX_REC_LEVEL)
                  {
                      evalErrorType = 5;
                  }
                  else
                  {
                      ++evalRecursionLevel;
#                   ifndef FP_USE_THREAD_SAFE_EVAL
                      std::vector<double> tmpStack(Stack.size());
                      data->Stack.swap(tmpStack);
                      retVal = Eval(&tmpStack[SP - varAmount + 1]);
                      data->Stack.swap(tmpStack);
#                   else
                      retVal = Eval(&Stack[SP - varAmount + 1]);
#                   endif
                      --evalRecursionLevel;
                  }
                  SP -= varAmount-1;
                  Stack[SP] = retVal;
                  break;
              }
#       endif

          case   cExp: Stack[SP] = exp(Stack[SP]); break;

          case   cExp2:
            //#ifdef FP_SUPPORT_EXP2
            //  Stack[SP] = exp2(Stack[SP]);
            //#else
              Stack[SP] = pow(2.0, Stack[SP]);
            //#endif
              break;

          case cFloor: Stack[SP] = floor(Stack[SP]); break;

          case    cIf:
              {
                  unsigned jumpAddr = ByteCode[++IP];
                  unsigned immedAddr = ByteCode[++IP];
                  if(doubleToInt(Stack[SP]) == 0)
                  {
                      IP = jumpAddr;
                      DP = immedAddr;
                  }
                  --SP; break;
              }

          case   cInt: Stack[SP] = floor(Stack[SP]+.5); break;

          case   cLog:
#                    ifndef FP_NO_EVALUATION_CHECKS
                       if(Stack[SP] <= 0) { evalErrorType=3; return 0; }
#                    endif
                       Stack[SP] = log(Stack[SP]); break;

          case cLog10:
#                    ifndef FP_NO_EVALUATION_CHECKS
                       if(Stack[SP] <= 0) { evalErrorType=3; return 0; }
#                    endif
                       Stack[SP] = log10(Stack[SP]); break;

          case  cLog2:
#                    ifndef FP_NO_EVALUATION_CHECKS
                       if(Stack[SP] <= 0) { evalErrorType=3; return 0; }
#                    endif
                     #ifdef FP_SUPPORT_LOG2
                       Stack[SP] = log2(Stack[SP]);
                     #else
                       Stack[SP] = log(Stack[SP]) * 1.4426950408889634074;
                     #endif
                       break;

          case   cMax: Stack[SP-1] = Max(Stack[SP-1], Stack[SP]);
                       --SP; break;

          case   cMin: Stack[SP-1] = Min(Stack[SP-1], Stack[SP]);
                       --SP; break;

          case   cPow: Stack[SP-1] = pow(Stack[SP-1], Stack[SP]);
                       --SP; break;
          case   cRPow: Stack[SP-1] = pow(Stack[SP], Stack[SP-1]);
                        --SP; break;

          case   cSec:
              {
                  const double c = cos(Stack[SP]);
#               ifndef FP_NO_EVALUATION_CHECKS
                  if(c == 0) { evalErrorType=1; return 0; }
#               endif
                  Stack[SP] = 1/c; break;
              }

          case   cSin: Stack[SP] = sin(Stack[SP]); break;

          case  cSinh: Stack[SP] = sinh(Stack[SP]); break;

          case  cSqrt:
#                    ifndef FP_NO_EVALUATION_CHECKS
                       if(Stack[SP] < 0) { evalErrorType=2; return 0; }
#                    endif
                       Stack[SP] = sqrt(Stack[SP]); break;

          case   cTan: Stack[SP] = tan(Stack[SP]); break;

          case  cTanh: Stack[SP] = tanh(Stack[SP]); break;


// Misc:
          case cImmed: Stack[++SP] = Immed[DP++]; break;

          case  cJump: DP = ByteCode[IP+2];
                       IP = ByteCode[IP+1];
                       break;

// Operators:
          case   cNeg: Stack[SP] = -Stack[SP]; break;
          case   cAdd: Stack[SP-1] += Stack[SP]; --SP; break;
          case   cSub: Stack[SP-1] -= Stack[SP]; --SP; break;
          case   cMul: Stack[SP-1] *= Stack[SP]; --SP; break;

          case   cDiv:
#                    ifndef FP_NO_EVALUATION_CHECKS
                       if(Stack[SP] == 0) { evalErrorType=1; return 0; }
#                    endif
                       Stack[SP-1] /= Stack[SP]; --SP; break;

          case   cMod:
#                    ifndef FP_NO_EVALUATION_CHECKS
                       if(Stack[SP] == 0) { evalErrorType=1; return 0; }
#                    endif
                       Stack[SP-1] = fmod(Stack[SP-1], Stack[SP]);
                       --SP; break;

#ifdef FP_EPSILON
          case cEqual: Stack[SP-1] =
                           (fabs(Stack[SP-1]-Stack[SP]) <= FP_EPSILON);
                       --SP; break;

          case cNEqual: Stack[SP-1] =
                            (fabs(Stack[SP-1] - Stack[SP]) >= FP_EPSILON);
                       --SP; break;

          case  cLess: Stack[SP-1] = (Stack[SP-1] < Stack[SP]-FP_EPSILON);
                       --SP; break;

          case  cLessOrEq: Stack[SP-1] = (Stack[SP-1] <= Stack[SP]+FP_EPSILON);
                       --SP; break;

          case cGreater: Stack[SP-1] = (Stack[SP-1]-FP_EPSILON > Stack[SP]);
                         --SP; break;

          case cGreaterOrEq: Stack[SP-1] =
                                 (Stack[SP-1]+FP_EPSILON >= Stack[SP]);
                         --SP; break;
#else
          case cEqual: Stack[SP-1] = (Stack[SP-1] == Stack[SP]);
                       --SP; break;

          case cNEqual: Stack[SP-1] = (Stack[SP-1] != Stack[SP]);
                       --SP; break;

          case  cLess: Stack[SP-1] = (Stack[SP-1] < Stack[SP]);
                       --SP; break;

          case  cLessOrEq: Stack[SP-1] = (Stack[SP-1] <= Stack[SP]);
                       --SP; break;

          case cGreater: Stack[SP-1] = (Stack[SP-1] > Stack[SP]);
                         --SP; break;

          case cGreaterOrEq: Stack[SP-1] = (Stack[SP-1] >= Stack[SP]);
                         --SP; break;
#endif

          case   cNot: Stack[SP] = !doubleToInt(Stack[SP]); break;

          case   cAnd: Stack[SP-1] =
                           (doubleToInt(Stack[SP-1]) &&
                            doubleToInt(Stack[SP]));
                       --SP; break;

          case    cOr: Stack[SP-1] =
                           (doubleToInt(Stack[SP-1]) ||
                            doubleToInt(Stack[SP]));
                       --SP; break;

          case cNotNot: Stack[SP] = !!doubleToInt(Stack[SP]); break;

// Degrees-radians conversion:
          case   cDeg: Stack[SP] = RadiansToDegrees(Stack[SP]); break;
          case   cRad: Stack[SP] = DegreesToRadians(Stack[SP]); break;

// User-defined function calls:
          case cFCall:
              {
                  unsigned index = ByteCode[++IP];
                  unsigned params = data->FuncPtrs[index].params;
                  double retVal =
                      data->FuncPtrs[index].funcPtr(&Stack[SP-params+1]);
                  SP -= int(params)-1;
                  Stack[SP] = retVal;
                  break;
              }

          case cPCall:
              {
                  unsigned index = ByteCode[++IP];
                  unsigned params = data->FuncParsers[index].params;
                  double retVal =
                      data->FuncParsers[index].parserPtr->Eval
                      (&Stack[SP-params+1]);
                  SP -= int(params)-1;
                  Stack[SP] = retVal;
                  const int error =
                      data->FuncParsers[index].parserPtr->EvalError();
                  if(error)
                  {
                      evalErrorType = error;
                      return 0;
                  }
                  break;
              }


#ifdef FP_SUPPORT_OPTIMIZER
          case   cVar: break;  // Paranoia. These should never exist

          case   cFetch:
              {
                  unsigned stackOffs = ByteCode[++IP];
                  Stack[SP+1] = Stack[stackOffs]; ++SP;
                  break;
              }

          case   cPopNMov:
              {
                  unsigned stackOffs_target = ByteCode[++IP];
                  unsigned stackOffs_source = ByteCode[++IP];
                  Stack[stackOffs_target] = Stack[stackOffs_source];
                  SP = stackOffs_target;
                  break;
              }
#endif // FP_SUPPORT_OPTIMIZER

          case   cDup: Stack[SP+1] = Stack[SP]; ++SP; break;

          case   cInv:
#           ifndef FP_NO_EVALUATION_CHECKS
              if(Stack[SP] == 0.0) { evalErrorType=1; return 0; }
#           endif
              Stack[SP] = 1.0/Stack[SP];
              break;

          case   cSqr:
              Stack[SP] = Stack[SP]*Stack[SP];
              break;

          case   cRDiv:
#                    ifndef FP_NO_EVALUATION_CHECKS
                        if(Stack[SP-1] == 0) { evalErrorType=1; return 0; }
#                    endif
                        Stack[SP-1] = Stack[SP] / Stack[SP-1]; --SP; break;

          case   cRSub: Stack[SP-1] = Stack[SP] - Stack[SP-1]; --SP; break;

          case   cRSqrt:
#                      ifndef FP_NO_EVALUATION_CHECKS
                         if(Stack[SP] == 0) { evalErrorType=1; return 0; }
#                      endif
                         Stack[SP] = 1.0 / sqrt(Stack[SP]); break;

          case cNop: break;

// Variables:
          default:
              Stack[++SP] = Vars[ByteCode[IP]-VarBegin];
        }
    }

    evalErrorType=0;
    return Stack[SP];
}


//===========================================================================
// Variable deduction
//===========================================================================
namespace
{
    int deduceVariables(FunctionParser& fParser,
                        const char* funcStr,
                        std::string& destVarString,
                        int* amountOfVariablesFound,
                        std::vector<std::string>* destVarNames,
                        bool useDegrees)
    {
        typedef std::set<std::string> StrSet;
        StrSet varNames;

        int oldIndex = -1;

        while(true)
        {
            destVarString.clear();
            for(StrSet::iterator iter = varNames.begin();
                iter != varNames.end();
                ++iter)
            {
                if(iter != varNames.begin()) destVarString += ",";
                destVarString += *iter;
            }

            const int index =
                fParser.Parse(funcStr, destVarString, useDegrees);
            if(index < 0) break;
            if(index == oldIndex) return index;

            const char* endPtr = readIdentifier(funcStr + index);
            if(endPtr == funcStr + index) return index;

            varNames.insert(std::string(funcStr + index, endPtr));
            oldIndex = index;
        }

        if(amountOfVariablesFound)
            *amountOfVariablesFound = int(varNames.size());

        if(destVarNames)
            destVarNames->assign(varNames.begin(), varNames.end());

        return -1;
    }
}

int FunctionParser::ParseAndDeduceVariables
(const std::string& function,
 int* amountOfVariablesFound,
 bool useDegrees)
{
    std::string varString;
    return deduceVariables(*this, function.c_str(), varString,
                           amountOfVariablesFound, 0, useDegrees);
}

int FunctionParser::ParseAndDeduceVariables
(const std::string& function,
 std::string& resultVarString,
 int* amountOfVariablesFound,
 bool useDegrees)
{
    std::string varString;
    const int index =
        deduceVariables(*this, function.c_str(), varString,
                        amountOfVariablesFound, 0, useDegrees);
    if(index < 0) resultVarString = varString;
    return index;
}

int FunctionParser::ParseAndDeduceVariables
(const std::string& function,
 std::vector<std::string>& resultVars,
 bool useDegrees)
{
    std::string varString;
    std::vector<std::string> vars;
    const int index =
        deduceVariables(*this, function.c_str(), varString,
                        0, &vars, useDegrees);
    if(index < 0) resultVars.swap(vars);
    return index;
}


//===========================================================================
// Debug output
//===========================================================================
#ifdef FUNCTIONPARSER_SUPPORT_DEBUG_OUTPUT
#include <iomanip>
#include <sstream>
namespace
{
    inline void printHex(std::ostream& dest, unsigned n)
    {
        std::ios::fmtflags flags = dest.flags();
        dest.width(4); dest.fill('0'); std::hex(dest); //uppercase(dest);
        dest << n;
        dest.flags(flags);
    }

    void padLine(std::ostringstream& dest, unsigned destLength)
    {
        for(size_t currentLength = dest.str().length();
            currentLength < destLength;
            ++currentLength)
        {
            dest << ' ';
        }
    }

    typedef std::map<FUNCTIONPARSERTYPES::NamePtr, unsigned> VariablesMap;
    std::string findVariableName(const VariablesMap& varMap, unsigned index)
    {
        for(VariablesMap::const_iterator iter = varMap.begin();
            iter != varMap.end();
            ++iter)
        {
            if(iter->second == index)
                return std::string(iter->first.name,
                                   iter->first.name + iter->first.nameLength);
        }
        return "?";
    }

    typedef std::vector<double> FactorStack;

    const struct PowiMuliType
    {
        unsigned opcode_square;
        unsigned opcode_cumulate;
        unsigned opcode_invert;
        unsigned opcode_half;
        unsigned opcode_invhalf;
    } iseq_powi = {cSqr,cMul,cInv,cSqrt,cRSqrt},
      iseq_muli = {~unsigned(0), cAdd,cNeg, ~unsigned(0),~unsigned(0) };

    double ParsePowiMuli(
        const PowiMuliType& opcodes,
        const std::vector<unsigned>& ByteCode, unsigned& IP,
        unsigned limit,
        size_t factor_stack_base,
        FactorStack& stack)
    {
        double result = 1.0;
        while(IP < limit)
        {
            if(ByteCode[IP] == opcodes.opcode_square)
            {
                if(!IsIntegerConst(result)) break;
                result *= 2;
                ++IP;
                continue;
            }
            if(ByteCode[IP] == opcodes.opcode_invert)
            {
                if(result < 0) break;
                result = -result;
                ++IP;
                continue;
            }
            if(ByteCode[IP] == opcodes.opcode_half)
            {
                if(IsIntegerConst(result) && result > 0 &&
                   ((long)result) % 2 == 0)
                    break;
                if(IsIntegerConst(result * 0.5)) break;
                result *= 0.5;
                ++IP;
                continue;
            }
            if(ByteCode[IP] == opcodes.opcode_invhalf)
            {
                if(IsIntegerConst(result) && result > 0 &&
                   ((long)result) % 2 == 0)
                    break;
                if(IsIntegerConst(result * -0.5)) break;
                result *= -0.5;
                ++IP;
                continue;
            }

            unsigned dup_fetch_pos = IP;
            double lhs = 1.0;

    #ifdef FP_SUPPORT_OPTIMIZER
            if(ByteCode[IP] == cFetch)
            {
                unsigned index = ByteCode[++IP];
                if(index < factor_stack_base
                || size_t(index-factor_stack_base) >= stack.size())
                {
                    // It wasn't a powi-fetch after all
                    IP = dup_fetch_pos;
                    break;
                }
                lhs = stack[index - factor_stack_base];
                // Note: ^This assumes that cFetch of recentmost
                //        is always converted into cDup.
                goto dup_or_fetch;
            }
    #endif
            if(ByteCode[IP] == cDup)
            {
                lhs = result;
                goto dup_or_fetch;

            dup_or_fetch:
                stack.push_back(result);
                ++IP;
                double subexponent = ParsePowiMuli
                    (opcodes,
                     ByteCode, IP, limit,
                     factor_stack_base, stack);
                if(IP >= limit || ByteCode[IP] != opcodes.opcode_cumulate)
                {
                    // It wasn't a powi-dup after all
                    IP = dup_fetch_pos;
                    break;
                }
                ++IP; // skip opcode_cumulate
                stack.pop_back();
                result += lhs*subexponent;
                continue;
            }
            break;
        }
        return result;
    }

    double ParsePowiSequence(const std::vector<unsigned>& ByteCode,
                             unsigned& IP, unsigned limit,
                             size_t factor_stack_base)
    {
        FactorStack stack;
        stack.push_back(1.0);
        return ParsePowiMuli(iseq_powi, ByteCode, IP, limit,
                             factor_stack_base, stack);
    }

    double ParseMuliSequence(const std::vector<unsigned>& ByteCode,
                             unsigned& IP, unsigned limit,
                             size_t factor_stack_base)
    {
        FactorStack stack;
        stack.push_back(1.0);
        return ParsePowiMuli(iseq_muli, ByteCode, IP, limit,
                             factor_stack_base, stack);
    }

    struct IfInfo
    {
        std::pair<int,std::string> condition;
        std::pair<int,std::string> thenbranch;
        unsigned endif_location;

        IfInfo() : condition(), thenbranch(), endif_location() { }
    };
}

void FunctionParser::PrintByteCode(std::ostream& dest,
                                   bool showExpression) const
{
    dest << "Size of stack: " << data->StackSize << "\n";

    std::ostringstream outputBuffer;
    std::ostream& output = (showExpression ? outputBuffer : dest);

    const std::vector<unsigned>& ByteCode = data->ByteCode;
    const std::vector<double>& Immed = data->Immed;

    std::vector<std::pair<int,std::string> > stack;
    std::vector<IfInfo> if_stack;

    for(unsigned IP = 0, DP = 0; IP <= ByteCode.size(); ++IP)
    {
    after_powi_or_muli:;
        std::string n;
        bool out_params = false;
        unsigned params = 2, produces = 1, opcode = 0;

        if(showExpression && !if_stack.empty() &&
           if_stack.back().endif_location == IP)
        {
            printHex(output, IP);
            output << ": (phi)";
            stack.resize(stack.size()+2);
            std::swap(stack[stack.size()-3], stack[stack.size()-1]);
            std::swap(if_stack.back().condition,  stack[stack.size()-3]);
            std::swap(if_stack.back().thenbranch, stack[stack.size()-2]);
            opcode = cIf;
            params = 3;
            --IP;
            if_stack.pop_back();
        }
        else
        {
            if(IP >= ByteCode.size()) break;
            opcode = ByteCode[IP];

            if(showExpression && (
                opcode == cSqr || opcode == cDup
             || opcode == cInv
             || opcode == cSqrt || opcode == cRSqrt
    #ifdef FP_SUPPORT_OPTIMIZER
             || opcode == cFetch
    #endif
            ))
            {
                unsigned changed_ip = IP;
                double exponent =
                    ParsePowiSequence(ByteCode, changed_ip,
                                      if_stack.empty()
                                      ? (unsigned)ByteCode.size()
                                      : if_stack.back().endif_location,
                                      stack.size()-1);
                std::ostringstream operation;
                int prio = 0;
                if(exponent == 1.0)
                {
                    if(opcode != cDup) goto not_powi_or_muli;
                    double factor =
                        ParseMuliSequence(ByteCode, changed_ip,
                                          if_stack.empty()
                                          ? (unsigned)ByteCode.size()
                                          : if_stack.back().endif_location,
                                          stack.size()-1);
                    if(factor == 1.0 || factor == -1.0) goto not_powi_or_muli;
                    operation << '*' << factor;
                    prio = 3;
                }
                else
                {
                    prio = 2;
                    operation << '^' << exponent;
                }

                unsigned explanation_before = changed_ip-2;
                const char* explanation_prefix = "_";
                for(const unsigned first_ip = IP; IP < changed_ip; ++IP)
                {
                    printHex(output, IP);
                    output << ": ";

                    const char* sep = "|";
                    if(first_ip+1 == changed_ip)
                    { sep = "="; explanation_prefix = " "; }
                    else if(IP   == first_ip) sep = "\\";
                    else if(IP+1 == changed_ip) sep = "/";
                    else explanation_prefix = "=";

                    switch(ByteCode[IP])
                    {
                        case cInv: output << "inv"; break;
                        case cNeg: output << "neg"; break;
                        case cDup: output << "dup"; break;
                        case cSqr: output << "sqr"; break;
                        case cMul: output << "mul"; break;
                        case cAdd: output << "add"; break;
                        case cSqrt: output << "sqrt"; break;
                        case cRSqrt: output << "rsqrt"; break;
    #ifdef FP_SUPPORT_OPTIMIZER
                        case cFetch:
                        {
                            unsigned index = ByteCode[++IP];
                            output << "cFetch(" << index << ")";
                            break;
                        }
    #endif
                        default: break;
                    }
                    padLine(outputBuffer, 20);
                    output << sep;
                    if(IP >= explanation_before)
                    {
                        explanation_before = (unsigned)ByteCode.size();
                        output << explanation_prefix
                               << '[' << (stack.size()-1) << ']';
                        std::string& last = stack.back().second;
                        if(stack.back().first >= prio)
                            last = "(" + last + ")";
                        last += operation.str();
                        output << last;
                        stack.back().first = prio;
                    }
                    dest << outputBuffer.str() << std::endl;
                    outputBuffer.str("");
                }
                goto after_powi_or_muli;
            }
        not_powi_or_muli:;
            printHex(output, IP);
            output << ": ";

            switch(opcode)
            {
              case cIf:
              {
                  unsigned label = ByteCode[IP+1]+1;
                  output << "jz ";
                  printHex(output, label);
                  params = 1;
                  produces = 0;
                  IP += 2;

                  if_stack.resize(if_stack.size() + 1);
                  std::swap( if_stack.back().condition, stack.back() );
                  if_stack.back().endif_location = (unsigned) ByteCode.size();
                  stack.pop_back();
                  break;
              }

              case cJump:
              {
                  unsigned label = ByteCode[IP+1]+1;

                  std::swap(if_stack.back().thenbranch, stack.back());
                  if_stack.back().endif_location = label;
                  stack.pop_back();

                  output << "jump ";
                  printHex(output, label);
                  params = 0;
                  produces = 0;
                  IP += 2;
                  break;
              }
              case cImmed:
              {
                  if(showExpression)
                  {
                      std::ostringstream buf;
                      buf.precision(8);
                      buf << Immed[DP];
                      stack.push_back( std::make_pair(0, buf.str()) );
                  }
                  output.precision(8);
                  output << "push " << Immed[DP];
                  ++DP;
                  produces = 0;
                  break;
              }

              case cFCall:
                  {
                      const unsigned index = ByteCode[++IP];
                      params = data->FuncPtrs[index].params;
                      std::set<NameData>::const_iterator iter =
                          data->nameData.begin();
                      while(iter->type != NameData::FUNC_PTR ||
                            iter->index != index)
                          ++iter;
                      output << "fcall " << iter->name;
                      out_params = true;
                      break;
                  }

              case cPCall:
                  {
                      const unsigned index = ByteCode[++IP];
                      params = data->FuncParsers[index].params;
                      std::set<NameData>::const_iterator iter =
                          data->nameData.begin();
                      while(iter->type != NameData::PARSER_PTR ||
                            iter->index != index)
                          ++iter;
                      output << "pcall " << iter->name;
                      out_params = true;
                      break;
                  }

              default:
                  if(OPCODE(opcode) < VarBegin)
                  {
                      switch(opcode)
                      {
                        case cNeg: n = "neg"; params = 1; break;
                        case cAdd: n = "add"; break;
                        case cSub: n = "sub"; break;
                        case cMul: n = "mul"; break;
                        case cDiv: n = "div"; break;
                        case cMod: n = "mod"; break;
                        case cPow: n = "pow"; break;
                        case cRPow: n = "rpow"; break;
                        case cEqual: n = "eq"; break;
                        case cNEqual: n = "neq"; break;
                        case cLess: n = "lt"; break;
                        case cLessOrEq: n = "le"; break;
                        case cGreater: n = "gt"; break;
                        case cGreaterOrEq: n = "ge"; break;
                        case cAnd: n = "and"; break;
                        case cOr: n = "or"; break;
                        case cNot: n = "not"; params = 1; break;
                        case cNotNot: n = "notnot"; params = 1; break;
                        case cDeg: n = "deg"; params = 1; break;
                        case cRad: n = "rad"; params = 1; break;

    #ifndef FP_DISABLE_EVAL
                        case cEval: n = "call 0"; break;
    #endif

    #ifdef FP_SUPPORT_OPTIMIZER
                        case cVar:    n = "(var)"; break;
                        case cFetch:
                        {
                            unsigned index = ByteCode[++IP];
                            if(showExpression)
                                stack.push_back(stack[index]);
                            output << "cFetch(" << index << ")";
                            produces = 0;
                            break;
                        }
                        case cPopNMov:
                        {
                            size_t a = ByteCode[++IP];
                            size_t b = ByteCode[++IP];
                            if(showExpression)
                            {
                                std::pair<int, std::string> stacktop(0, "?");
                                if(b < stack.size()) stacktop = stack[b];
                                stack.resize(a);
                                stack.push_back(stacktop);
                            }
                            output << "cPopNMov(" << a << ", " << b << ")";
                            produces = 0;
                            break;
                        }
    #endif
                        case cDup:
                        {
                            if(showExpression)
                                stack.push_back(stack.back());
                            output << "dup";
                            produces = 0;
                            break;
                        }
                        case cInv: n = "inv"; params = 1; break;
                        case cSqr: n = "sqr"; params = 1; break;
                        case cRDiv: n = "rdiv"; break;
                        case cRSub: n = "rsub"; break;
                        case cRSqrt: n = "rsqrt"; params = 1; break;

                        case cNop:
                            output << "nop"; params = 0; produces = 0;
                            break;

                        default:
                            n = Functions[opcode-cAbs].name;
                            params = Functions[opcode-cAbs].params;
                            out_params = params != 1;
                      }
                  }
                  else
                  {
                      if(showExpression)
                      {
                          stack.push_back(std::make_pair(0,
                              (findVariableName(data->variableRefs, opcode))));
                      }
                      output << "push Var" << opcode-VarBegin;
                      produces = 0;
                  }
            }
        }
        if(produces) output << n;
        if(out_params) output << " (" << params << ")";
        if(showExpression)
        {
            padLine(outputBuffer, 20);

            if(produces > 0)
            {
                std::ostringstream buf;
                const char *paramsep = ",", *suff = "";
                int prio = 0; bool commutative = false;
                switch(opcode)
                {
                  case cIf: buf << "if("; suff = ")";
                      break;
                  case cOr:  prio = 6; paramsep = "|"; commutative = true;
                      break;
                  case cAnd: prio = 5; paramsep = "&"; commutative = true;
                      break;
                  case cAdd: prio = 4; paramsep = "+"; commutative = true;
                      break;
                  case cSub: prio = 4; paramsep = "-";
                      break;
                  case cMul: prio = 3; paramsep = "*"; commutative = true;
                      break;
                  case cDiv: prio = 3; paramsep = "/";
                      break;
                  case cPow: prio = 2; paramsep = "^";
                      break;
#ifdef FP_SUPPORT_OPTIMIZER
                  case cSqr: prio = 2; suff = "^2";
                      break;
#endif
                  case cNeg: buf << "(-"; suff = ")";
                      break;
                  default: buf << n << '('; suff = ")";
                }

                const char* sep = "";
                for(unsigned a=0; a<params; ++a)
                {
                    buf << sep;
                    if(stack.size() + a < params)
                        buf << "?";
                    else
                    {
                        const std::pair<int,std::string>& prev =
                            stack[stack.size() - params + a];
                        if(prio > 0 && (prev.first > prio ||
                                        (prev.first==prio && !commutative)))
                            buf << '(' << prev.second << ')';
                        else
                            buf << prev.second;
                    }
                    sep = paramsep;
                }
                if(stack.size() >= params)
                    stack.resize(stack.size() - params);
                else
                    stack.clear();
                buf << suff;
                stack.push_back(std::make_pair(prio, buf.str()));
                //if(n.size() <= 4 && !out_params) padLine(outputBuffer, 20);
            }
            //padLine(outputBuffer, 20);
            output << "= ";
            if((opcode == cIf && params != 3) ||
               opcode == cJump || opcode == cNop)
                output << "(void)";
            else if(stack.empty())
                output << "[?] ?";
            else
                output << '[' << (stack.size()-1) << ']'
                       << stack.back().second;
        }

        if(showExpression)
        {
            dest << outputBuffer.str() << std::endl;
            outputBuffer.str("");
        }
        else
            output << std::endl;
    }
}
#endif


#ifndef FP_SUPPORT_OPTIMIZER
void FunctionParser::Optimize()
{
    // Do nothing if no optimizations are supported.
}
#endif

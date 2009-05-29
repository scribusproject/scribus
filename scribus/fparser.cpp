/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
//==============================
// Function parser v2.8 by Warp
//==============================

#include "fparser.h"
#include "fpconfig.h"
#include "fptypes.h"
using namespace FUNCTIONPARSERTYPES;

#include "scclocale.h"
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>

using namespace std;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace
{
    const unsigned FUNC_AMOUNT = sizeof(Functions)/sizeof(Functions[0]);


    // BCB4 does not implement the standard lower_bound function.
    // This is used instead:
    const FuncDefinition* fp_lower_bound(const FuncDefinition* first,
                                         const FuncDefinition* last,
                                         const FuncDefinition& value)
    {
        while(first < last)
        {
            const FuncDefinition* middle = first+(last-first)/2;
            if(*middle < value) first = middle+1;
            else last = middle;
        }
        return last;
    }


    // Returns a pointer to the FuncDefinition instance which 'name' is
    // the same as the one given by 'F'. If no such function name exists,
    // returns 0.
    inline const FuncDefinition* FindFunction(const char* F)
    {
        FuncDefinition func = { F, 0, 0, 0 };
        while(isalnum(F[func.nameLength])) ++func.nameLength;
        if(func.nameLength)
        {
            const FuncDefinition* found =
                fp_lower_bound(Functions, Functions+FUNC_AMOUNT, func);
            if(found == Functions+FUNC_AMOUNT || func < *found)
                return 0;
            return found;
        }
        return 0;
    }
}


//---------------------------------------------------------------------------
// Copy-on-write method
//---------------------------------------------------------------------------
void FunctionParser::copyOnWrite()
{
    if(data->referenceCounter > 1)
    {
        Data* oldData = data;
        data = new Data(*oldData);
        --(oldData->referenceCounter);
        data->referenceCounter = 1;
    }
}


//---------------------------------------------------------------------------
// Constructors and destructors
//---------------------------------------------------------------------------
//===========================================================================
FunctionParser::FunctionParser():
    parseErrorType(FP_NO_ERROR), evalErrorType(0),
    data(new Data),
    evalRecursionLevel(0)
{
    data->referenceCounter = 1;
}

FunctionParser::~FunctionParser()
{
    if(--(data->referenceCounter) == 0)
    {
        delete data;
    }
}

FunctionParser::FunctionParser(const FunctionParser& cpy):
    parseErrorType(cpy.parseErrorType),
    evalErrorType(cpy.evalErrorType),
    data(cpy.data),
    evalRecursionLevel(0)
{
    ++(data->referenceCounter);
}

FunctionParser& FunctionParser::operator=(const FunctionParser& cpy)
{
    if(data != cpy.data)
    {
        if(--(data->referenceCounter) == 0) delete data;

        parseErrorType = cpy.parseErrorType;
        evalErrorType = cpy.evalErrorType;
        data = cpy.data;
        evalRecursionLevel = cpy.evalRecursionLevel;

        ++(data->referenceCounter);
    }

    return *this;
}


FunctionParser::Data::Data():
    useDegreeConversion(false),
    ByteCode(0), ByteCodeSize(0),
    Immed(0), ImmedSize(0),
    Stack(0), StackSize(0)
{}

FunctionParser::Data::~Data()
{
    if(ByteCode) { delete[] ByteCode; ByteCode=0; }
    if(Immed) { delete[] Immed; Immed=0; }
    if(Stack) { delete[] Stack; Stack=0; }
}

// Makes a deep-copy of Data:
FunctionParser::Data::Data(const Data& cpy):
    varAmount(cpy.varAmount), useDegreeConversion(cpy.useDegreeConversion),
    Variables(cpy.Variables), Constants(cpy.Constants),
    FuncPtrNames(cpy.FuncPtrNames), FuncPtrs(cpy.FuncPtrs),
    FuncParserNames(cpy.FuncParserNames), FuncParsers(cpy.FuncParsers),
    ByteCode(0), ByteCodeSize(cpy.ByteCodeSize),
    Immed(0), ImmedSize(cpy.ImmedSize),
    Stack(0), StackSize(cpy.StackSize)
{
    if(ByteCodeSize) ByteCode = new unsigned[ByteCodeSize];
    if(ImmedSize) Immed = new double[ImmedSize];
    if(StackSize) Stack = new double[StackSize];

    for(unsigned i=0; i<ByteCodeSize; ++i) ByteCode[i] = cpy.ByteCode[i];
    for(unsigned i=0; i<ImmedSize; ++i) Immed[i] = cpy.Immed[i];

    // No need to copy the stack contents because it's obsolete outside Eval()
}


//---------------------------------------------------------------------------
// Function parsing
//---------------------------------------------------------------------------
//===========================================================================
namespace
{
    // Error messages returned by ErrorMsg():
    const char* ParseErrorMessage[]=
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
        ""
    };


    // Parse variables
    bool ParseVars(const string& Vars, map<string, unsigned>& dest)
    {
        unsigned varNumber = VarBegin;
        unsigned ind1 = 0, ind2;

        while(ind1 < Vars.size())
        {
            if(!isalpha(Vars[ind1]) && Vars[ind1]!='_') return false;
            for(ind2=ind1+1; ind2<Vars.size() && Vars[ind2]!=','; ++ind2)
                if(!isalnum(Vars[ind2]) && Vars[ind2]!='_') return false;
            const string varName = Vars.substr(ind1, ind2-ind1);

            if(dest.insert(make_pair(varName, varNumber++)).second == false)
                return false;

            ind1 = ind2+1;
        }
        return true;
    }
}

bool FunctionParser::isValidName(const std::string& name) const
{
    if(name.empty() || (!isalpha(name[0]) && name[0] != '_')) return false;
    for(unsigned i=0; i<name.size(); ++i)
        if(!isalnum(name[i]) && name[i] != '_') return false;

    if(FindFunction(name.c_str())) return false;

    return true;
}


// Constants:
bool FunctionParser::AddConstant(const std::string& name, double value)
{
    if(isValidName(name))
    {
        const char* n = name.c_str();
        if(FindVariable(n, data->FuncParserNames) !=
           data->FuncParserNames.end() ||
           FindVariable(n, data->FuncPtrNames) !=
           data->FuncPtrNames.end())
            return false;

        copyOnWrite();

        data->Constants[name] = value;
        return true;
    }
    return false;
}

// Function pointers
bool FunctionParser::AddFunction(const std::string& name,
                                 FunctionPtr func, unsigned paramsAmount)
{
    if(isValidName(name))
    {
        const char* n = name.c_str();
        if(FindVariable(n, data->FuncParserNames) !=
           data->FuncParserNames.end() ||
           FindConstant(n) != data->Constants.end())
            return false;

        copyOnWrite();

        data->FuncPtrNames[name] = data->FuncPtrs.size();
        data->FuncPtrs.push_back(Data::FuncPtrData(func, paramsAmount));
        return true;
    }
    return false;
}

bool FunctionParser::checkRecursiveLinking(const FunctionParser* fp) const
{
    if(fp == this) return true;
    for(unsigned i=0; i<fp->data->FuncParsers.size(); ++i)
        if(checkRecursiveLinking(fp->data->FuncParsers[i])) return true;
    return false;
}

bool FunctionParser::AddFunction(const std::string& name,
                                 FunctionParser& parser)
{
    if(isValidName(name))
    {
        const char* n = name.c_str();
        if(FindVariable(n, data->FuncPtrNames) != data->FuncPtrNames.end() ||
           FindConstant(n) != data->Constants.end())
            return false;

        if(checkRecursiveLinking(&parser)) return false;

        copyOnWrite();

        data->FuncParserNames[name] = data->FuncParsers.size();
        data->FuncParsers.push_back(&parser);
        return true;
    }
    return false;
}



// Main parsing function
// ---------------------
int FunctionParser::Parse(const std::string& Function,
                          const std::string& Vars,
                          bool useDegrees)
{
    copyOnWrite();

    data->Variables.clear();

    if(!ParseVars(Vars, data->Variables))
    {
        parseErrorType = INVALID_VARS;
        return Function.size();
    }
    data->varAmount = data->Variables.size(); // this is for Eval()

    const char* Func = Function.c_str();

    parseErrorType = FP_NO_ERROR;

    int Result = CheckSyntax(Func);
    if(Result>=0) return Result;

    data->useDegreeConversion = useDegrees;
    if(!Compile(Func)) return Function.size();

    data->Variables.clear();

    parseErrorType = FP_NO_ERROR;
    return -1;
}

namespace
{
    const char* const fpOperators[] =
    {
        "+", "-", "*", "/", "%", "^",
        "=", "!=", "<=", "<", ">=", ">", "&", "|",
        0
    };

    // Is given char an operator?
    // (Returns 0 if not, else the size of the operator)
    inline int IsOperator(const char* F)
    {
        for(unsigned opInd = 0; fpOperators[opInd]; ++opInd)
        {
            const char* op = fpOperators[opInd];
            for(unsigned n = 0; F[n] == *op; ++n)
            {
                ++op;
                if(*op == 0) return op-fpOperators[opInd];
            }
        }
        return 0;
    }

    // skip whitespace
    inline void sws(const char* F, int& Ind)
    {
        while(F[Ind] && isspace(F[Ind])) ++Ind;
    }
}

// Returns an iterator to the variable with the same name as 'F', or to
// Variables.end() if no such variable exists:
inline FunctionParser::Data::VarMap_t::const_iterator
FunctionParser::FindVariable(const char* F, const Data::VarMap_t& vars) const
{
    if(vars.size())
    {
        unsigned ind = 0;
        while(isalnum(F[ind]) || F[ind] == '_') ++ind;
        if(ind)
        {
            string name(F, ind);
            return vars.find(name);
        }
    }
    return vars.end();
}

inline FunctionParser::Data::ConstMap_t::const_iterator
FunctionParser::FindConstant(const char* F) const
{
    if(data->Constants.size())
    {
        unsigned ind = 0;
        while(isalnum(F[ind]) || F[ind] == '_') ++ind;
        if(ind)
        {
            string name(F, ind);
            return data->Constants.find(name);
        }
    }
    return data->Constants.end();
}

//---------------------------------------------------------------------------
// Check function string syntax
// ----------------------------
int FunctionParser::CheckSyntax(const char* Function)
{
    const Data::VarMap_t& Variables = data->Variables;
    const Data::ConstMap_t& Constants = data->Constants;
    const Data::VarMap_t& FuncPtrNames = data->FuncPtrNames;
    const Data::VarMap_t& FuncParserNames = data->FuncParserNames;

    vector<int> functionParenthDepth;

    int Ind=0, ParenthCnt=0, c;
    char* Ptr;

    while(true)
    {
        sws(Function, Ind);
        c=Function[Ind];

// Check for valid operand (must appear)

        // Check for leading - or !
        if(c=='-' || c=='!') { sws(Function, ++Ind); c=Function[Ind]; }
        if(c==0) { parseErrorType=PREMATURE_EOS; return Ind; }

        // Check for math function
        bool foundFunc = false;
        const FuncDefinition* fptr = FindFunction(&Function[Ind]);
        if(fptr)
        {
            Ind += fptr->nameLength;
            foundFunc = true;
        }
        else
        {
            // Check for user-defined function
            Data::VarMap_t::const_iterator fIter =
                FindVariable(&Function[Ind], FuncPtrNames);
            if(fIter != FuncPtrNames.end())
            {
                Ind += fIter->first.size();
                foundFunc = true;
            }
            else
            {
                Data::VarMap_t::const_iterator pIter =
                    FindVariable(&Function[Ind], FuncParserNames);
                if(pIter != FuncParserNames.end())
                {
                    Ind += pIter->first.size();
                    foundFunc = true;
                }
            }
        }

        if(foundFunc)
        {
            sws(Function, Ind);
            c = Function[Ind];
            if(c!='(') { parseErrorType=EXPECT_PARENTH_FUNC; return Ind; }

            int Ind2 = Ind+1;
            sws(Function, Ind2);
            if(Function[Ind2] == ')')
            {
                Ind = Ind2+1;
                sws(Function, Ind);
                c = Function[Ind];
                // Ugly, but other methods would just be uglier...
                goto CheckOperator;
            }

            functionParenthDepth.push_back(ParenthCnt+1);
        }

        // Check for opening parenthesis
        if(c=='(')
        {
            ++ParenthCnt;
            sws(Function, ++Ind);
            if(Function[Ind]==')') { parseErrorType=EMPTY_PARENTH; return Ind;}
            continue;
        }

        // Check for number
        if(isdigit(c) || (c=='.' && isdigit(Function[Ind+1])))
        {
            ScCLocale::strtod(&Function[Ind], &Ptr);
            Ind += int(Ptr-&Function[Ind]);
            sws(Function, Ind);
            c = Function[Ind];
        }
        else
        { // Check for variable
            Data::VarMap_t::const_iterator vIter =
                FindVariable(&Function[Ind], Variables);
            if(vIter != Variables.end())
                Ind += vIter->first.size();
            else
            {
                // Check for constant
                Data::ConstMap_t::const_iterator cIter =
                    FindConstant(&Function[Ind]);
                if(cIter != Constants.end())
                    Ind += cIter->first.size();
                else
                { parseErrorType=SYNTAX_ERROR; return Ind; }
            }
            sws(Function, Ind);
            c = Function[Ind];
        }

        // Check for closing parenthesis
        while(c==')')
        {
            if(functionParenthDepth.size() &&
               functionParenthDepth.back() == ParenthCnt)
                functionParenthDepth.pop_back();
            if((--ParenthCnt)<0) { parseErrorType=MISM_PARENTH; return Ind; }
            sws(Function, ++Ind);
            c=Function[Ind];
        }

// If we get here, we have a legal operand and now a legal operator or
// end of string must follow

    CheckOperator:
        // Check for EOS
        if(c==0) break; // The only way to end the checking loop without error

        // Check for operator
        int opSize = 0;
        if(c == ',' && !functionParenthDepth.empty() &&
           functionParenthDepth.back() == ParenthCnt)
            opSize = 1;
        else
            opSize = IsOperator(Function+Ind);
        if(opSize == 0)
        { parseErrorType=EXPECT_OPERATOR; return Ind; }

// If we get here, we have an operand and an operator; the next loop will
// check for another operand (must appear)
        Ind += opSize;
    } // while

    // Check that all opened parentheses are also closed
    if(ParenthCnt>0) { parseErrorType=MISSING_PARENTH; return Ind; }

// The string is ok
    parseErrorType=FP_NO_ERROR;
    return -1;
}


// Compile function string to bytecode
// -----------------------------------
bool FunctionParser::Compile(const char* Function)
{
    if(data->ByteCode) { delete[] data->ByteCode; data->ByteCode=0; }
    if(data->Immed) { delete[] data->Immed; data->Immed=0; }
    if(data->Stack) { delete[] data->Stack; data->Stack=0; }

    vector<unsigned> byteCode; byteCode.reserve(1024);
    tempByteCode = &byteCode;

    vector<double> immed; immed.reserve(1024);
    tempImmed = &immed;

    data->StackSize = StackPtr = 0;

    CompileExpression(Function, 0);
    if(parseErrorType != FP_NO_ERROR) return false;

    data->ByteCodeSize = byteCode.size();
    data->ImmedSize = immed.size();

    if(data->ByteCodeSize)
    {
        data->ByteCode = new unsigned[data->ByteCodeSize];
        memcpy(data->ByteCode, &byteCode[0],
               sizeof(unsigned)*data->ByteCodeSize);
    }
    if(data->ImmedSize)
    {
        data->Immed = new double[data->ImmedSize];
        memcpy(data->Immed, &immed[0],
               sizeof(double)*data->ImmedSize);
    }
    if(data->StackSize)
        data->Stack = new double[data->StackSize];

    return true;
}


inline void FunctionParser::AddCompiledByte(unsigned c)
{
    tempByteCode->push_back(c);
}

inline void FunctionParser::AddImmediate(double i)
{
    tempImmed->push_back(i);
}

inline void FunctionParser::AddFunctionOpcode(unsigned opcode)
{
    if(data->useDegreeConversion)
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
              AddCompiledByte(cRad);
        }

    AddCompiledByte(opcode);

    if(data->useDegreeConversion)
        switch(opcode)
        {
          case cAcos:
#ifndef NO_ASINH
          case cAcosh:
          case cAsinh:
          case cAtanh:
#endif
          case cAsin:
          case cAtan:
          case cAtan2:
              AddCompiledByte(cDeg);
        }
}

inline void FunctionParser::incStackPtr()
{
    if(++StackPtr > data->StackSize) ++(data->StackSize);
}


// Compile if()
int FunctionParser::CompileIf(const char* F, int ind)
{
    int ind2 = CompileExpression(F, ind, true); // condition
    sws(F, ind2);
    if(F[ind2] != ',') { parseErrorType=ILL_PARAMS_AMOUNT; return ind2; }
    AddCompiledByte(cIf);
    unsigned curByteCodeSize = tempByteCode->size();
    AddCompiledByte(0); // Jump index; to be set later
    AddCompiledByte(0); // Immed jump index; to be set later

    --StackPtr;

    ind2 = CompileExpression(F, ind2+1, true); // then
    sws(F, ind2);
    if(F[ind2] != ',') { parseErrorType=ILL_PARAMS_AMOUNT; return ind2; }
    AddCompiledByte(cJump);
    unsigned curByteCodeSize2 = tempByteCode->size();
    unsigned curImmedSize2 = tempImmed->size();
    AddCompiledByte(0); // Jump index; to be set later
    AddCompiledByte(0); // Immed jump index; to be set later

    --StackPtr;

    ind2 = CompileExpression(F, ind2+1, true); // else
    sws(F, ind2);
    if(F[ind2] != ')') { parseErrorType=ILL_PARAMS_AMOUNT; return ind2; }

    // Set jump indices
    (*tempByteCode)[curByteCodeSize] = curByteCodeSize2+1;
    (*tempByteCode)[curByteCodeSize+1] = curImmedSize2;
    (*tempByteCode)[curByteCodeSize2] = tempByteCode->size()-1;
    (*tempByteCode)[curByteCodeSize2+1] = tempImmed->size();

    return ind2+1;
}

int FunctionParser::CompileFunctionParams(const char* F, int ind,
                                          unsigned requiredParams)
{
    int ind2 = ind;
    if(requiredParams > 0)
    {
        unsigned curStackPtr = StackPtr;
        ind2 = CompileExpression(F, ind);

        if(StackPtr != curStackPtr+requiredParams)
        { parseErrorType=ILL_PARAMS_AMOUNT; return ind; }

        StackPtr -= requiredParams - 1;
    }
    else
    {
        incStackPtr();
    }

    sws(F, ind2);
    return ind2+1; // F[ind2] is ')'
}

// Compiles element
int FunctionParser::CompileElement(const char* F, int ind)
{
    sws(F, ind);
    char c = F[ind];

    if(c == '(')
    {
        ind = CompileExpression(F, ind+1);
        sws(F, ind);
        return ind+1; // F[ind] is ')'
    }

    if(isdigit(c) || c=='.' /*|| c=='-'*/) // Number
    {
        const char* startPtr = &F[ind];
        char* endPtr;
        double val = ScCLocale::strtod(startPtr, &endPtr);
        AddImmediate(val);
        AddCompiledByte(cImmed);
        incStackPtr();
        return ind+(endPtr-startPtr);
    }

    if(isalpha(c) || c == '_') // Function, variable or constant
    {
        const FuncDefinition* func = FindFunction(F+ind);
        if(func) // is function
        {
            int ind2 = ind + func->nameLength;
            sws(F, ind2); // F[ind2] is '('
            if(strcmp(func->name, "if") == 0) // "if" is a special case
            {
                return CompileIf(F, ind2+1);
            }

#ifndef DISABLE_EVAL
            unsigned requiredParams =
                strcmp(func->name, "eval") == 0 ?
                data->Variables.size() : func->params;
#else
            unsigned requiredParams = func->params;
#endif
            ind2 = CompileFunctionParams(F, ind2+1, requiredParams);
            AddFunctionOpcode(func->opcode);
            return ind2; // F[ind2-1] is ')'
        }

        Data::VarMap_t::const_iterator vIter =
            FindVariable(F+ind, data->Variables);
        if(vIter != data->Variables.end()) // is variable
        {
            AddCompiledByte(vIter->second);
            incStackPtr();
            return ind + vIter->first.size();
        }

        Data::ConstMap_t::const_iterator cIter = FindConstant(F+ind);
        if(cIter != data->Constants.end()) // is constant
        {
            AddImmediate(cIter->second);
            AddCompiledByte(cImmed);
            incStackPtr();
            return ind + cIter->first.size();
        }

        Data::VarMap_t::const_iterator fIter =
            FindVariable(F+ind, data->FuncPtrNames);
        if(fIter != data->FuncPtrNames.end()) // is user-defined func pointer
        {
            unsigned index = fIter->second;

            int ind2 = ind + fIter->first.length();
            sws(F, ind2); // F[ind2] is '('

            ind2 = CompileFunctionParams(F, ind2+1,
                                         data->FuncPtrs[index].params);

            AddCompiledByte(cFCall);
            AddCompiledByte(index);
            return ind2;
        }

        Data::VarMap_t::const_iterator pIter =
            FindVariable(F+ind, data->FuncParserNames);
        if(pIter != data->FuncParserNames.end()) // is user-defined func parser
        {
            unsigned index = pIter->second;

            int ind2 = ind + pIter->first.length();
            sws(F, ind2); // F[ind2] is '('

            ind2 = CompileFunctionParams
                (F, ind2+1, data->FuncParsers[index]->data->varAmount);

            AddCompiledByte(cPCall);
            AddCompiledByte(index);
            return ind2;
        }
    }

    parseErrorType = UNEXPECTED_ERROR;
    return ind;
}

// Compiles '^'
int FunctionParser::CompilePow(const char* F, int ind)
{
    int ind2 = CompileElement(F, ind);
    sws(F, ind2);

    while(F[ind2] == '^')
    {
        ind2 = CompileUnaryMinus(F, ind2+1);
        sws(F, ind2);
        AddCompiledByte(cPow);
        --StackPtr;
    }

    return ind2;
}

// Compiles unary '-'
int FunctionParser::CompileUnaryMinus(const char* F, int ind)
{
    sws(F, ind);
    if(F[ind] == '-' || F[ind] == '!')
    {
        int ind2 = ind+1;
        sws(F, ind2);
        ind2 = CompilePow(F, ind2);
        sws(F, ind2);

        // if we are negating a constant, negate the constant itself:
        if(F[ind] == '-' && tempByteCode->back() == cImmed)
            tempImmed->back() = -tempImmed->back();

        // if we are negating a negation, we can remove both:
        else if((F[ind] == '-' && tempByteCode->back() == cNeg))
            tempByteCode->pop_back();

        else
            AddCompiledByte(F[ind] == '-' ? cNeg : cNot);

        return ind2;
    }

    int ind2 = CompilePow(F, ind);
    sws(F, ind2);
    return ind2;
}

// Compiles '*', '/' and '%'
int FunctionParser::CompileMult(const char* F, int ind)
{
    int ind2 = CompileUnaryMinus(F, ind);
    sws(F, ind2);
    char op;

    while((op = F[ind2]) == '*' || op == '/' || op == '%')
    {
        ind2 = CompileUnaryMinus(F, ind2+1);
        sws(F, ind2);
        switch(op)
        {
          case '*': AddCompiledByte(cMul); break;
          case '/': AddCompiledByte(cDiv); break;
          case '%': AddCompiledByte(cMod); break;
        }
        --StackPtr;
    }

    return ind2;
}

// Compiles '+' and '-'
int FunctionParser::CompileAddition(const char* F, int ind)
{
    int ind2 = CompileMult(F, ind);
    sws(F, ind2);
    char op;

    while((op = F[ind2]) == '+' || op == '-')
    {
        ind2 = CompileMult(F, ind2+1);
        sws(F, ind2);
        AddCompiledByte(op=='+' ? cAdd : cSub);
        --StackPtr;
    }

    return ind2;
}

// Compiles '=', '<' and '>'
int FunctionParser::CompileComparison(const char* F, int ind)
{
    int ind2 = CompileAddition(F, ind);
    sws(F, ind2);
    char op;

    while((op = F[ind2]) == '=' || op == '<' || op == '>' || op == '!')
    {
        int opSize = (F[ind2+1] == '=' ? 2 : 1);
        ind2 = CompileAddition(F, ind2+opSize);
        sws(F, ind2);
        switch(op)
        {
          case '=':
              AddCompiledByte(cEqual); break;
          case '<':
              AddCompiledByte(opSize == 1 ? cLess : cLessOrEq); break;
          case '>':
              AddCompiledByte(opSize == 1 ? cGreater : cGreaterOrEq); break;
          case '!':
              AddCompiledByte(cNEqual); break;
        }
        --StackPtr;
    }

    return ind2;
}

// Compiles '&'
int FunctionParser::CompileAnd(const char* F, int ind)
{
    int ind2 = CompileComparison(F, ind);
    sws(F, ind2);

    while(F[ind2] == '&')
    {
        ind2 = CompileComparison(F, ind2+1);
        sws(F, ind2);
        AddCompiledByte(cAnd);
        --StackPtr;
    }

    return ind2;
}

// Compiles '|'
int FunctionParser::CompileOr(const char* F, int ind)
{
    int ind2 = CompileAnd(F, ind);
    sws(F, ind2);

    while(F[ind2] == '|')
    {
        ind2 = CompileAnd(F, ind2+1);
        sws(F, ind2);
        AddCompiledByte(cOr);
        --StackPtr;
    }

    return ind2;
}

// Compiles ','
int FunctionParser::CompileExpression(const char* F, int ind, bool stopAtComma)
{
    int ind2 = CompileOr(F, ind);
    sws(F, ind2);

    if(stopAtComma) return ind2;

    while(F[ind2] == ',')
    {
        ind2 = CompileOr(F, ind2+1);
        sws(F, ind2);
    }

    return ind2;
}


// Return parse error message
// --------------------------
const char* FunctionParser::ErrorMsg() const
{
    if(parseErrorType != FP_NO_ERROR) return ParseErrorMessage[parseErrorType];
    return 0;
}

//---------------------------------------------------------------------------
// Function evaluation
//---------------------------------------------------------------------------
//===========================================================================
namespace
{
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

double FunctionParser::Eval(const double* Vars)
{
    const unsigned* const ByteCode = data->ByteCode;
    const double* const Immed = data->Immed;
    double* const Stack = data->Stack;
    const unsigned ByteCodeSize = data->ByteCodeSize;
    unsigned IP, DP=0;
    int SP=-1;

    for(IP=0; IP<ByteCodeSize; ++IP)
    {
        switch(ByteCode[IP])
        {
// Functions:
          case   cAbs: Stack[SP] = fabs(Stack[SP]); break;
          case  cAcos: if(Stack[SP] < -1 || Stack[SP] > 1)
                       { evalErrorType=4; return 0; }
                       Stack[SP] = acos(Stack[SP]); break;
#ifndef NO_ASINH
          case cAcosh: Stack[SP] = acosh(Stack[SP]); break;
#endif
          case  cAsin: if(Stack[SP] < -1 || Stack[SP] > 1)
                       { evalErrorType=4; return 0; }
                       Stack[SP] = asin(Stack[SP]); break;
#ifndef NO_ASINH
          case cAsinh: Stack[SP] = asinh(Stack[SP]); break;
#endif
          case  cAtan: Stack[SP] = atan(Stack[SP]); break;
          case cAtan2: Stack[SP-1] = atan2(Stack[SP-1], Stack[SP]);
                       --SP; break;
#ifndef NO_ASINH
          case cAtanh: Stack[SP] = atanh(Stack[SP]); break;
#endif
          case  cCeil: Stack[SP] = ceil(Stack[SP]); break;
          case   cCos: Stack[SP] = cos(Stack[SP]); break;
          case  cCosh: Stack[SP] = cosh(Stack[SP]); break;

          case   cCot:
              {
                  double t = tan(Stack[SP]);
                  if(t == 0) { evalErrorType=1; return 0; }
                  Stack[SP] = 1/t; break;
              }
          case   cCsc:
              {
                  double s = sin(Stack[SP]);
                  if(s == 0) { evalErrorType=1; return 0; }
                  Stack[SP] = 1/s; break;
              }


#ifndef DISABLE_EVAL
          case  cEval:
              {
                  double retVal = 0;
                  if(evalRecursionLevel == EVAL_MAX_REC_LEVEL)
                  {
                      evalErrorType = 5;
                  }
                  else
                  {
                      data->Stack = new double[data->StackSize];
                      ++evalRecursionLevel;
                      retVal = Eval(&Stack[SP-data->varAmount+1]);
                      --evalRecursionLevel;
                      delete[] data->Stack;
                      data->Stack = Stack;
                  }
                  SP -= data->varAmount-1;
                  Stack[SP] = retVal;
                  break;
              }
#endif

          case   cExp: Stack[SP] = exp(Stack[SP]); break;
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
          case   cLog: if(Stack[SP] <= 0) { evalErrorType=3; return 0; }
                       Stack[SP] = log(Stack[SP]); break;
          case cLog10: if(Stack[SP] <= 0) { evalErrorType=3; return 0; }
                       Stack[SP] = log10(Stack[SP]); break;
          case   cMax: Stack[SP-1] = Max(Stack[SP-1], Stack[SP]);
                       --SP; break;
          case   cMin: Stack[SP-1] = Min(Stack[SP-1], Stack[SP]);
                       --SP; break;
          case   cSec:
              {
                  double c = cos(Stack[SP]);
                  if(c == 0) { evalErrorType=1; return 0; }
                  Stack[SP] = 1/c; break;
              }
          case   cSin: Stack[SP] = sin(Stack[SP]); break;
          case  cSinh: Stack[SP] = sinh(Stack[SP]); break;
          case  cSqrt: if(Stack[SP] < 0) { evalErrorType=2; return 0; }
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
          case   cDiv: if(Stack[SP] == 0) { evalErrorType=1; return 0; }
                       Stack[SP-1] /= Stack[SP]; --SP; break;
          case   cMod: if(Stack[SP] == 0) { evalErrorType=1; return 0; }
                       Stack[SP-1] = fmod(Stack[SP-1], Stack[SP]);
                       --SP; break;
          case   cPow: Stack[SP-1] = pow(Stack[SP-1], Stack[SP]);
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

          case   cAnd: Stack[SP-1] =
                           (doubleToInt(Stack[SP-1]) &&
                            doubleToInt(Stack[SP]));
                       --SP; break;
          case    cOr: Stack[SP-1] =
                           (doubleToInt(Stack[SP-1]) ||
                            doubleToInt(Stack[SP]));
                       --SP; break;
          case   cNot: Stack[SP] = !doubleToInt(Stack[SP]); break;

// Degrees-radians conversion:
          case   cDeg: Stack[SP] = RadiansToDegrees(Stack[SP]); break;
          case   cRad: Stack[SP] = DegreesToRadians(Stack[SP]); break;

// User-defined function calls:
          case cFCall:
              {
                  unsigned index = ByteCode[++IP];
                  unsigned params = data->FuncPtrs[index].params;
                  double retVal =
                      data->FuncPtrs[index].ptr(&Stack[SP-params+1]);
                  SP -= int(params)-1;
                  Stack[SP] = retVal;
                  break;
              }

          case cPCall:
              {
                  unsigned index = ByteCode[++IP];
                  unsigned params = data->FuncParsers[index]->data->varAmount;
                  double retVal =
                      data->FuncParsers[index]->Eval(&Stack[SP-params+1]);
                  SP -= int(params)-1;
                  Stack[SP] = retVal;
                  break;
              }


#ifdef SUPPORT_OPTIMIZER
          case   cVar: break; // Paranoia. These should never exist
          case   cDup: Stack[SP+1] = Stack[SP]; ++SP; break;
          case   cInv:
              if(Stack[SP] == 0.0) { evalErrorType=1; return 0; }
              Stack[SP] = 1.0/Stack[SP];
              break;
#endif

// Variables:
          default:
              Stack[++SP] = Vars[ByteCode[IP]-VarBegin];
        }
    }

    evalErrorType=0;
    return Stack[SP];
}


#ifdef FUNCTIONPARSER_SUPPORT_DEBUG_OUTPUT
namespace
{
    inline void printHex(std::ostream& dest, unsigned n)
    {
        dest.width(8); dest.fill('0'); hex(dest); //uppercase(dest);
        dest << n;
    }
}

void FunctionParser::PrintByteCode(std::ostream& dest) const
{
    const unsigned* const ByteCode = data->ByteCode;
    const double* const Immed = data->Immed;

    for(unsigned IP=0, DP=0; IP<data->ByteCodeSize; ++IP)
    {
        printHex(dest, IP);
        dest << ": ";

        unsigned opcode = ByteCode[IP];

        switch(opcode)
        {
          case cIf:
              dest << "jz\t";
              printHex(dest, ByteCode[IP+1]+1);
              dest << endl;
              IP += 2;
              break;

          case cJump:
              dest << "jump\t";
              printHex(dest, ByteCode[IP+1]+1);
              dest << endl;
              IP += 2;
              break;
          case cImmed:
              dest.precision(10);
              dest << "push\t" << Immed[DP++] << endl;
              break;

          case cFCall:
              {
                  unsigned index = ByteCode[++IP];
                  Data::VarMap_t::const_iterator iter =
                      data->FuncPtrNames.begin();
                  while(iter->second != index) ++iter;
                  dest << "fcall\t" << iter->first
                       << " (" << data->FuncPtrs[index].params << ")" << endl;
                  break;
              }

          case cPCall:
              {
                  unsigned index = ByteCode[++IP];
                  Data::VarMap_t::const_iterator iter =
                      data->FuncParserNames.begin();
                  while(iter->second != index) ++iter;
                  dest << "pcall\t" << iter->first
                       << " (" << data->FuncParsers[index]->data->varAmount
                       << ")" << endl;
                  break;
              }

          default:
              if(opcode < VarBegin)
              {
                  string n;
                  unsigned params = 1;
                  switch(opcode)
                  {
                    case cNeg: n = "neg"; break;
                    case cAdd: n = "add"; break;
                    case cSub: n = "sub"; break;
                    case cMul: n = "mul"; break;
                    case cDiv: n = "div"; break;
                    case cMod: n = "mod"; break;
                    case cPow: n = "pow"; break;
                    case cEqual: n = "eq"; break;
                    case cNEqual: n = "neq"; break;
                    case cLess: n = "lt"; break;
                    case cLessOrEq: n = "le"; break;
                    case cGreater: n = "gt"; break;
                    case cGreaterOrEq: n = "ge"; break;
                    case cAnd: n = "and"; break;
                    case cOr: n = "or"; break;
                    case cNot: n = "not"; break;
                    case cDeg: n = "deg"; break;
                    case cRad: n = "rad"; break;

#ifndef DISABLE_EVAL
                    case cEval: n = "call\t0"; break;
#endif

#ifdef SUPPORT_OPTIMIZER
                    case cVar: n = "(var)"; break;
                    case cDup: n = "dup"; break;
                    case cInv: n = "inv"; break;
#endif

                    default:
                        n = Functions[opcode-cAbs].name;
                        params = Functions[opcode-cAbs].params;
                  }
                  dest << n;
                  if(params != 1) dest << " (" << params << ")";
                  dest << endl;
              }
              else
              {
                  dest << "push\tVar" << opcode-VarBegin << endl;
              }
        }
    }
}
#endif




#ifndef SUPPORT_OPTIMIZER
void FunctionParser::MakeTree(void *) const {}
void FunctionParser::Optimize()
{
    // Do nothing if no optimizations are supported.
}
#endif

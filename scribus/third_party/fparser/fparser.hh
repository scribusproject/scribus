/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Copyright: Juha Nieminen                                                *|
\***************************************************************************/

#ifndef ONCE_FPARSER_H_
#define ONCE_FPARSER_H_

#include <string>
#include <vector>

#ifdef FUNCTIONPARSER_SUPPORT_DEBUG_OUTPUT
#include <iostream>
#endif

namespace FPoptimizer_CodeTree { class CodeTree; }

class FunctionParser
{
public:
    enum ParseErrorType
    {
        SYNTAX_ERROR=0, MISM_PARENTH, MISSING_PARENTH, EMPTY_PARENTH,
        EXPECT_OPERATOR, OUT_OF_MEMORY, UNEXPECTED_ERROR, INVALID_VARS,
        ILL_PARAMS_AMOUNT, PREMATURE_EOS, EXPECT_PARENTH_FUNC,
        NO_FUNCTION_PARSED_YET,
        FP_NO_ERROR
    };


    int Parse(const char* Function, const std::string& Vars,
              bool useDegrees = false);
    int Parse(const std::string& Function, const std::string& Vars,
              bool useDegrees = false);

    void setDelimiterChar(char);

    const char* ErrorMsg() const;
    inline ParseErrorType GetParseErrorType() const { return parseErrorType; }

    double Eval(const double* Vars);
    inline int EvalError() const { return evalErrorType; }

    bool AddConstant(const std::string& name, double value);
    bool AddUnit(const std::string& name, double value);

    typedef double (*FunctionPtr)(const double*);

    bool AddFunction(const std::string& name,
                     FunctionPtr, unsigned paramsAmount);
    bool AddFunction(const std::string& name, FunctionParser&);

    bool RemoveIdentifier(const std::string& name);

    void Optimize();


    int ParseAndDeduceVariables(const std::string& function,
                                int* amountOfVariablesFound = 0,
                                bool useDegrees = false);
    int ParseAndDeduceVariables(const std::string& function,
                                std::string& resultVarString,
                                int* amountOfVariablesFound = 0,
                                bool useDegrees = false);
    int ParseAndDeduceVariables(const std::string& function,
                                std::vector<std::string>& resultVars,
                                bool useDegrees = false);


    FunctionParser();
    ~FunctionParser();

    // Copy constructor and assignment operator (implemented using the
    // copy-on-write technique for efficiency):
    FunctionParser(const FunctionParser&);
    FunctionParser& operator=(const FunctionParser&);


    void ForceDeepCopy();


#ifdef FUNCTIONPARSER_SUPPORT_DEBUG_OUTPUT
    // For debugging purposes only:
    void PrintByteCode(std::ostream& dest, bool showExpression = true) const;
#endif



//========================================================================
private:
//========================================================================

// Private data:
// ------------
    char delimiterChar;
    ParseErrorType parseErrorType;
    int evalErrorType;

    friend class FPoptimizer_CodeTree::CodeTree;

    struct Data;
    Data* data;

    bool useDegreeConversion;
    unsigned evalRecursionLevel;
    unsigned StackPtr;
    const char* errorLocation;


// Private methods:
// ---------------
    void CopyOnWrite();
    bool CheckRecursiveLinking(const FunctionParser*) const;
    bool NameExists(const char*, unsigned);
    bool ParseVariables(const std::string&);
    int ParseFunction(const char*, bool);
    const char* SetErrorType(ParseErrorType, const char*);

    void AddFunctionOpcode_CheckDegreesConversion(unsigned);
    void AddFunctionOpcode(unsigned);
    inline void AddMultiplicationByConst(double value);
    template<typename Operation>
    inline void AddBinaryOperationByConst();
    inline void incStackPtr();
    bool CompilePowi(int);

    const char* CompileIf(const char*);
    const char* CompileFunctionParams(const char*, unsigned);
    const char* CompileElement(const char*);
    const char* CompilePossibleUnit(const char*);
    const char* CompilePow(const char*);
    const char* CompileUnaryMinus(const char*);
    const char* CompileMult(const char*);
    const char* CompileAddition(const char*);
    const char* CompileComparison(const char*);
    const char* CompileAnd(const char*);
    const char* CompileExpression(const char*);
};

#endif

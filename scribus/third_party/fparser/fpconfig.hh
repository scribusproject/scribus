/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Copyright: Juha Nieminen                                                *|
\***************************************************************************/

// Configuration file
// ------------------

// NOTE:
// This file is for the internal use of the function parser only.
// You don't need to include this file in your source files, just
// include "fparser.hh".

/*
 Note that these do not change what FunctionParser supports, they only
 change how the function is evaluated, potentially making it faster when
 these functions are involved.
 These will make the source code use asinh(), acosh(), atanh(), exp2()
 and log2().
*/
//#define FP_SUPPORT_TR1_MATH_FUNCS

#ifdef FP_SUPPORT_TR1_MATH_FUNCS
#define FP_SUPPORT_ASINH
#define FP_SUPPORT_EXP2
#define FP_SUPPORT_LOG2
#endif

/*
 Comment out the following line to enable the eval() function, which can
 be used in the function string to recursively call the same function.
 Note that enabling this function may be dangerous even if the maximum
 recursion level is limited because it is still possible to write functions
 using it which take enormous  amounts of time to evaluate even though the
 maximum recursion is never reached. This may be undesirable in some
 applications.
 Alternatively you can define the FP_ENABLE_EVAL precompiler constant in
 your compiler settings.
*/
#ifndef FP_ENABLE_EVAL
#define FP_DISABLE_EVAL
#endif


/*
 Maximum recursion level for eval() calls:
*/
#define FP_EVAL_MAX_REC_LEVEL 1000


/*
 Comment out the following lines out if you are not going to use the
 optimizer and want a slightly smaller library. The Optimize() method
 can still be called, but it will not do anything.
 If you are unsure, just leave it. It won't slow down the other parts of
 the library.
*/
#ifndef FP_NO_SUPPORT_OPTIMIZER
#define FP_SUPPORT_OPTIMIZER
#endif


/*
 Epsilon value used with the comparison operators (must be non-negative):
 (Comment it out if you don't want to use epsilon in comparisons. Might
 lead to marginally faster evaluation of the comparison operators, but
 can introduce inaccuracies in comparisons.)
*/
#define FP_EPSILON 1e-14


/*
 No member function of FunctionParser is thread-safe. Most prominently,
 Eval() is not thread-safe. By uncommenting one of these lines the Eval()
 function can be made thread-safe at the cost of a possible small overhead.
 The second version requires that the compiler supports the alloca() function,
 which is not standard, but is faster.
 */
//#define FP_USE_THREAD_SAFE_EVAL
//#define FP_USE_THREAD_SAFE_EVAL_WITH_ALLOCA

/*
 Uncomment (or define in your compiler options) to disable evaluation checks.
 (Consult the documentation for details.)
 */
//#define FP_NO_EVALUATION_CHECKS

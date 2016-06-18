#line 1 "fpoptimizer/fpoptimizer_header.txt"
/***************************************************************************\
|* Function Parser for C++ v3.3.2                                          *|
|*-------------------------------------------------------------------------*|
|* Function optimizer                                                      *|
|*-------------------------------------------------------------------------*|
|* Copyright: Joel Yliluoma                                                *|
\***************************************************************************/

/* NOTE:
   This is a concatenation of all the header and source files of the
   original optimizer source code. All the code has been concatenated
   into this single file for convenience of usage (in other words, to
   simply use the optimizer, it's enough to add this file to the project
   rather than a multitude of files which the original optimizer source
   code is composed of).

   Thus this file exists for the usage of the Function parser library
   only, and is not suitable for developing it further. If you want to
   develop the library further, you should download the development
   version of the library, which has all the original source files.
 */

#include "fpconfig.hh"
#ifdef FP_SUPPORT_OPTIMIZER


#line 1 "fpoptimizer/fpoptimizer_hash.hh"
#ifndef FPoptimizerHashHH
#define FPoptimizerHashHH

#ifdef _MSC_VER

typedef unsigned long long fphash_value_t;
#define FPHASH_CONST(x) x##ULL

#else

#include <stdint.h>
typedef uint_fast64_t fphash_value_t;
#define FPHASH_CONST(x) x##ULL

#endif

namespace FUNCTIONPARSERTYPES
{
    struct fphash_t
    {
        fphash_value_t hash1, hash2;

        bool operator==(const fphash_t& rhs) const
        { return hash1 == rhs.hash1 && hash2 == rhs.hash2; }

        bool operator!=(const fphash_t& rhs) const
        { return hash1 != rhs.hash1 || hash2 != rhs.hash2; }

        bool operator<(const fphash_t& rhs) const
        { return hash1 != rhs.hash1 ? hash1 < rhs.hash1 : hash2 < rhs.hash2; }
    };
}

#endif

#line 1 "fpoptimizer/fpoptimizer_autoptr.hh"
#ifndef FPOptimizerAutoPtrHH
#define FPOptimizerAutoPtrHH

template<typename Ref>
class FPOPT_autoptr
{
public:
    FPOPT_autoptr()                   : p(0)   { }
    FPOPT_autoptr(Ref*        b) : p(b)   { Birth(); }
    FPOPT_autoptr(const FPOPT_autoptr& b) : p(b.p) { Birth(); }

    inline Ref& operator* () const { return *p; }
    inline Ref* operator->() const { return p; }
    inline bool operator! () const { return p; }

    FPOPT_autoptr& operator= (Ref*        b) { Set(b); return *this; }
    FPOPT_autoptr& operator= (const FPOPT_autoptr& b) { Set(b.p); return *this; }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    FPOPT_autoptr(FPOPT_autoptr&& b)      : p(b.p) { b.p = 0; }
    FPOPT_autoptr& operator= (FPOPT_autoptr&& b) { if(p != b.p) { Forget(); p=b.p; b.p=0; }
                                                   return *this; }
#endif

    ~FPOPT_autoptr() { Forget(); }

    void UnsafeSetP(Ref* newp) { p = newp; }
    void swap(FPOPT_autoptr<Ref>& b) { Ref* tmp=p; p=b.p; b.p=tmp; }

private:
    inline static void Have(Ref* p2);
    inline void Forget();
    inline void Birth();
    inline void Set(Ref* p2);
private:
    Ref* p;
};

//
template<typename Ref>
inline void FPOPT_autoptr<Ref>::Forget()
{
    if(!p) return;
    p->RefCount -= 1;
    if(!p->RefCount) delete p;
    //assert(p->RefCount >= 0);
}
template<typename Ref>
inline void FPOPT_autoptr<Ref>::Have(Ref* p2)
{
    if(p2) ++(p2->RefCount);
}
template<typename Ref>
inline void FPOPT_autoptr<Ref>::Birth()
{
    Have(p);
}
template<typename Ref>
inline void FPOPT_autoptr<Ref>::Set(Ref* p2)
{
    Have(p2);
    Forget();
    p = p2;
}

#endif

#line 1 "fpoptimizer/fpoptimizer_codetree.hh"
#ifndef FPOptimizer_CodeTreeHH
#define FPOptimizer_CodeTreeHH

#include "fpconfig.hh"
#include "fparser.hh"
#include "fptypes.hh"

#ifdef FP_SUPPORT_OPTIMIZER

#include <vector>
#include <utility>

// line removed
// line removed

#ifdef FP_EPSILON
 #define NEGATIVE_MAXIMUM (-FP_EPSILON)
#else
 #define NEGATIVE_MAXIMUM (-1e-14)
#endif

namespace FPoptimizer_Grammar
{
    struct Grammar;
}

namespace FPoptimizer_ByteCode
{
    class ByteCodeSynth;
}

namespace FPoptimizer_CodeTree
{
    class CodeTree;

    struct MinMaxTree
    {
        double min,max;
        bool has_min, has_max;
        MinMaxTree() : min(),max(),has_min(false),has_max(false) { }
        MinMaxTree(double mi,double ma): min(mi),max(ma),has_min(true),has_max(true) { }
        MinMaxTree(bool,double ma): min(),max(ma),has_min(false),has_max(true) { }
        MinMaxTree(double mi,bool): min(mi),max(),has_min(true),has_max(false) { }
    };

    struct CodeTreeData;
    class CodeTree
    {
        typedef FPOPT_autoptr<CodeTreeData> DataP;
        DataP data;

    public:
    public:
        CodeTree();
        ~CodeTree();

        explicit CodeTree(double v); // produce an immed
        struct VarTag { };
        explicit CodeTree(unsigned varno, VarTag); // produce a var reference
        struct CloneTag { };
        explicit CodeTree(const CodeTree& b, CloneTag);

        /* Generates a CodeTree from the given bytecode */
        void GenerateFrom(
            const std::vector<unsigned>& byteCode,
            const std::vector<double>& immed,
            const FunctionParser::Data& data);

        void SynthesizeByteCode(
            std::vector<unsigned>& byteCode,
            std::vector<double>&   immed,
            size_t& stacktop_max);
        void SynthesizeByteCode(FPoptimizer_ByteCode::ByteCodeSynth& synth) const;

        void SetParams(const std::vector<CodeTree>& RefParams);
        void SetParamsMove(std::vector<CodeTree>& RefParams);

        CodeTree GetUniqueRef();
        // ^use this when CodeTree tmp=x; tmp.CopyOnWrite(); does not do exactly what you want

#ifdef __GXX_EXPERIMENTAL_CXX0X__
        void SetParams(std::vector<CodeTree>&& RefParams);
#endif
        void SetParam(size_t which, const CodeTree& b);
        void SetParamMove(size_t which, CodeTree& b);
        void AddParam(const CodeTree& param);
        void AddParamMove(CodeTree& param);
        void AddParams(const std::vector<CodeTree>& RefParams);
        void AddParamsMove(std::vector<CodeTree>& RefParams);
        void AddParamsMove(std::vector<CodeTree>& RefParams, size_t replacing_slot);
        void DelParam(size_t index);
        void DelParams();

        void Become(const CodeTree& b);

        inline size_t GetParamCount() const { return GetParams().size(); }
        inline CodeTree& GetParam(size_t n) { return GetParams()[n]; }
        inline const CodeTree& GetParam(size_t n) const { return GetParams()[n]; }
        inline void SetOpcode(FUNCTIONPARSERTYPES::OPCODE o);
        inline void SetFuncOpcode(FUNCTIONPARSERTYPES::OPCODE o, unsigned f);
        inline void SetVar(unsigned v);
        inline void SetImmed(double v);
        inline FUNCTIONPARSERTYPES::OPCODE GetOpcode() const;
        inline FUNCTIONPARSERTYPES::fphash_t GetHash() const;
        inline const std::vector<CodeTree>& GetParams() const;
        inline std::vector<CodeTree>& GetParams();
        inline size_t GetDepth() const;
        inline double GetImmed() const;
        inline unsigned GetVar() const;
        inline unsigned GetFuncNo() const;
	inline bool IsDefined() const { return !data == false; }

        inline bool    IsImmed() const { return GetOpcode() == FUNCTIONPARSERTYPES::cImmed; }
        inline bool      IsVar() const { return GetOpcode() == FUNCTIONPARSERTYPES::cVar; }
        bool    IsLongIntegerImmed() const { return IsImmed() && GetImmed() == (double)GetLongIntegerImmed(); }
        long   GetLongIntegerImmed() const { return (long)GetImmed(); }
        bool    IsLogicalValue() const;
        inline unsigned GetRefCount() const;
        /* This function calculates the minimum and maximum values
         * of the tree's result. If an estimate cannot be made,
         * has_min/has_max are indicated as false.
         */
        MinMaxTree CalculateResultBoundaries_do() const;
        MinMaxTree CalculateResultBoundaries() const;

        enum TriTruthValue { IsAlways, IsNever, Unknown };
        TriTruthValue GetEvennessInfo() const;

        bool IsAlwaysSigned(bool positive) const;
        bool IsAlwaysParity(bool odd) const
            { return GetEvennessInfo() == (odd?IsNever:IsAlways); }
        bool IsAlwaysInteger() const;

        void ConstantFolding();
        void ConstantFolding_FromLogicalParent();
        bool ConstantFolding_AndLogic();
        bool ConstantFolding_OrLogic();
        bool ConstantFolding_LogicCommon(bool is_or);
        bool ConstantFolding_MulGrouping();
        bool ConstantFolding_AddGrouping();
        bool ConstantFolding_Assimilate();

        void Rehash(bool constantfolding = true);
        inline void Mark_Incompletely_Hashed();
        inline bool Is_Incompletely_Hashed() const;

        inline const FPoptimizer_Grammar::Grammar* GetOptimizedUsing() const;
        inline void SetOptimizedUsing(const FPoptimizer_Grammar::Grammar* g);

        void swap(CodeTree& b) { data.swap(b.data); }
        bool IsIdenticalTo(const CodeTree& b) const;
        void CopyOnWrite();
    };

    struct CodeTreeData
    {
        int RefCount;

        /* Describing the codetree node */
        FUNCTIONPARSERTYPES::OPCODE Opcode;
        union
        {
            double   Value;   // In case of cImmed: value of the immed
            unsigned Var;     // In case of cVar:   variable number
            unsigned Funcno;  // In case of cFCall or cPCall
        };

        // Parameters for the function
        //  These use the sign:
        //   For cAdd: operands to add together (0 to n)
        //             sign indicates that the value is negated before adding (0-x)
        //   For cMul: operands to multiply together (0 to n)
        //             sign indicates that the value is inverted before multiplying (1/x)
        //   For cAnd: operands to bitwise-and together (0 to n)
        //             sign indicates that the value is inverted before anding (!x)
        //   For cOr:  operands to bitwise-or together (0 to n)
        //             sign indicates that the value is inverted before orring (!x)
        //  These don't use the sign (sign is always false):
        //   For cMin: operands to select the minimum of
        //   For cMax: operands to select the maximum of
        //   For cImmed, not used
        //   For cVar,   not used
        //   For cIf:  operand 1 = condition, operand 2 = yes-branch, operand 3 = no-branch
        //   For anything else: the parameters required by the operation/function
        std::vector<CodeTree> Params;

        /* Internal operation */
        FUNCTIONPARSERTYPES::fphash_t      Hash;
        size_t        Depth;
        const FPoptimizer_Grammar::Grammar* OptimizedUsing;

        CodeTreeData();
        CodeTreeData(const CodeTreeData& b);
        explicit CodeTreeData(double i);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
        CodeTreeData(CodeTreeData&& b);
#endif

        bool IsIdenticalTo(const CodeTreeData& b) const;
        void Sort();
        void Recalculate_Hash_NoRecursion();
    };

    inline void CodeTree::SetOpcode(FUNCTIONPARSERTYPES::OPCODE o)
        { data->Opcode = o; }
    inline void CodeTree::SetFuncOpcode(FUNCTIONPARSERTYPES::OPCODE o, unsigned f)
        { SetOpcode(o); data->Funcno = f; }
    inline void CodeTree::SetVar(unsigned v)
        { SetOpcode(FUNCTIONPARSERTYPES::cVar); data->Var = v; }
    inline void CodeTree::SetImmed(double v)
        { SetOpcode(FUNCTIONPARSERTYPES::cImmed); data->Value = v; }
    inline FUNCTIONPARSERTYPES::OPCODE CodeTree::GetOpcode() const { return data->Opcode; }
    inline FUNCTIONPARSERTYPES::fphash_t CodeTree::GetHash() const { return data->Hash; }
    inline const std::vector<CodeTree>& CodeTree::GetParams() const { return data->Params; }
    inline std::vector<CodeTree>& CodeTree::GetParams() { return data->Params; }
    inline size_t CodeTree::GetDepth() const { return data->Depth; }
    inline double CodeTree::GetImmed() const { return data->Value; }
    inline unsigned CodeTree::GetVar() const { return data->Var; }
    inline unsigned CodeTree::GetFuncNo() const { return data->Funcno; }

    inline const FPoptimizer_Grammar::Grammar* CodeTree::GetOptimizedUsing() const
        { return data->OptimizedUsing; }
    inline void CodeTree::SetOptimizedUsing(const FPoptimizer_Grammar::Grammar* g)
        { data->OptimizedUsing = g; }
    inline unsigned CodeTree::GetRefCount() const { return data->RefCount; }

    inline void CodeTree::Mark_Incompletely_Hashed() { data->Depth = 0; }
    inline bool CodeTree::Is_Incompletely_Hashed() const { return data->Depth == 0; }

    void FixIncompleteHashes(CodeTree& tree);
}

#endif

#endif

#line 1 "fpoptimizer/fpoptimizer_grammar.hh"
#ifndef FPOPT_NAN_CONST

#include <iostream>

#include "fpconfig.hh"
#include "fparser.hh"
#include "fptypes.hh"

#define FPOPT_NAN_CONST (-1712345.25) /* Would use 0.0 / 0.0 here, but some compilers don't accept it. */

namespace FPoptimizer_CodeTree
{
    class CodeTree;
}

namespace FPoptimizer_Grammar
{
    enum ImmedConstraint_Value
    {
        ValueMask = 0x07,
        Value_AnyNum     = 0x0, // any value
        Value_EvenInt    = 0x1, // any even integer (0,2,4, etc)
        Value_OddInt     = 0x2, // any odd integer (1,3, etc)
        Value_IsInteger  = 0x3, // any integer-value (excludes e.g. 0.2)
        Value_NonInteger = 0x4, // any non-integer (excludes e.g. 1 or 5)
        Value_Logical    = 0x5  // a result of cNot,cNotNot,cAnd,cOr or comparators
    };
    enum ImmedConstraint_Sign
    {
        SignMask  = 0x18,
        Sign_AnySign     = 0x00, // - or +
        Sign_Positive    = 0x08, // positive only
        Sign_Negative    = 0x10, // negative only
        Sign_NoIdea      = 0x18  // where sign cannot be guessed
    };
    enum ImmedConstraint_Oneness
    {
        OnenessMask   = 0x60,
        Oneness_Any      = 0x00,
        Oneness_One      = 0x20, // +1 or -1
        Oneness_NotOne   = 0x40  // anything but +1 or -1
    };
    enum ImmedConstraint_Constness
    {
        ConstnessMask = 0x80,
        Constness_Any    = 0x00,
        Constness_Const  = 0x80
    };

    /* The param_opcode field of the ParamSpec has the following
     * possible values (from enum SpecialOpcode):
     *   NumConstant:
     *      this describes a specific constant value (constvalue)
     *      that must be matched / synthesized.
     *   ParamHolder:
     *      this describes any node
     *      that must be matched / synthesized.
     *      "index" is the ID of the NamedHolder:
     *      In matching, all NamedHolders having the same ID
     *      must match the identical node.
     *      In synthesizing, the node matched by
     *      a NamedHolder with this ID must be synthesized.
     *    SubFunction:
     *      this describes a subtree
     *      that must be matched / synthesized.
     *      The subtree is described in subfunc_opcode,param_begin..+param_count.
     *      If the type is GroupFunction, the tree is expected
     *      to yield a constant value which is tested.
     */
    enum SpecialOpcode
    {
        NumConstant,        // Holds a particular value (syntax-time constant)
        ParamHolder,        // Holds a particular named param
        SubFunction         // Holds an opcode and the params
    };

    enum ParamMatchingType
    {
        PositionalParams, // this set of params in this order
        SelectedParams,   // this set of params in any order
        AnyParams,        // these params are included
        GroupFunction     // this function represents a constant value
    };

    enum RuleType
    {
        ProduceNewTree, // replace self with the first (and only) from replaced_param
        ReplaceParams   // replace indicate params with replaced_params
    };

#ifdef __GNUC__
# define PACKED_GRAMMAR_ATTRIBUTE __attribute__((packed))
#else
# define PACKED_GRAMMAR_ATTRIBUTE
#endif

    enum { PARAM_INDEX_BITS = 9 };

    /* A ParamSpec object describes
     * either a parameter (leaf, node) that must be matched,
     * or a parameter (leaf, node) that must be synthesized.
     */
    typedef std::pair<SpecialOpcode, const void*> ParamSpec;
    ParamSpec ParamSpec_Extract(unsigned paramlist, unsigned index);
    bool ParamSpec_Compare(const void* a, const void* b, SpecialOpcode type);
    unsigned ParamSpec_GetDepCode(const ParamSpec& b);

    struct ParamSpec_ParamHolder
    {
        unsigned index       : 8; // holder ID
        unsigned constraints : 8; // constraints
        unsigned depcode     :16;
    } PACKED_GRAMMAR_ATTRIBUTE;

    struct ParamSpec_NumConstant
    {
        double constvalue;        // the value
    } PACKED_GRAMMAR_ATTRIBUTE;

    struct ParamSpec_SubFunctionData
    {
        /* Expected parameters (leaves) of the tree: */
        unsigned param_count         : 2;
        unsigned param_list          : 30;
        /* The opcode that the tree must have when SubFunction */
        FUNCTIONPARSERTYPES::OPCODE subfunc_opcode : 8;

        /* When matching, type describes the method of matching.
         *
         *               Sample input tree:      (cOr 2 3)  (cOr 2 4) (cOr 3 2) (cOr 4 2 3) (cOr 2)
         * Possible methods:
         *    PositionalParams, e.g. (cOr [2 3]):  match     no match  no match  no match   no match
         *      The nodes described here are
         *      to be matched, in this order.
         *    SelectedParams,   e.g. (cOr {2 3}):  match     no match   match    no match   no match
         *      The nodes described here are
         *      to be matched, in any order.
         *    AnyParams,        e.g. (cOr 2 3  ):  match     no match   match     match     no match
         *      At least the nodes described here
         *      are to be matched, in any order.
         * When synthesizing, the type is ignored.
         */
        ParamMatchingType match_type : 3; /* When SubFunction */
        /* Note: match_type needs 2, but we specify 3 because
         * otherwise Microsoft VC++ borks things up
         * as it interprets the value as signed.
         */
        /* Optional restholder index for capturing the rest of parameters (0=not used)
         * Only valid when match_type = AnyParams
         */
        unsigned restholder_index : 5;
    } PACKED_GRAMMAR_ATTRIBUTE; // size: 2+30+6+2+8=48 bits=6 bytes

    struct ParamSpec_SubFunction
    {
        ParamSpec_SubFunctionData data;
        unsigned constraints : 8; // constraints
        unsigned depcode     : 8;
    } PACKED_GRAMMAR_ATTRIBUTE; // 8 bytes

    /* Theoretical minimal sizes in each param_opcode cases:
     * Assume param_opcode needs 3 bits.
     *    NumConstant:   3 + 64              (or 3+4 if just use index to clist[])
     *    ParamHolder:   3 + 7 + 2           (7 for constraints, 2 for immed index)
     *    SubFunction:   3 + 7 + 2 + 2 + 3*9 = 41
     */

    /* A rule describes a pattern for matching
     * and the method how to reconstruct the
     * matched node(s) in the tree.
     */
    struct Rule
    {
        /* For optimization: Number of minimum parameters (leaves)
         * that the source tree must contain for this rule to match.
         */
        unsigned  n_minimum_params : 2;

        /* If the rule matched, this field describes how to perform
         * the replacement.
         *   When type==ProduceNewTree,
         *       the source tree is replaced entirely with
         *       the new tree described at repl_param_begin[0].
         *   When type==ReplaceParams,
         *       the matching leaves in the source tree are removed
         *       and new leaves are constructedfrom the trees
         *       described at repl_param_begin[0..repl_param_count].
         *       Other leaves remain intact.
         */
        RuleType  ruletype         : 1;

        /* The replacement parameters (if NewTree, begin[0] represents the new tree) */
        unsigned  repl_param_count : 2; /* Assumed to be 1 when type == ProduceNewTree */
        unsigned  repl_param_list  : 27;

        /* The function that we must match. Always a SubFunction. */
        ParamSpec_SubFunctionData match_tree;
    } PACKED_GRAMMAR_ATTRIBUTE; // size: 2+1+2+27 + 48 = 80 bits = 10 bytes

    /* Grammar is a set of rules for tree substitutions. */
    struct Grammar
    {
        /* The rules of this grammar */
        const Rule*    rule_begin;
        unsigned       rule_count;
    };

    bool ApplyGrammar(const Grammar& grammar,
                      FPoptimizer_CodeTree::CodeTree& tree,
                      bool recurse = true);

    /* GrammarPack is the structure which contains all
     * the information regarding the optimization rules.
     */
    extern const struct GrammarPack
    {
        Grammar               glist[3];
    } pack;

    void DumpParam(const ParamSpec& p, std::ostream& o = std::cout);
    void DumpParams(unsigned paramlist, unsigned count, std::ostream& o = std::cout);
}

#endif

#line 1 "fpoptimizer/fpoptimizer_consts.hh"
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define CONSTANT_E     2.71828182845904509080  // exp(1)
#define CONSTANT_EI    0.3678794411714423215955 // exp(-1)
#define CONSTANT_2E    7.3890560989306502272304 // exp(2)
#define CONSTANT_2EI   0.135335283236612691894 // exp(-2)
#define CONSTANT_PI    M_PI                    // atan2(0,-1)
#define CONSTANT_L10   2.30258509299404590109  // log(10)
#define CONSTANT_L2    0.69314718055994530943  // log(2)
#define CONSTANT_L10I  0.43429448190325176116  // 1/log(10)
#define CONSTANT_L2I   1.4426950408889634074   // 1/log(2)
#define CONSTANT_L10E  CONSTANT_L10I           // log10(e)
#define CONSTANT_L10EI CONSTANT_L10            // 1/log10(e)
#define CONSTANT_L10B  0.3010299956639811952137 // log10(2)
#define CONSTANT_L10BI 3.3219280948873623478703 // 1/log10(2)
#define CONSTANT_LB10  CONSTANT_L10BI          // log2(10)
#define CONSTANT_LB10I CONSTANT_L10B           // 1/log2(10)
#define CONSTANT_L2E   CONSTANT_L2I            // log2(e)
#define CONSTANT_L2EI  CONSTANT_L2             // 1/log2(e)
#define CONSTANT_DR    (180.0 / M_PI)          // 180/pi
#define CONSTANT_RD    (M_PI / 180.0)          // pi/180

#define CONSTANT_POS_INF     HUGE_VAL  // positive infinity, from math.h
#define CONSTANT_NEG_INF   (-HUGE_VAL) // negative infinity
#define CONSTANT_PIHALF (M_PI / 2)

#line 1 "fpoptimizer/fpoptimizer_optimize.hh"
// line removed
// line removed

#ifdef FP_SUPPORT_OPTIMIZER

#include <map>
#include <vector>
#include <iostream>

//#define DEBUG_SUBSTITUTIONS

namespace FPoptimizer_Optimize
{
    using namespace FPoptimizer_Grammar;
    using namespace FPoptimizer_CodeTree;
    using namespace FUNCTIONPARSERTYPES;

    /* This struct collects information regarding the matching process so far */
    class MatchInfo
    {
    public:
        std::multimap<unsigned, CodeTree> restholder_matches;
        std::map<unsigned, CodeTree> paramholder_matches;
        std::vector<unsigned> matched_params;
    public:
        /* These functions save data from matching */
        void SaveRestHolderMatch(unsigned restholder_index, const CodeTree& treeptr)
        {
            restholder_matches.insert( std::make_pair(restholder_index, treeptr) );
        }

        bool SaveOrTestParamHolder(unsigned paramholder_index, const CodeTree& treeptr)
        {
            std::map<unsigned, CodeTree>::iterator i = paramholder_matches.lower_bound(paramholder_index);
            if(i == paramholder_matches.end() || i->first != paramholder_index)
                { paramholder_matches.insert(i, std::make_pair(paramholder_index, treeptr));
                  return true; }
            return treeptr.IsIdenticalTo(i->second);
        }

        void SaveMatchedParamIndex(unsigned index)
        {
            matched_params.push_back(index);
        }

        /* These functions retrieve the data from matching
         * for use when synthesizing the resulting tree.
         */
        const CodeTree& GetParamHolderValueIfFound( unsigned paramholder_index ) const
        {
            std::map<unsigned, CodeTree>::const_iterator i = paramholder_matches.find(paramholder_index);
            if(i == paramholder_matches.end() || i->first != paramholder_index)
            {
                static const CodeTree dummytree;
                return dummytree;
            }
            return i->second;
        }

        const CodeTree& GetParamHolderValue( unsigned paramholder_index ) const
            { return paramholder_matches.find(paramholder_index)->second; }

        std::vector<CodeTree> GetRestHolderValues( unsigned restholder_index ) const
        {
            std::vector<CodeTree> result;
            for(std::multimap<unsigned, CodeTree>::const_iterator
                i = restholder_matches.lower_bound(restholder_index);
                i != restholder_matches.end() && i->first == restholder_index;
                ++i)
                result.push_back(i->second);
            return result;
        }

        const std::vector<unsigned>& GetMatchedParamIndexes() const
            { return matched_params; }

        /* */
        void swap(MatchInfo& b)
        {
            restholder_matches.swap(b.restholder_matches);
            paramholder_matches.swap(b.paramholder_matches);
            matched_params.swap(b.matched_params);
        }
        MatchInfo& operator=(const MatchInfo& b)
        {
            restholder_matches = b.restholder_matches;
            paramholder_matches = b.paramholder_matches;
            matched_params = b.matched_params;
            return *this;
        }
    };

    class MatchPositionSpecBase;

    // For iterating through match candidates
    typedef FPOPT_autoptr<MatchPositionSpecBase> MatchPositionSpecBaseP;

    class MatchPositionSpecBase
    {
    public:
        int RefCount;
    public:
        MatchPositionSpecBase() : RefCount(0) { }
        virtual ~MatchPositionSpecBase() { }
    };
    struct MatchResultType
    {
        bool found;
        MatchPositionSpecBaseP specs;

        MatchResultType(bool f) : found(f), specs() { }
        MatchResultType(bool f,
                        const MatchPositionSpecBaseP& s) : found(f), specs(s) { }
    };

    /* Synthesize the given grammatic rule's replacement into the codetree */
    void SynthesizeRule(
        const Rule& rule,
        CodeTree& tree,
        MatchInfo& info);

    /* Test the given parameter to a given CodeTree */
    MatchResultType TestParam(
        const ParamSpec& parampair,
        const CodeTree& tree,
        const MatchPositionSpecBaseP& start_at,
        MatchInfo& info);

    /* Test the list of parameters to a given CodeTree */
    MatchResultType TestParams(
        const ParamSpec_SubFunctionData& model_tree,
        const CodeTree& tree,
        const MatchPositionSpecBaseP& start_at,
        MatchInfo& info,
        bool TopLevel);
}

namespace FPoptimizer_Grammar
{
    void DumpTree(const FPoptimizer_CodeTree::CodeTree& tree, std::ostream& o = std::cout);
    void DumpTreeWithIndent(const FPoptimizer_CodeTree::CodeTree& tree, std::ostream& o = std::cout, const std::string& indent = "\\");
    void DumpHashes(const FPoptimizer_CodeTree::CodeTree& tree, std::ostream& o = std::cout);
    void DumpMatch(const Rule& rule,
                   const FPoptimizer_CodeTree::CodeTree& tree,
                   const FPoptimizer_Optimize::MatchInfo& info,
                   bool DidMatch,
                   std::ostream& o = std::cout);
}

#endif

#line 1 "fpoptimizer/crc32.hh"
/* crc32 */

#ifdef _MSC_VER

 typedef unsigned int crc32_t;

#else

 #include <stdint.h>
 typedef uint_least32_t crc32_t;

#endif

namespace crc32
{
    enum { startvalue = 0xFFFFFFFFUL, poly = 0xEDB88320UL };

    /* This code constructs the CRC32 table at compile-time,
     * avoiding the need for a huge explicitly written table of magical numbers. */
    template<crc32_t crc> // One byte of a CRC32 (eight bits):
    struct b8
    {
        enum { b1 = (crc & 1) ? (poly ^ (crc >> 1)) : (crc >> 1),
               b2 = (b1  & 1) ? (poly ^ (b1  >> 1)) : (b1  >> 1),
               b3 = (b2  & 1) ? (poly ^ (b2  >> 1)) : (b2  >> 1),
               b4 = (b3  & 1) ? (poly ^ (b3  >> 1)) : (b3  >> 1),
               b5 = (b4  & 1) ? (poly ^ (b4  >> 1)) : (b4  >> 1),
               b6 = (b5  & 1) ? (poly ^ (b5  >> 1)) : (b5  >> 1),
               b7 = (b6  & 1) ? (poly ^ (b6  >> 1)) : (b6  >> 1),
               res= (b7  & 1) ? (poly ^ (b7  >> 1)) : (b7  >> 1) };
    };
    inline crc32_t update(crc32_t crc, unsigned/* char */b) // __attribute__((pure))
    {
        // Four values of the table
        #define B4(n) b8<n>::res,b8<n+1>::res,b8<n+2>::res,b8<n+3>::res
        // Sixteen values of the table
        #define R(n) B4(n),B4(n+4),B4(n+8),B4(n+12)
        // The whole table, index by steps of 16
        static const crc32_t table[256] =
        { R(0x00),R(0x10),R(0x20),R(0x30), R(0x40),R(0x50),R(0x60),R(0x70),
          R(0x80),R(0x90),R(0xA0),R(0xB0), R(0xC0),R(0xD0),R(0xE0),R(0xF0) };
        #undef R
        #undef B4
        return ((crc >> 8) /* & 0x00FFFFFF*/) ^ table[/*(unsigned char)*/(crc^b)&0xFF];
    }
    inline crc32_t calc_upd(crc32_t c, const unsigned char* buf, size_t size)
    {
        crc32_t value = c;
        for(size_t p=0; p<size; ++p) value = update(value, buf[p]);
        return value;
    }
    inline crc32_t calc(const unsigned char* buf, size_t size)
    {
        return calc_upd(startvalue, buf, size);
    }
}

#line 1 "fpoptimizer/fpoptimizer_opcodename.hh"
// line removed
#include <string>

const std::string FP_GetOpcodeName(FPoptimizer_Grammar::SpecialOpcode opcode, bool pad=false);
const std::string FP_GetOpcodeName(FUNCTIONPARSERTYPES::OPCODE opcode,        bool pad=false);

#line 1 "fpoptimizer/fpoptimizer_opcodename.cc"
#include <string>
#include <sstream>
#include <assert.h>

#include <iostream>

#include "fpconfig.hh"
#include "fptypes.hh"

// line removed
// line removed

using namespace FPoptimizer_Grammar;
using namespace FUNCTIONPARSERTYPES;

const std::string FP_GetOpcodeName(FPoptimizer_Grammar::SpecialOpcode opcode, bool pad)
{
#if 1
    /* Symbolic meanings for the opcodes? */
    const char* p = 0;
    switch( opcode )
    {
        case NumConstant:   p = "NumConstant"; break;
        case ParamHolder:   p = "ParamHolder"; break;
        case SubFunction:   p = "SubFunction"; break;
    }
    std::ostringstream tmp;
    //if(!p) std::cerr << "o=" << opcode << "\n";
    assert(p);
    tmp << p;
    if(pad) while(tmp.str().size() < 12) tmp << ' ';
    return tmp.str();
#else
    /* Just numeric meanings */
    std::ostringstream tmp;
    tmp << opcode;
    if(pad) while(tmp.str().size() < 5) tmp << ' ';
    return tmp.str();
#endif
}

const std::string FP_GetOpcodeName(FUNCTIONPARSERTYPES::OPCODE opcode,        bool pad)
{
#if 1
    /* Symbolic meanings for the opcodes? */
    const char* p = 0;
    switch(opcode)
    {
        case cAbs: p = "cAbs"; break;
        case cAcos: p = "cAcos"; break;
        case cAcosh: p = "cAcosh"; break;
        case cAsin: p = "cAsin"; break;
        case cAsinh: p = "cAsinh"; break;
        case cAtan: p = "cAtan"; break;
        case cAtan2: p = "cAtan2"; break;
        case cAtanh: p = "cAtanh"; break;
        case cCeil: p = "cCeil"; break;
        case cCos: p = "cCos"; break;
        case cCosh: p = "cCosh"; break;
        case cCot: p = "cCot"; break;
        case cCsc: p = "cCsc"; break;
        case cEval: p = "cEval"; break;
        case cExp: p = "cExp"; break;
        case cExp2: p = "cExp2"; break;
        case cFloor: p = "cFloor"; break;
        case cIf: p = "cIf"; break;
        case cInt: p = "cInt"; break;
        case cLog: p = "cLog"; break;
        case cLog2: p = "cLog2"; break;
        case cLog10: p = "cLog10"; break;
        case cMax: p = "cMax"; break;
        case cMin: p = "cMin"; break;
        case cPow: p = "cPow"; break;
        case cSec: p = "cSec"; break;
        case cSin: p = "cSin"; break;
        case cSinh: p = "cSinh"; break;
        case cSqrt: p = "cSqrt"; break;
        case cTan: p = "cTan"; break;
        case cTanh: p = "cTanh"; break;
        case cImmed: p = "cImmed"; break;
        case cJump: p = "cJump"; break;
        case cNeg: p = "cNeg"; break;
        case cAdd: p = "cAdd"; break;
        case cSub: p = "cSub"; break;
        case cMul: p = "cMul"; break;
        case cDiv: p = "cDiv"; break;
        case cMod: p = "cMod"; break;
        case cEqual: p = "cEqual"; break;
        case cNEqual: p = "cNEqual"; break;
        case cLess: p = "cLess"; break;
        case cLessOrEq: p = "cLessOrEq"; break;
        case cGreater: p = "cGreater"; break;
        case cGreaterOrEq: p = "cGreaterOrEq"; break;
        case cNot: p = "cNot"; break;
        case cAnd: p = "cAnd"; break;
        case cOr: p = "cOr"; break;
        case cDeg: p = "cDeg"; break;
        case cRad: p = "cRad"; break;
        case cFCall: p = "cFCall"; break;
        case cPCall: p = "cPCall"; break;
        case cRPow: p = "cRPow"; break;
#ifdef FP_SUPPORT_OPTIMIZER
        case cVar: p = "cVar"; break;
        case cFetch: p = "cFetch"; break;
        case cPopNMov: p = "cPopNMov"; break;
#endif
        case cDup: p = "cDup"; break;
        case cInv: p = "cInv"; break;
        case cSqr: p = "cSqr"; break;
        case cRDiv: p = "cRDiv"; break;
        case cRSub: p = "cRSub"; break;
        case cNotNot: p = "cNotNot"; break;
        case cRSqrt: p = "cRSqrt"; break;
        case cNop: p = "cNop"; break;
        case VarBegin: p = "VarBegin"; break;
    }
    std::ostringstream tmp;
    //if(!p) std::cerr << "o=" << opcode << "\n";
    assert(p);
    tmp << p;
    if(pad) while(tmp.str().size() < 12) tmp << ' ';
    return tmp.str();
#else
    /* Just numeric meanings */
    std::ostringstream tmp;
    tmp << opcode;
    if(pad) while(tmp.str().size() < 5) tmp << ' ';
    return tmp.str();
#endif
}

#line 1 "fpoptimizer/fpoptimizer_bytecodesynth.hh"
#include "fpconfig.hh"
#include "fparser.hh"
#include "fptypes.hh"

#ifdef FP_SUPPORT_OPTIMIZER

#include <vector>
#include <utility>

// line removed

namespace FPoptimizer_ByteCode
{
    class ByteCodeSynth
    {
    public:
        ByteCodeSynth()
            : ByteCode(), Immed(), StackTop(0), StackMax(0)
        {
            /* estimate the initial requirements as such */
            ByteCode.reserve(64);
            Immed.reserve(8);
            StackHash.reserve(16);
        }

        void Pull(std::vector<unsigned>& bc,
                  std::vector<double>&   imm,
                  size_t& StackTop_max)
        {
            ByteCode.swap(bc);
            Immed.swap(imm);
            StackTop_max = StackMax;
        }

        size_t GetByteCodeSize() const { return ByteCode.size(); }
        size_t GetStackTop()     const { return StackTop; }

        void PushVar(unsigned varno)
        {
            ByteCode.push_back(varno);
            SetStackTop(StackTop+1);
        }

        void PushImmed(double immed)
        {
            using namespace FUNCTIONPARSERTYPES;
            ByteCode.push_back(cImmed);
            Immed.push_back(immed);
            SetStackTop(StackTop+1);
        }

        void StackTopIs(const FPoptimizer_CodeTree::CodeTree& hash)
        {
            if(StackTop > 0)
            {
                StackHash[StackTop-1].first = true;
                StackHash[StackTop-1].second = hash;
            }
        }

        void AddOperation(unsigned opcode, unsigned eat_count, unsigned produce_count = 1)
        {
            using namespace FUNCTIONPARSERTYPES;
            SetStackTop(StackTop - eat_count);

            if(opcode == cMul && ByteCode.back() == cDup)
                ByteCode.back() = cSqr;
            else
                ByteCode.push_back(opcode);
            SetStackTop(StackTop + produce_count);
        }

        void DoPopNMov(size_t targetpos, size_t srcpos)
        {
            using namespace FUNCTIONPARSERTYPES;
            ByteCode.push_back(cPopNMov);
            ByteCode.push_back( (unsigned) targetpos);
            ByteCode.push_back( (unsigned) srcpos);

            SetStackTop(srcpos+1);
            StackHash[targetpos] = StackHash[srcpos];
            SetStackTop(targetpos+1);
        }

        void DoDup(size_t src_pos)
        {
            using namespace FUNCTIONPARSERTYPES;
            if(src_pos == StackTop-1)
            {
                ByteCode.push_back(cDup);
            }
            else
            {
                ByteCode.push_back(cFetch);
                ByteCode.push_back( (unsigned) src_pos);
            }
            SetStackTop(StackTop + 1);
            StackHash[StackTop-1] = StackHash[src_pos];
        }

        bool FindAndDup(const FPoptimizer_CodeTree::CodeTree& hash)
        {
            for(size_t a=StackTop; a-->0; )
            {
                if(StackHash[a].first && StackHash[a].second.IsIdenticalTo(hash))
                {
                    DoDup(a);
                    return true;
                }
            }
            return false;
        }

        void SynthIfStep1(size_t& ofs)
        {
            using namespace FUNCTIONPARSERTYPES;
            SetStackTop(StackTop-1); // the If condition was popped.

            ofs = ByteCode.size();
            ByteCode.push_back(cIf);
            ByteCode.push_back(0); // code index
            ByteCode.push_back(0); // Immed index
        }
        void SynthIfStep2(size_t& ofs)
        {
            using namespace FUNCTIONPARSERTYPES;
            SetStackTop(StackTop-1); // ignore the pushed then-branch result.

            ByteCode[ofs+1] = unsigned( ByteCode.size()+2 );
            ByteCode[ofs+2] = unsigned( Immed.size()      );

            ofs = ByteCode.size();
            ByteCode.push_back(cJump);
            ByteCode.push_back(0); // code index
            ByteCode.push_back(0); // Immed index
        }
        void SynthIfStep3(size_t& ofs)
        {
            SetStackTop(StackTop-1); // ignore the pushed else-branch result.

            ByteCode[ofs+1] = unsigned( ByteCode.size()-1 );
            ByteCode[ofs+2] = unsigned( Immed.size()      );

            SetStackTop(StackTop+1); // one or the other was pushed.
        }

    private:
        void SetStackTop(size_t value)
        {
            StackTop = value;
            if(StackTop > StackMax)
            {
                StackMax = StackTop;
                StackHash.resize(StackMax);
            }
        }

    private:
        std::vector<unsigned> ByteCode;
        std::vector<double>   Immed;

        std::vector<
            std::pair<bool/*known*/, FPoptimizer_CodeTree::CodeTree/*hash*/>
                   > StackHash;
        size_t StackTop;
        size_t StackMax;
    };

    struct SequenceOpCode;
    extern const SequenceOpCode AddSequence; /* Multiplication implemented with adds */
    extern const SequenceOpCode MulSequence; /* Exponentiation implemented with muls */

    /* Generate a sequence that multiplies or exponentifies the
     * last operand in the stack by the given constant integer
     * amount (positive or negative).
     */
    void AssembleSequence(
        long count,
        const SequenceOpCode& sequencing,
        ByteCodeSynth& synth);
}

#endif

#line 1 "fpoptimizer/fpoptimizer_bytecodesynth.cc"
// line removed

#ifdef FP_SUPPORT_OPTIMIZER

using namespace FUNCTIONPARSERTYPES;

namespace FPoptimizer_ByteCode
{
    const struct SequenceOpCode
    {
        double basevalue;
        unsigned op_flip;
        unsigned op_normal, op_normal_flip;
        unsigned op_inverse, op_inverse_flip;
    } AddSequence = {0.0, cNeg, cAdd, cAdd, cSub, cRSub },
      MulSequence = {1.0, cInv, cMul, cMul, cDiv, cRDiv };
}

using namespace FPoptimizer_ByteCode;

#define POWI_TABLE_SIZE 256
#define POWI_WINDOW_SIZE 3
namespace FPoptimizer_ByteCode
{
    signed char powi_table[POWI_TABLE_SIZE] =
    {
          0,   1,   1,   1,   2,   1,   3,   1, /*   0 -   7 */
          4,   1,   5,   1,   6,   1,   7,   5, /*   8 -  15 */
          8,   1,   9,   1,  10,   1,  11,   1, /*  16 -  23 */
         12,   5,  13,   9,  14,   1,  15,   1, /*  24 -  31 */
         16,   1,  17,   1,  18,   1,  19,  13, /*  32 -  39 */
         20,   1,  21,   1,  22,   9,   1,   2, /*  40 -  47 */
         24,   1,  25,  17,  26,   1,  27,  11, /*  48 -  55 */
         28,  19,  29,   8,  30,   1,  31,  21, /*  56 -  63 */
         32,   1,  33,   1,  34,   1,  35,   1, /*  64 -  71 */
         36,   1,  37,  25,  38,   1,  39,   1, /*  72 -  79 */
         40,   9,  41,   1,  42,  17,   1,  29, /*  80 -  87 */
         44,   1,  45,   1,  46,  31,  47,  19, /*  88 -  95 */
         48,   1,  49,  33,  50,   1,  51,   1, /*  96 - 103 */
         52,  35,  53,   8,  54,   1,  55,  37, /* 104 - 111 */
         56,   1,  57,  16,  58,  13,  59,  17, /* 112 - 119 */
         60,   1,  61,  41,  62,  25,  63,   1, /* 120 - 127 */
         64,   1,  65,   1,  66,   1,  67,  45, /* 128 - 135 */
         68,   1,  69,   1,  70,   1,  71,   8, /* 136 - 143 */
         72,   1,  73,  49,  74,   1,  75,   1, /* 144 - 151 */
         76,  17,   1,  31,  78,   1,  79,  53, /* 152 - 159 */
         80,   1,  81,   1,  82,  33,   1,   2, /* 160 - 167 */
         84,   1,  85,  19,  86,   8,  87,  35, /* 168 - 175 */
         88,   1,  89,   1,  90,   1,  91,  61, /* 176 - 183 */
         92,  37,  93,  17,  94,  21,  95,   1, /* 184 - 191 */
         96,   1,  97,  65,  98,   1,  99,   1, /* 192 - 199 */
        100,  67, 101,   8, 102,  41, 103,  69, /* 200 - 207 */
        104,   1, 105,  16, 106,  71, 107,   1, /* 208 - 215 */
        108,   1, 109,  73, 110,  17, 111,   1, /* 216 - 223 */
        112,  45, 113,  32, 114,   1, 115,  33, /* 224 - 231 */
        116,   1, 117,   1, 118,   1, 119,   1, /* 232 - 239 */
        120,   1, 121,  81, 122,  49, 123,  19, /* 240 - 247 */
        124,   1, 125,   1, 126,   1, 127,  85  /* 248 - 255 */
    }; /* as in gcc, but custom-optimized for stack calculation */
}
static const int POWI_CACHE_SIZE = 256;

#define FPO(x) /**/
//#define FPO(x) x


namespace
{
    class PowiCache
    {
    private:
        int cache[POWI_CACHE_SIZE];
        int cache_needed[POWI_CACHE_SIZE];

    public:
        PowiCache()
            : cache(), cache_needed() /* Assume we have no factors in the cache */
        {
            /* Decide which factors we would need multiple times.
             * Output:
             *   cache[]        = these factors were generated
             *   cache_needed[] = number of times these factors were desired
             */
            cache[1] = 1; // We have this value already.
        }

        bool Plan_Add(long value, int count)
        {
            if(value >= POWI_CACHE_SIZE) return false;
            //FPO(fprintf(stderr, "%ld will be needed %d times more\n", count, need_count));
            cache_needed[value] += count;
            return cache[value] != 0;
        }

        void Plan_Has(long value)
        {
            if(value < POWI_CACHE_SIZE)
                cache[value] = 1; // This value has been generated
        }

        void Start(size_t value1_pos)
        {
            for(int n=2; n<POWI_CACHE_SIZE; ++n)
                cache[n] = -1; /* Stack location for each component */

            Remember(1, value1_pos);

            DumpContents();
        }

        int Find(long value) const
        {
            if(value < POWI_CACHE_SIZE)
            {
                if(cache[value] >= 0)
                {
                    // found from the cache
                    FPO(fprintf(stderr, "* I found %ld from cache (%u,%d)\n",
                        value, (unsigned)cache[value], cache_needed[value]));
                    return cache[value];
                }
            }
            return -1;
        }

        void Remember(long value, size_t stackpos)
        {
            if(value >= POWI_CACHE_SIZE) return;

            FPO(fprintf(stderr, "* Remembering that %ld can be found at %u (%d uses remain)\n",
                value, (unsigned)stackpos, cache_needed[value]));
            cache[value] = (int) stackpos;
        }

        void DumpContents() const
        {
            FPO(for(int a=1; a<POWI_CACHE_SIZE; ++a)
                if(cache[a] >= 0 || cache_needed[a] > 0)
                {
                    fprintf(stderr, "== cache: sp=%d, val=%d, needs=%d\n",
                        cache[a], a, cache_needed[a]);
                })
        }

        int UseGetNeeded(long value)
        {
            if(value >= 0 && value < POWI_CACHE_SIZE)
                return --cache_needed[value];
            return 0;
        }
    };


    size_t AssembleSequence_Subdivide(
        long count,
        PowiCache& cache,
        const SequenceOpCode& sequencing,
        ByteCodeSynth& synth);

    void Subdivide_Combine(
        size_t apos, long aval,
        size_t bpos, long bval,
        PowiCache& cache,

        unsigned cumulation_opcode,
        unsigned cimulation_opcode_flip,

        ByteCodeSynth& synth);

    void PlanNtimesCache
        (long value,
         PowiCache& cache,
         int need_count,
         int recursioncount=0)
    {
        if(value < 1) return;

    #ifdef FP_GENERATING_POWI_TABLE
        if(recursioncount > 32) throw false;
    #endif

        if(cache.Plan_Add(value, need_count)) return;

        long half = 1;
        if(value < POWI_TABLE_SIZE)
            half = powi_table[value];
        else if(value & 1)
            half = value & ((1 << POWI_WINDOW_SIZE) - 1); // that is, value & 7
        else
            half = value / 2;

        long otherhalf = value-half;
        if(half > otherhalf || half<0) std::swap(half,otherhalf);

        FPO(fprintf(stderr, "value=%ld, half=%ld, otherhalf=%ld\n", value,half,otherhalf));

        if(half == otherhalf)
        {
            PlanNtimesCache(half,      cache, 2, recursioncount+1);
        }
        else
        {
            PlanNtimesCache(half,      cache, 1, recursioncount+1);
            PlanNtimesCache(otherhalf>0?otherhalf:-otherhalf,
                                       cache, 1, recursioncount+1);
        }

        cache.Plan_Has(value);
    }

    size_t AssembleSequence_Subdivide(
        long value,
        PowiCache& cache,
        const SequenceOpCode& sequencing,
        ByteCodeSynth& synth)
    {
        int cachepos = cache.Find(value);
        if(cachepos >= 0)
        {
            // found from the cache
            return cachepos;
        }

        long half = 1;
        if(value < POWI_TABLE_SIZE)
            half = powi_table[value];
        else if(value & 1)
            half = value & ((1 << POWI_WINDOW_SIZE) - 1); // that is, value & 7
        else
            half = value / 2;
        long otherhalf = value-half;
        if(half > otherhalf || half<0) std::swap(half,otherhalf);

        FPO(fprintf(stderr, "* I want %ld, my plan is %ld + %ld\n", value, half, value-half));

        if(half == otherhalf)
        {
            size_t half_pos = AssembleSequence_Subdivide(half, cache, sequencing, synth);

            // self-cumulate the subdivide result
            Subdivide_Combine(half_pos,half, half_pos,half, cache,
                sequencing.op_normal, sequencing.op_normal_flip,
                synth);
        }
        else
        {
            long part1 = half;
            long part2 = otherhalf>0?otherhalf:-otherhalf;

            size_t part1_pos = AssembleSequence_Subdivide(part1, cache, sequencing, synth);
            size_t part2_pos = AssembleSequence_Subdivide(part2, cache, sequencing, synth);

            FPO(fprintf(stderr, "Subdivide(%ld: %ld, %ld)\n", value, half, otherhalf));

            Subdivide_Combine(part1_pos,part1, part2_pos,part2, cache,
                otherhalf>0 ? sequencing.op_normal      : sequencing.op_inverse,
                otherhalf>0 ? sequencing.op_normal_flip : sequencing.op_inverse_flip,
                synth);
        }
        size_t stackpos = synth.GetStackTop()-1;
        cache.Remember(value, stackpos);
        cache.DumpContents();
        return stackpos;
    }

    void Subdivide_Combine(
        size_t apos, long aval,
        size_t bpos, long bval,
        PowiCache& cache,
        unsigned cumulation_opcode,
        unsigned cumulation_opcode_flip,
        ByteCodeSynth& synth)
    {
        /*FPO(fprintf(stderr, "== making result for (sp=%u, val=%d, needs=%d) and (sp=%u, val=%d, needs=%d), stacktop=%u\n",
            (unsigned)apos, aval, aval>=0 ? cache_needed[aval] : -1,
            (unsigned)bpos, bval, bval>=0 ? cache_needed[bval] : -1,
            (unsigned)synth.GetStackTop()));*/

        // Figure out whether we can trample a and b
        int a_needed = cache.UseGetNeeded(aval);
        int b_needed = cache.UseGetNeeded(bval);

        bool flipped = false;

        #define DUP_BOTH() do { \
            if(apos < bpos) { size_t tmp=apos; apos=bpos; bpos=tmp; flipped=!flipped; } \
            FPO(fprintf(stderr, "-> dup(%u) dup(%u) op\n", (unsigned)apos, (unsigned)bpos)); \
            synth.DoDup(apos); \
            synth.DoDup(apos==bpos ? synth.GetStackTop()-1 : bpos); } while(0)
        #define DUP_ONE(p) do { \
            FPO(fprintf(stderr, "-> dup(%u) op\n", (unsigned)p)); \
            synth.DoDup(p); \
        } while(0)

        if(a_needed > 0)
        {
            if(b_needed > 0)
            {
                // If they must both be preserved, make duplicates
                // First push the one that is at the larger stack
                // address. This increases the odds of possibly using cDup.
                DUP_BOTH();

                //SCENARIO 1:
                // Input:  x B A x x
                // Temp:   x B A x x A B
                // Output: x B A x x R
                //SCENARIO 2:
                // Input:  x A B x x
                // Temp:   x A B x x B A
                // Output: x A B x x R
            }
            else
            {
                // A must be preserved, but B can be trampled over

                // SCENARIO 1:
                //  Input:  x B x x A
                //   Temp:  x B x x A A B   (dup both, later first)
                //  Output: x B x x A R
                // SCENARIO 2:
                //  Input:  x A x x B
                //   Temp:  x A x x B A
                //  Output: x A x x R       -- only commutative cases
                // SCENARIO 3:
                //  Input:  x x x B A
                //   Temp:  x x x B A A B   (dup both, later first)
                //  Output: x x x B A R
                // SCENARIO 4:
                //  Input:  x x x A B
                //   Temp:  x x x A B A     -- only commutative cases
                //  Output: x x x A R
                // SCENARIO 5:
                //  Input:  x A B x x
                //   Temp:  x A B x x A B   (dup both, later first)
                //  Output: x A B x x R

                // if B is not at the top, dup both.
                if(bpos != synth.GetStackTop()-1)
                    DUP_BOTH();    // dup both
                else
                {
                    DUP_ONE(apos); // just dup A
                    flipped=!flipped;
                }
            }
        }
        else if(b_needed > 0)
        {
            // B must be preserved, but A can be trampled over
            // This is a mirror image of the a_needed>0 case, so I'll cut the chase
            if(apos != synth.GetStackTop()-1)
                DUP_BOTH();
            else
                DUP_ONE(bpos);
        }
        else
        {
            // Both can be trampled over.
            // SCENARIO 1:
            //  Input:  x B x x A
            //   Temp:  x B x x A B
            //  Output: x B x x R
            // SCENARIO 2:
            //  Input:  x A x x B
            //   Temp:  x A x x B A
            //  Output: x A x x R       -- only commutative cases
            // SCENARIO 3:
            //  Input:  x x x B A
            //  Output: x x x R         -- only commutative cases
            // SCENARIO 4:
            //  Input:  x x x A B
            //  Output: x x x R
            // SCENARIO 5:
            //  Input:  x A B x x
            //   Temp:  x A B x x A B   (dup both, later first)
            //  Output: x A B x x R
            // SCENARIO 6:
            //  Input:  x x x C
            //   Temp:  x x x C C   (c is both A and B)
            //  Output: x x x R

            if(apos == bpos && apos == synth.GetStackTop()-1)
                DUP_ONE(apos); // scenario 6
            else if(apos == synth.GetStackTop()-1 && bpos == synth.GetStackTop()-2)
            {
                FPO(fprintf(stderr, "-> op\n")); // scenario 3
                flipped=!flipped;
            }
            else if(apos == synth.GetStackTop()-2 && bpos == synth.GetStackTop()-1)
                FPO(fprintf(stderr, "-> op\n")); // scenario 4
            else if(apos == synth.GetStackTop()-1)
                DUP_ONE(bpos); // scenario 1
            else if(bpos == synth.GetStackTop()-1)
            {
                DUP_ONE(apos); // scenario 2
                flipped=!flipped;
            }
            else
                DUP_BOTH(); // scenario 5
        }
        // Add them together.
        synth.AddOperation(flipped ? cumulation_opcode_flip : cumulation_opcode, 2);
    }
}

namespace FPoptimizer_ByteCode
{
    void AssembleSequence(
        long count,
        const SequenceOpCode& sequencing,
        ByteCodeSynth& synth)
    {
        if(count == 0)
            synth.PushImmed(sequencing.basevalue);
        else
        {
            if(count < 0)
            {
                synth.AddOperation(sequencing.op_flip, 1);
                count = -count;
            }

            if(count > 1)
            {
                /* To prevent calculating the same factors over and over again,
                 * we use a cache. */
                PowiCache cache;
                PlanNtimesCache(count, cache, 1);

                size_t stacktop_desired = synth.GetStackTop();

                cache.Start( synth.GetStackTop()-1 );

                FPO(fprintf(stderr, "Calculating result for %ld...\n", count));
                size_t res_stackpos = AssembleSequence_Subdivide(
                    count, cache, sequencing,
                    synth);

                size_t n_excess = synth.GetStackTop() - stacktop_desired;
                if(n_excess > 0 || res_stackpos != stacktop_desired-1)
                {
                    // Remove the cache values
                    synth.DoPopNMov(stacktop_desired-1, res_stackpos);
                }
            }
        }
    }
}

#endif

#line 1 "fpoptimizer/fpoptimizer_codetree.cc"
#include <list>
#include <algorithm>

// line removed
#include "fptypes.hh"
// line removed
// line removed

#ifdef FP_SUPPORT_OPTIMIZER

using namespace FUNCTIONPARSERTYPES;
//using namespace FPoptimizer_Grammar;

namespace
{
    bool MarkIncompletes(FPoptimizer_CodeTree::CodeTree& tree)
    {
        if(tree.Is_Incompletely_Hashed())
            return true;

        bool needs_rehash = false;
        for(size_t a=0; a<tree.GetParamCount(); ++a)
            needs_rehash |= MarkIncompletes(tree.GetParam(a));
        if(needs_rehash)
            tree.Mark_Incompletely_Hashed();
        return needs_rehash;
    }

    void FixIncompletes(FPoptimizer_CodeTree::CodeTree& tree)
    {
        if(tree.Is_Incompletely_Hashed())
        {
            for(size_t a=0; a<tree.GetParamCount(); ++a)
                FixIncompletes(tree.GetParam(a));
            tree.Rehash();
        }
    }
}

namespace FPoptimizer_CodeTree
{
    CodeTree::CodeTree()
        : data(new CodeTreeData)
    {
    }

    CodeTree::CodeTree(double i)
        : data(new CodeTreeData(i))
    {
        data->Recalculate_Hash_NoRecursion();
    }

    CodeTree::CodeTree(unsigned v, CodeTree::VarTag)
        : data(new CodeTreeData)
    {
        data->Opcode = cVar;
        data->Var    = v;
        data->Recalculate_Hash_NoRecursion();
    }

    CodeTree::CodeTree(const CodeTree& b, CodeTree::CloneTag)
        : data(new CodeTreeData(*b.data))
    {
    }

    CodeTree::~CodeTree()
    {
    }

    struct ParamComparer
    {
        bool operator() (const CodeTree& a, const CodeTree& b) const
        {
            if(a.GetDepth() != b.GetDepth())
                return a.GetDepth() > b.GetDepth();
            return a.GetHash() < b.GetHash();
        }
    };

    void CodeTreeData::Sort()
    {
        /* If the tree is commutative, order the parameters
         * in a set order in order to make equality tests
         * efficient in the optimizer
         */
        switch(Opcode)
        {
            case cAdd:
            case cMul:
            case cMin:
            case cMax:
            case cAnd:
            case cOr:
            case cEqual:
            case cNEqual:
                std::sort(Params.begin(), Params.end(), ParamComparer());
                break;
            case cLess:
                if(ParamComparer() (Params[1], Params[0]))
                    { std::swap(Params[0], Params[1]); Opcode = cGreater; }
                break;
            case cLessOrEq:
                if(ParamComparer() (Params[1], Params[0]))
                    { std::swap(Params[0], Params[1]); Opcode = cGreaterOrEq; }
                break;
            case cGreater:
                if(ParamComparer() (Params[1], Params[0]))
                    { std::swap(Params[0], Params[1]); Opcode = cLess; }
                break;
            case cGreaterOrEq:
                if(ParamComparer() (Params[1], Params[0]))
                    { std::swap(Params[0], Params[1]); Opcode = cLessOrEq; }
                break;
            default:
                break;
        }
    }

    void CodeTree::Rehash(bool constantfolding)
    {
        if(constantfolding)
            ConstantFolding();
        data->Sort();
        data->Recalculate_Hash_NoRecursion();
    }

    void CodeTreeData::Recalculate_Hash_NoRecursion()
    {
        fphash_t NewHash = { Opcode * FPHASH_CONST(0x3A83A83A83A83A0),
                             Opcode * FPHASH_CONST(0x1131462E270012B)};
        Depth = 1;
        switch(Opcode)
        {
            case cImmed:
                if(Value != 0.0)
                {
                    crc32_t crc = crc32::calc( (const unsigned char*) &Value,
                                                sizeof(Value) );
                    NewHash.hash1 ^= crc | (fphash_value_t(crc) << FPHASH_CONST(32));
                    NewHash.hash2 += ((~fphash_value_t(crc)) * 3) ^ 1234567;
                }
                break; // no params
            case cVar:
                NewHash.hash1 ^= (Var<<24) | (Var>>24);
                NewHash.hash2 += (fphash_value_t(Var)*5) ^ 2345678;
                break; // no params
            case cFCall: case cPCall:
            {
                crc32_t crc = crc32::calc( (const unsigned char*) &Funcno, sizeof(Funcno) );
                NewHash.hash1 ^= (crc<<24) | (crc>>24);
                NewHash.hash2 += ((~fphash_value_t(crc)) * 7) ^ 3456789;
                /* passthru */
            }
            default:
            {
                size_t MaxChildDepth = 0;
                for(size_t a=0; a<Params.size(); ++a)
                {
                    if(Params[a].GetDepth() > MaxChildDepth)
                        MaxChildDepth = Params[a].GetDepth();

                    NewHash.hash1 += (1)*FPHASH_CONST(0x2492492492492492);
                    NewHash.hash1 *= FPHASH_CONST(1099511628211);
                    //assert(&*Params[a] != this);
                    NewHash.hash1 += Params[a].GetHash().hash1;

                    NewHash.hash2 += (3)*FPHASH_CONST(0x9ABCD801357);
                    NewHash.hash2 *= FPHASH_CONST(0xECADB912345);
                    NewHash.hash2 += (~Params[a].GetHash().hash1) ^ 4567890;
                }
                Depth += MaxChildDepth;
            }
        }
        if(Hash != NewHash)
        {
            Hash = NewHash;
            OptimizedUsing = 0;
        }
    }

    void CodeTree::AddParam(const CodeTree& param)
    {
        //std::cout << "AddParam called\n";
        data->Params.push_back(param);
    }
    void CodeTree::AddParamMove(CodeTree& param)
    {
        data->Params.push_back(CodeTree());
        data->Params.back().swap(param);
    }
    void CodeTree::SetParam(size_t which, const CodeTree& b)
    {
        DataP slot_holder ( data->Params[which].data );
        data->Params[which] = b;
    }
    void CodeTree::SetParamMove(size_t which, CodeTree& b)
    {
        DataP slot_holder ( data->Params[which].data );
        data->Params[which].swap(b);
    }

    void CodeTree::AddParams(const std::vector<CodeTree>& RefParams)
    {
        data->Params.insert(data->Params.end(), RefParams.begin(), RefParams.end());
    }
    void CodeTree::AddParamsMove(std::vector<CodeTree>& RefParams)
    {
        size_t endpos = data->Params.size(), added = RefParams.size();
        data->Params.resize(endpos + added, CodeTree());
        for(size_t p=0; p<added; ++p)
            data->Params[endpos+p].swap( RefParams[p] );
    }
    void CodeTree::AddParamsMove(std::vector<CodeTree>& RefParams, size_t replacing_slot)
    {
        DataP slot_holder ( data->Params[replacing_slot].data );
        DelParam(replacing_slot);
        AddParamsMove(RefParams);
    /*
        const size_t n_added = RefParams.size();
        const size_t oldsize = data->Params.size();
        const size_t newsize = oldsize + n_added - 1;
        if(RefParams.empty())
            DelParam(replacing_slot);
        else
        {
            //    0 1 2 3 4 5 6 7 8 9 10 11
            //    a a a a X b b b b b
            //    a a a a Y Y Y b b b b  b
            //
            //   replacing_slot = 4
            //   n_added = 3
            //   oldsize = 10
            //   newsize = 12
            //   tail_length = 5

            data->Params.resize(newsize);
            data->Params[replacing_slot].data = 0;
            const size_t tail_length = oldsize - replacing_slot -1;
            for(size_t tail=0; tail<tail_length; ++tail)
                data->Params[newsize-1-tail].data.UnsafeSetP(
                &*data->Params[newsize-1-tail-(n_added-1)].data);
            for(size_t head=1; head<n_added; ++head)
                data->Params[replacing_slot+head].data.UnsafeSetP( 0 );
            for(size_t p=0; p<n_added; ++p)
                data->Params[replacing_slot+p].swap( RefParams[p] );
        }
    */
    }

    void CodeTree::SetParams(const std::vector<CodeTree>& RefParams)
    {
        //std::cout << "SetParams called" << (do_clone ? ", clone" : ", no clone") << "\n";
        std::vector<CodeTree> tmp(RefParams);
        data->Params.swap(tmp);
    }

    void CodeTree::SetParamsMove(std::vector<CodeTree>& RefParams)
    {
        data->Params.swap(RefParams);
        RefParams.clear();
    }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    void CodeTree::SetParams(std::vector<CodeTree>&& RefParams)
    {
        //std::cout << "SetParams&& called\n";
        SetParamsMove(RefParams);
    }
#endif

    void CodeTree::DelParam(size_t index)
    {
        std::vector<CodeTree>& Params = data->Params;
        //std::cout << "DelParam(" << index << ") called\n";
    #ifdef __GXX_EXPERIMENTAL_CXX0X__
        /* rvalue reference semantics makes this optimal */
        Params.erase( Params.begin() + index );
    #else
        /* This labor evades the need for refcount +1/-1 shuffling */
        Params[index].data = 0;
        for(size_t p=index; p+1<Params.size(); ++p)
            Params[p].data.UnsafeSetP( &*Params[p+1].data );
        Params[Params.size()-1].data.UnsafeSetP( 0 );
        Params.resize(Params.size()-1);
    #endif
    }

    void CodeTree::DelParams()
    {
        data->Params.clear();
    }

    /* Is the value of this tree definitely odd(true) or even(false)? */
    CodeTree::TriTruthValue CodeTree::GetEvennessInfo() const
    {
        if(!IsImmed()) return Unknown;
        if(!IsLongIntegerImmed()) return Unknown;
        return (GetLongIntegerImmed() & 1) ? IsNever : IsAlways;
    }

    bool CodeTree::IsLogicalValue() const
    {
        switch(data->Opcode)
        {
            case cImmed:
                return FloatEqual(data->Value, 0.0)
                    || FloatEqual(data->Value, 1.0);
            case cAnd:
            case cOr:
            case cNot:
            case cNotNot:
            case cEqual:
            case cNEqual:
            case cLess:
            case cLessOrEq:
            case cGreater:
            case cGreaterOrEq:
                /* These operations always produce truth values (0 or 1) */
                return true;
            case cMul:
            {
                std::vector<CodeTree>& Params = data->Params;
                for(size_t a=0; a<Params.size(); ++a)
                    if(!Params[a].IsLogicalValue())
                        return false;
                return true;
            }
            case cIf:
            {
                std::vector<CodeTree>& Params = data->Params;
                return Params[1].IsLogicalValue()
                    && Params[2].IsLogicalValue();
            }
            default:
                break;
        }
        return false; // Not a logical value.
    }

    bool CodeTree::IsAlwaysInteger() const
    {
        switch(data->Opcode)
        {
            case cImmed:
                return IsLongIntegerImmed();
            case cFloor:
            case cInt:
                return true;
            case cAnd:
            case cOr:
            case cNot:
            case cNotNot:
            case cEqual:
            case cNEqual:
            case cLess:
            case cLessOrEq:
            case cGreater:
            case cGreaterOrEq:
                /* These operations always produce truth values (0 or 1) */
                return true; /* 0 and 1 are both integers */
            case cIf:
            {
                std::vector<CodeTree>& Params = data->Params;
                return Params[1].IsAlwaysInteger()
                    && Params[2].IsAlwaysInteger();
            }
            default:
                break;
        }
        return false; /* Don't know whether it's integer. */
    }

    bool CodeTree::IsAlwaysSigned(bool positive) const
    {
        MinMaxTree tmp = CalculateResultBoundaries();

        if(positive)
            return tmp.has_min && tmp.min >= 0.0
              && (!tmp.has_max || tmp.max >= 0.0);
        else
            return tmp.has_max && tmp.max < 0.0
              && (!tmp.has_min || tmp.min < 0.0);
    }

    bool CodeTree::IsIdenticalTo(const CodeTree& b) const
    {
        if((!data) != (!b.data)) return false;
        if(&*data == &*b.data) return true;
        return data->IsIdenticalTo(*b.data);
    }

    bool CodeTreeData::IsIdenticalTo(const CodeTreeData& b) const
    {
        if(Hash   != b.Hash) return false; // a quick catch-all
        if(Opcode != b.Opcode) return false;
        switch(Opcode)
        {
            case cImmed: return FloatEqual(Value, b.Value);
            case cVar:   return Var == b.Var;
            case cFCall:
            case cPCall: if(Funcno != b.Funcno) return false; break;
            default: break;
        }
        if(Params.size() != b.Params.size()) return false;
        for(size_t a=0; a<Params.size(); ++a)
        {
            if(!Params[a].IsIdenticalTo(b.Params[a])) return false;
        }
        return true;
    }

    void CodeTree::Become(const CodeTree& b)
    {
        if(&b != this && &*data != &*b.data)
        {
            DataP tmp = b.data;
            CopyOnWrite();
            data.swap(tmp);
        }
    }

    void CodeTree::CopyOnWrite()
    {
        if(data->RefCount > 1)
            data = new CodeTreeData(*data);
    }

    CodeTree CodeTree::GetUniqueRef()
    {
        if(data->RefCount > 1)
            return CodeTree(*this, CloneTag());
        return *this;
    }

    CodeTreeData::CodeTreeData()
        : RefCount(0),
          Opcode(), Params(), Hash(), Depth(1), OptimizedUsing(0)
    {
    }

    CodeTreeData::CodeTreeData(const CodeTreeData& b)
        : RefCount(0),
          Opcode(b.Opcode),
          Params(b.Params),
          Hash(b.Hash),
          Depth(b.Depth),
          OptimizedUsing(b.OptimizedUsing)
    {
        switch(Opcode)
        {
            case cVar:   Var   = b.Var; break;
            case cImmed: Value = b.Value; break;
            case cPCall:
            case cFCall: Funcno = b.Funcno; break;
            default: break;
        }
    }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    CodeTreeData::CodeTreeData(CodeTreeData&& b)
        : RefCount(0),
          Opcode(b.Opcode),
          Params(b.Params),
          Hash(b.Hash),
          Depth(b.Depth),
          OptimizedUsing(b.OptimizedUsing)
    {
        switch(Opcode)
        {
            case cVar:   Var   = b.Var; break;
            case cImmed: Value = b.Value; break;
            case cPCall:
            case cFCall: Funcno = b.Funcno; break;
            default: break;
        }
    }
#endif

    CodeTreeData::CodeTreeData(double i)
        : RefCount(0), Opcode(cImmed), Params(), Hash(), Depth(1), OptimizedUsing(0)
    {
        Value = i;
    }

    void FixIncompleteHashes(CodeTree& tree)
    {
        MarkIncompletes(tree);
        FixIncompletes(tree);
    }
}

#endif

#line 1 "fpoptimizer/fpoptimizer_grammar.cc"
#include "fparser.hh"
#include "fptypes.hh"
// line removed
// line removed

// line removed
using namespace FPoptimizer_Grammar;
using namespace FUNCTIONPARSERTYPES;

#include <cctype>

namespace FPoptimizer_Grammar
{
    bool ParamSpec_Compare(const void* aa, const void* bb, SpecialOpcode type)
    {
        switch(type)
        {
            case ParamHolder:
            {
                ParamSpec_ParamHolder& a = *(ParamSpec_ParamHolder*) aa;
                ParamSpec_ParamHolder& b = *(ParamSpec_ParamHolder*) bb;
                return a.constraints == b.constraints
                    && a.index       == b.index
                    && a.depcode     == b.depcode;
            }
            case NumConstant:
            {
                ParamSpec_NumConstant& a = *(ParamSpec_NumConstant*) aa;
                ParamSpec_NumConstant& b = *(ParamSpec_NumConstant*) bb;
                return FloatEqual(a.constvalue, b.constvalue);
            }
            case SubFunction:
            {
                ParamSpec_SubFunction& a = *(ParamSpec_SubFunction*) aa;
                ParamSpec_SubFunction& b = *(ParamSpec_SubFunction*) bb;
                return a.constraints    == b.constraints
                    && a.data.subfunc_opcode   == b.data.subfunc_opcode
                    && a.data.match_type       == b.data.match_type
                    && a.data.param_count      == b.data.param_count
                    && a.data.param_list       == b.data.param_list
                    && a.data.restholder_index == b.data.restholder_index
                    && a.depcode               == b.depcode;
            }
        }
        return true;
    }

    unsigned ParamSpec_GetDepCode(const ParamSpec& b)
    {
        switch(b.first)
        {
            case ParamHolder:
            {
                const ParamSpec_ParamHolder* s = (const ParamSpec_ParamHolder*) b.second;
                return s->depcode;
            }
            case SubFunction:
            {
                const ParamSpec_SubFunction* s = (const ParamSpec_SubFunction*) b.second;
                return s->depcode;
            }
            default: break;
        }
        return 0;
    }

    void DumpParam(const ParamSpec& parampair, std::ostream& o)
    {
        static const char ParamHolderNames[][2]  = {"%","&", "x","y","z","a","b","c"};

        //o << "/*p" << (&p-pack.plist) << "*/";
        unsigned constraints = 0;
        switch(parampair.first)
        {
            case NumConstant:
              { const ParamSpec_NumConstant& param = *(const ParamSpec_NumConstant*) parampair.second;
                o.precision(12);
                o << param.constvalue; break; }
            case ParamHolder:
              { const ParamSpec_ParamHolder& param = *(const ParamSpec_ParamHolder*) parampair.second;
                o << ParamHolderNames[param.index];
                constraints = param.constraints;
                break; }
            case SubFunction:
              { const ParamSpec_SubFunction& param = *(const ParamSpec_SubFunction*) parampair.second;
                constraints = param.constraints;
                if(param.data.match_type == GroupFunction)
                {
                    if(param.data.subfunc_opcode == cNeg)
                        { o << "-"; DumpParams(param.data.param_list, param.data.param_count, o); }
                    else if(param.data.subfunc_opcode == cInv)
                        { o << "/"; DumpParams(param.data.param_list, param.data.param_count, o); }
                    else
                    {
                        std::string opcode = FP_GetOpcodeName(param.data.subfunc_opcode).substr(1);
                        for(size_t a=0; a<opcode.size(); ++a) opcode[a] = (char) std::toupper(opcode[a]);
                        o << opcode << "( ";
                        DumpParams(param.data.param_list, param.data.param_count, o);
                        o << " )";
                    }
                }
                else
                {
                    o << '(' << FP_GetOpcodeName(param.data.subfunc_opcode) << ' ';
                    if(param.data.match_type == PositionalParams) o << '[';
                    if(param.data.match_type == SelectedParams) o << '{';
                    DumpParams(param.data.param_list, param.data.param_count, o);
                    if(param.data.restholder_index != 0)
                        o << " <" << param.data.restholder_index << '>';
                    if(param.data.match_type == PositionalParams) o << "]";
                    if(param.data.match_type == SelectedParams) o << "}";
                    o << ')';
                }
                break; }
        }
        switch( ImmedConstraint_Value(constraints & ValueMask) )
        {
            case ValueMask: break;
            case Value_AnyNum: break;
            case Value_EvenInt:   o << "@E"; break;
            case Value_OddInt:    o << "@O"; break;
            case Value_IsInteger: o << "@I"; break;
            case Value_NonInteger:o << "@F"; break;
            case Value_Logical:   o << "@L"; break;
        }
        switch( ImmedConstraint_Sign(constraints & SignMask) )
        {
            case SignMask: break;
            case Sign_AnySign: break;
            case Sign_Positive:   o << "@P"; break;
            case Sign_Negative:   o << "@N"; break;
        }
        switch( ImmedConstraint_Oneness(constraints & OnenessMask) )
        {
            case OnenessMask: break;
            case Oneness_Any: break;
            case Oneness_One:     o << "@1"; break;
            case Oneness_NotOne:  o << "@M"; break;
        }
    }

    void DumpParams(unsigned paramlist, unsigned count, std::ostream& o)
    {
        for(unsigned a=0; a<count; ++a)
        {
            if(a > 0) o << ' ';
            const ParamSpec& param = ParamSpec_Extract(paramlist,a);
            DumpParam(param, o);
            unsigned depcode = ParamSpec_GetDepCode(param);
            if(depcode != 0)
                o << "@D" << depcode;
        }
    }
}

#line 1 "fpoptimizer/fpoptimizer_grammar_data.cc"
/* This file is automatically generated. Do not edit... */
// line removed
// line removed
#include "fpconfig.hh"
#include "fptypes.hh"
#include <algorithm>

using namespace FPoptimizer_Grammar;
using namespace FUNCTIONPARSERTYPES;

#define P1(a) a
#define P2(a,b) (P1(a) | (b << PARAM_INDEX_BITS))
#define P3(a,b,c) (P2(a,b) | (c << (PARAM_INDEX_BITS*2)))

namespace
{
    const struct ParamSpec_List
    {
        ParamSpec_ParamHolder plist_p[20];
#define P(n) (n)
        ParamSpec_NumConstant plist_n[14];
#define N(n) (n+20)
        ParamSpec_SubFunction plist_s[292];
#define S(n) (n+20+14)
    } /*PACKED_GRAMMAR_ATTRIBUTE*/ plist =
    {
        { /* plist_p - ParamSpec_ParamHolder[20] */
        /* 0	*/ {0, Sign_Negative | Constness_Const, 0x0}, /* %@N */
        /* 1	*/ {0, Constness_Const, 0x0}, /* % */
        /* 2	*/ {0, Sign_Positive | Constness_Const, 0x0}, /* %@P */
        /* 3	*/ {0, Constness_Const, 0x1}, /* % */
        /* 4	*/ {0, Oneness_NotOne | Constness_Const, 0x0}, /* %@M */
        /* 5	*/ {1, Constness_Const, 0x0}, /* & */
        /* 6	*/ {1, Oneness_NotOne | Constness_Const, 0x0}, /* &@M */
        /* 7	*/ {2, 0, 0x0}, /* x */
        /* 8	*/ {2, 0, 0x4}, /* x */
        /* 9	*/ {2, Value_Logical, 0x4}, /* x@L */
        /* 10	*/ {2, Value_Logical, 0x0}, /* x@L */
        /* 11	*/ {2, Sign_Positive, 0x0}, /* x@P */
        /* 12	*/ {2, Sign_NoIdea, 0x0}, /* x */
        /* 13	*/ {3, 0, 0x0}, /* y */
        /* 14	*/ {3, Value_EvenInt, 0x0}, /* y@E */
        /* 15	*/ {3, Value_OddInt, 0x0}, /* y@O */
        /* 16	*/ {3, Value_NonInteger, 0x0}, /* y@F */
        /* 17	*/ {3, Value_Logical, 0x0}, /* y@L */
        /* 18	*/ {4, 0, 0x16}, /* z */
        /* 19	*/ {4, 0, 0x0}, /* z */
        },

        { /* plist_n - ParamSpec_NumConstant[14] */
        /* 0	*/ {-2}, /* -2 */
        /* 1	*/ {-CONSTANT_PIHALF}, /* -1.57079632679 */
        /* 2	*/ {-1}, /* -1 */
        /* 3	*/ {0}, /* 0 */
        /* 4	*/ {CONSTANT_RD}, /* 0.0174532925199 */
        /* 5	*/ {CONSTANT_2EI}, /* 0.135335283237 */
        /* 6	*/ {CONSTANT_EI}, /* 0.367879441171 */
        /* 7	*/ {0.5}, /* 0.5 */
        /* 8	*/ {1}, /* 1 */
        /* 9	*/ {CONSTANT_PIHALF}, /* 1.57079632679 */
        /* 10	*/ {2}, /* 2 */
        /* 11	*/ {CONSTANT_E}, /* 2.71828182846 */
        /* 12	*/ {CONSTANT_2E}, /* 7.38905609893 */
        /* 13	*/ {CONSTANT_DR}, /* 57.2957795131 */
        },

        { /* plist_s - ParamSpec_SubFunction[292] */
        /* 0	*/ {{1,P1(P(7))                , cAbs        ,PositionalParams,0}, 0, 0x0}, /* (cAbs [x]) */
        /* 1	*/ {{1,P1(P(13))               , cAbs        ,PositionalParams,0}, 0, 0x0}, /* (cAbs [y]) */
        /* 2	*/ {{1,P1(S(194))              , cAbs        ,PositionalParams,0}, 0, 0x0}, /* (cAbs [(cMul {x y})]) */
        /* 3	*/ {{1,P1(P(7))                , cAcos       ,PositionalParams,0}, 0, 0x0}, /* (cAcos [x]) */
        /* 4	*/ {{1,P1(P(7))                , cAcosh      ,PositionalParams,0}, 0, 0x0}, /* (cAcosh [x]) */
        /* 5	*/ {{1,P1(P(7))                , cAsin       ,PositionalParams,0}, 0, 0x0}, /* (cAsin [x]) */
        /* 6	*/ {{1,P1(P(7))                , cAsinh      ,PositionalParams,0}, 0, 0x0}, /* (cAsinh [x]) */
        /* 7	*/ {{1,P1(P(7))                , cAtan       ,PositionalParams,0}, 0, 0x0}, /* (cAtan [x]) */
        /* 8	*/ {{1,P1(P(7))                , cAtanh      ,PositionalParams,0}, 0, 0x0}, /* (cAtanh [x]) */
        /* 9	*/ {{1,P1(P(7))                , cCos        ,PositionalParams,0}, 0, 0x0}, /* (cCos [x]) */
        /* 10	*/ {{1,P1(P(7))                , cCos        ,PositionalParams,0}, 0, 0x4}, /* (cCos [x]) */
        /* 11	*/ {{1,P1(P(13))               , cCos        ,PositionalParams,0}, 0, 0x0}, /* (cCos [y]) */
        /* 12	*/ {{1,P1(S(134))              , cCos        ,PositionalParams,0}, 0, 0x0}, /* (cCos [(cAdd {x y})]) */
        /* 13	*/ {{1,P1(S(141))              , cCos        ,PositionalParams,0}, 0, 0x0}, /* (cCos [(cAdd {x (cMul {-1 y})})]) */
        /* 14	*/ {{1,P1(S(161))              , cCos        ,PositionalParams,0}, 0, 0x0}, /* (cCos [(cAdd  <1>)]) */
        /* 15	*/ {{1,P1(S(254))              , cCos        ,PositionalParams,0}, 0, 0x0}, /* (cCos [(cMul -% <1>)]) */
        /* 16	*/ {{1,P1(P(7))                , cCosh       ,PositionalParams,0}, 0, 0x4}, /* (cCosh [x]) */
        /* 17	*/ {{1,P1(P(7))                , cCosh       ,PositionalParams,0}, 0, 0x0}, /* (cCosh [x]) */
        /* 18	*/ {{1,P1(S(191))              , cCosh       ,PositionalParams,0}, 0, 0x0}, /* (cCosh [(cMul {x LOG( & )})]) */
        /* 19	*/ {{1,P1(P(7))                , cCot        ,PositionalParams,0}, 0, 0x0}, /* (cCot [x]) */
        /* 20	*/ {{1,P1(P(7))                , cCsc        ,PositionalParams,0}, 0, 0x0}, /* (cCsc [x]) */
        /* 21	*/ {{3,P3(P(13),N(3),S(161))   , cIf         ,PositionalParams,0}, 0, 0x0}, /* (cIf [y 0 (cAdd  <1>)]) */
        /* 22	*/ {{3,P3(P(13),N(3),S(279))   , cIf         ,PositionalParams,0}, 0, 0x0}, /* (cIf [y 0 (cOr  <1>)]) */
        /* 23	*/ {{3,P3(P(13),N(8),S(229))   , cIf         ,PositionalParams,0}, 0, 0x0}, /* (cIf [y 1 (cMul  <1>)]) */
        /* 24	*/ {{3,P3(P(13),N(8),S(270))   , cIf         ,PositionalParams,0}, 0, 0x0}, /* (cIf [y 1 (cAnd  <1>)]) */
        /* 25	*/ {{3,P3(P(13),S(161),N(3))   , cIf         ,PositionalParams,0}, 0, 0x0}, /* (cIf [y (cAdd  <1>) 0]) */
        /* 26	*/ {{3,P3(P(13),S(161),S(162)) , cIf         ,PositionalParams,0}, 0, 0x0}, /* (cIf [y (cAdd  <1>) (cAdd  <2>)]) */
        /* 27	*/ {{3,P3(P(13),S(229),N(8))   , cIf         ,PositionalParams,0}, 0, 0x0}, /* (cIf [y (cMul  <1>) 1]) */
        /* 28	*/ {{3,P3(P(13),S(229),S(230)) , cIf         ,PositionalParams,0}, 0, 0x0}, /* (cIf [y (cMul  <1>) (cMul  <2>)]) */
        /* 29	*/ {{3,P3(P(13),S(270),S(271)) , cIf         ,PositionalParams,0}, 0, 0x0}, /* (cIf [y (cAnd  <1>) (cAnd  <2>)]) */
        /* 30	*/ {{3,P3(P(13),S(279),N(3))   , cIf         ,PositionalParams,0}, 0, 0x0}, /* (cIf [y (cOr  <1>) 0]) */
        /* 31	*/ {{3,P3(P(13),S(270),N(8))   , cIf         ,PositionalParams,0}, 0, 0x0}, /* (cIf [y (cAnd  <1>) 1]) */
        /* 32	*/ {{3,P3(P(13),S(279),S(280)) , cIf         ,PositionalParams,0}, 0, 0x0}, /* (cIf [y (cOr  <1>) (cOr  <2>)]) */
        /* 33	*/ {{1,P1(P(7))                , cLog        ,PositionalParams,0}, 0, 0x0}, /* (cLog [x]) */
        /* 34	*/ {{1,P1(P(13))               , cLog        ,PositionalParams,0}, 0, 0x0}, /* (cLog [y]) */
        /* 35	*/ {{1,P1(P(19))               , cLog        ,PositionalParams,0}, 0, 0x0}, /* (cLog [z]) */
        /* 36	*/ {{1,P1(S(0))                , cLog        ,PositionalParams,0}, 0, 0x0}, /* (cLog [(cAbs [x])]) */
        /* 37	*/ {{1,P1(S(194))              , cLog        ,PositionalParams,0}, 0, 0x0}, /* (cLog [(cMul {x y})]) */
        /* 38	*/ {{1,P1(S(229))              , cLog        ,PositionalParams,0}, 0, 0x0}, /* (cLog [(cMul  <1>)]) */
        /* 39	*/ {{1,P1(P(1))                , cLog        ,GroupFunction   ,0}, Constness_Const, 0x0}, /* LOG( % ) */
        /* 40	*/ {{1,P1(P(5))                , cLog        ,GroupFunction   ,0}, Constness_Const, 0x0}, /* LOG( & ) */
        /* 41	*/ {{2,P2(P(1),N(8))           , cMin        ,GroupFunction   ,0}, Constness_Const, 0x0}, /* MIN( % 1 ) */
        /* 42	*/ {{2,P2(P(1),P(5))           , cMin        ,GroupFunction   ,0}, Constness_Const, 0x0}, /* MIN( % & ) */
        /* 43	*/ {{2,P2(N(5),P(7))           , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [0.135335283237 x]) */
        /* 44	*/ {{2,P2(N(6),P(7))           , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [0.367879441171 x]) */
        /* 45	*/ {{2,P2(N(11),P(7))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [2.71828182846 x]) */
        /* 46	*/ {{2,P2(N(11),P(7))          , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [2.71828182846 x]) */
        /* 47	*/ {{2,P2(N(12),P(7))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [7.38905609893 x]) */
        /* 48	*/ {{2,P2(N(11),S(230))        , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [2.71828182846 (cMul  <2>)]) */
        /* 49	*/ {{2,P2(P(5),P(7))           , cPow        ,PositionalParams,0}, 0, 0x6}, /* (cPow [& x]) */
        /* 50	*/ {{2,P2(P(5),P(7))           , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [& x]) */
        /* 51	*/ {{2,P2(P(7),N(10))          , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [x 2]) */
        /* 52	*/ {{2,P2(P(7),N(10))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x 2]) */
        /* 53	*/ {{2,P2(P(13),N(10))         , cPow        ,PositionalParams,0}, 0, 0x8}, /* (cPow [y 2]) */
        /* 54	*/ {{2,P2(P(7),P(1))           , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x %]) */
        /* 55	*/ {{2,P2(P(7),P(5))           , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x &]) */
        /* 56	*/ {{2,P2(P(1),P(13))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [% y]) */
        /* 57	*/ {{2,P2(P(11),P(13))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x@P y]) */
        /* 58	*/ {{2,P2(P(7),P(14))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x y@E]) */
        /* 59	*/ {{2,P2(P(7),P(15))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x y@O]) */
        /* 60	*/ {{2,P2(P(7),P(16))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x y@F]) */
        /* 61	*/ {{2,P2(P(12),P(13))         , cPow        ,PositionalParams,0}, Sign_Positive, 0x0}, /* (cPow [x y])@P */
        /* 62	*/ {{2,P2(P(7),S(39))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x LOG( % )]) */
        /* 63	*/ {{2,P2(P(7),S(41))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x MIN( % 1 )]) */
        /* 64	*/ {{2,P2(P(7),S(42))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x MIN( % & )]) */
        /* 65	*/ {{2,P2(P(7),S(130))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x (cAdd {1 -MIN( % 1 )})]) */
        /* 66	*/ {{2,P2(P(7),S(135))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x (cAdd {% -MIN( % 1 )})]) */
        /* 67	*/ {{2,P2(P(7),S(136))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x (cAdd {% -MIN( % & )})]) */
        /* 68	*/ {{2,P2(P(7),S(137))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [x (cAdd {& -MIN( % & )})]) */
        /* 69	*/ {{2,P2(P(1),S(161))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [% (cAdd  <1>)]) */
        /* 70	*/ {{2,P2(P(19),S(161))        , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [z (cAdd  <1>)]) */
        /* 71	*/ {{2,P2(S(9),N(2))           , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cCos [x]) -1]) */
        /* 72	*/ {{2,P2(S(9),N(2))           , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cCos [x]) -1]) */
        /* 73	*/ {{2,P2(S(17),N(2))          , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cCosh [x]) -1]) */
        /* 74	*/ {{2,P2(S(17),N(2))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cCosh [x]) -1]) */
        /* 75	*/ {{2,P2(S(102),N(2))         , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cSin [x]) -1]) */
        /* 76	*/ {{2,P2(S(111),N(2))         , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cSinh [x]) -1]) */
        /* 77	*/ {{2,P2(S(116),N(2))         , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cTan [x]) -1]) */
        /* 78	*/ {{2,P2(S(116),N(2))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cTan [x]) -1]) */
        /* 79	*/ {{2,P2(S(121),N(2))         , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cTanh [x]) -1]) */
        /* 80	*/ {{2,P2(S(121),N(2))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cTanh [x]) -1]) */
        /* 81	*/ {{2,P2(S(131),N(2))         , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cAdd {1 (cMul {-1 x})}) -1]) */
        /* 82	*/ {{2,P2(S(143),N(2))         , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cAdd {(cPow [0.135335283237 x]) -1}) -1]) */
        /* 83	*/ {{2,P2(S(145),N(2))         , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cAdd {(cPow [7.38905609893 x]) -1}) -1]) */
        /* 84	*/ {{2,P2(S(146),N(2))         , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cAdd {(cPow [0.135335283237 x]) 1}) -1]) */
        /* 85	*/ {{2,P2(S(148),N(2))         , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cAdd {(cPow [7.38905609893 x]) 1}) -1]) */
        /* 86	*/ {{2,P2(S(186),N(2))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cMul {-1 (cTanh [x])}) -1]) */
        /* 87	*/ {{2,P2(S(150),N(7))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cAdd {(cPow [x 2]) -1}) 0.5]) */
        /* 88	*/ {{2,P2(S(151),N(7))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cAdd {(cPow [x 2]) 1}) 0.5]) */
        /* 89	*/ {{2,P2(S(230),N(2))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cMul  <2>) -1]) */
        /* 90	*/ {{2,P2(S(239),N(2))         , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cMul x <2>) -1]) */
        /* 91	*/ {{2,P2(S(9),N(10))          , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cCos [x]) 2]) */
        /* 92	*/ {{2,P2(S(9),N(10))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cCos [x]) 2]) */
        /* 93	*/ {{2,P2(S(35),N(2))          , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cLog [z]) -1]) */
        /* 94	*/ {{2,P2(S(102),N(10))        , cPow        ,PositionalParams,0}, 0, 0x4}, /* (cPow [(cSin [x]) 2]) */
        /* 95	*/ {{2,P2(S(102),N(10))        , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cSin [x]) 2]) */
        /* 96	*/ {{2,P2(S(134),N(10))        , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cAdd {x y}) 2]) */
        /* 97	*/ {{2,P2(S(229),P(5))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [(cMul  <1>) &]) */
        /* 98	*/ {{2,P2(S(290),P(7))         , cPow        ,PositionalParams,0}, 0, 0x0}, /* (cPow [/& x]) */
        /* 99	*/ {{2,P2(S(290),P(7))         , cPow        ,PositionalParams,0}, 0, 0x6}, /* (cPow [/& x]) */
        /* 100	*/ {{2,P2(P(1),P(5))           , cPow        ,GroupFunction   ,0}, Constness_Const, 0x0}, /* POW( % & ) */
        /* 101	*/ {{1,P1(P(7))                , cSec        ,PositionalParams,0}, 0, 0x0}, /* (cSec [x]) */
        /* 102	*/ {{1,P1(P(7))                , cSin        ,PositionalParams,0}, 0, 0x0}, /* (cSin [x]) */
        /* 103	*/ {{1,P1(P(7))                , cSin        ,PositionalParams,0}, 0, 0x4}, /* (cSin [x]) */
        /* 104	*/ {{1,P1(P(13))               , cSin        ,PositionalParams,0}, 0, 0x0}, /* (cSin [y]) */
        /* 105	*/ {{1,P1(S(134))              , cSin        ,PositionalParams,0}, 0, 0x0}, /* (cSin [(cAdd {x y})]) */
        /* 106	*/ {{1,P1(S(141))              , cSin        ,PositionalParams,0}, 0, 0x0}, /* (cSin [(cAdd {x (cMul {-1 y})})]) */
        /* 107	*/ {{1,P1(S(161))              , cSin        ,PositionalParams,0}, 0, 0x0}, /* (cSin [(cAdd  <1>)]) */
        /* 108	*/ {{1,P1(S(234))              , cSin        ,PositionalParams,0}, 0, 0x0}, /* (cSin [(cMul % <1>)]) */
        /* 109	*/ {{1,P1(S(254))              , cSin        ,PositionalParams,0}, 0, 0x0}, /* (cSin [(cMul -% <1>)]) */
        /* 110	*/ {{1,P1(P(7))                , cSinh       ,PositionalParams,0}, 0, 0x4}, /* (cSinh [x]) */
        /* 111	*/ {{1,P1(P(7))                , cSinh       ,PositionalParams,0}, 0, 0x0}, /* (cSinh [x]) */
        /* 112	*/ {{1,P1(S(191))              , cSinh       ,PositionalParams,0}, 0, 0x0}, /* (cSinh [(cMul {x LOG( & )})]) */
        /* 113	*/ {{1,P1(S(229))              , cSinh       ,PositionalParams,0}, 0, 0x0}, /* (cSinh [(cMul  <1>)]) */
        /* 114	*/ {{1,P1(S(234))              , cSinh       ,PositionalParams,0}, 0, 0x0}, /* (cSinh [(cMul % <1>)]) */
        /* 115	*/ {{1,P1(S(254))              , cSinh       ,PositionalParams,0}, 0, 0x0}, /* (cSinh [(cMul -% <1>)]) */
        /* 116	*/ {{1,P1(P(7))                , cTan        ,PositionalParams,0}, 0, 0x0}, /* (cTan [x]) */
        /* 117	*/ {{1,P1(P(7))                , cTan        ,PositionalParams,0}, 0, 0x4}, /* (cTan [x]) */
        /* 118	*/ {{1,P1(S(229))              , cTan        ,PositionalParams,0}, 0, 0x0}, /* (cTan [(cMul  <1>)]) */
        /* 119	*/ {{1,P1(S(234))              , cTan        ,PositionalParams,0}, 0, 0x0}, /* (cTan [(cMul % <1>)]) */
        /* 120	*/ {{1,P1(S(254))              , cTan        ,PositionalParams,0}, 0, 0x0}, /* (cTan [(cMul -% <1>)]) */
        /* 121	*/ {{1,P1(P(7))                , cTanh       ,PositionalParams,0}, 0, 0x0}, /* (cTanh [x]) */
        /* 122	*/ {{1,P1(P(7))                , cTanh       ,PositionalParams,0}, 0, 0x4}, /* (cTanh [x]) */
        /* 123	*/ {{1,P1(S(229))              , cTanh       ,PositionalParams,0}, 0, 0x0}, /* (cTanh [(cMul  <1>)]) */
        /* 124	*/ {{1,P1(S(234))              , cTanh       ,PositionalParams,0}, 0, 0x0}, /* (cTanh [(cMul % <1>)]) */
        /* 125	*/ {{1,P1(S(254))              , cTanh       ,PositionalParams,0}, 0, 0x0}, /* (cTanh [(cMul -% <1>)]) */
        /* 126	*/ {{1,P1(P(1))                , cNeg        ,GroupFunction   ,0}, Constness_Const, 0x0}, /* -% */
        /* 127	*/ {{1,P1(S(41))               , cNeg        ,GroupFunction   ,0}, Constness_Const, 0x0}, /* -MIN( % 1 ) */
        /* 128	*/ {{1,P1(S(42))               , cNeg        ,GroupFunction   ,0}, Constness_Const, 0x0}, /* -MIN( % & ) */
        /* 129	*/ {{2,P2(N(8),P(7))           , cAdd        ,SelectedParams  ,0}, 0, 0x4}, /* (cAdd {1 x}) */
        /* 130	*/ {{2,P2(N(8),S(127))         , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {1 -MIN( % 1 )}) */
        /* 131	*/ {{2,P2(N(8),S(173))         , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {1 (cMul {-1 x})}) */
        /* 132	*/ {{2,P2(N(8),S(216))         , cAdd        ,SelectedParams  ,0}, 0, 0x1}, /* (cAdd {1 (cMul {(cLog [x]) /%})}) */
        /* 133	*/ {{2,P2(N(9),S(232))         , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {1.57079632679 (cMul %@N <1>)}) */
        /* 134	*/ {{2,P2(P(7),P(13))          , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {x y}) */
        /* 135	*/ {{2,P2(P(1),S(127))         , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {% -MIN( % 1 )}) */
        /* 136	*/ {{2,P2(P(1),S(128))         , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {% -MIN( % & )}) */
        /* 137	*/ {{2,P2(P(5),S(128))         , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {& -MIN( % & )}) */
        /* 138	*/ {{2,P2(P(7),S(21))          , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {x (cIf [y 0 (cAdd  <1>)])}) */
        /* 139	*/ {{2,P2(P(7),S(25))          , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {x (cIf [y (cAdd  <1>) 0])}) */
        /* 140	*/ {{2,P2(P(7),S(26))          , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {x (cIf [y (cAdd  <1>) (cAdd  <2>)])}) */
        /* 141	*/ {{2,P2(P(7),S(174))         , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {x (cMul {-1 y})}) */
        /* 142	*/ {{2,P2(S(43),N(2))          , cAdd        ,SelectedParams  ,0}, 0, 0x4}, /* (cAdd {(cPow [0.135335283237 x]) -1}) */
        /* 143	*/ {{2,P2(S(43),N(2))          , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cPow [0.135335283237 x]) -1}) */
        /* 144	*/ {{2,P2(S(47),N(2))          , cAdd        ,SelectedParams  ,0}, 0, 0x4}, /* (cAdd {(cPow [7.38905609893 x]) -1}) */
        /* 145	*/ {{2,P2(S(47),N(2))          , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cPow [7.38905609893 x]) -1}) */
        /* 146	*/ {{2,P2(S(43),N(8))          , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cPow [0.135335283237 x]) 1}) */
        /* 147	*/ {{2,P2(S(43),N(8))          , cAdd        ,SelectedParams  ,0}, 0, 0x4}, /* (cAdd {(cPow [0.135335283237 x]) 1}) */
        /* 148	*/ {{2,P2(S(47),N(8))          , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cPow [7.38905609893 x]) 1}) */
        /* 149	*/ {{2,P2(S(47),N(8))          , cAdd        ,SelectedParams  ,0}, 0, 0x4}, /* (cAdd {(cPow [7.38905609893 x]) 1}) */
        /* 150	*/ {{2,P2(S(52),N(2))          , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cPow [x 2]) -1}) */
        /* 151	*/ {{2,P2(S(52),N(8))          , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cPow [x 2]) 1}) */
        /* 152	*/ {{2,P2(S(33),P(1))          , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cLog [x]) %}) */
        /* 153	*/ {{2,P2(S(229),N(0))         , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cMul  <1>) -2}) */
        /* 154	*/ {{2,P2(S(33),P(5))          , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cLog [x]) &}) */
        /* 155	*/ {{2,P2(S(220),S(38))        , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cMul {LOG( % ) y}) (cLog [(cMul  <1>)])}) */
        /* 156	*/ {{2,P2(S(38),S(39))         , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cLog [(cMul  <1>)]) LOG( % )}) */
        /* 157	*/ {{2,P2(S(240),S(192))       , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cMul % & <1>) (cMul {& (cAdd  <2>)})}) */
        /* 158	*/ {{2,P2(S(256),S(193))       , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {MUL( % & ) (cMul {& (cAdd  <1>)})}) */
        /* 159	*/ {{2,P2(S(249),S(248))       , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cMul (cPow [x (cAdd {% -MIN( % 1 )})]) <1>) (cMul (cPow [x (cAdd {1 -MIN( % 1 )})]) <2>)}) */
        /* 160	*/ {{2,P2(S(250),S(251))       , cAdd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAdd {(cMul (cPow [x (cAdd {% -MIN( % & )})]) <1>) (cMul (cPow [x (cAdd {& -MIN( % & )})]) <2>)}) */
        /* 161	*/ {{0,0                       , cAdd        ,AnyParams       ,1}, 0, 0x0}, /* (cAdd  <1>) */
        /* 162	*/ {{0,0                       , cAdd        ,AnyParams       ,2}, 0, 0x0}, /* (cAdd  <2>) */
        /* 163	*/ {{0,0                       , cAdd        ,AnyParams       ,1}, Sign_Positive, 0x0}, /* (cAdd  <1>)@P */
        /* 164	*/ {{1,P1(N(1))                , cAdd        ,AnyParams       ,1}, 0, 0x0}, /* (cAdd -1.57079632679 <1>) */
        /* 165	*/ {{1,P1(P(1))                , cAdd        ,AnyParams       ,1}, 0, 0x0}, /* (cAdd % <1>) */
        /* 166	*/ {{1,P1(P(6))                , cAdd        ,AnyParams       ,1}, 0, 0x0}, /* (cAdd &@M <1>) */
        /* 167	*/ {{1,P1(P(7))                , cAdd        ,AnyParams       ,1}, 0, 0x4}, /* (cAdd x <1>) */
        /* 168	*/ {{1,P1(P(7))                , cAdd        ,AnyParams       ,2}, 0, 0x4}, /* (cAdd x <2>) */
        /* 169	*/ {{1,P1(P(7))                , cAdd        ,AnyParams       ,1}, 0, 0x0}, /* (cAdd x <1>) */
        /* 170	*/ {{1,P1(S(93))               , cAdd        ,AnyParams       ,1}, 0, 0x16}, /* (cAdd (cPow [(cLog [z]) -1]) <1>) */
        /* 171	*/ {{1,P1(S(235))              , cAdd        ,AnyParams       ,2}, 0, 0x0}, /* (cAdd (cMul %@M <1>) <2>) */
        /* 172	*/ {{1,P1(S(252))              , cAdd        ,AnyParams       ,1}, 0, 0x16}, /* (cAdd (cMul (cPow [(cLog [z]) -1]) <2>) <1>) */
        /* 173	*/ {{2,P2(N(2),P(7))           , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 x}) */
        /* 174	*/ {{2,P2(N(2),P(13))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 y}) */
        /* 175	*/ {{2,P2(N(2),S(13))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 (cCos [(cAdd {x (cMul {-1 y})})])}) */
        /* 176	*/ {{2,P2(N(2),S(14))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 (cCos [(cAdd  <1>)])}) */
        /* 177	*/ {{2,P2(N(2),S(17))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 (cCosh [x])}) */
        /* 178	*/ {{2,P2(N(2),S(50))          , cMul        ,SelectedParams  ,0}, 0, 0x6}, /* (cMul {-1 (cPow [& x])}) */
        /* 179	*/ {{2,P2(N(2),S(92))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 (cPow [(cCos [x]) 2])}) */
        /* 180	*/ {{2,P2(N(2),S(95))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 (cPow [(cSin [x]) 2])}) */
        /* 181	*/ {{2,P2(N(2),S(98))          , cMul        ,SelectedParams  ,0}, 0, 0x6}, /* (cMul {-1 (cPow [/& x])}) */
        /* 182	*/ {{2,P2(N(2),S(109))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 (cSin [(cMul -% <1>)])}) */
        /* 183	*/ {{2,P2(N(2),S(111))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 (cSinh [x])}) */
        /* 184	*/ {{2,P2(N(2),S(113))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 (cSinh [(cMul  <1>)])}) */
        /* 185	*/ {{2,P2(N(2),S(118))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 (cTan [(cMul  <1>)])}) */
        /* 186	*/ {{2,P2(N(2),S(121))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 (cTanh [x])}) */
        /* 187	*/ {{2,P2(N(2),S(123))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {-1 (cTanh [(cMul  <1>)])}) */
        /* 188	*/ {{2,P2(N(11),S(70))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {2.71828182846 (cPow [z (cAdd  <1>)])}) */
        /* 189	*/ {{2,P2(P(1),S(50))          , cMul        ,SelectedParams  ,0}, 0, 0x7}, /* (cMul {% (cPow [& x])}) */
        /* 190	*/ {{2,P2(P(1),S(98))          , cMul        ,SelectedParams  ,0}, 0, 0x7}, /* (cMul {% (cPow [/& x])}) */
        /* 191	*/ {{2,P2(P(7),S(40))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {x LOG( & )}) */
        /* 192	*/ {{2,P2(P(5),S(162))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {& (cAdd  <2>)}) */
        /* 193	*/ {{2,P2(P(5),S(161))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {& (cAdd  <1>)}) */
        /* 194	*/ {{2,P2(P(7),P(13))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {x y}) */
        /* 195	*/ {{3,P3(P(7),P(13),S(153))   , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {x y (cAdd {(cMul  <1>) -2})}) */
        /* 196	*/ {{2,P2(P(13),P(19))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {y z}) */
        /* 197	*/ {{2,P2(P(7),S(23))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {x (cIf [y 1 (cMul  <1>)])}) */
        /* 198	*/ {{2,P2(P(7),S(27))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {x (cIf [y (cMul  <1>) 1])}) */
        /* 199	*/ {{2,P2(P(7),S(28))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {x (cIf [y (cMul  <1>) (cMul  <2>)])}) */
        /* 200	*/ {{2,P2(P(13),S(33))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {y (cLog [x])}) */
        /* 201	*/ {{2,P2(P(13),S(36))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {y (cLog [(cAbs [x])])}) */
        /* 202	*/ {{2,P2(S(17),N(2))          , cMul        ,SelectedParams  ,0}, 0, 0x4}, /* (cMul {(cCosh [x]) -1}) */
        /* 203	*/ {{2,P2(S(44),N(2))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {(cPow [0.367879441171 x]) -1}) */
        /* 204	*/ {{2,P2(S(45),N(2))          , cMul        ,SelectedParams  ,0}, 0, 0x4}, /* (cMul {(cPow [2.71828182846 x]) -1}) */
        /* 205	*/ {{2,P2(S(111),N(2))         , cMul        ,SelectedParams  ,0}, 0, 0x4}, /* (cMul {(cSinh [x]) -1}) */
        /* 206	*/ {{2,P2(S(8),N(10))          , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {(cAtanh [x]) 2}) */
        /* 207	*/ {{3,P3(S(18),N(10),P(1))    , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {(cCosh [(cMul {x LOG( & )})]) 2 %}) */
        /* 208	*/ {{2,P2(S(112),N(0))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {(cSinh [(cMul {x LOG( & )})]) -2}) */
        /* 209	*/ {{2,P2(S(18),N(10))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {(cCosh [(cMul {x LOG( & )})]) 2}) */
        /* 210	*/ {{2,P2(S(112),N(10))        , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {(cSinh [(cMul {x LOG( & )})]) 2}) */
        /* 211	*/ {{3,P3(S(112),N(10),P(1))   , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {(cSinh [(cMul {x LOG( & )})]) 2 %}) */
        /* 212	*/ {{2,P2(S(9),S(11))          , cMul        ,SelectedParams  ,0}, 0, 0x12}, /* (cMul {(cCos [x]) (cCos [y])}) */
        /* 213	*/ {{2,P2(S(126),S(98))        , cMul        ,SelectedParams  ,0}, 0, 0x7}, /* (cMul {-% (cPow [/& x])}) */
        /* 214	*/ {{2,P2(S(129),S(81))        , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {(cAdd {1 x})@D4 (cPow [(cAdd {1 (cMul {-1 x})}) -1])@D4}) */
        /* 215	*/ {{2,P2(S(9),S(104))         , cMul        ,SelectedParams  ,0}, 0, 0x12}, /* (cMul {(cCos [x]) (cSin [y])}) */
        /* 216	*/ {{2,P2(S(33),S(289))        , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {(cLog [x]) /%}) */
        /* 217	*/ {{3,P3(S(9),S(11),N(2))     , cMul        ,SelectedParams  ,0}, 0, 0x12}, /* (cMul {(cCos [x]) (cCos [y]) -1}) */
        /* 218	*/ {{2,P2(S(102),S(104))       , cMul        ,SelectedParams  ,0}, 0, 0x12}, /* (cMul {(cSin [x]) (cSin [y])}) */
        /* 219	*/ {{3,P3(S(9),S(104),N(2))    , cMul        ,SelectedParams  ,0}, 0, 0x12}, /* (cMul {(cCos [x]) (cSin [y]) -1}) */
        /* 220	*/ {{2,P2(S(39),P(13))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {LOG( % ) y}) */
        /* 221	*/ {{2,P2(S(102),S(11))        , cMul        ,SelectedParams  ,0}, 0, 0x12}, /* (cMul {(cSin [x]) (cCos [y])}) */
        /* 222	*/ {{3,P3(S(102),S(104),N(2))  , cMul        ,SelectedParams  ,0}, 0, 0x12}, /* (cMul {(cSin [x]) (cSin [y]) -1}) */
        /* 223	*/ {{2,P2(S(63),S(159))        , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {(cPow [x MIN( % 1 )]) (cAdd {(cMul (cPow [x (cAdd {% -MIN( % 1 )})]) <1>) (cMul (cPow [x (cAdd {1 -MIN( % 1 )})]) <2>)})}) */
        /* 224	*/ {{2,P2(S(64),S(160))        , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {(cPow [x MIN( % & )]) (cAdd {(cMul (cPow [x (cAdd {% -MIN( % & )})]) <1>) (cMul (cPow [x (cAdd {& -MIN( % & )})]) <2>)})}) */
        /* 225	*/ {{2,P2(S(70),S(48))         , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {(cPow [z (cAdd  <1>)]) (cPow [2.71828182846 (cMul  <2>)])}) */
        /* 226	*/ {{2,P2(S(100),S(69))        , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {POW( % & ) (cPow [% (cAdd  <1>)])}) */
        /* 227	*/ {{2,P2(S(100),S(62))        , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {POW( % & ) (cPow [x LOG( % )])}) */
        /* 228	*/ {{2,P2(S(100),S(97))        , cMul        ,SelectedParams  ,0}, 0, 0x0}, /* (cMul {POW( % & ) (cPow [(cMul  <1>) &])}) */
        /* 229	*/ {{0,0                       , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul  <1>) */
        /* 230	*/ {{0,0                       , cMul        ,AnyParams       ,2}, 0, 0x0}, /* (cMul  <2>) */
        /* 231	*/ {{1,P1(N(2))                , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul -1 <1>) */
        /* 232	*/ {{1,P1(P(0))                , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul %@N <1>) */
        /* 233	*/ {{1,P1(P(2))                , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul %@P <1>) */
        /* 234	*/ {{1,P1(P(1))                , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul % <1>) */
        /* 235	*/ {{1,P1(P(4))                , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul %@M <1>) */
        /* 236	*/ {{1,P1(P(7))                , cMul        ,AnyParams       ,1}, 0, 0x4}, /* (cMul x <1>) */
        /* 237	*/ {{1,P1(P(7))                , cMul        ,AnyParams       ,2}, 0, 0x4}, /* (cMul x <2>) */
        /* 238	*/ {{1,P1(P(7))                , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul x <1>) */
        /* 239	*/ {{1,P1(P(7))                , cMul        ,AnyParams       ,2}, 0, 0x0}, /* (cMul x <2>) */
        /* 240	*/ {{2,P2(P(1),P(5))           , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul % & <1>) */
        /* 241	*/ {{2,P2(P(7),P(13))          , cMul        ,AnyParams       ,1}, 0, 0x12}, /* (cMul x y <1>) */
        /* 242	*/ {{1,P1(S(0))                , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul (cAbs [x]) <1>) */
        /* 243	*/ {{1,P1(S(33))               , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul (cLog [x]) <1>) */
        /* 244	*/ {{1,P1(S(39))               , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul LOG( % ) <1>) */
        /* 245	*/ {{1,P1(S(54))               , cMul        ,AnyParams       ,1}, 0, 0x4}, /* (cMul (cPow [x %]) <1>) */
        /* 246	*/ {{1,P1(S(55))               , cMul        ,AnyParams       ,2}, 0, 0x4}, /* (cMul (cPow [x &]) <2>) */
        /* 247	*/ {{1,P1(S(56))               , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul (cPow [% y]) <1>) */
        /* 248	*/ {{1,P1(S(65))               , cMul        ,AnyParams       ,2}, 0, 0x0}, /* (cMul (cPow [x (cAdd {1 -MIN( % 1 )})]) <2>) */
        /* 249	*/ {{1,P1(S(66))               , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul (cPow [x (cAdd {% -MIN( % 1 )})]) <1>) */
        /* 250	*/ {{1,P1(S(67))               , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul (cPow [x (cAdd {% -MIN( % & )})]) <1>) */
        /* 251	*/ {{1,P1(S(68))               , cMul        ,AnyParams       ,2}, 0, 0x0}, /* (cMul (cPow [x (cAdd {& -MIN( % & )})]) <2>) */
        /* 252	*/ {{1,P1(S(93))               , cMul        ,AnyParams       ,2}, 0, 0x0}, /* (cMul (cPow [(cLog [z]) -1]) <2>) */
        /* 253	*/ {{2,P2(S(93),S(33))         , cMul        ,AnyParams       ,1}, 0, 0x16}, /* (cMul (cPow [(cLog [z]) -1]) (cLog [x]) <1>) */
        /* 254	*/ {{1,P1(S(126))              , cMul        ,AnyParams       ,1}, 0, 0x0}, /* (cMul -% <1>) */
        /* 255	*/ {{2,P2(S(291),S(33))        , cMul        ,AnyParams       ,1}, 0, 0x1}, /* (cMul /LOG( % ) (cLog [x]) <1>) */
        /* 256	*/ {{2,P2(P(1),P(5))           , cMul        ,GroupFunction   ,0}, Constness_Const, 0x0}, /* MUL( % & ) */
        /* 257	*/ {{2,P2(P(7),P(13))          , cEqual      ,PositionalParams,0}, 0, 0x12}, /* (cEqual [x y]) */
        /* 258	*/ {{2,P2(P(7),P(13))          , cEqual      ,PositionalParams,0}, 0, 0x0}, /* (cEqual [x y]) */
        /* 259	*/ {{2,P2(P(7),P(19))          , cEqual      ,PositionalParams,0}, 0, 0x20}, /* (cEqual [x z]) */
        /* 260	*/ {{2,P2(P(13),P(19))         , cEqual      ,PositionalParams,0}, 0, 0x24}, /* (cEqual [y z]) */
        /* 261	*/ {{2,P2(P(13),P(19))         , cEqual      ,PositionalParams,0}, 0, 0x0}, /* (cEqual [y z]) */
        /* 262	*/ {{1,P1(P(7))                , cNot        ,PositionalParams,0}, 0, 0x0}, /* (cNot [x]) */
        /* 263	*/ {{1,P1(P(13))               , cNot        ,PositionalParams,0}, 0, 0x0}, /* (cNot [y]) */
        /* 264	*/ {{1,P1(S(275))              , cNot        ,PositionalParams,0}, 0, 0x0}, /* (cNot [(cOr {x y})]) */
        /* 265	*/ {{2,P2(P(7),P(13))          , cAnd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAnd {x y}) */
        /* 266	*/ {{2,P2(P(7),S(24))          , cAnd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAnd {x (cIf [y 1 (cAnd  <1>)])}) */
        /* 267	*/ {{2,P2(P(7),S(31))          , cAnd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAnd {x (cIf [y (cAnd  <1>) 1])}) */
        /* 268	*/ {{2,P2(P(7),S(29))          , cAnd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAnd {x (cIf [y (cAnd  <1>) (cAnd  <2>)])}) */
        /* 269	*/ {{2,P2(S(262),P(13))        , cAnd        ,SelectedParams  ,0}, 0, 0x0}, /* (cAnd {(cNot [x]) y}) */
        /* 270	*/ {{0,0                       , cAnd        ,AnyParams       ,1}, 0, 0x0}, /* (cAnd  <1>) */
        /* 271	*/ {{0,0                       , cAnd        ,AnyParams       ,2}, 0, 0x0}, /* (cAnd  <2>) */
        /* 272	*/ {{1,P1(P(7))                , cAnd        ,AnyParams       ,1}, 0, 0x4}, /* (cAnd x <1>) */
        /* 273	*/ {{1,P1(P(10))               , cAnd        ,AnyParams       ,1}, 0, 0x4}, /* (cAnd x@L <1>) */
        /* 274	*/ {{1,P1(P(7))                , cAnd        ,AnyParams       ,2}, 0, 0x4}, /* (cAnd x <2>) */
        /* 275	*/ {{2,P2(P(7),P(13))          , cOr         ,SelectedParams  ,0}, 0, 0x0}, /* (cOr {x y}) */
        /* 276	*/ {{2,P2(P(7),S(22))          , cOr         ,SelectedParams  ,0}, 0, 0x0}, /* (cOr {x (cIf [y 0 (cOr  <1>)])}) */
        /* 277	*/ {{2,P2(P(7),S(30))          , cOr         ,SelectedParams  ,0}, 0, 0x0}, /* (cOr {x (cIf [y (cOr  <1>) 0])}) */
        /* 278	*/ {{2,P2(P(7),S(32))          , cOr         ,SelectedParams  ,0}, 0, 0x0}, /* (cOr {x (cIf [y (cOr  <1>) (cOr  <2>)])}) */
        /* 279	*/ {{0,0                       , cOr         ,AnyParams       ,1}, 0, 0x0}, /* (cOr  <1>) */
        /* 280	*/ {{0,0                       , cOr         ,AnyParams       ,2}, 0, 0x0}, /* (cOr  <2>) */
        /* 281	*/ {{1,P1(P(7))                , cOr         ,AnyParams       ,1}, 0, 0x4}, /* (cOr x <1>) */
        /* 282	*/ {{1,P1(P(10))               , cOr         ,AnyParams       ,1}, 0, 0x4}, /* (cOr x@L <1>) */
        /* 283	*/ {{1,P1(P(7))                , cOr         ,AnyParams       ,2}, 0, 0x4}, /* (cOr x <2>) */
        /* 284	*/ {{1,P1(P(7))                , cNotNot     ,PositionalParams,0}, 0, 0x4}, /* (cNotNot [x]) */
        /* 285	*/ {{1,P1(P(7))                , cNotNot     ,PositionalParams,0}, 0, 0x0}, /* (cNotNot [x]) */
        /* 286	*/ {{1,P1(P(13))               , cNotNot     ,PositionalParams,0}, 0, 0x0}, /* (cNotNot [y]) */
        /* 287	*/ {{1,P1(S(229))              , cDeg        ,PositionalParams,0}, 0, 0x0}, /* (cDeg [(cMul  <1>)]) */
        /* 288	*/ {{1,P1(S(229))              , cRad        ,PositionalParams,0}, 0, 0x0}, /* (cRad [(cMul  <1>)]) */
        /* 289	*/ {{1,P1(P(1))                , cInv        ,GroupFunction   ,0}, Constness_Const, 0x0}, /* /% */
        /* 290	*/ {{1,P1(P(5))                , cInv        ,GroupFunction   ,0}, Constness_Const, 0x0}, /* /& */
        /* 291	*/ {{1,P1(S(39))               , cInv        ,GroupFunction   ,0}, Constness_Const, 0x0}, /* /LOG( % ) */
        },

    };
    const Rule rlist[161] =
    {
        /* 0:	(cCos [(cAdd {1.57079632679 (cMul %@N <1>)})])
         *	->	(cSin [(cMul -% <1>)])
         */		 {1, ProduceNewTree, 1,P1(S(109))              , {1,P1(S(133))              , cCos        ,PositionalParams,0}},
        /* 1:	(cCos [(cAdd -1.57079632679 <1>)])
         *	->	(cSin [(cAdd  <1>)])
         */		 {1, ProduceNewTree, 1,P1(S(107))              , {1,P1(S(164))              , cCos        ,PositionalParams,0}},
        /* 2:	(cCos [(cAcos [x])])
         *	->	x
         */		 {1, ProduceNewTree, 1,P1(P(7))                , {1,P1(S(3))                , cCos        ,PositionalParams,0}},
        /* 3:	(cCos [(cMul %@N <1>)])
         *	:	(cMul -% <1>)
         */		 {1, ReplaceParams , 1,P1(S(254))              , {1,P1(S(232))              , cCos        ,PositionalParams,0}},
        /* 4:	(cIf [y (cMul x <1>)@D4 (cMul x <2>)@D4])
         *	->	(cMul {x (cIf [y (cMul  <1>) (cMul  <2>)])})
         */		 {3, ProduceNewTree, 1,P1(S(199))              , {3,P3(P(13),S(236),S(237)) , cIf         ,PositionalParams,0}},
        /* 5:	(cIf [y x@D4 x@D4])
         *	->	x
         */		 {3, ProduceNewTree, 1,P1(P(7))                , {3,P3(P(13),P(8),P(8))     , cIf         ,PositionalParams,0}},
        /* 6:	(cIf [y (cAdd x <1>)@D4 x@D4])
         *	->	(cAdd {x (cIf [y (cAdd  <1>) 0])})
         */		 {3, ProduceNewTree, 1,P1(S(139))              , {3,P3(P(13),S(167),P(8))   , cIf         ,PositionalParams,0}},
        /* 7:	(cIf [y (cMul x <1>)@D4 x@D4])
         *	->	(cMul {x (cIf [y (cMul  <1>) 1])})
         */		 {3, ProduceNewTree, 1,P1(S(198))              , {3,P3(P(13),S(236),P(8))   , cIf         ,PositionalParams,0}},
        /* 8:	(cIf [y x@D4 (cAdd x <1>)@D4])
         *	->	(cAdd {x (cIf [y 0 (cAdd  <1>)])})
         */		 {3, ProduceNewTree, 1,P1(S(138))              , {3,P3(P(13),P(8),S(167))   , cIf         ,PositionalParams,0}},
        /* 9:	(cIf [y (cAdd x <1>)@D4 (cAdd x <2>)@D4])
         *	->	(cAdd {x (cIf [y (cAdd  <1>) (cAdd  <2>)])})
         */		 {3, ProduceNewTree, 1,P1(S(140))              , {3,P3(P(13),S(167),S(168)) , cIf         ,PositionalParams,0}},
        /* 10:	(cIf [y x@D4 (cMul x <1>)@D4])
         *	->	(cMul {x (cIf [y 1 (cMul  <1>)])})
         */		 {3, ProduceNewTree, 1,P1(S(197))              , {3,P3(P(13),P(8),S(236))   , cIf         ,PositionalParams,0}},
        /* 11:	(cIf [y (cNotNot [x])@D4 (cAnd x <1>)@D4])
         *	->	(cAnd {x (cIf [y 1 (cAnd  <1>)])})
         */		 {3, ProduceNewTree, 1,P1(S(266))              , {3,P3(P(13),S(284),S(272)) , cIf         ,PositionalParams,0}},
        /* 12:	(cIf [y (cAnd x <1>)@D4 (cNotNot [x])@D4])
         *	->	(cAnd {x (cIf [y (cAnd  <1>) 1])})
         */		 {3, ProduceNewTree, 1,P1(S(267))              , {3,P3(P(13),S(272),S(284)) , cIf         ,PositionalParams,0}},
        /* 13:	(cIf [y (cOr x <1>)@D4 (cNotNot [x])@D4])
         *	->	(cOr {x (cIf [y (cOr  <1>) 0])})
         */		 {3, ProduceNewTree, 1,P1(S(277))              , {3,P3(P(13),S(281),S(284)) , cIf         ,PositionalParams,0}},
        /* 14:	(cIf [y x@L@D4 (cAnd x@L <1>)@D4])
         *	->	(cAnd {x (cIf [y 1 (cAnd  <1>)])})
         */		 {3, ProduceNewTree, 1,P1(S(266))              , {3,P3(P(13),P(9),S(273))   , cIf         ,PositionalParams,0}},
        /* 15:	(cIf [y (cAnd x@L <1>)@D4 x@L@D4])
         *	->	(cAnd {x (cIf [y (cAnd  <1>) 1])})
         */		 {3, ProduceNewTree, 1,P1(S(267))              , {3,P3(P(13),S(273),P(9))   , cIf         ,PositionalParams,0}},
        /* 16:	(cIf [y (cOr x@L <1>)@D4 x@L@D4])
         *	->	(cOr {x (cIf [y (cOr  <1>) 0])})
         */		 {3, ProduceNewTree, 1,P1(S(277))              , {3,P3(P(13),S(282),P(9))   , cIf         ,PositionalParams,0}},
        /* 17:	(cIf [y (cAnd x <1>)@D4 (cAnd x <2>)@D4])
         *	->	(cAnd {x (cIf [y (cAnd  <1>) (cAnd  <2>)])})
         */		 {3, ProduceNewTree, 1,P1(S(268))              , {3,P3(P(13),S(272),S(274)) , cIf         ,PositionalParams,0}},
        /* 18:	(cIf [y (cNotNot [x])@D4 (cOr x <1>)@D4])
         *	->	(cOr {x (cIf [y 0 (cOr  <1>)])})
         */		 {3, ProduceNewTree, 1,P1(S(276))              , {3,P3(P(13),S(284),S(281)) , cIf         ,PositionalParams,0}},
        /* 19:	(cIf [y x@L@D4 (cOr x@L <1>)@D4])
         *	->	(cOr {x (cIf [y 0 (cOr  <1>)])})
         */		 {3, ProduceNewTree, 1,P1(S(276))              , {3,P3(P(13),P(9),S(282))   , cIf         ,PositionalParams,0}},
        /* 20:	(cIf [y (cOr x <1>)@D4 (cOr x <2>)@D4])
         *	->	(cOr {x (cIf [y (cOr  <1>) (cOr  <2>)])})
         */		 {3, ProduceNewTree, 1,P1(S(278))              , {3,P3(P(13),S(281),S(283)) , cIf         ,PositionalParams,0}},
        /* 21:	(cLog [(cPow [x@P y])])
         *	->	(cMul {y (cLog [x])})
         */		 {1, ProduceNewTree, 1,P1(S(200))              , {1,P1(S(57))               , cLog        ,PositionalParams,0}},
        /* 22:	(cLog [(cPow [x y@E])])
         *	->	(cMul {y (cLog [(cAbs [x])])})
         */		 {1, ProduceNewTree, 1,P1(S(201))              , {1,P1(S(58))               , cLog        ,PositionalParams,0}},
        /* 23:	(cLog [(cMul %@P <1>)])
         *	->	(cAdd {(cLog [(cMul  <1>)]) LOG( % )})
         */		 {1, ProduceNewTree, 1,P1(S(156))              , {1,P1(S(233))              , cLog        ,PositionalParams,0}},
        /* 24:	(cLog [(cMul (cPow [% y]) <1>)])
         *	->	(cAdd {(cMul {LOG( % ) y}) (cLog [(cMul  <1>)])})
         */		 {1, ProduceNewTree, 1,P1(S(155))              , {1,P1(S(247))              , cLog        ,PositionalParams,0}},
        /* 25:	(cMax x@D4 x@D4)
         *	:	x
         */		 {2, ReplaceParams , 1,P1(P(7))                , {2,P2(P(8),P(8))           , cMax        ,AnyParams       ,0}},
        /* 26:	(cMin x@D4 x@D4)
         *	:	x
         */		 {2, ReplaceParams , 1,P1(P(7))                , {2,P2(P(8),P(8))           , cMin        ,AnyParams       ,0}},
        /* 27:	(cPow [(cMul % <1>) &])
         *	->	(cMul {POW( % & ) (cPow [(cMul  <1>) &])})
         */		 {2, ProduceNewTree, 1,P1(S(228))              , {2,P2(S(234),P(5))         , cPow        ,PositionalParams,0}},
        /* 28:	(cPow [% (cAdd {(cLog [x]) &})])
         *	->	(cMul {POW( % & ) (cPow [x LOG( % )])})
         */		 {2, ProduceNewTree, 1,P1(S(227))              , {2,P2(P(1),S(154))         , cPow        ,PositionalParams,0}},
        /* 29:	(cPow [z@D16 (cAdd (cMul (cPow [(cLog [z]) -1]) <2>) <1>)@D16])
         *	->	(cMul {(cPow [z (cAdd  <1>)]) (cPow [2.71828182846 (cMul  <2>)])})
         */		 {2, ProduceNewTree, 1,P1(S(225))              , {2,P2(P(18),S(172))        , cPow        ,PositionalParams,0}},
        /* 30:	(cPow [z@D16 (cAdd (cPow [(cLog [z]) -1]) <1>)@D16])
         *	->	(cMul {2.71828182846 (cPow [z (cAdd  <1>)])})
         */		 {2, ProduceNewTree, 1,P1(S(188))              , {2,P2(P(18),S(170))        , cPow        ,PositionalParams,0}},
        /* 31:	(cPow [% (cAdd &@M <1>)])
         *	->	(cMul {POW( % & ) (cPow [% (cAdd  <1>)])})
         */		 {2, ProduceNewTree, 1,P1(S(226))              , {2,P2(P(1),S(166))         , cPow        ,PositionalParams,0}},
        /* 32:	(cPow [% (cLog [x])])
         *	:	x LOG( % )
         */		 {2, ReplaceParams , 2,P2(P(7),S(39))          , {2,P2(P(1),S(33))          , cPow        ,PositionalParams,0}},
        /* 33:	(cPow [(cAbs [x]) y@E])
         *	:	x y
         */		 {2, ReplaceParams , 2,P2(P(7),P(13))          , {2,P2(S(0),P(14))          , cPow        ,PositionalParams,0}},
        /* 34:	(cPow [(cMul (cAbs [x]) <1>) y@E])
         *	:	(cMul x <1>) y
         */		 {2, ReplaceParams , 2,P2(S(238),P(13))        , {2,P2(S(242),P(14))        , cPow        ,PositionalParams,0}},
        /* 35:	(cPow [% (cMul (cLog [x]) <1>)])
         *	:	x (cMul LOG( % ) <1>)
         */		 {2, ReplaceParams , 2,P2(P(7),S(244))         , {2,P2(P(1),S(243))         , cPow        ,PositionalParams,0}},
        /* 36:	(cPow [(cPow [x@P y]) z])
         *	:	x (cMul {y z})
         */		 {2, ReplaceParams , 2,P2(P(7),S(196))         , {2,P2(S(57),P(19))         , cPow        ,PositionalParams,0}},
        /* 37:	(cPow [(cPow [x y@O]) z])
         *	:	x (cMul {y z})
         */		 {2, ReplaceParams , 2,P2(P(7),S(196))         , {2,P2(S(59),P(19))         , cPow        ,PositionalParams,0}},
        /* 38:	(cPow [(cPow [x y@F]) z])
         *	:	x (cMul {y z})
         */		 {2, ReplaceParams , 2,P2(P(7),S(196))         , {2,P2(S(60),P(19))         , cPow        ,PositionalParams,0}},
        /* 39:	(cPow [(cPow [x y])@P z])
         *	:	(cAbs [x]) (cMul {y z})
         */		 {2, ReplaceParams , 2,P2(S(0),S(196))         , {2,P2(S(61),P(19))         , cPow        ,PositionalParams,0}},
        /* 40:	(cPow [z@D16 (cMul (cPow [(cLog [z]) -1]) (cLog [x]) <1>)@D16])
         *	:	x (cMul  <1>)
         */		 {2, ReplaceParams , 2,P2(P(7),S(229))         , {2,P2(P(18),S(253))        , cPow        ,PositionalParams,0}},
        /* 41:	(cPow [%@D1 (cMul /LOG( % ) (cLog [x]) <1>)@D1])
         *	:	x (cMul  <1>)
         */		 {2, ReplaceParams , 2,P2(P(7),S(229))         , {2,P2(P(3),S(255))         , cPow        ,PositionalParams,0}},
        /* 42:	(cSin [(cMul %@N <1>)])
         *	->	(cMul {-1 (cSin [(cMul -% <1>)])})
         */		 {1, ProduceNewTree, 1,P1(S(182))              , {1,P1(S(232))              , cSin        ,PositionalParams,0}},
        /* 43:	(cSin [(cAdd {1.57079632679 (cMul %@N <1>)})])
         *	->	(cCos [(cMul -% <1>)])
         */		 {1, ProduceNewTree, 1,P1(S(15))               , {1,P1(S(133))              , cSin        ,PositionalParams,0}},
        /* 44:	(cSin [(cAdd -1.57079632679 <1>)])
         *	->	(cMul {-1 (cCos [(cAdd  <1>)])})
         */		 {1, ProduceNewTree, 1,P1(S(176))              , {1,P1(S(164))              , cSin        ,PositionalParams,0}},
        /* 45:	(cSin [(cAsin [x])])
         *	->	x
         */		 {1, ProduceNewTree, 1,P1(P(7))                , {1,P1(S(5))                , cSin        ,PositionalParams,0}},
        /* 46:	(cAdd (cMul (cPow [x %]) <1>)@D4 (cMul (cPow [x &]) <2>)@D4)
         *	:	(cMul {(cPow [x MIN( % & )]) (cAdd {(cMul (cPow [x (cAdd {% -MIN( % & )})]) <1>) (cMul (cPow [x (cAdd {& -MIN( % & )})]) <2>)})})
         */		 {2, ReplaceParams , 1,P1(S(224))              , {2,P2(S(245),S(246))       , cAdd        ,AnyParams       ,0}},
        /* 47:	(cAdd (cMul (cPow [x %]) <1>)@D4 (cMul x <2>)@D4)
         *	:	(cMul {(cPow [x MIN( % 1 )]) (cAdd {(cMul (cPow [x (cAdd {% -MIN( % 1 )})]) <1>) (cMul (cPow [x (cAdd {1 -MIN( % 1 )})]) <2>)})})
         */		 {2, ReplaceParams , 1,P1(S(223))              , {2,P2(S(245),S(237))       , cAdd        ,AnyParams       ,0}},
        /* 48:	(cAdd (cPow [(cSin [x]) 2])@D4 (cPow [(cCos [x]) 2])@D4)
         *	:	1
         */		 {2, ReplaceParams , 1,P1(N(8))                , {2,P2(S(94),S(91))         , cAdd        ,AnyParams       ,0}},
        /* 49:	(cAdd 1 (cMul {-1 (cPow [(cSin [x]) 2])}))
         *	:	(cPow [(cCos [x]) 2])
         */		 {2, ReplaceParams , 1,P1(S(92))               , {2,P2(N(8),S(180))         , cAdd        ,AnyParams       ,0}},
        /* 50:	(cAdd 1 (cMul {-1 (cPow [(cCos [x]) 2])}))
         *	:	(cPow [(cSin [x]) 2])
         */		 {2, ReplaceParams , 1,P1(S(95))               , {2,P2(N(8),S(179))         , cAdd        ,AnyParams       ,0}},
        /* 51:	(cAdd (cMul {(cSin [x]) (cCos [y])})@D12 (cMul {(cCos [x]) (cSin [y])})@D12)
         *	:	(cSin [(cAdd {x y})])
         */		 {2, ReplaceParams , 1,P1(S(105))              , {2,P2(S(221),S(215))       , cAdd        ,AnyParams       ,0}},
        /* 52:	(cAdd (cMul {(cSin [x]) (cCos [y])})@D12 (cMul {(cCos [x]) (cSin [y]) -1})@D12)
         *	:	(cSin [(cAdd {x (cMul {-1 y})})])
         */		 {2, ReplaceParams , 1,P1(S(106))              , {2,P2(S(221),S(219))       , cAdd        ,AnyParams       ,0}},
        /* 53:	(cAdd (cMul {(cCos [x]) (cCos [y])})@D12 (cMul {(cSin [x]) (cSin [y])})@D12)
         *	:	(cCos [(cAdd {x y})])
         */		 {2, ReplaceParams , 1,P1(S(12))               , {2,P2(S(212),S(218))       , cAdd        ,AnyParams       ,0}},
        /* 54:	(cAdd (cMul {(cCos [x]) (cCos [y]) -1})@D12 (cMul {(cSin [x]) (cSin [y])})@D12)
         *	:	(cMul {-1 (cCos [(cAdd {x (cMul {-1 y})})])})
         */		 {2, ReplaceParams , 1,P1(S(175))              , {2,P2(S(217),S(218))       , cAdd        ,AnyParams       ,0}},
        /* 55:	(cAdd (cMul {(cCos [x]) (cCos [y])})@D12 (cMul {(cSin [x]) (cSin [y]) -1})@D12)
         *	:	(cCos [(cAdd {x (cMul {-1 y})})])
         */		 {2, ReplaceParams , 1,P1(S(13))               , {2,P2(S(212),S(222))       , cAdd        ,AnyParams       ,0}},
        /* 56:	(cAdd (cLog [x]) (cLog [y]))
         *	:	(cLog [(cMul {x y})])
         */		 {2, ReplaceParams , 1,P1(S(37))               , {2,P2(S(33),S(34))         , cAdd        ,AnyParams       ,0}},
        /* 57:	(cAdd (cPow [x 2])@D4 (cPow [y 2])@D8 (cMul x y <1>)@D12)
         *	:	(cPow [(cAdd {x y}) 2]) (cMul {x y (cAdd {(cMul  <1>) -2})})
         */		 {3, ReplaceParams , 2,P2(S(96),S(195))        , {3,P3(S(51),S(53),S(241))  , cAdd        ,AnyParams       ,0}},
        /* 58:	(cMul {%@D1 (cAdd {1 (cMul {(cLog [x]) /%})})@D1})
         *	->	(cAdd {(cLog [x]) %})
         */		 {2, ProduceNewTree, 1,P1(S(152))              , {2,P2(P(3),S(132))         , cMul        ,SelectedParams  ,0}},
        /* 59:	(cMul (cAdd (cMul %@M <1>) <2>) &)
         *	:	(cAdd {(cMul % & <1>) (cMul {& (cAdd  <2>)})})
         */		 {2, ReplaceParams , 1,P1(S(157))              , {2,P2(S(171),P(5))         , cMul        ,AnyParams       ,0}},
        /* 60:	(cMul (cAdd % <1>) &)
         *	:	(cAdd {MUL( % & ) (cMul {& (cAdd  <1>)})})
         */		 {2, ReplaceParams , 1,P1(S(158))              , {2,P2(S(165),P(5))         , cMul        ,AnyParams       ,0}},
        /* 61:	(cMul (cAbs [x]) (cAbs [y]))
         *	:	(cAbs [(cMul {x y})])
         */		 {2, ReplaceParams , 1,P1(S(2))                , {2,P2(S(0),S(1))           , cMul        ,AnyParams       ,0}},
        /* 62:	(cMul (cPow [(cMul x <2>) -1])@D4 x@D4)
         *	:	(cPow [(cMul  <2>) -1])
         */		 {2, ReplaceParams , 1,P1(S(89))               , {2,P2(S(90),P(8))          , cMul        ,AnyParams       ,0}},
        /* 63:	(cMul (cNot [x]) (cNotNot [y]))
         *	:	(cAnd {(cNot [x]) y})
         */		 {2, ReplaceParams , 1,P1(S(269))              , {2,P2(S(262),S(286))       , cMul        ,AnyParams       ,0}},
        /* 64:	(cMul (cNotNot [x]) (cNotNot [y]))
         *	:	(cAnd {x y})
         */		 {2, ReplaceParams , 1,P1(S(265))              , {2,P2(S(285),S(286))       , cMul        ,AnyParams       ,0}},
        /* 65:	(cMul (cNot [x]) (cNot [y]))
         *	:	(cNot [(cOr {x y})])
         */		 {2, ReplaceParams , 1,P1(S(264))              , {2,P2(S(262),S(263))       , cMul        ,AnyParams       ,0}},
        /* 66:	(cNot [(cMul {-1 x})])
         *	:	x
         */		 {1, ReplaceParams , 1,P1(P(7))                , {1,P1(S(173))              , cNot        ,PositionalParams,0}},
        /* 67:	(cAnd (cEqual [x y])@D12 (cEqual [y z])@D24 (cEqual [x z])@D20)
         *	:	(cEqual [x y]) (cEqual [y z])
         */		 {3, ReplaceParams , 2,P2(S(258),S(261))       , {3,P3(S(257),S(260),S(259)), cAnd        ,AnyParams       ,0}},
        /*********************/
        /* 68:	(cAtan2 [(cMul x <1>)@D4 (cMul x <2>)@D4])
         *	:	(cMul  <1>) (cMul  <2>)
         */		 {2, ReplaceParams , 2,P2(S(229),S(230))       , {2,P2(S(236),S(237))       , cAtan2      ,PositionalParams,0}},
        /* 69:	(cCosh [(cAsinh [x])])
         *	->	(cPow [(cAdd {(cPow [x 2]) 1}) 0.5])
         */		 {1, ProduceNewTree, 1,P1(S(88))               , {1,P1(S(6))                , cCosh       ,PositionalParams,0}},
        /* 70:	(cCosh [(cMul -1 <1>)])
         *	:	(cMul  <1>)
         */		 {1, ReplaceParams , 1,P1(S(229))              , {1,P1(S(231))              , cCosh       ,PositionalParams,0}},
        /* 71:	(cIf [y (cMul x <1>)@D4 (cMul x <2>)@D4])
         *	->	(cMul {x (cIf [y (cMul  <1>) (cMul  <2>)])})
         */		 {3, ProduceNewTree, 1,P1(S(199))              , {3,P3(P(13),S(236),S(237)) , cIf         ,PositionalParams,0}},
        /* 72:	(cIf [y x@D4 x@D4])
         *	->	x
         */		 {3, ProduceNewTree, 1,P1(P(7))                , {3,P3(P(13),P(8),P(8))     , cIf         ,PositionalParams,0}},
        /* 73:	(cIf [y (cAdd x <1>)@D4 x@D4])
         *	->	(cAdd {x (cIf [y (cAdd  <1>) 0])})
         */		 {3, ProduceNewTree, 1,P1(S(139))              , {3,P3(P(13),S(167),P(8))   , cIf         ,PositionalParams,0}},
        /* 74:	(cIf [y (cMul x <1>)@D4 x@D4])
         *	->	(cMul {x (cIf [y (cMul  <1>) 1])})
         */		 {3, ProduceNewTree, 1,P1(S(198))              , {3,P3(P(13),S(236),P(8))   , cIf         ,PositionalParams,0}},
        /* 75:	(cIf [y x@D4 (cAdd x <1>)@D4])
         *	->	(cAdd {x (cIf [y 0 (cAdd  <1>)])})
         */		 {3, ProduceNewTree, 1,P1(S(138))              , {3,P3(P(13),P(8),S(167))   , cIf         ,PositionalParams,0}},
        /* 76:	(cIf [y (cAdd x <1>)@D4 (cAdd x <2>)@D4])
         *	->	(cAdd {x (cIf [y (cAdd  <1>) (cAdd  <2>)])})
         */		 {3, ProduceNewTree, 1,P1(S(140))              , {3,P3(P(13),S(167),S(168)) , cIf         ,PositionalParams,0}},
        /* 77:	(cIf [y x@D4 (cMul x <1>)@D4])
         *	->	(cMul {x (cIf [y 1 (cMul  <1>)])})
         */		 {3, ProduceNewTree, 1,P1(S(197))              , {3,P3(P(13),P(8),S(236))   , cIf         ,PositionalParams,0}},
        /* 78:	(cIf [y (cNotNot [x])@D4 (cAnd x <1>)@D4])
         *	->	(cAnd {x (cIf [y 1 (cAnd  <1>)])})
         */		 {3, ProduceNewTree, 1,P1(S(266))              , {3,P3(P(13),S(284),S(272)) , cIf         ,PositionalParams,0}},
        /* 79:	(cIf [y (cAnd x <1>)@D4 (cNotNot [x])@D4])
         *	->	(cAnd {x (cIf [y (cAnd  <1>) 1])})
         */		 {3, ProduceNewTree, 1,P1(S(267))              , {3,P3(P(13),S(272),S(284)) , cIf         ,PositionalParams,0}},
        /* 80:	(cIf [y (cOr x <1>)@D4 (cNotNot [x])@D4])
         *	->	(cOr {x (cIf [y (cOr  <1>) 0])})
         */		 {3, ProduceNewTree, 1,P1(S(277))              , {3,P3(P(13),S(281),S(284)) , cIf         ,PositionalParams,0}},
        /* 81:	(cIf [y x@L@D4 (cAnd x@L <1>)@D4])
         *	->	(cAnd {x (cIf [y 1 (cAnd  <1>)])})
         */		 {3, ProduceNewTree, 1,P1(S(266))              , {3,P3(P(13),P(9),S(273))   , cIf         ,PositionalParams,0}},
        /* 82:	(cIf [y (cAnd x@L <1>)@D4 x@L@D4])
         *	->	(cAnd {x (cIf [y (cAnd  <1>) 1])})
         */		 {3, ProduceNewTree, 1,P1(S(267))              , {3,P3(P(13),S(273),P(9))   , cIf         ,PositionalParams,0}},
        /* 83:	(cIf [y (cOr x@L <1>)@D4 x@L@D4])
         *	->	(cOr {x (cIf [y (cOr  <1>) 0])})
         */		 {3, ProduceNewTree, 1,P1(S(277))              , {3,P3(P(13),S(282),P(9))   , cIf         ,PositionalParams,0}},
        /* 84:	(cIf [y (cAnd x <1>)@D4 (cAnd x <2>)@D4])
         *	->	(cAnd {x (cIf [y (cAnd  <1>) (cAnd  <2>)])})
         */		 {3, ProduceNewTree, 1,P1(S(268))              , {3,P3(P(13),S(272),S(274)) , cIf         ,PositionalParams,0}},
        /* 85:	(cIf [y (cNotNot [x])@D4 (cOr x <1>)@D4])
         *	->	(cOr {x (cIf [y 0 (cOr  <1>)])})
         */		 {3, ProduceNewTree, 1,P1(S(276))              , {3,P3(P(13),S(284),S(281)) , cIf         ,PositionalParams,0}},
        /* 86:	(cIf [y x@L@D4 (cOr x@L <1>)@D4])
         *	->	(cOr {x (cIf [y 0 (cOr  <1>)])})
         */		 {3, ProduceNewTree, 1,P1(S(276))              , {3,P3(P(13),P(9),S(282))   , cIf         ,PositionalParams,0}},
        /* 87:	(cIf [y (cOr x <1>)@D4 (cOr x <2>)@D4])
         *	->	(cOr {x (cIf [y (cOr  <1>) (cOr  <2>)])})
         */		 {3, ProduceNewTree, 1,P1(S(278))              , {3,P3(P(13),S(281),S(283)) , cIf         ,PositionalParams,0}},
        /* 88:	(cLog [(cPow [x@P y])])
         *	->	(cMul {y (cLog [x])})
         */		 {1, ProduceNewTree, 1,P1(S(200))              , {1,P1(S(57))               , cLog        ,PositionalParams,0}},
        /* 89:	(cLog [(cPow [x y@E])])
         *	->	(cMul {y (cLog [(cAbs [x])])})
         */		 {1, ProduceNewTree, 1,P1(S(201))              , {1,P1(S(58))               , cLog        ,PositionalParams,0}},
        /* 90:	(cLog [(cMul %@P <1>)])
         *	->	(cAdd {(cLog [(cMul  <1>)]) LOG( % )})
         */		 {1, ProduceNewTree, 1,P1(S(156))              , {1,P1(S(233))              , cLog        ,PositionalParams,0}},
        /* 91:	(cLog [(cMul (cPow [% y]) <1>)])
         *	->	(cAdd {(cMul {LOG( % ) y}) (cLog [(cMul  <1>)])})
         */		 {1, ProduceNewTree, 1,P1(S(155))              , {1,P1(S(247))              , cLog        ,PositionalParams,0}},
        /* 92:	(cLog [(cMul {(cAdd {1 x})@D4 (cPow [(cAdd {1 (cMul {-1 x})}) -1])@D4})])
         *	->	(cMul {(cAtanh [x]) 2})
         */		 {1, ProduceNewTree, 1,P1(S(206))              , {1,P1(S(214))              , cLog        ,PositionalParams,0}},
        /* 93:	(cMax x@D4 x@D4)
         *	:	x
         */		 {2, ReplaceParams , 1,P1(P(7))                , {2,P2(P(8),P(8))           , cMax        ,AnyParams       ,0}},
        /* 94:	(cMin x@D4 x@D4)
         *	:	x
         */		 {2, ReplaceParams , 1,P1(P(7))                , {2,P2(P(8),P(8))           , cMin        ,AnyParams       ,0}},
        /* 95:	(cPow [(cMul % <1>) &])
         *	->	(cMul {POW( % & ) (cPow [(cMul  <1>) &])})
         */		 {2, ProduceNewTree, 1,P1(S(228))              , {2,P2(S(234),P(5))         , cPow        ,PositionalParams,0}},
        /* 96:	(cPow [% (cAdd {(cLog [x]) &})])
         *	->	(cMul {POW( % & ) (cPow [x LOG( % )])})
         */		 {2, ProduceNewTree, 1,P1(S(227))              , {2,P2(P(1),S(154))         , cPow        ,PositionalParams,0}},
        /* 97:	(cPow [z@D16 (cAdd (cMul (cPow [(cLog [z]) -1]) <2>) <1>)@D16])
         *	->	(cMul {(cPow [z (cAdd  <1>)]) (cPow [2.71828182846 (cMul  <2>)])})
         */		 {2, ProduceNewTree, 1,P1(S(225))              , {2,P2(P(18),S(172))        , cPow        ,PositionalParams,0}},
        /* 98:	(cPow [z@D16 (cAdd (cPow [(cLog [z]) -1]) <1>)@D16])
         *	->	(cMul {2.71828182846 (cPow [z (cAdd  <1>)])})
         */		 {2, ProduceNewTree, 1,P1(S(188))              , {2,P2(P(18),S(170))        , cPow        ,PositionalParams,0}},
        /* 99:	(cPow [% (cAdd &@M <1>)])
         *	->	(cMul {POW( % & ) (cPow [% (cAdd  <1>)])})
         */		 {2, ProduceNewTree, 1,P1(S(226))              , {2,P2(P(1),S(166))         , cPow        ,PositionalParams,0}},
        /* 100:	(cPow [% (cLog [x])])
         *	:	x LOG( % )
         */		 {2, ReplaceParams , 2,P2(P(7),S(39))          , {2,P2(P(1),S(33))          , cPow        ,PositionalParams,0}},
        /* 101:	(cPow [(cAbs [x]) y@E])
         *	:	x y
         */		 {2, ReplaceParams , 2,P2(P(7),P(13))          , {2,P2(S(0),P(14))          , cPow        ,PositionalParams,0}},
        /* 102:	(cPow [(cMul (cAbs [x]) <1>) y@E])
         *	:	(cMul x <1>) y
         */		 {2, ReplaceParams , 2,P2(S(238),P(13))        , {2,P2(S(242),P(14))        , cPow        ,PositionalParams,0}},
        /* 103:	(cPow [% (cMul (cLog [x]) <1>)])
         *	:	x (cMul LOG( % ) <1>)
         */		 {2, ReplaceParams , 2,P2(P(7),S(244))         , {2,P2(P(1),S(243))         , cPow        ,PositionalParams,0}},
        /* 104:	(cPow [(cPow [x@P y]) z])
         *	:	x (cMul {y z})
         */		 {2, ReplaceParams , 2,P2(P(7),S(196))         , {2,P2(S(57),P(19))         , cPow        ,PositionalParams,0}},
        /* 105:	(cPow [(cPow [x y@O]) z])
         *	:	x (cMul {y z})
         */		 {2, ReplaceParams , 2,P2(P(7),S(196))         , {2,P2(S(59),P(19))         , cPow        ,PositionalParams,0}},
        /* 106:	(cPow [(cPow [x y@F]) z])
         *	:	x (cMul {y z})
         */		 {2, ReplaceParams , 2,P2(P(7),S(196))         , {2,P2(S(60),P(19))         , cPow        ,PositionalParams,0}},
        /* 107:	(cPow [(cPow [x y])@P z])
         *	:	(cAbs [x]) (cMul {y z})
         */		 {2, ReplaceParams , 2,P2(S(0),S(196))         , {2,P2(S(61),P(19))         , cPow        ,PositionalParams,0}},
        /* 108:	(cPow [z@D16 (cMul (cPow [(cLog [z]) -1]) (cLog [x]) <1>)@D16])
         *	:	x (cMul  <1>)
         */		 {2, ReplaceParams , 2,P2(P(7),S(229))         , {2,P2(P(18),S(253))        , cPow        ,PositionalParams,0}},
        /* 109:	(cPow [%@D1 (cMul /LOG( % ) (cLog [x]) <1>)@D1])
         *	:	x (cMul  <1>)
         */		 {2, ReplaceParams , 2,P2(P(7),S(229))         , {2,P2(P(3),S(255))         , cPow        ,PositionalParams,0}},
        /* 110:	(cSinh [(cMul -1 <1>)])
         *	->	(cMul {-1 (cSinh [(cMul  <1>)])})
         */		 {1, ProduceNewTree, 1,P1(S(184))              , {1,P1(S(231))              , cSinh       ,PositionalParams,0}},
        /* 111:	(cSinh [(cAcosh [x])])
         *	->	(cPow [(cAdd {(cPow [x 2]) -1}) 0.5])
         */		 {1, ProduceNewTree, 1,P1(S(87))               , {1,P1(S(4))                , cSinh       ,PositionalParams,0}},
        /* 112:	(cTan [(cMul -1 <1>)])
         *	->	(cMul {-1 (cTan [(cMul  <1>)])})
         */		 {1, ProduceNewTree, 1,P1(S(185))              , {1,P1(S(231))              , cTan        ,PositionalParams,0}},
        /* 113:	(cTan [(cAtan [x])])
         *	->	x
         */		 {1, ProduceNewTree, 1,P1(P(7))                , {1,P1(S(7))                , cTan        ,PositionalParams,0}},
        /* 114:	(cTanh [(cMul -1 <1>)])
         *	->	(cMul {-1 (cTanh [(cMul  <1>)])})
         */		 {1, ProduceNewTree, 1,P1(S(187))              , {1,P1(S(231))              , cTanh       ,PositionalParams,0}},
        /* 115:	(cAdd (cLog [x]) (cLog [y]))
         *	:	(cLog [(cMul {x y})])
         */		 {2, ReplaceParams , 1,P1(S(37))               , {2,P2(S(33),S(34))         , cAdd        ,AnyParams       ,0}},
        /* 116:	(cAdd (cCosh [x])@D4 (cSinh [x])@D4)
         *	:	(cPow [2.71828182846 x])
         */		 {2, ReplaceParams , 1,P1(S(45))               , {2,P2(S(16),S(110))        , cAdd        ,AnyParams       ,0}},
        /* 117:	(cAdd (cMul {(cCosh [x]) -1})@D4 (cSinh [x])@D4)
         *	:	(cMul {(cPow [0.367879441171 x]) -1})
         */		 {2, ReplaceParams , 1,P1(S(203))              , {2,P2(S(202),S(110))       , cAdd        ,AnyParams       ,0}},
        /* 118:	(cAdd (cPow [& x])@D6 (cMul {-1 (cPow [/& x])})@D6)
         *	:	(cMul {(cSinh [(cMul {x LOG( & )})]) 2})
         */		 {2, ReplaceParams , 1,P1(S(210))              , {2,P2(S(49),S(181))        , cAdd        ,AnyParams       ,0}},
        /* 119:	(cAdd (cPow [& x])@D6 (cPow [/& x])@D6)
         *	:	(cMul {(cCosh [(cMul {x LOG( & )})]) 2})
         */		 {2, ReplaceParams , 1,P1(S(209))              , {2,P2(S(49),S(99))         , cAdd        ,AnyParams       ,0}},
        /* 120:	(cAdd (cMul {-1 (cPow [& x])})@D6 (cPow [/& x])@D6)
         *	:	(cMul {(cSinh [(cMul {x LOG( & )})]) -2})
         */		 {2, ReplaceParams , 1,P1(S(208))              , {2,P2(S(178),S(99))        , cAdd        ,AnyParams       ,0}},
        /* 121:	(cAdd (cMul {% (cPow [& x])})@D7 (cMul {-% (cPow [/& x])})@D7)
         *	:	(cMul {(cSinh [(cMul {x LOG( & )})]) 2 %})
         */		 {2, ReplaceParams , 1,P1(S(211))              , {2,P2(S(189),S(213))       , cAdd        ,AnyParams       ,0}},
        /* 122:	(cAdd (cMul {% (cPow [& x])})@D7 (cMul {% (cPow [/& x])})@D7)
         *	:	(cMul {(cCosh [(cMul {x LOG( & )})]) 2 %})
         */		 {2, ReplaceParams , 1,P1(S(207))              , {2,P2(S(189),S(190))       , cAdd        ,AnyParams       ,0}},
        /* 123:	(cAdd (cCosh [x])@D4 (cMul {(cSinh [x]) -1})@D4)
         *	:	(cPow [0.367879441171 x])
         */		 {2, ReplaceParams , 1,P1(S(44))               , {2,P2(S(16),S(205))        , cAdd        ,AnyParams       ,0}},
        /* 124:	(cAdd (cSinh [x])@D4 (cMul {(cPow [2.71828182846 x]) -1})@D4)
         *	:	(cMul {-1 (cCosh [x])})
         */		 {2, ReplaceParams , 1,P1(S(177))              , {2,P2(S(110),S(204))       , cAdd        ,AnyParams       ,0}},
        /* 125:	(cAdd (cCosh [x])@D4 (cMul {(cPow [2.71828182846 x]) -1})@D4)
         *	:	(cMul {-1 (cSinh [x])})
         */		 {2, ReplaceParams , 1,P1(S(183))              , {2,P2(S(16),S(204))        , cAdd        ,AnyParams       ,0}},
        /* 126:	(cAdd (cMul {(cSinh [x]) -1})@D4 (cPow [2.71828182846 x])@D4)
         *	:	(cCosh [x])
         */		 {2, ReplaceParams , 1,P1(S(17))               , {2,P2(S(205),S(46))        , cAdd        ,AnyParams       ,0}},
        /* 127:	(cAdd (cMul {(cCosh [x]) -1})@D4 (cPow [2.71828182846 x])@D4)
         *	:	(cSinh [x])
         */		 {2, ReplaceParams , 1,P1(S(111))              , {2,P2(S(202),S(46))        , cAdd        ,AnyParams       ,0}},
        /* 128:	(cMul {%@D1 (cAdd {1 (cMul {(cLog [x]) /%})})@D1})
         *	->	(cAdd {(cLog [x]) %})
         */		 {2, ProduceNewTree, 1,P1(S(152))              , {2,P2(P(3),S(132))         , cMul        ,SelectedParams  ,0}},
        /* 129:	(cMul (cPow [(cMul x <2>) -1])@D4 x@D4)
         *	:	(cPow [(cMul  <2>) -1])
         */		 {2, ReplaceParams , 1,P1(S(89))               , {2,P2(S(90),P(8))          , cMul        ,AnyParams       ,0}},
        /* 130:	(cMul (cNot [x]) (cNotNot [y]))
         *	:	(cAnd {(cNot [x]) y})
         */		 {2, ReplaceParams , 1,P1(S(269))              , {2,P2(S(262),S(286))       , cMul        ,AnyParams       ,0}},
        /* 131:	(cMul (cNotNot [x]) (cNotNot [y]))
         *	:	(cAnd {x y})
         */		 {2, ReplaceParams , 1,P1(S(265))              , {2,P2(S(285),S(286))       , cMul        ,AnyParams       ,0}},
        /* 132:	(cMul (cNot [x]) (cNot [y]))
         *	:	(cNot [(cOr {x y})])
         */		 {2, ReplaceParams , 1,P1(S(264))              , {2,P2(S(262),S(263))       , cMul        ,AnyParams       ,0}},
        /* 133:	(cMul (cPow [(cTanh [x]) -1])@D4 (cSinh [x])@D4)
         *	:	(cCosh [x])
         */		 {2, ReplaceParams , 1,P1(S(17))               , {2,P2(S(79),S(110))        , cMul        ,AnyParams       ,0}},
        /* 134:	(cMul (cSinh [x])@D4 (cPow [(cCosh [x]) -1])@D4)
         *	:	(cTanh [x])
         */		 {2, ReplaceParams , 1,P1(S(121))              , {2,P2(S(110),S(73))        , cMul        ,AnyParams       ,0}},
        /* 135:	(cMul (cTanh [x])@D4 (cPow [(cSinh [x]) -1])@D4)
         *	:	(cPow [(cCosh [x]) -1])
         */		 {2, ReplaceParams , 1,P1(S(74))               , {2,P2(S(122),S(76))        , cMul        ,AnyParams       ,0}},
        /* 136:	(cMul (cPow [(cSinh [x]) -1])@D4 (cCosh [x])@D4)
         *	:	(cPow [(cTanh [x]) -1])
         */		 {2, ReplaceParams , 1,P1(S(80))               , {2,P2(S(76),S(16))         , cMul        ,AnyParams       ,0}},
        /* 137:	(cMul (cTanh [x])@D4 (cCosh [x])@D4)
         *	:	(cSinh [x])
         */		 {2, ReplaceParams , 1,P1(S(111))              , {2,P2(S(122),S(16))        , cMul        ,AnyParams       ,0}},
        /* 138:	(cMul (cPow [(cTan [x]) -1])@D4 (cSin [x])@D4)
         *	:	(cCos [x])
         */		 {2, ReplaceParams , 1,P1(S(9))                , {2,P2(S(77),S(103))        , cMul        ,AnyParams       ,0}},
        /* 139:	(cMul (cSin [x])@D4 (cPow [(cCos [x]) -1])@D4)
         *	:	(cTan [x])
         */		 {2, ReplaceParams , 1,P1(S(116))              , {2,P2(S(103),S(71))        , cMul        ,AnyParams       ,0}},
        /* 140:	(cMul (cTan [x])@D4 (cPow [(cSin [x]) -1])@D4)
         *	:	(cPow [(cCos [x]) -1])
         */		 {2, ReplaceParams , 1,P1(S(72))               , {2,P2(S(117),S(75))        , cMul        ,AnyParams       ,0}},
        /* 141:	(cMul (cPow [(cSin [x]) -1])@D4 (cCos [x])@D4)
         *	:	(cPow [(cTan [x]) -1])
         */		 {2, ReplaceParams , 1,P1(S(78))               , {2,P2(S(75),S(10))         , cMul        ,AnyParams       ,0}},
        /* 142:	(cMul (cTan [x])@D4 (cCos [x])@D4)
         *	:	(cSin [x])
         */		 {2, ReplaceParams , 1,P1(S(102))              , {2,P2(S(117),S(10))        , cMul        ,AnyParams       ,0}},
        /* 143:	(cMul -1 (cSinh [(cMul % <1>)]))
         *	:	(cSinh [(cMul -% <1>)])
         */		 {2, ReplaceParams , 1,P1(S(115))              , {2,P2(N(2),S(114))         , cMul        ,AnyParams       ,0}},
        /* 144:	(cMul -1 (cTanh [(cMul % <1>)]))
         *	:	(cTanh [(cMul -% <1>)])
         */		 {2, ReplaceParams , 1,P1(S(125))              , {2,P2(N(2),S(124))         , cMul        ,AnyParams       ,0}},
        /* 145:	(cMul -1 (cSin [(cMul % <1>)]))
         *	:	(cSin [(cMul -% <1>)])
         */		 {2, ReplaceParams , 1,P1(S(109))              , {2,P2(N(2),S(108))         , cMul        ,AnyParams       ,0}},
        /* 146:	(cMul -1 (cTan [(cMul % <1>)]))
         *	:	(cTan [(cMul -% <1>)])
         */		 {2, ReplaceParams , 1,P1(S(120))              , {2,P2(N(2),S(119))         , cMul        ,AnyParams       ,0}},
        /* 147:	(cMul (cAdd {(cPow [0.135335283237 x]) -1})@D4 (cPow [(cAdd {(cPow [0.135335283237 x]) 1}) -1])@D4)
         *	:	-1 (cTanh [x])
         */		 {2, ReplaceParams , 2,P2(N(2),S(121))         , {2,P2(S(142),S(84))        , cMul        ,AnyParams       ,0}},
        /* 148:	(cMul (cAdd {(cPow [7.38905609893 x]) -1})@D4 (cPow [(cAdd {(cPow [7.38905609893 x]) 1}) -1])@D4)
         *	:	(cTanh [x])
         */		 {2, ReplaceParams , 1,P1(S(121))              , {2,P2(S(144),S(85))        , cMul        ,AnyParams       ,0}},
        /* 149:	(cMul (cAdd {(cPow [0.135335283237 x]) 1})@D4 (cPow [(cAdd {(cPow [0.135335283237 x]) -1}) -1])@D4)
         *	:	(cPow [(cMul {-1 (cTanh [x])}) -1])
         */		 {2, ReplaceParams , 1,P1(S(86))               , {2,P2(S(147),S(82))        , cMul        ,AnyParams       ,0}},
        /* 150:	(cMul (cAdd {(cPow [7.38905609893 x]) 1})@D4 (cPow [(cAdd {(cPow [7.38905609893 x]) -1}) -1])@D4)
         *	:	(cPow [(cTanh [x]) -1])
         */		 {2, ReplaceParams , 1,P1(S(80))               , {2,P2(S(149),S(83))        , cMul        ,AnyParams       ,0}},
        /* 151:	(cNot [(cMul {-1 x})])
         *	:	x
         */		 {1, ReplaceParams , 1,P1(P(7))                , {1,P1(S(173))              , cNot        ,PositionalParams,0}},
        /* 152:	(cAnd (cEqual [x y])@D12 (cEqual [y z])@D24 (cEqual [x z])@D20)
         *	:	(cEqual [x y]) (cEqual [y z])
         */		 {3, ReplaceParams , 2,P2(S(258),S(261))       , {3,P3(S(257),S(260),S(259)), cAnd        ,AnyParams       ,0}},
        /*********************/
        /* 153:	(cPow [(cSin [x]) %@N])
         *	:	(cCsc [x]) -%
         */		 {2, ReplaceParams , 2,P2(S(20),S(126))        , {2,P2(S(102),P(0))         , cPow        ,PositionalParams,0}},
        /* 154:	(cPow [(cCos [x]) %@N])
         *	:	(cSec [x]) -%
         */		 {2, ReplaceParams , 2,P2(S(101),S(126))       , {2,P2(S(9),P(0))           , cPow        ,PositionalParams,0}},
        /* 155:	(cPow [(cTan [x]) %@N])
         *	:	(cCot [x]) -%
         */		 {2, ReplaceParams , 2,P2(S(19),S(126))        , {2,P2(S(116),P(0))         , cPow        ,PositionalParams,0}},
        /* 156:	(cMul 57.2957795131 <1>)
         *	->	(cDeg [(cMul  <1>)])
         */		 {1, ProduceNewTree, 1,P1(S(287))              , {1,P1(N(13))               , cMul        ,AnyParams       ,1}},
        /* 157:	(cMul 0.0174532925199 <1>)
         *	->	(cRad [(cMul  <1>)])
         */		 {1, ProduceNewTree, 1,P1(S(288))              , {1,P1(N(4))                , cMul        ,AnyParams       ,1}},
        /* 158:	(cAnd x@L y)
         *	:	(cMul {x y})
         */		 {2, ReplaceParams , 1,P1(S(194))              , {2,P2(P(10),P(13))         , cAnd        ,AnyParams       ,0}},
        /* 159:	(cOr x@L y@L)
         *	:	(cAdd {x y})
         */		 {2, ReplaceParams , 1,P1(S(134))              , {2,P2(P(10),P(17))         , cOr         ,AnyParams       ,0}},
        /* 160:	(cOr x@L (cAdd  <1>)@P)
         *	:	(cAdd x <1>)
         */		 {2, ReplaceParams , 1,P1(S(169))              , {2,P2(P(10),S(163))        , cOr         ,AnyParams       ,0}},
    };
}

namespace FPoptimizer_Grammar
{
    const GrammarPack pack =
    {
        {
            /* 0 */	{ &rlist[0], 68 },
            /* 1 */	{ &rlist[68], 85 },
            /* 2 */	{ &rlist[153], 8 },
        }
    };
}
#undef P
#undef N
#undef S
#undef P1
#undef P2
#undef P3

namespace FPoptimizer_Grammar
{
    ParamSpec ParamSpec_Extract(unsigned paramlist, unsigned index)
    {
        index = (paramlist >> (index * PARAM_INDEX_BITS)) % (1 << PARAM_INDEX_BITS);
        const unsigned p_begin = 0;
        const unsigned n_begin = p_begin + sizeof(plist.plist_p)/sizeof(*plist.plist_p);
        const unsigned s_begin = n_begin + sizeof(plist.plist_n)/sizeof(*plist.plist_n);
      /*const unsigned     end = s_begin + sizeof(plist.plist_s)/sizeof(*plist.plist_s);*/
        if(index < s_begin)
        {
            if(index < n_begin)
                return ParamSpec(ParamHolder,(const void*)&plist.plist_p[index-p_begin]);
            else
                return ParamSpec(NumConstant,(const void*)&plist.plist_n[index-n_begin]);
        }
        else
            return ParamSpec(SubFunction,(const void*)&plist.plist_s[index-s_begin]);
    }
}

#line 1 "fpoptimizer/fpoptimizer_optimize.cc"
// line removed
// line removed
// line removed
// line removed

#include <stdio.h>

#include <algorithm>
#include <map>
#include <sstream>

#include "fpconfig.hh"
#include "fparser.hh"
#include "fptypes.hh"

#ifdef FP_SUPPORT_OPTIMIZER

using namespace FUNCTIONPARSERTYPES;
using namespace FPoptimizer_Grammar;
using namespace FPoptimizer_CodeTree;
using namespace FPoptimizer_Optimize;

namespace
{
    /* I have heard that std::equal_range() is practically worthless
     * due to the insane limitation that the two parameters for Comp() must
     * be of the same type. Hence we must reinvent the wheel and implement
     * our own here. This is practically identical to the one from
     * GNU libstdc++, except rewritten. -Bisqwit
     */
    template<typename It, typename T, typename Comp>
    std::pair<It, It>
    MyEqualRange(It first, It last, const T& val, Comp comp)
    {
        size_t len = last-first;
        while(len > 0)
        {
            size_t half = len/2;
            It middle(first); middle += half;
            if(comp(*middle, val))
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else if(comp(val, *middle))
            {
                len = half;
            }
            else
            {
                // The following implements this:
                // // left = lower_bound(first, middle, val, comp);
                It left(first);
              {///
                It& first2 = left;
                It last2(middle);
                size_t len2 = last2-first2;
                while(len2 > 0)
                {
                    size_t half2 = len2 / 2;
                    It middle2(first2); middle2 += half2;
                    if(comp(*middle2, val))
                    {
                        first2 = middle2;
                        ++first2;
                        len2 = len2 - half2 - 1;
                    }
                    else
                        len2 = half2;
                }
                // left = first2;  - not needed, already happens due to reference
              }///
                first += len;
                // The following implements this:
                // // right = upper_bound(++middle, first, val, comp);
                It right(++middle);
              {///
                It& first2 = right;
                It& last2 = first;
                size_t len2 = last2-first2;
                while(len2 > 0)
                {
                    size_t half2 = len2 / 2;
                    It middle2(first2); middle2 += half2;
                    if(comp(val, *middle2))
                        len2 = half2;
                    else
                    {
                        first2 = middle2;
                        ++first2;
                        len2 = len2 - half2 - 1;
                    }
                }
                // right = first2;  - not needed, already happens due to reference
              }///
                return std::pair<It,It> (left,right);
            }
        }
        return std::pair<It,It> (first,first);
    }

    /* A helper for std::equal_range */
    struct OpcodeRuleCompare
    {
        bool operator() (const CodeTree& tree,
                         const Rule& rule) const
        {
            /* If this function returns true, len=half.
             */

            if(tree.GetOpcode() != rule.match_tree.subfunc_opcode)
                return tree.GetOpcode() < rule.match_tree.subfunc_opcode;

            if(tree.GetParamCount() < rule.n_minimum_params)
            {
                // Tree has fewer params than required?
                return true; // Failure
            }
            return false;
        }
        bool operator() (const Rule& rule,
                         const CodeTree& tree) const
        {
            /* If this function returns true, rule will be excluded from the equal_range
             */

            if(rule.match_tree.subfunc_opcode != tree.GetOpcode())
                return rule.match_tree.subfunc_opcode < tree.GetOpcode();

            if(rule.n_minimum_params < tree.GetParamCount())
            {
                // Tree has more params than the pattern has?
                switch(rule.match_tree.match_type)
                {
                    case PositionalParams:
                    case SelectedParams:
                        return true; // Failure
                    case AnyParams:
                        return false; // Not a failure
		    case GroupFunction:
			break;
                }
            }
            return false;
        }
    };

    /* Test and apply a rule to a given CodeTree */
    bool TestRuleAndApplyIfMatch(
        const Rule& rule,
        CodeTree& tree)
    {
        MatchInfo info;

        MatchResultType found(false, MatchPositionSpecBaseP());

        /*std::cout << "TESTING: ";
        DumpMatch(rule, *tree, info, false);*/

        for(;;)
        {
            found = TestParams(rule.match_tree, tree, found.specs, info, true);
            if(found.found) break;
            if(!found.specs)
            {
                // Did not match
        #ifdef DEBUG_SUBSTITUTIONS
                //DumpMatch(rule, tree, info, false);
        #endif
                return false;
            }
        }
        // Matched
    #ifdef DEBUG_SUBSTITUTIONS
        DumpMatch(rule, tree, info, true);
    #endif
        SynthesizeRule(rule, tree, info);
        return true;
    }
}

namespace FPoptimizer_Grammar
{
    /* Apply the grammar to a given CodeTree */
    bool ApplyGrammar(
        const Grammar& grammar,
        CodeTree& tree,
        bool recurse)
    {
        if(tree.GetOptimizedUsing() == &grammar)
        {
#ifdef DEBUG_SUBSTITUTIONS
            std::cout << "Already optimized:  ";
            DumpTree(tree);
            std::cout << "\n" << std::flush;
#endif
            return false;
        }

        /* First optimize all children */
        if(recurse)
        {
            bool changed = false;

            for(size_t a=0; a<tree.GetParamCount(); ++a)
                if(ApplyGrammar( grammar, tree.GetParam(a) ))
                    changed = true;

            if(changed)
            {
                // Give the parent node a rerun at optimization
                tree.Mark_Incompletely_Hashed();
                return true;
            }
        }

        /* Figure out which rules _may_ match this tree */
        typedef const Rule* ruleit;

        std::pair<ruleit, ruleit> range
            = MyEqualRange(grammar.rule_begin,
                           grammar.rule_begin + grammar.rule_count,
                           tree,
                           OpcodeRuleCompare());

#ifdef DEBUG_SUBSTITUTIONS
        std::cout << "Input (Grammar #"
                  << (&grammar - pack.glist)
                  << ", " << FP_GetOpcodeName(tree.GetOpcode())
                  << "[" << tree.GetParamCount()
                  << "]" ", rules "
                  << (range.first - pack.glist[0].rule_begin)
                  << ".."
                  << (range.second - pack.glist[0].rule_begin)
                  << ": ";
        DumpTree(tree);
        std::cout << "\n" << std::flush;
#endif

        bool changed = false;

        for(; range.first != range.second; ++range.first)
        {
            /* Check if this rule matches */
            if(TestRuleAndApplyIfMatch(*range.first, tree))
            {
                changed = true;
                break;
            }
        }

        if(changed)
        {
#ifdef DEBUG_SUBSTITUTIONS
            std::cout << "Changed." << std::endl;
            std::cout << "Output: ";
            DumpTree(tree);
            std::cout << "\n" << std::flush;
#endif
            // Give the parent node a rerun at optimization
            tree.Mark_Incompletely_Hashed();
            return true;
        }

        // No changes, consider the tree properly optimized.
        tree.SetOptimizedUsing(&grammar);
        return false;
    }
}

#endif

#line 1 "fpoptimizer/fpoptimizer_optimize_match.cc"
#include "fpconfig.hh"
#include "fparser.hh"
#include "fptypes.hh"

#ifdef FP_SUPPORT_OPTIMIZER

#include <algorithm>
#include <assert.h>
#include <cmath>

#include <memory> /* for auto_ptr */

// line removed
// line removed

using namespace FUNCTIONPARSERTYPES;
using namespace FPoptimizer_Grammar;
using namespace FPoptimizer_CodeTree;
using namespace FPoptimizer_Optimize;

namespace
{
    /* Test the given constraints to a given CodeTree */
    bool TestImmedConstraints(unsigned bitmask, const CodeTree& tree)
    {
        switch(bitmask & ValueMask)
        {
            case Value_AnyNum: case ValueMask: break;
            case Value_EvenInt:
                if(tree.GetEvennessInfo() != CodeTree::IsAlways)
                    return false;
                break;
            case Value_OddInt:
                if(tree.GetEvennessInfo() != CodeTree::IsNever)
                    return false;
                break;
            case Value_IsInteger:
                if(!tree.IsAlwaysInteger()) return false;
                break;
            case Value_NonInteger:
                if(tree.IsAlwaysInteger()) return false;
                break;
            case Value_Logical:
                if(!tree.IsLogicalValue()) return false;
                break;
        }
        switch(bitmask & SignMask)
        {
            case Sign_AnySign: /*case SignMask:*/ break;
            case Sign_Positive:
                if(!tree.IsAlwaysSigned(true)) return false;
                break;
            case Sign_Negative:
                if(!tree.IsAlwaysSigned(false)) return false;
                break;
            case Sign_NoIdea:
                if(tree.IsAlwaysSigned(true)) return false;
                if(tree.IsAlwaysSigned(false)) return false;
                break;
        }
        switch(bitmask & OnenessMask)
        {
            case Oneness_Any: case OnenessMask: break;
            case Oneness_One:
                if(!tree.IsImmed()) return false;
                if(!FloatEqual(fabs(tree.GetImmed()), 1.0)) return false;
                break;
            case Oneness_NotOne:
                if(!tree.IsImmed()) return false;
                if(FloatEqual(fabs(tree.GetImmed()), 1.0)) return false;
                break;
        }
        switch(bitmask & ConstnessMask)
        {
            case Constness_Any: /*case ConstnessMask:*/ break;
            case Constness_Const:
                if(!tree.IsImmed()) return false;
                break;
        }
        return true;
    }

    /* Test the list of parameters to a given CodeTree */
    /* A helper function which simply checks whether the
     * basic shape of the tree matches what we are expecting
     * i.e. given number of numeric constants, etc.
     */
    bool IsLogisticallyPlausibleParamsMatch(
        const ParamSpec_SubFunctionData& params,
        const CodeTree& tree)
    {
        /* First, check if the tree has any chances of matching... */
        /* Figure out what we need. */
        struct Needs
        {
            int SubTrees; // This many subtrees
            int Others;   // This many others (namedholder)
            unsigned SubTreesDetail[VarBegin]; // This many subtrees of each opcode type

            int Immeds;      // This many immeds

            Needs(): SubTrees(0), Others(0), SubTreesDetail(), Immeds() { }
        } NeedList;

        // Figure out what we need
        unsigned minimum_need = 0;
        for(unsigned a = 0; a < params.param_count; ++a)
        {
            const ParamSpec& parampair = ParamSpec_Extract(params.param_list, a);
            switch(parampair.first)
            {
                case SubFunction:
                {
                    const ParamSpec_SubFunction& param = *(const ParamSpec_SubFunction*) parampair.second;
                    if(param.data.match_type == GroupFunction)
                        NeedList.Immeds += 1;
                    else
                    {
                        NeedList.SubTrees += 1;
                        assert( param.data.subfunc_opcode < VarBegin );
                        NeedList.SubTreesDetail[ param.data.subfunc_opcode ] += 1;
                    }
                    ++minimum_need;
                    break;
                }
                case NumConstant:
                case ParamHolder:
                    NeedList.Others += 1;
                    ++minimum_need;
                    break;
            }
        }
        if(tree.GetParamCount() < minimum_need)
        {
            // Impossible to satisfy
            return false;
        }

        // Figure out what we have (note: we already assume that the opcode of the tree matches!)
        for(size_t a=0; a<tree.GetParamCount(); ++a)
        {
            unsigned opcode = tree.GetParam(a).GetOpcode();
            switch(opcode)
            {
                case cImmed:
                    if(NeedList.Immeds > 0) NeedList.Immeds -= 1;
                    else NeedList.Others -= 1;
                    break;
                case cVar:
                case cFCall:
                case cPCall:
                    NeedList.Others -= 1;
                    break;
                default:
                    assert( opcode < VarBegin );
                    if(NeedList.SubTrees > 0
                    && NeedList.SubTreesDetail[opcode] > 0)
                    {
                        NeedList.SubTrees -= 1;
                        NeedList.SubTreesDetail[opcode] -= 1;
                    }
                    else NeedList.Others -= 1;
            }
        }

        // Check whether all needs were satisfied
        if(NeedList.Immeds > 0
        || NeedList.SubTrees > 0
        || NeedList.Others > 0)
        {
            // Something came short, impossible to satisfy.
            return false;
        }

        if(params.match_type != AnyParams)
        {
            if(NeedList.Immeds < 0
            || NeedList.SubTrees < 0
            || NeedList.Others < 0/*
            || params.count != tree.GetParamCount() - already checked*/)
            {
                // Something was too much.
                return false;
            }
        }
        return true;
    }

    /* Construct CodeTree from a GroupFunction, hopefully evaluating to a constant value */
    CodeTree CalculateGroupFunction(
        const ParamSpec& parampair,
        const MatchInfo& info)
    {
        using namespace std;

        switch( parampair.first )
        {
            case NumConstant:
            {
                const ParamSpec_NumConstant& param = *(const ParamSpec_NumConstant*) parampair.second;
                return CodeTree( param.constvalue ); // Note: calculates hash too.
            }
            case ParamHolder:
            {
                const ParamSpec_ParamHolder& param = *(const ParamSpec_ParamHolder*) parampair.second;
                CodeTree result ( info.GetParamHolderValueIfFound( param.index ) );
                if(result.IsDefined())
                    return result;
                break; // The immed is not defined
            }
            case SubFunction:
            {
                const ParamSpec_SubFunction& param = *(const ParamSpec_SubFunction*) parampair.second;
                /* Synthesize a CodeTree which will take care of
                 * constant-folding our expression. It will also
                 * indicate whether the result is, in fact,
                 * a constant at all. */
                CodeTree result;
                result.SetOpcode( param.data.subfunc_opcode );
                for(unsigned a=0; a<param.data.param_count; ++a)
                    result.AddParam(
                            CalculateGroupFunction(
                                ParamSpec_Extract(param.data.param_list, a), info)
                                    );
                result.Rehash();
                return result;
            }
        }
        // Issue an un-calculatable tree.
        return CodeTree(999, CodeTree::VarTag());
    }
}

namespace FPoptimizer_Optimize
{
    /* Test the given parameter to a given CodeTree */
    MatchResultType TestParam(
        const ParamSpec& parampair,
        const CodeTree& tree,
        const MatchPositionSpecBaseP& start_at,
        MatchInfo& info)
    {
        /* What kind of param are we expecting */
        switch( parampair.first )
        {
            case NumConstant: /* A particular numeric value */
            {
                const ParamSpec_NumConstant& param = *(const ParamSpec_NumConstant*) parampair.second;
                if(!tree.IsImmed()) return false;
                return FloatEqual(tree.GetImmed(), param.constvalue);
            }
            case ParamHolder: /* Any arbitrary node */
            {
                const ParamSpec_ParamHolder& param = *(const ParamSpec_ParamHolder*) parampair.second;
                if(!TestImmedConstraints(param.constraints, tree)) return false;
                return info.SaveOrTestParamHolder(param.index, tree);
            }
            case SubFunction:
            {
                const ParamSpec_SubFunction& param = *(const ParamSpec_SubFunction*) parampair.second;
                if(param.data.match_type == GroupFunction)
                { /* A constant value acquired from this formula */
                    if(!TestImmedConstraints(param.constraints, tree)) return false;
                    /* Construct the formula */
                    CodeTree  grammar_func = CalculateGroupFunction(parampair, info);
                    /* Evaluate it and compare */
                    return grammar_func.IsIdenticalTo(tree);
                }
                else /* A subtree conforming these specs */
                {
                    if(!start_at)
                    {
                        if(!TestImmedConstraints(param.constraints, tree)) return false;
                        if(tree.GetOpcode() != param.data.subfunc_opcode) return false;
                    }
                    return TestParams(param.data,
                                      tree, start_at, info, false);
                }
            }
        }
        return false;
    }

    struct PositionalParams_Rec
    {
        MatchPositionSpecBaseP start_at; /* child's start_at */
        MatchInfo              info;     /* backup of "info" at start */

        PositionalParams_Rec(): start_at(), info() { }
    };
    class MatchPositionSpec_PositionalParams
        : public MatchPositionSpecBase,
          public std::vector<PositionalParams_Rec>
    {
    public:
        explicit MatchPositionSpec_PositionalParams(size_t n)
            : MatchPositionSpecBase(),
              std::vector<PositionalParams_Rec> (n)
              { }
    };

    struct AnyWhere_Rec
    {
        MatchPositionSpecBaseP start_at; /* child's start_at */
        AnyWhere_Rec() : start_at() { }
    };
    class MatchPositionSpec_AnyWhere
        : public MatchPositionSpecBase,
          public std::vector<AnyWhere_Rec>
    {
    public:
        unsigned trypos;   /* which param index to try next */

        explicit MatchPositionSpec_AnyWhere(size_t n)
            : MatchPositionSpecBase(),
              std::vector<AnyWhere_Rec> (n),
              trypos(0)
              { }
    };

    MatchResultType TestParam_AnyWhere(
        const ParamSpec& parampair,
        const CodeTree& tree,
        const MatchPositionSpecBaseP& start_at,
        MatchInfo&         info,
        std::vector<bool>& used,
        bool TopLevel)
    {
        FPOPT_autoptr<MatchPositionSpec_AnyWhere> position;
        unsigned a;
        if(!start_at == false)
        {
            position = (MatchPositionSpec_AnyWhere*) &*start_at;
            a = position->trypos;
            goto retry_anywhere_2;
        }
        else
        {
            position = new MatchPositionSpec_AnyWhere(tree.GetParamCount());
            a = 0;
        }
        for(; a < tree.GetParamCount(); ++a)
        {
            if(used[a]) continue;

        retry_anywhere:
          { MatchResultType r = TestParam(
                parampair,
                tree.GetParam(a),
                (*position)[a].start_at,
                info);

            (*position)[a].start_at = r.specs;
            if(r.found)
            {
                used[a]               = true; // matched
                if(TopLevel) info.SaveMatchedParamIndex(a);

                position->trypos = a; // in case of backtrack, try a again
                return MatchResultType(true, &*position);
            } }
        retry_anywhere_2:
            if(!((*position)[a].start_at) == false) // is there another try?
            {
                goto retry_anywhere;
            }
            // no, move on
        }
        return false;
    }

    struct AnyParams_Rec
    {
        MatchPositionSpecBaseP start_at; /* child's start_at */
        MatchInfo              info;     /* backup of "info" at start */
        std::vector<bool>      used;     /* which params are remaining */

        explicit AnyParams_Rec(size_t nparams)
            : start_at(), info(), used(nparams) { }
    };
    class MatchPositionSpec_AnyParams
        : public MatchPositionSpecBase,
          public std::vector<AnyParams_Rec>
    {
    public:
        explicit MatchPositionSpec_AnyParams(size_t n, size_t m)
            : MatchPositionSpecBase(),
              std::vector<AnyParams_Rec> (n, AnyParams_Rec(m))
              { }
    };

    /* Test the list of parameters to a given CodeTree */
    MatchResultType TestParams(
        const ParamSpec_SubFunctionData& model_tree,
        const CodeTree& tree,
        const MatchPositionSpecBaseP& start_at,
        MatchInfo& info,
        bool TopLevel)
    {
        /* When PositionalParams or SelectedParams, verify that
         * the number of parameters is exactly as expected.
         */
        if(model_tree.match_type != AnyParams)
        {
            if(model_tree.param_count != tree.GetParamCount())
                return false;
        }

        /* Verify that the tree basically conforms the shape we are expecting */
        /* This test is not necessary; it may just save us some work. */
        if(!IsLogisticallyPlausibleParamsMatch(model_tree, tree))
        {
            return false;
        }

        /* Verify each parameter that they are found in the tree as expected. */
        switch(model_tree.match_type)
        {
            case PositionalParams:
            {
                /* Simple: Test all given parameters in succession. */
                FPOPT_autoptr<MatchPositionSpec_PositionalParams> position;
                unsigned a;
                if(!start_at == false)
                {
                    position = (MatchPositionSpec_PositionalParams*) &*start_at;
                    a = model_tree.param_count - 1;
                    goto retry_positionalparams_2;
                }
                else
                {
                    position = new MatchPositionSpec_PositionalParams(model_tree.param_count);
                    a = 0;
                }

                for(; a < model_tree.param_count; ++a)
                {
                    (*position)[a].info = info;
                retry_positionalparams:
                  { MatchResultType r = TestParam(
                        ParamSpec_Extract(model_tree.param_list, a),
                        tree.GetParam(a),
                        (*position)[a].start_at,
                        info);

                    (*position)[a].start_at = r.specs;
                    if(r.found)
                    {
                        continue;
                  } }
                retry_positionalparams_2:
                    // doesn't match
                    if(!((*position)[a].start_at) == false) // is there another try?
                    {
                        info = (*position)[a].info;
                        goto retry_positionalparams;
                    }
                    // no, backtrack
                    if(a > 0)
                    {
                        --a;
                        goto retry_positionalparams_2;
                    }
                    // cannot backtrack
                    info = (*position)[0].info;
                    return false;
                }
                if(TopLevel)
                    for(unsigned a = 0; a < model_tree.param_count; ++a)
                        info.SaveMatchedParamIndex(a);
                return MatchResultType(true, &*position);
            }
            case SelectedParams:
                // same as AnyParams, except that model_tree.count==tree.GetParamCount()
                //                       and that there are no RestHolders
            case AnyParams:
            {
                /* Ensure that all given parameters are found somewhere, in any order */

                FPOPT_autoptr<MatchPositionSpec_AnyParams> position;
                std::vector<bool> used( tree.GetParamCount() );
                std::vector<unsigned> depcodes( model_tree.param_count );
                std::vector<unsigned> test_order( model_tree.param_count );
                for(unsigned a=0; a<model_tree.param_count; ++a)
                {
                    const ParamSpec parampair = ParamSpec_Extract(model_tree.param_list, a);
                    depcodes[a] = ParamSpec_GetDepCode(parampair);
                }
                { unsigned b=0;
                for(unsigned a=0; a<model_tree.param_count; ++a)
                    if(depcodes[a] != 0)
                        test_order[b++] = a;
                for(unsigned a=0; a<model_tree.param_count; ++a)
                    if(depcodes[a] == 0)
                        test_order[b++] = a;
                }

                unsigned a;
                if(!start_at == false)
                {
                    position = (MatchPositionSpec_AnyParams*) &*start_at;
                    a = model_tree.param_count - 1;
                    goto retry_anyparams_2;
                }
                else
                {
                    position = new MatchPositionSpec_AnyParams(model_tree.param_count,
                                                               tree.GetParamCount());
                    a = 0;
                    if(model_tree.param_count != 0)
                    {
                        (*position)[0].info   = info;
                        (*position)[0].used   = used;
                    }
                }
                // Match all but restholders
                for(; a < model_tree.param_count; ++a)
                {
                    if(a > 0) // this test is not necessary, but it saves from doing
                    {         // duplicate work, because [0] was already saved above.
                        (*position)[a].info   = info;
                        (*position)[a].used   = used;
                    }
                retry_anyparams:
                  { MatchResultType r = TestParam_AnyWhere(
                        ParamSpec_Extract(model_tree.param_list, test_order[a]),
                        tree,
                        (*position)[a].start_at,
                        info,
                        used,
                        TopLevel);
                    (*position)[a].start_at = r.specs;
                    if(r.found)
                    {
                        continue;
                  } }
                retry_anyparams_2:
                    // doesn't match
                    if(!((*position)[a].start_at) == false) // is there another try?
                    {
                        info = (*position)[a].info;
                        used = (*position)[a].used;
                        goto retry_anyparams;
                    }
                    // no, backtrack
                    if(a > 0)
                    {
                        --a;
                        goto retry_anyparams_2;
                    }
                    // cannot backtrack
                    info = (*position)[0].info;
                    return false;
                }
                // Capture anything remaining in the restholder
                if(model_tree.restholder_index != 0)
                {
                    for(unsigned b = 0; b < tree.GetParamCount(); ++b)
                    {
                        if(used[b]) continue; // Ignore subtrees that were already used
                        // Save this tree to this restholder

                        info.SaveRestHolderMatch(model_tree.restholder_index,
                                                 tree.GetParam(b));
                        used[b] = true;
                        if(TopLevel) info.SaveMatchedParamIndex(b);
                    }
                }
                return MatchResultType(true, &*position);
            }
            case GroupFunction: // never occurs
                break;
        }
        return false; // doesn't match
    }
}

#endif

#line 1 "fpoptimizer/fpoptimizer_optimize_synth.cc"
#include "fpconfig.hh"
#include "fparser.hh"
#include "fptypes.hh"

#ifdef FP_SUPPORT_OPTIMIZER

#include <algorithm>
#include <assert.h>

// line removed

namespace FPoptimizer_Optimize
{
    /* Synthesize the given grammatic parameter into the codetree */
    void SynthesizeParam(
        const ParamSpec& parampair,
        CodeTree& tree,
        MatchInfo& info,
        bool inner = true)
    {
        switch( parampair.first )
        {
            case NumConstant:
              { const ParamSpec_NumConstant& param = *(const ParamSpec_NumConstant*) parampair.second;
                tree.SetImmed( param.constvalue );
                if(inner) tree.Rehash(false);
                break; }
            case ParamHolder:
              { const ParamSpec_ParamHolder& param = *(const ParamSpec_ParamHolder*) parampair.second;
                tree.Become( info.GetParamHolderValue( param.index ) );
                break; }
            case SubFunction:
              { const ParamSpec_SubFunction& param = *(const ParamSpec_SubFunction*) parampair.second;
                tree.SetOpcode( param.data.subfunc_opcode );
                for(unsigned a=0; a < param.data.param_count; ++a)
                {
                    CodeTree nparam;
                    SynthesizeParam( ParamSpec_Extract(param.data.param_list, a), nparam, info, true );
                    tree.AddParamMove(nparam);
                }
                if(param.data.restholder_index != 0)
                {
                    std::vector<CodeTree> trees
                        ( info.GetRestHolderValues( param.data.restholder_index ) );
                    tree.AddParamsMove(trees);
                    // ^note: this fails if the same restholder is synth'd twice
                    if(tree.GetParamCount() == 1)
                    {
                        /* Convert cMul <1> into <1> when <1> only contains one operand.
                         * This is redundant code; it is also done in ConstantFolding(),
                         * but it might be better for performance to do it here, too.
                         */
                        assert(tree.GetOpcode() == cAdd || tree.GetOpcode() == cMul
                            || tree.GetOpcode() == cMin || tree.GetOpcode() == cMax
                            || tree.GetOpcode() == cAnd || tree.GetOpcode() == cOr);
                        tree.Become(tree.GetParam(0));
                    }
                }
                if(inner)
                    tree.Rehash();
                break; }
        }
    }

    void SynthesizeRule(
        const Rule& rule,
        CodeTree& tree,
        MatchInfo& info)
    {
        switch(rule.ruletype)
        {
            case ProduceNewTree:
            {
                tree.DelParams();
                SynthesizeParam( ParamSpec_Extract(rule.repl_param_list, 0), tree, info, false );
                break;
            }
            case ReplaceParams:
            default:
            {
                /* Delete the matched parameters from the source tree */
                std::vector<unsigned> list = info.GetMatchedParamIndexes();
                std::sort(list.begin(), list.end());
                for(size_t a=list.size(); a-->0; )
                    tree.DelParam( list[a] );

                /* Synthesize the replacement params */
                for(unsigned a=0; a < rule.repl_param_count; ++a)
                {
                    CodeTree nparam;
                    SynthesizeParam( ParamSpec_Extract(rule.repl_param_list, a), nparam, info, true );
                    tree.AddParamMove(nparam);
                }
                break;
            }
        }
    }
}

#endif

#line 1 "fpoptimizer/fpoptimizer_optimize_debug.cc"
// line removed
// line removed
// line removed
// line removed

#include <sstream>

#ifdef DEBUG_SUBSTITUTIONS
using namespace FUNCTIONPARSERTYPES;
using namespace FPoptimizer_Grammar;
using namespace FPoptimizer_CodeTree;
using namespace FPoptimizer_Optimize;

namespace FPoptimizer_Grammar
{
    void DumpMatch(const Rule& rule,
                   const CodeTree& tree,
                   const MatchInfo& info,
                   bool DidMatch,
                   std::ostream& o)
    {
        static const char ParamHolderNames[][2] = {"%","&","x","y","z","a","b","c"};

        o <<
            "Found " << (DidMatch ? "match" : "mismatch") << ":\n"
            "  Pattern    : ";
        { ParamSpec tmp;
          tmp.first = SubFunction;
          ParamSpec_SubFunction tmp2;
          tmp2.data = rule.match_tree;
          tmp.second = (const void*) &tmp2;
          DumpParam(tmp, o);
        }
        o << "\n"
            "  Replacement: ";
        DumpParams(rule.repl_param_list, rule.repl_param_count, o);
        o << "\n";

        o <<
            "  Tree       : ";
        DumpTree(tree, o);
        o << "\n";
        if(DidMatch) DumpHashes(tree, o);

        for(std::map<unsigned, CodeTree>::const_iterator
            i = info.paramholder_matches.begin();
            i != info.paramholder_matches.end();
            ++i)
        {
            o << "           " << ParamHolderNames[i->first] << " = ";
            DumpTree(i->second, o);
            o << "\n";
        }

        for(std::multimap<unsigned, CodeTree>::const_iterator
            i = info.restholder_matches.begin();
            i != info.restholder_matches.end();
            ++i)
        {
            o << "         <" << i->first << "> = ";
            DumpTree(i->second, o);
            o << std::endl;
        }
        o << std::flush;
    }

    void DumpHashes(const CodeTree& tree,
                    std::map<fphash_t, std::set<std::string> >& done,
                    std::ostream& o)
    {
        for(size_t a=0; a<tree.GetParamCount(); ++a)
            DumpHashes(tree.GetParam(a), done, o);

        std::ostringstream buf;
        DumpTree(tree, buf);
        done[tree.GetHash()].insert(buf.str());
    }
    void DumpHashes(const CodeTree& tree, std::ostream& o)
    {
        std::map<fphash_t, std::set<std::string> > done;
        DumpHashes(tree, done, o);

        for(std::map<fphash_t, std::set<std::string> >::const_iterator
            i = done.begin();
            i != done.end();
            ++i)
        {
            const std::set<std::string>& flist = i->second;
            if(flist.size() != 1) o << "ERROR - HASH COLLISION?\n";
            for(std::set<std::string>::const_iterator
                j = flist.begin();
                j != flist.end();
                ++j)
            {
                o << '[' << std::hex << i->first.hash1
                              << ',' << i->first.hash2
                              << ']' << std::dec;
                o << ": " << *j << "\n";
            }
        }
    }
    void DumpTree(const CodeTree& tree, std::ostream& o)
    {
        //o << "/*" << tree.Depth << "*/";
        const char* sep2 = "";
        /*
        o << '[' << std::hex << tree.Hash.hash1
                      << ',' << tree.Hash.hash2
                      << ']' << std::dec;
        */
        switch(tree.GetOpcode())
        {
            case cImmed: o << tree.GetImmed(); return;
            case cVar:   o << "Var" << (tree.GetVar() - VarBegin); return;
            case cAdd: sep2 = " +"; break;
            case cMul: sep2 = " *"; break;
            case cAnd: sep2 = " &"; break;
            case cOr: sep2 = " |"; break;
            case cPow: sep2 = " ^"; break;
            default:
                o << FP_GetOpcodeName(tree.GetOpcode());
                if(tree.GetOpcode() == cFCall || tree.GetOpcode() == cPCall)
                    o << ':' << tree.GetFuncNo();
        }
        o << '(';
        if(tree.GetParamCount() <= 1 && *sep2) o << (sep2+1) << ' ';
        for(size_t a=0; a<tree.GetParamCount(); ++a)
        {
            if(a > 0) o << ' ';

            DumpTree(tree.GetParam(a), o);

            if(a+1 < tree.GetParamCount()) o << sep2;
        }
        o << ')';
    }

    void DumpTreeWithIndent(const CodeTree& tree, std::ostream& o, const std::string& indent)
    {
        o << '[' << std::hex << (void*)(&tree.GetParams())
                 << std::dec
                 << ',' << tree.GetRefCount()
                 << ']';
        o << indent << '_';

        switch(tree.GetOpcode())
        {
            case cImmed: o << "cImmed " << tree.GetImmed(); o << '\n'; return;
            case cVar:   o << "cVar " << (tree.GetVar() - VarBegin); o << '\n'; return;
            default:
                o << FP_GetOpcodeName(tree.GetOpcode());
                if(tree.GetOpcode() == cFCall || tree.GetOpcode() == cPCall)
                    o << ':' << tree.GetFuncNo();
                o << '\n';
        }
        for(size_t a=0; a<tree.GetParamCount(); ++a)
        {
            std::string ind = indent;
            for(size_t p=0; p < ind.size(); p+=2)
                if(ind[p] == '\\')
                    ind[p] = ' ';
            ind += (a+1 < tree.GetParamCount()) ? " |" : " \\";
            DumpTreeWithIndent(tree.GetParam(a), o, ind);
        }
        o << std::flush;
    }
}
#endif


#line 1 "fpoptimizer/fpoptimizer_main.cc"
#include "fpconfig.hh"
#include "fparser.hh"
#include "fptypes.hh"

// line removed
// line removed

using namespace FUNCTIONPARSERTYPES;

#ifdef FP_SUPPORT_OPTIMIZER
using namespace FPoptimizer_CodeTree;
using namespace FPoptimizer_Grammar;

void FunctionParser::Optimize()
{
    CopyOnWrite();

    //PrintByteCode(std::cout);

    CodeTree tree;
    tree.GenerateFrom(data->ByteCode, data->Immed, *data);

    while(ApplyGrammar(pack.glist[0], tree)) // intermediate
        { //std::cout << "Rerunning 1\n";
            FixIncompleteHashes(tree);
        }

    while(ApplyGrammar(pack.glist[1], tree)) // final1
        { //std::cout << "Rerunning 2\n";
            FixIncompleteHashes(tree);
        }

    while(ApplyGrammar(pack.glist[2], tree)) // final2
        { //std::cout << "Rerunning 3\n";
            FixIncompleteHashes(tree);
        }

    std::vector<unsigned> byteCode;
    std::vector<double> immed;
    size_t stacktop_max = 0;
    tree.SynthesizeByteCode(byteCode, immed, stacktop_max);

    /*std::cout << std::flush;
    std::cerr << std::flush;
    fprintf(stderr, "Estimated stacktop %u\n", (unsigned)stacktop_max);
    fflush(stderr);*/

    if(data->StackSize != stacktop_max)
    {
        data->StackSize = stacktop_max; // note: gcc warning is meaningful
        data->Stack.resize(stacktop_max);
    }

    data->ByteCode.swap(byteCode);
    data->Immed.swap(immed);

    //PrintByteCode(std::cout);
}

#endif

#line 1 "fpoptimizer/fpoptimizer_codetree_to_bytecode.cc"
#include <cmath>
#include <list>
#include <cassert>

// line removed
#include "fptypes.hh"
// line removed
// line removed
// line removed

#ifdef FP_SUPPORT_OPTIMIZER

using namespace FUNCTIONPARSERTYPES;
//using namespace FPoptimizer_Grammar;

#ifndef FP_GENERATING_POWI_TABLE
static const unsigned MAX_POWI_BYTECODE_LENGTH = 15;
#else
static const unsigned MAX_POWI_BYTECODE_LENGTH = 999;
#endif
static const unsigned MAX_MULI_BYTECODE_LENGTH = 3;

namespace
{
    using namespace FPoptimizer_CodeTree;

    bool AssembleSequence(
                  const CodeTree& tree, long count,
                  const FPoptimizer_ByteCode::SequenceOpCode& sequencing,
                  FPoptimizer_ByteCode::ByteCodeSynth& synth,
                  size_t max_bytecode_grow_length);
}

namespace
{
    typedef
        std::multimap<fphash_t,  std::pair<size_t, CodeTree> >
        TreeCountType;
    typedef
        std::multimap<fphash_t, CodeTree>
        DoneTreesType;

    void FindTreeCounts(TreeCountType& TreeCounts, const CodeTree& tree)
    {
        TreeCountType::iterator i = TreeCounts.lower_bound(tree.GetHash());
        for(; i != TreeCounts.end() && i->first == tree.GetHash(); ++i)
        {
            if(tree.IsIdenticalTo( i->second.second ) )
            {
                i->second.first += 1;
                goto found;
        }   }
        TreeCounts.insert(i, std::make_pair(tree.GetHash(), std::make_pair(size_t(1), tree)));
    found:
        for(size_t a=0; a<tree.GetParamCount(); ++a)
            FindTreeCounts(TreeCounts, tree.GetParam(a));
    }

    void RememberRecursivelyHashList(DoneTreesType& hashlist,
                                     const CodeTree& tree)
    {
        hashlist.insert( std::make_pair(tree.GetHash(), tree) );
        for(size_t a=0; a<tree.GetParamCount(); ++a)
            RememberRecursivelyHashList(hashlist, tree.GetParam(a));
    }

    #ifdef DEBUG_SUBSTITUTIONS
    CodeTree* root;
    #endif

    bool IsOptimizableUsingPowi(long immed, long penalty = 0)
    {
        FPoptimizer_ByteCode::ByteCodeSynth synth;
        return AssembleSequence(CodeTree(0, CodeTree::VarTag()),
                                immed,
                                FPoptimizer_ByteCode::MulSequence,
                                synth,
                                MAX_POWI_BYTECODE_LENGTH - penalty);
    }

    void ChangeIntoSqrtChain(CodeTree& tree, long sqrt_chain)
    {
        long abs_sqrt_chain = sqrt_chain < 0 ? -sqrt_chain : sqrt_chain;
        while(abs_sqrt_chain > 2)
        {
            CodeTree tmp;
            tmp.SetOpcode(cSqrt);
            tmp.AddParamMove(tree.GetParam(0));
            tmp.Rehash();
            tree.SetParamMove(0, tmp);
            abs_sqrt_chain /= 2;
        }
        tree.DelParam(1);
        tree.SetOpcode(sqrt_chain < 0 ? cRSqrt : cSqrt);
    }

    bool RecreateInversionsAndNegations(CodeTree& tree)
    {
        bool changed = false;

        for(size_t a=0; a<tree.GetParamCount(); ++a)
            if(RecreateInversionsAndNegations( tree.GetParam(a) ))
                changed = true;

        if(changed)
        {
        exit_changed:
            tree.Mark_Incompletely_Hashed();
            return true;
        }

        switch(tree.GetOpcode()) // Recreate inversions and negations
        {
            case cMul:
            {
                std::vector<CodeTree> div_params;

                for(size_t a = tree.GetParamCount(); a-- > 0; )
                {
                    const CodeTree& powgroup = tree.GetParam(a);
                    if(powgroup.GetOpcode() == cPow
                    && powgroup.GetParam(1).IsImmed())
                    {
                        const CodeTree& exp_param = powgroup.GetParam(1);
                        double exponent = exp_param.GetImmed();
                        if(FloatEqual(exponent, -1.0))
                        {
                            tree.CopyOnWrite();
                            div_params.push_back(tree.GetParam(a).GetParam(0));
                            tree.DelParam(a); // delete the pow group
                        }
                        else if(exponent < 0 && IsIntegerConst(exponent))
                        {
                            CodeTree edited_powgroup;
                            edited_powgroup.SetOpcode(cPow);
                            edited_powgroup.AddParam(powgroup.GetParam(0));
                            edited_powgroup.AddParam(CodeTree(-exponent));
                            edited_powgroup.Rehash();
                            div_params.push_back(edited_powgroup);
                            tree.CopyOnWrite();
                            tree.DelParam(a); // delete the pow group
                        }
                    }
                }
                if(!div_params.empty())
                {
                    changed = true;

                    CodeTree divgroup;
                    divgroup.SetOpcode(cMul);
                    divgroup.SetParamsMove(div_params);
                    divgroup.Rehash(); // will reduce to div_params[0] if only one item
                    CodeTree mulgroup;
                    mulgroup.SetOpcode(cMul);
                    mulgroup.SetParamsMove(tree.GetParams());
                    mulgroup.Rehash(); // will reduce to 1.0 if none remained in this cMul
                    if(mulgroup.IsImmed() && FloatEqual(mulgroup.GetImmed(), 1.0))
                    {
                        tree.SetOpcode(cInv);
                        tree.AddParamMove(divgroup);
                    }
                    else
                    {
                        if(mulgroup.GetDepth() >= divgroup.GetDepth())
                        {
                            tree.SetOpcode(cDiv);
                            tree.AddParamMove(mulgroup);
                            tree.AddParamMove(divgroup);
                        }
                        else
                        {
                            tree.SetOpcode(cRDiv);
                            tree.AddParamMove(divgroup);
                            tree.AddParamMove(mulgroup);
                        }
                    }
                }
                break;
            }
            case cAdd:
            {
                std::vector<CodeTree> sub_params;

                for(size_t a = tree.GetParamCount(); a-- > 0; )
                    if(tree.GetParam(a).GetOpcode() == cMul)
                    {
                        bool is_signed = false; // if the mul group has a -1 constant...

                        CodeTree mulgroup = tree.GetParam(a);

                        for(size_t b=mulgroup.GetParamCount(); b-- > 0; )
                            if(mulgroup.GetParam(b).IsImmed()
                            && FloatEqual(mulgroup.GetParam(b).GetImmed(), -1.0))
                            {
                                mulgroup.CopyOnWrite();
                                mulgroup.DelParam(b);
                                is_signed = !is_signed;
                            }
                        if(is_signed)
                        {
                            mulgroup.Rehash();
                            sub_params.push_back(mulgroup);
                            tree.CopyOnWrite();
                            tree.DelParam(a);
                        }
                    }
                if(!sub_params.empty())
                {
                    CodeTree subgroup;
                    subgroup.SetOpcode(cAdd);
                    subgroup.SetParamsMove(sub_params);
                    subgroup.Rehash(); // will reduce to sub_params[0] if only one item
                    CodeTree addgroup;
                    addgroup.SetOpcode(cAdd);
                    addgroup.SetParamsMove(tree.GetParams());
                    addgroup.Rehash(); // will reduce to 0.0 if none remained in this cAdd
                    if(addgroup.IsImmed() && FloatEqual(addgroup.GetImmed(), 0.0))
                    {
                        tree.SetOpcode(cNeg);
                        tree.AddParamMove(subgroup);
                    }
                    else
                    {
                        if(addgroup.GetDepth() == 1)
                        {
                            /* 5 - (x+y+z) is best expressed as rsub(x+y+z, 5);
                             * this has lowest stack usage.
                             * This is identified by addgroup having just one member.
                             */
                            tree.SetOpcode(cRSub);
                            tree.AddParamMove(subgroup);
                            tree.AddParamMove(addgroup);
                        }
                        else if(subgroup.GetOpcode() == cAdd)
                        {
                            /* a+b-(x+y+z) is expressed as a+b-x-y-z.
                             * Making a long chain of cSubs is okay, because the
                             * cost of cSub is the same as the cost of cAdd.
                             * Thus we get the lowest stack usage.
                             * This approach cannot be used for cDiv.
                             */
                            tree.SetOpcode(cSub);
                            tree.AddParamMove(addgroup);
                            tree.AddParamMove(subgroup.GetParam(0));
                            for(size_t a=1; a<subgroup.GetParamCount(); ++a)
                            {
                                CodeTree innersub;
                                innersub.SetOpcode(cSub);
                                innersub.SetParamsMove(tree.GetParams());
                                innersub.Rehash(false);
                                //tree.DelParams();
                                tree.AddParamMove(innersub);
                                tree.AddParamMove(subgroup.GetParam(a));
                            }
                        }
                        else
                        {
                            tree.SetOpcode(cSub);
                            tree.AddParamMove(addgroup);
                            tree.AddParamMove(subgroup);
                        }
                    }
                }
                break;
            }
            case cPow:
            {
                const CodeTree& p0 = tree.GetParam(0);
                const CodeTree& p1 = tree.GetParam(1);
                if(p1.IsImmed())
                {
                    if(p1.GetImmed() != 0.0 && !p1.IsLongIntegerImmed())
                    {
                        double inverse_exponent = 1.0 / p1.GetImmed();
                        if(inverse_exponent >= -16.0 && inverse_exponent <= 16.0
                        && IsIntegerConst(inverse_exponent))
                        {
                            long sqrt_chain = (long) inverse_exponent;
                            long abs_sqrt_chain = sqrt_chain < 0 ? -sqrt_chain : sqrt_chain;
                            if((abs_sqrt_chain & (abs_sqrt_chain-1)) == 0) // 2, 4, 8 or 16
                            {
                                ChangeIntoSqrtChain(tree, sqrt_chain);
                                changed = true;
                                break;
                            }
                        }
                    }
                    if(!p1.IsLongIntegerImmed())
                    {
                        // x^1.5 is sqrt(x^3)
                        for(int sqrt_count=1; sqrt_count<=4; ++sqrt_count)
                        {
                            double with_sqrt_exponent = p1.GetImmed() * (1 << sqrt_count);
                            if(IsIntegerConst(with_sqrt_exponent))
                            {
                                long int_sqrt_exponent = (long)with_sqrt_exponent;
                                if(int_sqrt_exponent < 0)
                                    int_sqrt_exponent = -int_sqrt_exponent;
                                if(IsOptimizableUsingPowi(int_sqrt_exponent, sqrt_count))
                                {
                                    long sqrt_chain = 1 << sqrt_count;
                                    if(with_sqrt_exponent < 0) sqrt_chain = -sqrt_chain;

                                    CodeTree tmp;
                                    tmp.AddParamMove(tree.GetParam(0));
                                    tmp.AddParam(CodeTree());
                                    ChangeIntoSqrtChain(tmp, sqrt_chain);
                                    tmp.Rehash();
                                    tree.SetParamMove(0, tmp);
                                    tree.SetParam(1, CodeTree(p1.GetImmed() * (double)sqrt_chain));
                                    changed = true;
                                }
                                break;
                            }
                        }
                    }
                }
                if(!p1.IsLongIntegerImmed()
                || !IsOptimizableUsingPowi(p1.GetLongIntegerImmed()))
                {
                    if(p0.IsImmed() && p0.GetImmed() > 0.0)
                    {
                        // Convert into cExp or Exp2.
                        //    x^y = exp(log(x) * y) =
                        //    Can only be done when x is positive, though.
                        double mulvalue = std::log( p0.GetImmed() );
                        if(mulvalue == 1.0)
                        {
                            // exp(1)^x becomes exp(x)
                            tree.DelParam(0);
                        }
                        else
                        {
                            // exp(4)^x becomes exp(4*x)
                            CodeTree exponent;
                            exponent.SetOpcode(cMul);
                            exponent.AddParam( CodeTree( mulvalue ) );
                            exponent.AddParam(p1);
                            exponent.Rehash();
                            tree.SetParamMove(0, exponent);
                            tree.DelParam(1);
                        }
                        tree.SetOpcode(cExp);
                        changed = true;
                    }
                    else if(p1.IsImmed() && !p1.IsLongIntegerImmed())
                    {
                        // x^y can be safely converted into exp(y * log(x))
                        // when y is _not_ integer, because we know that x >= 0.
                        // Otherwise either expression will give a NaN or inf.
                        CodeTree log;
                        log.SetOpcode(cLog);
                        log.AddParam(p0);
                        log.Rehash();
                        CodeTree exponent;
                        exponent.SetOpcode(cMul);
                        exponent.AddParam(p1);
                        exponent.AddParamMove(log);
                        exponent.Rehash();
                        tree.SetOpcode(cExp);
                        tree.SetParamMove(0, exponent);
                        tree.DelParam(1);
                        changed = true;
                    }
                }
                break;
            }

            default: break;
        }

        if(changed)
            goto exit_changed;

        return changed;
    }
}

namespace FPoptimizer_CodeTree
{
    void CodeTree::SynthesizeByteCode(
        std::vector<unsigned>& ByteCode,
        std::vector<double>&   Immed,
        size_t& stacktop_max)
    {
    #ifdef DEBUG_SUBSTITUTIONS
        std::cout << "Making bytecode for:\n";
        FPoptimizer_Grammar::DumpTreeWithIndent(*this); root=this;
    #endif
        while(RecreateInversionsAndNegations(*this))
        {
        #ifdef DEBUG_SUBSTITUTIONS
            std::cout << "One change issued, produced:\n";
            FPoptimizer_Grammar::DumpTreeWithIndent(*root);
        #endif
            FixIncompleteHashes(*this);
        }
    #ifdef DEBUG_SUBSTITUTIONS
        std::cout << "After recreating inv/neg:  "; FPoptimizer_Grammar::DumpTree(*this); std::cout << "\n";
    #endif

        FPoptimizer_ByteCode::ByteCodeSynth synth;

      { // begin scope for TreeCounts, AlreadyDoneTrees
        /* Find common subtrees */
        TreeCountType TreeCounts;
        FindTreeCounts(TreeCounts, *this);

        /* Synthesize some of the most common ones */
        DoneTreesType AlreadyDoneTrees;
    FindMore: ;
        size_t best_score = 0;
        TreeCountType::const_iterator synth_it;
        for(TreeCountType::const_iterator
            i = TreeCounts.begin();
            i != TreeCounts.end();
            ++i)
        {
            const fphash_t& hash = i->first;
            size_t         score = i->second.first;
            const CodeTree& tree = i->second.second;
            // It must always occur at least twice
            if(score < 2) continue;
            // And it must not be a simple expression
            if(tree.GetDepth() < 2) CandSkip: continue;
            // And it must not yet have been synthesized
            DoneTreesType::const_iterator j = AlreadyDoneTrees.lower_bound(hash);
            for(; j != AlreadyDoneTrees.end() && j->first == hash; ++j)
            {
                if(j->second.IsIdenticalTo(tree))
                    goto CandSkip;
            }
            // Is a candidate.
            score *= tree.GetDepth();
            if(score > best_score)
                { best_score = score; synth_it = i; }
        }
        if(best_score > 0)
        {
    #ifdef DEBUG_SUBSTITUTIONS
            std::cout << "Found Common Subexpression:"; FPoptimizer_Grammar::DumpTree(synth_it->second.second); std::cout << "\n";
    #endif
            /* Synthesize the selected tree */
            synth_it->second.second.SynthesizeByteCode(synth);
            /* Add the tree and all its children to the AlreadyDoneTrees list,
             * to prevent it from being re-synthesized
             */
            RememberRecursivelyHashList(AlreadyDoneTrees, synth_it->second.second);
            goto FindMore;
        }
      } // end scope for TreeCounts, AlreadyDoneTrees

    #ifdef DEBUG_SUBSTITUTIONS
        std::cout << "Actually synthesizing:\n";
        FPoptimizer_Grammar::DumpTreeWithIndent(*this);
    #endif
        /* Then synthesize the actual expression */
        SynthesizeByteCode(synth);
      #if 0
        /* Ensure that the expression result is
         * the only thing that remains in the stack
         */
        /* Removed: Fparser does not seem to care! */
        /* Seems that it is not required even when cEval is supported. */
        if(synth.GetStackTop() > 1)
            synth.DoPopNMov(0, synth.GetStackTop()-1);
      #endif
        synth.Pull(ByteCode, Immed, stacktop_max);
    }

    void CodeTree::SynthesizeByteCode(FPoptimizer_ByteCode::ByteCodeSynth& synth) const
    {
        // If the synth can already locate our operand in the stack,
        // never mind synthesizing it again, just dup it.
        if(synth.FindAndDup(*this))
        {
            return;
        }

        switch(GetOpcode())
        {
            case cVar:
                synth.PushVar(GetVar());
                break;
            case cImmed:
                synth.PushImmed(GetImmed());
                break;
            case cAdd:
            case cMul:
            case cMin:
            case cMax:
            case cAnd:
            case cOr:
            {
                if(GetOpcode() == cMul) // Special treatment for cMul sequences
                {
                    // If the paramlist contains an Immed, and that Immed
                    // fits in a long-integer, try to synthesize it
                    // as add-sequences instead.
                    for(size_t a=0; a<GetParamCount(); ++a)
                    {
                        if(GetParam(a).IsLongIntegerImmed())
                        {
                            long value = GetParam(a).GetLongIntegerImmed();

                            CodeTree tmp(*this, CodeTree::CloneTag());
                            tmp.DelParam(a);
                            tmp.Rehash();
                            bool success = AssembleSequence(
                                tmp, value, FPoptimizer_ByteCode::AddSequence,
                                synth,
                                MAX_MULI_BYTECODE_LENGTH);

                            if(success)
                            {
                                // this tree was treated just fine
                                synth.StackTopIs(*this);
                                return;
                            }
                        }
                    }
                }

                int n_stacked = 0;
                for(size_t a=0; a<GetParamCount(); ++a)
                {
                    GetParam(a).SynthesizeByteCode(synth);
                    ++n_stacked;

                    if(n_stacked > 1)
                    {
                        // Cumulate at the earliest opportunity.
                        synth.AddOperation(GetOpcode(), 2); // stack state: -2+1 = -1
                        n_stacked = n_stacked - 2 + 1;
                    }
                }
                if(n_stacked == 0)
                {
                    // Uh, we got an empty cAdd/cMul/whatever...
                    // Synthesize a default value.
                    // This should never happen.
                    switch(GetOpcode())
                    {
                        case cAdd:
                        case cOr:
                            synth.PushImmed(0);
                            break;
                        case cMul:
                        case cAnd:
                            synth.PushImmed(1);
                            break;
                        case cMin:
                        case cMax:
                            //synth.PushImmed(NaN);
                            synth.PushImmed(0);
                            break;
                        default:
                            break;
                    }
                    ++n_stacked;
                }
                assert(n_stacked == 1);
                break;
            }
            case cPow:
            {
                const CodeTree& p0 = GetParam(0);
                const CodeTree& p1 = GetParam(1);

                if(!p1.IsLongIntegerImmed()
                || !AssembleSequence( /* Optimize integer exponents */
                        p0, p1.GetLongIntegerImmed(),
                        FPoptimizer_ByteCode::MulSequence,
                        synth,
                        MAX_POWI_BYTECODE_LENGTH)
                  )
                {
                    p0.SynthesizeByteCode(synth);
                    p1.SynthesizeByteCode(synth);
                    synth.AddOperation(GetOpcode(), 2); // Create a vanilla cPow.
                }
                break;
            }
            case cIf:
            {
                size_t ofs;
                // If the parameter amount is != 3, we're screwed.
                GetParam(0).SynthesizeByteCode(synth); // expression
                synth.SynthIfStep1(ofs);
                GetParam(1).SynthesizeByteCode(synth); // true branch
                synth.SynthIfStep2(ofs);
                GetParam(2).SynthesizeByteCode(synth); // false branch
                synth.SynthIfStep3(ofs);
                break;
            }
            case cFCall:
            case cPCall:
            {
                // If the parameter count is invalid, we're screwed.
                for(size_t a=0; a<GetParamCount(); ++a)
                    GetParam(a).SynthesizeByteCode(synth);
                synth.AddOperation(GetOpcode(), (unsigned) GetParamCount());
                synth.AddOperation(GetFuncNo(), 0, 0);
                break;
            }
            default:
            {
                // If the parameter count is invalid, we're screwed.
                for(size_t a=0; a<GetParamCount(); ++a)
                    GetParam(a).SynthesizeByteCode(synth);
                synth.AddOperation(GetOpcode(), (unsigned) GetParamCount());
                break;
            }
        }
        synth.StackTopIs(*this);
    }
}

namespace
{
    bool AssembleSequence(
        const CodeTree& tree, long count,
        const FPoptimizer_ByteCode::SequenceOpCode& sequencing,
        FPoptimizer_ByteCode::ByteCodeSynth& synth,
        size_t max_bytecode_grow_length)
    {
        if(count != 0)
        {
            FPoptimizer_ByteCode::ByteCodeSynth backup = synth;

            tree.SynthesizeByteCode(synth);

            // Ignore the size generated by subtree
            size_t bytecodesize_backup = synth.GetByteCodeSize();

            FPoptimizer_ByteCode::AssembleSequence(count, sequencing, synth);

            size_t bytecode_grow_amount = synth.GetByteCodeSize() - bytecodesize_backup;
            if(bytecode_grow_amount > max_bytecode_grow_length)
            {
                synth = backup;
                return false;
            }
            return true;
        }
        else
        {
            FPoptimizer_ByteCode::AssembleSequence(count, sequencing, synth);
            return true;
        }
    }
}

#endif

#line 1 "fpoptimizer/fpoptimizer_bytecode_to_codetree.cc"
#include <cmath>
#include <cassert>

// line removed
// line removed
// line removed
// line removed
#include "fptypes.hh"

// line removed
#include "fparser.hh"


#ifdef FP_SUPPORT_OPTIMIZER

using namespace FUNCTIONPARSERTYPES;
//using namespace FPoptimizer_Grammar;

namespace
{
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
        const std::vector<unsigned>& ByteCode, size_t& IP,
        size_t limit,
        size_t factor_stack_base,
        FactorStack& stack)
    {
        double result = 1;
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
                result = -result;
                ++IP;
                continue;
            }
            if(ByteCode[IP] == opcodes.opcode_half)
            {
                if(IsIntegerConst(result) && result > 0 && ((long)result) % 2 == 0)
                    break;
                result *= 0.5;
                ++IP;
                continue;
            }
            if(ByteCode[IP] == opcodes.opcode_invhalf)
            {
                if(IsIntegerConst(result) && result > 0 && ((long)result) % 2 == 0)
                    break;
                result *= -0.5;
                ++IP;
                continue;
            }

            size_t dup_fetch_pos = IP;
            double lhs = 1.0;

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

    double ParsePowiSequence(const std::vector<unsigned>& ByteCode, size_t& IP,
                           size_t limit,
                           size_t factor_stack_base)
    {
        FactorStack stack;
        stack.push_back(1.0);
        return ParsePowiMuli(iseq_powi, ByteCode, IP, limit, factor_stack_base, stack);
    }

    double ParseMuliSequence(const std::vector<unsigned>& ByteCode, size_t& IP,
                           size_t limit,
                           size_t factor_stack_base)
    {
        FactorStack stack;
        stack.push_back(1.0);
        return ParsePowiMuli(iseq_muli, ByteCode, IP, limit, factor_stack_base, stack);
    }
}

namespace FPoptimizer_CodeTree
{
    class CodeTreeParserData
    {
    public:
        CodeTreeParserData() : stack() { }

        void Eat(size_t nparams, OPCODE opcode)
        {
            CodeTree newnode;
            newnode.SetOpcode(opcode);
            size_t stackhead = stack.size() - nparams;
            for(size_t a=0; a<nparams; ++a)
                newnode.AddParamMove( stack[stackhead + a] );

            switch(opcode)
            {
                //        asinh: log(x + sqrt(x*x + 1))
                //cAsinh [x] -> cLog (cAdd x (cPow (cAdd (cPow x 2) 1) 0.5))
                // Note: ^ Replacement function refers to x twice

                //        acosh: log(x + sqrt(x*x - 1))
                //cAcosh [x] -> cLog (cAdd x (cPow (cAdd (cPow x 2) -1) 0.5))

                //        atanh: log( (1+x) / (1-x)) / 2
                //cAtanh [x] -> cMul (cLog (cMul (cAdd 1 x) (cPow (cAdd 1 (cMul -1 x)) -1))) 0.5

                //     The hyperbolic functions themselves are:
                //        sinh: (exp(x)-exp(-x)) / 2  = exp(-x) * (exp(2*x)-1) / 2
                //cSinh [x] -> cMul 0.5 (cPow [CONSTANT_EI x]) (cAdd [-1 (cPow [CONSTANT_2E x])])

                //        cosh: (exp(x)+exp(-x)) / 2  = exp(-x) * (exp(2*x)+1) / 2
                //        cosh(-x) = cosh(x)
                //cCosh [x] -> cMul 0.5 (cPow [CONSTANT_EI x]) (cAdd [ 1 (cPow [CONSTANT_2E x])])

                //        tanh: sinh/cosh = (exp(2*x)-1) / (exp(2*x)+1)
                //cTanh [x] -> (cMul (cAdd {(cPow [CONSTANT_2E x]) -1}) (cPow [(cAdd {(cPow [CONSTANT_2E x]) 1}) -1]))
                case cTanh:
                {
                    CodeTree sinh, cosh;
                    sinh.SetOpcode(cSinh); sinh.AddParam(newnode.GetParam(0)); sinh.Rehash();
                    cosh.SetOpcode(cCosh); cosh.AddParamMove(newnode.GetParam(0)); cosh.Rehash();
                    CodeTree pow;
                    pow.SetOpcode(cPow);
                    pow.AddParamMove(cosh);
                    pow.AddParam(CodeTree(-1.0));
                    pow.Rehash();
                    newnode.SetOpcode(cMul);
                    newnode.SetParamMove(0, sinh);
                    newnode.AddParamMove(pow);
                    break;
                }

                //        tan: sin/cos
                //cTan [x] -> (cMul (cSin [x]) (cPow [(cCos [x]) -1]))
                case cTan:
                {
                    CodeTree sin, cos;
                    sin.SetOpcode(cSin); sin.AddParam(newnode.GetParam(0)); sin.Rehash();
                    cos.SetOpcode(cCos); cos.AddParamMove(newnode.GetParam(0)); cos.Rehash();
                    CodeTree pow;
                    pow.SetOpcode(cPow);
                    pow.AddParamMove(cos);
                    pow.AddParam(CodeTree(-1.0));
                    pow.Rehash();
                    newnode.SetOpcode(cMul);
                    newnode.SetParamMove(0, sin);
                    newnode.AddParamMove(pow);
                    break;
                }

                case cPow:
                {
                    const CodeTree& p0 = newnode.GetParam(0);
                    const CodeTree& p1 = newnode.GetParam(1);
                    if(p1.GetOpcode() == cAdd)
                    {
                        // convert x^(a + b) into x^a * x^b just so that
                        // some optimizations can be run on it.
                        // For instance, exp(log(x)*-61.1 + log(z)*-59.1)
                        // won't be changed into exp(log(x*z)*-61.1)*z^2
                        // unless we do this.
                        std::vector<CodeTree> mulgroup(p1.GetParamCount());
                        for(size_t a=0; a<p1.GetParamCount(); ++a)
                        {
                            CodeTree pow;
                            pow.SetOpcode(cPow);
                            pow.AddParam(p0);
                            pow.AddParam(p1.GetParam(a));
                            pow.Rehash();
                            mulgroup[a].swap(pow);
                        }
                        newnode.SetOpcode(cMul);
                        newnode.SetParamsMove(mulgroup);
                    }
                    break;
                }

                // Should we change sin(x) into cos(pi/2-x)
                //               or cos(x) into sin(pi/2-x)?
                //                        note: cos(x-pi/2) = cos(pi/2-x) = sin(x)
                //                        note: sin(x-pi/2) = -sin(pi/2-x) = -cos(x)
                default: break;
            }

            newnode.Rehash();
        /*
            using namespace FPoptimizer_Grammar;
            bool recurse = false;
            while(ApplyGrammar(pack.glist[0], newnode, recurse)) // intermediate
            { //std::cout << "Rerunning 1\n";
                FixIncompleteHashes(newnode);
                recurse = true;
            }
        */
            FindClone(newnode, false);
        #ifdef DEBUG_SUBSTITUTIONS
            std::cout << "POP " << nparams << ", " << FP_GetOpcodeName(opcode)
                      << "->" << FP_GetOpcodeName(newnode.GetOpcode())
                      << ": PUSH ";
            FPoptimizer_Grammar::DumpTree(newnode);
            std::cout <<std::endl;
        #endif
            stack.resize(stackhead+1);
            stack.back().swap(newnode);
        }

        void EatFunc(size_t nparams, OPCODE opcode, unsigned funcno)
        {
            CodeTree newnode;
            newnode.SetFuncOpcode(opcode, funcno);
            size_t stackhead = stack.size() - nparams;
            for(size_t a=0; a<nparams; ++a)
                newnode.AddParamMove( stack[stackhead + a] );
            newnode.Rehash(false);
        #ifdef DEBUG_SUBSTITUTIONS
            std::cout << "POP " << nparams << ", PUSH ";
            FPoptimizer_Grammar::DumpTree(newnode);
            std::cout << std::endl;
        #endif
            FindClone(newnode);
            stack.resize(stackhead+1);
            stack.back().swap(newnode);
        }

        void AddConst(double value)
        {
            CodeTree newnode(value);
            FindClone(newnode);
            Push(newnode);
        }

        void AddVar(unsigned varno)
        {
            CodeTree newnode(varno, CodeTree::VarTag());
            FindClone(newnode);
            Push(newnode);
        }

        void SwapLastTwoInStack()
        {
            stack[stack.size()-1].swap( stack[stack.size()-2] );
        }

        void Dup()
        {
            Fetch(stack.size()-1);
        }

        void Fetch(size_t which)
        {
            Push(stack[which]);
        }

        template<typename T>
        void Push(T tree)
        {
        #ifdef DEBUG_SUBSTITUTIONS
            std::cout << "PUSH ";
            FPoptimizer_Grammar::DumpTree(tree);
            std::cout << std::endl;
        #endif
            stack.push_back(tree);
        }

        void PopNMov(size_t target, size_t source)
        {
            stack[target] = stack[source];
            stack.resize(target+1);
        }

        CodeTree PullResult()
        {
            clones.clear();
            CodeTree result(stack.back());
            stack.resize(stack.size()-1);
            return result;
        }

        size_t GetStackTop() const { return stack.size(); }
    private:
        void FindClone(CodeTree& tree, bool recurse = true)
        {
            std::multimap<fphash_t, CodeTree>::const_iterator
                i = clones.lower_bound(tree.GetHash());
            for(; i != clones.end() && i->first == tree.GetHash(); ++i)
            {
                if(i->second.IsIdenticalTo(tree))
                    tree.Become(i->second);
            }
            if(recurse)
                for(size_t a=0; a<tree.GetParamCount(); ++a)
                    FindClone(tree.GetParam(a));
            clones.insert(std::make_pair(tree.GetHash(), tree));
        }
    private:
        std::vector<CodeTree> stack;
        std::multimap<fphash_t, CodeTree> clones;

    private:
        CodeTreeParserData(const CodeTreeParserData&);
        CodeTreeParserData& operator=(const CodeTreeParserData&);
    };

    struct IfInfo
    {
        CodeTree condition;
        CodeTree thenbranch;
        size_t endif_location;
    };

    void CodeTree::GenerateFrom(
        const std::vector<unsigned>& ByteCode,
        const std::vector<double>& Immed,
        const FunctionParser::Data& fpdata)
    {
        CodeTreeParserData sim;
        std::vector<IfInfo> if_stack;

        for(size_t IP=0, DP=0; ; ++IP)
        {
        after_powi:
            while(!if_stack.empty() && if_stack.back().endif_location == IP)
            {
                // The "else" of an "if" ends here
                CodeTree elsebranch = sim.PullResult();
                sim.Push(if_stack.back().condition);
                sim.Push(if_stack.back().thenbranch);
                sim.Push(elsebranch);
                sim.Eat(3, cIf);
                if_stack.pop_back();
            }
            if(IP >= ByteCode.size()) break;

            unsigned opcode = ByteCode[IP];
            if(opcode == cSqr || opcode == cDup
            || opcode == cInv || opcode == cNeg
            || opcode == cSqrt || opcode == cRSqrt
            || opcode == cFetch)
            {
                // Parse a powi sequence
                //size_t was_ip = IP;
                double exponent = ParsePowiSequence(
                    ByteCode, IP, if_stack.empty() ? ByteCode.size() : if_stack.back().endif_location,
                    sim.GetStackTop()-1);
                if(exponent != 1.0)
                {
                    //std::cout << "Found exponent at " << was_ip << ": " << exponent << "\n";
                    sim.AddConst(exponent);
                    sim.Eat(2, cPow);
                    goto after_powi;
                }
                if(opcode == cDup
                || opcode == cFetch
                || opcode == cNeg)
                {
                    double factor = ParseMuliSequence(
                        ByteCode, IP, if_stack.empty() ? ByteCode.size() : if_stack.back().endif_location,
                        sim.GetStackTop()-1);
                    if(factor != 1.0)
                    {
                        //std::cout << "Found factor at " << was_ip << ": " << factor << "\n";
                        sim.AddConst(factor);
                        sim.Eat(2, cMul);
                        goto after_powi;
                    }
                }
            }
            if(OPCODE(opcode) >= VarBegin)
            {
                sim.AddVar(opcode);
            }
            else
            {
                switch( OPCODE(opcode) )
                {
                    // Specials
                    case cIf:
                    {
                        if_stack.resize(if_stack.size() + 1);
                        CodeTree res( sim.PullResult() );
                        if_stack.back().condition.swap( res );
                        if_stack.back().endif_location = ByteCode.size();
                        IP += 2; // dp,sp for elsebranch are irrelevant.
                        continue;
                    }
                    case cJump:
                    {
                        CodeTree res( sim.PullResult() );
                        if_stack.back().thenbranch.swap( res );
                        if_stack.back().endif_location = ByteCode[IP+1]+1;
                        IP += 2;
                        continue;
                    }
                    case cImmed:
                        sim.AddConst(Immed[DP++]);
                        break;
                    case cDup:
                        sim.Dup();
                        break;
                    case cNop:
                        break;
                    case cFCall:
                    {
                        unsigned funcno = ByteCode[++IP];
                        unsigned params = fpdata.FuncPtrs[funcno].params;
                        sim.EatFunc(params, OPCODE(opcode), funcno);
                        break;
                    }
                    case cPCall:
                    {
                        unsigned funcno = ByteCode[++IP];
                        unsigned params = fpdata.FuncParsers[funcno].params;
                        sim.EatFunc(params, OPCODE(opcode), funcno);
                        break;
                    }
                    // Unary operators requiring special attention
                    /*case cInv:  // already handled by powi_opt
                        sim.AddConst(-1);
                        sim.Eat(2, cPow); // 1/x is x^-1
                        break; */
                    /*case cNeg: // already handled by powi_opt
                        sim.AddConst(-1);
                        sim.Eat(2, cMul); // -x is x*-1
                        break;*/
                    /*case cSqr: // already handled by powi_opt
                        sim.AddConst(2.0);
                        sim.Eat(2, cPow);
                        break;*/
                    // Unary functions requiring special attention
                    /*case cSqrt: // already handled by powi_opt
                        sim.AddConst(0.5);
                        sim.Eat(2, cPow);
                        break;*/
                    /*case cRSqrt: // already handled by powi_opt
                        sim.AddConst(-0.5);
                        sim.Eat(2, cPow);
                        break; */
                    case cDeg:
                        sim.AddConst(CONSTANT_DR);
                        sim.Eat(2, cMul);
                        break;
                    case cRad:
                        sim.AddConst(CONSTANT_RD);
                        sim.Eat(2, cMul);
                        break;
                    case cExp:
                        sim.AddConst(CONSTANT_E);
                        sim.SwapLastTwoInStack();
                        sim.Eat(2, cPow);
                        break;
                    case cExp2: // from fpoptimizer
                        sim.AddConst(2.0);
                        sim.SwapLastTwoInStack();
                        sim.Eat(2, cPow);
                        break;
                    case cCot:
                        sim.Eat(1, cTan);
                        sim.AddConst(-1);
                        sim.Eat(2, cPow);
                        break;
                    case cCsc:
                        sim.Eat(1, cSin);
                        sim.AddConst(-1);
                        sim.Eat(2, cPow);
                        break;
                    case cSec:
                        sim.Eat(1, cCos);
                        sim.AddConst(-1);
                        sim.Eat(2, cPow);
                        break;
                    case cLog10:
                        sim.Eat(1, cLog);
                        sim.AddConst(CONSTANT_L10I);
                        sim.Eat(2, cMul);
                        break;
                    case cLog2:
                        sim.Eat(1, cLog);
                        sim.AddConst(CONSTANT_L2I);
                        sim.Eat(2, cMul);
                        break;
                    //case cLog:
                    //    sim.Eat(1, cLog2);
                    //    sim.AddConst(CONSTANT_L2);
                    //    sim.Eat(2, cMul);
                    //    break;
                    // Binary operators requiring special attention
                    case cSub:
                        sim.AddConst(-1);
                        sim.Eat(2, cMul); // -x is x*-1
                        sim.Eat(2, cAdd); // Minus is negative adding
                        break;
                    case cRSub: // from fpoptimizer
                        sim.SwapLastTwoInStack();
                        sim.AddConst(-1);
                        sim.Eat(2, cMul); // -x is x*-1
                        sim.Eat(2, cAdd);
                        break;
                    case cDiv:
                        sim.AddConst(-1);
                        sim.Eat(2, cPow); // 1/x is x^-1
                        sim.Eat(2, cMul); // Divide is inverse multiply
                        break;
                    case cRDiv: // from fpoptimizer
                        sim.SwapLastTwoInStack();
                        sim.AddConst(-1);
                        sim.Eat(2, cPow); // 1/x is x^-1
                        sim.Eat(2, cMul); // Divide is inverse multiply
                        break;
                    case cRPow:
                        sim.SwapLastTwoInStack();
                        sim.Eat(2, cPow);
                        break;
                    // Binary operators not requiring special attention
                    case cAdd: case cMul:
                    case cMod: case cPow:
                    case cEqual: case cLess: case cGreater:
                    case cNEqual: case cLessOrEq: case cGreaterOrEq:
                    case cAnd: case cOr:
                        sim.Eat(2, OPCODE(opcode));
                        break;
                    // Unary operators not requiring special attention
                    case cNot:
                    case cNotNot: // from fpoptimizer
                        sim.Eat(1, OPCODE(opcode));
                        break;
                    // Special opcodes generated by fpoptimizer itself
                    case cFetch:
                        sim.Fetch(ByteCode[++IP]);
                        break;
                    case cPopNMov:
                    {
                        unsigned stackOffs_target = ByteCode[++IP];
                        unsigned stackOffs_source = ByteCode[++IP];
                        sim.PopNMov(stackOffs_target, stackOffs_source);
                        break;
                    }
                    // Note: cVar should never be encountered in bytecode.
                    // Other functions
#ifndef FP_DISABLE_EVAL
                    case cEval:
                    {
                        size_t paramcount = fpdata.variableRefs.size();
                        sim.Eat(paramcount, OPCODE(opcode));
                        break;
                    }
#endif
                    default:
                        unsigned funcno = opcode-cAbs;
                        assert(funcno < FUNC_AMOUNT);
                        const FuncDefinition& func = Functions[funcno];
                        sim.Eat(func.params, OPCODE(opcode));
                        break;
                }
            }
        }
        Become(sim.PullResult());
    #ifdef DEBUG_SUBSTITUTIONS
        std::cout << "Produced tree:\n";
        FPoptimizer_Grammar::DumpTreeWithIndent(*this);
    #endif
    }
}

#endif

#line 1 "fpoptimizer/fpoptimizer_constantfolding.cc"
// line removed
// line removed
// line removed

#include <cmath> /* for CalculateResultBoundaries() */
#include <algorithm>

#include "fpconfig.hh"
#include "fparser.hh"
#include "fptypes.hh"

#ifdef FP_SUPPORT_OPTIMIZER

using namespace FUNCTIONPARSERTYPES;
using namespace FPoptimizer_CodeTree;

#define FP_MUL_COMBINE_EXPONENTS

#ifdef _MSC_VER
#include <float.h>
#define isinf(x) (!_finite(x))
#endif

namespace
{
    struct ComparisonSet /* For optimizing And, Or */
    {
        static const int Lt_Mask = 0x1; // 1=less
        static const int Eq_Mask = 0x2; // 2=equal
        static const int Le_Mask = 0x3; // 1+2 = Less or Equal
        static const int Gt_Mask = 0x4; // 4=greater
        static const int Ne_Mask = 0x5; // 4+1 = Greater or Less, i.e. Not equal
        static const int Ge_Mask = 0x6; // 4+2 = Greater or Equal
        static int Swap_Mask(int m) { return (m&Eq_Mask)
                                  | ((m&Lt_Mask) ? Gt_Mask : 0)
                                  | ((m&Gt_Mask) ? Lt_Mask : 0); }
        struct Comparison
        {
            CodeTree a;
            CodeTree b;
            int relationship;
        };
        std::vector<Comparison> relationships;
        struct Item
        {
            CodeTree value;
            bool negated;
        };
        std::vector<Item> plain_set;

        enum RelationshipResult
        {
            Ok,
            BecomeZero,
            BecomeOne,
            Suboptimal
        };

        RelationshipResult AddItem(const CodeTree& a, bool negated, bool is_or)
        {
            for(size_t c=0; c<plain_set.size(); ++c)
                if(plain_set[c].value.IsIdenticalTo(a))
                {
                    if(negated != plain_set[c].negated)
                        return is_or ? BecomeOne : BecomeZero;
                    return Suboptimal;
                }
            Item pole;
            pole.value   = a;
            pole.negated = negated;
            plain_set.push_back(pole);
            return Ok;
        }

        RelationshipResult AddRelationship(CodeTree a, CodeTree b, int reltype, bool is_or)
        {
            if(is_or)
            {
                if(reltype == 7) return BecomeOne;
            }
            else
            {
                if(reltype == 0) return BecomeZero;
            }

            if(!(a.GetHash() < b.GetHash()))
            {
                a.swap(b);
                reltype = Swap_Mask(reltype);
            }

            for(size_t c=0; c<relationships.size(); ++c)
            {
                if(relationships[c].a.IsIdenticalTo(a)
                && relationships[c].b.IsIdenticalTo(b))
                {
                    if(is_or)
                    {
                        int newrel = relationships[c].relationship | reltype;
                        if(newrel == 7) return BecomeOne;
                        relationships[c].relationship = newrel;
                    }
                    else
                    {
                        int newrel = relationships[c].relationship & reltype;
                        if(newrel == 0) return BecomeZero;
                        relationships[c].relationship = newrel;
                    }
                    return Suboptimal;
                }
            }
            Comparison comp;
            comp.a = a;
            comp.b = b;
            comp.relationship = reltype;
            relationships.push_back(comp);
            return Ok;
        }

        RelationshipResult AddAndRelationship(CodeTree a, CodeTree b, int reltype)
        {
            return AddRelationship(a, b, reltype, false);
        }

        RelationshipResult AddOrRelationship(CodeTree a, CodeTree b, int reltype)
        {
            return AddRelationship(a, b, reltype, true);
        }
    };

    struct CollectionSet /* For optimizing Add,  Mul */
    {
        struct Collection
        {
            CodeTree value;
            CodeTree factor;
            bool factor_needs_rehashing;

            Collection() : value(),factor(), factor_needs_rehashing(false) { }
            Collection(const CodeTree& v, const CodeTree& f)
                : value(v), factor(f), factor_needs_rehashing(false) { }
        };
        std::multimap<fphash_t, Collection> collections;

        enum CollectionResult
        {
            Ok,
            Suboptimal
        };

        typedef std::multimap<fphash_t, Collection>::iterator PositionType;

        PositionType FindIdenticalValueTo(const CodeTree& value)
        {
            fphash_t hash = value.GetHash();
            for(PositionType
                i = collections.lower_bound(hash);
                i != collections.end() && i->first == hash;
                ++i)
            {
                if(value.IsIdenticalTo(i->second.value))
                    return i;
            }
            return collections.end();
        }
        bool Found(const PositionType& b) { return b != collections.end(); }

        CollectionResult AddCollectionTo(const CodeTree& factor,
                                         const PositionType& into_which)
        {
            Collection& c = into_which->second;
            if(c.factor_needs_rehashing)
                c.factor.AddParam(factor);
            else
            {
                CodeTree add;
                add.SetOpcode(cAdd);
                add.AddParamMove(c.factor);
                add.AddParam(factor);
                c.factor.swap(add);
                c.factor_needs_rehashing = true;
            }
            return Suboptimal;
        }

        CollectionResult AddCollection(const CodeTree& value, const CodeTree& factor)
        {
            const fphash_t hash = value.GetHash();
            PositionType i = collections.lower_bound(hash);
            for(; i != collections.end() && i->first == hash; ++i)
            {
                if(i->second.value.IsIdenticalTo(value))
                    return AddCollectionTo(factor, i);
            }
            collections.insert(
                i,
                std::make_pair( hash, Collection(value, factor) ) );
            return Ok;
        }

        CollectionResult AddCollection(const CodeTree& a)
        {
            return AddCollection(a, CodeTree(1.0) );
        }
    };

    struct Select2ndRev
    {
        template<typename T>
        inline bool operator() (const T& a, const T& b) const
        {
            return a.second > b.second;
        }
    };
    struct Select1st
    {
        template<typename T1, typename T2>
        inline bool operator() (const std::pair<T1,T2>& a,
                                const std::pair<T1,T2>& b) const
        {
            return a.first < b.first;
        }

        template<typename T1, typename T2>
        inline bool operator() (const std::pair<T1,T2>& a, T1 b) const
        {
            return a.first < b;
        }

        template<typename T1, typename T2>
        inline bool operator() (T1 a, const std::pair<T1,T2>& b) const
        {
            return a < b.first;
        }
    };

    bool IsEvenIntegerConst(double v)
    {
        return IsIntegerConst(v) && ((long)v % 2) == 0;
    }

    struct ConstantExponentCollection
    {
        typedef std::pair<double, std::vector<CodeTree> > ExponentInfo;
        std::vector<ExponentInfo> data;

        void MoveToSet_Unique(double exponent, std::vector<CodeTree>& source_set)
        {
            data.push_back( std::pair<double, std::vector<CodeTree> >
                            (exponent, std::vector<CodeTree>() ) );
            data.back().second.swap(source_set);
        }
        void MoveToSet_NonUnique(double exponent, std::vector<CodeTree>& source_set)
        {
            std::vector<ExponentInfo>::iterator i
                = std::lower_bound(data.begin(), data.end(), exponent, Select1st());
            if(i != data.end() && i->first == exponent)
            {
                i->second.insert(i->second.end(), source_set.begin(), source_set.end());
            }
            else
            {
                //MoveToSet_Unique(exponent, source_set);
                data.insert(i,  std::pair<double, std::vector<CodeTree> >
                                (exponent, source_set) );
            }
        }

        bool Optimize()
        {
            /* TODO: Group them such that:
             *
             *      x^3 *         z^2 becomes (x*z)^2 * x^1
             *      x^3 * y^2.5 * z^2 becomes (x*z*y)^2 * y^0.5 * x^1
             *                    rather than (x*y*z)^2 * (x*y)^0.5 * x^0.5
             *
             *      x^4.5 * z^2.5     becomes (z * x)^2.5 * x^2
             *                        becomes (x*z*x)^2 * (z*x)^0.5
             *                        becomes (z*x*x*z*x)^0.5 * (z*x*x)^1.5 -- buzz, bad.
             *
             */
            bool changed = false;
            std::sort( data.begin(), data.end(), Select1st() );
        redo:
            /* Supposed algorithm:
             * For the smallest pair of data[] where the difference
             * between the two is a "neat value" (x*16 is positive integer),
             * do the combining as indicated above.
             */
            /*
             * NOTE: Hanged in Testbed test P44, looping the following
             *       (Var0 ^ 0.75) * ((1.5 * Var0) ^ 1.0)
             *     = (Var0 ^ 1.75) *  (1.5         ^ 1.0)
             *       Fixed by limiting to cases where (exp_a != 1.0).
             *
             * NOTE: Converting (x*z)^0.5 * x^16.5
             *              into x^17 * z^0.5
             *       is handled by code within CollectMulGroup().
             *       However, bacause it is prone for infinite looping,
             *       the use of "IsIdenticalTo(before)" is added at the
             *       end of ConstantFolding_MulGrouping().
             *
             *       This algorithm could make it into (x*z*x)^0.5 * x^16,
             *       but this is wrong, for it falsely includes x^evenint.. twice.
             */
            for(size_t a=0; a<data.size(); ++a)
            {
                double exp_a = data[a].first;
                if(FloatEqual(exp_a, 1.0)) continue;
                for(size_t b=a+1; b<data.size(); ++b)
                {
                    double exp_b = data[b].first;
                    double exp_diff = exp_b - exp_a;
                    if(exp_diff >= fabs(exp_a)) break;
                    double exp_diff_still_probable_integer = exp_diff * 16.0;
                    if(IsIntegerConst(exp_diff_still_probable_integer)
                    && !(IsIntegerConst(exp_b) && !IsIntegerConst(exp_diff))
                      )
                    {
                        /* When input is x^3 * z^2,
                         * exp_a = 2
                         * a_set = z
                         * exp_b = 3
                         * b_set = x
                         * exp_diff = 3-2 = 1
                         */
                        std::vector<CodeTree>& a_set = data[a].second;
                        std::vector<CodeTree>& b_set = data[b].second;
          #ifdef DEBUG_SUBSTITUTIONS
                        std::cout << "Before ConstantExponentCollection iteration:\n";
                        Dump(std::cout);
          #endif
                        if(IsIntegerConst(exp_b)
                        && IsEvenIntegerConst(exp_b)
                        //&& !IsEvenIntegerConst(exp_diff)
                        && !IsEvenIntegerConst(exp_diff+exp_a))
                        {
                            CodeTree tmp2;
                            tmp2.SetOpcode(cMul);
                            tmp2.SetParamsMove(b_set);
                            tmp2.Rehash();
                            CodeTree tmp;
                            tmp.SetOpcode(cAbs);
                            tmp.AddParamMove(tmp2);
                            tmp.Rehash();
                            b_set.resize(1);
                            b_set[0].swap(tmp);
                        }

                        a_set.insert(a_set.end(), b_set.begin(), b_set.end());

                        std::vector<CodeTree> b_copy = b_set;
                        data.erase(data.begin() + b);
                        MoveToSet_NonUnique(exp_diff, b_copy);
                        changed = true;

          #ifdef DEBUG_SUBSTITUTIONS
                        std::cout << "After ConstantExponentCollection iteration:\n";
                        Dump(std::cout);
          #endif
                        goto redo;
                    }
                }
            }
            return changed;
        }

    #ifdef DEBUG_SUBSTITUTIONS
        void Dump(std::ostream& out)
        {
            for(size_t a=0; a<data.size(); ++a)
            {
                out.precision(12);
                out << data[a].first << ": ";
                for(size_t b=0; b<data[a].second.size(); ++b)
                {
                    if(b > 0) out << '*';
                    FPoptimizer_Grammar::DumpTree(data[a].second[b], out);
                }
                out << std::endl;
            }
        }
    #endif

    };
}

namespace FPoptimizer_CodeTree
{
    void CodeTree::ConstantFolding_FromLogicalParent()
    {
    redo:;
        switch(GetOpcode())
        {
            case cNotNot:
            //ReplaceTreeWithParam0:
                Become(GetParam(0));
                goto redo;
            case cIf:
                CopyOnWrite();
                while(GetParam(1).GetOpcode() == cNotNot)
                    SetParamMove(1, GetParam(1).GetUniqueRef().GetParam(0));
                GetParam(1).ConstantFolding_FromLogicalParent();

                while(GetParam(2).GetOpcode() == cNotNot)
                    SetParamMove(2, GetParam(2).GetUniqueRef().GetParam(0));
                GetParam(2).ConstantFolding_FromLogicalParent();

                Rehash();
                break;
            default: break;
        }
    }

    bool CodeTree::ConstantFolding_LogicCommon(bool is_or)
    {
        bool should_regenerate = false;
        ComparisonSet comp;
        for(size_t a=0; a<GetParamCount(); ++a)
        {
            ComparisonSet::RelationshipResult change = ComparisonSet::Ok;
            const CodeTree& atree = GetParam(a);
            switch(atree.GetOpcode())
            {
                case cEqual:
                    change = comp.AddRelationship(atree.GetParam(0), atree.GetParam(1), ComparisonSet::Eq_Mask, is_or);
                    break;
                case cNEqual:
                    change = comp.AddRelationship(atree.GetParam(0), atree.GetParam(1), ComparisonSet::Ne_Mask, is_or);
                    break;
                case cLess:
                    change = comp.AddRelationship(atree.GetParam(0), atree.GetParam(1), ComparisonSet::Lt_Mask, is_or);
                    break;
                case cLessOrEq:
                    change = comp.AddRelationship(atree.GetParam(0), atree.GetParam(1), ComparisonSet::Le_Mask, is_or);
                    break;
                case cGreater:
                    change = comp.AddRelationship(atree.GetParam(0), atree.GetParam(1), ComparisonSet::Gt_Mask, is_or);
                    break;
                case cGreaterOrEq:
                    change = comp.AddRelationship(atree.GetParam(0), atree.GetParam(1), ComparisonSet::Ge_Mask, is_or);
                    break;
                case cNot:
                    change = comp.AddItem(atree.GetParam(0), true, is_or);
                    break;
                default:
                    change = comp.AddItem(atree, false, is_or);
            }
            switch(change)
            {
            ReplaceTreeWithZero:
                    data = new CodeTreeData(0.0);
                    return true;
            ReplaceTreeWithOne:
                    data = new CodeTreeData(1.0);
                    return true;
                case ComparisonSet::Ok: // ok
                    break;
                case ComparisonSet::BecomeZero: // whole set was invalidated
                    goto ReplaceTreeWithZero;
                case ComparisonSet::BecomeOne: // whole set was validated
                    goto ReplaceTreeWithOne;
                case ComparisonSet::Suboptimal: // something was changed
                    should_regenerate = true;
                    break;
            }
        }
        if(should_regenerate)
        {
          #ifdef DEBUG_SUBSTITUTIONS
            std::cout << "Before ConstantFolding_LogicCommon: "; FPoptimizer_Grammar::DumpTree(*this);
            std::cout << "\n";
          #endif
            DelParams();
            for(size_t a=0; a<comp.plain_set.size(); ++a)
            {
                if(comp.plain_set[a].negated)
                {
                    CodeTree r;
                    r.SetOpcode(cNot);
                    r.AddParamMove(comp.plain_set[a].value);
                    r.Rehash();
                    AddParamMove(r);
                }
                else
                    AddParamMove(comp.plain_set[a].value);
            }
            for(size_t a=0; a<comp.relationships.size(); ++a)
            {
                CodeTree r;
                r.SetOpcode(cAtan2);
                switch(comp.relationships[a].relationship)
                {
                    case ComparisonSet::Lt_Mask: r.SetOpcode( cLess ); break;
                    case ComparisonSet::Eq_Mask: r.SetOpcode( cEqual ); break;
                    case ComparisonSet::Gt_Mask: r.SetOpcode( cGreater ); break;
                    case ComparisonSet::Le_Mask: r.SetOpcode( cLessOrEq ); break;
                    case ComparisonSet::Ne_Mask: r.SetOpcode( cNEqual ); break;
                    case ComparisonSet::Ge_Mask: r.SetOpcode( cGreaterOrEq ); break;
                }
                r.AddParamMove(comp.relationships[a].a);
                r.AddParamMove(comp.relationships[a].b);
                r.Rehash();
                AddParamMove(r);
            }
          #ifdef DEBUG_SUBSTITUTIONS
            std::cout << "After ConstantFolding_LogicCommon: "; FPoptimizer_Grammar::DumpTree(*this);
            std::cout << "\n";
          #endif
            return true;
        }
        /*
        Note: One thing this does not yet do, is to detect chains
              such as x=y & y=z & x=z, which could be optimized
              to x=y & x=z.
        */
        return false;
    }

    bool CodeTree::ConstantFolding_AndLogic()
    {
        return ConstantFolding_LogicCommon(false);
    }
    bool CodeTree::ConstantFolding_OrLogic()
    {
        return ConstantFolding_LogicCommon(true);
    }

    static CodeTree CollectMulGroup_Item(
        CodeTree& value,
        bool& has_highlevel_opcodes)
    {
        switch(value.GetOpcode())
        {
            case cPow:
            {
                CodeTree exponent = value.GetParam(1);
                value.Become( value.GetParam(0) );
                return exponent;
            }
            case cSqrt:
                value.Become( value.GetParam(0) );
                has_highlevel_opcodes = true;
                return CodeTree(0.5);
            case cRSqrt:
                value.Become( value.GetParam(0) );
                has_highlevel_opcodes = true;
                return CodeTree(-0.5);
            case cInv:
                value.Become( value.GetParam(0) );
                has_highlevel_opcodes = true;
                return CodeTree(-1.0);
            default: break;
        }
        return CodeTree(1.0);
    }

    static void CollectMulGroup(
        CollectionSet& mul, const CodeTree& tree, const CodeTree& factor,
        bool& should_regenerate,
        bool& has_highlevel_opcodes
    )
    {
        for(size_t a=0; a<tree.GetParamCount(); ++a)
        {
            CodeTree value(tree.GetParam(a));

            CodeTree exponent ( CollectMulGroup_Item(value, has_highlevel_opcodes) );

            if(!factor.IsImmed() || factor.GetImmed() != 1.0)
            {
                CodeTree new_exp;
                new_exp.SetOpcode(cMul);
                new_exp.AddParam( exponent );
                new_exp.AddParam( factor );
                new_exp.Rehash();
                exponent.swap( new_exp );
            }
        #if 0 /* FIXME: This does not work */
            if(value.GetOpcode() == cMul)
            {
                if(1)
                {
                    // Avoid erroneously converting
                    //          (x*z)^0.5 * z^2
                    // into     x^0.5 * z^2.5
                    // It should be x^0.5 * abs(z)^2.5, but this is not a good conversion.
                    bool exponent_is_even = exponent.IsImmed() && IsEvenIntegerConst(exponent.GetImmed());

                    for(size_t b=0; b<value.GetParamCount(); ++b)
                    {
                        bool tmp=false;
                        CodeTree val(value.GetParam(b));
                        CodeTree exp(CollectMulGroup_Item(val, tmp));
                        if(exponent_is_even
                        || (exp.IsImmed() && IsEvenIntegerConst(exp.GetImmed())))
                        {
                            CodeTree new_exp;
                            new_exp.SetOpcode(cMul);
                            new_exp.AddParam(exponent);
                            new_exp.AddParamMove(exp);
                            new_exp.ConstantFolding();
                            if(!new_exp.IsImmed() || !IsEvenIntegerConst(new_exp.GetImmed()))
                            {
                                goto cannot_adopt_mul;
                            }
                        }
                    }
                }
                CollectMulGroup(mul, value, exponent,
                                should_regenerate,
                                has_highlevel_opcodes);
            }
            else cannot_adopt_mul:
        #endif
            {
                if(mul.AddCollection(value, exponent) == CollectionSet::Suboptimal)
                    should_regenerate = true;
            }
        }
    }

    bool CodeTree::ConstantFolding_MulGrouping()
    {
        bool has_highlevel_opcodes = false;
        bool should_regenerate = false;
        CollectionSet mul;

        CollectMulGroup(mul, *this, CodeTree(1.0),
                        should_regenerate,
                        has_highlevel_opcodes);

        typedef std::pair<CodeTree/*exponent*/,
                          std::vector<CodeTree>/*base value (mul group)*/
                         > exponent_list;
        typedef std::multimap<fphash_t,/*exponent hash*/
                              exponent_list> exponent_map;
        exponent_map by_exponent;

        for(CollectionSet::PositionType
            j = mul.collections.begin();
            j != mul.collections.end();
            ++j)
        {
            CodeTree& value = j->second.value;
            CodeTree& exponent = j->second.factor;
            if(j->second.factor_needs_rehashing) exponent.Rehash();
            const fphash_t exponent_hash = exponent.GetHash();

            exponent_map::iterator i = by_exponent.lower_bound(exponent_hash);
            for(; i != by_exponent.end() && i->first == exponent_hash; ++i)
                if(i->second.first.IsIdenticalTo(exponent))
                {
                    if(!exponent.IsImmed() || !FloatEqual(exponent.GetImmed(), 1.0))
                        should_regenerate = true;
                    i->second.second.push_back(value);
                    goto skip_b;
                }
            by_exponent.insert(i, std::make_pair(exponent_hash,
                std::make_pair(exponent,
                               std::vector<CodeTree> (size_t(1), value)
                              )));
        skip_b:;
        }

    #ifdef FP_MUL_COMBINE_EXPONENTS
        ConstantExponentCollection by_float_exponent;
        for(exponent_map::iterator
            j,i = by_exponent.begin();
            i != by_exponent.end();
            i=j)
        {
            j=i; ++j;
            exponent_list& list = i->second;
            if(list.first.IsImmed())
            {
                double exponent = list.first.GetImmed();
                if(!(exponent == 0.0))
                    by_float_exponent.MoveToSet_Unique(exponent, list.second);
                by_exponent.erase(i);
            }
        }
        if(by_float_exponent.Optimize())
            should_regenerate = true;
    #endif

        if(should_regenerate)
        {
            CodeTree before = *this;
            before.CopyOnWrite();

          #ifdef DEBUG_SUBSTITUTIONS
            std::cout << "Before ConstantFolding_MulGrouping: "; FPoptimizer_Grammar::DumpTree(before);
            std::cout << "\n";
          #endif
            DelParams();

            /* Group by exponents */
            /* First handle non-constant exponents */
            for(exponent_map::iterator
                i = by_exponent.begin();
                i != by_exponent.end();
                ++i)
            {
                exponent_list& list = i->second;
        #ifndef FP_MUL_COMBINE_EXPONENTS
                if(list.first.IsImmed())
                {
                    double exponent = list.first.GetImmed();
                    if(exponent == 0.0) continue;
                    if(FloatEqual(exponent, 1.0))
                    {
                        AddParamsMove(list.second);
                        continue;
                    }
                }
        #endif
                CodeTree mul;
                mul.SetOpcode(cMul);
                mul.SetParamsMove( list.second);
                mul.Rehash();

                if(has_highlevel_opcodes && list.first.IsImmed())
                {
                    if(list.first.GetImmed() == 0.5)
                    {
                        CodeTree sqrt;
                        sqrt.SetOpcode(cSqrt);
                        sqrt.AddParamMove(mul);
                        sqrt.Rehash();
                        AddParamMove(sqrt);
                        continue;
                    }
                    if(list.first.GetImmed() == -0.5)
                    {
                        CodeTree rsqrt;
                        rsqrt.SetOpcode(cRSqrt);
                        rsqrt.AddParamMove(mul);
                        rsqrt.Rehash();
                        AddParamMove(rsqrt);
                        continue;
                    }
                    if(list.first.GetImmed() == -1.0)
                    {
                        CodeTree inv;
                        inv.SetOpcode(cInv);
                        inv.AddParamMove(mul);
                        inv.Rehash();
                        AddParamMove(inv);
                        continue;
                    }
                }
                CodeTree pow;
                pow.SetOpcode(cPow);
                pow.AddParamMove(mul);
                pow.AddParamMove( list.first );
                pow.Rehash();
                AddParamMove(pow);
            }
        #ifdef FP_MUL_COMBINE_EXPONENTS
            by_exponent.clear();
            /* Then handle constant exponents */
            for(size_t a=0; a<by_float_exponent.data.size(); ++a)
            {
                double exponent = by_float_exponent.data[a].first;
                if(FloatEqual(exponent, 1.0))
                {
                    AddParamsMove(by_float_exponent.data[a].second);
                    continue;
                }
                CodeTree mul;
                mul.SetOpcode(cMul);
                mul.SetParamsMove( by_float_exponent.data[a].second );
                mul.Rehash();
                CodeTree pow;
                pow.SetOpcode(cPow);
                pow.AddParamMove(mul);
                pow.AddParam( CodeTree( exponent ) );
                pow.Rehash();
                AddParamMove(pow);
            }
        #endif
          #ifdef DEBUG_SUBSTITUTIONS
            std::cout << "After ConstantFolding_MulGrouping: "; FPoptimizer_Grammar::DumpTree(*this);
            std::cout << "\n";
          #endif
            // return true;
            return !IsIdenticalTo(before); // avoids infinite looping
        }
        return false;
    }

    bool CodeTree::ConstantFolding_AddGrouping()
    {
        bool should_regenerate = false;
        CollectionSet add;
        for(size_t a=0; a<GetParamCount(); ++a)
        {
            if(GetParam(a).GetOpcode() == cMul) continue;
            if(add.AddCollection(GetParam(a)) == CollectionSet::Suboptimal)
                should_regenerate = true;
            // This catches x + x and x - x
        }
        std::vector<bool> remaining ( GetParamCount() );
        size_t has_mulgroups_remaining = 0;
        for(size_t a=0; a<GetParamCount(); ++a)
        {
            const CodeTree& mulgroup = GetParam(a);
            if(mulgroup.GetOpcode() == cMul)
            {
                // This catches x + y*x*z, producing x*(1 + y*z)
                //
                // However we avoid changing 7 + 7*x into 7*(x+1),
                // because it may lead us into producing code such
                // as 20*x + 50*(x+1) + 10, which would be much
                // better expressed as 70*x + 60, and converting
                // back to that format would be needlessly hairy.
                for(size_t b=0; b<mulgroup.GetParamCount(); ++b)
                {
                    if(mulgroup.GetParam(b).IsImmed()) continue;
                    CollectionSet::PositionType c
                        = add.FindIdenticalValueTo(mulgroup.GetParam(b));
                    if(add.Found(c))
                    {
                        CodeTree tmp(mulgroup, CodeTree::CloneTag());
                        tmp.DelParam(b);
                        tmp.Rehash();
                        add.AddCollectionTo(tmp, c);
                        should_regenerate = true;
                        goto done_a;
                    }
                }
                remaining[a]  = true;
                has_mulgroups_remaining += 1;
            done_a:;
            }
        }

        if(has_mulgroups_remaining > 0)
        {
            if(has_mulgroups_remaining > 1) // is it possible to find a duplicate?
            {
                std::vector< std::pair<CodeTree, size_t> > occurance_counts;
                std::multimap<fphash_t, size_t> occurance_pos;
                bool found_dup = false;
                for(size_t a=0; a<GetParamCount(); ++a)
                    if(remaining[a])
                    {
                        // This catches x*a + x*b, producing x*(a+b)
                        for(size_t b=0; b<GetParam(a).GetParamCount(); ++b)
                        {
                            const CodeTree& p = GetParam(a).GetParam(b);
                            const fphash_t   p_hash = p.GetHash();
                            for(std::multimap<fphash_t, size_t>::const_iterator
                                i = occurance_pos.lower_bound(p_hash);
                                i != occurance_pos.end() && i->first == p_hash;
                                ++i)
                            {
                                if(occurance_counts[i->second].first.IsIdenticalTo(p))
                                {
                                    occurance_counts[i->second].second += 1;
                                    found_dup = true;
                                    goto found_mulgroup_item_dup;
                                }
                            }
                            occurance_counts.push_back(std::make_pair(p, size_t(1)));
                            occurance_pos.insert(std::make_pair(p_hash, occurance_counts.size()-1));
                        found_mulgroup_item_dup:;
                        }
                    }
                if(found_dup)
                {
                    // Find the "x" to group by
                    CodeTree group_by; { size_t max = 0;
                    for(size_t p=0; p<occurance_counts.size(); ++p)
                        if(occurance_counts[p].second <= 1)
                            occurance_counts[p].second = 0;
                        else
                        {
                            occurance_counts[p].second *= occurance_counts[p].first.GetDepth();
                            if(occurance_counts[p].second > max)
                                { group_by = occurance_counts[p].first; max = occurance_counts[p].second; }
                        } }
                    // Collect the items for adding in the group (a+b)
                    CodeTree group_add;
                    group_add.SetOpcode(cAdd);

        #ifdef DEBUG_SUBSTITUTIONS
                    std::cout << "Duplicate across some trees: ";
                    FPoptimizer_Grammar::DumpTree(group_by);
                    std::cout << " in ";
                    FPoptimizer_Grammar::DumpTree(*this);
                    std::cout << "\n";
        #endif
                    for(size_t a=0; a<GetParamCount(); ++a)
                        if(remaining[a])
                            for(size_t b=0; b<GetParam(a).GetParamCount(); ++b)
                                if(group_by.IsIdenticalTo(GetParam(a).GetParam(b)))
                                {
                                    CodeTree tmp(GetParam(a), CodeTree::CloneTag());
                                    tmp.DelParam(b);
                                    tmp.Rehash();
                                    group_add.AddParamMove(tmp);
                                    remaining[a] = false;
                                    break;
                                }
                    group_add.Rehash();
                    CodeTree group;
                    group.SetOpcode(cMul);
                    group.AddParamMove(group_by);
                    group.AddParamMove(group_add);
                    group.Rehash();
                    add.AddCollection(group);
                    should_regenerate = true;
                }
            }

            // all remaining mul-groups.
            for(size_t a=0; a<GetParamCount(); ++a)
                if(remaining[a])
                {
                    if(add.AddCollection(GetParam(a)) == CollectionSet::Suboptimal)
                        should_regenerate = true;
                }
        }

        if(should_regenerate)
        {
          #ifdef DEBUG_SUBSTITUTIONS
            std::cout << "Before ConstantFolding_AddGrouping: "; FPoptimizer_Grammar::DumpTree(*this);
            std::cout << "\n";
          #endif
            DelParams();

            for(CollectionSet::PositionType
                j = add.collections.begin();
                j != add.collections.end();
                ++j)
            {
                CodeTree& value = j->second.value;
                CodeTree& coeff = j->second.factor;
                if(j->second.factor_needs_rehashing) coeff.Rehash();

                if(coeff.IsImmed())
                {
                    if(coeff.GetImmed() == 0.0)
                        continue;
                    if(FloatEqual(coeff.GetImmed(), 1.0))
                    {
                        AddParamMove(value);
                        continue;
                    }
                }
                CodeTree mul;
                mul.SetOpcode(cMul);
                mul.AddParamMove(value);
                mul.AddParamMove(coeff);
                mul.Rehash();
                AddParamMove(mul);
            }
          #ifdef DEBUG_SUBSTITUTIONS
            std::cout << "After ConstantFolding_AddGrouping: "; FPoptimizer_Grammar::DumpTree(*this);
            std::cout << "\n";
          #endif
            return true;
        }
        return false;
    }

    bool CodeTree::ConstantFolding_Assimilate()
    {
        /* If the list contains another list of the same kind, assimilate it */
        bool assimilated = false;
        for(size_t a=GetParamCount(); a-- > 0; )
            if(GetParam(a).GetOpcode() == GetOpcode())
            {
              #ifdef DEBUG_SUBSTITUTIONS
                if(!assimilated)
                {
                    std::cout << "Before assimilation: "; FPoptimizer_Grammar::DumpTree(*this);
                    std::cout << "\n";
                    assimilated = true;
                }
              #endif
                // Assimilate its children and remove it
                AddParamsMove(GetParam(a).GetUniqueRef().GetParams(), a);
            }
      #ifdef DEBUG_SUBSTITUTIONS
        if(assimilated)
        {
            std::cout << "After assimilation:   "; FPoptimizer_Grammar::DumpTree(*this);
            std::cout << "\n";
        }
      #endif
        return assimilated;
    }

    void CodeTree::ConstantFolding()
    {
    #ifdef DEBUG_SUBSTITUTIONS
        std::cout << "Runs ConstantFolding for: "; FPoptimizer_Grammar::DumpTree(*this);
        std::cout << "\n";
    #endif
        using namespace std;
    redo:;

        // Insert here any hardcoded constant-folding optimizations
        // that you want to be done whenever a new subtree is generated.
        /* Not recursive. */

        double const_value = 1.0;
        size_t which_param = 0;

        if(GetOpcode() != cImmed)
        {
            MinMaxTree p = CalculateResultBoundaries();
            if(p.has_min && p.has_max && p.min == p.max)
            {
                // Replace us with this immed
                const_value = p.min;
                goto ReplaceTreeWithConstValue;
            }
        }

        /* Constant folding */
        switch(GetOpcode())
        {
            case cImmed:
                break; // nothing to do
            case cVar:
                break; // nothing to do

            ReplaceTreeWithOne:
                const_value = 1.0;
                goto ReplaceTreeWithConstValue;
            ReplaceTreeWithZero:
                const_value = 0.0;
            ReplaceTreeWithConstValue:
              #ifdef DEBUG_SUBSTITUTIONS
                std::cout << "Replacing "; FPoptimizer_Grammar::DumpTree(*this);
                std::cout << " with const value " << const_value << "\n";
              #endif
                data = new CodeTreeData(const_value);
                break;
            ReplaceTreeWithParam0:
                which_param = 0;
            ReplaceTreeWithParam:
              #ifdef DEBUG_SUBSTITUTIONS
                std::cout << "Before replace: "; FPoptimizer_Grammar::DumpTree(*this);
                std::cout << "\n";
              #endif
                Become(GetParam(which_param));
              #ifdef DEBUG_SUBSTITUTIONS
                std::cout << "After replace: "; FPoptimizer_Grammar::DumpTree(*this);
                std::cout << "\n";
              #endif
                goto redo;

            case cAnd:
            {
                for(size_t a=0; a<GetParamCount(); ++a)
                    GetParam(a).ConstantFolding_FromLogicalParent();
                ConstantFolding_Assimilate();
                // If the and-list contains an expression that evaluates to approx. zero,
                // the whole list evaluates to zero.
                // If all expressions within the and-list evaluate to approx. nonzero,
                // the whole list evaluates to one.
                bool all_values_are_nonzero = true;
                for(size_t a=0; a<GetParamCount(); ++a)
                {
                    MinMaxTree p = GetParam(a).CalculateResultBoundaries();
                    if(p.has_min && p.has_max
                    && p.min > -0.5 && p.max < 0.5) // -0.5 < x < 0.5 = zero
                    {
                        goto ReplaceTreeWithZero;
                    }
                    else if( (p.has_max && p.max <= -0.5)
                          || (p.has_min && p.min >= 0.5)) // |x| >= 0.5  = nonzero
                    {
                    }
                    else
                        all_values_are_nonzero = false;
                }
                if(all_values_are_nonzero) goto ReplaceTreeWithOne;
                switch(GetParamCount())
                {
                    case 0: goto ReplaceTreeWithZero;
                    case 1: SetOpcode(cNotNot); goto redo; // Replace self with the single operand
                    default: if(ConstantFolding_AndLogic()) goto redo;
                }
                break;
            }
            case cOr:
            {
                for(size_t a=0; a<GetParamCount(); ++a)
                    GetParam(a).ConstantFolding_FromLogicalParent();
                ConstantFolding_Assimilate();
                // If the or-list contains an expression that evaluates to approx. nonzero,
                // the whole list evaluates to one.
                // If all expressions within the and-list evaluate to approx. zero,
                // the whole list evaluates to zero.
                bool all_values_are_zero = true;
                for(size_t a=0; a<GetParamCount(); ++a)
                {
                    MinMaxTree p = GetParam(a).CalculateResultBoundaries();
                    if(p.has_min && p.has_max
                    && p.min > -0.5 && p.max < 0.5) // -0.5 < x < 0.5 = zero
                    {
                    }
                    else if( (p.has_max && p.max <= -0.5)
                          || (p.has_min && p.min >= 0.5)) // |x| >= 0.5  = nonzero
                    {
                        goto ReplaceTreeWithOne;
                    }
                    else
                        all_values_are_zero = false;
                }
                if(all_values_are_zero) goto ReplaceTreeWithZero;
                switch(GetParamCount())
                {
                    case 0: goto ReplaceTreeWithOne;
                    case 1: SetOpcode(cNotNot); goto redo; // Replace self with the single operand
                    default: if(ConstantFolding_OrLogic()) goto redo;
                }
                break;
            }
            case cNot:
            {
                GetParam(0).ConstantFolding_FromLogicalParent();
                switch(GetParam(0).GetOpcode())
                {
                    case cEqual:       SetOpcode(cNEqual); goto cNot_moveparam;
                    case cNEqual:      SetOpcode(cEqual); goto cNot_moveparam;
                    case cLess:        SetOpcode(cGreaterOrEq); goto cNot_moveparam;
                    case cGreater:     SetOpcode(cLessOrEq); goto cNot_moveparam;
                    case cLessOrEq:    SetOpcode(cGreater); goto cNot_moveparam;
                    case cGreaterOrEq: SetOpcode(cLess); goto cNot_moveparam;
                    //cNotNot already handled by ConstantFolding_FromLogicalParent()
                    case cNot:         SetOpcode(cNotNot); goto cNot_moveparam;
                         { cNot_moveparam:;
                               SetParamsMove(GetParam(0).GetUniqueRef().GetParams()); goto redo; }
                    default: break;
                }

                // If the sub-expression evaluates to approx. zero, yield one.
                // If the sub-expression evaluates to approx. nonzero, yield zero.
                MinMaxTree p = GetParam(0).CalculateResultBoundaries();
                if(p.has_min && p.has_max
                && p.min > -0.5 && p.max < 0.5) // -0.5 < x < 0.5 = zero
                {
                    goto ReplaceTreeWithOne;
                }
                else if( (p.has_max && p.max <= -0.5)
                      || (p.has_min && p.min >= 0.5)) // |x| >= 0.5  = nonzero
                    goto ReplaceTreeWithZero;
                break;
            }
            case cNotNot:
            {
                // The function of cNotNot is to protect a logical value from
                // changing. If the parameter is already a logical value,
                // then the cNotNot opcode is redundant.
                if(GetParam(0).IsLogicalValue())
                    goto ReplaceTreeWithParam0;

                // If the sub-expression evaluates to approx. zero, yield zero.
                // If the sub-expression evaluates to approx. nonzero, yield one.
                MinMaxTree p = GetParam(0).CalculateResultBoundaries();
                if(p.has_min && p.has_max
                && p.min > -0.5 && p.max < 0.5) // -0.5 < x < 0.5 = zero
                {
                    goto ReplaceTreeWithZero;
                }
                else if( (p.has_max && p.max <= -0.5)
                      || (p.has_min && p.min >= 0.5)) // |x| >= 0.5  = nonzero
                    goto ReplaceTreeWithOne;
                break;
            }
            case cIf:
            {
                GetParam(0).ConstantFolding_FromLogicalParent();
                // If the If() condition begins with a cNot,
                // remove the cNot and swap the branches.
                while(GetParam(0).GetOpcode() == cNot)
                {
                    GetParam(0).Become( GetParam(0).GetParam(0) );
                    GetParam(1).swap(GetParam(2));
                }

                // If the sub-expression evaluates to approx. zero, yield param3.
                // If the sub-expression evaluates to approx. nonzero, yield param2.
                MinMaxTree p = GetParam(0).CalculateResultBoundaries();
                if(p.has_min && p.has_max
                && p.min > -0.5 && p.max < 0.5) // -0.5 < x < 0.5 = zero
                {
                    which_param = 2;
                    goto ReplaceTreeWithParam;
                }
                else if( (p.has_max && p.max <= -0.5)
                      || (p.has_min && p.min >= 0.5)) // |x| >= 0.5  = nonzero
                {
                    which_param = 1;
                    goto ReplaceTreeWithParam;
                }
                break;
            }
            case cMul:
            {
            NowWeAreMulGroup: ;
                ConstantFolding_Assimilate();
                // If one sub-expression evalutes to exact zero, yield zero.
                double immed_product = 1.0;
                size_t n_immeds = 0; bool needs_resynth=false;
                for(size_t a=0; a<GetParamCount(); ++a)
                {
                    if(!GetParam(a).IsImmed()) continue;
                    // ^ Only check constant values
                    double immed = GetParam(a).GetImmed();
                    if(immed == 0.0) goto ReplaceTreeWithZero;
                    immed_product *= immed; ++n_immeds;
                }
                // Merge immeds.
                if(n_immeds > 1 || (n_immeds == 1 && FloatEqual(immed_product, 1.0)))
                    needs_resynth = true;
                if(needs_resynth)
                {
                    // delete immeds and add new ones
                #ifdef DEBUG_SUBSTITUTIONS
                    std::cout << "cMul: Will add new immed " << immed_product << "\n";
                #endif
                    for(size_t a=GetParamCount(); a-->0; )
                        if(GetParam(a).IsImmed())
                        {
                        #ifdef DEBUG_SUBSTITUTIONS
                            std::cout << " - For that, deleting immed " << GetParam(a).GetImmed();
                            std::cout << "\n";
                        #endif
                            DelParam(a);
                        }
                    if(!FloatEqual(immed_product, 1.0))
                        AddParam( CodeTree(immed_product) );
                }
                switch(GetParamCount())
                {
                    case 0: goto ReplaceTreeWithOne;
                    case 1: goto ReplaceTreeWithParam0; // Replace self with the single operand
                    default: if(ConstantFolding_MulGrouping()) goto redo;
                }
                break;
            }
            case cAdd:
            {
                ConstantFolding_Assimilate();
                double immed_sum = 0.0;
                size_t n_immeds = 0; bool needs_resynth=false;
                for(size_t a=0; a<GetParamCount(); ++a)
                {
                    if(!GetParam(a).IsImmed()) continue;
                    // ^ Only check constant values
                    double immed = GetParam(a).GetImmed();
                    immed_sum += immed; ++n_immeds;
                }
                // Merge immeds.
                if(n_immeds > 1 || (n_immeds == 1 && immed_sum == 0.0))
                    needs_resynth = true;
                if(needs_resynth)
                {
                    // delete immeds and add new ones
                #ifdef DEBUG_SUBSTITUTIONS
                    std::cout << "cAdd: Will add new immed " << immed_sum << "\n";
                    std::cout << "In: "; FPoptimizer_Grammar::DumpTree(*this);
                    std::cout << "\n";
                #endif
                    for(size_t a=GetParamCount(); a-->0; )
                        if(GetParam(a).IsImmed())
                        {
                        #ifdef DEBUG_SUBSTITUTIONS
                            std::cout << " - For that, deleting immed " << GetParam(a).GetImmed();
                            std::cout << "\n";
                        #endif
                            DelParam(a);
                        }
                    if(!(immed_sum == 0.0))
                        AddParam( CodeTree(immed_sum) );
                }
                switch(GetParamCount())
                {
                    case 0: goto ReplaceTreeWithZero;
                    case 1: goto ReplaceTreeWithParam0; // Replace self with the single operand
                    default: if(ConstantFolding_AddGrouping()) goto redo;
                }
                break;
            }
            case cMin:
            {
                ConstantFolding_Assimilate();
                /* Goal: If there is any pair of two operands, where
                 * their ranges form a disconnected set, i.e. as below:
                 *     xxxxx
                 *            yyyyyy
                 * Then remove the larger one.
                 *
                 * Algorithm: 1. figure out the smallest maximum of all operands.
                 *            2. eliminate all operands where their minimum is
                 *               larger than the selected maximum.
                 */
                MinMaxTree smallest_maximum;
                for(size_t a=0; a<GetParamCount(); ++a)
                {
                    MinMaxTree p = GetParam(a).CalculateResultBoundaries();
                    if(p.has_max && (!smallest_maximum.has_max || p.max < smallest_maximum.max))
                    {
                        smallest_maximum.max = p.max;
                        smallest_maximum.has_max = true;
                }   }
                if(smallest_maximum.has_max)
                    for(size_t a=GetParamCount(); a-- > 0; )
                    {
                        MinMaxTree p = GetParam(a).CalculateResultBoundaries();
                        if(p.has_min && p.min > smallest_maximum.max)
                            DelParam(a);
                    }
                //fprintf(stderr, "Remains: %u\n", (unsigned)GetParamCount());
                if(GetParamCount() == 1)
                {
                    // Replace self with the single operand
                    goto ReplaceTreeWithParam0;
                }
                break;
            }
            case cMax:
            {
                ConstantFolding_Assimilate();
                /* Goal: If there is any pair of two operands, where
                 * their ranges form a disconnected set, i.e. as below:
                 *     xxxxx
                 *            yyyyyy
                 * Then remove the smaller one.
                 *
                 * Algorithm: 1. figure out the biggest minimum of all operands.
                 *            2. eliminate all operands where their maximum is
                 *               smaller than the selected minimum.
                 */
                MinMaxTree biggest_minimum;
                for(size_t a=0; a<GetParamCount(); ++a)
                {
                    MinMaxTree p = GetParam(a).CalculateResultBoundaries();
                    if(p.has_min && (!biggest_minimum.has_min || p.min > biggest_minimum.min))
                    {
                        biggest_minimum.min = p.min;
                        biggest_minimum.has_min = true;
                }   }
                if(biggest_minimum.has_min)
                {
                    //fprintf(stderr, "Removing all where max < %g\n", biggest_minimum.min);
                    for(size_t a=GetParamCount(); a-- > 0; )
                    {
                        MinMaxTree p = GetParam(a).CalculateResultBoundaries();
                        if(p.has_max && p.max < biggest_minimum.min)
                        {
                            //fprintf(stderr, "Removing %g\n", p.max);
                            DelParam(a);
                        }
                    }
                }
                //fprintf(stderr, "Remains: %u\n", (unsigned)GetParamCount());
                if(GetParamCount() == 1)
                {
                    // Replace self with the single operand
                    goto ReplaceTreeWithParam0;
                }
                break;
            }

            case cEqual:
            {
                if(GetParam(0).IsIdenticalTo(GetParam(1))) goto ReplaceTreeWithOne;
                /* If we know the two operands' ranges don't overlap, we get zero.
                 * The opposite is more complex and is done in .dat code.
                 */
                MinMaxTree p0 = GetParam(0).CalculateResultBoundaries();
                MinMaxTree p1 = GetParam(1).CalculateResultBoundaries();
                if((p0.has_max && p1.has_min && p1.min > p0.max)
                || (p1.has_max && p0.has_min && p0.min > p1.max))
                    goto ReplaceTreeWithZero;
                break;
            }

            case cNEqual:
            {
                if(GetParam(0).IsIdenticalTo(GetParam(1))) goto ReplaceTreeWithZero;
                /* If we know the two operands' ranges don't overlap, we get one.
                 * The opposite is more complex and is done in .dat code.
                 */
                MinMaxTree p0 = GetParam(0).CalculateResultBoundaries();
                MinMaxTree p1 = GetParam(1).CalculateResultBoundaries();
                if((p0.has_max && p1.has_min && p1.min > p0.max)
                || (p1.has_max && p0.has_min && p0.min > p1.max))
                    goto ReplaceTreeWithOne;
                break;
            }

            case cLess:
            {
                if(GetParam(0).IsIdenticalTo(GetParam(1))) goto ReplaceTreeWithZero;
                MinMaxTree p0 = GetParam(0).CalculateResultBoundaries();
                MinMaxTree p1 = GetParam(1).CalculateResultBoundaries();
                if(p0.has_max && p1.has_min && p0.max < p1.min)
                    goto ReplaceTreeWithOne; // We know p0 < p1
                if(p1.has_max && p0.has_min && p1.max <= p0.min)
                    goto ReplaceTreeWithZero; // We know p1 >= p0
                break;
            }

            case cLessOrEq:
            {
                if(GetParam(0).IsIdenticalTo(GetParam(1))) goto ReplaceTreeWithOne;
                MinMaxTree p0 = GetParam(0).CalculateResultBoundaries();
                MinMaxTree p1 = GetParam(1).CalculateResultBoundaries();
                if(p0.has_max && p1.has_min && p0.max <= p1.min)
                    goto ReplaceTreeWithOne; // We know p0 <= p1
                if(p1.has_max && p0.has_min && p1.max < p0.min)
                    goto ReplaceTreeWithZero; // We know p1 > p0
                break;
            }

            case cGreater:
            {
                if(GetParam(0).IsIdenticalTo(GetParam(1))) goto ReplaceTreeWithZero;
                // Note: Eq case not handled
                MinMaxTree p0 = GetParam(0).CalculateResultBoundaries();
                MinMaxTree p1 = GetParam(1).CalculateResultBoundaries();
                if(p0.has_max && p1.has_min && p0.max <= p1.min)
                    goto ReplaceTreeWithZero; // We know p0 <= p1
                if(p1.has_max && p0.has_min && p1.max < p0.min)
                    goto ReplaceTreeWithOne; // We know p1 > p0
                break;
            }

            case cGreaterOrEq:
            {
                if(GetParam(0).IsIdenticalTo(GetParam(1))) goto ReplaceTreeWithOne;
                // Note: Eq case not handled
                MinMaxTree p0 = GetParam(0).CalculateResultBoundaries();
                MinMaxTree p1 = GetParam(1).CalculateResultBoundaries();
                if(p0.has_max && p1.has_min && p0.max < p1.min)
                    goto ReplaceTreeWithZero; // We know p0 < p1
                if(p1.has_max && p0.has_min && p1.max <= p0.min)
                    goto ReplaceTreeWithOne; // We know p1 >= p0
                break;
            }

            case cAbs:
            {
                /* If we know the operand is always positive, cAbs is redundant.
                 * If we know the operand is always negative, use actual negation.
                 */
                MinMaxTree p0 = GetParam(0).CalculateResultBoundaries();
                if(p0.has_min && p0.min >= 0.0)
                    goto ReplaceTreeWithParam0;
                if(p0.has_max && p0.max <= NEGATIVE_MAXIMUM)
                {
                    /* abs(negative) = negative*-1 */
                    SetOpcode(cMul);
                    AddParam( CodeTree(-1.0) );
                    /* The caller of ConstantFolding() will do Sort() and Rehash() next.
                     * Thus, no need to do it here. */
                    /* We were changed into a cMul group. Do cMul folding. */
                    goto NowWeAreMulGroup;
                }
                /* If the operand is a cMul group, find elements
                 * that are always positive and always negative,
                 * and move them out, e.g. abs(p*n*x*y) = p*(-n)*abs(x*y)
                 */
                if(GetParam(0).GetOpcode() == cMul)
                {
                    const CodeTree& p = GetParam(0);
                    std::vector<CodeTree> pos_set;
                    std::vector<CodeTree> neg_set;
                    for(size_t a=0; a<p.GetParamCount(); ++a)
                    {
                        p0 = p.GetParam(a).CalculateResultBoundaries();
                        if(p0.has_min && p0.min >= 0.0)
                            { pos_set.push_back(p.GetParam(a)); }
                        if(p0.has_max && p0.max <= NEGATIVE_MAXIMUM)
                            { neg_set.push_back(p.GetParam(a)); }
                    }
                #ifdef DEBUG_SUBSTITUTIONS
                    std::cout << "Abs: mul group has " << pos_set.size()
                              << " pos, " << neg_set.size() << "neg\n";
                #endif
                    if(!pos_set.empty() || !neg_set.empty())
                    {
                #ifdef DEBUG_SUBSTITUTIONS
                        std::cout << "AbsReplace-Before: ";
                        FPoptimizer_Grammar::DumpTree(*this);
                        std::cout << "\n" << std::flush;
                        FPoptimizer_Grammar::DumpHashes(*this, std::cout);
                #endif
                        CodeTree pclone;
                        pclone.SetOpcode(cMul);
                        for(size_t a=0; a<p.GetParamCount(); ++a)
                        {
                            p0 = p.GetParam(a).CalculateResultBoundaries();
                            if((p0.has_min && p0.min >= 0.0)
                            || (p0.has_max && p0.max <= NEGATIVE_MAXIMUM))
                                {/*pclone.DelParam(a);*/}
                            else
                                pclone.AddParam( p.GetParam(a) );
                            /* Here, p*n*x*y -> x*y.
                             * p is saved in pos_set[]
                             * n is saved in neg_set[]
                             */
                        }
                        pclone.Rehash();
                        CodeTree abs_mul;
                        abs_mul.SetOpcode(cAbs);
                        abs_mul.AddParamMove(pclone);
                        abs_mul.Rehash();
                        CodeTree mulgroup;
                        mulgroup.SetOpcode(cMul);
                        mulgroup.AddParamMove(abs_mul); // cAbs[whatever remains in p]
                        mulgroup.AddParamsMove(pos_set);
                        /* Now:
                         * mulgroup  = p * Abs(x*y)
                         */
                        if(!neg_set.empty())
                        {
                            if(neg_set.size() % 2)
                                mulgroup.AddParam( CodeTree(-1.0) );
                            mulgroup.AddParamsMove(neg_set);
                            /* Now:
                             * mulgroup = p * n * -1 * Abs(x*y)
                             */
                        }
                        Become(mulgroup);
                #ifdef DEBUG_SUBSTITUTIONS
                        std::cout << "AbsReplace-After: ";
                        FPoptimizer_Grammar::DumpTree(*this, std::cout);
                        std::cout << "\n" << std::flush;
                        FPoptimizer_Grammar::DumpHashes(*this, std::cout);
                #endif
                        /* We were changed into a cMul group. Do cMul folding. */
                        goto NowWeAreMulGroup;
                    }
                }
                break;
            }

            #define HANDLE_UNARY_CONST_FUNC(funcname) \
                if(GetParam(0).IsImmed()) \
                    { const_value = funcname(GetParam(0).GetImmed()); \
                      goto ReplaceTreeWithConstValue; }

            case cLog:   HANDLE_UNARY_CONST_FUNC(log); break;
            case cAcosh: HANDLE_UNARY_CONST_FUNC(fp_acosh); break;
            case cAsinh: HANDLE_UNARY_CONST_FUNC(fp_asinh); break;
            case cAtanh: HANDLE_UNARY_CONST_FUNC(fp_atanh); break;
            case cAcos: HANDLE_UNARY_CONST_FUNC(acos); break;
            case cAsin: HANDLE_UNARY_CONST_FUNC(asin); break;
            case cAtan: HANDLE_UNARY_CONST_FUNC(atan); break;
            case cCosh: HANDLE_UNARY_CONST_FUNC(cosh); break;
            case cSinh: HANDLE_UNARY_CONST_FUNC(sinh); break;
            case cTanh: HANDLE_UNARY_CONST_FUNC(tanh); break;
            case cSin: HANDLE_UNARY_CONST_FUNC(sin); break;
            case cCos: HANDLE_UNARY_CONST_FUNC(cos); break;
            case cTan: HANDLE_UNARY_CONST_FUNC(tan); break;
            case cCeil: HANDLE_UNARY_CONST_FUNC(ceil); break;
            case cFloor: HANDLE_UNARY_CONST_FUNC(floor); break;
            case cSqrt: HANDLE_UNARY_CONST_FUNC(sqrt); break; // converted into cPow x 0.5
            case cExp: HANDLE_UNARY_CONST_FUNC(exp); break; // convered into cPow CONSTANT_E x
            case cInt:
                if(GetParam(0).IsImmed())
                    { const_value = floor(GetParam(0).GetImmed() + 0.5);
                      goto ReplaceTreeWithConstValue; }
                break;
            case cLog2:
                if(GetParam(0).IsImmed())
                    { const_value = log(GetParam(0).GetImmed()) * CONSTANT_L2I;
                      goto ReplaceTreeWithConstValue; }
                break;
            case cLog10:
                if(GetParam(0).IsImmed())
                    { const_value = log(GetParam(0).GetImmed()) * CONSTANT_L10I;
                      goto ReplaceTreeWithConstValue; }
                break;

            case cAtan2:
            {
                /* Range based optimizations for (y,x):
                 * If y is +0 and x <= -0, +pi is returned
                 * If y is -0 and x <= -0, -pi is returned (assumed never happening)
                 * If y is +0 and x >= +0, +0 is returned
                 * If y is -0 and x >= +0, -0 is returned  (assumed never happening)
                 * If x is +-0 and y < 0, -pi/2 is returned
                 * If x is +-0 and y > 0, +pi/2 is returned
                 * Otherwise, perform constant folding when available
                 * If we know x <> 0, convert into atan(y / x)
                 *   TODO: Figure out whether the above step is wise
                 *         It allows e.g. atan2(6*x, 3*y) -> atan(2*x/y)
                 *         when we know y != 0
                 */
                MinMaxTree p0 = GetParam(0).CalculateResultBoundaries();
                MinMaxTree p1 = GetParam(1).CalculateResultBoundaries();
                if(p0.has_min && p0.has_max && p0.min == 0.0)
                {
                    if(p1.has_max && p1.max < 0)
                        { const_value = CONSTANT_PI; goto ReplaceTreeWithConstValue; }
                    if(p1.has_max && p1.max >= 0.0)
                        { const_value = p0.min; goto ReplaceTreeWithConstValue; }
                }
                if(p1.has_min && p1.has_max && p1.min == 0.0)
                {
                    if(p0.has_max && p0.max < 0)
                        { const_value = -CONSTANT_PIHALF; goto ReplaceTreeWithConstValue; }
                    if(p0.has_min && p0.min > 0)
                        { const_value =  CONSTANT_PIHALF; goto ReplaceTreeWithConstValue; }
                }
                if(GetParam(0).IsImmed()
                && GetParam(1).IsImmed())
                    { const_value = atan2(GetParam(0).GetImmed(),
                                          GetParam(1).GetImmed());
                      goto ReplaceTreeWithConstValue; }
                if((p1.has_min && p1.min > 0.0)
                || (p1.has_max && p1.max < NEGATIVE_MAXIMUM)) // become atan(p0 / p1)
                {
                    CodeTree pow_tree;
                    pow_tree.SetOpcode(cPow);
                    pow_tree.AddParam(GetParam(1));
                    pow_tree.AddParam(CodeTree(-1.0));
                    pow_tree.Rehash();
                    CodeTree div_tree;
                    div_tree.SetOpcode(cMul);
                    div_tree.AddParam(GetParam(0));
                    div_tree.AddParamMove(pow_tree);
                    div_tree.Rehash();
                    SetOpcode(cAtan);
                    SetParamMove(0, div_tree);
                    DelParam(1);
                }
                break;
            }

            case cPow:
            {
                if(GetParam(0).IsImmed()
                && GetParam(1).IsImmed())
                    { const_value = pow(GetParam(0).GetImmed(),
                                        GetParam(1).GetImmed());
                      goto ReplaceTreeWithConstValue; }
                if(GetParam(1).IsImmed()
                && GetParam(1).GetImmed() == 1.0)
                {
                    // x^1 = x
                    goto ReplaceTreeWithParam0;
                }
                if(GetParam(0).IsImmed()
                && GetParam(0).GetImmed() == 1.0)
                {
                    // 1^x = 1
                    goto ReplaceTreeWithOne;
                }

                // 5^(20*x) = (5^20)^x
                if(GetParam(0).IsImmed()
                && GetParam(1).GetOpcode() == cMul)
                {
                    bool changes = false;
                    double base_immed = GetParam(0).GetImmed();
                    CodeTree& mulgroup = GetParam(1);
                    for(size_t a=mulgroup.GetParamCount(); a-->0; )
                        if(mulgroup.GetParam(a).IsImmed())
                        {
                            double imm = mulgroup.GetParam(a).GetImmed();
                            //if(imm >= 0.0)
                            {
                                double new_base_immed = std::pow(base_immed, imm);
#if __cplusplus < 201103L
								if(isinf(new_base_immed) || new_base_immed == 0.0)
#else
								if(std::isinf(new_base_immed) || new_base_immed == 0.0)
#endif
                                {
                                    // It produced an infinity. Do not change.
                                    break;
                                }

                                if(!changes)
                                {
                                    changes = true;
                                    mulgroup.CopyOnWrite();
                                }
                                base_immed = new_base_immed;
                                mulgroup.DelParam(a);
                                break; //
                            }
                        }
                    if(changes)
                    {
                        GetParam(0).Become(CodeTree(base_immed));
                        mulgroup.Rehash();
                    }
                }
                // (x^3)^2 = x^6
                // NOTE: If 3 is even and 3*2 is not, x must be changed to abs(x).
                if(GetParam(0).GetOpcode() == cPow
                && GetParam(1).IsImmed()
                && GetParam(0).GetParam(1).IsImmed())
                {
                    double a = GetParam(0).GetParam(1).GetImmed();
                    double b = GetParam(1).GetImmed();
                    double c = a * b; // new exponent
                    if(IsEvenIntegerConst(a) // a is an even int?
                    && !IsEvenIntegerConst(c)) // c is not?
                    {
                        CodeTree newbase;
                        newbase.SetOpcode(cAbs);
                        newbase.AddParam(GetParam(0).GetParam(0));
                        newbase.Rehash();
                        SetParamMove(0, newbase);
                    }
                    else
                        SetParam(0, GetParam(0).GetParam(0));
                    SetParam(1, CodeTree(c));
                }
                break;
            }

            case cMod:
            {
                /* Can more be done than this? */
                if(GetParam(0).IsImmed()
                && GetParam(1).IsImmed())
                    { const_value = fmod(GetParam(0).GetImmed(),
                                         GetParam(1).GetImmed());
                      goto ReplaceTreeWithConstValue; }
                break;
            }

            /* The following opcodes are processed by GenerateFrom()
             * within fpoptimizer_bytecode_to_codetree.cc and thus
             * they will never occur in the calling context:
             */
            case cDiv: // converted into cPow y -1
            case cRDiv: // similar to above
            case cSub: // converted into cMul y -1
            case cRSub: // similar to above
            case cRad: // converted into cMul x CONSTANT_RD
            case cDeg: // converted into cMul x CONSTANT_DR
            case cSqr: // converted into cMul x x
            case cExp2: // converted into cPow 2.0 x
            case cRSqrt: // converted into cPow x -0.5
            case cCot: // converted into cMul (cPow (cTan x) -1)
            case cSec: // converted into cMul (cPow (cCos x) -1)
            case cCsc: // converted into cMul (cPow (cSin x) -1)
            case cRPow: // converted into cPow y x
                break; /* Should never occur */

            /* The following opcodes are processed by GenerateFrom(),
             * but they may still be synthesized in the grammar matching
             * process:
             * TODO: Figure out whether we should just convert
             * these particular trees into their atomic counterparts
             */
            case cNeg: // converted into cMul x -1
            {
                if(GetParam(0).IsImmed())
                {
                    const_value = -GetParam(0).GetImmed();
                    goto ReplaceTreeWithConstValue;
                }
                break;
            }
            case cInv: // converted into cPow x -1
            {
                if(GetParam(0).IsImmed())
                {
                    const_value = 1.0 / GetParam(0).GetImmed();
                    goto ReplaceTreeWithConstValue;
                }
                break;
            }

            /* Opcodes that do not occur in the tree for other reasons */
            case cDup:
            case cFetch:
            case cPopNMov:
            case cNop:
            case cJump:
            case VarBegin:
                break; /* Should never occur */
            /* Opcodes that we can't do anything about */
            case cPCall:
            case cFCall:
            case cEval:
                break;
        }
    }

    MinMaxTree CodeTree::CalculateResultBoundaries() const
#ifdef DEBUG_SUBSTITUTIONS_extra_verbose
    {
        MinMaxTree tmp = CalculateResultBoundaries_do();
        std::cout << "Estimated boundaries: ";
        if(tmp.has_min) std::cout << tmp.min; else std::cout << "-inf";
        std::cout << " .. ";
        if(tmp.has_max) std::cout << tmp.max; else std::cout << "+inf";
        std::cout << ": ";
        FPoptimizer_Grammar::DumpTree(*this);
        std::cout << std::endl;
        return tmp;
    }
    MinMaxTree CodeTree::CalculateResultBoundaries_do() const
#endif
    {
        using namespace std;
        switch( GetOpcode() )
        {
            case cImmed:
                return MinMaxTree(GetImmed(), GetImmed()); // a definite value.
            case cAnd:
            case cOr:
            case cNot:
            case cNotNot:
            case cEqual:
            case cNEqual:
            case cLess:
            case cLessOrEq:
            case cGreater:
            case cGreaterOrEq:
            {
                /* These operations always produce truth values (0 or 1) */
                /* Narrowing them down is a matter of performing Constant optimization */
                return MinMaxTree( 0.0, 1.0 );
            }
            case cAbs:
            {
                /* cAbs always produces a positive value */
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                if(m.has_min && m.has_max)
                {
                    if(m.min < 0.0 && m.max >= 0.0) // ex. -10..+6 or -6..+10
                    {
                        /* -x..+y: spans across zero. min=0, max=greater of |x| and |y|. */
                        double tmp = -m.min; if(tmp > m.max) m.max = tmp;
                        m.min = 0.0; m.has_min = true;
                    }
                    else if(m.min < 0.0) // ex. -10..-4
                        { double tmp = m.max; m.max = -m.min; m.min = -tmp; }
                }
                else if(!m.has_min && m.has_max && m.max < 0.0) // ex. -inf..-10
                {
                    m.min = fabs(m.max); m.has_min = true; m.has_max = false;
                }
                else if(!m.has_max && m.has_min && m.min > 0.0) // ex. +10..+inf
                {
                    m.min = fabs(m.min); m.has_min = true; m.has_max = false;
                }
                else // ex. -inf..+inf, -inf..+10, -10..+inf
                {
                    // all of these cover -inf..0, 0..+inf, or both
                    m.min = 0.0; m.has_min = true; m.has_max = false;
                }
                return m;
            }

            case cLog: /* Defined for 0.0 < x <= inf */
            {
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                if(m.has_min) { if(m.min < 0.0) m.has_min = false; else m.min = log(m.min); } // No boundaries
                if(m.has_max) { if(m.max < 0.0) m.has_max = false; else m.max = log(m.max); }
                return m;
            }

            case cLog2: /* Defined for 0.0 < x <= inf */
            {
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                if(m.has_min) { if(m.min < 0.0) m.has_min = false; else m.min = log(m.min)*CONSTANT_L2I; } // No boundaries
                if(m.has_max) { if(m.max < 0.0) m.has_max = false; else m.max = log(m.max)*CONSTANT_L2I; }
                return m;
            }

            case cAcosh: /* defined for             1.0 <  x <= inf */
            {
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                if(m.has_min) { if(m.min <= 1.0) m.has_min = false; else m.min = fp_acosh(m.min); } // No boundaries
                if(m.has_max) { if(m.max <= 1.0) m.has_max = false; else m.max = fp_acosh(m.max); }
                return m;
            }
            case cAsinh: /* defined for all values -inf <= x <= inf */
            {
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                if(m.has_min) m.min = fp_asinh(m.min); // No boundaries
                if(m.has_max) m.max = fp_asinh(m.max);
                return m;
            }
            case cAtanh: /* defined for all values -inf <= x <= inf */
            {
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                if(m.has_min) m.min = fp_atanh(m.min); // No boundaries
                if(m.has_max) m.max = fp_atanh(m.max);
                return m;
            }
            case cAcos: /* defined for -1.0 <= x < 1, results within CONSTANT_PI..0 */
            {
                /* Somewhat complicated to narrow down from this */
                /* TODO: A resourceful programmer may add it later. */
                return MinMaxTree( 0.0, CONSTANT_PI );
            }
            case cAsin: /* defined for -1.0 <= x < 1, results within -CONSTANT_PIHALF..CONSTANT_PIHALF */
            {
                /* Somewhat complicated to narrow down from this */
                /* TODO: A resourceful programmer may add it later. */
                return MinMaxTree( -CONSTANT_PIHALF, CONSTANT_PIHALF );
            }
            case cAtan: /* defined for all values -inf <= x <= inf */
            {
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                if(m.has_min) m.min = atan(m.min); else { m.min = -CONSTANT_PIHALF; m.has_min = true; }
                if(m.has_max) m.max = atan(m.max); else { m.max =  CONSTANT_PIHALF; m.has_max = true; }
                return m;
            }
            case cAtan2: /* too complicated to estimate */
            {
                /* Somewhat complicated to narrow down from this */
                /* TODO: A resourceful programmer may add it later. */
                return MinMaxTree(-CONSTANT_PI, CONSTANT_PI);
            }

            case cSin:
            case cCos:
            {
                /* Could be narrowed down from here,
                 * but it's too complicated due to
                 * the cyclic nature of the function. */
                /* TODO: A resourceful programmer may add it later. */
                return MinMaxTree(-1.0, 1.0);
            }
            case cTan:
            {
                /* Could be narrowed down from here,
                 * but it's too complicated due to
                 * the cyclic nature of the function */
                /* TODO: A resourceful programmer may add it later. */
                return MinMaxTree(); // (CONSTANT_NEG_INF, CONSTANT_POS_INF);
            }

            case cCeil:
            {
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                m.max = std::ceil(m.max); // ceil() may increase the value, may not decrease
                return m;
            }
            case cFloor:
            {
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                m.min = std::floor(m.min); // floor() may decrease the value, may not increase
                return m;
            }
            case cInt:
            {
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                m.min = std::floor(m.min); // int() may either increase or decrease the value
                m.max = std::ceil(m.max); // for safety, we assume both
                return m;
            }
            case cSinh: /* defined for all values -inf <= x <= inf */
            {
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                if(m.has_min) m.min = sinh(m.min); // No boundaries
                if(m.has_max) m.max = sinh(m.max);
                return m;
            }
            case cTanh: /* defined for all values -inf <= x <= inf */
            {
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                if(m.has_min) m.min = tanh(m.min); // No boundaries
                if(m.has_max) m.max = tanh(m.max);
                return m;
            }
            case cCosh: /* defined for all values -inf <= x <= inf, results within 1..inf */
            {
                MinMaxTree m = GetParam(0).CalculateResultBoundaries();
                if(m.has_min)
                {
                    if(m.has_max) // max, min
                    {
                        if(m.min >= 0.0 && m.max >= 0.0) // +x .. +y
                            { m.min = cosh(m.min); m.max = cosh(m.max); }
                        else if(m.min < 0.0 && m.max >= 0.0) // -x .. +y
                            { double tmp = cosh(m.min); m.max = cosh(m.max);
                              if(tmp > m.max) m.max = tmp;
                              m.min = 1.0; }
                        else // -x .. -y
                            { m.min = cosh(m.min); m.max = cosh(m.max);
                              std::swap(m.min, m.max); }
                    }
                    else // min, no max
                    {
                        if(m.min >= 0.0) // 0..inf -> 1..inf
                            { m.has_max = true; m.max = cosh(m.min); m.min = 1.0; }
                        else
                            { m.has_max = false; m.min = 1.0; } // Anything between 1..inf
                    }
                }
                else // no min
                {
                    m.has_min = true; m.min = 1.0; // always a lower boundary
                    if(m.has_max) // max, no min
                    {
                        m.min = cosh(m.max); // n..inf
                        m.has_max = false; // No upper boundary
                    }
                    else // no max, no min
                        m.has_max = false; // No upper boundary
                }
                return m;
            }

            case cIf:
            {
                // No guess which branch is chosen. Produce a spanning min & max.
                MinMaxTree res1 = GetParam(1).CalculateResultBoundaries();
                MinMaxTree res2 = GetParam(2).CalculateResultBoundaries();
                if(!res2.has_min) res1.has_min = false; else if(res2.min < res1.min) res1.min = res2.min;
                if(!res2.has_max) res1.has_max = false; else if(res2.max > res1.max) res1.max = res2.max;
                return res1;
            }

            case cMin:
            {
                bool has_unknown_min = false;
                bool has_unknown_max = false;

                MinMaxTree result;
                for(size_t a=0; a<GetParamCount(); ++a)
                {
                    MinMaxTree m = GetParam(a).CalculateResultBoundaries();
                    if(!m.has_min)
                        has_unknown_min = true;
                    else if(!result.has_min || m.min < result.min)
                        result.min = m.min;

                    if(!m.has_max)
                        has_unknown_max = true;
                    else if(!result.has_max || m.max < result.max)
                        result.max = m.max;
                }
                if(has_unknown_min) result.has_min = false;
                if(has_unknown_max) result.has_max = false;
                return result;
            }
            case cMax:
            {
                bool has_unknown_min = false;
                bool has_unknown_max = false;

                MinMaxTree result;
                for(size_t a=0; a<GetParamCount(); ++a)
                {
                    MinMaxTree m = GetParam(a).CalculateResultBoundaries();
                    if(!m.has_min)
                        has_unknown_min = true;
                    else if(!result.has_min || m.min > result.min)
                        result.min = m.min;

                    if(!m.has_max)
                        has_unknown_max = true;
                    else if(!result.has_max || m.max > result.max)
                        result.max = m.max;
                }
                if(has_unknown_min) result.has_min = false;
                if(has_unknown_max) result.has_max = false;
                return result;
            }
            case cAdd:
            {
                /* It's complicated. Follow the logic below. */
                /* Note: This also deals with the following opcodes:
                 *       cNeg, cSub, cRSub
                 */
                MinMaxTree result(0.0, 0.0);
                for(size_t a=0; a<GetParamCount(); ++a)
                {
                    MinMaxTree item = GetParam(a).CalculateResultBoundaries();

                    if(item.has_min) result.min += item.min;
                    else             result.has_min = false;
                    if(item.has_max) result.max += item.max;
                    else             result.has_max = false;

                    if(!result.has_min && !result.has_max) break; // hopeless
                }
                if(result.has_min && result.has_max
                && result.min > result.max) std::swap(result.min, result.max);
                return result;
            }
            case cMul:
            {
                /* It's complicated. Follow the logic below. */
                /* Note: This also deals with the following opcodes:
                 *       cInv, cDiv, cRDiv, cRad, cDeg, cSqr
                 *       cCot, Sec, cCsc, cLog2, cLog10
                 */

                struct Value
                {
                    enum ValueType { Finite, MinusInf, PlusInf };
                    ValueType valueType;
                    double value;

                    Value(ValueType t): valueType(t), value(0) {}
                    Value(double v): valueType(Finite), value(v) {}

                    bool isNegative() const
                    {
                        return valueType == MinusInf ||
                            (valueType == Finite && value < 0.0);
                    }

                    void operator*=(const Value& rhs)
                    {
                        if(valueType == Finite && rhs.valueType == Finite)
                            value *= rhs.value;
                        else
                            valueType = (isNegative() != rhs.isNegative() ?
                                         MinusInf : PlusInf);
                    }

                    bool operator<(const Value& rhs) const
                    {
                        return
                            (valueType == MinusInf && rhs.valueType != MinusInf) ||
                            (valueType == Finite &&
                             (rhs.valueType == PlusInf ||
                              (rhs.valueType == Finite && value < rhs.value)));
                    }
                };

                struct MultiplicationRange
                {
                    Value minValue, maxValue;

                    MultiplicationRange():
                        minValue(Value::PlusInf),
                        maxValue(Value::MinusInf) {}

                    void multiply(Value value1, const Value& value2)
                    {
                        value1 *= value2;
                        if(value1 < minValue) minValue = value1;
                        if(maxValue < value1) maxValue = value1;
                    }
                };

                MinMaxTree result(1.0, 1.0);
                for(size_t a=0; a<GetParamCount(); ++a)
                {
                    MinMaxTree item = GetParam(a).CalculateResultBoundaries();
                    if(!item.has_min && !item.has_max) return MinMaxTree(); // hopeless

                    Value minValue0 = result.has_min ? Value(result.min) : Value(Value::MinusInf);
                    Value maxValue0 = result.has_max ? Value(result.max) : Value(Value::PlusInf);
                    Value minValue1 = item.has_min ? Value(item.min) : Value(Value::MinusInf);
                    Value maxValue1 = item.has_max ? Value(item.max) : Value(Value::PlusInf);

                    MultiplicationRange range;
                    range.multiply(minValue0, minValue1);
                    range.multiply(minValue0, maxValue1);
                    range.multiply(maxValue0, minValue1);
                    range.multiply(maxValue0, maxValue1);

                    if(range.minValue.valueType == Value::Finite)
                        result.min = range.minValue.value;
                    else result.has_min = false;

                    if(range.maxValue.valueType == Value::Finite)
                        result.max = range.maxValue.value;
                    else result.has_max = false;

                    if(!result.has_min && !result.has_max) break; // hopeless
                }
                if(result.has_min && result.has_max
                && result.min > result.max) std::swap(result.min, result.max);
                return result;
            }
            case cMod:
            {
                /* TODO: The boundaries of modulo operator could be estimated better. */

                MinMaxTree x = GetParam(0).CalculateResultBoundaries();
                MinMaxTree y = GetParam(1).CalculateResultBoundaries();

                if(y.has_max)
                {
                    if(y.max >= 0.0)
                    {
                        if(!x.has_min || x.min < 0)
                            return MinMaxTree(-y.max, y.max);
                        else
                            return MinMaxTree(0.0, y.max);
                    }
                    else
                    {
                        if(!x.has_max || x.max >= 0)
                            return MinMaxTree(y.max, -y.max);
                        else
                            return MinMaxTree(y.max, NEGATIVE_MAXIMUM);
                    }
                }
                else
                    return MinMaxTree();
            }
            case cPow:
            {
                if(GetParam(1).IsImmed() && GetParam(1).GetImmed() == 0.0)
                {
                    // Note: This makes 0^0 evaluate into 1.
                    return MinMaxTree(1.0, 1.0); // x^0 = 1
                }
                if(GetParam(0).IsImmed() && GetParam(0).GetImmed() == 0.0)
                {
                    // Note: This makes 0^0 evaluate into 0.
                    return MinMaxTree(0.0, 0.0); // 0^x = 0
                }
                if(GetParam(0).IsImmed() && FloatEqual(GetParam(0).GetImmed(), 1.0))
                {
                    return MinMaxTree(1.0, 1.0); // 1^x = 1
                }

                MinMaxTree p0 = GetParam(0).CalculateResultBoundaries();
                MinMaxTree p1 = GetParam(1).CalculateResultBoundaries();
                TriTruthValue p0_positivity =
                    (p0.has_min && p0.min >= 0.0) ? IsAlways
                  : (p0.has_max && p0.max < 0.0 ? IsNever
                    : Unknown);
                TriTruthValue p1_evenness = GetParam(1).GetEvennessInfo();

                /* If param0 IsAlways, the return value is also IsAlways */
                /* If param1 is even, the return value is IsAlways */
                /* If param1 is odd, the return value is same as param0's */
                /* If param0 is negative and param1 is not integer,
                 * the return value is imaginary (assumed Unknown)
                 *
                 * Illustrated in this truth table:
                 *  P=positive, N=negative
                 *  E=even, O=odd, U=not integer
                 *  *=unknown, X=invalid (unknown), x=maybe invalid (unknown)
                 *
                 *   param1: PE PO P* NE NO N* PU NU *
                 * param0:
                 *   PE      P  P  P  P  P  P  P  P  P
                 *   PO      P  P  P  P  P  P  P  P  P
                 *   PU      P  P  P  P  P  P  P  P  P
                 *   P*      P  P  P  P  P  P  P  P  P
                 *   NE      P  N  *  P  N  *  X  X  x
                 *   NO      P  N  *  P  N  *  X  X  x
                 *   NU      P  N  *  P  N  *  X  X  x
                 *   N*      P  N  *  P  N  *  X  X  x
                 *   *       P  *  *  P  *  *  x  x  *
                 *
                 * Note: This also deals with the following opcodes:
                 *       cSqrt  (param0, PU) (x^0.5)
                 *       cRSqrt (param0, NU) (x^-0.5)
                 *       cExp   (PU, param1) (CONSTANT_E^x)
                 */
                TriTruthValue result_positivity = Unknown;
                switch(p0_positivity)
                {
                    case IsAlways:
                        // e.g.   5^x = positive.
                        result_positivity = IsAlways;
                        break;
                    case IsNever:
                    {
                        result_positivity = p1_evenness;
                        break;
                    }
                    default:
                        switch(p1_evenness)
                        {
                            case IsAlways:
                                // e.g. x^( 4) = positive
                                // e.g. x^(-4) = positive
                                result_positivity = IsAlways;
                                break;
                            case IsNever:
                                break;
                            case Unknown:
                            {
                                /* If p1 is const non-integer,
                                 * assume the result is positive
                                 * though it may be NaN instead.
                                 */
                                if(GetParam(1).IsImmed()
                                && !GetParam(1).IsAlwaysInteger()
                                && GetParam(1).GetImmed() >= 0.0)
                                {
                                    result_positivity = IsAlways;
                                }
                                break;
                            }
                        }
                }
                switch(result_positivity)
                {
                    case IsAlways:
                    {
                        /* The result is always positive.
                         * Figure out whether we know the minimum value. */
                        double min = 0.0;
                        if(p0.has_min && p1.has_min)
                        {
                            min = pow(p0.min, p1.min);
                            if(p0.min < 0.0 && (!p1.has_max || p1.max >= 0.0) && min >= 0.0)
                                min = 0.0;
                        }
                        if(p0.has_min && p0.min >= 0.0 && p0.has_max && p1.has_max)
                        {
                            double max = pow(p0.max, p1.max);
                            if(min > max) std::swap(min, max);
                            return MinMaxTree(min, max);
                        }
                        return MinMaxTree(min, false);
                    }
                    case IsNever:
                    {
                        /* The result is always negative.
                         * TODO: Figure out whether we know the maximum value.
                         */
                        return MinMaxTree(false, NEGATIVE_MAXIMUM);
                    }
                    default:
                    {
                        /* It can be negative or positive.
                         * We know nothing about the boundaries. */
                        break;
                    }
                }
                break;
            }

            /* The following opcodes are processed by GenerateFrom()
             * within fpoptimizer_bytecode_to_codetree.cc and thus
             * they will never occur in the calling context:
             */
            case cNeg: // converted into cMul x -1
            case cInv: // converted into cPow x -1
            case cDiv: // converted into cPow y -1
            case cRDiv: // similar to above
            case cSub: // converted into cMul y -1
            case cRSub: // similar to above
            case cRad: // converted into cMul x CONSTANT_RD
            case cDeg: // converted into cMul x CONSTANT_DR
            case cSqr: // converted into cMul x x
            case cExp: // converted into cPow CONSTANT_E x
            case cExp2: // converted into cPow 2 x
            case cSqrt: // converted into cPow x 0.5
            case cRSqrt: // converted into cPow x -0.5
            case cCot: // converted into cMul (cPow (cTan x) -1)
            case cSec: // converted into cMul (cPow (cCos x) -1)
            case cCsc: // converted into cMul (cPow (cSin x) -1)
            case cLog10: // converted into cMul CONSTANT_L10I (cLog x)
            case cRPow: // converted into cPow y x
                break; /* Should never occur */

            /* Opcodes that do not occur in the tree for other reasons */
            case cDup:
            case cFetch:
            case cPopNMov:
            case cNop:
            case cJump:
            case VarBegin:
                break; /* Should never occur */

            /* Opcodes that are completely unpredictable */
            case cVar:
            case cPCall:
            case cFCall:
            case cEval:
                break; // Cannot deduce


            //default:
                break;
        }
        return MinMaxTree(); /* Cannot deduce */
    }
}

#endif

#line 1 "fpoptimizer/fpoptimizer_footer.txt"

#endif


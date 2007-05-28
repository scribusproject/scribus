/*
 *  digester.cpp
 *  
 *
 *  Created by Andreas Vox on 02.06.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */

#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <deque>
#include <functional>
#include <set>

#include "digester.h"
#include "automata.h"
#include "actions.h"

namespace desaxe {
	
typedef std::pair<Xml_string, Action> rule_t;
	
typedef unsigned short token_t;
typedef std::vector<token_t> path_t;
enum special_token { EMPTY = 0, START = 1, ANY = 2, REPEAT = 3 } ;

typedef unsigned short nfa_state_t;

struct DFA_State
{
	unsigned int ID;
	std::vector<rule_t> rules;
	
	DFA_State() : rules() {}
};

typedef DFA_State* dfa_state_t;


/**
 *   private implementation for Digester.
 *   uses a DFA to keep track of the currently recognized pattern(s).
 *   if you have no idea what a (non)deterministic finite automaton is you 
 *   probably don't want to read the rest of this code.
 */

class RuleState 
{ 
public:
	RuleState();
	RuleState(const RuleState& other);
	~RuleState();
	/// adds a rule to the list
	void addRule(const Xml_string& pattern, Action action);
	/// sets the pattern recognizing automaton to its start state
	void reset();
	/// returns the rules which apply for the currently recognized pattern
	const std::vector<rule_t>& rulesForCurrentState();
	/// enters a new state when \c tag is read
	void open(const Xml_string& tag);
	/// returns to the old state when the close tag is read
	void close();
	/// diagnostics
	void dump();
private:
	/// user sets these rules
	std::vector<rule_t> rules;
	
	/// rule patterns get broken into tokens:
	std::map<Xml_string, token_t> tokens;
	/// lists the accepting (NFA) states for each rule (same sequence as rules)
	std::vector<nfa_state_t> accepting;
	/// the dfa corresponding to the rule patterns
	automata::DFA<dfa_state_t, token_t> *dfa;
	/// stack of dfa states
	std::vector<dfa_state_t> stateStack;

	/// assigns a unique token to each string
	token_t createToken(const Xml_string& tok);
	void makeTokens();
	
	/// creates a nondeterministic automaton as base for the DFA
	automata::NFA<nfa_state_t, token_t>* createNFA();

	/// uses rules and createNFA() to create the DFA and assigns rules to states
	void compileDFA();
	/// is true after compileDFA has run
	bool valid;
};


Digester::Digester() : objects(), storage(), errors() 
{ 
	state = new RuleState();
	result_.ptr = NULL;
	result_.type = "";
}


Digester::~Digester() {
	delete state;
	deletePatches(patches);
}


Digester& Digester::operator=(const Digester& other)
{
	delete state;
	state = new RuleState(*other.state);
	objects = other.objects;
	storage = other.storage;
	result_ = other.result_;
	errors = other.errors;
	return *this;
}


int Digester::nrOfErrors() const
{
	return errors.size();
}

const Xml_string Digester::getError(int i) const
{
	return errors[i];
}


void Digester::addRule(const Xml_string& pattern, Action action)
{
	action.setDigester(this);
	state->addRule(pattern, action);
}


void Digester::beginDoc()
{
	state->reset();
#ifdef DESAXE_DEBUG	
	state->dump();
#endif
}

void Digester::endDoc()
{
}

void Digester::begin(const Xml_string& tag, Xml_attr attr)
{
	state->open(tag);
	const std::vector<rule_t>& rules (state->rulesForCurrentState());
	std::vector<rule_t>::const_iterator it;
	for(it=rules.begin(); it!=rules.end(); ++it)
	{
#ifdef DESAXE_DEBUG
		std::cerr << "B " << it->first << " " << typeid(it->second).name() << "\n";
#endif
		const_cast<Action&>(it->second).begin(tag, attr);
	}
}

void Digester::end(const Xml_string& tag)
{
	const std::vector<rule_t>& rules (state->rulesForCurrentState());
	std::vector<rule_t>::const_reverse_iterator it;
	for(it=rules.rbegin(); it!=rules.rend(); ++it)
	{
#ifdef DESAXE_DEBUG
		std::cerr << "E " << it->first << " " << typeid(it->second).name() << "\n";
#endif
		const_cast<Action&>(it->second).end(tag);
	}
	state->close();
}

void Digester::chars(const Xml_string& text)
{
	const std::vector<rule_t>& rules (state->rulesForCurrentState());
	std::vector<rule_t>::const_iterator it;
	for(it=rules.begin(); it!=rules.end(); ++it)
	{
#ifdef DESAXE_DEBUG
		std::cerr << "C " << it->first << " " << typeid(it->second).name() << "\n";
#endif
		const_cast<Action&>(it->second).chars(text);
	}
}


Xml_string Digester::concat(const Xml_string& pattern1, const Xml_string& pattern2)
{
	if (pattern1 == "")
		return pattern2;
	else if (pattern2 == "")
		return pattern1;
	else if ( (pattern1[pattern1.length()-1] != '/') && (pattern2[0] != '/') )
		// insert "/" as separator
		return pattern1 + "/" + pattern2;
	else if ( (pattern1[pattern1.length()-1]=='/') || (pattern2[0]=='/') )
		return pattern1 + pattern2;
	else // cut off one "/"
		return pattern1 + QString::fromStdString(std::string(static_cast<const std::string&>(pattern2.toStdString()), 1, std::string::npos));
}


RuleState::RuleState() : rules(), dfa(NULL), stateStack(), valid(false)
{}

RuleState::RuleState(const RuleState& other) : rules(other.rules), dfa(NULL), stateStack(), valid(false)
{}

RuleState::~RuleState()
{
	if (dfa) 
	{
		std::set<dfa_state_t> morituri(dfa->states());
		for (std::set<dfa_state_t>::iterator i=morituri.begin(); i != morituri.end(); ++i) {
			delete *i;
		}
		delete dfa;
	}
}


void RuleState::addRule(const Xml_string& pattern, Action action)
{
	rules.push_back(std::pair<Xml_string, Action>(pattern,action));
	valid = false;
}


inline
void RuleState::reset()
{
	stateStack.clear();
	if (!valid)
		compileDFA();
	stateStack.push_back(dfa->start());
}

void RuleState::dump()
{
	std::cout << "Rules:\n";
	for (unsigned int r=0; r<rules.size(); ++r) {
		std::cout << r << ":\t\"" << rules[r].first.toStdString() << "\" accepted in  (" << accepting[r] << ")\n";
	}
	std::cout << "\nTokens:\n";
	for (std::map<Xml_string, token_t>::iterator it=tokens.begin(); it!=tokens.end(); ++it) {
		std::cout << it->first.toStdString() << ":\t--> " << it->second << "\n";
	}
	std::cout << "\nAutomaton:\n";
	const std::set<dfa_state_t>& states(dfa->states());
	const std::set<token_t>& inputs(dfa->inputs());
	std::cout << "STATE";
	for (std::set<token_t>::const_iterator i=inputs.begin(); i != inputs.end(); ++i) {
		std::cout << "\t" << *i;
	}
	std::cout << "\tRULES\n\n";
	for (std::set<dfa_state_t>::const_iterator s=states.begin(); s!=states.end(); ++s) {
		std::cout << (*s)->ID;
		for (std::set<token_t>::const_iterator i=inputs.begin(); i!=inputs.end(); ++i) {
			dfa_state_t nstate = dfa->next(*s,*i);
			std::cout << "\t";
			if (nstate)
				std::cout << nstate->ID;
			else
				std::cout << "--";
		}
		for (std::vector<rule_t>::iterator rs=(*s)->rules.begin(); rs!=(*s)->rules.end(); ++rs)
			std::cout << "\t\"" << rs->first.toStdString() << "\"";
		std::cout << "\n";
	}
}

inline
void RuleState::open(const Xml_string& tag)
{
	dfa_state_t nstate = dfa->next(stateStack.back(), tokens[tag]);
	assert(nstate != NULL);
#ifdef DESAXE_DEBUG
	std::cerr << "to state " << nstate->ID << "\n"; 	
#endif
	if (nstate->ID == dfa->deflt()->ID) {
		nstate = dfa->next(stateStack.back(), ANY);
#ifdef DESAXE_DEBUG
		std::cerr << "empty, trying any:" << nstate->ID << "\n"; 
#endif
	}
	stateStack.push_back(nstate);
}

inline
void RuleState::close()
{
	stateStack.pop_back();
}


inline
const std::vector<rule_t>& RuleState::rulesForCurrentState()
{
	return stateStack.back()->rules;
}

void RuleState::makeTokens()
{
	tokens.clear();
	tokens[""] = EMPTY;
	tokens["/"] = START;
	tokens["*"] = ANY;
	tokens["**"] = REPEAT;
}



token_t RuleState::createToken(const Xml_string& tok)
{
	if (tokens.find(tok) == tokens.end())
		tokens[tok] = tokens.size() + 1;
	return tokens[tok];
}



automata::NFA<nfa_state_t, token_t>* RuleState::createNFA()
{
	using automata::NFA;
	
	/// maps paths uniquely to an nfa_state
	std::map<path_t, nfa_state_t> nfa_states;
	nfa_states.clear();

	path_t prefix;

	// START and EMPTY are both: tokens and nfa_states
	prefix.push_back(START);
	nfa_states[prefix] = START;
	
	prefix[0] = EMPTY;
	nfa_states[prefix] = EMPTY;
	
	std::set<nfa_state_t> deflt;
	deflt.insert(EMPTY);
	
	NFA<nfa_state_t, token_t> *nfa = new NFA<nfa_state_t, token_t>(START, deflt);
	
	nfa->addState(EMPTY);
	nfa->addInput(ANY);
	nfa->addTransition(EMPTY, ANY, EMPTY);

	for (unsigned int i = 0; i < rules.size(); ++i)
	{
		const std::string currPattern(fromXMLString(rules[i].first));
		const unsigned int len = currPattern.length();
		int pos;
		nfa_state_t lastState;
		
		// determine if this is a start pattern
		prefix.resize(1);
		if (currPattern[0] == '/') {
			prefix[0] = START;
			pos = 1;
			lastState = START;
		}
		else {
			prefix[0] = EMPTY;
			pos = 0;
			lastState = EMPTY;
		}
		
//		std::cerr << "looking at pattern: " << currPattern << "\n";
		// for all prefixes
		do {
			std::string::size_type pos2 = currPattern.find('/', pos);
			if (pos2 == std::string::npos)
				pos2 = len;
			
			std::string diff(currPattern.substr(pos, pos2-pos));
			token_t tok = createToken(fromSTLString(diff));
//			std::cerr << pos << "-" << pos2 << "\t: " << diff << " = " << tok << "\n";

			// create loop if REPEAT token
			if (tok == REPEAT) {
				nfa->addTransition(lastState, ANY, lastState); //FIXME: that's wrong, need to create repeating state
//				std::cerr << "T " << lastState << "--*-->" << lastState << "\n";
				pos = pos2 + 1;
				continue;
			}
			
			prefix.push_back(tok);
			// create new state if necessary
			nfa_state_t nstate;
			if (nfa_states.find(prefix) != nfa_states.end()) {
				nstate = nfa_states[prefix];
			}
			else {
				nstate = nfa_states.size();
				nfa->addState(nstate);
				nfa_states[prefix] = nstate;
			}
			// add transition
			nfa->addInput(tok);
			nfa->addTransition(lastState, tok, nstate);
//			std::cerr << "T " << lastState << "--(" << tok << ")-->" << nstate << "\n";
			lastState = nstate;
			pos = pos2 + 1;
		} while(pos < signed(len));
		accepting.push_back(lastState);
//		std::cerr << "accepted in " << lastState << "\n";

		// copy all transition from EMPTY to all other states
		const NFA<nfa_state_t, token_t>::Transitions& transFromEmpty(nfa->transitions(EMPTY));
		std::set<nfa_state_t>::const_iterator it, st;
		NFA<nfa_state_t, token_t>::Transitions::const_iterator tr;
		for (it = nfa->states().begin(); it != nfa->states().end(); ++it) {
			if (*it == EMPTY)
				continue;
			for (tr = transFromEmpty.begin(); tr != transFromEmpty.end(); ++tr)
				for (st = tr->second.begin(); st != tr->second.end(); ++st)
					nfa->addTransition(*it, tr->first, *st);
		}
		
		// ANY transitions
		const std::set<token_t>& inputs(nfa->inputs());
		std::set<token_t>::const_iterator tok;
		for (it = nfa->states().begin(); it != nfa->states().end(); ++it) {
			const std::set<nfa_state_t>& anyStates(nfa->next(*it, ANY));
			for (st = anyStates.begin(); st != anyStates.end(); ++st)
				for (tok=inputs.begin(); tok != inputs.end(); ++tok)
					if (*tok != ANY)
						nfa->addTransition(*it, *tok, *st);
		}
	}
	return nfa;
}

struct CreateDFAState : public std::unary_function <std::set<nfa_state_t>, dfa_state_t> {

	CreateDFAState(const std::vector<rule_t>& rules, const std::vector<nfa_state_t>& accepting) 
	: n(0), rules_(rules), accepting_(accepting) 
	{}

	dfa_state_t operator()(const std::set<nfa_state_t>& combination)
	{
		dfa_state_t result = new DFA_State();
		result->ID = n++;
		result->rules.clear();
		for (unsigned int i=0; i < rules_.size(); ++i)
			if (combination.find(accepting_[i]) != combination.end())
				result->rules.push_back(rules_[i]);
		return result;
	} 

	unsigned int n;
	const std::vector<rule_t>& rules_;
	const std::vector<nfa_state_t>& accepting_;
};

//dfa_state_t create_fun(const std::set<nfa_state_t>& combination);

void RuleState::compileDFA()
{	
	// build NFA:
	makeTokens();	
	automata::NFA<nfa_state_t, token_t> *nfa = createNFA();
	// make deterministic:
	CreateDFAState create(rules, accepting);
//	dfa = nfa->constructDFA<dfa_state_t>(create);        // wont work
//	dfa = nfa->constructDFA<dfa_state_t>(create_fun);    // works
//	std::set<nfa_state_t> arg;
//	dfa_state_t res = create(arg);                       // works
//	create("bullshit");                                  // never works :-)
	dfa = nfa->constructDFA<dfa_state_t,CreateDFAState&>(create);   // finally
	delete nfa;
	valid = true;
}

} // namespace

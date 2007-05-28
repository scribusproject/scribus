/*
 *  automata.h
 *  
 *
 *  Created by Andreas Vox on 02.06.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */


#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <map>
#include <set>
#include <deque>

namespace automata {

template<class STATE, class INPUT, class OUTPUT>
class FA_base {
public:
	FA_base(STATE s, OUTPUT d);
	FA_base(const std::set<STATE>& states, const std::set<INPUT>& inputs, STATE start, STATE deflt);
	virtual ~FA_base();

	typedef std::map<INPUT, OUTPUT> Transitions;

	const std::set<STATE>& states() const;
	const std::set<INPUT>& inputs() const;
	const Transitions& transitions(STATE s) const;
	const STATE start() const;
	const OUTPUT deflt() const;
	const OUTPUT next(STATE from, INPUT input) const;
	
	void addState(STATE newState);
	void addInput(INPUT newInput);
	void setTransition(STATE from, INPUT input, OUTPUT to);

protected:
	std::set<STATE> states_;
	std::set<INPUT> inputs_;
	std::map<STATE, Transitions> transitions_;
	const Transitions noTransitions;
	STATE start_;
	OUTPUT default_;
};

template<class STATE, class INPUT, class OUTPUT>
FA_base<STATE, INPUT, OUTPUT>::FA_base(STATE s, OUTPUT d) : states_(), inputs_(), transitions_(), noTransitions(), start_(s), default_(d) 
{ 
	states_.insert(s); 
}

template<class STATE, class INPUT, class OUTPUT>
FA_base<STATE, INPUT, OUTPUT>::FA_base(const std::set<STATE>& states, const std::set<INPUT>& inputs, STATE start, STATE deflt) : states_(states), inputs_(inputs), transitions_(), noTransitions(), start_(start), default_(deflt) 
{
	if (states_.find(start) == states_.end())
		states_.insert(start);	
}

template<class STATE, class INPUT, class OUTPUT>
const std::set<STATE>& FA_base<STATE, INPUT, OUTPUT>::states() const 
{ 
	return states_; 
}

template<class STATE, class INPUT, class OUTPUT>
const std::set<INPUT>& FA_base<STATE, INPUT, OUTPUT>::inputs() const 
{ 
	return inputs_; 
}

template<class STATE, class INPUT, class OUTPUT>
const STATE FA_base<STATE, INPUT, OUTPUT>::start() const
{ 
	return start_; 
}

template<class STATE, class INPUT, class OUTPUT>
const OUTPUT FA_base<STATE, INPUT, OUTPUT>::deflt() const
{ 
	return default_; 
}

template<class STATE, class INPUT>
class DFA : public FA_base<STATE, INPUT, STATE> {
public:
	DFA(STATE s, STATE deflt): FA_base<STATE, INPUT, STATE>(s, deflt) {  }
	DFA(const std::set<STATE>& states, const std::set<INPUT>& inputs, STATE start, STATE deflt)
	: FA_base<STATE, INPUT, STATE>(states, inputs, start, deflt) { }
};


template<class STATE, class INPUT>
class NFA : public FA_base<STATE, INPUT, std::set<STATE> > {
public:
	NFA(STATE s, std::set<STATE> d): FA_base<STATE, INPUT, std::set<STATE> >(s, d)  { }
	NFA(std::set<STATE>& states, std::set<INPUT>& inputs, STATE start, std::set<STATE> deflt)
	: FA_base<STATE, INPUT, STATE>(states, inputs, start, deflt) { }

	void addTransition(STATE from, INPUT input, STATE to);
	
	/*
	  NFA:  S x I -> P(S)
	  DFA':  P(S) x I -> P(S)
	  DFA:  N x I -> N
	 Algo:
	  todo = { NFA.start }
	  forall src in todo:
	     from = create(src)
	     ntrans: I -> P(S)
	     for all s in src
	       for all (i,t) in NFA.trans[s]:
	         ntrans[i] += t;
	     for all (i,nt) in ntrans:
	       n = create(nt);
		   if n is new:
	         DFA.addState(n);
			 todo += nt;
	       DFA.addTransition(from, i, n)
	 */
	template<class NSTATE, class XXX>
	DFA<NSTATE,INPUT>* constructDFA( XXX createState )
//	DFA<NSTATE,INPUT>* constructDFA(NSTATE (*createState)( std::set<STATE> ))  // wont work
	{
		std::map<std::set<STATE>, NSTATE> newStates;
	
		std::set<STATE> startSet;
		startSet.insert(FA_base<STATE,INPUT,std::set<STATE> >::start_);
		NSTATE nstart = createState(startSet);
		newStates[startSet] = nstart;

		NSTATE deflt = createState(FA_base<STATE,INPUT,std::set<STATE> >::default_);
		newStates[FA_base<STATE,INPUT,std::set<STATE> >::default_] = deflt;
		
		DFA<NSTATE,INPUT>* result = new DFA<NSTATE,INPUT>(nstart, deflt);
		result->addState(deflt);
		
		std::deque<std::set<STATE> > todo;
		todo.push_back(startSet);
		
		while (todo.size() > 0)
		{
			const std::set<STATE> from = todo.front();
			todo.pop_front();
			std::map<INPUT,std::set<STATE> > ntrans;
			// for all states in from
			typename std::set<STATE>::const_iterator s_it;
			typename std::map<INPUT, std::set<STATE> >::iterator tr_it;
			for (s_it=from.begin(); s_it != from.end(); ++s_it)
			{
				// for all transitions
				typename FA_base<STATE,INPUT,std::set<STATE> >::Transitions& 
					trans(FA_base<STATE,INPUT,std::set<STATE> >::transitions_[*s_it]);
				for (tr_it = trans.begin(); tr_it != trans.end(); ++tr_it)
				{
					ntrans[tr_it->first].insert(tr_it->second.begin(), tr_it->second.end());
				}
			}
			// construct new transitions
			const NSTATE nfrom = newStates[from];
			for (tr_it = ntrans.begin(); tr_it != ntrans.end(); ++tr_it)
			{
				std::set<STATE> & state(tr_it->second);
				// create follow state
				NSTATE nstate;
				if ( newStates.find(state) == newStates.end() ) {
					nstate = createState(state);
					result->addState(nstate);
					newStates[state] = nstate;
					// remember follow state in todo if new
					todo.push_back(state);
				}
				else {
					nstate = newStates[state];
				}
				result->setTransition(nfrom, tr_it->first, nstate);
			}
		}
		const std::set<INPUT>& inp(FA_base<STATE, INPUT, std::set<STATE> >::inputs());
		typename std::set<INPUT>::const_iterator i;
		for (i=inp.begin(); i != inp.end(); ++i)
			result->addInput(*i);
		return result;
	}
};


template<class STATE, class INPUT, class OUTPUT>
FA_base<STATE,INPUT,OUTPUT>::~FA_base() 
{
	// clean up
}
	
template<class STATE, class INPUT, class OUTPUT>
const typename FA_base<STATE,INPUT,OUTPUT>::Transitions& FA_base<STATE,INPUT,OUTPUT>::transitions(STATE s) const
{ 
	typename std::map<STATE, Transitions>::const_iterator tr = transitions_.find(s);
	if (tr != transitions_.end())
		return tr->second;
	else
		return noTransitions; 
}

template<class STATE, class INPUT, class OUTPUT>
const OUTPUT FA_base<STATE,INPUT,OUTPUT>::next(STATE from, INPUT input) const
{
	const Transitions& tr(transitions(from));
	typename Transitions::const_iterator it = tr.find(input);
	return it==tr.end() ? default_ : it->second;
}

template<class STATE, class INPUT, class OUTPUT>
void FA_base<STATE,INPUT,OUTPUT>::addState(STATE newState)
{
	if (states_.find(newState) == states_.end())
		states_.insert(newState);
}


template<class STATE, class INPUT, class OUTPUT>
void FA_base<STATE,INPUT,OUTPUT>::addInput(INPUT newInput)
{
	if (inputs_.find(newInput) == inputs_.end())
		inputs_.insert(newInput);
}


template<class STATE, class INPUT, class OUTPUT>
void FA_base<STATE,INPUT,OUTPUT>::setTransition(STATE from, INPUT input, OUTPUT to)
{
	Transitions& trans(transitions_[from]);
	trans[input] = to;
}
	

template<class STATE, class INPUT>
void NFA<STATE, INPUT>::addTransition(STATE from, INPUT input, STATE to)
{
	typename FA_base<STATE,INPUT,std::set<STATE> >::Transitions & 
		trans(FA_base<STATE,INPUT,std::set<STATE> >::transitions_[from]);
	trans[input].insert(to);
}

} // namespace

#endif

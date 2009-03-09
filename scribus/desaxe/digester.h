/*
 *  digester.h
 *  
 *
 *  Created by Andreas Vox on 02.06.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */



#ifndef DIGESTER_H
#define DIGESTER_H

#include <cassert>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <typeinfo>

#include "desaxe_conf.h"
#include "saxhandler.h"


//#define DESAXE_DEBUG 1

namespace desaxe {
	
class Action;
class RuleState;


namespace PRIVATE {
	
	class VarPtr
	{
	public:
		void* ptr;
		std::string type;
	};

	
	template<class ObjType>
	inline
	VarPtr mkcell(ObjType* obj)
	{
		VarPtr result;
		result.ptr = obj;//	result.ptr = const_cast<void*>(static_cast<const void*>(obj)); //??
		result.type = typeid(obj).name();
		return result;
	}
	
	
	template<class ObjType>
	inline
	void chkcell(const VarPtr& cell, std::vector<VarPtr>* stack = NULL)
	{
		ObjType* dummy = NULL;
		if( cell.type != typeid(dummy).name() )
		{
			std::cerr << "requested type '" << typeid(dummy).name() << "' doesnt match cell type '" << cell.type << "'\n";
			if (stack)
			{
				int i=0;
				std::vector<VarPtr>::iterator it;
				for (it = stack->begin(); it != stack->end(); ++it)
				{
					std::cerr << i++ << "\t" << (*it).type << "\t" << (*it).ptr << "\n";
				}
			}
			assert (false);
		}
	}
	


class Patch {
public:
	// internal linked list
	Patch* next;
	Patch(Patch* nxt) : next(nxt) {}
	
	virtual void run(VarPtr lnk) = 0;
	virtual ~Patch() {}
};


} // namespace PRIVATE	



/**
   Digester helps you to create C++ objects from a SAX stream.
   All you have to do is register actions for patterns of element tags
   you expect in your XML stream. These actions can create new objects, set
   attributes, or call methods. Actions operate on intermediate objects which 
   are hold on a stack maintained by Digester.  
 */
class Digester : public SaxHandler {	
public:
	Digester();
	Digester& operator=(const Digester& other);
	virtual ~Digester();
	void addRule(const Xml_string& pattern, Action action);
	
	void parseFile(const Xml_string& filename);
	void parseMemory(const char* data, unsigned int length);
	
	template<class ObjType>
		ObjType*  result();	
	int nrOfErrors() const;
	const Xml_string getError(int i) const;
	
// called by SAX parser:
	void beginDoc();
	void endDoc();
	void begin(const Xml_string& tag, Xml_attr attr);
	void end(const Xml_string& tag);
	void chars(const Xml_string& text);
	
// used by actions:
	void fail();
	void error(const Xml_string& msg);
	
	template<class ObjType>
	ObjType*  top(unsigned int offset = 0);
	
	template<class ObjType>
	ObjType*  bottom(unsigned int offset = 0);
	
	template<class ObjType>
	void setResult(	ObjType* res );
	
	void pop();
	void popn(unsigned int number);
	
	template<class ObjType>
	void push(ObjType* obj);
	
//  used to resolve idrefs and for general storage	
	template<class ObjType>
		ObjType*  lookup(const Xml_string& idref);	
	template<class ObjType>
		void store(const Xml_string& idref, ObjType* res );
	
//  used to resolve idrefs if use is before definition
	template<class LinkType>
		void patchCall(const Xml_string& idref, void (*fun)(LinkType*) );
	template<class ObjType, class LinkType>
		void patchInvoke(const Xml_string& idref, ObjType* obj, void (ObjType::*fun)(LinkType*) );
	
	// used to insert "/" where necessary
	static Xml_string concat(const Xml_string& pattern1, const Xml_string& pattern2);

private:
	RuleState* 
		state;
	
	std::vector<PRIVATE::VarPtr> 
		objects;
	
	std::map<Xml_string, PRIVATE::VarPtr> 
		storage;

	std::map<Xml_string, PRIVATE::Patch*>
		patches;
	
	PRIVATE::VarPtr 
		result_;
	
	std::vector<Xml_string> 
		errors;
};



template<class ObjType>
inline
ObjType*  Digester::top(unsigned int offset) 
{
#ifdef DESAXE_DEBUG
	std::cerr << "top(" << offset << ") of " << objects.size() << "\n";
#endif
	unsigned int count = objects.size();
	assert (offset < count); 
	PRIVATE::chkcell<ObjType>(objects[count - offset - 1], &objects);
#ifdef DESAXE_DEBUG
	std::cerr << "stack-> " << static_cast<ObjType*>(objects[count - offset - 1].ptr) << "\n";
#endif
	
	return static_cast<ObjType*>(objects[count - offset - 1].ptr);
}


template<class ObjType>
inline
ObjType*  Digester::bottom(unsigned int offset) 
{ 
#ifdef DESAXE_DEBUG
	std::cerr << "bottom(" << offset << ") of " << objects.size() << "\n";
#endif
	unsigned int count = objects.size();
	assert (offset < count); 
	PRIVATE::chkcell<ObjType> (objects[offset]);
	return static_cast<ObjType*>(objects[offset].ptr);
}


template<class ObjType>
inline
ObjType*  Digester::result() 
{ 
	ObjType* dummy = NULL;
	if (result_.type != typeid(dummy).name())
		return NULL;
#ifdef DESAXE_DEBUG
	std::cerr << "result-> " << static_cast<ObjType*>(result_.ptr) << "\n";
#endif
	return static_cast<ObjType*>(result_.ptr);
}


template<class ObjType>
inline
void Digester::setResult(ObjType* res) 
{ 
#ifdef DESAXE_DEBUG
	std::cerr << res << " ->result\n";
#endif
	result_ = PRIVATE::mkcell(res);
}


inline
void Digester::pop()
{
	assert (1 <= (unsigned int) objects.size());
	objects.pop_back();
}

inline
void Digester::popn(unsigned int number)
{
	unsigned int count = (unsigned int) objects.size();
	assert (number <= count);
	objects.resize(count - number);
}


template<class ObjType>
inline
void Digester::push(ObjType* obj)
{
#ifdef DESAXE_DEBUG
	std::cerr << "stack<- " << obj << "\n";
#endif
	objects.push_back(PRIVATE::mkcell(obj));
}


// now lookup / store / patch business

namespace PRIVATE {
	
	template <class LinkType>
	struct Patch1 : public Patch
	{
		typedef void (*FunType1)(LinkType*);
		FunType1 fun;
		
		Patch1(FunType1 fn, Patch* nxt = NULL) : Patch(nxt), fun(fn) {}
		
		void run(VarPtr link) 
		{ 
			fun( static_cast<LinkType*>(link.ptr) ); 
		}
	};
	
	
	template <class ObjType, class LinkType>
		struct Patch2 : public Patch
	{
		typedef void (ObjType::*FunType2)(LinkType*);
		ObjType* obj;
		FunType2 fun;
		
		Patch2(ObjType* ob, FunType2 fn, Patch* nxt = NULL) : Patch(nxt), obj(ob), fun(fn) {}

		void run(VarPtr link)
		{
			(obj->*fun)( static_cast<LinkType*>(link.ptr) ); 
		}
	};
	
	
	inline
	void runPatches(Patch*& list, VarPtr link)
	{
		while (list)
		{
			Patch* nxt = list->next;
			list->run(link);
			delete list;
			list = nxt;
		}
	}
	
	inline
	void deletePatches(std::map<Xml_string, Patch*>& patches)
	{
		std::map<Xml_string, Patch*>::iterator it;
		for (it = patches.begin(); it != patches.end(); ++it)
		{
			Patch* list = it->second;
			while (list)
			{
				Patch* nxt = list->next;
				delete list;
				list = nxt;
			}
		}
		patches.clear();
	}

	
//	template<> class Patch1<void>;
//	template<> class Patch2<VarPtr,void>;
	
} //namespace PRIVATE



template<class ObjType>
inline
ObjType*  Digester::lookup(const Xml_string& idref) 
{ 
	using namespace PRIVATE;

	std::map<Xml_string, VarPtr>::iterator cell = storage.find(idref);
	if (cell == storage.end())
	{
#ifdef DESAXE_DEBUG
		std::cerr << "lookup[" << idref << "]-> NULL\n";
#endif
		return NULL;
	}
	else
	{
		chkcell<ObjType> (cell->second);
#ifdef DESAXE_DEBUG
		std::cerr << "lookup[" << idref << "]-> " << static_cast<ObjType*>(cell->second.ptr) << "\n";
#endif
		return static_cast<ObjType*>(cell->second.ptr);
	}
}



template<class ObjType>
inline
void Digester::store(const Xml_string& idref, ObjType* obj) 
{
	using namespace PRIVATE;
#ifdef DESAXE_DEBUG
	std::cerr << "store[" << idref << "] <- " << obj << "\n";
#endif
	storage[idref] = mkcell(obj);
	runPatches(patches[idref], storage[idref]);
}



template<class LinkType>
void Digester::patchCall(const Xml_string& idref, void (*fun)(LinkType*) )
{
	using namespace PRIVATE;
	
	std::map<Xml_string, VarPtr>::iterator cell = storage.find(idref);
	if (cell == storage.end())
	{
		patches[idref] = new Patch1<LinkType>(fun, patches[idref] );
	}
	else
	{
		Patch1<LinkType>(fun).run(cell->second);
	}
}


template<class ObjType, class LinkType>
void Digester::patchInvoke(const Xml_string& idref, ObjType* obj, void (ObjType::*fun)(LinkType*) )
{
	using namespace PRIVATE;
	
	std::map<Xml_string, VarPtr>::iterator cell = storage.find(idref);
	if (cell == storage.end())
	{
		patches[idref] = new Patch2<ObjType,LinkType>(obj, fun, patches[idref] );
	}
	else
	{
		Patch2<ObjType,LinkType>(obj, fun).run(cell->second);
	}
}

} // namespace desaxe

#endif

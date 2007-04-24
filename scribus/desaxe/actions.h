/*
 *  actions.h
 *  
 *
 *  Created by Andreas Vox on 02.06.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */


#ifndef ACTIONS_H
#define ACTIONS_H

#include <map>
#include <string>
#include "digester.h"

namespace desaxe {

/**
 *   Action / Action_body follow the handle/body pattern. This allows to omit the
 *   new operator when creating Action expressions and make dynamic memory handling
 *   more secure. 
 *   Base class for the body of all actions. Usually Digester calls these methods,
 *   but subclasses are also allowed to call those methods if they know what they are
 *   doing...
 */
class Action_body
{
protected:
	Action_body() :                     dig(NULL)  {}
	virtual ~Action_body()                         {}
	virtual void begin(const Xml_string&, Xml_attr) {} 
                                               
	virtual void end(const Xml_string&)             {}
	virtual void chars(const Xml_string&)           {}

	Digester* dig;
private:
	int refs;
	friend class Action;
};



/**
 *   Actions do all the real work when digesting XML files: creating new objects,
 *   setting attributes, calling methods. Each Action gets control two or more times
 *   for each time the corresponding rule triggers: 1 x begin(), 0- x chars(), 1 x end().
 *   They come in two basic flavors: Generators create a new object in their begin()
 *   method and put it on the object stack. Other Actions use objects on the stack,
 *   XML attributes or XML text to store this data in other objects. This is usually
 *   done in the end() method.
 *   Warning: end() methods are called in reverse order. This is to ensure that they
 *   see exactly the same stack content as their corresponging begin() method.
 *   This is the handle class which delegates to the body
 */
class Action
{
public:
	inline Digester* digester()                { return body->dig; }
	inline void setDigester(Digester* dig)     { body->dig = dig; }
	
	inline void begin(const Xml_string& tag, Xml_attr attr) 
	                                           { body->begin(tag, attr); }
	inline void end(const Xml_string& tag)     { body->end(tag); }
	inline void chars(const Xml_string& data)  { body->chars(data); }

	// Handle stuff:
	Action(const Action& other)
	{
		body = other.body;
		body->refs++;
	}
	virtual ~Action()                          
	{
		if (--body->refs == 0)
			delete body;
	}
	Action& operator=(const Action& other)
	{
		if (body != other.body) {
			if (--body->refs == 0)
				delete body;
			body = other.body;
			++body->refs;
		}
		return *this;
	}

protected:
	Action(Action_body* body_)
	{
		body = body_;
		body->refs = 1;
	}

private:
	Action();  // not defined
	
	Action_body* body;
};


class Dummy {};

/**
 * This class implements typed constructors. They are in a subclass since
 * we don't want the type arguments in Action itself.
 * Use "class MyAction : public MakeAction<MyAction_body> {} " to define
 * your own Actions. You have to implement MyAction constructors if you
 * want constructors with arguments.
 */
template <class Body, class Arg1=Dummy, class Arg2=Dummy, class Arg3=Dummy, class Arg4=Dummy, class Arg5=Dummy>
struct MakeAction : public Action 
{
	MakeAction() : Action(new Body()) {}
	MakeAction(Arg1 a1) : Action(new Body(a1)) {}
	MakeAction(Arg1 a1, Arg2 a2) : Action(new Body(a1, a2)) {}
	MakeAction(Arg1 a1, Arg2 a2, Arg3 a3) : Action(new Body(a1, a2, a3)) {}
	MakeAction(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) : Action(new Body(a1, a2, a3, a4)) {}
	MakeAction(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) : Action(new Body(a1, a2, a3, a4, a5)) {}
};



/** 
 *   Abstract class for actions which leave a new object on the stack
 *   The begin() method is defined in subclasses and puts the object on stack,
 *   the generic end() method removes this object from stack.
 */
template<class Type>
class Generator_body : public Action_body
{
public:
	virtual Type* eval(Digester* dig_, const Xml_string& tag, Xml_attr attr)
	{
		dig = dig_;
		begin(tag, attr);
		Type* res;
		res = dig->template top<Type>();
		end(tag);
		return res;
	}
protected:
	virtual void end(const Xml_string& ) { dig->pop(); }
};



/**
 * Generators have their own handle class, which is a subclass of Action
 */
template<class Type>
class Generator : public Action
{
public:
	Type* eval(Digester* dig, const Xml_string& tag, Xml_attr attr)
	{
		return static_cast<Generator_body<Type>*>(body)->eval(dig, tag, attr);
	}

protected:
	Generator(Generator_body<Type>* body_) : Action(body_) {}
};


/**
 * This class implements typed constructors. They are in a subclass since
 * we don't want the type arguments in Generator itself.
 * Use "class MyGenerator : public MakeGenerator<MyGenarator_body, MyType> {} " to 
 * define your own generators. You have to implement MyGenerator constructors if you
 * want constructors with arguments.
 */
template <class Body, class Obj_Type, class Arg1=Dummy, class Arg2=Dummy, class Arg3=Dummy>
struct MakeGenerator : public Generator<Obj_Type> 
{
	MakeGenerator() : Generator<Obj_Type>(new Body()) {}
	MakeGenerator(Arg1 a) : Generator<Obj_Type>(new Body(a)) {}
	MakeGenerator(Arg1 a1, Arg2 a2) : Generator<Obj_Type>(new Body(a1, a2)) {}
	MakeGenerator(Arg1 a1, Arg2 a2, Arg3 a3) : Generator<Obj_Type>(new Body(a1, a2, a3)) {}
};

} // namespace

#endif

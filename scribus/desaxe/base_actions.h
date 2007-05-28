/*
 *  base_actions.h
 *  
 *
 *  Created by Andreas Vox on 02.06.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */


#ifndef BASE_ACTIONS_H
#define BASE_ACTIONS_H

/*****

    Defines the following actions:

    Attribute<D>( name, def)                        - -> D
    Attribute<D>( name )                            - -> D
    *RefAttribute<D>( name )                         - -> D
    *Get<O,D>( gen<O>, fun )                         - -> D
    *Set<O,D>( gen<O>, fun, gen<D> )                 - -> -
    *Top<D>( n )                                     - -> D
    *Bottom<D>( n )                                  - -> D
    *WithText<O,D>( gen<O>, fun )                    - -> -
    *Invoke<O, F, A1, ...>(fun, a1, ...)             O -> O
    *InvokeLater<O, F, A1, ...>(fun, a1, ...)        O -> O
    *Call<F,A1, ..., An>(fun, a1, ..., an)           - -> -
    *CallLater<F,A1, ..., An>(fun, a1, ..., an)      - -> -
    *Filter<D,E> ( gen<D>, fun )                     - -> E
    *Combine<D,E,F> ( gen<D>, gen<D>, fun)           - -> F

*****/


/**
 *   Converts an attribute to an object of type Data_Type and pushes it onto the stack.
 */


template<class Data_Type>
class Attribute_body : public Generator_body<Data_Type>
{
public:
	Attribute_body(const Xml_string& name) 
	: name_(name), default_(NULL) 
{}
	
	Attribute_body(const Xml_string& name, const Data_Type& def) 
	: name_(name), default_(new Data_Type(def)) 
{}
	
	~Attribute_body() 
{ 
		if (default_) 
			delete default_; 
}

void begin(const Xml_string&, Xml_attr)
{
	if (attr.contains(name_))
		dig->push(new Data_Type(attr[name_]));
	else if (default_)
		dig->push(new Data_Type(default_));
	else
		dig->push(NULL);
}
private:
const Xml_string name_;
const Data_Type* default_;
};


template <class Type>
struct  Attribute : public MakeGenerator<Attribute_body<Type>, const Xml_string&, const Type&> 
{ 
	Attribute(const Xml_string& name)
	: MakeGenerator<Attribute_body<Type>, const Xml_string&>::MakeGenerator(name) {} 
	
	Attribute(const Xml_string& name, const Type& def)
	: MakeGenerator<Attribute_body<Type>, const Xml_string&, const Type&>::MakeGenerator(name, def) {} 
	
};



/*
 template<class Functor>
 class Call0 : public Action
 {
public:
	 Call0(Functor f) : fun(f)
	{}
	 
	 void begin(const std::string, std::map<std::string,std::string> attr)
	{
		 fun();
	}	
private:
	 Functor fun;
 };
 
 
 template<class Functor, class Arg1>
 class Call1 : public Action
 {
public:
	 Call1(Functor f, Generator<Arg1> a1) : fun(f), arg1(a1)
	{}
	 
	 void begin(const std::string, std::map<std::string,std::string> attr)
	{
		 fun(arg1->eval(digester(), tag, attr));
	}
private:
	 Functor fun;
	 Generator<Arg1> arg1;
 };
 
 template<class Functor, class Arg1, class Arg2>
 class Call2 : public Action
 {
public:
	 Call2(Functor f, Generator<Arg1>* a1, Generator<Arg2>* a2) : fun(f), arg1(a1), arg2(a2) 
	{}
	 
	 void begin(const std::string, std::map<std::string,std::string> attr)
	{
		 fun(arg1->eval(digester(), tag, attr), 
			 arg2->eval(digester(), tag, attr));
	}
private:
	 Functor fun;
	 Generator<Arg1> arg1;	
	 Generator<Arg2> arg2;	
 };
 
 /* Baustelle
 template<class Obj_Type, class Data_Type>
 class Invoke : public Action 
 {
public:
	 Invoke(void (Obj_Type::*inv)(Data_Type&)) : set_(set) {}
	 void end(const std::string);
private:
	 void (Obj_Type::*set_)(Data_Type&);
 };
 */
 

#endif


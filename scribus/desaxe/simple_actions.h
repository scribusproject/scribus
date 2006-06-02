/*
 *  simple_actions.h
 *  
 *
 *  Created by Andreas Vox on 02.06.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */



#ifndef SIMPLE_ACTIONS_H
#define SIMPLE_ACTIONS_H

/*****

    Defines the following actions:

    Factory<D>( fun )                               - -> D
    Factory<D>()                                    - -> D
    Prototype<D>( data )                            - -> D
    Getter<O,D>( fun )                              O -> O D
    Setter<O,D>( fun )                              O D -> O D
    SetAttribute<O,D>( fun, name, default)          O -> O
    SetAttributes<O>( fun )                         O -> O
    SetText<O,D>( fun )                             O -> O
    AddText<O,D>( fun )                             O -> O
    *Transform<D,E> ( fun )                          D -> E
    *TransformLater<D,E> ( fun )                     D -> E
    *Idref<O>()                                      O -> O
    Result<O>()                                     O -> O

*****/


namespace desaxe {

/**
 *   Pushes a new object of type Obj_Type onto the stack.
 *   If no create method is given, Obj_Type needs a default constructor
 */
template<class Obj_Type>
class Factory_body : public Generator_body<Obj_Type>
{
public:
	typedef Obj_Type* (*FunType)();
	
	Factory_body() 
	: create_(NULL) 
    {}
	
	Factory_body(FunType create) 
	: create_(create) 
    {}
	
	void begin(const std::string, std::map<std::string,std::string>)
    { 
		dig->push(create_? create_() : new Obj_Type()); 
    }	
private:
    const Obj_Type* (*create_)();
};

template <class Type>
struct  Factory : public MakeGenerator<Factory_body<Type>, Type, typename Factory_body<Type>::FunType> 
{
	Factory(typename Factory_body<Type>::FunType create)
	: MakeGenerator<Factory_body<Type>, Type, typename Factory_body<Type>::FunType>::MakeGenerator(create) {} 

	Factory()
	: MakeGenerator<Factory_body<Type>, Type, typename Factory_body<Type>::FunType>::MakeGenerator() {} 
};


/**
 *   Pushes a new object of type Obj_Type onto the stack which is initialized with the tag name.
 *   If no create method is given, Obj_Type needs a constructor which taks a std::string argument
 */
template<class Obj_Type>
class FactoryWithName_body : public Generator_body<Obj_Type>
{
public:
	typedef Obj_Type* (*FunType)(std::string);
	
	FactoryWithName_body() 
	: create_(NULL) 
	{}
	
	FactoryWithName_body(FunType create) 
	: create_(create) 
	{}
	
	void begin(const std::string tag, std::map<std::string,std::string>)
	{ 
		dig->push(create_? create_(tag) : new Obj_Type(tag)); 
	}	
private:
	FunType create_;
};

template <class Type>
struct  FactoryWithName : public MakeGenerator<FactoryWithName_body<Type>, Type, typename FactoryWithName_body<Type>::FunType> 
{ 
	FactoryWithName()
	: MakeGenerator<FactoryWithName_body<Type>, Type, typename FactoryWithName_body<Type>::FunType>::MakeGenerator() {} 

	FactoryWithName(typename FactoryWithName_body<Type>::FunType create)
	: MakeGenerator<FactoryWithName_body<Type>, Type, typename FactoryWithName_body<Type>::FunType>::MakeGenerator(create) {} 
};


/**
 *   Pushes a clone of proto onto the stack. Obj_Type needs a copy constructor
 */
template<class Obj_Type>
class Prototype_body : public Generator_body<Obj_Type>
{
public:
	Prototype_body(const Obj_Type& proto) 
	: proto_(new Obj_Type(proto)) 
	{}
	
	~Prototype_body() 
	{ 
		delete proto_; 
	}
	
	void begin(const std::string, std::map<std::string,std::string>)
	{
		dig->push(new Obj_Type(proto));
	}
private:
	const Obj_Type* proto_;
};


template <class Type>
struct  Prototype : public MakeGenerator<Prototype_body<Type>, Type, const Type&> 
{
	Prototype(const Type& def)
	: MakeGenerator<Prototype_body<Type>, Type, const Type&>::MakeGenerator(def) {}
};



/** 
 *  Reads an object of type Data_Type from the object on top of the stack,
 *  then pushes this Data_Type object onto the stack.
 */
template<class Obj_Type, class Data_Type>
class Getter_body : public Generator_body<Data_Type>
{
public:
	typedef const Data_Type& (Obj_Type::*FunType)();
	
	Getter_body(FunType get) 
	: get_(get)
	{}
	
	void begin(const std::string, std::map<std::string,std::string>)
	{
		Obj_Type* obj = dig->template top<Obj_Type>(1);
		Data_Type* data = (obj->*get_)();
		dig->push(data);
	}
private:
	FunType get_;	
};


template <class Type, class Data>
struct  Getter : public MakeGenerator<Getter_body<Type, Data>, Data, typename Getter_body<Type, Data>::FunType> 
{
	Getter(typename Getter_body<Type, Data>::FunType get)
	: MakeGenerator<Getter_body<Type, Data>, Data, typename Getter_body<Type, Data>::FunType>::MakeGenerator(get) {} 
};


/**
 *    Writes the topmost object to the topmost but one object on the stack.
 */
template<class Obj_Type, class Data_Type>
class SetterP_body : public Action_body 
{
public:
	typedef void (Obj_Type::*FunType)(Data_Type*);
	
	SetterP_body(FunType set) 
	: set_(set) 
	{}
	
	void end(const std::string)
	{ 
		Data_Type* data= dig->template top<Data_Type>(); 
		Obj_Type* obj = dig->template top<Obj_Type>(1);
		std::cerr << "setter(ptr): " << obj << " .= " << data << "\n";
		(obj->*set_)( data ); 
	}	
private:
	FunType set_;
};


template <class Type, class Data>
struct  SetterP : public MakeAction<SetterP_body<Type, Data>, typename SetterP_body<Type, Data>::FunType> 
{
	SetterP(typename SetterP_body<Type, Data>::FunType set)
	: MakeAction<SetterP_body<Type, Data>, typename SetterP_body<Type, Data>::FunType>::MakeAction(set) {} 
};


/**
 *  Writes the topmost object to the topmost but one object on the stack.
 */
template<class Obj_Type, class Data_Type>
class Setter_body : public Action_body 
{
public:
	typedef void (Obj_Type::*FunType)(Data_Type);
	
	Setter_body(FunType set) 
	: set_(set) 
	{}
	
	void end(const std::string)
	{ 
		Data_Type* data= dig->template top<Data_Type>(); 
		Obj_Type* obj = dig->template top<Obj_Type>(1);
		std::cerr << "setter: " << obj << " .= *(" << data << ")\n";
		(obj->*set_)( *data ); 
	}	
private:
	FunType set_;
};


template <class Type, class Data>
struct  Setter : public MakeAction<Setter_body<Type, Data>, typename Setter_body<Type, Data>::FunType> 
{
	Setter(typename Setter_body<Type, Data>::FunType set)
	: MakeAction<Setter_body<Type, Data>, typename Setter_body<Type, Data>::FunType>::MakeAction(set) {} 
};


/**
 *   Stores all attributes to the topmost object on the stack.
 */
template<class Obj_Type>
class SetAttributes_body : public Action_body
{
public:
	typedef void (Obj_Type::*FunType)(const std::string&, const std::string&) ;
	
	SetAttributes_body(FunType set) : set_(set) 
	{}
	
	void begin(const std::string, std::map<std::string,std::string> attr)
	{
		Obj_Type* obj = dig->template top<Obj_Type>();
		std::map<std::string,std::string>::iterator it;
		for(it=attr.begin(); it != attr.end(); ++it)
			(obj->*set_)( it->first, it->second ); 
	}	
private:
	FunType set_;
};


template <class Type>
struct  SetAttributes : public MakeAction<SetAttributes_body<Type>, typename SetAttributes_body<Type>::FunType> 
{
	SetAttributes(typename SetAttributes_body<Type>::FunType set)
	: MakeAction<SetAttributes_body<Type>, typename SetAttributes_body<Type>::FunType>::MakeAction(set) {} 
};



/**
 *   Stores text (PCDATA) in the topmost object on the stack.
 *   This might be called more than once.
 */
template<class Obj_Type>
class AddText_body : public Action_body
{
public:
	typedef void (Obj_Type::*FunType)(const std::string&);
	
	AddText_body(FunType add) : addT(add) 
	{}
	
	void chars(const std::string txt)
	{
		Obj_Type* obj = dig->template top<Obj_Type>();
		(obj->*addT)( txt ); 
	}	
private:
	FunType addT;
};



template <class Type>
struct  AddText : public MakeAction<AddText_body<Type>, typename AddText_body<Type>::FunType> 
{
	AddText(typename AddText_body<Type>::FunType set)
	: MakeAction<AddText_body<Type>, typename AddText_body<Type>::FunType>::MakeAction(set) {} 
};




/**
 *   Collects text (PCDATA) chanks and later stores it in the topmost object 
 *   on the stack.
 *   This is only called once at the close tag of the current pattern.
 *   WARNING: not safe for nesting
 */
template<class Obj_Type>
class SetText_body : public Action_body
{
public:
	typedef void (Obj_Type::*FunType)(const std::string&);
	
	SetText_body(FunType set) : setT(set) 
	{}
	
	void begin(const std::string, std::map<std::string, std::string>)
	{
		txt = "";
	}
	
	void chars(const std::string chunk)
	{
		txt += chunk;
	}
	
	void end(const std::string tag)
	{
		Obj_Type* obj = dig->template top<Obj_Type>();
		(obj->*setT)( txt ); 
	}	
	
private:
	FunType setT;
	std::string txt;
};



template <class Type>
struct  SetText : public MakeAction<SetText_body<Type>, typename SetText_body<Type>::FunType> 
{
	SetText(typename SetText_body<Type>::FunType set)
	: MakeAction<SetText_body<Type>, typename SetText_body<Type>::FunType>::MakeAction(set) {} 
};




/**
 *   defines the topmost object as the result
 */
template<class Data_Type>
class Result_body : public Action_body 
{
public:
	Result_body() 
	{}
	
	void end(const std::string)
	{ 
		dig->setResult(dig->template top<Data_Type>());
	}
};



template <class Data>
struct  Result : public MakeAction<Result_body<Data> > {};


} // namespace

#endif

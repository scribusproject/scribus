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

#include "actions.h"
/*****

    Defines the following actions:

    Factory<D>( fun )                               - -> D
    Factory<D>()                                    - -> D
    Prototype<D>( data )                            - -> D
    Top<O>( 3 )                                     O x x x -> O x x x O
    Getter<O,D>( fun )                              O -> O D
    Setter<O,D>( fun )                              O D -> O D
    SetterWithConversion<O,D,S>( fun )              O S -> O S
    SetAttribute<O,D>( fun, name, default)          O -> O
    SetAttribute<O,D>( fun, name)                   O -> O
    SetAttributeWithConversion<O,D>( f, n, c, def)  O -> O
    SetAttributeWithConversion<O,D>( f, n, convert) O -> O
    SetAttributes<O>( fun )                         O -> O
    SetText<O,D>( fun )                             O -> O
    AddText<O,D>( fun )                             O -> O
    Transform<D,E> ( fun )                          D -> E
    *TransformLater<D,E> ( fun )                     D -> E
    IdRef<O>()                                      O -> O
    Lookup<D>( name )                               - -> D
    PatchIdRefAttribute<O,D>( fun, name)            O -> O
    Result<O>()                                     O -> O

*****/


namespace desaxe {

	using namespace desaxe;
	
/**
 *   Pushes a new object of type Obj_Type onto the stack.
 *   Obj_Type needs a default constructor
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
	
	void begin(const Xml_string&, Xml_attr)
    { 
		this->dig->push(create_? create_() : new Obj_Type()); 
    }	
private:
    Obj_Type* (*create_)();
};

template <class Type>
struct  Factory : public MakeGenerator<Factory_body<Type>, Type, typename Factory_body<Type>::FunType> 
{
	Factory(typename Factory_body<Type>::FunType create)
	: MakeGenerator<Factory_body<Type>, Type, typename Factory_body<Type>::FunType>(create) {} 

	Factory()
	: MakeGenerator<Factory_body<Type>, Type, typename Factory_body<Type>::FunType>() {} 
};


/**
 *   Pushes a new object of type Obj_Type onto the stack.
 */
template<class Obj_Type>
class FactoryWithArgs_body : public Generator_body<Obj_Type>
{
public:
	typedef Obj_Type* (*FunType)(const Xml_string&, Xml_attr);
	
	FactoryWithArgs_body(FunType create) 
		: create_(create) 
    {}
	
	void begin(const Xml_string& name, Xml_attr attr)
    { 
		this->dig->push(create_(name, attr)); 
    }	
private:
		Obj_Type* (*create_)(const Xml_string&, Xml_attr);
};

template <class Type>
struct  FactoryWithArgs : public MakeGenerator<FactoryWithArgs_body<Type>, Type, typename FactoryWithArgs_body<Type>::FunType> 
{
	FactoryWithArgs(typename FactoryWithArgs_body<Type>::FunType create)
	: MakeGenerator<FactoryWithArgs_body<Type>, Type, typename FactoryWithArgs_body<Type>::FunType>::MakeGenerator(create) {} 
};


/**
 *   Pushes a new object of type Obj_Type onto the stack which is initialized with the tag name.
 *   If no create method is given, Obj_Type needs a constructor which taks a Xml_string argument
 */
template<class Obj_Type>
class FactoryWithName_body : public Generator_body<Obj_Type>
{
public:
	typedef Obj_Type* (*FunType)(const Xml_string&);
	
	FactoryWithName_body() 
	: create_(NULL) 
	{}
	
	FactoryWithName_body(FunType create) 
	: create_(create) 
	{}
	
	void begin(const Xml_string& tag, Xml_attr)
	{ 
		this->dig->push(create_? create_(tag) : new Obj_Type(tag)); 
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
	
	void begin(const Xml_string&, Xml_attr)
	{
		this->dig->push(new Obj_Type(proto_));
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
 *   Pushes a new pointer to the n-th object on the stack. 
 */
template<class Obj_Type>
class Top_body : public Generator_body<Obj_Type>
{
public:
	Top_body(unsigned int n) 
	: distance(n) 
	{}
		
	void begin(const Xml_string&, Xml_attr)
	{
		this->dig->push(this->dig->template top<Obj_Type>(distance));
	}
private:
	unsigned int distance;
};


template <class Type>
struct  Top : public MakeGenerator<Top_body<Type>, Type, unsigned int> 
{
	Top(unsigned int distance)
	: MakeGenerator<Top_body<Type>, Type, unsigned int>::MakeGenerator(distance) {}
	Top()
	: MakeGenerator<Top_body<Type>, Type, unsigned int>::MakeGenerator(0) {}
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
	
	void begin(const Xml_string&, Xml_attr)
	{
		Obj_Type* obj = this->dig->template top<Obj_Type>(1);
		Data_Type* data = (obj->*get_)();
		this->dig->push(data);
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
 *    Writes the topmost object to the topmost object but one on the stack, where the set method takes a pointer
 */
template<class Obj_Type, class Data_Type, class Store_Type>
class SetterP_body : public Action_body 
{
public:
	typedef void (Obj_Type::*FunType)(Data_Type*);
	
	SetterP_body(FunType set) 
	: set_(set) 
	{}
	
	void end(const Xml_string&)
	{ 
		Store_Type* data = this->dig->template top<Store_Type>(); 
		Obj_Type* obj = this->dig->template top<Obj_Type>(1);
#ifdef DESAXE_DEBUG
		std::cerr << "setter(ptr): " << obj << " .= " << data << "\n";
#endif
		(obj->*set_)( data ); 
	}	
private:
	FunType set_;
};


template <class Type, class Data, class Store = Data>
struct  SetterP : public MakeAction<SetterP_body<Type, Data, Store>, typename SetterP_body<Type, Data, Store>::FunType> 
{
	SetterP(typename SetterP_body<Type, Data, Store>::FunType set)
	: MakeAction<SetterP_body<Type, Data, Store>, typename SetterP_body<Type, Data, Store>::FunType>(set) {} 
};


/**
 *  Writes the topmost object to the topmost object but one on the stack.
 */
template<class Obj_Type, class Data_Type>
class Setter_body : public Action_body 
{
public:
	typedef void (Obj_Type::*FunType)(Data_Type);
	
	Setter_body(FunType set) 
	: set_(set) 
	{}
	
	void end(const Xml_string&)
	{ 
		Data_Type* data = this->dig->template top<Data_Type>(); 
		Obj_Type* obj = this->dig->template top<Obj_Type>(1);
#ifdef DESAXE_DEBUG
		std::cerr << "setter: " << obj << " .= *(" << data << ")\n";
#endif
		(obj->*set_)( *data ); 
	}	
private:
	FunType set_;
};


template <class Type, class Data>
struct  Setter : public MakeAction<Setter_body<Type, Data>, typename Setter_body<Type, Data>::FunType> 
{
	Setter(typename Setter_body<Type, Data>::FunType set)
	: MakeAction<Setter_body<Type, Data>, typename Setter_body<Type, Data>::FunType>(set) {} 
};


/**
*  Writes the topmost object to the topmost object but one on the stack.
 */
template<class Obj_Type, class Data_Type, class Store_Type>
class SetterWithConversion_body : public Action_body 
{
public:
	typedef void (Obj_Type::*FunType)(Data_Type);
	typedef Data_Type (*ConvType)(Store_Type);
	
	SetterWithConversion_body(FunType set, ConvType conv) 
		: set_(set), conv_(conv)
	{}
	
	void end(const Xml_string&)
	{ 
		Store_Type* data = this->dig->template top<Store_Type>(); 
		Obj_Type* obj = this->dig->template top<Obj_Type>(1);
#ifdef DESAXE_DEBUG
		std::cerr << "setter: " << obj << " .= " << conv_ << "(*" << data << ")\n";
#endif
		if (conv_)
			(obj->*set_)( conv_(*data) );
		else
			(obj->*set_)( static_cast<Data_Type>(*data) ); 
	}	
private:
		FunType set_;
	ConvType conv_;
};


template <class Type, class Data, class Store>
struct  SetterWithConversion : public MakeAction<SetterWithConversion_body<Type, Data, Store>, typename SetterWithConversion_body<Type, Data, Store>::FunType, typename SetterWithConversion_body<Type, Data, Store>::ConvType> 
{
	typedef SetterWithConversion_body<Type, Data, Store> BodyType;
	SetterWithConversion(typename SetterWithConversion_body<Type, Data, Store>::FunType set)
	: MakeAction<SetterWithConversion_body<Type, Data, Store>, typename SetterWithConversion_body<Type, Data, Store>::FunType, typename SetterWithConversion_body<Type, Data, Store>::ConvType>(set, NULL) {} 
	SetterWithConversion(typename BodyType::FunType set, typename SetterWithConversion_body<Type, Data, Store>::ConvType conv)
	: MakeAction<SetterWithConversion_body<Type, Data, Store>, typename SetterWithConversion_body<Type, Data, Store>::FunType, typename SetterWithConversion_body<Type, Data, Store>::ConvType>(set, conv) {} 
};


/**
 *   Stores all attributes to the topmost object on the stack.
 */
template<class Obj_Type>
class SetAttributes_body : public Action_body
{
public:
	typedef void (Obj_Type::*FunType)(const Xml_string&, const Xml_string&) ;
	
	SetAttributes_body(FunType set) : set_(set) 
	{}
	
	void begin(const Xml_string&, Xml_attr attr)
	{
		Obj_Type* obj = this->dig->template top<Obj_Type>();
		Xml_attr::iterator it;
		for(it=attr.begin(); it != attr.end(); ++it)
			(obj->*set_)( Xml_key(it), Xml_data(it) ); 
	}	
private:
	FunType set_;
};


template <class Type>
struct  SetAttributes : public MakeAction<SetAttributes_body<Type>, typename SetAttributes_body<Type>::FunType> 
{
	SetAttributes(typename SetAttributes_body<Type>::FunType set)
	: MakeAction<SetAttributes_body<Type>, typename SetAttributes_body<Type>::FunType>(set) {} 
};



/**
 *   Stores named attribute to the topmost object on the stack if attribute present,
 *   or stores defauot if present.
 */
template<class Obj_Type, class Data_Type>
class SetAttribute_body : public Action_body
{
public:
	typedef void (Obj_Type::*FunType)(Data_Type) ;
	
	SetAttribute_body(FunType set, const Xml_string& name) 
		: set_(set), name_(name), default_(), hasDefault_(false)
	{}
	
	SetAttribute_body(FunType set, const Xml_string& name, Data_Type deflt) 
		: set_(set), name_(name), default_(deflt), hasDefault_(true)
	{}
		
	void begin(const Xml_string&, Xml_attr attr)
	{
		Obj_Type* obj = this->dig->template top<Obj_Type>();
		Xml_attr::iterator it = attr.find(name_);
		if (it != attr.end() )
			(obj->*set_)( Data_Type(Xml_data(it)) );
		else if (hasDefault_)
			(obj->*set_)( default_ );			
	}	
private:
	FunType set_;
	Xml_string name_;
	Data_Type default_;
	bool hasDefault_;
};


template <class Type, class Data>
struct  SetAttribute : public MakeAction<SetAttribute_body<Type,Data>, typename SetAttribute_body<Type,Data>::FunType, const Xml_string&, Data> 
{
	SetAttribute(typename SetAttribute_body<Type,Data>::FunType set, const Xml_string& name)
	: MakeAction<SetAttribute_body<Type,Data>, typename SetAttribute_body<Type,Data>::FunType, const Xml_string&, Data>(set,name) {} 

	SetAttribute(typename SetAttribute_body<Type,Data>::FunType set, const Xml_string& name, Data deflt)
	: MakeAction<SetAttribute_body<Type,Data>, typename SetAttribute_body<Type,Data>::FunType, const Xml_string&, Data>(set,name,deflt) {} 
};



/**
*   Stores named attribute to the topmost object on the stack if attribute present,
 *   or stores default if present.
 */
template<class Obj_Type, class Data_Type>
class SetAttributeWithConversion_body : public Action_body
{
public:
	typedef void (Obj_Type::*FunType)(Data_Type) ;
	typedef Data_Type (*ConvType)(const Xml_string&);
	
	SetAttributeWithConversion_body(FunType set, const Xml_string& name, ConvType conv) 
		: set_(set), name_(name), conv_(conv), default_(), hasDefault_(false)
	{}
	
	SetAttributeWithConversion_body(FunType set, const Xml_string& name, ConvType conv, Data_Type deflt) 
		: set_(set), name_(name), conv_(conv), default_(deflt), hasDefault_(true)
	{}
	
	void begin(const Xml_string&, Xml_attr attr)
	{
		Obj_Type* obj = this->dig->template top<Obj_Type>();
		Xml_attr::iterator it = attr.find(name_);
		if (it != attr.end() && conv_)
			(obj->*set_)( conv_(Xml_data(it)) );
		else if (hasDefault_)
			(obj->*set_)( default_ );			
	}	
private:
	FunType set_;
	Xml_string name_;
	ConvType conv_;
	Data_Type default_;
	bool hasDefault_;
};


template <class Type, class Data>
struct  SetAttributeWithConversion : public MakeAction<SetAttributeWithConversion_body<Type,Data>, typename SetAttributeWithConversion_body<Type,Data>::FunType, const Xml_string&, typename SetAttributeWithConversion_body<Type,Data>::ConvType, Data> 
{	
	typedef SetAttributeWithConversion_body<Type,Data> BodyType;
	
	SetAttributeWithConversion(typename BodyType::FunType set, const Xml_string& name, typename BodyType::ConvType conv)
	: MakeAction<BodyType, typename BodyType::FunType, const Xml_string&, typename BodyType::ConvType, Data>(set,name,conv) {} 
	
	SetAttributeWithConversion(typename BodyType::FunType set, const Xml_string& name, typename BodyType::ConvType conv, Data deflt)
	: MakeAction<BodyType, typename BodyType::FunType, const Xml_string&, typename BodyType::ConvType, Data>(set,name,conv,deflt) {} 
};



/**
 *   Stores text (PCDATA) in the topmost object on the stack.
 *   This might be called more than once.
 */
template<class Obj_Type>
class AddText_body : public Action_body
{
public:
	typedef void (Obj_Type::*FunType)(const Xml_string&);
	
	AddText_body(FunType add) : addT(add) 
	{}
	
	void chars(const Xml_string& txt)
	{
		Obj_Type* obj = this->dig->template top<Obj_Type>();
		(obj->*addT)( txt ); 
	}	
private:
	FunType addT;
};



template <class Type>
struct  AddText : public MakeAction<AddText_body<Type>, typename AddText_body<Type>::FunType> 
{
	AddText(typename AddText_body<Type>::FunType set)
	: MakeAction<AddText_body<Type>, typename AddText_body<Type>::FunType>(set) {} 
};




/**
 *   Collects text (PCDATA) chunks and later stores it in the topmost object 
 *   on the stack.
 *   This is only called once at the close tag of the current pattern.
 *   WARNING: not safe for nesting
 */
template<class Obj_Type>
class SetText_body : public Action_body
{
public:
	typedef void (Obj_Type::*FunType)(const Xml_string&);
	
	SetText_body(FunType set) : setT(set) 
	{}
	
	void begin(const Xml_string&, Xml_attr)
	{
		txt = "";
	}
	
	void chars(const Xml_string& chunk)
	{
		txt += chunk;
	}
	
	void end(const Xml_string& tag)
	{
		Obj_Type* obj = this->dig->template top<Obj_Type>();
		(obj->*setT)( txt ); 
	}	
	
private:
	FunType setT;
	Xml_string txt;
};



template <class Type>
struct  SetText : public MakeAction<SetText_body<Type>, typename SetText_body<Type>::FunType> 
{
	typedef SetText_body<Type> BodyType;
	SetText(typename BodyType::FunType set)
	: MakeAction<BodyType, typename BodyType::FunType>(set) {} 
};




template<class Obj_Type>
class Store_body : public Action_body
{
public:
	Store_body(const Xml_string& name) : m_name(name) {}
	
	void begin(const Xml_string& tag, Xml_attr attr)
	{
		Obj_Type* obj = this->dig->template top<Obj_Type>();
//		qDebug() << QString("Store: %1 <- %2").arg(tag).arg(typeid(obj).name());
		this->dig->template store<Obj_Type>(m_name, obj);
	}

private:
	Xml_string m_name;
};


template <class Type>
struct  Store : public MakeAction<Store_body<Type>, const Xml_string& > 
{
	Store(const Xml_string& name) : MakeAction<Store_body<Type>, const Xml_string& >(name) {}
};



/** 
*  This applies "id" and "idref" attributes to the object on top of the stack.
*  In case of an "id" attribute, if there is no entry with this ID in
*  the digester's storage, the topmost object is stored there. Otherwise the
*  topmost object is replaced with the stored object.
*  In case of an "idref" attribute, if there is no entry with this ID in
*  the digester's storage, the topmost object is also stored there. Then the 
*  trigger "WithinIdRef" is set during begin() and the processing continues 
*  normally. When end() is called, the topmost object is replaced by the
*  stored one (this will be a no-op if there wasnt an entry in storage before)
*/
template<class Obj_Type>
class IdRef_body : public Action_body
{
public:
	IdRef_body() : stack() {}
	
	void begin(const Xml_string&, Xml_attr attr)
	{
		Obj_Type* obj = this->dig->template top<Obj_Type>();
		Mode mode;
		Xml_attr::iterator it = attr.find("id");
		if (it != attr.end())
		{
			mode.ID = attr["id"];
			mode.isIdRef = false;
		}
		else {
			Xml_attr::iterator it = attr.find("idref");
			if (it != attr.end())
			{
				mode.ID = attr["idref"];
				mode.isIdRef = true;
			}
			else {
				mode.ID = "";
				mode.isIdRef = false;
			}
		}
		if (mode.ID != "")
		{
			Obj_Type* storedObj = this->dig->template lookup<Obj_Type>(mode.ID);
			if ( !storedObj )
			{
				this->dig->store(mode.ID, obj);
			}
			else if ( !mode.isIdRef )
			{
				delete (this->dig->template top<Obj_Type>());
				this->dig->pop();
				this->dig->push(this->dig->template lookup<Obj_Type>(mode.ID));
			}
			else
			{
				// NYI: set trigger
			}
		}
		stack.push_back(mode);
		
	}
	void end(const Xml_string&)
	{
		Mode mode = stack.back();
		stack.pop_back();
		if (mode.isIdRef)
		{
			delete (this->dig->template top<Obj_Type>());
			this->dig->pop();
			this->dig->push(this->dig->template lookup<Obj_Type>(mode.ID));
			// NYI reset trigger
		}
	}
private:
	struct Mode { Xml_string ID; bool isIdRef; };
	std::vector<Mode> stack;
};


template <class Type>
struct  IdRef : public MakeAction<IdRef_body<Type> > 
{};


/** 
 *  Reads an object of type Data_Type from the digesters
 *  storage and pushes it onto the stack.
 *  WARNING: this might be a NULL pointer
 */
template<class Data_Type>
class Lookup_body : public Generator_body<Data_Type>
{
public:
	Lookup_body(const Xml_string& ID) 
		: ID_(ID)
	{}
	
	void begin(const Xml_string&, Xml_attr)
	{
		Data_Type* data = this->dig->template lookup<Data_Type>(ID_);
		this->dig->push(data);
	}
private:
	Xml_string ID_;	
};


template <class Data>
struct  Lookup : public MakeGenerator<Lookup_body<Data>, const Xml_string&> 
{
	Lookup(Xml_string ID)
	: MakeGenerator<Lookup_body<Data>, const Xml_string&>::MakeGenerator(ID) {} 
};



/**
*  Transforms the topmost object to the topmost object but one on the stack. // FIXME: this should just be a getter...
 */
template<class Obj_Type, class Arg_Type>
class Transform_body : public Action_body 
{
public:
	typedef Obj_Type (*FunType)(const Arg_Type&);
	
	Transform_body(FunType fun) 
		: fun_(fun), stack()
	{}
	
	void begin(const Xml_string&, Xml_attr)
	{ 
		Cell cell;
		cell.arg = this->dig->template top<Arg_Type>(); 
		cell.obj = fun_(*cell.arg);
#ifdef DESAXE_DEBUG
		std::cerr << "transform: " << cell.arg << " -> " << cell.obj << ")\n";
#endif
		stack.push_back(cell);
		this->dig->pop();
		this->dig->push(&cell.obj); 
	}	

	void end(const Xml_string&)
	{
		Cell cell = stack.back();
		stack.pop_back();
		this->dig->pop();
		this->dig->push(cell.arg); 
	}
private:
		FunType fun_;
	struct Cell { Arg_Type* arg; Obj_Type obj; }; 
	std::vector<Cell> stack;
};


template <class Type, class Arg>
struct  Transform : public MakeAction<Transform_body<Type, Arg>, typename Transform_body<Type, Arg>::FunType> 
{
	typedef Transform_body<Type, Arg> BodyType;
	Transform(typename BodyType::FunType f) : MakeAction<BodyType, typename BodyType::FunType>(f) {} 
};




template<class Obj_Type, class Data_Type>
class PatchIdRefAttribute_body : public Action_body
{
public:
	typedef void (Obj_Type::*FunType)(Data_Type*) ;
	
	PatchIdRefAttribute_body(FunType set, const Xml_string& name) 
		: set_(set), name_(name)
	{}
	
	void begin(const Xml_string&, Xml_attr attr)
	{
		Xml_attr::iterator it = attr.find(name_);
		if (it != attr.end())
		{
			Obj_Type* obj = this->dig->template top<Obj_Type>();
			this->dig->template patchInvoke<Obj_Type,Data_Type>(Xml_data(it), obj, set_);
		}
	}	
private:
	FunType set_;
	Xml_string name_;
};


template <class Type, class Data>
struct  PatchIdRefAttribute : public MakeAction<PatchIdRefAttribute_body<Type,Data>, typename PatchIdRefAttribute_body<Type,Data>::FunType, const Xml_string&> 
{
	typedef PatchIdRefAttribute_body<Type,Data> BodyType;
	PatchIdRefAttribute(typename BodyType::FunType set, const Xml_string& name)
	: MakeAction<BodyType, typename BodyType::FunType, const Xml_string&>(set,name) {} 
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
	
	void end(const Xml_string&)
	{ 
		this->dig->setResult(dig->template top<Data_Type>());
	}
};



template <class Data>
struct  Result : public MakeAction<Result_body<Data> > {};


} // namespace

#endif

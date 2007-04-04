/*
 *  saxio.h
 *  
 *
 *  Created by Andreas Vox on 21.09.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */



#ifndef SAXIO_H
#define SAXIO_H

#include "desaxe_conf.h"
#include "saxhandler.h"
//#include "digester.h"

namespace desaxe {
class Digester;
}

/** Interface class for any object which wants to use deSaXe to load from 
    and save to XML.
 */
class SaxIO {
public:
	/**
	    Writes an XML representation of this object
	 */
	virtual void saxx(SaxHandler &, const Xml_string& /* elemtag */) const = 0;
	/**
	    Writes an XML representation of this object using the default element tag, see below
	 */
	virtual void saxx(SaxHandler & ) const = 0;
	virtual ~SaxIO() {}
	/**
		Element tag which is usually used to serialize this object. "" means no elem is created by saxx()
	 */
	static const Xml_string saxxDefaultElem;
	/**
	  Fills the digester with rules wich ensure that an object which was
	  saved via saxx() will be reconstructed on top of the digester stack.
	  The prefix pattern shall be used to create patterns which constrain
	  the added rules to this context.
	  Unfortunately there are no virtual static functions, so this is just a dummy
	  serving as a template.
	 */
	static void desaxeRules(const Xml_string& /* prefixPattern */, desaxe::Digester & /* ruleset */, const Xml_string /* elemtag */ = saxxDefaultElem) {}
};

#endif

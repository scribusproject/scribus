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
#include "digester.h"


/** Interface class for any object which wants to use deSaXe to load from 
    and save to XML.
 */
class SaxIO {
public:
	/**
	  Writes an XML representation of this object
	 */ 
	virtual void saxx(SaxHandler & ) const = 0;
	virtual ~SaxIO() {}
	/**
	  Fills the digester with rules wich ensure that an object which was
	  saved via saxx() will be reconstructed on top of the digester stack.
	  The prefix pattern shall be used to create patterns which constrain
	  the added rules to this context.
	  Unfortunately there are no virtual static functions, so this is just a dummy
	  serving as a template.
	 */
	static void desaxeRules(Xml_string prefixPattern, desaxe::Digester & ruleset) {}
};

#endif

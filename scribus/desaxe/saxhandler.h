/*
 *  saxhandler.h
 *  
 *
 *  Created by Andreas Vox on 21.09.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */



#ifndef SAXHANDLER_H
#define SAXHANDLER_H

#include "desaxe_conf.h"

/**
  Interface class which describes the ability of reading a SAX stream.
  Methods for handling namespaces and encodings might be added later.
 */
class SaxHandler {
public:
	virtual void beginDoc() = 0;
	virtual void endDoc() = 0;
	virtual void begin(Xml_string tag, Xml_attr attr) = 0;
	virtual void end(Xml_string tag) = 0;
	inline  void beginEnd(Xml_string tag, Xml_attr attr);
	virtual void chars(Xml_string text) = 0;
	virtual ~SaxHandler() {}
};

inline  void SaxHandler::beginEnd(Xml_string tag, Xml_attr attr)
{
	begin(tag, attr);
	end(tag);
}
#endif

/*
 *  saxfilter.h
 *  
 *
 *  Created by Andreas Vox on 21.09.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */



#ifndef DESAXE_UNIQUEID_H
#define DESAXE_UNIQUEID_H

#include <set>
#include "saxfilter.h"

/**
 *  This filter suppresses the output of   
 */
class UniqueID : public SaxFilter {
public:
	UniqueID(SaxHandler* delegate) : SaxFilter(delegate), seenIDs(), level(0)  {}
	virtual void begin(const Xml_string& tag, Xml_attr attr);
	virtual void end(const Xml_string& tag);
	virtual void chars(const Xml_string& text);
private:
	std::set<Xml_string> seenIDs;
	int level;
};

#endif

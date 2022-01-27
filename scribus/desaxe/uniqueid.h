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
class UniqueID : public SaxFilter
{
public:
	UniqueID(SaxHandler* delegate) : SaxFilter(delegate) {}
	void begin(const Xml_string& tag, Xml_attr attr) override;
	void end(const Xml_string& tag) override;
	void chars(const Xml_string& text) override;
private:
	std::set<Xml_string> m_seenIDs;
	int m_level {0};
};

#endif

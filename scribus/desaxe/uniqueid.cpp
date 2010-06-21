/*
 *  saxfilter.h
 *  
 *
 *  Created by Andreas Vox on 21.09.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */

#include "uniqueid.h"

void UniqueID::begin(const Xml_string& tag, Xml_attr attr)
{
	if (level > 0)  // skip mode
		++level;
	else 
	{
		Xml_attr::iterator it = attr.find("id");
		if (it != attr.end() && seenIDs.find(Xml_data(it)) != seenIDs.end())
		{
			// enter skip mode
			level = 1;
			// replace with  <tag idref="seenid" />
			Xml_attr idattr;
			idattr["idref"] = Xml_data(it);
			SaxFilter::begin(tag, idattr);
			SaxFilter::end(tag);
		}
		else 
		{
			if (it != attr.end())
				seenIDs.insert(Xml_data(it));
			SaxFilter::begin(tag, attr);
		}
	}
}


void UniqueID::end(const Xml_string& tag)
{
	if (level > 0)  // skip mode
		--level;
	else
		SaxFilter::end(tag);
}

void UniqueID::chars(const Xml_string& text)
{
	if (level == 0)
		SaxFilter::chars(text);
}

/*
 *  saxXML.h
 *  
 *
 *  Created by Andreas Vox on 21.09.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */



#ifndef SAXXML_H
#define SAXXML_H

#include <fstream>
#include "saxhandler.h"
#include "scribusapi.h"

class SCRIBUS_API SaxXML : public SaxHandler {
public:
	SaxXML(std::ostream& file, bool pretty=false);
	SaxXML(const char* filename, bool pretty=false);
	~SaxXML();
	
	void beginDoc();
	void endDoc();
	void begin(const Xml_string& tag, Xml_attr attr);
	void end(const Xml_string& tag);
	void chars(const Xml_string& text);

private:
	std::ofstream m_file;
	std::ostream& m_stream;
	bool m_pretty;
	int m_indentLevel;
	bool m_manyAttributes;
	bool pendingEmptyTag;
	void finalizePendingEmptyTag();
};

#endif

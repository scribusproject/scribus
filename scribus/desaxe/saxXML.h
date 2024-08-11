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

class SCRIBUS_API SaxXML : public SaxHandler
{
	public:
		SaxXML(std::ostream& file, bool pretty = false);
		SaxXML(const char* filename, bool pretty = false);
		~SaxXML() override;

		void beginDoc() override;
		void endDoc() override;
		void begin(const Xml_string& tag, Xml_attr attr) override;
		void end(const Xml_string& tag) override;
		void chars(const Xml_string& text) override;

	private:
		std::ofstream m_file;
		std::ostream& m_stream;
		bool m_pretty { false };
		int m_indentLevel { 0 };
		bool m_manyAttributes { false };
		bool m_pendingEmptyTag { false };
		void finalizePendingEmptyTag();
};

#endif

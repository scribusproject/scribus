/*
 *  digester_parse.h
 *  
 *
 *  Created by Andreas Vox on 02.06.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */


#include <qxml.h>
#include <qfile.h>
#include <qstring.h>

#include "digester.h"

namespace desaxe {
	
class DigesterParser : public QXmlDefaultHandler
{
public:
	DigesterParser(Digester* digester) : dig(digester) {}
	bool startDocument()
	{
		dig->reset();
		return true;
	}
    bool startElement( const QString& nsURI, const QString& locName, const QString& qName,
                       const QXmlAttributes& qattr)
	{
		std::map<std::string,std::string> attr;
		for (int i=0; i < qattr.count(); ++i)
			attr[qattr.qName(i)] = std::string(qattr.value(i).utf8());
		dig->begin(qName, attr);
		return true;
	}

    bool endElement( const QString& nsURI, const QString& locName, const QString& qName)
	{
		dig->end(qName);
		return true;
	}

	bool characters ( const QString & ch ) 
	{
		dig->chars(ch);
		return true;
	}
private:
	Digester * dig;
};

void Digester::parseFile(const std::string filename)
{
	DigesterParser* handler = new DigesterParser(this);
	QFile xmlFile( filename );
	QXmlInputSource source( &xmlFile );
	QXmlSimpleReader reader;
	reader.setContentHandler( handler );
	reader.parse( source );
}

void Digester::parseMemory(const char* data, unsigned int length)
{
	assert(false);
}

} // namespace


/*
 *  desaxe_test.h
 *  
 *
 *  Created by Andreas Vox on 02.06.06.
 *  This example file is in the public domain.
 *
 */

#include <iostream.h>
#include <QFile>
#include <QFileInfo>
#include <QDomDocument>
#include <QByteArray>

#include "digester.h"
#include "actions.h"
#include "simple_actions.h"

class ELEM {
public:
	ELEM(const Xml_string& n) : name_(n), attributes(), children(), text() {}
	
	void setName(const Xml_string& n) 
	{ 
		name_ = n; 
	}
	
	const Xml_string& name() 
	{ 
		return name_; 
	}
	
	void setAttribute(const Xml_string& key, const Xml_string& val)
	{
		attributes[key] = val;
	}
	
	void addElem(ELEM* child)
	{
//		cerr << "adding child: " << child << "\n";
		children.push_back(child);
	}
		
	void addText(const Xml_string& txt)
	{
		text += txt;
	}

	void print()
	{
		std::cout << "<" << name_ << " ";
		Xml_attr::iterator it;
		for (it=attributes.begin(); it != attributes.end(); ++it)
			std::cout << Xml_key(it) << "=\"" << Xml_data(it) << "\" ";
		std::cout << ">\n" << text << "\n";
		
		for (uint i=0; i < children.size(); ++i)
			children[i]->print();
		std::cout << "</" << name_ << ">\n";
	}

private:
	Xml_string name_;
	Xml_attr attributes;
	std::vector<ELEM*> children;
	Xml_string text;
};


void dummyDoc(desaxe::Digester& dig)
{
	dig.reset();
	
	Xml_attr attr;
	dig.begin("doc", attr);
	attr["level"] = "1";
	dig.chars("aha.");
	dig.begin("elem", attr);
	attr["level"] = "2";
	dig.chars("soso.");
	dig.begin("elem", attr);
	attr["level"] = "3";
	dig.chars("alpha ");
	dig.end("elem");
	dig.chars("bravo ");
	dig.end("elem");
	dig.chars("charly ");
	attr["level"] = "2";
	attr["test"] = "bingo";
	dig.begin("elem", attr);
	dig.chars("delta ");
			 attr["level"] = "3";
	         dig.begin("elem", attr);
	            dig.chars("echo ");
				dig.end("elem");
				dig.chars("foxtrott ");	
				dig.end("elem");
				attr["test"] = "naja";
				dig.chars("golf ");
				attr["level"] = "2";
				dig.begin("elem", attr);
				dig.chars("hotel ");
				attr["level"] = "3";
				dig.begin("elem", attr);
				dig.chars("india ");
				dig.end("elem");
				dig.chars("juliet ");
				dig.end("elem");
				dig.chars("kilo ");
				dig.end("doc");
}				

bool loadRawText(const QString & filename, QByteArray & buf)
{
	bool ret = false;
	QFile f(filename);
	QFileInfo fi(f);
	if (fi.exists())
	{
		QByteArray tempBuf(f.size() + 1);
		if (f.open(QIODevice::ReadOnly))
		{
			unsigned int bytesRead = f.readBlock(tempBuf.data(), f.size());
			tempBuf[bytesRead] = '\0';
			ret = bytesRead == f.size();
			if (ret)
				buf = tempBuf; // sharing makes this efficient
		}
	}
	if (f.isOpen())
		f.close();
	return ret;
}


int main(int argc, char** argv)
{
	using namespace desaxe;
	
	Digester dig;
	Action fac = FactoryWithName<ELEM>();
	Action s_attr = SetAttributes<ELEM>( & ELEM::setAttribute );
	Action res = Result<ELEM>();
	Action set  = SetterP<ELEM, ELEM>( & ELEM::addElem );
	Action txt = AddText<ELEM>( & ELEM::addText );
	
	dig.addRule("*",fac);
	dig.addRule("*",s_attr);
	dig.addRule("*",txt);
	dig.addRule("/*",res);
	dig.addRule("*/*", set);
	
	if (argc < 2) {
		dummyDoc(dig);
		dig.result<ELEM>()->print();
	}
	else {
		const char* file = argc == 2? "Dokument-1.xml" : argv[2];
		switch (QString(argv[1]).toInt()) {
			case 1:
				dig.parseFile(file);
				break;
			case 2:
			{
				QByteArray docBytes("");
				loadRawText(file, docBytes);
				QDomDocument docu("scridoc");
				return docu.setContent(QString(docBytes));
			}
			default:
				dig.parseFile(file);
				dig.result<ELEM>()->print();
		}
	}	
}
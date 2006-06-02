/*
 *  desaxe_test.h
 *  
 *
 *  Created by Andreas Vox on 02.06.06.
 *  This example file is in the public domain.
 *
 */

#include <iostream.h>

#include "digester.h"
#include "actions.h"
#include "simple_actions.h"

class ELEM {
public:
	ELEM(const std::string& n) : name_(n), attributes(), children(), text() {}
	
	void setName(const std::string& n) 
	{ 
		name_ = n; 
	}
	
	const std::string& name() 
	{ 
		return name_; 
	}
	
	void setAttribute(const std::string& key, const std::string& val)
	{
		attributes[key] = val;
	}
	
	void addElem(ELEM* child)
	{
		cerr << "adding child: " << child << "\n";
		children.push_back(child);
	}
		
	void addText(const std::string& txt)
	{
		text += txt;
	}

	void print()
	{
		std::cout << "<" << name_ << " ";
		std::map<std::string,std::string>::iterator it;
		for (it=attributes.begin(); it != attributes.end(); ++it)
			std::cout << it->first << "=\"" << it->second << "\" ";
		std::cout << ">\n" << text << "\n";
		
		for (uint i=0; i < children.size(); ++i)
			children[i]->print();
		std::cout << "</" << name_ << ">\n";
	}

private:
	std::string name_;
	std::map<std::string,std::string> attributes;
	std::vector<ELEM*> children;
	std::string text;
};



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
	
	dig.reset();
	
	std::map<std::string, std::string> attr;
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
	dig.result<ELEM>()->print();
	
	dig.parseFile("Dokument-1.xml");
	dig.result<ELEM>()->print();
	
}
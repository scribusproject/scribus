/*
 *  saxfilter.h
 *  
 *
 *  Created by Andreas Vox on 21.09.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */



#ifndef SAXFILTER_H
#define SAXFILTER_H

#include "saxhandler.h"

/**
  Abstract super class for filtering a SAX stream.  
 */
class SaxFilter : public SaxHandler {
public:
	SaxFilter(SaxHandler* delegate)        : m_delegate(delegate)  {}
	void setDelegate(SaxHandler* delegate) { m_delegate = delegate; }
	SaxHandler* delegate()                 { return m_delegate; }
	void beginDoc() override;
	void endDoc() override;
	void begin(const Xml_string& tag, Xml_attr attr) override;
	void end(const Xml_string& tag) override;
	void chars(const Xml_string& text) override;

private:
	SaxHandler* m_delegate { nullptr };
};


/**
 *  This filter will suppress the closing tag for the
 *  next opening tag. This allows to add content to
 *  the element, eg:
 *     KeepOpen keepOpenHandler( &myhandler );
 *     myElem.saxx(keepOpenHandler);
 *     more_output (myhandler);
 *     myhandler.end(keepOpenhandler.opentag());
 */
class KeepOpen : public SaxFilter
{
public:
	KeepOpen(SaxHandler* delegate);
	void begin(const Xml_string& tag, Xml_attr attr) override;
	void end(const Xml_string& tag) override;
	Xml_string openTag();

private:
	int m_level {0};
	Xml_string m_tag;
};


/**
 *  Renames the next opening(and corresponding closing) tag.
 *  If oldname is not empty, it will also rename all inner tags with name oldname
 */
class RenameElem : public SaxFilter
{
public:
	RenameElem(SaxHandler* delegate, const Xml_string& oldname, const Xml_string& newname);
	RenameElem(SaxHandler* delegate, const Xml_string& newname);
	void begin(const Xml_string& tag, Xml_attr attr) override;
	void end(const Xml_string& tag) override;

private:
	int m_level {0};
	Xml_string m_old;
	Xml_string m_new;
};


/**
 *  Adds more attributes to the next opening tag
 */
class AddAttributes : public SaxFilter {
public:
	AddAttributes(SaxHandler* delegate, Xml_attr add);
	void begin(const Xml_string& tag, Xml_attr attr) override;

private:
	bool m_oneshot {false};
	Xml_attr m_attributes;
};


/**
 *  Can be used to suppress output to a filter, eg.
 *     SaxHandlerNull nullhandler();
 *     myfilter.setDelegate( & nullhandler );
 *     ... (continue parsing)
 */
class SaxHandlerNull : public SaxHandler {
public:
	SaxHandlerNull() {}
	void beginDoc() override {}
	void endDoc() override {}
	void begin(const Xml_string& tag, Xml_attr attr) override {}
	void end(const Xml_string& tag) override {}
	void chars(const Xml_string& text) override {}
};


#endif

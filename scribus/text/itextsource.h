/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#ifndef ITEXTSOURCE_H
#define ITEXTSOURCE_H

#include "scribusapi.h"
#include "sctextstruct.h"

class CharStyle;
class ParagraphStyle;
class InlineFrame;
class GlyphCluster;
class ScribusDoc;


class SCRIBUS_API ITextSource {

public:

	virtual int length() const = 0; 
	virtual QChar text(int pos) const = 0;
	virtual QString text(int pos, uint len) const = 0;
	
	/// Checks if it's the start of a paragraph or "block"
	virtual bool isBlockStart(int pos) const = 0;
	
	/// Returns a blockstart position > pos.
	/// You may lie here, i.e. lump blocks together or just return the end of the text
	virtual int nextBlockStart(int pos) const = 0;
	
	virtual const CharStyle& charStyle(int pos) const = 0; 
	virtual const ParagraphStyle& paragraphStyle(int pos) const = 0;

	virtual void setCharStyle(int pos, uint len, const CharStyle& style) = 0;
	virtual void setStyle(int pos, const ParagraphStyle& style) = 0;

	virtual LayoutFlags flags(int pos) const = 0;
	virtual bool hasFlag(int pos, LayoutFlags flag) const = 0;
	virtual void setFlag(int pos, LayoutFlags flag) = 0;
	virtual void clearFlag(int pos, LayoutFlags flag) = 0;

	virtual bool hasObject(int pos) const = 0;
	virtual InlineFrame object(int pos) const = 0;
	virtual bool hasExpansionPoint(int pos) const = 0;
	virtual ExpansionPoint expansionPoint(int pos) const = 0;
	
	virtual const ITextSource* parent() const { return NULL; }
	virtual int parentPos() const { return 0; }
	
	const ITextSource* original() const
    {
		return parent() == NULL? this : parent()->original();
	}
	
	const int originalStartPos() const
	{
		return parent() == NULL? 0 : parent()->parent() == NULL? parentPos() : parent()->originalStartPos();
    }
};


#endif // ITEXTSOURCE_H

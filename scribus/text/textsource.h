/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#ifndef TEXTSOURCE_H
#define TEXTSOURCE_H

#include <QString>

#include "scribusapi.h"
#include "itextsource.h"
#include "styles/paragraphstyle.h"


class CharStyle;
class ParagraphStyle;
class InlineFrame;


/**
 * provide a segment of an original textsource
 */
class SCRIBUS_API TextSourceSegment : public ITextSource
{
	ITextSource* m_source;
	int m_sourceOffset;
	int m_segmentLength;
	
	void chk(int pos);
	int toSrc(int pos);
	
public:
	
	TextSourceSegment(ITextSource* original, int offset, int segmentLength);
	
	bool canCombine(const TextSourceSegment& other) const;
	void combine(const TextSourceSegment& other);
	
	virtual int length() const;
	virtual QChar text(int pos) const;
	virtual QString text(int pos, uint len) const;
	
	virtual bool isBlockStart(int pos) const;
	virtual int nextBlockStart(int pos) const;
	
	virtual const CharStyle& charStyle(int pos) const;
	virtual const ParagraphStyle& paragraphStyle(int pos) const;
	virtual LayoutFlags flags(int pos) const;
	virtual bool hasFlag(int pos, LayoutFlags flag) const;
	virtual void setFlag(int pos, LayoutFlags flag);
	virtual void clearFlag(int pos, LayoutFlags flag);
	
	virtual bool hasObject(int pos) const;
	virtual InlineFrame object(int pos) const;
	virtual bool hasExpansionPoint(int pos) const;
	virtual ExpansionPoint expansionPoint(int pos) const;
	
	virtual const ITextSource* parent() const;
	virtual int parentPos() const;
};



/**
 * provide a simple text source with fixed style, optionally linking it to an expansion point
 */
class SCRIBUS_API SimpleTextSource : public ITextSource
{
	QString m_text;
	ParagraphStyle m_style;
	
	ITextSource* m_parent;
	int m_parentPos;
	
public:
	
	SimpleTextSource(QString text, const ParagraphStyle& style, ITextSource* par=NULL, int parentStart = 0) : m_text(text), m_style(style), m_parent(par), m_parentPos(parentStart) {}
	
	virtual int length() const;
	virtual QChar text(int pos) const;
	virtual QString text(int pos, uint len) const;
	
	virtual bool isBlockStart(int pos) const;
	virtual int nextBlockStart(int pos) const;
	
	virtual const CharStyle& charStyle(int pos) const;
	virtual const ParagraphStyle& paragraphStyle(int pos) const;
	virtual LayoutFlags flags(int pos) const;
	virtual bool hasFlag(int pos, LayoutFlags flag) const;
	virtual void setFlag(int pos, LayoutFlags flag);
	virtual void clearFlag(int pos, LayoutFlags flag);
	
	virtual bool hasObject(int pos) const;
	virtual InlineFrame object(int pos) const;
	virtual bool hasExpansionPoint(int pos) const;
	virtual ExpansionPoint expansionPoint(int pos) const;
	
	virtual const ITextSource* parent() const;
	virtual int parentPos() const;
};

#endif // ITEXTSOURCE_H


/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include "textsource.h"



// TextSourceSegment members

TextSourceSegment::TextSourceSegment(ITextSpource* original, int offset, int segmentLength)
: m_source(original), m_sourceOffset(offset), m_segmentLength(segmentLength)
{
}


bool canCombine(const TextSourceSegment& other) const
{
	return m_source == other.m_source && m_sourceOffset + m_segmentLength == other.m_sourceOffset;
}


TextSourceSegment combine(const TextSourceSegment& other);
{
	if (canCombine(other))
		return TextSourceSegment(m_source, qMin(m_sourceOffset, other.m_sourceOffset), m_segmentLength + other.m_segmentLength);
	else
		return TextSourceSegment(m_source, -1, 0);
}


void chk(int pos)
{
	assert (pos >= 0);
	assert (pos < m_segmentLength) ;
}


int TextSourceSegment toSrc(int pos)
{
	chk(pos);
	return pos + m_sourceOffset;
}


int TextSourceSegment::length() const
{
	return m_segmentLength;
}


QChar TextSourceSegment::text(int pos) const
{
	return m_source->text(toSrc(pos));
}


QString TextSourceSegment::text(int pos, uint len) const
{
	chk(pos + len-1);
	return m_source->text(toSrc(pos), len);
}


int TextSourceSegment::nextBlockStart(int pos) const
{
	int result = m_source->nextBlockStart(toSrc(pos));
	return qMin(result - m_sourceOffset, m_segmentLength);
}


// the rest is just boring stuff of the same:

bool TextSourceSegment::isBlockStart(int pos) const { return m_source->isBlockStart(toSrc(pos)); }
const CharStyle& TextSourceSegment::charStyle(int pos) const { return m_source->charStyle(toSrc(pos)); }
const ParagraphStyle& TextSourceSegment::paragraphStyle(int pos) const { return m_source->paragraphStyle(toSrc(pos)); }
LayoutFlags TextSourceSegment::flags(int pos) const { return m_source->flags(toSrc(pos)); }
bool TextSourceSegment::hasFlag(int pos, LayoutFlags flag) const { return m_source->hasFlag(toSrc(pos)); }
void TextSourceSegment::setFlag(int pos, LayoutFlags flag) { return m_source->setFlag(toSrc(pos)); }
void TextSourceSegment::clearFlag(int pos, LayoutFlags flag) { return m_source->clearFlag(toSrc(pos)); }
bool TextSourceSegment::hasObject(int pos) const{ return m_source->hasObject(toSrc(pos)); }
InlineFrame TextSourceSegment::object(int pos) const { return m_source->object(toSrc(pos)); }
bool TextSourceSegment::hasExpansionPoint(int pos) const { return m_source->hasExpansionPoint(toSrc(pos)); }
ExpansionPoint TextSourceSegment::expansionPoint(int pos) const { return m_source->expansionPoint(toSrc(pos)); }
const ITextSource* TextSourceSegment::parent() const { return m_source->parent(); }
int TextSourceSegment::parentPos() const { return m_source->parentPos() };



// SimpleTextSource members

int SimpleTextSource::length() const
{
	return m_text.length();
}


QChar SimpleTextSource::text(int pos) const
{
	return m_text.at(pos);
}


QString SimpleTextSource::text(int pos, uint len) const
{
	assert (pos + len <= length() );
	return m_text.substring(pos, len);
}


bool SimpleTextSource::isBlockStart(int pos) const
{
	return pos == 0;
}


int SimpleTextSource::nextBlockStart(int pos) const
{
	return length();
}


const CharStyle& SimpleTextSource::charStyle(int pos) const
{
	return m_style.charStyle();
}


const ParagraphStyle& SimpleTextSource::paragraphStyle(int pos) const
{
	return m_style();
}


LayoutFlags SimpleTextSource::flags(int pos) const
{
	return LayoutFlags_None;
}


bool SimpleTextSource::hasFlag(int pos, LayoutFlags flag) const
{
	return false;
}


void SimpleTextSource::setFlag(int pos, LayoutFlags flag)
{
}


void SimpleTextSource::clearFlag(int pos, LayoutFlags flag)
{
}


bool SimpleTextSource::hasObject(int pos) const
{
	return false;
}


InlineFrame SimpleTextSource::object(int pos) const
{
	return InlineFrame();
}


bool SimpleTextSource::hasExpansionPoint(int pos) const
{
	return false;
}


ExpansionPoint SimpleTextSource::expansionPoint(int pos) const
{
	return ExpansionPoint();
}


const ITextSource* SimpleTextSource::parent() const
{
	return m_parent;
}


int SimpleTextSource::parentPos() const
{
	return m_parentPos;
}

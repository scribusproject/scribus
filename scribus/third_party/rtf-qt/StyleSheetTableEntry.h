/*
    Copyright (C)  2010  Brad Hards <bradh@frogmouth.net>

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RTFREADER_STYLESHEETTABLEENTRY_H
#define RTFREADER_STYLESHEETTABLEENTRY_H
namespace RtfReader
{
    enum TextAlignment {
	LeftAligned,
	CentreAligned,
	RightAligned,
	Justified,
	Distributed };
	
	class StyleSheetTableEntry
    {
      public:
	StyleSheetTableEntry() : m_textAlignment( LeftAligned ),
	  m_leftIndent( 0 ), m_rightIndent( 0 ),
	  m_layoutDirection( Qt::LeftToRight )
	{}

	enum TextAlignment textAlignment() const
	{ return m_textAlignment; }

	void setTextAlignment( enum TextAlignment textAlignment )
	{ m_textAlignment = textAlignment; }

	int leftIndent() const
	{ return m_leftIndent; }

	void setLeftIndent( const int leftIndent )
	{ m_leftIndent = leftIndent; }

	int rightIndent() const
	{ return m_rightIndent; }

	void setRightIndent( const int rightIndent )
	{ m_rightIndent = rightIndent; }

	int topMargin() const
	{ return m_topMargin; }

	void setTopMargin( const int topMargin )
	{ m_topMargin = topMargin; }

	Qt::LayoutDirection layoutDirection() const
	{ return m_layoutDirection; }

	void setLayoutDirection( const Qt::LayoutDirection layoutDirection )
	{ m_layoutDirection = layoutDirection; }

	QString styleName() const
	{ return m_styleName; }

	void setStyleName( const QString &styleName )
	{ m_styleName = styleName; }

      protected:
	QString			m_styleName;
	enum TextAlignment 	m_textAlignment;
	int 			m_leftIndent;
	int			m_rightIndent;
	Qt::LayoutDirection	m_layoutDirection;
	int                     m_topMargin;
    };
}

#endif

/*
    Copyright (C)  2008, 2010  Brad Hards <bradh@frogmouth.net>

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

#ifndef RTFREADER_RTFOUTPUTSTRATEGY_H
#define RTFREADER_RTFOUTPUTSTRATEGY_H

#include <QDateTime>
#include <QString>
#include <QVariant>
#include <QColor>

#include "FontTableEntry.h"
class ParagraphStyle;
/**
   Namespace for the public interface for the RtfReader classes
*/
namespace RtfReader
{
    /**
	Abstract output strategy for RTF Parser
    */
	class  AbstractRtfOutput
    {
      public:
	AbstractRtfOutput();
	virtual ~AbstractRtfOutput();

	/**
	  The author of this RTF file

	  \note This may not be valid - applications may not set this,
	  or may not update it.
	*/
	virtual QString author() const;
	virtual void setAuthor( const QString &author );

	/**
	  The company of the author of this RTF file

	  \note This may not be valid - applications may not set this,
	  or may not update it.
	*/
	virtual QString companyName() const;
	virtual void setCompanyName( const QString &companyName );

	/**
	  The title of this RTF file

	  \note This may not be valid - applications may not set this,
	  or may not update it.
	*/
	virtual QString title() const;
	virtual void setTitle( const QString &title );

	/**
	  The subject of this RTF file

	  \note This may not be valid - applications may not set this,
	  or may not update it.
	*/
	virtual QString subject() const;
	virtual void setSubject( const QString &subject );

	/**
	  The last person make changes to this RTF file

	  \note This may not be valid - applications may not set this,
	  or may not update it.
	*/
	virtual QString operatorName() const;
	virtual void setOperatorName( const QString &operatorName );

	/**
	  The manager(s) of the author of this RTF file

	  \note This may not be valid - applications may not set this,
	  or may not update it.
	*/
	virtual QString managerName() const;
	virtual void setManagerName( const QString &managerName );

	/**
	  The comment associated with this RTF file

	  This is often something related to the application that wrote the file

	  \note This may not be valid - applications may not set this,
	  or may not update it.

	  \sa documentComment
	*/
	virtual QString comment() const;
	virtual void setComment( const QString &comment );

	/**
	  The document comment associated with this RTF file

	  This is often from the "Summary Info" or "Properties" dialog box

	  \note This may not be valid - applications may not set this,
	  or may not update it.
	*/
	virtual QString documentComment() const;
	virtual void setDocumentComment( const QString &documentComment );

	/**
	  The keywords associated with this RTF file

	  \note This may not be valid - applications may not set this,
	  or may not update it.
	*/
	virtual QString keywords() const;
	virtual void setKeywords( const QString &keywords );

	/**
	  The category information associated with this RTF file

	  \note This may not be valid - applications may not set this,
	  or may not update it.
	*/
	virtual QString category() const;
	virtual void setCategory( const QString &category );

	/**
	  The base address for all relative hyperlinks inserted in the document.

	  This can be a path or a URL

	  \note This may not be valid - applications may not set this,
	  or may not update it.
	*/
	virtual QString hLinkBase() const;
	virtual void setHLinkBase( const QString &hLinkBase );

	/**
	  The generator of this RTF file

	  This is usually something like the name of the program and the version of
	  that program.

	  \note This may not be valid - applications may not set this,
	  or may not update it.
	*/
	virtual QString generatorInformation() const;
	virtual void setGeneratorInformation( const QString &generatorInformation );

	/**
	  When this RTF file was created

	  \note This may not be valid - applications may not set this,
	  or time may not be set, or it may be set to something silly.
	*/
	virtual QDateTime created() const;
	virtual void setCreatedDateTime( const QDateTime &dateTime );

	/**
	  When this RTF file was revised / edited

	  \note This may not be valid - applications may not set this,
	  or time may not be set, or it may be set to something silly.
	*/
	virtual QDateTime revised() const;
	virtual void setRevisedDateTime( const QDateTime &dateTime );

	/**
	  When this RTF file was printed

	  \note This may not be valid - applications may not set this,
	  or time may not be set, or it may be set to something silly.
	*/
	virtual QDateTime printed() const;
	virtual void setPrintedDateTime( const QDateTime &dateTime );

	/**
	  The total editing time for this file (in minutes)

	  \note This may not be valid - applications may not set this,
	  or it may be set to something silly.
	*/
	virtual int totalEditingTime() const;
	virtual void setTotalEditingTime( const int totalEditingTime );

	/**
	  The number of pages in this document (from the document metadata)

	  \note This may not be valid - applications may not set this,
	  or it may be set to something silly.
	*/
	virtual int numberOfPages() const;
	virtual void setNumberOfPages( const int numberOfPages );

	/**
	  The number of words in this document (from the document metadata)

	  \note This may not be valid - applications may not set this,
	  or it may be set to something silly.
	*/
	virtual int numberOfWords() const;
	virtual void setNumberOfWords( const int numberOfWords );

	/**
	  The number of characters in this document (from the document metadata)

	  \note This may not be valid - applications may not set this,
	  or it may be set to something silly.
	*/
	virtual int numberOfCharacters() const;
	virtual void setNumberOfCharacters( const int numberOfCharacters );

	/**
	  The number of characters (not including spaces) in this document
	  (from the document metadata)

	  \note This may not be valid - applications may not set this,
	  or it may be set to something silly.
	*/
	virtual int numberOfCharactersWithoutSpaces() const;
	virtual void setNumberOfCharactersWithoutSpaces( const int numberOfCharactersWithoutSpaces );

	/**
	  The version number for this document (from the document metadata)

	  \note This may not be valid - applications may not set this,
	  or it may be set to something silly.
	*/
	virtual int versionNumber() const;
	virtual void setVersionNumber( const int versionNumber );

	/**
	  The version number for this document (from the document metadata)

	  \note This may not be valid - applications may not set this,
	  or it may be set to something silly.
	*/
	virtual int internalVersionNumber() const;
	virtual void setInternalVersionNumber( const int internalVersionNumber );

	QVariant userProp( const QString &propertyName ) const;
	QList<QString> userPropNames() const;
	void addUserProp( const QString &propertyName, const QVariant &propertyValue );

	// TODO: consider adding default implementation for this
	virtual void setEncoding(const int enc) = 0;
	virtual void appendText( const QByteArray &text ) = 0;
	virtual void appendUnicodeText( const QString &text ) = 0;
	virtual void insertPar() = 0;
	virtual void insertTab() = 0;
	virtual void insertLeftQuote() = 0;
	virtual void insertRightQuote() = 0;
	virtual void insertLeftDoubleQuote() = 0;
	virtual void insertRightDoubleQuote() = 0;
	virtual void insertEnDash() = 0;
	virtual void insertEmDash() = 0;
	virtual void insertEnSpace() = 0;
	virtual void insertEmSpace() = 0;
	virtual void insertBullet() = 0;
	virtual void insertNewLine() = 0;
	virtual void resetParagraphFormat() = 0;
	virtual void setParagraphAlignmentLeft() = 0;
	virtual void setParagraphAlignmentCentred() = 0;
	virtual void setParagraphAlignmentJustified() = 0;
	virtual void setParagraphAlignmentRight() = 0;
	virtual void setFirstLineIndent( const int twips ) = 0;
	virtual void setLeftIndent( const int twips ) = 0;
	virtual void setRightIndent( const int twips ) = 0;
	virtual void setFontItalic( const int value ) = 0;
	virtual void setFontBold( const int value ) = 0;
	virtual void setFontUnderline( const int value ) = 0;
	virtual void setFontWordUnderline( const int value ) = 0;
	virtual void setFontOutline( const int value ) = 0;
	virtual void setFontShadow(const int value) = 0;
	virtual void setFontSmallCaps(const int value) = 0;
	virtual void setFontCaps(const int value) = 0;
	virtual void setFontStrikeOut(const int value) = 0;
	virtual void setFontScaleH(const int value) = 0;
	virtual void setFontOffset(const int value) = 0;
	virtual void setFontStretch(const int value) = 0;
	virtual void setFontStretchTw(const int value) = 0;
	virtual void setFontPointSize( const int value ) = 0;
	virtual void setFontLineSpacing(const int value) = 0;
	virtual void setFontSuperscript() = 0;
	virtual void setFontSubscript() = 0;
	virtual void setForegroundColour( const int value ) = 0;
	virtual void setHighlightColour( const int value ) = 0;
	virtual void setParagraphPatternBackgroundColour( const int value ) = 0;
	virtual void setCharacterPatternBackgroundColour( const int value ) = 0;
	virtual void setFont( const int fontIndex ) = 0;
	virtual void setDefaultFont( const int fontIndex ) = 0;
	virtual void setTextDirectionLeftToRight() = 0;
	virtual void setTextDirectionRightToLeft() = 0;
	virtual void resetCharacterProperties() = 0;
	virtual void createImage( const QByteArray &image, int width, int height, int type) = 0;
	virtual void setPageHeight( const int pageHeight ) = 0;
	virtual void setPageWidth( const int pageWidth ) = 0;
	virtual void setSpaceBefore( const int value ) = 0;
	virtual void setSpaceAfter( const int value ) = 0;
	virtual void keepWithNext() = 0;
	virtual void setDropCapsLines(const int value) = 0;
	virtual void setDropCaps() = 0;
	virtual void addTabStop( const int value, const int type ) = 0;
	virtual void appendToColourTable( const QColor &colour ) = 0;
	virtual void insertFontTableEntry( FontTableEntry fontTableEntry, quint32 fontTableIndex ) = 0;
	virtual void insertStyleSheetTableEntry( quint32 stylesheetTableIndex, const ParagraphStyle& stylesheetTableEntry ) = 0;
	virtual void resolveStyleSheetParents(QHash<quint32, int> &parentSet) = 0;
	virtual void useStyleSheetTableEntry(const int styleIndex) = 0;

	virtual void startGroup();
	virtual void endGroup();
	virtual QTextCodec *getCurrentCodec() { return 0; }

      private:
	// The author of the document (from document meta-data, if any)
	QString m_author;

	// The company of the author of the document (from document meta-data, if any)
	QString m_companyName;

	// The operator of the document (from document meta-data, if any)
	QString m_operatorName;

	// The title of the document (from document meta-data, if any)
	QString m_title;

	// The subject of the document (from document meta-data, if any)
	QString m_subject;

	// The category (or categories) of the document (from document meta-data, if any)
	QString m_category;

	// The manager(s) of the author of the document (from document meta-data, if any)
	QString m_managerName;

	// The comment associated with the document (from document meta-data, if any)
	QString m_comment;

	// The document comment (from document meta-data, if any)
	QString m_documentComment;

	// The keywords associated with the document (from document meta-data, if any)
	QString m_keywords;

	// The hyperlink base associated with this document (path or URL)
	QString m_hlinkbase;

	// The date and time this document was created (from document meta-data, if any)
	QDateTime m_created;

	// The date and time this document was revised (from document meta-data, if any)
	QDateTime m_revised;

	// The generator of this document (from document meta-data, if any)
	QString m_generatorInformation;

	// The date and time this document was printed (from document meta-data, if any)
	QDateTime m_printed;

	// The total editing time for the document (from document meta-data, if any)
	int m_totalEditingTime;

	// The number of pages in the document (from document meta-data, if any)
	int m_numberOfPages;

	// The number of words in the document (from document meta-data, if any)
	int m_numberOfWords;

	// The number of characters in the document (from document meta-data, if any)
	int m_numberOfCharacters;

	// The number of characters (without spaces) in the document (from document meta-data, if any)
	int m_numberOfCharactersWithoutSpaces;

	// The version of the document (from document meta-data, if any)
	int m_versionNumber;

	// The internal version of the document (from document meta-data, if any)
	int m_internalVersionNumber;

	// User properties dictionary (from document meta-data, if any)
	QHash< QString, QVariant > m_userProps;
    };
}

#endif

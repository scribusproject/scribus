/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sat Okt 03 2015
	copyright            : (C) 2015 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef SLADOCUMENTRTFOUTPUT_H
#define SLADOCUMENTRTFOUTPUT_H

#include <QTextCodec>
#include "AbstractRtfOutput.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"

class QImage;
class QTextImageFormat;
class QString;
class QColor;
class PageItem;
class ScribusDoc;
class ParagraphStyle;
class CharStyle;

namespace RtfReader
{
	class SlaDocumentRtfOutput : public AbstractRtfOutput
	{
	public:
		explicit SlaDocumentRtfOutput(PageItem* ite, ScribusDoc* doc);
		virtual ~SlaDocumentRtfOutput();
		virtual void startGroup();
		virtual void endGroup();
		virtual void setEncoding(const int enc);
		virtual void appendText(const QByteArray &text);
		virtual void appendUnicodeText(const QString &text);
		virtual void insertPar();
		virtual void insertTab();
		virtual void insertLeftQuote();
		virtual void insertRightQuote();
		virtual void insertLeftDoubleQuote();
		virtual void insertRightDoubleQuote();
		virtual void insertEnDash();
		virtual void insertEmDash();
		virtual void insertEnSpace();
		virtual void insertEmSpace();
		virtual void insertBullet();
		virtual void insertNewLine();
		virtual void setFontItalic(const int value);
		virtual void setFontBold(const int value);
		virtual void setFontUnderline(const int value);
		virtual void setFontWordUnderline(const int value);
		virtual void setFontOutline(const int value);
		virtual void setFontShadow(const int value);
		virtual void setFontSmallCaps(const int value);
		virtual void setFontCaps(const int value);
		virtual void setFontStrikeOut(const int value);
		virtual void setFontScaleH(const int value);
		virtual void setFontOffset(const int value);
		virtual void setFontStretch(const int value);
		virtual void setFontStretchTw(const int value);
		virtual void setFontPointSize(const int pointSize);
		virtual void setFontLineSpacing(const int value);
		virtual void setForegroundColour(const int colourIndex);
		virtual void setHighlightColour(const int colourIndex);
		virtual void setParagraphPatternBackgroundColour(const int colourIndex);
		virtual void setFont(const int fontIndex);
		virtual void setDefaultFont(const int fontIndex);
		virtual void setFontSuperscript();
		virtual void setFontSubscript();
		virtual void setTextDirectionLeftToRight();
		virtual void setTextDirectionRightToLeft();
		virtual void appendToColourTable(const QColor &colour);
		virtual void insertFontTableEntry(FontTableEntry fontTableEntry, quint32 fontTableIndex);
		virtual void insertStyleSheetTableEntry(quint32 stylesheetTableIndex, ParagraphStyle stylesheetTableEntry);
		virtual void resolveStyleSheetParents(QHash<quint32, int> &parentSet);
		virtual void useStyleSheetTableEntry(const int styleIndex);
		virtual void resetParagraphFormat();
		virtual void resetCharacterProperties();
		virtual void setParagraphAlignmentLeft();
		virtual void setParagraphAlignmentCentred();
		virtual void setParagraphAlignmentJustified();
		virtual void setParagraphAlignmentRight();
		virtual void setFirstLineIndent(const int twips);
		virtual void setLeftIndent(const int twips);
		virtual void setRightIndent(const int twips);
		virtual void createImage(const QByteArray &image, int width, int height, int type);
		virtual void setPageHeight(const int pageHeight);
		virtual void setPageWidth(const int pageWidth);
		virtual void setSpaceBefore(const int twips);
		virtual void setSpaceAfter(const int twips);
		virtual void keepWithNext();
		virtual void setDropCapsLines(const int value);
		virtual void setDropCaps();
		virtual void addTabStop(const int value, const int type);
		QString getFontName(QString name);
		double pixelsFromTwips(const int twips);
		virtual QTextCodec *getCurrentCodec() { return m_codec; }
	private:
		PageItem* m_item;
		ScribusDoc* m_Doc;
		QTextCodec *m_codec;
		QStack<ParagraphStyle> m_textStyle;
		QStack<CharStyle> m_textCharStyle;
		QList<QString> m_colourTable;
		QHash<int, FontTableEntry> m_fontTable;
		QHash<int, FontTableEntry> m_fontTableReal;
		QHash<int, ParagraphStyle> m_stylesTable;
		QList<QByteArray> m_codecList;
		bool m_keepn;
		bool m_isBold;
		bool m_isItalic;
	};
}
#endif // SLADOCUMENTRTFOUTPUT_H

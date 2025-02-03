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

#include <QHash>
#include <QList>
#include <QStack>
#include <QString>
#include <QTextCodec>

#include "AbstractRtfOutput.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"

class CharStyle;
class PageItem;
class ParagraphStyle;
class QByteArray;
class QColor;
class QString;
class ScribusDoc;

namespace RtfReader
{
	class SlaDocumentRtfOutput : public AbstractRtfOutput
	{
	public:
		explicit SlaDocumentRtfOutput(PageItem* ite, ScribusDoc* doc, bool prefix);

		void startGroup() override;
		void endGroup() override;
		void setEncoding(const int enc) override;
		void appendText(const QByteArray &textBytes) override;
		void appendUnicodeText(const QString &text) override;
		void insertPar() override;
		void insertTab() override;
		void insertLeftQuote() override;
		void insertRightQuote() override;
		void insertLeftDoubleQuote() override;
		void insertRightDoubleQuote() override;
		void insertEnDash() override;
		void insertEmDash() override;
		void insertEnSpace() override;
		void insertEmSpace() override;
		void insertBullet() override;
		void insertNewLine() override;
		void setFontItalic(const int value) override;
		void setFontBold(const int value) override;
		void setFontUnderline(const int value) override;
		void setFontWordUnderline(const int value) override;
		void setFontOutline(const int value) override;
		void setFontShadow(const int value) override;
		void setFontSmallCaps(const int value) override;
		void setFontCaps(const int value) override;
		void setFontStrikeOut(const int value) override;
		void setFontScaleH(const int value) override;
		void setFontOffset(const int value) override;
		void setFontStretch(const int value) override;
		void setFontStretchTw(const int value) override;
		void setFontPointSize(const int pointSize) override;
		void setFontLineSpacing(const int value) override;
		void setForegroundColour(const int colourIndex) override;
		void setHighlightColour(const int colourIndex) override;
		void setParagraphPatternBackgroundColour(const int colourIndex) override;
		void setCharacterPatternBackgroundColour(const int colourIndex) override;
		void setFont(const int fontIndex) override;
		void setDefaultFont(const int fontIndex) override;
		void setFontSuperscript() override;
		void setFontSubscript() override;
		void setTextDirectionLeftToRight() override;
		void setTextDirectionRightToLeft() override;
		void appendToColourTable(const QColor &colour) override;
		void insertFontTableEntry(FontTableEntry fontTableEntry, quint32 fontTableIndex) override;
		void insertStyleSheetTableEntry(quint32 stylesheetTableIndex, const ParagraphStyle& stylesheetTableEntry) override;
		void resolveStyleSheetParents(QHash<quint32, int> &parentSet) override;
		void useStyleSheetTableEntry(const int styleIndex) override;
		void resetParagraphFormat() override;
		void resetCharacterProperties() override;
		void setParagraphAlignmentLeft() override;
		void setParagraphAlignmentCentred() override;
		void setParagraphAlignmentJustified() override;
		void setParagraphAlignmentRight() override;
		void setFirstLineIndent(const int twips) override;
		void setLeftIndent(const int twips) override;
		void setRightIndent(const int twips) override;
		void createImage(const QByteArray &image, int width, int height, int type) override;
		void setPageHeight(const int pageHeight) override;
		void setPageWidth(const int pageWidth) override;
		void setSpaceBefore(const int twips) override;
		void setSpaceAfter(const int twips) override;
		void keepWithNext() override;
		void setDropCapsLines(const int value) override;
		void setDropCaps() override;
		void addTabStop(const int value, const int type) override;
		QString getFontName(const QString& name);
		double pixelsFromTwips(const int twips);
		QTextCodec *getCurrentCodec()  override { return m_codec; }

	private:
		PageItem* m_item { nullptr };
		ScribusDoc* m_Doc { nullptr };
		QTextCodec *m_codec { nullptr };
		QStack<ParagraphStyle> m_textStyle;
		QStack<CharStyle> m_textCharStyle;
		QList<QString> m_colourTable;
		QHash<int, FontTableEntry> m_fontTable;
		QHash<int, FontTableEntry> m_fontTableReal;
		QHash<int, ParagraphStyle> m_stylesTable;
		QList<QByteArray> m_codecList;
		bool m_keepn { false };
		bool m_isBold { false };
		bool m_isItalic { false };
		bool m_prefixName { false };
	};
}
#endif // SLADOCUMENTRTFOUTPUT_H

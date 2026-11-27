/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Fri Oct 30 2026
	copyright            : (C) 2026 by Scribus Team
 ***************************************************************************/
#ifndef RSTIM_H
#define RSTIM_H

#include "pluginapi.h"
#include "pageitem.h"
#include "rstparser.h"
#include "fonts/scface.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"
#include "styles/styleset.h"

#include <QString>
#include <QStringList>
#include <QVector>

extern "C" PLUGIN_API void GetText2(const QString& filename, const QString& encoding, bool textOnly, bool prefix, bool append, PageItem *textItem);
extern "C" PLUGIN_API QString FileFormatName();
extern "C" PLUGIN_API QStringList FileExtensions();

class RstIm
{
	public:
		RstIm(const QString& fileName, const QString& encoding, PageItem *textItem, bool textOnly, bool prefix, bool append);
		~RstIm() = default;

	private:
		QStringList readFile(const QString& fileName, const QString& encoding) const;
		void setupFonts();
		void insertBlocks(const QVector<RstBlock>& blocks);

		QString styleName(const QString& baseName) const;
		QString paragraphStyleFor(const RstBlock& block);
		QString charStyleFor(RstInlineType type);
		ParagraphStyle newParagraphStyle(const QString& name) const;
		bool registerParagraphStyle(const ParagraphStyle& style);
		double indentWidth(int indent) const;
		double textWidth(const QString& text) const;

		ScribusDoc* m_Doc { nullptr };
		PageItem* m_item { nullptr };
		bool m_prefixName { false };
		bool m_textOnly { false };
		bool m_append { false };

		StyleSet<ParagraphStyle> m_paraStyles;
		StyleSet<CharStyle> m_charStyles;

		ScFace m_regularFace;
		ScFace m_boldFace;
		ScFace m_italicFace;
		ScFace m_monoFace;
		double m_baseFontSize { 120.0 }; // tenths of a point
};

#endif

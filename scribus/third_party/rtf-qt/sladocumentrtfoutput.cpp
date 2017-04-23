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
#include "sladocumentrtfoutput.h"

#include <QApplication>
#include <QColor>
#include <QImage>
#include <QString>
#include <QTextImageFormat>
#include "pageitem.h"
#include "prefsmanager.h"
#include "fileloader.h"
#include "loadsaveplugin.h"
#include "scribusdoc.h"
#include "selection.h"
#include "ui/missing.h"
#include "util.h"
#include <QDebug>

namespace RtfReader
{
	SlaDocumentRtfOutput::SlaDocumentRtfOutput(PageItem *ite, ScribusDoc *doc, bool prefix) : AbstractRtfOutput()
	{
		m_item = ite;
		m_Doc = doc;
		m_prefixName = prefix;
		QString pStyle = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setParent(pStyle);
		newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
		m_textStyle.push(newStyle);
		m_textCharStyle.push(newStyle.charStyle());
		m_textCharStyle.top().setFontSize(120.0);
		QList<ParagraphStyle::TabRecord> tbs;
		tbs.clear();
		m_textStyle.top().setTabValues(tbs);
		m_fontTable.clear();
		m_fontTableReal.clear();
		m_codecList = QTextCodec::availableCodecs();
		for (int i = 0; i < m_codecList.count(); ++i)
		{
			QByteArray codecName = m_codecList.at(i);
			m_codecList[i] = codecName.toLower();
		}
		if (m_codecList.contains("cp1252"))
			m_codec = QTextCodec::codecForName("cp1252");		// Default ANSI codec
		else
			m_codec = QTextCodec::codecForLocale();
		m_keepn = false;
		m_isBold = false;
		m_isItalic = false;
	}

	SlaDocumentRtfOutput::~SlaDocumentRtfOutput()
	{
	}

	void SlaDocumentRtfOutput::setEncoding(const int enc)
	{
		QByteArray ba;
		ba.setNum(enc);
		QByteArray encTest = "cp" + ba;
		if (m_codecList.contains(encTest))
			m_codec = QTextCodec::codecForName(encTest);
		else if (m_codecList.contains("cp1252"))
			m_codec = QTextCodec::codecForName("cp1252");
	}

	void SlaDocumentRtfOutput::startGroup()
	{
		m_textStyle.push(m_textStyle.top());
		m_textCharStyle.push(m_textCharStyle.top());
	}

	void SlaDocumentRtfOutput::endGroup()
	{
		m_textCharStyle.pop();
		m_textStyle.pop();
	}

	void SlaDocumentRtfOutput::appendText(const QByteArray &text)
	{
		int posC = m_item->itemText.length();
		QString m_txt = m_codec->toUnicode(text);
		if (text.count() > 0)
		{
			m_txt.replace(QChar(10), SpecialChars::LINEBREAK);
			m_txt.replace(QChar(12), SpecialChars::FRAMEBREAK);
			m_txt.replace(QChar(30), SpecialChars::NBHYPHEN);
			m_txt.replace(QChar(160), SpecialChars::NBSPACE);
			m_item->itemText.insertChars(posC, m_txt);
			m_item->itemText.applyStyle(posC, m_textStyle.top());
			m_item->itemText.applyCharStyle(posC, m_txt.length(), m_textCharStyle.top());
		}
	}

	void SlaDocumentRtfOutput::appendUnicodeText(const QString &text)
	{
		int posC = m_item->itemText.length();
		QString m_txt = text;
		if (text.count() > 0)
		{
			m_txt.replace(QChar(10), SpecialChars::LINEBREAK);
			m_txt.replace(QChar(12), SpecialChars::FRAMEBREAK);
			m_txt.replace(QChar(30), SpecialChars::NBHYPHEN);
			m_txt.replace(QChar(160), SpecialChars::NBSPACE);
			m_item->itemText.insertChars(posC, m_txt);
			m_item->itemText.applyStyle(posC, m_textStyle.top());
			m_item->itemText.applyCharStyle(posC, m_txt.length(), m_textCharStyle.top());
		}
	}

	void SlaDocumentRtfOutput::insertPar()
	{
		int posT = m_item->itemText.length();
		if (posT > 0)
		{
			m_item->itemText.insertChars(posT, SpecialChars::PARSEP);
			m_item->itemText.applyStyle(posT, m_textStyle.top());
		}
	}

	void SlaDocumentRtfOutput::insertTab()
	{
		int posT = m_item->itemText.length();
		m_item->itemText.insertChars(posT, SpecialChars::TAB);
		m_item->itemText.applyStyle(posT, m_textStyle.top());
	}

	void SlaDocumentRtfOutput::insertLeftQuote()
	{
		m_item->itemText.insertChars(QString(QChar(0x2018)), true);
	}

	void SlaDocumentRtfOutput::insertRightQuote()
	{
		m_item->itemText.insertChars(QString(QChar(0x2019)), true);
	}

	void SlaDocumentRtfOutput::insertLeftDoubleQuote()
	{
		m_item->itemText.insertChars(QString(QChar(0x201c)), true);
	}

	void SlaDocumentRtfOutput::insertRightDoubleQuote()
	{
		m_item->itemText.insertChars(QString(QChar(0x201d)), true);
	}

	void SlaDocumentRtfOutput::insertEnDash()
	{
		m_item->itemText.insertChars(QString(QChar(0x2013)), true);
	}

	void SlaDocumentRtfOutput::insertEmDash()
	{
		m_item->itemText.insertChars(QString(QChar(0x2014)), true);
	}

	void SlaDocumentRtfOutput::insertEmSpace()
	{
		m_item->itemText.insertChars(QString(QChar(0x2003)), true);
	}

	void SlaDocumentRtfOutput::insertEnSpace()
	{
		m_item->itemText.insertChars(QString(QChar(0x2002)), true);
	}

	void SlaDocumentRtfOutput::insertBullet()
	{
		m_item->itemText.insertChars(QString(QChar(0x2022)), true);
	}

	void SlaDocumentRtfOutput::insertNewLine()
	{
		int posT = m_item->itemText.length();
		if (posT > 0)
		{
			m_item->itemText.insertChars(posT, SpecialChars::LINEBREAK);
			m_item->itemText.applyStyle(posT, m_textStyle.top());
		}
	}

	void SlaDocumentRtfOutput::setFontItalic(const int value)
	{
		m_isItalic = (value != 0);
		int posC = m_item->itemText.length();
		m_item->itemText.insertChars(posC, "B");
		m_item->itemText.applyStyle(posC, m_textStyle.top());
		m_item->itemText.applyCharStyle(posC, 1, m_textCharStyle.top());
		QString fam = m_item->itemText.charStyle(posC).font().family();
		m_item->itemText.removeChars(posC, 1);
		if (fam.isEmpty())
			return;
		QStringList slist = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.fontMap[fam];
		if (m_isItalic)
		{
			if (m_isBold)
			{
				if (slist.contains("Bold Italic"))
					m_textCharStyle.top().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold Italic"]);
			}
			else if (slist.contains("Italic"))
				m_textCharStyle.top().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Italic"]);
		}
		else
		{
			if (m_isBold)
			{
				if (slist.contains("Bold"))
					m_textCharStyle.top().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold"]);
			}
			else if (slist.contains("Regular"))
				m_textCharStyle.top().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Regular"]);
		}
	}

	void SlaDocumentRtfOutput::setFontBold(const int value)
	{
		m_isBold = (value != 0);
		int posC = m_item->itemText.length();
		m_item->itemText.insertChars(posC, "B");
		m_item->itemText.applyStyle(posC, m_textStyle.top());
		m_item->itemText.applyCharStyle(posC, 1, m_textCharStyle.top());
		QString fam = m_item->itemText.charStyle(posC).font().family();
		m_item->itemText.removeChars(posC, 1);
		if (fam.isEmpty())
			return;
		QStringList slist = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.fontMap[fam];
		if (m_isBold)
		{
			if (m_isItalic)
			{
				if (slist.contains("Bold Italic"))
					m_textCharStyle.top().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold Italic"]);
			}
			else if (slist.contains("Bold"))
				m_textCharStyle.top().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold"]);
		}
		else
		{
			if (m_isItalic)
			{
				if (slist.contains("Italic"))
					m_textCharStyle.top().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Italic"]);
			}
			else if (slist.contains("Regular"))
				m_textCharStyle.top().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Regular"]);
		}
	}

	void SlaDocumentRtfOutput::setFontUnderline(const int value)
	{
		StyleFlag styleEffects = m_textCharStyle.top().effects();
		if (value != 0)
			styleEffects |= ScStyle_Underline;
		else
			styleEffects &= ~ScStyle_Underline;
		m_textCharStyle.top().setFeatures(styleEffects.featureList());
	}

	void SlaDocumentRtfOutput::setFontWordUnderline(const int value)
	{
		StyleFlag styleEffects = m_textCharStyle.top().effects();
		if (value != 0)
			styleEffects |= ScStyle_UnderlineWords;
		else
			styleEffects &= ~ScStyle_UnderlineWords;
		m_textCharStyle.top().setFeatures(styleEffects.featureList());
	}

	void SlaDocumentRtfOutput::setFontOutline(const int value)
	{
		StyleFlag styleEffects = m_textCharStyle.top().effects();
		if (value != 0)
			styleEffects |= ScStyle_Outline;
		else
			styleEffects &= ~ScStyle_Outline;
		m_textCharStyle.top().setFeatures(styleEffects.featureList());
	}

	void SlaDocumentRtfOutput::setFontShadow(const int value)
	{
		StyleFlag styleEffects = m_textCharStyle.top().effects();
		if (value != 0)
			styleEffects |= ScStyle_Shadowed;
		else
			styleEffects &= ~ScStyle_Shadowed;
		m_textCharStyle.top().setFeatures(styleEffects.featureList());
	}

	void SlaDocumentRtfOutput::setFontSmallCaps(const int value)
	{
		StyleFlag styleEffects = m_textCharStyle.top().effects();
		if (value != 0)
			styleEffects |= ScStyle_SmallCaps;
		else
			styleEffects &= ~ScStyle_SmallCaps;
		m_textCharStyle.top().setFeatures(styleEffects.featureList());
	}

	void SlaDocumentRtfOutput::setFontCaps(const int value)
	{
		StyleFlag styleEffects = m_textCharStyle.top().effects();
		if (value != 0)
			styleEffects |= ScStyle_AllCaps;
		else
			styleEffects &= ~ScStyle_AllCaps;
		m_textCharStyle.top().setFeatures(styleEffects.featureList());
	}

	void SlaDocumentRtfOutput::setFontStrikeOut(const int value)
	{
		StyleFlag styleEffects = m_textCharStyle.top().effects();
		if (value != 0)
			styleEffects |= ScStyle_Strikethrough;
		else
			styleEffects &= ~ScStyle_Strikethrough;
		m_textCharStyle.top().setFeatures(styleEffects.featureList());
	}

	void SlaDocumentRtfOutput::setFontScaleH(const int value)
	{
		m_textCharStyle.top().setScaleH(value * 10.0);
	}

	void SlaDocumentRtfOutput::setFontOffset(const int value)
	{
		m_textCharStyle.top().setBaselineOffset((value * 10000 / 2) / m_textCharStyle.top().fontSize());
	}

	void SlaDocumentRtfOutput::setFontStretch(const int value)
	{
		m_textCharStyle.top().setTracking((value * 10000 / 4) / m_textCharStyle.top().fontSize());
	}

	void SlaDocumentRtfOutput::setFontStretchTw(const int value)
	{
		m_textCharStyle.top().setTracking((pixelsFromTwips(value) * 10000) / m_textCharStyle.top().fontSize());
	}

	void SlaDocumentRtfOutput::setFontPointSize(const int pointSize)
	{
		m_textCharStyle.top().setFontSize(pointSize * 10.0);
		m_textStyle.top().setLineSpacing(pointSize);
	}

	void SlaDocumentRtfOutput::setFontLineSpacing(const int value)
	{
		if (value == 0)
			m_textStyle.top().setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
		else
		{
			m_textStyle.top().setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
			m_textStyle.top().setLineSpacing(pixelsFromTwips(qAbs(value)));
		}
	}

	void SlaDocumentRtfOutput::setForegroundColour(const int colourIndex)
	{
		if ((m_colourTable.count() != 0) && (colourIndex < m_colourTable.count()))
			m_textCharStyle.top().setFillColor(m_colourTable.value(colourIndex));
	}

	void SlaDocumentRtfOutput::setHighlightColour(const int colourIndex)
	{
	//	qDebug() << "setHighlightColour";
	}

	void SlaDocumentRtfOutput::setParagraphPatternBackgroundColour(const int colourIndex)
	{
		if ((m_colourTable.count() != 0) && (colourIndex < m_colourTable.count()))
			m_textStyle.top().setBackgroundColor(m_colourTable.value(colourIndex));
	}

	void SlaDocumentRtfOutput::setCharacterPatternBackgroundColour(const int colourIndex)
	{
		if ((m_colourTable.count() != 0) && (colourIndex < m_colourTable.count()))
			m_textCharStyle.top().setBackColor(m_colourTable.value(colourIndex));
	}

	void SlaDocumentRtfOutput::setFont(const int fontIndex)
	{
		if (m_fontTableReal.contains(fontIndex))
		{
			m_textCharStyle.top().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[m_fontTableReal[fontIndex].fontName()]);
			if (m_fontTableReal[fontIndex].encoding() > 0)
				setEncoding(m_fontTableReal[fontIndex].encoding());
		}
		else
		{
			if (m_fontTable.contains(fontIndex))
			{
				FontTableEntry fontTableEntry = m_fontTable[fontIndex];
				QString fontName = getFontName(fontTableEntry.fontName());
				m_textCharStyle.top().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fontName]);
				fontTableEntry.setFontName(fontName);
				m_fontTableReal.insert(fontIndex, fontTableEntry);
				if (fontTableEntry.encoding() > 0)
					setEncoding(fontTableEntry.encoding());
			}
		}
	}

	void SlaDocumentRtfOutput::setDefaultFont(const int fontIndex)
	{
		setFont(fontIndex);
	}

	void SlaDocumentRtfOutput::appendToColourTable(const QColor &colour)
	{
		ScColor tmp;
		tmp.fromQColor(colour);
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		QString fNam = m_Doc->PageColors.tryAddColor("FromRtf"+colour.name(), tmp);
		m_colourTable.append(fNam);
	}

	void SlaDocumentRtfOutput::insertFontTableEntry(FontTableEntry fontTableEntry, quint32 fontTableIndex)
	{
		m_fontTable.insert(fontTableIndex, fontTableEntry);

	}

	void SlaDocumentRtfOutput::insertStyleSheetTableEntry(quint32 stylesheetTableIndex, const ParagraphStyle& stylesheetTableEntry)
	{
		ParagraphStyle pStyle(stylesheetTableEntry);
		if (m_prefixName)
			pStyle.setName(m_item->itemName() + "_" + stylesheetTableEntry.name());
		if (pStyle.charStyle().fontVariant() != "")
		{
			int fontInd = pStyle.charStyle().fontVariant().toInt();
			pStyle.charStyle().setFontVariant("");
			if (m_fontTable.contains(fontInd))
			{
				FontTableEntry fontTableEntry = m_fontTable[fontInd];
				QString fontName = getFontName(fontTableEntry.fontName());
				pStyle.charStyle().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fontName]);
				fontTableEntry.setFontName(fontName);
				m_fontTableReal.insert(fontInd, fontTableEntry);
			}
		}
		StyleSet<ParagraphStyle>tmp;
		tmp.create(pStyle);
		m_Doc->redefineStyles(tmp, false);
		m_stylesTable.insert(stylesheetTableIndex, pStyle);
	}

	void SlaDocumentRtfOutput::resolveStyleSheetParents(QHash<quint32, int> &parentSet)
	{
		for (QHash<quint32, int>::iterator it = parentSet.begin(); it != parentSet.end(); ++it)
		{
			if (it.value() >= 0)
			{
				ParagraphStyle old = m_Doc->paragraphStyle(m_stylesTable[it.key()].name());
				old.setParent(m_stylesTable[it.value()].name());
				StyleSet<ParagraphStyle>tmp2;
				tmp2.create(old);
				m_Doc->redefineStyles(tmp2, false);
				m_stylesTable.insert(it.key(), old);
			}
		}
	}

	void SlaDocumentRtfOutput::useStyleSheetTableEntry(const int styleIndex)
	{
		if (m_stylesTable.contains(styleIndex))
		{
			ParagraphStyle newStyle2;
			newStyle2.setParent(m_stylesTable[styleIndex].name());
			m_textStyle.pop();
			m_textStyle.push(newStyle2);
			m_textCharStyle.pop();
			m_textCharStyle.push(newStyle2.charStyle());
		}
	}

	void SlaDocumentRtfOutput::resetParagraphFormat()
	{
		QString pStyle = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setParent(pStyle);
		newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
		m_textStyle.pop();
		m_textStyle.push(newStyle);
		QList<ParagraphStyle::TabRecord> tbs;
		tbs.clear();
		m_textStyle.top().setTabValues(tbs);
	}

	void SlaDocumentRtfOutput::resetCharacterProperties()
	{
		QString pStyle = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setParent(pStyle);
		newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
		m_textCharStyle.pop();
		m_textCharStyle.push(newStyle.charStyle());
		m_textCharStyle.top().setFontSize(120.0);
	}

	void SlaDocumentRtfOutput::setParagraphAlignmentLeft()
	{
		m_textStyle.top().setAlignment(ParagraphStyle::Leftaligned);
	}

	void SlaDocumentRtfOutput::setParagraphAlignmentCentred()
	{
		m_textStyle.top().setAlignment(ParagraphStyle::Centered);
	}

	void SlaDocumentRtfOutput::setParagraphAlignmentJustified()
	{
		m_textStyle.top().setAlignment(ParagraphStyle::Justified);
	}

	void SlaDocumentRtfOutput::setParagraphAlignmentRight()
	{
		m_textStyle.top().setAlignment(ParagraphStyle::Rightaligned);
	}

	void SlaDocumentRtfOutput::setLeftIndent(const int twips)
	{
		m_textStyle.top().setLeftMargin(pixelsFromTwips(twips));
	}

	void SlaDocumentRtfOutput::setRightIndent(const int twips)
	{
		m_textStyle.top().setRightMargin(pixelsFromTwips(twips));
	}

	void SlaDocumentRtfOutput::setSpaceBefore(const int twips)
	{
		m_textStyle.top().setGapBefore(pixelsFromTwips(twips));
	}

	void SlaDocumentRtfOutput::setSpaceAfter(const int twips)
	{
		m_textStyle.top().setGapAfter(pixelsFromTwips(twips));
	}

	void SlaDocumentRtfOutput::keepWithNext()
	{
		m_keepn = true;
	}

	void SlaDocumentRtfOutput::setDropCapsLines(const int value)
	{
		m_textStyle.top().setDropCapLines(value);
	}

	void SlaDocumentRtfOutput::setDropCaps()
	{
		m_textStyle.top().setHasDropCap(true);
	}

	void SlaDocumentRtfOutput::addTabStop(const int value, const int type)
	{
		double tabVal = pixelsFromTwips(value);
		ParagraphStyle::TabRecord tb;
		tb.tabType = type;
		tb.tabPosition = tabVal;
		tb.tabFillChar = QChar();
		QList<ParagraphStyle::TabRecord> tbs = m_textStyle.top().tabValues();
		if (tbs.count() == 0)
			tbs.append(tb);
		else
		{
			bool ins = false;
			for (int a = 0; a < tbs.count() - 1; a++)
			{
				if ((tb > tbs[a]) && (tb < tbs[a+1]))
				{
					tbs.insert(a, tb);
					ins = true;
					break;
				}
			}
			if (!ins)
				tbs.append(tb);
		}
		m_textStyle.top().setTabValues(tbs);
	}

	void SlaDocumentRtfOutput::setFirstLineIndent(const int twips)
	{
		m_textStyle.top().setFirstIndent(pixelsFromTwips(twips));
	}

	void SlaDocumentRtfOutput::setFontSuperscript()
	{
		StyleFlag styleEffects = m_textCharStyle.top().effects();
		styleEffects |= ScStyle_Superscript;
		m_textCharStyle.top().setFeatures(styleEffects.featureList());
	}

	void SlaDocumentRtfOutput::setFontSubscript()
	{
		StyleFlag styleEffects = m_textCharStyle.top().effects();
		styleEffects |= ScStyle_Subscript;
		m_textCharStyle.top().setFeatures(styleEffects.featureList());
	}

	void SlaDocumentRtfOutput::setTextDirectionLeftToRight()
	{
	//	qDebug() << "setTextDirectionLeftToRight";
	}

	void SlaDocumentRtfOutput::setTextDirectionRightToLeft()
	{
	//	qDebug() << "setTextDirectionRightToLeft";
	}

	void SlaDocumentRtfOutput::createImage(const QByteArray &image, int width, int height, int type)
	{
		QString imgExt = "";
		double ww = pixelsFromTwips(width);
		double hh = pixelsFromTwips(height);
		if ((type == 0) || (type == 3) || (type == 4))
		{
			if (type == 0)
				imgExt = "jpg";
			else if (type == 3)
				imgExt = "pict";
			else if (type == 4)
				imgExt = "pmg";
			QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + QString("/scribus_temp_rtf_XXXXXX.") + imgExt);
			tempFile->setAutoRemove(false);
			if (tempFile->open())
			{
				tempFile->write(image);
				QString fileName = getLongPathName(tempFile->fileName());
				tempFile->close();
				int posT = m_item->itemText.length();
				int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Rectangle, 0, 0, ww, hh, 0, CommonStrings::None, CommonStrings::None);
				PageItem* item = m_Doc->Items->at(z);
				item->OldB2 = item->width();
				item->OldH2 = item->height();
				item->updateClip();
				item->isInlineImage = true;
				item->isTempFile = true;
				item->AspectRatio = true;
				item->ScaleType   = false;
				m_Doc->loadPict(fileName, item);
				m_Doc->Items->takeAt(z);
				item->isEmbedded = true;
				item->gXpos = 0;
				item->gYpos = 0;
				item->gWidth = item->width();
				item->gHeight = item->height();
				int fIndex = m_Doc->addToInlineFrames(item);
				m_item->itemText.insertObject(fIndex);
				m_item->itemText.applyStyle(posT, m_textStyle.top());
			}
			delete tempFile;
		}
		else if ((type == 1) || (type == 2))
		{
			if (type == 1)
				imgExt = "wmf";
			else if (type == 2)
				imgExt = "emf";
			QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + QString("/scribus_temp_rtf_XXXXXX.") + imgExt);
			tempFile->setAutoRemove(false);
			if (tempFile->open())
			{
				tempFile->write(image);
				QString fileName = getLongPathName(tempFile->fileName());
				tempFile->close();
				FileLoader *fileLoader = new FileLoader(fileName);
				int testResult = fileLoader->testFile();
				delete fileLoader;
				if (testResult != -1)
				{
					const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
					if( fmt )
					{
						fmt->setupTargets(m_Doc, 0, 0, 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
						fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
						if (m_Doc->m_Selection->count() > 0)
						{
							int posT = m_item->itemText.length();
							PageItem* item = m_Doc->groupObjectsSelection();
							item->setWidthHeight(ww, hh, true);
							item->OldB2 = item->width();
							item->OldH2 = item->height();
							item->updateClip();
							m_Doc->Items->removeAll(item);
							item->isEmbedded = true;
							item->gXpos = 0;
							item->gYpos = 0;
							item->gWidth = item->width();
							item->gHeight = item->height();
							int fIndex = m_Doc->addToInlineFrames(item);
							m_item->itemText.insertObject(fIndex);
							m_item->itemText.applyStyle(posT, m_textStyle.top());
						}
					}
				}
			}
			delete tempFile;
		}
	}

	void SlaDocumentRtfOutput::setPageHeight(const int pageHeight)
	{
	}

	void SlaDocumentRtfOutput::setPageWidth(const int pageWidth)
	{
	}

	QString SlaDocumentRtfOutput::getFontName(QString name)
	{
		QString fontName(name);
		SCFontsIterator it(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts);
		for ( ; it.hasNext(); it.next())
		{
			if (it.current().family().toLower() == fontName.toLower())
			{
				if (it.currentKey().toLower() == fontName.toLower()) // exact Match
				{
					return fontName;
				}
				else
				{
					QStringList slist = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.fontMap[it.current().family()];
					slist.sort();
					if (slist.count() > 0)
					{
						int reInd = slist.indexOf("Regular");
						if (reInd < 0)
							fontName = it.current().family() + " " + slist[0];
						else
							fontName = it.current().family() + " " + slist[reInd];
						return fontName;
					}
				}
			}
		}
		if (!PrefsManager::instance()->appPrefs.fontPrefs.GFontSub.contains(fontName))
		{
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			MissingFont dia(0, fontName, m_Doc);
			dia.exec();
			qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
			PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[fontName] = dia.getReplacementFont();
			fontName = dia.getReplacementFont();
		}
		else
			fontName = PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[fontName];
		return fontName;
	}

	double SlaDocumentRtfOutput::pixelsFromTwips(const int twips)
	{
		return twips / 1440.0 * 72.0;
	}
}

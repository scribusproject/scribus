/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribuscore.h"
#include "sampleitem.h"
#include "ui/loremipsum.h"
#include "scpainter.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "commonstrings.h"
#include "pageitem_textframe.h"
#include "prefsmanager.h"
#include <QColor>
#include <QString>
#include <QCursor>
//Added by qt3to4:
#include <QPixmap>

SampleItem::SampleItem() : QObject()
{
	m_Doc = new ScribusDoc();
	if (!m_Doc)
		return;
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(1, 1, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
	// tmp colors. to be removed in descrictor
	m_Doc->PageColors.insert("__blackforpreview__", ScColor(0, 0, 0, 255));
	m_Doc->PageColors.insert("__whiteforpreview__", ScColor(0, 0, 0, 0));
	m_Doc->PageColors.insert("__whiteforpreviewbg__", ScColor(0, 0, 0, 0));
	m_bgShade = 100;
	m_tmpStyle.setName("(preview temporary)");
	m_tmpStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
	m_tmpStyle.setLineSpacing((m_Doc->itemToolPrefs().textSize / 10.0) * (static_cast<double>(m_Doc->typographicPrefs().autoLineSpacing) / 100));
	m_tmpStyle.setAlignment(ParagraphStyle::Leftaligned);
	m_tmpStyle.setLeftMargin(0);
	m_tmpStyle.setFirstIndent(0);
	m_tmpStyle.setRightMargin(0);
	m_tmpStyle.setGapBefore(0);
	m_tmpStyle.setGapAfter(0);
	m_tmpStyle.charStyle().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[m_Doc->itemToolPrefs().textFont]);
	m_tmpStyle.charStyle().setFontSize(m_Doc->itemToolPrefs().textSize);
//	tmpStyle.tabValues().clear();
	m_tmpStyle.setHasDropCap(false);
	m_tmpStyle.setHasBullet(false);
	m_tmpStyle.setHasNum(false);
	m_tmpStyle.setDropCapLines(0);//2;
	m_tmpStyle.setParEffectOffset(0);
	m_tmpStyle.charStyle().setFeatures(QStringList(CharStyle::INHERIT));
	m_tmpStyle.charStyle().setFillColor("__blackforpreview__");
	m_tmpStyle.charStyle().setFillShade(100); //m_Doc->toolSettings.dShade;
	m_tmpStyle.charStyle().setStrokeColor("__whiteforpreview__");
	m_tmpStyle.charStyle().setStrokeShade(100); //m_Doc->toolSettings.dShade2;
	m_tmpStyle.charStyle().setShadowXOffset(50);
	m_tmpStyle.charStyle().setShadowYOffset(-50);
	m_tmpStyle.charStyle().setOutlineWidth(10);
	m_tmpStyle.charStyle().setUnderlineOffset(0); //m_Doc->typographicSettings.valueUnderlinePos;
	m_tmpStyle.charStyle().setUnderlineWidth(0); //m_Doc->typographicSettings.valueUnderlineWidth;
	m_tmpStyle.charStyle().setStrikethruOffset(0); //m_Doc->typographicSettings.valueStrikeThruPos;
	m_tmpStyle.charStyle().setStrikethruWidth(0); //m_Doc->typographicSettings.valueStrikeThruPos;
	m_tmpStyle.charStyle().setScaleH(1000);
	m_tmpStyle.charStyle().setScaleV(1000);
	m_tmpStyle.charStyle().setBaselineOffset(0);
	m_tmpStyle.charStyle().setTracking(0);
}

SampleItem::~SampleItem()
{
	delete m_Doc;
}

void SampleItem::setText(QString aText)
{
	m_text = aText;
}

void SampleItem::setLoremIpsum(int para)
{
	LoremParser *m = new LoremParser("loremipsum.xml");
	m_text = m->createLorem(para);
// really needed ??
//	text = QString::fromUtf8(text);
	delete m;
}

void SampleItem::setStyle(const ParagraphStyle& aStyle)
{
	m_tmpStyle = aStyle;
}

void SampleItem::setBgColor(QColor c)
{
	m_Doc->PageColors["__whiteforpreviewbg__"].fromQColor(c);
}

void SampleItem::setBgShade(int c)
{
	m_bgShade = c;
}

void SampleItem::setBgColorMgmt(bool enable)
{
	m_Doc->PageColors["__whiteforpreviewbg__"].setSpotColor(!enable);
}

void SampleItem::setTxColor(QColor c)
{
	m_Doc->PageColors["__blackforpreview__"].fromQColor(c);
}

void SampleItem::setTxShade(int c)
{
	m_tmpStyle.charStyle().setFillShade(c);
}

void SampleItem::setLineSpaMode(int lineSpaMode)
{
	m_tmpStyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(lineSpaMode));
}

void SampleItem::setLineSpa(double lineSpa)
{
	m_tmpStyle.setLineSpacing(lineSpa);
}

void SampleItem::setTextAlignment(int textAlignment)
{
	m_tmpStyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(textAlignment));
}

void SampleItem::setIndent(double indent)
{
	m_tmpStyle.setLeftMargin(indent);
}

void SampleItem::setFirst(double first)
{
	m_tmpStyle.setFirstIndent(first);
}

void SampleItem::setGapBefore(double gapBefore)
{
	m_tmpStyle.setGapBefore(gapBefore);
}

void SampleItem::setGapAfter(double gapAfter)
{
	m_tmpStyle.setGapAfter(gapAfter);
}

void SampleItem::setFont(QString font)
{
	m_tmpStyle.charStyle().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[font]);
}

void SampleItem::setFontSize(int fontSize, bool autoLineSpa)
{
	m_tmpStyle.charStyle().setFontSize(fontSize);
	if (autoLineSpa)
		m_tmpStyle.setLineSpacing((fontSize / 10)  * (m_Doc->typographicPrefs().autoLineSpacing / 100.0));
}

/*void SampleItem::setTabValues(QValueList<PageItem::TabRecord> tabValues)
{
	tmpStyle.TabValues = tabValues;
}*/

void SampleItem::setDrop(bool drop)
{
	m_tmpStyle.setHasDropCap(drop);
}

void SampleItem::setDropLin(int dropLin)
{
	m_tmpStyle.setDropCapLines(dropLin);
}

void SampleItem::setParEffectDist(double dist)
{
	m_tmpStyle.setParEffectOffset(dist);
}

void SampleItem::setBullet(bool bul)
{
	m_tmpStyle.setHasBullet(bul);
}

void SampleItem::setNum(bool num)
{
	m_tmpStyle.setHasNum(num);
}

void SampleItem::setFontEffect(int fontEffect)
{
	m_tmpStyle.charStyle().setFeatures(static_cast<StyleFlag>(fontEffect).featureList());
}

void SampleItem::setFColor(QString fColor)
{
	m_tmpStyle.charStyle().setFillColor(fColor);
}

void SampleItem::setFShade(int fShade)
{
	m_tmpStyle.charStyle().setFillShade(fShade);
}

void SampleItem::setSColor(QString sColor)
{
	m_tmpStyle.charStyle().setStrokeColor(sColor);
}

void SampleItem::setSShade(int sShade)
{
	m_tmpStyle.charStyle().setStrokeShade(sShade);
}

void SampleItem::setBaseAdj(bool baseAdj)
{
	m_tmpStyle.setLineSpacingMode(baseAdj ? ParagraphStyle::BaselineGridLineSpacing : ParagraphStyle::FixedLineSpacing);
}

void SampleItem::setTxtShadowX(int txtShadowX)
{
	m_tmpStyle.charStyle().setShadowXOffset(txtShadowX);
}

void SampleItem::setTxtShadowY(int txtShadowY)
{
	m_tmpStyle.charStyle().setShadowYOffset(txtShadowY);
}

void SampleItem::setTxtOutline(int txtOutline)
{
	m_tmpStyle.charStyle().setOutlineWidth(txtOutline);
}

void SampleItem::setTxtUnderPos(int txtUnderPos)
{
	m_tmpStyle.charStyle().setUnderlineOffset(txtUnderPos);
}

void SampleItem::setTxtUnderWidth(int txtUnderWidth)
{
	m_tmpStyle.charStyle().setUnderlineWidth(txtUnderWidth);
}

void SampleItem::setTxtStrikePos(int txtStrikePos)
{
	m_tmpStyle.charStyle().setStrikethruOffset(txtStrikePos);
}

void SampleItem::setTxtStrikeWidth(int txtStrikeWidth)
{
	m_tmpStyle.charStyle().setStrikethruWidth(txtStrikeWidth);
}

void SampleItem::setScaleH(int scaleH)
{
	m_tmpStyle.charStyle().setScaleH(scaleH);
}

void SampleItem::setScaleV(int scaleV)
{
	m_tmpStyle.charStyle().setScaleV(scaleV);
}

void SampleItem::setBaseOff(int baseOff)
{
	m_tmpStyle.charStyle().setBaselineOffset(baseOff);
}

void SampleItem::setKernVal(int kernVal)
{
	m_tmpStyle.charStyle().setTracking(kernVal);
}

QPixmap SampleItem::getSample(int width, int height)
{
	// if it's false => the used font will be removed from used fonts
	// after sample creating
	bool previouslyUsedFont = false;

	if (m_tmpStyle.charStyle().font().isNone())
		return QPixmap();

	UndoManager::instance()->setUndoEnabled(false); // disable undo

	double frameWidth  = width / PrefsManager::instance()->appPrefs.displayPrefs.displayScale;
	double frameHeight = height / PrefsManager::instance()->appPrefs.displayPrefs.displayScale;

	PageItem_TextFrame *previewItem = new PageItem_TextFrame(m_Doc, 0, 0, frameWidth, frameHeight, 0, "__whiteforpreviewbg__", "__whiteforpreview__");
	QImage pm(width, height, QImage::Format_ARGB32);
	ScPainter *painter = new ScPainter(&pm, width, height, 1.0, 0);
	painter->setZoomFactor(PrefsManager::instance()->appPrefs.displayPrefs.displayScale);

	if (m_Doc->UsedFonts.contains(m_tmpStyle.charStyle().font().scName()))
		previouslyUsedFont = true;

	m_Doc->AddFont(m_tmpStyle.charStyle().font().scName(), qRound(m_Doc->itemToolPrefs().textSize / 10.0));

	previewItem->FrameType = PageItem::TextFrame;
	previewItem->itemText.clear();
//	previewItem->setFont(tmpStyle.charStyle().font()->scName());
	previewItem->Cols = 1;
	m_text.replace(QChar(10),QChar(13)).replace(QChar(5),QChar(13));
	previewItem->itemText.insertChars(0, m_text);
	previewItem->itemText.setDefaultStyle(m_tmpStyle);
	previewItem->setFillColor("__whiteforpreviewbg__");
	previewItem->setFillShade(m_bgShade);
	previewItem->SetRectFrame();
	previewItem->setSampleItem(true);
	previewItem->DrawObj(painter, QRect(0, 0, frameWidth, frameHeight));
	painter->end();
	delete(painter);
	delete previewItem;

	// cleanups and resets
	if (!previouslyUsedFont)
	{
		QString fontName = m_tmpStyle.charStyle().font().scName();
		(*m_Doc->AllFonts)[fontName].decreaseUsage(); // was increased by AddFont()
		m_Doc->UsedFonts.remove(fontName);
	}
//	m_Doc->docParagraphStyles.remove(tmpIndex);
	UndoManager::instance()->setUndoEnabled(true);
	return QPixmap::fromImage(pm);
}

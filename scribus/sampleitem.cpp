/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribuscore.h"
#include "sampleitem.h"
//#include "sampleitem.moc"
#include "loremipsum.h"
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
#include "text/nlsconfig.h"

SampleItem::SampleItem(ScribusDoc* doc) :
	QObject()
{
	used = true;
	m_Doc=doc;

	if (doc==0)
	{
		qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
		// FIXME: main preformance issue here! PV
		m_Doc=ScCore->primaryMainWindow()->doFileNew(//pageWidth, pageHeight,
									0,0,
									//topMargin, leftMargin, rightMargin, bottomMargin,
									1, 1, 1, 1,
									// autoframes. It's disabled in python
									// columnDistance, numberCols, autoframes,
									0, 1, false,
									//pagesType, unit, firstPageOrder,
									1, 1, 1,
									//orientation, firstPageNr, "Custom", requires gui, page count, showview);
									1, 1, "custom", false, 1, false);
		//m_Doc = new ScribusDoc();
		Q_ASSERT(m_Doc!=0);
		if (!m_Doc)
			return;
		m_Doc->pageSets[1/*pagesType*/].FirstPage = 1;//firstPageOrder;
		used = false;
		qApp->changeOverrideCursor(Qt::ArrowCursor);
	}
	// tmp colors. to be removed in descrictor
	m_Doc->PageColors.insert("__blackforpreview__", ScColor(0, 0, 0, 255));
	m_Doc->PageColors.insert("__whiteforpreview__", ScColor(0, 0, 0, 0));
	m_Doc->PageColors.insert("__whiteforpreviewbg__", ScColor(0, 0, 0, 0));
	bgShade = 100;
	tmpStyle.setName("(preview temporary)");
	tmpStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
	tmpStyle.setLineSpacing((m_Doc->toolSettings.defSize / 10.0)
		* static_cast<double>(m_Doc->typographicSettings.autoLineSpacing) / 100
		+ (m_Doc->toolSettings.defSize / 10.0));
	tmpStyle.setAlignment(ParagraphStyle::Leftaligned);
	tmpStyle.setLeftMargin(0);
	tmpStyle.setFirstIndent(0);
	tmpStyle.setRightMargin(0);
	tmpStyle.setGapBefore(0);
	tmpStyle.setGapAfter(0);
	tmpStyle.charStyle().setFont(PrefsManager::instance()->appPrefs.AvailFonts[m_Doc->toolSettings.defFont]);
	tmpStyle.charStyle().setFontSize(m_Doc->toolSettings.defSize);
//	tmpStyle.tabValues().clear();
	tmpStyle.setHasDropCap(false);
	tmpStyle.setDropCapLines(0);//2;
	tmpStyle.setDropCapOffset(0);
	tmpStyle.charStyle().setFeatures(QStringList(CharStyle::INHERIT));
	tmpStyle.charStyle().setFillColor("__blackforpreview__");
	tmpStyle.charStyle().setFillShade(100); //m_Doc->toolSettings.dShade;
	tmpStyle.charStyle().setStrokeColor("__whiteforpreview__");
	tmpStyle.charStyle().setStrokeShade(100); //m_Doc->toolSettings.dShade2;
	tmpStyle.charStyle().setShadowXOffset(50);
	tmpStyle.charStyle().setShadowYOffset(-50);
	tmpStyle.charStyle().setOutlineWidth(10);
	tmpStyle.charStyle().setUnderlineOffset(0); //m_Doc->typographicSettings.valueUnderlinePos;
	tmpStyle.charStyle().setUnderlineWidth(0); //m_Doc->typographicSettings.valueUnderlineWidth;
	tmpStyle.charStyle().setStrikethruOffset(0); //m_Doc->typographicSettings.valueStrikeThruPos;
	tmpStyle.charStyle().setStrikethruWidth(0); //m_Doc->typographicSettings.valueStrikeThruPos;
	tmpStyle.charStyle().setScaleH(1000);
	tmpStyle.charStyle().setScaleV(1000);
	tmpStyle.charStyle().setBaselineOffset(0);
	tmpStyle.charStyle().setTracking(0);
}

SampleItem::~SampleItem()
{
	cleanupTemporary();
	// clean tmp document
	if (used == false)
	{
		m_Doc->setModified(false);
		//Do this manually as the sample item's doc was never put into the main windows workspace and doc list etc
		//m_Doc->scMW()->slotFileClose();
		m_Doc->view()->disconnect();
		m_Doc->WinHan->disconnect();
		m_Doc->disconnect();
		delete m_Doc->view();
		delete m_Doc->WinHan;
		delete m_Doc;
	}
}

void SampleItem::setText(QString aText)
{
	text = aText;
}

void SampleItem::setLoremIpsum(int para)
{
	LoremParser *m = new LoremParser("loremipsum.xml");
	text = m->createLorem(para);
// really needed ??
//	text = QString::fromUtf8(text);
	delete m;
}

void SampleItem::setStyle(const ParagraphStyle& aStyle)
{
	tmpStyle = aStyle;
}

void SampleItem::setBgColor(QColor c)
{
	m_Doc->PageColors["__whiteforpreviewbg__"].fromQColor(c);
}

void SampleItem::setBgShade(int c)
{
	bgShade = c;
}

void SampleItem::setBgColorMngt(bool enable)
{
	m_Doc->PageColors["__whiteforpreviewbg__"].setSpotColor(!enable);
}

void SampleItem::setTxColor(QColor c)
{
	m_Doc->PageColors["__blackforpreview__"].fromQColor(c);
}

void SampleItem::setTxShade(int c)
{
	tmpStyle.charStyle().setFillShade(c);
}

void SampleItem::setLineSpaMode(int lineSpaMode)
{
	tmpStyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(lineSpaMode));
}

void SampleItem::setLineSpa(double lineSpa)
{
	tmpStyle.setLineSpacing(lineSpa);
}

void SampleItem::setTextAlignment(int textAlignment)
{
	tmpStyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(textAlignment));
}

void SampleItem::setIndent(double indent)
{
	tmpStyle.setLeftMargin(indent);
}

void SampleItem::setFirst(double first)
{
	tmpStyle.setFirstIndent(first);
}

void SampleItem::setGapBefore(double gapBefore)
{
	tmpStyle.setGapBefore(gapBefore);
}

void SampleItem::setGapAfter(double gapAfter)
{
	tmpStyle.setGapAfter(gapAfter);
}

void SampleItem::setFont(QString font)
{
	tmpStyle.charStyle().setFont(PrefsManager::instance()->appPrefs.AvailFonts[font]);
}

void SampleItem::setFontSize(int fontSize, bool autoLineSpa)
{
	tmpStyle.charStyle().setFontSize(fontSize);
	if (autoLineSpa)
		tmpStyle.setLineSpacing(((fontSize / 10)  * (m_Doc->typographicSettings.autoLineSpacing / 100) + (fontSize / 10)));
}

/*void SampleItem::setTabValues(QValueList<PageItem::TabRecord> tabValues)
{
	tmpStyle.TabValues = tabValues;
}*/

void SampleItem::setDrop(bool drop)
{
	tmpStyle.setHasDropCap(drop);
}

void SampleItem::setDropLin(int dropLin)
{
	tmpStyle.setDropCapLines(dropLin);
}

void SampleItem::setDropDist(double dropDist)
{
	tmpStyle.setDropCapOffset(dropDist);
}

void SampleItem::setFontEffect(int fontEffect)
{
	tmpStyle.charStyle().setFeatures(static_cast<StyleFlag>(fontEffect).featureList());
}

void SampleItem::setFColor(QString fColor)
{
	tmpStyle.charStyle().setFillColor(fColor);
}

void SampleItem::setFShade(int fShade)
{
	tmpStyle.charStyle().setFillShade(fShade);
}

void SampleItem::setSColor(QString sColor)
{
	tmpStyle.charStyle().setStrokeColor(sColor);
}

void SampleItem::setSShade(int sShade)
{
	tmpStyle.charStyle().setStrokeShade(sShade);
}

void SampleItem::setBaseAdj(bool baseAdj)
{
	tmpStyle.setLineSpacingMode(baseAdj ? ParagraphStyle::BaselineGridLineSpacing : ParagraphStyle::FixedLineSpacing);
}

void SampleItem::setTxtShadowX(int txtShadowX)
{
	tmpStyle.charStyle().setShadowXOffset(txtShadowX);
}

void SampleItem::setTxtShadowY(int txtShadowY)
{
	tmpStyle.charStyle().setShadowYOffset(txtShadowY);
}

void SampleItem::setTxtOutline(int txtOutline)
{
	tmpStyle.charStyle().setOutlineWidth(txtOutline);
}

void SampleItem::setTxtUnderPos(int txtUnderPos)
{
	tmpStyle.charStyle().setUnderlineOffset(txtUnderPos);
}

void SampleItem::setTxtUnderWidth(int txtUnderWidth)
{
	tmpStyle.charStyle().setUnderlineWidth(txtUnderWidth);
}

void SampleItem::setTxtStrikePos(int txtStrikePos)
{
	tmpStyle.charStyle().setStrikethruOffset(txtStrikePos);
}

void SampleItem::setTxtStrikeWidth(int txtStrikeWidth)
{
	tmpStyle.charStyle().setStrikethruWidth(txtStrikeWidth);
}

void SampleItem::setScaleH(int scaleH)
{
	tmpStyle.charStyle().setScaleH(scaleH);
}

void SampleItem::setScaleV(int scaleV)
{
	tmpStyle.charStyle().setScaleV(scaleV);
}

void SampleItem::setBaseOff(int baseOff)
{
	tmpStyle.charStyle().setBaselineOffset(baseOff);
}

void SampleItem::setKernVal(int kernVal)
{
	tmpStyle.charStyle().setTracking(kernVal);
}

QPixmap SampleItem::getSample(int width, int height)
{
	// if it's false => the used font will be removed from used fonts
	// after sample creating
	bool previouslyUsedFont = false;

	if (tmpStyle.charStyle().font().isNone())
		return QPixmap();

	UndoManager::instance()->setUndoEnabled(false); // disable undo

	PageItem_TextFrame *previewItem = new PageItem_TextFrame(m_Doc, 0, 0, width, height, 0, "__whiteforpreviewbg__", "__whiteforpreview__");
	QImage pm(width, height, QImage::Format_ARGB32);
	ScPainter *painter = new ScPainter(&pm, width, height, 1.0, 0);
	double sca = 1.0; // original scale to set back at the end...
	int userAppMode = m_Doc->appMode; // We need to be in normal when creating/repainting items
	m_Doc->appMode = modeNormal;

	if (m_Doc->view() != NULL)
	{
		sca = m_Doc->view()->scale();
		m_Doc->view()->setScale(1.0 * PrefsManager::instance()->appPrefs.DisScale);
	}
	painter->setZoomFactor(m_Doc->view()->scale());

	if (m_Doc->UsedFonts.contains(tmpStyle.charStyle().font().scName()))
		previouslyUsedFont = true;

	m_Doc->AddFont(tmpStyle.charStyle().font().scName(), qRound(m_Doc->toolSettings.defSize / 10.0));
//	m_Doc->docParagraphStyles.create(tmpStyle);
//	int tmpIndex = m_Doc->docParagraphStyles.count() - 1;

	previewItem->FrameType = PageItem::TextFrame;
	previewItem->itemText.clear();
//	previewItem->setFont(tmpStyle.charStyle().font()->scName());
	previewItem->Cols = 1;
	text.replace(QChar(10),QChar(13)).replace(QChar(5),QChar(13));
	previewItem->itemText.insertChars(0, text);
	previewItem->itemText.setDefaultStyle(tmpStyle);
	previewItem->setFillColor("__whiteforpreviewbg__");
	previewItem->setFillShade(bgShade);
	previewItem->SetRectFrame();
	previewItem->Frame = false;
	previewItem->DrawObj(painter, QRect());
	painter->end();
	delete(painter);
	delete previewItem;

	// cleanups and resets
	if (!previouslyUsedFont)
		m_Doc->UsedFonts.remove(tmpStyle.charStyle().font().scName());
	if (m_Doc->view() != NULL)
		m_Doc->view()->setScale(sca);
	m_Doc->appMode = userAppMode;
//	m_Doc->docParagraphStyles.remove(tmpIndex);
	UndoManager::instance()->setUndoEnabled(true);
	return QPixmap::fromImage(pm);
}

void SampleItem::cleanupTemporary()
{
	// clear tmp colors
	m_Doc->PageColors.remove("__blackforpreview__");
	m_Doc->PageColors.remove("__whiteforpreview__");
	m_Doc->PageColors.remove("__whiteforpreviewbg__");
}

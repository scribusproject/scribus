/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribus.h"
#include "sampleitem.h"
#include "loremipsum.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "commonstrings.h"
#include <qcolor.h>
#include <qstring.h>

extern ScribusMainWindow* ScMW;


SampleItem::SampleItem() : QObject()
{
	used = true;
	if (ScMW->doc == NULL)
	{
		ScMW->doFileNew(//pageWidth, pageHeight,
									0,0,
									//topMargin, leftMargin, rightMargin, bottomMargin,
									1, 1, 1, 1,
									// autoframes. It's disabled in python
									// columnDistance, numberCols, autoframes,
									0, 1, false,
									//pagesType, unit, firstPageOrder,
									1, 1, 1,
									//orientation, firstPageNr, "Custom");
									1, 1, "custom", 1, false);
		ScMW->doc->pageSets[1/*pagesType*/].FirstPage = 1;//firstPageOrder;
		used = false;
	}
	doc = ScMW->doc;
	// tmp colors. to be removed in descrictor
	ScMW->doc->PageColors.insert("__blackforpreview__", ScColor(0, 0, 0, 255));
	ScMW->doc->PageColors.insert("__whiteforpreview__", ScColor(0, 0, 0, 0));
	ScMW->doc->PageColors.insert("__whiteforpreviewbg__", ScColor(0, 0, 0, 0));
	bgShade = 100;
	tmpStyle.Vname = "(preview temporary)";
	tmpStyle.LineSpaMode = 0;
	tmpStyle.LineSpa = ((doc->toolSettings.defSize / 10.0) * static_cast<double>(doc->typographicSettings.autoLineSpacing) / 100) + (doc->toolSettings.defSize / 10.0);
	tmpStyle.textAlignment = 0;
	tmpStyle.Indent = 0;
	tmpStyle.First = 0;
	tmpStyle.gapBefore = 0;
	tmpStyle.gapAfter = 0;
	tmpStyle.Font = doc->toolSettings.defFont;
	tmpStyle.FontSize = doc->toolSettings.defSize;
	tmpStyle.TabValues.clear();
	tmpStyle.Drop = false;
	tmpStyle.DropLin = 0;//2;
	tmpStyle.DropDist = 0;
	tmpStyle.FontEffect = 0;
	tmpStyle.FColor = "__blackforpreview__";
	tmpStyle.FShade = 100; //doc->toolSettings.dShade;
	tmpStyle.SColor = "__whiteforpreview__";
	tmpStyle.SShade = 100; //doc->toolSettings.dShade2;
	tmpStyle.BaseAdj = false;
	tmpStyle.txtShadowX = 50;
	tmpStyle.txtShadowY = -50;
	tmpStyle.txtOutline = 10;
	tmpStyle.txtUnderPos = 0; //doc->typographicSettings.valueUnderlinePos;
	tmpStyle.txtUnderWidth = 0; //doc->typographicSettings.valueUnderlineWidth;
	tmpStyle.txtStrikePos = 0; //doc->typographicSettings.valueStrikeThruPos;
	tmpStyle.txtStrikeWidth = 0; //doc->typographicSettings.valueStrikeThruPos;
	tmpStyle.scaleH = 1000;
	tmpStyle.scaleV = 1000;
	tmpStyle.baseOff = 0;
	tmpStyle.kernVal = 0;
}

SampleItem::~SampleItem()
{
	cleanupTemporary();
	// clean tmp document
	if (used == false)
	{
		doc->setModified(false);
		ScMW->slotFileClose();
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
	text = QString::fromUtf8(text);
	delete m;
}

void SampleItem::setStyle(ParagraphStyle aStyle)
{
	tmpStyle = aStyle;
}

void SampleItem::setBgColor(QColor c)
{
	ScMW->doc->PageColors["__whiteforpreviewbg__"].fromQColor(c);
}

void SampleItem::setBgShade(int c)
{
	bgShade = c;
}

void SampleItem::setTxColor(QColor c)
{
	ScMW->doc->PageColors["__blackforpreview__"].fromQColor(c);
}

void SampleItem::setTxShade(int c)
{
	tmpStyle.FShade = c;
}

void SampleItem::setLineSpaMode(int lineSpaMode)
{
	tmpStyle.LineSpaMode = lineSpaMode;
}

void SampleItem::setLineSpa(double lineSpa)
{
	tmpStyle.LineSpa = lineSpa;
}

void SampleItem::setTextAlignment(int textAlignment)
{
	tmpStyle.textAlignment = textAlignment;
}

void SampleItem::setIndent(double indent)
{
	tmpStyle.Indent = indent;
}

void SampleItem::setFirst(double first)
{
	tmpStyle.First = first;
}

void SampleItem::setGapBefore(double gapBefore)
{
	tmpStyle.gapBefore = gapBefore;
}

void SampleItem::setGapAfter(double gapAfter)
{
	tmpStyle.gapAfter = gapAfter;
}

void SampleItem::setFont(QString font)
{
	tmpStyle.Font = font;
}

void SampleItem::setFontSize(int fontSize, bool autoLineSpa)
{
	tmpStyle.FontSize = fontSize;
	if (autoLineSpa)
		tmpStyle.LineSpa = ((fontSize / 10)  * (doc->typographicSettings.autoLineSpacing / 100) + (fontSize / 10));
}

/*void SampleItem::setTabValues(QValueList<PageItem::TabRecord> tabValues)
{
	tmpStyle.TabValues = tabValues;
}*/

void SampleItem::setDrop(bool drop)
{
	tmpStyle.Drop = drop;
}

void SampleItem::setDropLin(int dropLin)
{
	tmpStyle.DropLin = dropLin;
}

void SampleItem::setDropDist(double dropDist)
{
	tmpStyle.DropDist = dropDist;
}

void SampleItem::setFontEffect(int fontEffect)
{
	tmpStyle.FontEffect = fontEffect;
}

void SampleItem::setFColor(QString fColor)
{
	tmpStyle.FColor = fColor;
}

void SampleItem::setFShade(int fShade)
{
	tmpStyle.FShade = fShade;
}

void SampleItem::setSColor(QString sColor)
{
	tmpStyle.SColor = sColor;
}

void SampleItem::setSShade(int sShade)
{
	tmpStyle.SShade = sShade;
}

void SampleItem::setBaseAdj(bool baseAdj)
{
	tmpStyle.BaseAdj = baseAdj;
}

void SampleItem::setTxtShadowX(int txtShadowX)
{
	tmpStyle.txtShadowX = txtShadowX;
}

void SampleItem::setTxtShadowY(int txtShadowY)
{
	tmpStyle.txtShadowY = txtShadowY;
}

void SampleItem::setTxtOutline(int txtOutline)
{
	tmpStyle.txtOutline = txtOutline;
}

void SampleItem::setTxtUnderPos(int txtUnderPos)
{
	tmpStyle.txtUnderPos = txtUnderPos;
}

void SampleItem::setTxtUnderWidth(int txtUnderWidth)
{
	tmpStyle.txtUnderWidth = txtUnderWidth;
}

void SampleItem::setTxtStrikePos(int txtStrikePos)
{
	tmpStyle.txtStrikePos = txtStrikePos;
}

void SampleItem::setTxtStrikeWidth(int txtStrikeWidth)
{
	tmpStyle.txtStrikeWidth = txtStrikeWidth;
}

void SampleItem::setScaleH(int scaleH)
{
	tmpStyle.scaleH = scaleH;
}

void SampleItem::setScaleV(int scaleV)
{
	tmpStyle.scaleV = scaleV;
}

void SampleItem::setBaseOff(int baseOff)
{
	tmpStyle.baseOff = baseOff;
}

void SampleItem::setKernVal(int kernVal)
{
	tmpStyle.kernVal = kernVal;
}

QPixmap SampleItem::getSample(int width, int height)
{
	UndoManager::instance()->setUndoEnabled(false); // disable undo

	PageItem_TextFrame *previewItem = new PageItem_TextFrame(doc, 0, 0, width, height, 0, "__whiteforpreviewbg__", "__whiteforpreview__");
	QPixmap pm(width, height);
	ScPainter *painter = new ScPainter(&pm, width, height, 0, 0);
	double sca = 1.0; // original scale to set back at the end...
	int userAppMode = ScMW->doc->appMode; // We need to be in normal when creating/repainting items
	ScMW->doc->appMode = modeNormal;

	if (ScMW->view != NULL)
	{
		sca = ScMW->view->scale();
		ScMW->view->setScale(1.0);
	}

	QFont fo = QFont(tmpStyle.Font);
	fo.setPointSize(qRound(doc->toolSettings.defSize / 10.0));
	doc->AddFont(tmpStyle.Font, qRound(doc->toolSettings.defSize / 10.0));
	doc->docParagraphStyles.append(tmpStyle);
	int tmpIndex = doc->docParagraphStyles.count() - 1;

	previewItem->FrameType = PageItem::TextFrame;
	previewItem->itemText.clear();
	previewItem->setFont(tmpStyle.Font);
	previewItem->Cols = 1;
	for (uint i = 0; i < text.length(); ++i)
	{
		ScText *hg = new ScText;
		hg->ch = text.at(i);
		if ((hg->ch == QChar(10)) || (hg->ch == QChar(5)))
			hg->ch = QChar(13);
		previewItem->itemText.append(hg);
	}

	doc->chAbStyle(previewItem, tmpIndex);
	previewItem->setFillColor("__whiteforpreviewbg__");
	previewItem->setFillShade(bgShade);
	previewItem->SetRectFrame();
	previewItem->Frame = false;
	previewItem->DrawObj(painter, QRect(0, 0, width, height));
	painter->end();
	delete(painter);
	delete previewItem;

	if (ScMW->view != NULL)
		ScMW->view->setScale(sca);
	doc->appMode = userAppMode;
	doc->docParagraphStyles.remove(doc->docParagraphStyles.fromLast());
	UndoManager::instance()->setUndoEnabled(true);
	return pm;
}

void SampleItem::cleanupTemporary()
{
	// clear tmp colors
	ScMW->doc->PageColors.remove("__blackforpreview__");
	ScMW->doc->PageColors.remove("__whiteforpreview__");
	ScMW->doc->PageColors.remove("__whiteforpreviewbg__");
}

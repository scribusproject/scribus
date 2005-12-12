#include "scribus.h"
#include "sampleitem.h"
#include "loremipsum.h"
#include <qcolor.h>
#include <qstring.h>

extern ScribusMainWindow* ScMW;


SampleItem::SampleItem(QWidget *parent) : QObject(parent)
{
	tmpStyle.Vname = "(preview temporary)";
	tmpStyle.LineSpaMode = 0;
	tmpStyle.LineSpa = ((ScMW->doc->toolSettings.defSize / 10.0) * static_cast<double>(ScMW->doc->typographicSettings.autoLineSpacing) / 100) + (ScMW->doc->toolSettings.defSize / 10.0);
	tmpStyle.textAlignment = 0;
	tmpStyle.Indent = 0;
	tmpStyle.First = 0;
	tmpStyle.gapBefore = 0;
	tmpStyle.gapAfter = 0;
	tmpStyle.Font = ScMW->doc->toolSettings.defFont;
	tmpStyle.FontSize = ScMW->doc->toolSettings.defSize;
	tmpStyle.TabValues.clear();
	tmpStyle.Drop = false;
	tmpStyle.DropLin = 2;
	tmpStyle.DropDist = 0;
	tmpStyle.FontEffect = 0;
	tmpStyle.FColor = ScMW->doc->toolSettings.dBrush;
	tmpStyle.FShade = ScMW->doc->toolSettings.dShade;
	tmpStyle.SColor = ScMW->doc->toolSettings.dPen;
	tmpStyle.SShade = ScMW->doc->toolSettings.dShade2;
	tmpStyle.BaseAdj = false;
	tmpStyle.txtShadowX = 50;
	tmpStyle.txtShadowY = -50;
	tmpStyle.txtOutline = 10;
	tmpStyle.txtUnderPos = ScMW->doc->typographicSettings.valueUnderlinePos;
	tmpStyle.txtUnderWidth = ScMW->doc->typographicSettings.valueUnderlineWidth;
	tmpStyle.txtStrikePos = ScMW->doc->typographicSettings.valueStrikeThruPos;
	tmpStyle.txtStrikeWidth = ScMW->doc->typographicSettings.valueStrikeThruPos;
	tmpStyle.scaleH = 1000;
	tmpStyle.scaleV = 1000;
	tmpStyle.baseOff = 0;
	tmpStyle.kernVal = 0;
	bgColor = QColor(255, 255, 255);
}

SampleItem::~SampleItem()
{
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
	bgColor = c;
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

void SampleItem::setFontSize(int fontSize)
{
	tmpStyle.FontSize = fontSize;
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
	PageItem_TextFrame *previewItem = new PageItem_TextFrame(ScMW->doc, 0, 0, width, height, 0, "None", ScMW->doc->toolSettings.dPenText);
	QPixmap pm(width, height);
	ScPainter *painter = new ScPainter(&pm, width, height, 0, 0);
	double sca = ScMW->view->getScale();

	QFont fo = QFont(tmpStyle.Font);
	fo.setPointSize(qRound(ScMW->doc->toolSettings.defSize / 10.0));
	ScMW->doc->AddFont(tmpStyle.Font, qRound(ScMW->doc->toolSettings.defSize / 10.0));
	ScMW->doc->docParagraphStyles.append(tmpStyle);
	int tmpIndex = ScMW->doc->docParagraphStyles.count() - 1;

	previewItem->FrameType = PageItem::TextFrame;
	painter->clear(bgColor);
	pm.fill(bgColor);
	ScMW->view->setScale(1.0);

	previewItem->itemText.clear();
	previewItem->setFont(tmpStyle.Font);
	previewItem->Cols = 1;
	for (uint i = 0; i < text.length(); ++i)
	{
		ScText *hg = new ScText;
		hg->ch = text.at(i);
		if ((hg->ch == QChar(10)) || (hg->ch == QChar(5)))
			hg->ch = QChar(13);
		hg->cfont = (*ScMW->doc->AllFonts)[tmpStyle.Font];
		hg->csize = tmpStyle.FontSize;
		hg->ccolor = tmpStyle.FColor;
		hg->cshade = tmpStyle.FShade;
		hg->cstroke = tmpStyle.SColor;
		hg->cshade2 = tmpStyle.SShade;
		hg->cscale = tmpStyle.scaleH;
		hg->cscalev = tmpStyle.scaleV;
		hg->cbase = tmpStyle.baseOff;
		hg->cshadowx = tmpStyle.txtShadowX;
		hg->cshadowy = tmpStyle.txtShadowY;
		hg->coutline = tmpStyle.txtOutline;
		hg->cunderpos = tmpStyle.txtUnderPos;
		hg->cunderwidth = tmpStyle.txtUnderWidth;
		hg->cstrikepos = tmpStyle.txtStrikePos;
		hg->cstrikewidth = tmpStyle.txtStrikeWidth;
		hg->cselect = false;
		hg->cstyle = tmpStyle.FontEffect;
		hg->cab = tmpIndex;
		hg->cextra = tmpStyle.kernVal;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		hg->cembedded = 0;
		previewItem->itemText.append(hg);
	}

	previewItem->DrawObj(painter, QRect(0, 0, width, height));
	painter->end();
	delete(painter);
	ScMW->view->setScale(sca);
	delete previewItem;
	ScMW->doc->docParagraphStyles.remove(ScMW->doc->docParagraphStyles.fromLast());
	// test pm.save("/home/subzero/devel/tests/preview.png", "PNG");
	return pm;
}

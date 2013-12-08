/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "slaoutput.h"
#include <poppler/GlobalParams.h>
#include <poppler/poppler-config.h>
#include <poppler/FileSpec.h>
#include <poppler/fofi/FoFiTrueType.h>
#include <QApplication>
#include <QFile>
#include "commonstrings.h"
#include "loadsaveplugin.h"
#include "sccolorengine.h"
#include "util.h"
#include "util_math.h"
#include <tiffio.h>

LinkSubmitForm::LinkSubmitForm(Object *actionObj)
{
	Object obj1, obj2, obj3;
	fileName = NULL;
	m_flags = 0;
	if (actionObj->isDict())
	{
		if (!actionObj->dictLookup("F", &obj1)->isNull())
		{
			if (obj1.isDict())
			{
				if (!obj1.dictLookup("FS", &obj3)->isNull())
				{
					if (obj3.isName())
					{
						char *name = obj3.getName();
						if (!strcmp(name, "URL"))
						{
							if (!obj1.dictLookup("F", &obj2)->isNull())
								fileName = obj2.getString()->copy();
						}
						obj2.free();
					}
				}
				obj3.free();
			}
		}
		obj1.free();
		if (!actionObj->dictLookup("Flags", &obj1)->isNull())
		{
			if (obj1.isNum())
				m_flags = obj1.getInt();
		}
		obj1.free();
	}
}

LinkSubmitForm::~LinkSubmitForm()
{
	if (fileName)
		delete fileName;
}

LinkImportData::LinkImportData(Object *actionObj)
{
	Object obj1, obj3;
	fileName = NULL;
	if (actionObj->isDict())
	{
		if (!actionObj->dictLookup("F", &obj1)->isNull())
		{
			if (getFileSpecNameForPlatform (&obj1, &obj3))
			{
				fileName = obj3.getString()->copy();
				obj3.free();
			}
		}
		obj1.free();
	}
}

LinkImportData::~LinkImportData()
{
	if (fileName)
		delete fileName;
}

AnoOutputDev::~AnoOutputDev()
{
	if (m_fontName)
		delete m_fontName;
	if (m_itemText)
		delete m_itemText;
}

AnoOutputDev::AnoOutputDev(ScribusDoc* doc, QStringList *importedColors)
{
	m_doc = doc;
	m_importedColors = importedColors;
	CurrColorStroke = CommonStrings::None;
	CurrColorFill = CommonStrings::None;
	CurrColorText = "Black";
	m_fontSize = 12.0;
	m_fontName = NULL;
	m_itemText = NULL;
}

void AnoOutputDev::eoFill(GfxState *state)
{
	int shade = 100;
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
}

void AnoOutputDev::fill(GfxState *state)
{
	int shade = 100;
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
}

void AnoOutputDev::stroke(GfxState *state)
{
	int shade = 100;
	CurrColorStroke = getColor(state->getStrokeColorSpace(), state->getStrokeColor(), &shade);
}

void AnoOutputDev::drawString(GfxState *state, GooString *s)
{
	int shade = 100;
	CurrColorText = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
	m_fontSize = state->getFontSize();
	if (state->getFont())
		m_fontName = state->getFont()->getName()->copy();
	m_itemText = s->copy();
}

QString AnoOutputDev::getColor(GfxColorSpace *color_space, GfxColor *color, int *shade)
{
	QString fNam;
	QString namPrefix = "FromPDF";
	ScColor tmp;
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	*shade = 100;
	if ((color_space->getMode() == csDeviceRGB) || (color_space->getMode() == csCalRGB))
	{
		GfxRGB rgb;
		color_space->getRGB(color, &rgb);
		int Rc = qRound(colToDbl(rgb.r) * 255);
		int Gc = qRound(colToDbl(rgb.g) * 255);
		int Bc = qRound(colToDbl(rgb.b) * 255);
		tmp.setColorRGB(Rc, Gc, Bc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	}
	else if (color_space->getMode() == csDeviceCMYK)
	{
		GfxCMYK cmyk;
		color_space->getCMYK(color, &cmyk);
		int Cc = qRound(colToDbl(cmyk.c) * 255);
		int Mc = qRound(colToDbl(cmyk.m) * 255);
		int Yc = qRound(colToDbl(cmyk.y) * 255);
		int Kc = qRound(colToDbl(cmyk.k) * 255);
		tmp.setColor(Cc, Mc, Yc, Kc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	}
	else if ((color_space->getMode() == csCalGray) || (color_space->getMode() == csDeviceGray))
	{
		GfxGray gray;
		color_space->getGray(color, &gray);
		int Kc = 255 - qRound(colToDbl(gray) * 255);
		tmp.setColor(0, 0, 0, Kc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	}
	else if (color_space->getMode() == csSeparation)
	{
		GfxCMYK cmyk;
		color_space->getCMYK(color, &cmyk);
		int Cc = qRound(colToDbl(cmyk.c) * 255);
		int Mc = qRound(colToDbl(cmyk.m) * 255);
		int Yc = qRound(colToDbl(cmyk.y) * 255);
		int Kc = qRound(colToDbl(cmyk.k) * 255);
		tmp.setColor(Cc, Mc, Yc, Kc);
		tmp.setSpotColor(true);
		QString nam = QString(((GfxSeparationColorSpace*)color_space)->getName()->getCString());
		fNam = m_doc->PageColors.tryAddColor(nam, tmp);
		*shade = qRound(colToDbl(color->c[0]) * 100);
	}
	else
	{
		GfxRGB rgb;
		color_space->getRGB(color, &rgb);
		int Rc = qRound(colToDbl(rgb.r) * 255);
		int Gc = qRound(colToDbl(rgb.g) * 255);
		int Bc = qRound(colToDbl(rgb.b) * 255);
		tmp.setColorRGB(Rc, Gc, Bc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	//	qDebug() << "update fill color other colorspace" << color_space->getMode() << "treating as rgb" << Rc << Gc << Bc;
	}
	if (fNam == namPrefix+tmp.name())
		m_importedColors->append(fNam);
	return fNam;
}

SlaOutputDev::SlaOutputDev(ScribusDoc* doc, QList<PageItem*> *Elements, QStringList *importedColors, int flags)
{
	m_doc = doc;
	m_Elements = Elements;
	m_groupStack.clear();
	pushGroup();
	m_clipStack.clear();
	m_currentMask = "";
	m_importedColors = importedColors;
	CurrColorStroke = "Black";
	CurrColorFill = "Black";
	Coords = "";
	pathIsClosed = false;
	tmpSel = new Selection(m_doc, false);
	grStackDepth = 0;
	firstLayer = true;
	layerNum = 1;
	importerFlags = flags;
	currentLayer = m_doc->activeLayer();
	xref = NULL;
	m_fontEngine = 0;
	m_font = 0;
	updateGUICounter = 0;
	layersSetByOCG = false;
	cropOffsetX = 0;
	cropOffsetY = 0;
}

SlaOutputDev::~SlaOutputDev()
{
	m_groupStack.clear();
	tmpSel->clear();
	delete tmpSel;
	delete m_fontEngine;
}

/* get Actions not implemented by Poppler */
LinkAction* SlaOutputDev::SC_getAction(AnnotWidget *ano)
{
	LinkAction *linkAction = NULL;
	Object obj;
	Ref refa = ano->getRef();
	Object additionalActions;
	Object *act = xref->fetch(refa.num, refa.gen, &obj);
	if (act)
	{
		if (act->isDict())
		{
			Dict* adic = act->getDict();
			adic->lookupNF("A", &additionalActions);
			Object additionalActionsObject;
			if (additionalActions.fetch(pdfDoc->getXRef(), &additionalActionsObject)->isDict())
			{
				Object actionObject;
				additionalActionsObject.dictLookup("S", &actionObject);
				if (actionObject.isName("ImportData"))
				{
					linkAction = new LinkImportData(&additionalActionsObject);
				}
				else if (actionObject.isName("SubmitForm"))
				{
					linkAction = new LinkSubmitForm(&additionalActionsObject);
				}
				actionObject.free();
			}
			additionalActionsObject.free();
			additionalActions.free();
		}
	}
	obj.free();
	return linkAction;
}

/* Replacement for the crippled Poppler function LinkAction* AnnotWidget::getAdditionalAction(AdditionalActionsType type) */
LinkAction* SlaOutputDev::SC_getAdditionalAction(const char *key, AnnotWidget *ano)
{
	LinkAction *linkAction = NULL;
	Object obj;
	Ref refa = ano->getRef();
	Object additionalActions;
	Object *act = xref->fetch(refa.num, refa.gen, &obj);
	if (act)
	{
		if (act->isDict())
		{
			Dict* adic = act->getDict();
			adic->lookupNF("AA", &additionalActions);
			Object additionalActionsObject;
			if (additionalActions.fetch(pdfDoc->getXRef(), &additionalActionsObject)->isDict())
			{
				Object actionObject;
				if (additionalActionsObject.dictLookup(key, &actionObject)->isDict())
					linkAction = LinkAction::parseAction(&actionObject, pdfDoc->getCatalog()->getBaseURI());
				actionObject.free();
			}
			additionalActionsObject.free();
			additionalActions.free();
		}
	}
	obj.free();
	return linkAction;
}

GBool SlaOutputDev::annotations_callback(Annot *annota, void *user_data)
{
	SlaOutputDev *dev = (SlaOutputDev*)user_data;
	PDFRectangle *box = annota->getRect();
	double xCoor = dev->m_doc->currentPage()->xOffset() + box->x1 - dev->cropOffsetX;
	double yCoor = dev->m_doc->currentPage()->yOffset() + dev->m_doc->currentPage()->height() - box->y2 + dev->cropOffsetY;
	double width = box->x2 - box->x1;
	double height = box->y2 - box->y1;
	if (dev->rotate == 90)
	{
		xCoor = dev->m_doc->currentPage()->xOffset() - dev->cropOffsetX + box->y2;
		yCoor = dev->m_doc->currentPage()->yOffset() + dev->cropOffsetY + box->x1;
	}
	else if (dev->rotate == 180)
	{
		xCoor = dev->m_doc->currentPage()->xOffset() - dev->cropOffsetX + dev->m_doc->currentPage()->width() - box->x1;
		yCoor = dev->m_doc->currentPage()->yOffset() + dev->cropOffsetY + box->y2;
	}
	else if (dev->rotate == 270)
	{
		xCoor = dev->m_doc->currentPage()->xOffset() - dev->cropOffsetX + dev->m_doc->currentPage()->width() - box->y2;
		yCoor = dev->m_doc->currentPage()->yOffset() + dev->cropOffsetY + dev->m_doc->currentPage()->height() - box->x1;
	}
	bool retVal = true;
	if (annota->getType() == Annot::typeText)
		retVal = !dev->handleTextAnnot(annota, xCoor, yCoor, width, height);
	else if (annota->getType() == Annot::typeLink)
		retVal = !dev->handleLinkAnnot(annota, xCoor, yCoor, width, height);
	else if (annota->getType() == Annot::typeWidget)
		retVal = !dev->handleWidgetAnnot(annota, xCoor, yCoor, width, height);
	return retVal;
}

bool SlaOutputDev::handleTextAnnot(Annot* annota, double xCoor, double yCoor, double width, double height)
{
	AnnotText *anl = (AnnotText*)annota;
	int z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, xCoor, yCoor, width, height, 0, CommonStrings::None, CommonStrings::None, true);
	PageItem *ite = m_doc->Items->at(z);
	int flg = annota->getFlags();
	if (!(flg & 16))
		ite->setRotation(rotate, true);
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillEvenOdd(false);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->ContourLine = ite->PoLine.copy();
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	ite->setIsAnnotation(true);
	ite->AutoName = false;
	ite->annotation().setType(Annotation::Text);
	ite->annotation().setActionType(Annotation::Action_None);
	ite->annotation().setAnOpen(anl->getOpen());
	QString iconName = UnicodeParsedString(anl->getIcon());
	if (iconName == "Note")
		ite->annotation().setIcon(Annotation::Icon_Note);
	else if (iconName == "Comment")
		ite->annotation().setIcon(Annotation::Icon_Comment);
	else if (iconName == "Key")
		ite->annotation().setIcon(Annotation::Icon_Key);
	else if (iconName == "Help")
		ite->annotation().setIcon(Annotation::Icon_Help);
	else if (iconName == "NewParagraph")
		ite->annotation().setIcon(Annotation::Icon_NewParagraph);
	else if (iconName == "Paragraph")
		ite->annotation().setIcon(Annotation::Icon_Paragraph);
	else if (iconName == "Insert")
		ite->annotation().setIcon(Annotation::Icon_Insert);
	else if (iconName == "Cross")
		ite->annotation().setIcon(Annotation::Icon_Cross);
	else if (iconName == "Circle")
		ite->annotation().setIcon(Annotation::Icon_Circle);
	else
		ite->annotation().setIcon(Annotation::Icon_Note);
	ite->setItemName( CommonStrings::itemName_TextAnnotation + QString("%1").arg(m_doc->TotalItems));
	ite->itemText.insertChars(UnicodeParsedString(annota->getContents()));
	return true;
}

bool SlaOutputDev::handleLinkAnnot(Annot* annota, double xCoor, double yCoor, double width, double height)
{
	AnnotLink *anl = (AnnotLink*)annota;
	LinkAction *act = anl->getAction();
	if (!act)
		return false;
	bool validLink = false;
	int pagNum = 0;
	int xco = 0;
	int yco = 0;
	QString fileName = "";
	if (act->getKind() == actionGoTo)
	{
		LinkGoTo *gto = (LinkGoTo*)act;
		LinkDest *dst = gto->getDest();
		if (dst)
		{
			if (dst->getKind() == destXYZ)
			{
				if (dst->isPageRef())
				{
					Ref dstr = dst->getPageRef();
					pagNum = pdfDoc->findPage(dstr.num, dstr.gen);
				}
				else
					pagNum = dst->getPageNum();
				xco = dst->getLeft();
				yco = dst->getTop();
				validLink = true;
			}
		}
		else
		{
			GooString *ndst = gto->getNamedDest();
			if (ndst)
			{
				LinkDest *dstn = pdfDoc->findDest(ndst);
				if (dstn)
				{
					if (dstn->getKind() == destXYZ)
					{
						if (dstn->isPageRef())
						{
							Ref dstr = dstn->getPageRef();
							pagNum = pdfDoc->findPage(dstr.num, dstr.gen);
						}
						else
							pagNum = dstn->getPageNum();
						xco = dstn->getLeft();
						yco = dstn->getTop();
						validLink = true;
					}
				}
			}
		}
	}
	else if (act->getKind() == actionGoToR)
	{
		LinkGoToR *gto = (LinkGoToR*)act;
		fileName = UnicodeParsedString(gto->getFileName());
		LinkDest *dst = gto->getDest();
		if (dst)
		{
			if (dst->getKind() == destXYZ)
			{
				pagNum = dst->getPageNum();
				xco = dst->getLeft();
				yco = dst->getTop();
				validLink = true;
			}
		}
		else
		{
			GooString *ndst = gto->getNamedDest();
			if (ndst)
			{
				LinkDest *dstn = pdfDoc->findDest(ndst);
				if (dstn)
				{
					if (dstn->getKind() == destXYZ)
					{
						pagNum = dstn->getPageNum();
						xco = dstn->getLeft();
						yco = dstn->getTop();
						validLink = true;
					}
				}
			}
		}
	}
	else if (act->getKind() == actionURI)
	{
		LinkURI *gto = (LinkURI*)act;
		validLink = true;
		fileName = UnicodeParsedString(gto->getURI());
	}
	if (validLink)
	{
		int z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, xCoor, yCoor, width, height, 0, CommonStrings::None, CommonStrings::None, true);
		PageItem *ite = m_doc->Items->at(z);
		int flg = annota->getFlags();
		if (!(flg & 16))
			ite->setRotation(rotate, true);
		ite->ClipEdited = true;
		ite->FrameType = 3;
		ite->setFillEvenOdd(false);
		ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
		ite->ContourLine = ite->PoLine.copy();
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_Elements->append(ite);
		if (m_groupStack.count() != 0)
		{
			m_groupStack.top().Items.append(ite);
			applyMask(ite);
		}
		ite->setIsAnnotation(true);
		ite->AutoName = false;
		if (act->getKind() == actionGoTo)
		{
			ite->annotation().setZiel(pagNum - 1);
			ite->annotation().setAction(QString("%1 %2").arg(xco).arg(yco));
			ite->annotation().setActionType(2);
		}
		else if (act->getKind() == actionGoToR)
		{
			ite->annotation().setZiel(pagNum - 1);
			ite->annotation().setExtern(fileName);
			ite->annotation().setAction(QString("%1 %2").arg(xco).arg(yco));
			ite->annotation().setActionType(9);
		}
		else if (act->getKind() == actionURI)
		{
			ite->annotation().setAction("");
			ite->annotation().setExtern(fileName);
			ite->annotation().setActionType(8);
		}
		ite->annotation().setType(Annotation::Link);
		ite->setItemName( CommonStrings::itemName_LinkAnnotation + QString("%1").arg(m_doc->TotalItems));
	}
	return validLink;
}

bool SlaOutputDev::handleWidgetAnnot(Annot* annota, double xCoor, double yCoor, double width, double height)
{
	bool retVal = false;
	bool found = false;
	int formcount = m_formWidgets->getNumWidgets();
	for (int i = 0; i < formcount; ++i)
	{
		FormWidget *fm = m_formWidgets->getWidget(i);
		if (fm)
		{
			AnnotWidget *ano = fm->getWidgetAnnotation();
			if (ano)
			{
				if (ano == (AnnotWidget*)annota)
				{
					found = true;
					int wtyp = -1;
					if (fm->getType() == formButton)
					{
						FormWidgetButton *btn = (FormWidgetButton*)fm;
						if (btn)
						{
							if (btn->getButtonType() == formButtonCheck)
							{
								wtyp = Annotation::Checkbox;
								retVal = true;
							}
							else if (btn->getButtonType() == formButtonPush)
							{
								wtyp = Annotation::Button;
								retVal = true;
							}
							else if (btn->getButtonType() == formButtonRadio)
							{
								wtyp = Annotation::RadioButton;
								retVal = true;
							}
						}
					}
					else if (fm->getType() == formText)
					{
						wtyp = Annotation::Textfield;
						retVal = true;
					}
					else if (fm->getType() == formChoice)
					{
						FormWidgetChoice *btn = (FormWidgetChoice*)fm;
						if (btn)
						{
							if (btn->isCombo())
							{
								wtyp = Annotation::Combobox;
								retVal = true;
							}
							else if (btn->isListBox())
							{
								wtyp = Annotation::Listbox;
								retVal = true;
							}
						}
					}
					if (retVal)
					{
						AnnotAppearanceCharacs *achar = ano->getAppearCharacs();
						bool fgFound = false;
						bool bgFound = false;
						if (achar)
						{
							AnnotColor *bgCol = achar->getBackColor();
							if (bgCol)
							{
								bgFound = true;
								CurrColorFill = getAnnotationColor(bgCol);
							}
							else
								CurrColorFill = CommonStrings::None;
							AnnotColor *fgCol = achar->getBorderColor();
							if (fgCol)
							{
								fgFound = true;
								CurrColorStroke = getAnnotationColor(fgCol);
							}
							else
							{
								fgCol = achar->getBackColor();
								if (fgCol)
									CurrColorStroke = getAnnotationColor(fgCol);
								else
									CurrColorStroke = CommonStrings::None;
							}
						}
						QString CurrColorText = "Black";
						double fontSize = 12;
						QString fontName = "";
						QString itemText = "";
						AnnotAppearance *apa = annota->getAppearStreams();
						if (apa || !achar)
						{
							AnoOutputDev *Adev = new AnoOutputDev(m_doc, m_importedColors);
							Gfx *gfx;
#ifdef POPPLER_VERSION
							gfx = new Gfx(pdfDoc, Adev, pdfDoc->getPage(m_actPage)->getResourceDict(), annota->getRect(), NULL);
#else
							gfx = new Gfx(xref, Adev, pdfDoc->getPage(m_actPage)->getResourceDict(), catalog, annota->getRect(), NULL);
#endif
							ano->draw(gfx, false);
							if (!bgFound)
								CurrColorFill = Adev->CurrColorFill;
							if (!fgFound)
								CurrColorStroke = Adev->CurrColorStroke;
							CurrColorText = Adev->CurrColorText;
							fontSize = Adev->m_fontSize;
							fontName = UnicodeParsedString(Adev->m_fontName);
							itemText = UnicodeParsedString(Adev->m_itemText);
							delete gfx;
							delete Adev;
						}
						int z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, xCoor, yCoor, width, height, 0, CurrColorFill, CommonStrings::None, true);
						PageItem *ite = m_doc->Items->at(z);
						int flg = annota->getFlags();
						if (!(flg & 16))
							ite->setRotation(rotate, true);
						ite->ClipEdited = true;
						ite->FrameType = 3;
						ite->setFillEvenOdd(false);
						ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
						ite->ContourLine = ite->PoLine.copy();
						ite->setTextFlowMode(PageItem::TextFlowDisabled);
						m_Elements->append(ite);
						if (m_groupStack.count() != 0)
						{
							m_groupStack.top().Items.append(ite);
							applyMask(ite);
						}
						ite->setIsAnnotation(true);
						ite->AutoName = false;
						AnnotBorder *brd = annota->getBorder();
						if (brd)
						{
							int bsty = brd->getStyle();
							if (bsty == AnnotBorder::borderDashed)
								bsty = 1;
							else if (bsty == AnnotBorder::borderBeveled)
								bsty = 3;
							else if (bsty == AnnotBorder::borderInset)
								bsty = 4;
							else if (bsty == AnnotBorder::borderUnderlined)
								bsty = 2;
							ite->annotation().setBsty(bsty);
							ite->annotation().setBorderColor(CurrColorStroke);
							ite->annotation().setBwid(qRound(brd->getWidth()));
						}
						else
						{
							ite->annotation().setBsty(0);
							ite->annotation().setBorderColor(CommonStrings::None);
							ite->annotation().setBwid(0);
						}
						QString tmTxt = "";
						tmTxt = UnicodeParsedString(fm->getPartialName());
						if (!tmTxt.isEmpty())
							ite->setItemName(tmTxt);
						tmTxt = "";
						tmTxt = UnicodeParsedString(fm->getAlternateUiName());
						if (!tmTxt.isEmpty())
							ite->annotation().setToolTip(tmTxt);
						tmTxt = "";
						if (achar)
						{
							tmTxt = UnicodeParsedString(achar->getRolloverCaption());
							if (!tmTxt.isEmpty())
								ite->annotation().setRollOver(tmTxt);
							tmTxt = "";
							tmTxt = UnicodeParsedString(achar->getAlternateCaption());
							if (!tmTxt.isEmpty())
								ite->annotation().setDown(tmTxt);
						}
						ite->annotation().setType(wtyp);
						ite->annotation().setFlag(0);
						if (flg & 2)
							ite->annotation().setVis(1);
						if (flg & 32)
							ite->annotation().setVis(3);
						if (wtyp == Annotation::Button)
						{
							ite->setFillColor(CurrColorFill);
							if (achar)
								ite->itemText.insertChars(UnicodeParsedString(achar->getNormalCaption()));
							else
								ite->itemText.insertChars(itemText);
							applyTextStyle(ite, fontName, CurrColorText, fontSize);
							ite->annotation().addToFlag(Annotation::Flag_PushButton);
							FormWidgetButton *btn = (FormWidgetButton*)fm;
							if (!btn->isReadOnly())
								ite->annotation().addToFlag(Annotation::Flag_Edit);
							handleActions(ite, ano);
						}
						else if (wtyp == Annotation::Textfield)
						{
							FormWidgetText *btn = (FormWidgetText*)fm;
							if (btn)
							{
								ite->itemText.insertChars(UnicodeParsedString(btn->getContent()));
								applyTextStyle(ite, fontName, CurrColorText, fontSize);
								if (btn->isMultiline())
									ite->annotation().addToFlag(Annotation::Flag_Multiline);
								if (btn->isPassword())
									ite->annotation().addToFlag(Annotation::Flag_Password);
								if (btn->noSpellCheck())
									ite->annotation().addToFlag(Annotation::Flag_DoNotSpellCheck);
								if (btn->noScroll())
									ite->annotation().addToFlag(Annotation::Flag_DoNotScroll);
								int mxLen = btn->getMaxLen();
								if (mxLen > 0)
									ite->annotation().setMaxChar(mxLen);
								else
									ite->annotation().setMaxChar(-1);
								if (!btn->isReadOnly())
									ite->annotation().addToFlag(Annotation::Flag_Edit);
								handleActions(ite, ano);
							}
						}
						else if (wtyp == Annotation::Checkbox)
						{
							FormWidgetButton *btn = (FormWidgetButton*)fm;
							if (btn)
							{
								ite->annotation().setIsChk(btn->getState());
								ite->annotation().setCheckState(ite->annotation().IsChk());
								handleActions(ite, ano);
								if (itemText == "4")
									ite->annotation().setChkStil(0);
								else if (itemText == "5")
									ite->annotation().setChkStil(1);
								else if (itemText == "F")
									ite->annotation().setChkStil(2);
								else if (itemText == "l")
									ite->annotation().setChkStil(3);
								else if (itemText == "H")
									ite->annotation().setChkStil(4);
								else if (itemText == "n")
									ite->annotation().setChkStil(5);
								else
									ite->annotation().setChkStil(0);
								if (!btn->isReadOnly())
									ite->annotation().addToFlag(Annotation::Flag_Edit);
							}
						}
						else if ((wtyp == Annotation::Combobox) || (wtyp == Annotation::Listbox))
						{
							FormWidgetChoice *btn = (FormWidgetChoice*)fm;
							if (btn)
							{
								if (wtyp == 5)
									ite->annotation().addToFlag(Annotation::Flag_Combo);
								int co = btn->getNumChoices();
								if (co > 0)
								{
									QString inh = UnicodeParsedString(btn->getChoice(0));
									for (int a = 1; a < co; a++)
									{
										inh += "\n" + UnicodeParsedString(btn->getChoice(a));
									}
									ite->itemText.insertChars(inh);
								}
								applyTextStyle(ite, fontName, CurrColorText, fontSize);
								if (!btn->isReadOnly())
									ite->annotation().addToFlag(Annotation::Flag_Edit);
								handleActions(ite, ano);
							}
						}
						else if (wtyp == Annotation::RadioButton)
						{
							FormWidgetButton *btn = (FormWidgetButton*)fm;
							if (btn)
							{
								ite->setItemName( CommonStrings::itemName_RadioButton + QString("%1").arg(m_doc->TotalItems));
								ite->annotation().setIsChk(btn->getState());
								ite->annotation().setCheckState(ite->annotation().IsChk());
								handleActions(ite, ano);
								m_radioButtons.insert(annota->getRef().num, ite);
							}
						}
					}
					break;
				}
			}
		}
	}
	if (!found)
	{
		Object obj1;
		Ref refa = annota->getRef();
		Object *act = xref->fetch(refa.num, refa.gen, &obj1);
		if (act)
		{
			if (act->isDict())
			{
				Dict* dict = act->getDict();
				Object obj2;
				//childs
				if (dict->lookup("Kids", &obj2)->isArray())
				{
				  // Load children
					QList<int> radList;
					for (int i = 0 ; i < obj2.arrayGetLength(); i++)
					{
						Object childRef, childObj;
						if (!obj2.arrayGetNF(i, &childRef)->isRef())
						{
							childRef.free();
							continue;
						}
						if (!obj2.arrayGet(i, &childObj)->isDict())
						{
							childObj.free();
							childRef.free();
							continue;
						}
						const Ref ref = childRef.getRef();
						radList.append(ref.num);
						childObj.free();
						childRef.free();
					}
					QString tmTxt = UnicodeParsedString(annota->getName());
					m_radioMap.insert(tmTxt, radList);
				}
				obj2.free();
			}
		}
		obj1.free();
	}
	return retVal;
}

void SlaOutputDev::applyTextStyle(PageItem* ite, QString fontName, QString textColor, double fontSize)
{
	CharStyle newStyle;
	newStyle.setFillColor(textColor);
	newStyle.setFontSize(fontSize * 10);
	if (!fontName.isEmpty())
	{
		SCFontsIterator it(*m_doc->AllFonts);
		for ( ; it.hasNext() ; it.next())
		{
			ScFace& face(it.current());
			if ((face.psName() == fontName) && (face.usable()) && (face.type() == ScFace::TTF))
			{
				newStyle.setFont(face);
				break;
			}
			else if ((face.family() == fontName) && (face.usable()) && (face.type() == ScFace::TTF))
			{
				newStyle.setFont(face);
				break;
			}
			else if ((face.scName() == fontName) && (face.usable()) && (face.type() == ScFace::TTF))
			{
				newStyle.setFont(face);
				break;
			}
		}
	}
	ParagraphStyle dstyle(ite->itemText.defaultStyle());
	dstyle.charStyle().applyCharStyle(newStyle);
	ite->itemText.setDefaultStyle(dstyle);
	ite->itemText.applyCharStyle(0, ite->itemText.length(), newStyle);
	ite->invalid = true;
}

void SlaOutputDev::handleActions(PageItem* ite, AnnotWidget *ano)
{
	LinkAction *Lact = ano->getAction();
	if (Lact)
	{
		if (Lact->getKind() == actionJavaScript)
		{
			LinkJavaScript *jsa = (LinkJavaScript*)Lact;
			if (jsa->isOk())
			{
				ite->annotation().setActionType(1);
				ite->annotation().setAction(UnicodeParsedString(jsa->getScript()));
			}
		}
		else if (Lact->getKind() == actionGoTo)
		{
			int pagNum = 0;
			int xco = 0;
			int yco = 0;
			LinkGoTo *gto = (LinkGoTo*)Lact;
			LinkDest *dst = gto->getDest();
			if (dst)
			{
				if (dst->getKind() == destXYZ)
				{
					if (dst->isPageRef())
					{
						Ref dstr = dst->getPageRef();
						pagNum = pdfDoc->findPage(dstr.num, dstr.gen);
					}
					else
						pagNum = dst->getPageNum();
					xco = dst->getLeft();
					yco = dst->getTop();
					ite->annotation().setZiel(pagNum - 1);
					ite->annotation().setAction(QString("%1 %2").arg(xco).arg(yco));
					ite->annotation().setActionType(2);
				}
			}
			else
			{
				GooString *ndst = gto->getNamedDest();
				if (ndst)
				{
					LinkDest *dstn = pdfDoc->findDest(ndst);
					if (dstn)
					{
						if (dstn->getKind() == destXYZ)
						{
							if (dstn->isPageRef())
							{
								Ref dstr = dstn->getPageRef();
								pagNum = pdfDoc->findPage(dstr.num, dstr.gen);
							}
							else
								pagNum = dstn->getPageNum();
							xco = dstn->getLeft();
							yco = dstn->getTop();
							ite->annotation().setZiel(pagNum - 1);
							ite->annotation().setAction(QString("%1 %2").arg(xco).arg(yco));
							ite->annotation().setActionType(2);
						}
					}
				}
			}
		}
		else if (Lact->getKind() == actionGoToR)
		{
			int pagNum = 0;
			int xco = 0;
			int yco = 0;
			LinkGoToR *gto = (LinkGoToR*)Lact;
			QString fileName = UnicodeParsedString(gto->getFileName());
			LinkDest *dst = gto->getDest();
			if (dst)
			{
				if (dst->getKind() == destXYZ)
				{
					pagNum = dst->getPageNum();
					xco = dst->getLeft();
					yco = dst->getTop();
					ite->annotation().setZiel(pagNum - 1);
					ite->annotation().setExtern(fileName);
					ite->annotation().setAction(QString("%1 %2").arg(xco).arg(yco));
					ite->annotation().setActionType(9);
				}
			}
			else
			{
				GooString *ndst = gto->getNamedDest();
				if (ndst)
				{
					LinkDest *dstn = pdfDoc->findDest(ndst);
					if (dstn)
					{
						if (dstn->getKind() == destXYZ)
						{
							pagNum = dstn->getPageNum();
							xco = dstn->getLeft();
							yco = dstn->getTop();
							ite->annotation().setZiel(pagNum - 1);
							ite->annotation().setExtern(fileName);
							ite->annotation().setAction(QString("%1 %2").arg(xco).arg(yco));
							ite->annotation().setActionType(9);
						}
					}
				}
			}
		}
		else if (Lact->getKind() == actionUnknown)
		{
			LinkUnknown *uno = (LinkUnknown*)Lact;
			QString actString = UnicodeParsedString(uno->getAction());
			if (actString == "ResetForm")
			{
				ite->annotation().setActionType(4);
			}
			else
			{
				LinkAction* scact = SC_getAction(ano);
				if (scact)
				{
					if (actString == "ImportData")
					{
						LinkImportData *impo = (LinkImportData*)scact;
						if (impo->isOk())
						{
							ite->annotation().setActionType(5);
							ite->annotation().setAction(UnicodeParsedString(impo->getFileName()));
						}
					}
					else if (actString == "SubmitForm")
					{
						LinkSubmitForm *impo = (LinkSubmitForm*)scact;
						if (impo->isOk())
						{
							ite->annotation().setActionType(3);
							ite->annotation().setAction(UnicodeParsedString(impo->getFileName()));
							int fl = impo->getFlags();
							if (fl == 0)
								ite->annotation().setHTML(0);
							else if (fl == 4)
								ite->annotation().setHTML(1);
							else if (fl == 64)
								ite->annotation().setHTML(2);
							else if (fl == 512)
								ite->annotation().setHTML(3);
						}
					}
				}
			}
		}
		else if (Lact->getKind() == actionNamed)
		{
			LinkNamed *uno = (LinkNamed*)Lact;
			ite->annotation().setActionType(10);
			ite->annotation().setAction(UnicodeParsedString(uno->getName()));
		}
		else
			qDebug() << "Found unsupported Action of type" << Lact->getKind();
	}
	LinkAction *Aact = SC_getAdditionalAction("D", ano);
	if (Aact)
	{
		if (Aact->getKind() == actionJavaScript)
		{
			LinkJavaScript *jsa = (LinkJavaScript*)Aact;
			if (jsa->isOk())
			{
				ite->annotation().setD_act(UnicodeParsedString(jsa->getScript()));
				ite->annotation().setAAact(true);
			}
		}
		Aact = NULL;
	}
	Aact = SC_getAdditionalAction("E", ano);
	if (Aact)
	{
		if (Aact->getKind() == actionJavaScript)
		{
			LinkJavaScript *jsa = (LinkJavaScript*)Aact;
			if (jsa->isOk())
			{
				ite->annotation().setE_act(UnicodeParsedString(jsa->getScript()));
				ite->annotation().setAAact(true);
			}
		}
		Aact = NULL;
	}
	Aact = SC_getAdditionalAction("X", ano);
	if (Aact)
	{
		if (Aact->getKind() == actionJavaScript)
		{
			LinkJavaScript *jsa = (LinkJavaScript*)Aact;
			if (jsa->isOk())
			{
				ite->annotation().setX_act(UnicodeParsedString(jsa->getScript()));
				ite->annotation().setAAact(true);
			}
		}
		Aact = NULL;
	}
	Aact = SC_getAdditionalAction("Fo", ano);
	if (Aact)
	{
		if (Aact->getKind() == actionJavaScript)
		{
			LinkJavaScript *jsa = (LinkJavaScript*)Aact;
			if (jsa->isOk())
			{
				ite->annotation().setFo_act(UnicodeParsedString(jsa->getScript()));
				ite->annotation().setAAact(true);
			}
		}
		Aact = NULL;
	}
	Aact = SC_getAdditionalAction("Bl", ano);
	if (Aact)
	{
		if (Aact->getKind() == actionJavaScript)
		{
			LinkJavaScript *jsa = (LinkJavaScript*)Aact;
			if (jsa->isOk())
			{
				ite->annotation().setBl_act(UnicodeParsedString(jsa->getScript()));
				ite->annotation().setAAact(true);
			}
		}
		Aact = NULL;
	}
	Aact = SC_getAdditionalAction("C", ano);
	if (Aact)
	{
		if (Aact->getKind() == actionJavaScript)
		{
			LinkJavaScript *jsa = (LinkJavaScript*)Aact;
			if (jsa->isOk())
			{
				ite->annotation().setC_act(UnicodeParsedString(jsa->getScript()));
				ite->annotation().setAAact(true);
			}
		}
		Aact = NULL;
	}
	Aact = SC_getAdditionalAction("F", ano);
	if (Aact)
	{
		if (Aact->getKind() == actionJavaScript)
		{
			LinkJavaScript *jsa = (LinkJavaScript*)Aact;
			if (jsa->isOk())
			{
				ite->annotation().setF_act(UnicodeParsedString(jsa->getScript()));
				ite->annotation().setAAact(true);
				ite->annotation().setFormat(5);
			}
		}
		Aact = NULL;
	}
	Aact = SC_getAdditionalAction("K", ano);
	if (Aact)
	{
		if (Aact->getKind() == actionJavaScript)
		{
			LinkJavaScript *jsa = (LinkJavaScript*)Aact;
			if (jsa->isOk())
			{
				ite->annotation().setK_act(UnicodeParsedString(jsa->getScript()));
				ite->annotation().setAAact(true);
				ite->annotation().setFormat(5);
			}
		}
		Aact = NULL;
	}
	Aact = SC_getAdditionalAction("V", ano);
	if (Aact)
	{
		if (Aact->getKind() == actionJavaScript)
		{
			LinkJavaScript *jsa = (LinkJavaScript*)Aact;
			if (jsa->isOk())
			{
				ite->annotation().setV_act(UnicodeParsedString(jsa->getScript()));
				ite->annotation().setAAact(true);
			}
		}
		Aact = NULL;
	}
}

void SlaOutputDev::startDoc(PDFDoc *doc, XRef *xrefA, Catalog *catA)
{
	xref = xrefA;
	catalog = catA;
	pdfDoc = doc;
	updateGUICounter = 0;
	m_fontEngine = new SplashFontEngine(
#if HAVE_T1LIB_H
	globalParams->getEnableT1lib(),
#endif
#if HAVE_FREETYPE_H
	globalParams->getEnableFreeType(),
	true,
	true,
#endif
	true);
}

void SlaOutputDev::startPage(int pageNum, GfxState *)
{
	m_formWidgets = pdfDoc->getPage(pageNum)->getFormWidgets();
	m_radioMap.clear();
	m_radioButtons.clear();
	m_actPage = pageNum;
}

void SlaOutputDev::endPage()
{
	if (!m_radioMap.isEmpty())
	{
		QHash<QString, QList<int> >::iterator it;
		for (it = m_radioMap.begin(); it != m_radioMap.end(); ++it)
		{
			tmpSel->clear();
			QList<int> refList = it.value();
			for (int a = 0; a < refList.count(); a++)
			{
				if (m_radioButtons.contains(refList[a]))
				{
					tmpSel->addItem(m_radioButtons[refList[a]], true);
					m_Elements->removeAll(m_radioButtons[refList[a]]);
				}
			}
			if (!tmpSel->isEmpty())
			{
				PageItem *ite = m_doc->groupObjectsSelection(tmpSel);
				ite->setItemName(it.key());
				m_Elements->append(ite);
				if (m_groupStack.count() != 0)
				{
					m_groupStack.top().Items.append(ite);
				}
			}
		}
	}
	m_radioMap.clear();
	m_radioButtons.clear();
//	qDebug() << "ending page";
}

void SlaOutputDev::saveState(GfxState *state)
{
//	qDebug() << "saveState" << grStackDepth;
	grStackDepth++;
}

void SlaOutputDev::restoreState(GfxState *state)
{
//	qDebug() << "restoreState" << grStackDepth;
	grStackDepth--;
	if (m_groupStack.count() != 0)
		m_groupStack.top().maskName = "";
	if (m_clipStack.count() != 0)
	{
		while (m_clipStack.top().grStackDepth > grStackDepth)
		{
			clipEntry clp = m_clipStack.pop();
			PageItem *ite = clp.ClipItem;
			if (m_groupStack.count() != 0)
			{
				groupEntry gElements = m_groupStack.pop();
				if (gElements.Items.count() > 0)
				{
					for (int d = 0; d < gElements.Items.count(); d++)
					{
						m_Elements->removeAll(gElements.Items.at(d));
					}
					PageItem *sing = gElements.Items.first();
					if ((gElements.Items.count() == 1)
						 && (sing->isImageFrame() || sing->isGroup() || sing->isPolygon() || sing->isPolyLine())
						 && (ite->patternMask().isEmpty() || sing->patternMask().isEmpty() || (sing->patternMask() == ite->patternMask()))
						 && (state->getFillOpacity() == (1.0 - ite->fillTransparency()))
					   )
					{
						m_Elements->replace(m_Elements->indexOf(ite), sing);
						m_doc->Items->removeAll(sing);
						m_doc->Items->replace(m_doc->Items->indexOf(ite), sing);
						if (m_groupStack.top().Items.indexOf(ite) > -1)
							m_groupStack.top().Items.replace(m_groupStack.top().Items.indexOf(ite), sing);
						else
							m_groupStack.top().Items.append(sing);
						sing->setFillTransparency(1.0 - (state->getFillOpacity() * (1.0 - ite->fillTransparency())));
						sing->setFillBlendmode(getBlendMode(state));
						if (!ite->patternMask().isEmpty())
						{
							sing->setPatternMask(ite->patternMask());
							sing->setMaskType(ite->maskType());
						}
						if (sing->isGroup() || (sing->lineColor() == CommonStrings::None))
						{
							double dx = sing->xPos() - ite->xPos();
							double dy = sing->yPos() - ite->yPos();
							sing->PoLine.translate(dx, dy);
							if (sing->isGroup())
							{
								QList<PageItem*> allItems = sing->asGroupFrame()->groupItemList;
								for (int si = 0; si < allItems.count(); si++)
								{
									allItems[si]->moveBy(dx, dy, true);
								}
							}
							QPainterPath input1 = sing->PoLine.toQPainterPath(true);
							if (sing->fillEvenOdd())
								input1.setFillRule(Qt::OddEvenFill);
							else
								input1.setFillRule(Qt::WindingFill);
							QPainterPath input2 = ite->PoLine.toQPainterPath(true);
							if (ite->fillEvenOdd())
								input2.setFillRule(Qt::OddEvenFill);
							else
								input2.setFillRule(Qt::WindingFill);
							QPainterPath result = input1.intersected(input2);
							sing->setXYPos(ite->xPos(), ite->yPos(), true);
							sing->setWidthHeight(ite->width(), ite->height(), true);
							sing->groupWidth = ite->width();
							sing->groupHeight = ite->height();
							if (sing->isImageFrame())
							{
								QTransform ft;
								ft.translate(-result.boundingRect().x(), -result.boundingRect().y());
								ft.scale(ite->width() / result.boundingRect().width(), ite->height() / result.boundingRect().height());
								result = ft.map(result);
							}
							sing->PoLine.fromQPainterPath(result);
							m_doc->AdjustItemSize(sing);
							if (sing->isGroup())
								sing->asGroupFrame()->adjustXYPosition();
							if (sing->isImageFrame())
							{
								sing->setImageXYOffset(0, 0);
								sing->AdjustPictScale();
							}
						}
						delete ite;
					}
					else
					{
						m_doc->groupObjectsToItem(ite, gElements.Items);
						ite->setFillTransparency(1.0 - state->getFillOpacity());
						ite->setFillBlendmode(getBlendMode(state));
					}
				}
				else
				{
					m_Elements->removeAll(ite);
					m_doc->Items->removeAll(ite);
					m_groupStack.top().Items.removeAll(ite);
					delete ite;
				}
			}
			if (m_clipStack.count() == 0)
				break;
		}
	}
}
/*
void SlaOutputDev::updateFillColor(GfxState *state)
{
	int shade = 100;
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
}
*/
void SlaOutputDev::beginTransparencyGroup(GfxState *state, double *bbox, GfxColorSpace * /*blendingColorSpace*/, GBool /*isolated*/, GBool /*knockout*/, GBool forSoftMask)
{
//	qDebug() << "Start Group";
	pushGroup("", forSoftMask);
}

void SlaOutputDev::endTransparencyGroup(GfxState *state)
{
//	qDebug() << "End Group";
	if (m_groupStack.count() != 0)
	{
		groupEntry gElements = m_groupStack.pop();
		tmpSel->clear();
		if (gElements.Items.count() > 0)
		{
			if (gElements.forSoftMask)
			{
				for (int dre = 0; dre < gElements.Items.count(); ++dre)
				{
					tmpSel->addItem(gElements.Items.at(dre), true);
					m_Elements->removeAll(gElements.Items.at(dre));
				}
				PageItem *ite = m_doc->groupObjectsSelection(tmpSel);
				ite->setFillTransparency(1.0 - state->getFillOpacity());
				ite->setFillBlendmode(getBlendMode(state));
				ScPattern pat = ScPattern();
				pat.setDoc(m_doc);
				m_doc->DoDrawing = true;
				pat.pattern = ite->DrawObj_toImage(qMax(ite->width(), ite->height()));
				pat.xoffset = 0;
				pat.yoffset = 0;
				m_doc->DoDrawing = false;
				pat.width = ite->width();
				pat.height = ite->height();
				ite->gXpos = 0;
				ite->gYpos = 0;
				ite->setXYPos(ite->gXpos, ite->gYpos, true);
				pat.items.append(ite);
				m_doc->Items->removeAll(ite);
				QString id = QString("Pattern_from_PDF_%1S").arg(m_doc->docPatterns.count() + 1);
				m_doc->addPattern(id, pat);
				m_currentMask = id;
				tmpSel->clear();
				return;
			}
			else
			{
				PageItem *ite;
				for (int dre = 0; dre < gElements.Items.count(); ++dre)
				{
					tmpSel->addItem(gElements.Items.at(dre), true);
					m_Elements->removeAll(gElements.Items.at(dre));
				}
				if (gElements.Items.count() != 1)
					ite = m_doc->groupObjectsSelection(tmpSel);
				else
					ite = gElements.Items.first();
				ite->setFillTransparency(1.0 - state->getFillOpacity());
				ite->setFillBlendmode(getBlendMode(state));
				for (int as = 0; as < tmpSel->count(); ++as)
				{
					m_Elements->append(tmpSel->itemAt(as));
				}
				if (m_groupStack.count() != 0)
					applyMask(ite);
			}
		}
		if (m_groupStack.count() != 0)
		{
			for (int as = 0; as < tmpSel->count(); ++as)
			{
				m_groupStack.top().Items.append(tmpSel->itemAt(as));
			}
		}
		tmpSel->clear();
	}
}

void SlaOutputDev::setSoftMask(GfxState * /*state*/, double * /*bbox*/, GBool alpha, Function *transferFunc, GfxColor * /*backdropColor*/)
{
	if (m_groupStack.count() != 0)
	{
		double lum = 0;
		double lum2 = 0;
		if (transferFunc)
			transferFunc->transform(&lum, &lum2);
		else
			lum2 = lum;
		if (lum == lum2)
			m_groupStack.top().inverted = false;
		else
			m_groupStack.top().inverted = true;
//		qDebug() << "Inverted Softmask" << m_groupStack.top().inverted;
		m_groupStack.top().maskName = m_currentMask;
		m_groupStack.top().alpha = alpha;
	}
}

void SlaOutputDev::clearSoftMask(GfxState * /*state*/)
{
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().maskName = "";
	}
}

void SlaOutputDev::updateFillColor(GfxState *state)
{
	CurrFillShade = 100;
	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &CurrFillShade);
}

void SlaOutputDev::updateStrokeColor(GfxState *state)
{
	CurrStrokeShade = 100;
	CurrColorStroke = getColor(state->getStrokeColorSpace(), state->getStrokeColor(), &CurrStrokeShade);
}

void SlaOutputDev::clip(GfxState *state)
{
//	qDebug() << "Clip";
	double *ctm;
	ctm = state->getCTM();
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	QString output = convertPath(state->getPath());
	FPointArray out;
	if (!output.isEmpty())
	{
		out.parseSVG(output);
		if (!pathIsClosed)
			out.svgClosePath();
		out.map(m_ctm);
	}
	double xmin, ymin, xmax, ymax;
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	int z = m_doc->itemAdd(PageItem::Group, PageItem::Rectangle, crect.x() + m_doc->currentPage()->xOffset(), crect.y() + m_doc->currentPage()->yOffset(), crect.width(), crect.height(), 0, CommonStrings::None, CommonStrings::None, true);
	PageItem *ite = m_doc->Items->at(z);
	if (!output.isEmpty())
	{
		FPoint wh(getMinClipF(&out));
		out.translate(-wh.x(), -wh.y());
		FPoint dim = out.WidthHeight();
		if (!((dim.x() == 0.0) || (dim.y() == 0.0)))		// avoid degenerate clipping paths
			ite->PoLine = out.copy();  //FIXME: try to avoid copy if FPointArray when properly shared
	}
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillEvenOdd(false);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->ContourLine = ite->PoLine.copy();
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	clipEntry clp;
	clp.ClipItem = ite;
	clp.grStackDepth = grStackDepth;
	m_clipStack.push(clp);
	m_doc->GroupCounter++;
	pushGroup();
}

void SlaOutputDev::eoClip(GfxState *state)
{
//	qDebug() << "EoClip";
	double *ctm;
	ctm = state->getCTM();
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	QString output = convertPath(state->getPath());
	FPointArray out;
	if (!output.isEmpty())
	{
		out.parseSVG(output);
		if (!pathIsClosed)
			out.svgClosePath();
		out.map(m_ctm);
	}
	double xmin, ymin, xmax, ymax;
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	int z = m_doc->itemAdd(PageItem::Group, PageItem::Rectangle, crect.x() + m_doc->currentPage()->xOffset(), crect.y() + m_doc->currentPage()->yOffset(), crect.width(), crect.height(), 0, CommonStrings::None, CommonStrings::None, true);
	PageItem *ite = m_doc->Items->at(z);
	if (!output.isEmpty())
	{
		FPoint wh(getMinClipF(&out));
		out.translate(-wh.x(), -wh.y());
		FPoint dim = out.WidthHeight();
		if (!((dim.x() == 0.0) || (dim.y() == 0.0)))		// avoid degenerate clipping paths
			ite->PoLine = out.copy();  //FIXME: try to avoid copy if FPointArray when properly shared
	}
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillEvenOdd(true);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->ContourLine = ite->PoLine.copy();
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	clipEntry clp;
	clp.ClipItem = ite;
	clp.grStackDepth = grStackDepth;
	m_clipStack.push(clp);
	m_doc->GroupCounter++;
	pushGroup();
}

void SlaOutputDev::stroke(GfxState *state)
{
//	qDebug() << "Stroke";
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	QString output = convertPath(state->getPath());
	getPenState(state);
	if ((m_Elements->count() != 0) && (output == Coords))			// Path is the same as in last fill
	{
		PageItem* ite = m_Elements->last();
		ite->setLineColor(CurrColorStroke);
		ite->setLineShade(CurrStrokeShade);
		ite->setLineEnd(PLineEnd);
		ite->setLineJoin(PLineJoin);
		ite->setLineWidth(state->getTransformedLineWidth());
		ite->setDashes(DashValues);
		ite->setDashOffset(DashOffset);
		ite->setLineTransparency(1.0 - state->getStrokeOpacity());
	}
	else
	{
		FPointArray out;
		out.parseSVG(output);
		m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
		out.map(m_ctm);
		FPoint wh = out.WidthHeight();
		if ((out.size() > 3) && ((wh.x() != 0.0) || (wh.y() != 0.0)))
		{
			int z;
			if (pathIsClosed)
				z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, xCoor, yCoor, 10, 10, state->getTransformedLineWidth(), CommonStrings::None, CurrColorStroke, true);
			else
				z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, xCoor, yCoor, 10, 10, state->getTransformedLineWidth(), CommonStrings::None, CurrColorStroke, true);
			PageItem* ite = m_doc->Items->at(z);
			ite->PoLine = out.copy();
			ite->ClipEdited = true;
			ite->FrameType = 3;
			ite->setWidthHeight(wh.x(),wh.y());
			m_doc->AdjustItemSize(ite);
			if (m_Elements->count() != 0)
			{
				PageItem* lItem = m_Elements->last();
				if ((lItem->lineColor() == CommonStrings::None) && (lItem->PoLine == ite->PoLine))
				{
					lItem->setLineColor(CurrColorStroke);
					lItem->setLineWidth(state->getTransformedLineWidth());
					lItem->setLineShade(CurrStrokeShade);
					lItem->setLineTransparency(1.0 - state->getStrokeOpacity());
					lItem->setLineBlendmode(getBlendMode(state));
					lItem->setLineEnd(PLineEnd);
					lItem->setLineJoin(PLineJoin);
					lItem->setDashes(DashValues);
					lItem->setDashOffset(DashOffset);
					lItem->setTextFlowMode(PageItem::TextFlowDisabled);
					m_doc->Items->removeAll(ite);
				}
				else
				{
					ite->setLineShade(CurrStrokeShade);
					ite->setLineTransparency(1.0 - state->getStrokeOpacity());
					ite->setLineBlendmode(getBlendMode(state));
					ite->setLineEnd(PLineEnd);
					ite->setLineJoin(PLineJoin);
					ite->setDashes(DashValues);
					ite->setDashOffset(DashOffset);
					ite->setTextFlowMode(PageItem::TextFlowDisabled);
					m_Elements->append(ite);
					if (m_groupStack.count() != 0)
						m_groupStack.top().Items.append(ite);
				}
			}
			else
			{
				ite->setLineShade(CurrStrokeShade);
				ite->setLineTransparency(1.0 - state->getStrokeOpacity());
				ite->setLineBlendmode(getBlendMode(state));
				ite->setLineEnd(PLineEnd);
				ite->setLineJoin(PLineJoin);
				ite->setDashes(DashValues);
				ite->setDashOffset(DashOffset);
				ite->setTextFlowMode(PageItem::TextFlowDisabled);
				m_Elements->append(ite);
				if (m_groupStack.count() != 0)
					m_groupStack.top().Items.append(ite);
			}
		}
	}
}

void SlaOutputDev::fill(GfxState *state)
{
//	qDebug() << "Fill";
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	FPointArray out;
	QString output = convertPath(state->getPath());
	out.parseSVG(output);
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	out.map(m_ctm);
	Coords = output;
	FPoint wh = out.WidthHeight();
	if ((out.size() > 3) && ((wh.x() != 0.0) || (wh.y() != 0.0)))
	{
		int z;
		if (pathIsClosed)
			z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, xCoor, yCoor, 10, 10, 0, CurrColorFill, CommonStrings::None, true);
		else
			z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, xCoor, yCoor, 10, 10, 0, CurrColorFill, CommonStrings::None, true);
		PageItem* ite = m_doc->Items->at(z);
		ite->PoLine = out.copy();
		ite->ClipEdited = true;
		ite->FrameType = 3;
		ite->setFillShade(CurrFillShade);
		ite->setLineShade(100);
		ite->setFillEvenOdd(false);
		ite->setFillTransparency(1.0 - state->getFillOpacity());
		ite->setFillBlendmode(getBlendMode(state));
		ite->setLineEnd(PLineEnd);
		ite->setLineJoin(PLineJoin);
		ite->setWidthHeight(wh.x(),wh.y());
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_doc->AdjustItemSize(ite);
		m_Elements->append(ite);
		if (m_groupStack.count() != 0)
		{
			m_groupStack.top().Items.append(ite);
			applyMask(ite);
		}
	}
}

void SlaOutputDev::eoFill(GfxState *state)
{
//	qDebug() << "EoFill";
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	FPointArray out;
	QString output = convertPath(state->getPath());
	out.parseSVG(output);
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	out.map(m_ctm);
	Coords = output;
	FPoint wh = out.WidthHeight();
	if ((out.size() > 3) && ((wh.x() != 0.0) || (wh.y() != 0.0)))
	{
		int z;
		if (pathIsClosed)
			z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, xCoor, yCoor, 10, 10, 0, CurrColorFill, CommonStrings::None, true);
		else
			z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, xCoor, yCoor, 10, 10, 0, CurrColorFill, CommonStrings::None, true);
		PageItem* ite = m_doc->Items->at(z);
		ite->PoLine = out.copy();
		ite->ClipEdited = true;
		ite->FrameType = 3;
		ite->setFillShade(CurrFillShade);
		ite->setLineShade(100);
		ite->setFillEvenOdd(true);
		ite->setFillTransparency(1.0 - state->getFillOpacity());
		ite->setFillBlendmode(getBlendMode(state));
		ite->setLineEnd(PLineEnd);
		ite->setLineJoin(PLineJoin);
		ite->setWidthHeight(wh.x(),wh.y());
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_doc->AdjustItemSize(ite);
		m_Elements->append(ite);
		if (m_groupStack.count() != 0)
		{
			m_groupStack.top().Items.append(ite);
			applyMask(ite);
		}
	}
}

GBool SlaOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading, double tMin, double tMax)
{
	double GrStartX;
	double GrStartY;
	double GrEndX;
	double GrEndY;
	int shade = 100;
	Function *func = shading->getFunc(0);
	VGradient FillGradient = VGradient(VGradient::linear);
	FillGradient.clearStops();
	GfxColorSpace *color_space = shading->getColorSpace();
	if (func->getType() == 3)
	{
		StitchingFunction *stitchingFunc = (StitchingFunction*)func;
		double *bounds = stitchingFunc->getBounds();
		int num_funcs = stitchingFunc->getNumFuncs();
		// Add stops from all the stitched functions
		for ( int i = 0 ; i < num_funcs ; i++ )
		{
			GfxColor temp;
			((GfxAxialShading*)shading)->getColor(bounds[i], &temp);
			QString stopColor = getColor(color_space, &temp, &shade);
			FillGradient.addStop( ScColorEngine::getShadeColor(m_doc->PageColors[stopColor], m_doc, shade), bounds[i], 0.5, 1.0, stopColor, shade );
			if (i == num_funcs - 1)
			{
				((GfxAxialShading*)shading)->getColor(bounds[i+1], &temp);
				QString stopColor = getColor(color_space, &temp, &shade);
				FillGradient.addStop( ScColorEngine::getShadeColor(m_doc->PageColors[stopColor], m_doc, shade), bounds[i+1], 0.5, 1.0, stopColor, shade );
			}
		}
	}
	else if ((func->getType() == 2) || (func->getType() == 0))
	{
		GfxColor stop1;
		((GfxAxialShading*)shading)->getColor(0.0, &stop1);
		QString stopColor1 = getColor(color_space, &stop1, &shade);
		FillGradient.addStop( ScColorEngine::getShadeColor(m_doc->PageColors[stopColor1], m_doc, shade), 0.0, 0.5, 1.0, stopColor1, shade );
		GfxColor stop2;
		((GfxAxialShading*)shading)->getColor(1.0, &stop2);
		QString stopColor2 = getColor(color_space, &stop2, &shade);
		FillGradient.addStop( ScColorEngine::getShadeColor(m_doc->PageColors[stopColor2], m_doc, shade), 1.0, 0.5, 1.0, stopColor2, shade );
	}
	((GfxAxialShading*)shading)->getCoords(&GrStartX, &GrStartY, &GrEndX, &GrEndY);
	double xmin, ymin, xmax, ymax;
	// get the clip region bbox
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	double *ctm;
	ctm = state->getCTM();
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	FPointArray gr;
	gr.addPoint(GrStartX, GrStartY);
	gr.addPoint(GrEndX, GrEndY);
	gr.map(m_ctm);
	GrStartX = gr.point(0).x() - crect.x();
	GrStartY = gr.point(0).y() - crect.y();
	GrEndX = gr.point(1).x() - crect.x();
	GrEndY = gr.point(1).y() - crect.y();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	QString output = QString("M %1 %2").arg(0.0).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(0.0);
	output += QString("Z");
	pathIsClosed = true;
	Coords = output;
	int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + crect.x(), yCoor + crect.y(), crect.width(), crect.height(), 0, CurrColorFill, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(CurrFillShade);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	ite->setLineEnd(PLineEnd);
	ite->setLineJoin(PLineJoin);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->GrType = 6;
	ite->fill_gradient = FillGradient;
	ite->setGradientVector(GrStartX, GrStartY, GrEndX, GrEndY, 0, 0, 1, 0);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	return gTrue;
}

GBool SlaOutputDev::radialShadedFill(GfxState *state, GfxRadialShading *shading, double sMin, double sMax)
{
	double GrStartX;
	double GrStartY;
	double GrEndX;
	double GrEndY;
	int shade = 100;
	Function *func = shading->getFunc(0);
	VGradient FillGradient = VGradient(VGradient::linear);
	FillGradient.clearStops();
	GfxColorSpace *color_space = shading->getColorSpace();
	if (func->getType() == 3)
	{
		StitchingFunction *stitchingFunc = (StitchingFunction*)func;
		double *bounds = stitchingFunc->getBounds();
		int num_funcs = stitchingFunc->getNumFuncs();
		// Add stops from all the stitched functions
		for ( int i = 0 ; i < num_funcs ; i++ )
		{
			GfxColor temp;
			((GfxRadialShading*)shading)->getColor(bounds[i], &temp);
			QString stopColor = getColor(color_space, &temp, &shade);
			FillGradient.addStop( ScColorEngine::getShadeColor(m_doc->PageColors[stopColor], m_doc, shade), bounds[i], 0.5, 1.0, stopColor, shade );
			if (i == num_funcs - 1)
			{
				((GfxRadialShading*)shading)->getColor(bounds[i+1], &temp);
				QString stopColor = getColor(color_space, &temp, &shade);
				FillGradient.addStop( ScColorEngine::getShadeColor(m_doc->PageColors[stopColor], m_doc, shade), bounds[i+1], 0.5, 1.0, stopColor, shade );
			}
		}
	}
	else if ((func->getType() == 2) || (func->getType() == 0))
	{
		GfxColor stop1;
		((GfxRadialShading*)shading)->getColor(0.0, &stop1);
		QString stopColor1 = getColor(color_space, &stop1, &shade);
		FillGradient.addStop( ScColorEngine::getShadeColor(m_doc->PageColors[stopColor1], m_doc, shade), 0.0, 0.5, 1.0, stopColor1, shade );
		GfxColor stop2;
		((GfxRadialShading*)shading)->getColor(1.0, &stop2);
		QString stopColor2 = getColor(color_space, &stop2, &shade);
		FillGradient.addStop( ScColorEngine::getShadeColor(m_doc->PageColors[stopColor2], m_doc, shade), 1.0, 0.5, 1.0, stopColor2, shade );
	}
	double r0, x1, y1, r1;
	((GfxRadialShading*)shading)->getCoords(&GrStartX, &GrStartY, &r0, &x1, &y1, &r1);
	double xmin, ymin, xmax, ymax;
	// get the clip region bbox
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	double GrFocalX = x1;
	double GrFocalY = y1;
	GrEndX = GrFocalX + r1;
	GrEndY = GrFocalY;
	double *ctm;
	ctm = state->getCTM();
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	FPointArray gr;
	gr.addPoint(GrStartX, GrStartY);
	gr.addPoint(GrEndX, GrEndY);
	gr.addPoint(GrFocalX, GrFocalY);
	gr.map(m_ctm);
	GrStartX = gr.point(0).x() - crect.x();
	GrStartY = gr.point(0).y() - crect.y();
	GrEndX = gr.point(1).x() - crect.x();
	GrEndY = gr.point(1).y() - crect.y();
	GrFocalX = gr.point(2).x() - crect.x();
	GrFocalY = gr.point(2).y() - crect.y();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	QString output = QString("M %1 %2").arg(0.0).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(0.0);
	output += QString("Z");
	pathIsClosed = true;
	Coords = output;
	int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + crect.x(), yCoor + crect.y(), crect.width(), crect.height(), 0, CurrColorFill, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(CurrFillShade);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	ite->setLineEnd(PLineEnd);
	ite->setLineJoin(PLineJoin);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->GrType = 7;
	ite->fill_gradient = FillGradient;
	ite->setGradientVector(GrStartX, GrStartY, GrEndX, GrEndY, GrFocalX, GrFocalY, 1, 0);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	return gTrue;
}

GBool SlaOutputDev::gouraudTriangleShadedFill(GfxState *state, GfxGouraudTriangleShading *shading)
{
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	double xmin, ymin, xmax, ymax;
	// get the clip region bbox
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	QString output = QString("M %1 %2").arg(0.0).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(0.0);
	output += QString("Z");
	pathIsClosed = true;
	Coords = output;
	double *ctm;
	ctm = state->getCTM();
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + crect.x(), yCoor + crect.y(), crect.width(), crect.height(), 0, CurrColorFill, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(CurrFillShade);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	ite->setLineEnd(PLineEnd);
	ite->setLineJoin(PLineJoin);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	GfxColor color[3];
	double x0, y0, x1, y1, x2, y2;
	for (int i = 0; i < shading->getNTriangles(); i++)
	{
		int shade = 100;
		meshGradientPatch patchM;
		shading->getTriangle(i, &x0, &y0, &color[0],  &x1, &y1, &color[1],  &x2, &y2, &color[2]);
		patchM.BL.resetTo(FPoint(x0, y0));
		patchM.BL.shade = 100;
		patchM.BL.transparency = 1.0;
		patchM.BL.colorName = getColor(shading->getColorSpace(), &color[0], &shade);
		patchM.BL.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.BL.colorName], m_doc, shade);
		patchM.TL.resetTo(FPoint(x1, y1));
		patchM.TL.shade = 100;
		patchM.TL.transparency = 1.0;
		patchM.TL.colorName = getColor(shading->getColorSpace(), &color[1], &shade);
		patchM.TL.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.TL.colorName], m_doc, shade);
		patchM.TR.resetTo(FPoint(x2, y2));
		patchM.TR.shade = 100;
		patchM.TR.transparency = 1.0;
		patchM.TR.colorName = getColor(shading->getColorSpace(), &color[2], &shade);
		patchM.TR.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.TR.colorName], m_doc, shade);
		patchM.BR.resetTo(FPoint(x0, y0));
		patchM.BR.shade = 100;
		patchM.BR.transparency = 1.0;
		patchM.BR.colorName = getColor(shading->getColorSpace(), &color[0], &shade);
		patchM.BR.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.BR.colorName], m_doc, shade);
		patchM.TL.transform(m_ctm);
		patchM.TL.moveRel(-crect.x(), -crect.y());
		patchM.TR.transform(m_ctm);
		patchM.TR.moveRel(-crect.x(), -crect.y());
		patchM.BR.transform(m_ctm);
		patchM.BR.moveRel(-crect.x(), -crect.y());
		patchM.BL.transform(m_ctm);
		patchM.BL.moveRel(-crect.x(), -crect.y());
		ite->meshGradientPatches.append(patchM);
	}
	ite->GrType = 12;
	return gTrue;
}

GBool SlaOutputDev::patchMeshShadedFill(GfxState *state, GfxPatchMeshShading *shading)
{
//	qDebug() << "mesh shaded fill";
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	double xmin, ymin, xmax, ymax;
	// get the clip region bbox
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	QString output = QString("M %1 %2").arg(0.0).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(0.0);
	output += QString("Z");
	pathIsClosed = true;
	Coords = output;
	double *ctm;
	ctm = state->getCTM();
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + crect.x(), yCoor + crect.y(), crect.width(), crect.height(), 0, CurrColorFill, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(CurrFillShade);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	ite->setLineEnd(PLineEnd);
	ite->setLineJoin(PLineJoin);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
//	qDebug() << "Mesh of" << shading->getNPatches() << "Patches";
	ite->meshGradientPatches.clear();
	for (int i = 0; i < shading->getNPatches(); i++)
	{
		int shade = 100;
		GfxPatch *patch = shading->getPatch(i);
		GfxColor color;
		meshGradientPatch patchM;
		int u, v;
		patchM.BL.resetTo(FPoint(patch->x[0][0], patch->y[0][0]));
		patchM.BL.controlTop = FPoint(patch->x[0][1], patch->y[0][1]);
		patchM.BL.controlRight = FPoint(patch->x[1][0], patch->y[1][0]);
		patchM.BL.controlColor = FPoint(patch->x[1][1], patch->y[1][1]);
		u = 0;
		v = 0;
		if (shading->isParameterized())
		{
			shading->getParameterizedColor (patch->color[u][v].c[0], &color);
		}
		else
		{
			for (int k = 0; k < shading->getColorSpace()->getNComps(); k++)
			{
				color.c[k] = GfxColorComp (patch->color[u][v].c[k]);
			}
		}
		patchM.BL.colorName = getColor(shading->getColorSpace(), &color, &shade);
		patchM.BL.shade = 100;
		patchM.BL.transparency = 1.0;
		patchM.BL.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.BL.colorName], m_doc, shade);

		u = 0;
		v = 1;
		patchM.TL.resetTo(FPoint(patch->x[0][3], patch->y[0][3]));
		patchM.TL.controlRight = FPoint(patch->x[1][3], patch->y[1][3]);
		patchM.TL.controlBottom = FPoint(patch->x[0][2], patch->y[0][2]);
		patchM.TL.controlColor = FPoint(patch->x[1][2], patch->y[1][2]);
		if (shading->isParameterized())
		{
			shading->getParameterizedColor (patch->color[u][v].c[0], &color);
		}
		else
		{
			for (int k = 0; k < shading->getColorSpace()->getNComps(); k++)
			{
				color.c[k] = GfxColorComp (patch->color[u][v].c[k]);
			}
		}
		patchM.TL.colorName = getColor(shading->getColorSpace(), &color, &shade);
		patchM.TL.shade = 100;
		patchM.TL.transparency = 1.0;
		patchM.TL.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.TL.colorName], m_doc, shade);

		u = 1;
		v = 1;
		patchM.TR.resetTo(FPoint(patch->x[3][3], patch->y[3][3]));
		patchM.TR.controlBottom = FPoint(patch->x[3][2], patch->y[3][2]);
		patchM.TR.controlLeft = FPoint(patch->x[2][3], patch->y[2][3]);
		patchM.TR.controlColor = FPoint(patch->x[2][2], patch->y[2][2]);
		if (shading->isParameterized())
		{
			shading->getParameterizedColor (patch->color[u][v].c[0], &color);
		}
		else
		{
			for (int k = 0; k < shading->getColorSpace()->getNComps(); k++)
			{
				color.c[k] = GfxColorComp (patch->color[u][v].c[k]);
			}
		}
		patchM.TR.colorName = getColor(shading->getColorSpace(), &color, &shade);
		patchM.TR.shade = 100;
		patchM.TR.transparency = 1.0;
		patchM.TR.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.TR.colorName], m_doc, shade);

		u = 1;
		v = 0;
		patchM.BR.resetTo(FPoint(patch->x[3][0], patch->y[3][0]));
		patchM.BR.controlLeft = FPoint(patch->x[2][0], patch->y[2][0]);
		patchM.BR.controlTop = FPoint(patch->x[3][1], patch->y[3][1]);
		patchM.BR.controlColor = FPoint(patch->x[2][1], patch->y[2][1]);
		if (shading->isParameterized())
		{
			shading->getParameterizedColor (patch->color[u][v].c[0], &color);
		}
		else
		{
			for (int k = 0; k < shading->getColorSpace()->getNComps(); k++)
			{
				color.c[k] = GfxColorComp (patch->color[u][v].c[k]);
			}
		}
		patchM.BR.colorName = getColor(shading->getColorSpace(), &color, &shade);
		patchM.BR.shade = 100;
		patchM.BR.transparency = 1.0;
		patchM.BR.color = ScColorEngine::getShadeColorProof(m_doc->PageColors[patchM.BR.colorName], m_doc, shade);

		patchM.TL.transform(m_ctm);
		patchM.TL.moveRel(-crect.x(), -crect.y());
		patchM.TR.transform(m_ctm);
		patchM.TR.moveRel(-crect.x(), -crect.y());
		patchM.BR.transform(m_ctm);
		patchM.BR.moveRel(-crect.x(), -crect.y());
		patchM.BL.transform(m_ctm);
		patchM.BL.moveRel(-crect.x(), -crect.y());
		ite->meshGradientPatches.append(patchM);
	}
	ite->GrType = 12;
	return gTrue;
}

GBool SlaOutputDev::tilingPatternFill(GfxState *state, Gfx * /*gfx*/, Catalog *cat, Object *str, double *pmat, int paintType, int tilingType, Dict *resDict, double *mat, double *bbox, int x0, int y0, int x1, int y1, double xStep, double yStep)
{
	PDFRectangle box;
	Gfx *gfx;
	QString id;
	PageItem *ite;
	groupEntry gElements;
	gElements.forSoftMask = gFalse;
	gElements.alpha = gFalse;
	gElements.inverted = false;
	gElements.maskName = "";
	gElements.Items.clear();
	m_groupStack.push(gElements);
	double width, height;
	width = bbox[2] - bbox[0];
	height = bbox[3] - bbox[1];
	if (xStep != width || yStep != height)
		return gFalse;
	box.x1 = bbox[0];
	box.y1 = bbox[1];
	box.x2 = bbox[2];
	box.y2 = bbox[3];
	double *ctm;
	ctm = state->getCTM();
	m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	QTransform mm = QTransform(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5]);
	QTransform mmx = mm * m_ctm;

#ifdef POPPLER_VERSION
	gfx = new Gfx(pdfDoc, this, resDict, &box, NULL);
#else
	gfx = new Gfx(xref, this, resDict, catalog, &box, NULL);
#endif

	gfx->display(str);
	gElements = m_groupStack.pop();
	m_doc->m_Selection->clear();
	double pwidth = 0;
	double pheight = 0;
	if (gElements.Items.count() > 0)
	{
		for (int dre = 0; dre < gElements.Items.count(); ++dre)
		{
			m_doc->m_Selection->addItem(gElements.Items.at(dre), true);
			m_Elements->removeAll(gElements.Items.at(dre));
		}
		m_doc->itemSelection_FlipV();
		PageItem *ite;
		if (m_doc->m_Selection->count() > 1)
			ite = m_doc->groupObjectsSelection();
		else
			ite = m_doc->m_Selection->itemAt(0);
		ite->setFillTransparency(1.0 - state->getFillOpacity());
		ite->setFillBlendmode(getBlendMode(state));
		m_doc->m_Selection->clear();
		ScPattern pat = ScPattern();
		pat.setDoc(m_doc);
		m_doc->DoDrawing = true;
		pat.pattern = ite->DrawObj_toImage(qMax(ite->width(), ite->height()));
		pat.xoffset = 0;
		pat.yoffset = 0;
		m_doc->DoDrawing = false;
		pat.width = ite->width();
		pat.height = ite->height();
		pwidth = ite->width();
		pheight = ite->height();
		ite->gXpos = 0;
		ite->gYpos = 0;
		ite->setXYPos(ite->gXpos, ite->gYpos, true);
		pat.items.append(ite);
		m_doc->Items->removeAll(ite);
		id = QString("Pattern_from_PDF_%1").arg(m_doc->docPatterns.count() + 1);
		m_doc->addPattern(id, pat);
	}
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	double xmin, ymin, xmax, ymax;
	// get the clip region bbox
	state->getClipBBox(&xmin, &ymin, &xmax, &ymax);
	QRectF crect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
	crect = crect.normalized();
	QString output = QString("M %1 %2").arg(0.0).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(0.0);
	output += QString("L %1 %2").arg(crect.width()).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(crect.height());
	output += QString("L %1 %2").arg(0.0).arg(0.0);
	output += QString("Z");
	pathIsClosed = true;
	Coords = output;
	int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + crect.x(), yCoor + crect.y(), crect.width(), crect.height(), 0, CurrColorFill, CommonStrings::None, true);
	ite = m_doc->Items->at(z);
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(CurrFillShade);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	ite->setLineEnd(PLineEnd);
	ite->setLineJoin(PLineJoin);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->GrType = 8;
	ite->setPattern(id);
	ite->setPatternTransform(fabs(pmat[0]) * 100, fabs(pmat[3]) * 100, mmx.dx() - ctm[4], mmx.dy() - ctm[5], 0, -1 * pmat[1], pmat[2]);
	m_doc->AdjustItemSize(ite);
	m_Elements->append(ite);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(ite);
		applyMask(ite);
	}
	delete gfx;
	return gTrue;
}

void SlaOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool interpolate, GBool inlineImg)
{
//	qDebug() << "Draw Image Mask";
	QImage * image = 0;
	int invert_bit;
	int row_stride;
	int x, y, i, bit;
	unsigned char *dest = 0;
	unsigned char *buffer;
	Guchar *pix;
	ImageStream * imgStr = new ImageStream(str, width, 1, 1);
	imgStr->reset();
#ifdef WORDS_BIGENDIAN
	image = new QImage(width, height, QImage::Format_Mono);
#else
	image = new QImage(width, height, QImage::Format_MonoLSB);
#endif
	if (image == NULL || image->isNull())
	{
		delete imgStr;
		delete image;
		return;
	}
	invert_bit = invert ? 1 : 0;
	buffer = image->bits();
	row_stride = image->bytesPerLine();
	for (y = 0; y < height; y++)
	{
		pix = imgStr->getLine();
		dest = buffer + y * row_stride;
		i = 0;
		bit = 0;
		for (x = 0; x < width; x++)
		{
			if (bit == 0)
				dest[i] = 0;
			if (!(pix[x] ^ invert_bit))
			{
#ifdef WORDS_BIGENDIAN
				dest[i] |= (1 << (7 - bit));
#else
				dest[i] |= (1 << bit);
#endif
			}
			bit++;
			if (bit > 7)
			{
				bit = 0;
				i++;
			}
		}
	}
	QColor backColor = ScColorEngine::getShadeColorProof(m_doc->PageColors[CurrColorFill], m_doc, CurrFillShade);
	QImage res = QImage(width, height, QImage::Format_ARGB32);
	res.fill(backColor.rgb());
	unsigned char cc, cm, cy, ck;
	for( int yi = 0; yi < res.height(); ++yi )
	{
		QRgb *t = (QRgb*)(res.scanLine( yi ));
		for(int xi = 0; xi < res.width(); ++xi )
		{
			cc = qRed(*t);
			cm = qGreen(*t);
			cy = qBlue(*t);
			ck = image->pixel(xi, yi);
			if (ck == 0)
				(*t) = qRgba(cc, cm, cy, 0);
			else
				(*t) = qRgba(cc, cm, cy, 255);
			t++;
		}
	}
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	QRectF crect = QRectF(0, 0, width, height);
	m_ctm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, ctm[2] + ctm[4], ctm[3] + ctm[5]);
	QLineF cline = QLineF(0, 0, width, 0);
	QLineF tline = m_ctm.map(cline);
	QRectF trect = m_ctm.mapRect(crect);
	double sx = m_ctm.m11();
	double sy = m_ctm.m22();
	QTransform mm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, 0, 0);
	if ((mm.type() == QTransform::TxShear) || (mm.type() == QTransform::TxRotate))
	{
		mm.reset();
		mm.rotate(-tline.angle());
	}
	else
	{
		mm.reset();
		if (sx < 0)
			mm.scale(-1, 1);
		if (sy < 0)
			mm.scale(1, -1);
	}
	res = res.transformed(mm);
	if (res.isNull())
	{
		delete imgStr;
		delete image;
		return;
	}
	int z = m_doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, xCoor + trect.x(), yCoor + trect.y(), trect.width(), trect.height(), 0, CommonStrings::None, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->SetRectFrame();
	m_doc->setRedrawBounding(ite);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->setFillShade(100);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	m_doc->AdjustItemSize(ite);
	QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_pdf_XXXXXX.png");
	tempFile->setAutoRemove(false);
	if (tempFile->open())
	{
		QString fileName = getLongPathName(tempFile->fileName());
		if (!fileName.isEmpty())
		{
			tempFile->close();
			ite->isInlineImage = true;
			ite->isTempFile = true;
			res.save(fileName, "PNG");
			m_doc->loadPict(fileName, ite);
			ite->setImageScalingMode(false, false);
			m_Elements->append(ite);
			if (m_groupStack.count() != 0)
			{
				m_groupStack.top().Items.append(ite);
				applyMask(ite);
			}
		}
		else
			m_doc->Items->removeAll(ite);
	}
	else
		m_doc->Items->removeAll(ite);
	imgStr->close();
	delete tempFile;
	delete imgStr;
	delete image;
}

void SlaOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool interpolate, Stream *maskStr, int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap, GBool maskInterpolate)
{
//	qDebug() << "Masked Image Components" << colorMap->getNumPixelComps();
	ImageStream * imgStr = new ImageStream(str, width, colorMap->getNumPixelComps(), colorMap->getBits());
	imgStr->reset();
	unsigned int *dest = 0;
	unsigned char * buffer = new unsigned char[width * height * 4];
	QImage * image = 0;
	for (int y = 0; y < height; y++)
	{
		dest = (unsigned int *)(buffer + y * 4 * width);
		Guchar * pix = imgStr->getLine();
		colorMap->getRGBLine(pix, dest, width);
	}
	image = new QImage(buffer, width, height, QImage::Format_RGB32);
	if (image == NULL || image->isNull())
	{
		delete imgStr;
		delete[] buffer;
		delete image;
		return;
	}
	ImageStream *mskStr = new ImageStream(maskStr, maskWidth, maskColorMap->getNumPixelComps(), maskColorMap->getBits());
	mskStr->reset();
	Guchar *mdest = 0;
	unsigned char * mbuffer = new unsigned char[maskWidth * maskHeight];
	for (int y = 0; y < maskHeight; y++)
	{
		mdest = (Guchar *)(mbuffer + y * maskWidth);
		Guchar * pix = mskStr->getLine();
		maskColorMap->getGrayLine(pix, mdest, maskWidth);
	}
	if ((maskWidth != width) || (maskHeight != height))
	{
		delete imgStr;
		delete[] buffer;
		delete image;
		delete mskStr;
		delete[] mbuffer;
		return;
	}
	QImage res = image->convertToFormat(QImage::Format_ARGB32);
	unsigned char cc, cm, cy, ck;
	int s = 0;
	for( int yi=0; yi < res.height(); ++yi )
	{
		QRgb *t = (QRgb*)(res.scanLine( yi ));
		for(int xi=0; xi < res.width(); ++xi )
		{
			cc = qRed(*t);
			cm = qGreen(*t);
			cy = qBlue(*t);
			ck = mbuffer[s];
			(*t) = qRgba(cc, cm, cy, ck);
			s++;
			t++;
		}
	}
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	QRectF crect = QRectF(0, 0, width, height);
	m_ctm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, ctm[2] + ctm[4], ctm[3] + ctm[5]);
	QLineF cline = QLineF(0, 0, width, 0);
	QLineF tline = m_ctm.map(cline);
	QRectF trect = m_ctm.mapRect(crect);
	double sx = m_ctm.m11();
	double sy = m_ctm.m22();
	QTransform mm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, 0, 0);
	if ((mm.type() == QTransform::TxShear) || (mm.type() == QTransform::TxRotate))
	{
		mm.reset();
		mm.rotate(-tline.angle());
	}
	else
	{
		mm.reset();
		if (sx < 0)
			mm.scale(-1, 1);
		if (sy < 0)
			mm.scale(1, -1);
	}
	res = res.transformed(mm);
	int z = m_doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, xCoor + trect.x(), yCoor + trect.y(), trect.width(), trect.height(), 0, CommonStrings::None, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->SetRectFrame();
	m_doc->setRedrawBounding(ite);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->setFillShade(100);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	m_doc->AdjustItemSize(ite);
	QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_pdf_XXXXXX.png");
	tempFile->setAutoRemove(false);
	if (tempFile->open())
	{
		QString fileName = getLongPathName(tempFile->fileName());
		if (!fileName.isEmpty())
		{
			tempFile->close();
			ite->isInlineImage = true;
			ite->isTempFile = true;
			res.save(fileName, "PNG");
			m_doc->loadPict(fileName, ite);
		//	ite->setImageScalingMode(false, false);
			m_Elements->append(ite);
			if (m_groupStack.count() != 0)
			{
				m_groupStack.top().Items.append(ite);
				applyMask(ite);
			}
		}
		else
			m_doc->Items->removeAll(ite);
	}
	else
		m_doc->Items->removeAll(ite);
	delete tempFile;
	delete imgStr;
	delete[] buffer;
	delete image;
	delete mskStr;
	delete[] mbuffer;
}

void SlaOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,  int width, int height, GfxImageColorMap *colorMap, GBool interpolate, Stream *maskStr, int maskWidth, int maskHeight, GBool maskInvert, GBool maskInterpolate)
{
	ImageStream * imgStr = new ImageStream(str, width, colorMap->getNumPixelComps(), colorMap->getBits());
	imgStr->reset();
	unsigned int *dest = 0;
	unsigned char * buffer = new unsigned char[width * height * 4];
	QImage * image = 0;
	for (int y = 0; y < height; y++)
	{
		dest = (unsigned int *)(buffer + y * 4 * width);
		Guchar * pix = imgStr->getLine();
		colorMap->getRGBLine(pix, dest, width);
	}
	image = new QImage(buffer, width, height, QImage::Format_RGB32);
	if (image == NULL || image->isNull())
	{
		delete imgStr;
		delete[] buffer;
		delete image;
		return;
	}
	ImageStream *mskStr = new ImageStream(maskStr, maskWidth, 1, 1);
	mskStr->reset();
	Guchar *mdest = 0;
	int invert_bit = maskInvert ? 1 : 0;
	unsigned char * mbuffer = new unsigned char[maskWidth * maskHeight];
	for (int y = 0; y < maskHeight; y++)
	{
		mdest = (Guchar *)(mbuffer + y * maskWidth);
		Guchar * pix = mskStr->getLine();
		for (int x = 0; x < maskWidth; x++)
		{
			if (pix[x] ^ invert_bit)
				*mdest++ = 0;
			else
				*mdest++ = 255;
		}
	}
	if ((maskWidth != width) || (maskHeight != height))
	{
		delete imgStr;
		delete[] buffer;
		delete image;
		delete mskStr;
		delete[] mbuffer;
		return;
	}
	QImage res = image->convertToFormat(QImage::Format_ARGB32);
	unsigned char cc, cm, cy, ck;
	int s = 0;
	for( int yi=0; yi < res.height(); ++yi )
	{
		QRgb *t = (QRgb*)(res.scanLine( yi ));
		for(int xi=0; xi < res.width(); ++xi )
		{
			cc = qRed(*t);
			cm = qGreen(*t);
			cy = qBlue(*t);
			ck = mbuffer[s];
			(*t) = qRgba(cc, cm, cy, ck);
			s++;
			t++;
		}
	}
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	QRectF crect = QRectF(0, 0, width, height);
	m_ctm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, ctm[2] + ctm[4], ctm[3] + ctm[5]);
	QLineF cline = QLineF(0, 0, width, 0);
	QLineF tline = m_ctm.map(cline);
	QRectF trect = m_ctm.mapRect(crect);
	double sx = m_ctm.m11();
	double sy = m_ctm.m22();
	QTransform mm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, 0, 0);
	if ((mm.type() == QTransform::TxShear) || (mm.type() == QTransform::TxRotate))
	{
		mm.reset();
		mm.rotate(-tline.angle());
	}
	else
	{
		mm.reset();
		if (sx < 0)
			mm.scale(-1, 1);
		if (sy < 0)
			mm.scale(1, -1);
	}
	res = res.transformed(mm);
	int z = m_doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, xCoor + trect.x(), yCoor + trect.y(), trect.width(), trect.height(), 0, CommonStrings::None, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->SetRectFrame();
	m_doc->setRedrawBounding(ite);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->setFillShade(100);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	m_doc->AdjustItemSize(ite);
	QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_pdf_XXXXXX.png");
	tempFile->setAutoRemove(false);
	if (tempFile->open())
	{
		QString fileName = getLongPathName(tempFile->fileName());
		if (!fileName.isEmpty())
		{
			tempFile->close();
			ite->isInlineImage = true;
			ite->isTempFile = true;
			res.save(fileName, "PNG");
			m_doc->loadPict(fileName, ite);
		//	ite->setImageScalingMode(false, false);
			m_Elements->append(ite);
			if (m_groupStack.count() != 0)
			{
				m_groupStack.top().Items.append(ite);
				applyMask(ite);
			}
		}
		else
			m_doc->Items->removeAll(ite);
	}
	else
		m_doc->Items->removeAll(ite);
	delete tempFile;
	delete imgStr;
	delete[] buffer;
	delete image;
	delete mskStr;
	delete[] mbuffer;
}

void SlaOutputDev::drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool interpolate, int *maskColors, GBool inlineImg)
{
	ImageStream * imgStr = new ImageStream(str, width, colorMap->getNumPixelComps(), colorMap->getBits());
//	qDebug() << "Image Components" << colorMap->getNumPixelComps() << "Mask" << maskColors;
	imgStr->reset();
	QImage * image = 0;
	if (maskColors)
	{
		image = new QImage(width, height, QImage::Format_ARGB32);
		for (int y = 0; y < height; y++)
		{
			QRgb *s = (QRgb*)(image->scanLine(y));
			Guchar *pix = imgStr->getLine();
			for (int x = 0; x < width; x++)
			{
				GfxRGB rgb;
				colorMap->getRGB(pix, &rgb);
				int Rc = qRound(colToDbl(rgb.r) * 255);
				int Gc = qRound(colToDbl(rgb.g) * 255);
				int Bc = qRound(colToDbl(rgb.b) * 255);
				*s = qRgba(Rc, Gc, Bc, 255);
				for (int i = 0; i < colorMap->getNumPixelComps(); ++i)
				{
					if (pix[i] < maskColors[2*i] * 255 || pix[i] > maskColors[2*i+1] * 255)
					{
						*s = *s | 0xff000000;
						break;
					}
				}
				s++;
				pix += colorMap->getNumPixelComps();
			}
		}
	}
	else
	{
		image = new QImage(width, height, QImage::Format_ARGB32);
		for (int y = 0; y < height; y++)
		{
			QRgb *s = (QRgb*)(image->scanLine(y));
			Guchar *pix = imgStr->getLine();
			for (int x = 0; x < width; x++)
			{
				if (colorMap->getNumPixelComps() == 4)
				{
					GfxCMYK cmyk;
					colorMap->getCMYK(pix, &cmyk);
					int Cc = qRound(colToDbl(cmyk.c) * 255);
					int Mc = qRound(colToDbl(cmyk.m) * 255);
					int Yc = qRound(colToDbl(cmyk.y) * 255);
					int Kc = qRound(colToDbl(cmyk.k) * 255);
					*s = qRgba(Yc, Mc, Cc, Kc);
				}
				else
				{
					GfxRGB rgb;
					colorMap->getRGB(pix, &rgb);
					int Rc = qRound(colToDbl(rgb.r) * 255);
					int Gc = qRound(colToDbl(rgb.g) * 255);
					int Bc = qRound(colToDbl(rgb.b) * 255);
					*s = qRgba(Rc, Gc, Bc, 255);
				}
				s++;
				pix += colorMap->getNumPixelComps();
			}
		}
	}
	if (image == NULL || image->isNull())
	{
		delete imgStr;
		delete image;
		return;
	}
	double *ctm;
	ctm = state->getCTM();
	double xCoor = m_doc->currentPage()->xOffset();
	double yCoor = m_doc->currentPage()->yOffset();
	QRectF crect = QRectF(0, 0, width, height);
	m_ctm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, ctm[2] + ctm[4], ctm[3] + ctm[5]);
	QLineF cline = QLineF(0, 0, width, 0);
	QLineF tline = m_ctm.map(cline);
	QRectF trect = m_ctm.mapRect(crect);
	double sx = m_ctm.m11();
	double sy = m_ctm.m22();
	QTransform mm = QTransform(ctm[0] / width, ctm[1] / width, -ctm[2] / height, -ctm[3] / height, 0, 0);
	if ((mm.type() == QTransform::TxShear) || (mm.type() == QTransform::TxRotate))
	{
		mm.reset();
		mm.rotate(-tline.angle());
	}
	else
	{
		mm.reset();
		if (sx < 0)
			mm.scale(-1, 1);
		if (sy < 0)
			mm.scale(1, -1);
	}
	QImage img = image->transformed(mm);
	int z = m_doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, xCoor + trect.x(), yCoor + trect.y(), trect.width(), trect.height(), 0, CommonStrings::None, CommonStrings::None, true);
	PageItem* ite = m_doc->Items->at(z);
	ite->SetRectFrame();
	m_doc->setRedrawBounding(ite);
	ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->setFillShade(100);
	ite->setLineShade(100);
	ite->setFillEvenOdd(false);
	ite->setFillTransparency(1.0 - state->getFillOpacity());
	ite->setFillBlendmode(getBlendMode(state));
	m_doc->AdjustItemSize(ite);
	if (colorMap->getNumPixelComps() == 4)
	{
		QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_pdf_XXXXXX.tif");
		tempFile->setAutoRemove(false);
		if (tempFile->open())
		{
			QString fileName = getLongPathName(tempFile->fileName());
			if (!fileName.isEmpty())
			{
				tempFile->close();
				ite->isInlineImage = true;
				ite->isTempFile = true;
				TIFF* tif = TIFFOpen(fileName.toLocal8Bit().data(), "w");
				if (tif)
				{
					TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, img.width());
					TIFFSetField(tif, TIFFTAG_IMAGELENGTH, img.height());
					TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
					TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 4);
					TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
					TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_SEPARATED);
					TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
					for (int y = 0; y < img.height(); ++y)
					{
						TIFFWriteScanline(tif, img.scanLine(y), y);
					}
					TIFFClose(tif);
					m_doc->loadPict(fileName, ite);
				}
				ite->setImageScalingMode(false, false);
				m_Elements->append(ite);
				if (m_groupStack.count() != 0)
				{
					m_groupStack.top().Items.append(ite);
					applyMask(ite);
				}
			}
			else
				m_doc->Items->removeAll(ite);
		}
		delete tempFile;
	}
	else
	{
		QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_pdf_XXXXXX.png");
		tempFile->setAutoRemove(false);
		if (tempFile->open())
		{
			QString fileName = getLongPathName(tempFile->fileName());
			if (!fileName.isEmpty())
			{
				tempFile->close();
				ite->isInlineImage = true;
				ite->isTempFile = true;
				img.save(fileName, "PNG");
				m_doc->loadPict(fileName, ite);
				ite->setImageScalingMode(false, false);
				m_Elements->append(ite);
				if (m_groupStack.count() != 0)
				{
					m_groupStack.top().Items.append(ite);
					applyMask(ite);
				}
			}
			else
				m_doc->Items->removeAll(ite);
		}
		delete tempFile;
	}
	delete imgStr;
	delete image;
}

void SlaOutputDev::beginMarkedContent(char *name, Object *dictRef)
{
	mContent mSte;
	mSte.name = QString(name);
	mSte.ocgName = "";
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		if (dictRef->isNull())
			return;
		Object dictObj;
		Dict *dict;
		Object dictType;
		OCGs *contentConfig = catalog->getOptContentConfig();
		OptionalContentGroup *oc;
		if (dictRef->isRef())
		{
			oc = contentConfig->findOcgByRef(dictRef->getRef());
			if (oc)
			{
//				qDebug() << "Begin OCG Content (Ref) with Name " << QString(name) << "Layer" << UnicodeParsedString(oc->getName());
				m_doc->setActiveLayer(UnicodeParsedString(oc->getName()));
				mSte.ocgName = UnicodeParsedString(oc->getName());
			}
		}
		else
		{
			dictRef->fetch(xref, &dictObj);
			if (!dictObj.isDict())
			{
				dictObj.free();
				return;
			}
			dict = dictObj.getDict();
			dict->lookup("Type", &dictType);
			if (dictType.isName("OCG"))
			{
				oc = contentConfig->findOcgByRef(dictRef->getRef());
				if (oc)
				{
//					qDebug() << "Begin OCG Content with Name " << UnicodeParsedString(oc->getName());
					m_doc->setActiveLayer(UnicodeParsedString(oc->getName()));
					mSte.ocgName = UnicodeParsedString(oc->getName());
				}
			}
			dictType.free();
			dictObj.free();
		}
	}
	m_mcStack.push(mSte);
}

void SlaOutputDev::beginMarkedContent(char *name, Dict *properties)
{
//	qDebug() << "Begin Marked Content with Name " << QString(name);
	QString nam = QString(name);
	mContent mSte;
	mSte.name = nam;
	mSte.ocgName = "";
	m_mcStack.push(mSte);
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		if (nam == "Layer")		// Handle Adobe Illustrator Layer command
		{
			if (layersSetByOCG)
				return;
			Object obj;
			QString lName = QString("Layer_%1").arg(layerNum + 1);
			if (properties->lookup((char*)"Title", &obj))
			{
				if (obj.isString())
					lName =  QString(obj.getString()->getCString());
				obj.free();
			}
			for (ScLayers::iterator it = m_doc->Layers.begin(); it != m_doc->Layers.end(); ++it)
			{
				if (it->Name == lName)
				{
					m_doc->setActiveLayer(lName);
					return;
				}
			}
			layerNum++;
			if (!firstLayer)
				currentLayer = m_doc->addLayer(lName, true);
			firstLayer = false;
			if (properties->lookup((char*)"Visible", &obj))
			{
				if (obj.isBool())
					m_doc->setLayerVisible(currentLayer, obj.getBool());
				obj.free();
			}
			if (properties->lookup((char*)"Editable", &obj))
			{
				if (obj.isBool())
					m_doc->setLayerLocked(currentLayer, !obj.getBool());
				obj.free();
			}
			if (properties->lookup((char*)"Printed", &obj))
			{
				if (obj.isBool())
					m_doc->setLayerPrintable(currentLayer, obj.getBool());
				obj.free();
			}
			if (properties->lookup((char*)"Color", &obj))
			{
				if (obj.isArray())
				{
					Object obj1;
					obj.arrayGet(0, &obj1);
					int r = obj1.getNum() / 256;
					obj1.free();
					obj.arrayGet(1, &obj1);
					int g = obj1.getNum() / 256;
					obj1.free();
					obj.arrayGet(2, &obj1);
					int b = obj1.getNum() / 256;
					obj1.free();
					m_doc->setLayerMarker(currentLayer, QColor(r, g, b));
				}
				obj.free();
			}
		}
	}
}

void SlaOutputDev::endMarkedContent(GfxState *state)
{
//	qDebug() << "End Marked Content";
	if (m_mcStack.count() > 0)
	{
		mContent mSte = m_mcStack.pop();
		if (importerFlags & LoadSavePlugin::lfCreateDoc)
		{
			if (mSte.name == "OC")
			{
				for (ScLayers::iterator it = m_doc->Layers.begin(); it != m_doc->Layers.end(); ++it)
				{
					if (it->Name == mSte.ocgName)
					{
						m_doc->setActiveLayer(mSte.ocgName);
						return;
					}
				}
			}
		}
	}
}

void SlaOutputDev::markPoint(char *name)
{
//	qDebug() << "Begin Marked Point with Name " << QString(name);
}

void SlaOutputDev::markPoint(char *name, Dict *properties)
{
//	qDebug() << "Begin Marked Point with Name " << QString(name) << "and Properties";
	beginMarkedContent(name, properties);
}

// POPPLER_VERSION appeared in 0.19.0 first
#ifdef POPPLER_VERSION
void SlaOutputDev::updateFont(GfxState *state)
{
  GfxFont *gfxFont;
  GfxFontLoc *fontLoc;
  GfxFontType fontType;
  SplashOutFontFileID *id;
  SplashFontFile *fontFile;
  SplashFontSrc *fontsrc = NULL;
  FoFiTrueType *ff;
  Object refObj, strObj;
  GooString *fileName;
  char *tmpBuf;
  int tmpBufLen;
  int *codeToGID;
  double *textMat;
  double m11, m12, m21, m22, fontSize;
  SplashCoord mat[4];
  int n;
  int faceIndex = 0;
  SplashCoord matrix[6];

  m_font = NULL;
  fileName = NULL;
  tmpBuf = NULL;
  fontLoc = NULL;

  if (!(gfxFont = state->getFont())) {
	goto err1;
  }
  fontType = gfxFont->getType();
  if (fontType == fontType3) {
	goto err1;
  }

  // check the font file cache
  id = new SplashOutFontFileID(gfxFont->getID());
  if ((fontFile = m_fontEngine->getFontFile(id))) {
	delete id;

  } else {

	if (!(fontLoc = gfxFont->locateFont(xref, gFalse))) {
	  error(errSyntaxError, -1, "Couldn't find a font for '{0:s}'",
		gfxFont->getName() ? gfxFont->getName()->getCString()
						   : "(unnamed)");
	  goto err2;
	}

	// embedded font
	if (fontLoc->locType == gfxFontLocEmbedded) {
	  // if there is an embedded font, read it to memory
	  tmpBuf = gfxFont->readEmbFontFile(xref, &tmpBufLen);
	  if (! tmpBuf)
	goto err2;

	// external font
	} else { // gfxFontLocExternal
	  fileName = fontLoc->path;
	  fontType = fontLoc->fontType;
	}

	fontsrc = new SplashFontSrc;
	if (fileName)
	  fontsrc->setFile(fileName, gFalse);
	else
	  fontsrc->setBuf(tmpBuf, tmpBufLen, gTrue);

	// load the font file
	switch (fontType) {
	case fontType1:
	  if (!(fontFile = m_fontEngine->loadType1Font(
			   id,
			   fontsrc,
			   (const char **)((Gfx8BitFont *)gfxFont)->getEncoding()))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
		  gfxFont->getName() ? gfxFont->getName()->getCString()
							 : "(unnamed)");
	goto err2;
	  }
	  break;
	case fontType1C:
	  if (!(fontFile = m_fontEngine->loadType1CFont(
			   id,
			   fontsrc,
			   (const char **)((Gfx8BitFont *)gfxFont)->getEncoding()))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
		  gfxFont->getName() ? gfxFont->getName()->getCString()
							 : "(unnamed)");
	goto err2;
	  }
	  break;
	case fontType1COT:
	  if (!(fontFile = m_fontEngine->loadOpenTypeT1CFont(
			   id,
			   fontsrc,
			   (const char **)((Gfx8BitFont *)gfxFont)->getEncoding()))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
		  gfxFont->getName() ? gfxFont->getName()->getCString()
							 : "(unnamed)");
	goto err2;
	  }
	  break;
	case fontTrueType:
	case fontTrueTypeOT:
	if (fileName)
	 ff = FoFiTrueType::load(fileName->getCString());
	else
	ff = FoFiTrueType::make(tmpBuf, tmpBufLen);
	  if (ff) {
	codeToGID = ((Gfx8BitFont *)gfxFont)->getCodeToGIDMap(ff);
	n = 256;
	delete ff;
	  } else {
	codeToGID = NULL;
	n = 0;
	  }
	  if (!(fontFile = m_fontEngine->loadTrueTypeFont(
			   id,
			   fontsrc,
			   codeToGID, n))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
		  gfxFont->getName() ? gfxFont->getName()->getCString()
							 : "(unnamed)");
	goto err2;
	  }
	  break;
	case fontCIDType0:
	case fontCIDType0C:
	  if (!(fontFile = m_fontEngine->loadCIDFont(
			   id,
			   fontsrc))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
		  gfxFont->getName() ? gfxFont->getName()->getCString()
							 : "(unnamed)");
	goto err2;
	  }
	  break;
	case fontCIDType0COT:
	  if (((GfxCIDFont *)gfxFont)->getCIDToGID()) {
	n = ((GfxCIDFont *)gfxFont)->getCIDToGIDLen();
	codeToGID = (int *)gmallocn(n, sizeof(int));
	memcpy(codeToGID, ((GfxCIDFont *)gfxFont)->getCIDToGID(),
		   n * sizeof(int));
	  } else {
	codeToGID = NULL;
	n = 0;
	  }
	  if (!(fontFile = m_fontEngine->loadOpenTypeCFFFont(
			   id,
			   fontsrc,
			   codeToGID, n))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
		  gfxFont->getName() ? gfxFont->getName()->getCString()
							 : "(unnamed)");
	goto err2;
	  }
	  break;
	case fontCIDType2:
	case fontCIDType2OT:
	  codeToGID = NULL;
	  n = 0;
	  if (((GfxCIDFont *)gfxFont)->getCIDToGID()) {
	n = ((GfxCIDFont *)gfxFont)->getCIDToGIDLen();
	if (n) {
	  codeToGID = (int *)gmallocn(n, sizeof(int));
	  memcpy(codeToGID, ((GfxCIDFont *)gfxFont)->getCIDToGID(),
		  n * sizeof(Gushort));
	}
	  } else {
	if (fileName)
	  ff = FoFiTrueType::load(fileName->getCString());
	else
	  ff = FoFiTrueType::make(tmpBuf, tmpBufLen);
	if (! ff)
	  goto err2;
	codeToGID = ((GfxCIDFont *)gfxFont)->getCodeToGIDMap(ff, &n);
	delete ff;
	  }
	  if (!(fontFile = m_fontEngine->loadTrueTypeFont(
			   id,
			   fontsrc,
			   codeToGID, n, faceIndex))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
		  gfxFont->getName() ? gfxFont->getName()->getCString()
							 : "(unnamed)");
	goto err2;
	  }
	  break;
	default:
	  // this shouldn't happen
	  goto err2;
	}
  }

  // get the font matrix
  textMat = state->getTextMat();
  fontSize = state->getFontSize();
  m11 = textMat[0] * fontSize * state->getHorizScaling();
  m12 = textMat[1] * fontSize * state->getHorizScaling();
  m21 = textMat[2] * fontSize;
  m22 = textMat[3] * fontSize;
  matrix[0] = 1;
  matrix[1] = 0;
  matrix[2] = 0;
  matrix[3] = 1;
  matrix[4] = 0;
  matrix[5] = 0;
// create the scaled font
  mat[0] = m11;
  mat[1] = -m12;
  mat[2] = m21;
  mat[3] = -m22;
  m_font = m_fontEngine->getFont(fontFile, mat, matrix);

  delete fontLoc;
  if (fontsrc && !fontsrc->isFile)
	  fontsrc->unref();
  return;

 err2:
  delete id;
  delete fontLoc;
 err1:
  if (fontsrc && !fontsrc->isFile)
	  fontsrc->unref();
  return;
}
#else
void SlaOutputDev::updateFont(GfxState *state)
{
	GfxFont *gfxFont;
	GfxFontType fontType;
	SplashOutFontFileID *id;
	SplashFontFile *fontFile;
	SplashFontSrc *fontsrc = NULL;
	FoFiTrueType *ff;
	Ref embRef;
	Object refObj, strObj;
	GooString *fileName;
	char *tmpBuf;
	int tmpBufLen;
	Gushort *codeToGID;
	DisplayFontParam *dfp;
	double *textMat;
	double m11, m12, m21, m22, fontSize;
	SplashCoord mat[4];
	int n;
	int faceIndex = 0;
	SplashCoord matrix[6];

	m_font = NULL;
	fileName = NULL;
	tmpBuf = NULL;

	if (!(gfxFont = state->getFont()))
		goto err1;
	fontType = gfxFont->getType();
	if (fontType == fontType3)
		goto err1;

  // check the font file cache
	id = new SplashOutFontFileID(gfxFont->getID());
	if ((fontFile = m_fontEngine->getFontFile(id)))
	{
		delete id;
	}
	else
	{
		// if there is an embedded font, write it to disk
		if (gfxFont->getEmbeddedFontID(&embRef))
		{
			tmpBuf = gfxFont->readEmbFontFile(xref, &tmpBufLen);
			if (!tmpBuf)
				goto err2;
		// if there is an external font file, use it
		}
		else if (!(fileName = gfxFont->getExtFontFile()))
		{
	  // look for a display font mapping or a substitute font
			dfp = NULL;
			if (gfxFont->getName())
			{
				dfp = globalParams->getDisplayFont(gfxFont);
			}
			if (!dfp)
			{
		//		error(-1, "Couldn't find a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
				goto err2;
			}
			switch (dfp->kind)
			{
				case displayFontT1:
					fileName = dfp->t1.fileName;
					fontType = gfxFont->isCIDFont() ? fontCIDType0 : fontType1;
					break;
				case displayFontTT:
					fileName = dfp->tt.fileName;
					fontType = gfxFont->isCIDFont() ? fontCIDType2 : fontTrueType;
					faceIndex = dfp->tt.faceIndex;
					break;
			}
		}
		fontsrc = new SplashFontSrc;
		if (fileName)
			fontsrc->setFile(fileName, gFalse);
		else
			fontsrc->setBuf(tmpBuf, tmpBufLen, gTrue);
	// load the font file
		switch (fontType)
		{
			case fontType1:
				if (!(fontFile = m_fontEngine->loadType1Font( id, fontsrc, ((Gfx8BitFont *)gfxFont)->getEncoding())))
				{
			//		error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
					goto err2;
				}
				break;
			case fontType1C:
				if (!(fontFile = m_fontEngine->loadType1CFont( id, fontsrc, ((Gfx8BitFont *)gfxFont)->getEncoding())))
				{
		//			error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
					goto err2;
				}
				break;
			case fontType1COT:
				if (!(fontFile = m_fontEngine->loadOpenTypeT1CFont( id, fontsrc, ((Gfx8BitFont *)gfxFont)->getEncoding())))
				{
		//			error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
					goto err2;
				}
				break;
			case fontTrueType:
			case fontTrueTypeOT:
				if (fileName)
					ff = FoFiTrueType::load(fileName->getCString());
				else
					ff = FoFiTrueType::make(tmpBuf, tmpBufLen);
				if (ff)
				{
					codeToGID = ((Gfx8BitFont *)gfxFont)->getCodeToGIDMap(ff);
					n = 256;
					delete ff;
				}
				else
				{
					codeToGID = NULL;
					n = 0;
				}
				if (!(fontFile = m_fontEngine->loadTrueTypeFont( id, fontsrc, codeToGID, n)))
				{
	//				error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
					goto err2;
				}
			break;
		case fontCIDType0:
		case fontCIDType0C:
			if (!(fontFile = m_fontEngine->loadCIDFont( id, fontsrc)))
			{
	//			error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
				goto err2;
			}
			break;
		case fontCIDType0COT:
			if (!(fontFile = m_fontEngine->loadOpenTypeCFFFont( id, fontsrc)))
			{
	//			error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
				goto err2;
			}
			break;
		case fontCIDType2:
		case fontCIDType2OT:
			codeToGID = NULL;
			n = 0;
			if (((GfxCIDFont *)gfxFont)->getCIDToGID())
			{
				n = ((GfxCIDFont *)gfxFont)->getCIDToGIDLen();
				if (n)
				{
					codeToGID = (Gushort *)gmallocn(n, sizeof(Gushort));
					memcpy(codeToGID, ((GfxCIDFont *)gfxFont)->getCIDToGID(), n * sizeof(Gushort));
				}
			}
			else
			{
				if (fileName)
					ff = FoFiTrueType::load(fileName->getCString());
				else
					ff = FoFiTrueType::make(tmpBuf, tmpBufLen);
				if (!ff)
					goto err2;
				codeToGID = ((GfxCIDFont *)gfxFont)->getCodeToGIDMap(ff, &n);
				delete ff;
			}
			if (!(fontFile = m_fontEngine->loadTrueTypeFont( id, fontsrc, codeToGID, n, faceIndex)))
			{
	//			error(-1, "Couldn't create a font for '%s'", gfxFont->getName() ? gfxFont->getName()->getCString() : "(unnamed)");
				goto err2;
			}
			break;
		default:
	  // this shouldn't happen
			goto err2;
		}
	}
	// get the font matrix
	textMat = state->getTextMat();
	fontSize = state->getFontSize();
	m11 = textMat[0] * fontSize * state->getHorizScaling();
	m12 = textMat[1] * fontSize * state->getHorizScaling();
	m21 = textMat[2] * fontSize;
	m22 = textMat[3] * fontSize;
	matrix[0] = 1;
	matrix[1] = 0;
	matrix[2] = 0;
	matrix[3] = 1;
	matrix[4] = 0;
	matrix[5] = 0;
  // create the scaled font
	mat[0] = m11;
	mat[1] = -m12;
	mat[2] = m21;
	mat[3] = -m22;
	m_font = m_fontEngine->getFont(fontFile, mat, matrix);
	if (fontsrc && !fontsrc->isFile)
		fontsrc->unref();
	return;

err2:
	delete id;
err1:
	if (fontsrc && !fontsrc->isFile)
		fontsrc->unref();
	return;
}
#endif

void SlaOutputDev::drawChar(GfxState *state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode *u, int uLen)
{
	double x1, y1, x2, y2;
	int render;
	updateFont(state);
	if (!m_font)
		return;
  // check for invisible text -- this is used by Acrobat Capture
	render = state->getRender();
	if (render == 3)
		return;
	if (!(render & 1))
	{
		SplashPath * fontPath;
		fontPath = m_font->getGlyphPath(code);
		if (fontPath)
		{
			QPainterPath qPath;
			qPath.setFillRule(Qt::WindingFill);
			for (int i = 0; i < fontPath->getLength(); ++i)
			{
				Guchar f;
				fontPath->getPoint(i, &x1, &y1, &f);
				if (f & splashPathFirst)
					qPath.moveTo(x1,y1);
				else if (f & splashPathCurve)
				{
					double x3, y3;
					++i;
					fontPath->getPoint(i, &x2, &y2, &f);
					++i;
					fontPath->getPoint(i, &x3, &y3, &f);
					qPath.cubicTo(x1,y1,x2,y2,x3,y3);
				}
				else
					qPath.lineTo(x1,y1);
				if (f & splashPathLast)
					qPath.closeSubpath();
			}
			double *ctm;
			ctm = state->getCTM();
			m_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
			double xCoor = m_doc->currentPage()->xOffset();
			double yCoor = m_doc->currentPage()->yOffset();
			FPointArray textPath;
			textPath.fromQPainterPath(qPath);
			FPoint wh = textPath.WidthHeight();
			if ((textPath.size() > 3) && ((wh.x() != 0.0) || (wh.y() != 0.0)))
			{
				int z = m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, xCoor, yCoor, 10, 10, 0, CurrColorFill, CommonStrings::None, true);
				PageItem* ite = m_doc->Items->at(z);
				FPoint tp2(getMinClipF(&textPath));
				QTransform mm;
				mm.scale(1, -1);
				mm.translate(x, -y);
				textPath.map(mm);
				textPath.map(m_ctm);
				ite->PoLine = textPath.copy();
				ite->ClipEdited = true;
				ite->FrameType = 3;
				ite->setFillShade(CurrFillShade);
				ite->setFillEvenOdd(false);
				ite->setFillTransparency(1.0 - state->getFillOpacity());
				ite->setFillBlendmode(getBlendMode(state));
				ite->setLineEnd(PLineEnd);
				ite->setLineJoin(PLineJoin);
				ite->setTextFlowMode(PageItem::TextFlowDisabled);
				m_doc->AdjustItemSize(ite);
				if ((render & 3) == 1 || (render & 3) == 2)
				{
					ite->setLineColor(CurrColorStroke);
					ite->setLineWidth(state->getTransformedLineWidth());
					ite->setLineTransparency(1.0 - state->getStrokeOpacity());
					ite->setLineBlendmode(getBlendMode(state));
					ite->setLineShade(CurrStrokeShade);
				}
				m_Elements->append(ite);
				if (m_groupStack.count() != 0)
				{
					m_groupStack.top().Items.append(ite);
					applyMask(ite);
				}
				delete fontPath;
			}
		}
	}
}

GBool SlaOutputDev::beginType3Char(GfxState *state, double x, double y, double dx, double dy, CharCode code, Unicode *u, int uLen)
{
//	qDebug() << "beginType3Char";
	double *ctm;
	ctm = state->getCTM();
	QTransform orig_ctm = QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	GfxFont *gfxFont;
	GfxFontType fontType;
	if (!(gfxFont = state->getFont()))
		return gTrue;
	fontType = gfxFont->getType();
	if (fontType != fontType3)
		return gTrue;
	Ref* fref = gfxFont->getID();
	QString fRefID = QString("Font_%1_%2").arg(fref->num).arg(code);
	if (m_Font_Pattern_Map.contains(fRefID))
	{
		QLineF cline = QLineF(0, 0, 1, 0);
		QLineF tline = orig_ctm.map(cline);
		double xCoor = m_doc->currentPage()->xOffset();
		double yCoor = m_doc->currentPage()->yOffset();
		ScPattern pat = m_doc->docPatterns[m_Font_Pattern_Map[fRefID].pattern];
		QTransform mm;
		mm.translate(0, -pat.height * tline.length());
		mm = orig_ctm * mm;
		int z = 0;
		if (m_Font_Pattern_Map[fRefID].colored)
			z = m_doc->itemAdd(PageItem::Symbol, PageItem::Unspecified, xCoor + mm.dx(), yCoor + mm.dy(), pat.width * tline.length(), pat.height * tline.length(), 0, CommonStrings::None, CommonStrings::None, true);
		else
			z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + mm.dx(), yCoor + mm.dy(), pat.width * tline.length(), pat.height * tline.length(), 0, CurrColorFill, CommonStrings::None, true);
		PageItem *b = m_doc->Items->at(z);
		b->setWidth(pat.width * tline.length());
		b->setHeight(pat.height * tline.length());
		b->OldB2 = b->width();
		b->OldH2 = b->height();
		m_doc->RotMode(3);
		m_doc->RotateItem(-tline.angle(), b);
		m_doc->RotMode(0);
		b->setFillShade(CurrFillShade);
		b->setFillEvenOdd(false);
		b->setFillTransparency(1.0 - state->getFillOpacity());
		b->setFillBlendmode(getBlendMode(state));
		b->setLineEnd(PLineEnd);
		b->setLineJoin(PLineJoin);
		b->setTextFlowMode(PageItem::TextFlowDisabled);
		if (m_Font_Pattern_Map[fRefID].colored)
			b->setPattern(m_Font_Pattern_Map[fRefID].pattern);
		else
		{
			b->setFillShade(CurrFillShade);
			b->setPatternMask(m_Font_Pattern_Map[fRefID].pattern);
			b->setMaskTransform(b->width() / pat.width * 100, b->height() / pat.height * 100, 0, 0, 0, 0, 0);
			b->setMaskType(3);
		}
		m_Elements->append(b);
		if (m_groupStack.count() != 0)
		{
			m_groupStack.top().Items.append(b);
		}
		return gTrue;
	}
	else
	{
		F3Entry f3e;
		f3e.ctm = orig_ctm;
		f3e.glyphRef = fRefID;
		f3e.colored = false;
		m_F3Stack.push(f3e);
		ctm = state->getTextMat();
		state->setCTM(ctm[0], ctm[1], ctm[2], ctm[3], 0, 0);
		pushGroup();
		return gFalse;
	}
}

void SlaOutputDev::endType3Char(GfxState *state)
{
//	qDebug() << "endType3Char";
	double *ctm;
	ctm = state->getCTM();
	groupEntry gElements = m_groupStack.pop();
	m_doc->m_Selection->clear();
	if (gElements.Items.count() > 0)
	{
		for (int dre = 0; dre < gElements.Items.count(); ++dre)
		{
			m_doc->m_Selection->addItem(gElements.Items.at(dre), true);
			m_Elements->removeAll(gElements.Items.at(dre));
		}
		m_doc->itemSelection_FlipV();
		PageItem *ite;
		if (m_doc->m_Selection->count() > 1)
			ite = m_doc->groupObjectsSelection();
		else
			ite = m_doc->m_Selection->itemAt(0);
		ite->setFillTransparency(1.0 - state->getFillOpacity());
		ite->setFillBlendmode(getBlendMode(state));
		m_doc->m_Selection->clear();
		ScPattern pat = ScPattern();
		pat.setDoc(m_doc);
		m_doc->DoDrawing = true;
		pat.pattern = ite->DrawObj_toImage(qMax(ite->width(), ite->height()));
		pat.xoffset = 0;
		pat.yoffset = 0;
		m_doc->DoDrawing = false;
		pat.width = ite->width();
		pat.height = ite->height();
		ite->gXpos = 0;
		ite->gYpos = 0;
		ite->setXYPos(ite->gXpos, ite->gYpos, true);
		pat.items.append(ite);
		m_doc->Items->removeAll(ite);
		QString id = QString("Pattern_from_PDF_%1T").arg(m_doc->docPatterns.count() + 1);
		m_doc->addPattern(id, pat);
		tmpSel->clear();
		if (m_F3Stack.count() > 0)
		{
			F3Entry f3e = m_F3Stack.pop();
			state->setCTM(f3e.ctm.m11(), f3e.ctm.m12(), f3e.ctm.m21(), f3e.ctm.m22(), f3e.ctm.dx(), f3e.ctm.dy());
			QLineF cline = QLineF(0, 0, 1, 0);
			QLineF tline = f3e.ctm.map(cline);
			double xCoor = m_doc->currentPage()->xOffset();
			double yCoor = m_doc->currentPage()->yOffset();
			QTransform mm;
			mm.translate(0, -pat.height * tline.length());
			mm = f3e.ctm * mm;
		//	int shade = 100;
		//	CurrColorFill = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
			int z = 0;
			if (f3e.colored)
				z = m_doc->itemAdd(PageItem::Symbol, PageItem::Unspecified, xCoor + mm.dx(), yCoor + mm.dy(), pat.width * tline.length(), pat.height * tline.length(), 0, CommonStrings::None, CommonStrings::None, true);
			else
				z = m_doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, xCoor + mm.dx(), yCoor + mm.dy(), pat.width * tline.length(), pat.height * tline.length(), 0, CurrColorFill, CommonStrings::None, true);
			PageItem *b = m_doc->Items->at(z);
			b->setWidth(pat.width * tline.length());
			b->setHeight(pat.height * tline.length());
			b->OldB2 = b->width();
			b->OldH2 = b->height();
			m_doc->RotMode(3);
			m_doc->RotateItem(-tline.angle(), b);
			m_doc->RotMode(0);
			b->setFillEvenOdd(false);
			b->setFillTransparency(1.0 - state->getFillOpacity());
			b->setFillBlendmode(getBlendMode(state));
			b->setLineEnd(PLineEnd);
			b->setLineJoin(PLineJoin);
			b->setTextFlowMode(PageItem::TextFlowDisabled);
			if (f3e.colored)
				b->setPattern(id);
			else
			{
				b->setFillShade(CurrFillShade);
				b->setPatternMask(id);
				b->setMaskTransform(b->width() / pat.width * 100, b->height() / pat.height * 100, 0, 0, 0, 0, 0);
				b->setMaskType(3);
			}
			m_Elements->append(b);
			if (m_groupStack.count() != 0)
			{
				m_groupStack.top().Items.append(b);
			}
			F3GlyphEntry gid;
			gid.colored = f3e.colored;
			gid.pattern = id;
			m_Font_Pattern_Map.insert(f3e.glyphRef, gid);
		}
	}
}

void SlaOutputDev::type3D0(GfxState * /*state*/, double /*wx*/, double /*wy*/)
{
//	qDebug() << "type3D0";
	if (m_F3Stack.count() > 0)
		m_F3Stack.top().colored = true;
}

void SlaOutputDev::type3D1(GfxState *state, double wx, double wy, double llx, double lly, double urx, double ury)
{
//	qDebug() << "type3D1";
	if (m_F3Stack.count() > 0)
		m_F3Stack.top().colored = false;
}

void SlaOutputDev::beginTextObject(GfxState *state)
{
	pushGroup();
}

void SlaOutputDev::endTextObject(GfxState *state)
{
//	qDebug() << "End Text Object";
	if (m_groupStack.count() != 0)
	{
		groupEntry gElements = m_groupStack.pop();
		tmpSel->clear();
		if (gElements.Items.count() > 0)
		{
			for (int dre = 0; dre < gElements.Items.count(); ++dre)
			{
				tmpSel->addItem(gElements.Items.at(dre), true);
				m_Elements->removeAll(gElements.Items.at(dre));
			}
			PageItem *ite;
			if (gElements.Items.count() != 1)
				ite = m_doc->groupObjectsSelection(tmpSel);
			else
				ite = gElements.Items.first();
			ite->setFillTransparency(1.0 - state->getFillOpacity());
			ite->setFillBlendmode(getBlendMode(state));
			for (int as = 0; as < tmpSel->count(); ++as)
			{
				m_Elements->append(tmpSel->itemAt(as));
			}
			if (m_groupStack.count() != 0)
				applyMask(ite);
		}
		if (m_groupStack.count() != 0)
		{
			for (int as = 0; as < tmpSel->count(); ++as)
			{
				m_groupStack.top().Items.append(tmpSel->itemAt(as));
			}
		}
		tmpSel->clear();
	}
}

QString SlaOutputDev::getColor(GfxColorSpace *color_space, GfxColor *color, int *shade)
{
	QString fNam;
	QString namPrefix = "FromPDF";
	ScColor tmp;
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	*shade = 100;
	if (m_F3Stack.count() > 0)
	{
		if (!m_F3Stack.top().colored)
			return "Black";
	}
	if ((color_space->getMode() == csDeviceRGB) || (color_space->getMode() == csCalRGB))
	{
		GfxRGB rgb;
		color_space->getRGB(color, &rgb);
		int Rc = qRound(colToDbl(rgb.r) * 255);
		int Gc = qRound(colToDbl(rgb.g) * 255);
		int Bc = qRound(colToDbl(rgb.b) * 255);
		tmp.setColorRGB(Rc, Gc, Bc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	}
	else if (color_space->getMode() == csDeviceCMYK)
	{
		GfxCMYK cmyk;
		color_space->getCMYK(color, &cmyk);
		int Cc = qRound(colToDbl(cmyk.c) * 255);
		int Mc = qRound(colToDbl(cmyk.m) * 255);
		int Yc = qRound(colToDbl(cmyk.y) * 255);
		int Kc = qRound(colToDbl(cmyk.k) * 255);
		tmp.setColor(Cc, Mc, Yc, Kc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	}
	else if ((color_space->getMode() == csCalGray) || (color_space->getMode() == csDeviceGray))
	{
		GfxGray gray;
		color_space->getGray(color, &gray);
		int Kc = 255 - qRound(colToDbl(gray) * 255);
		tmp.setColor(0, 0, 0, Kc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	}
	else if (color_space->getMode() == csSeparation)
	{
		GfxCMYK cmyk;
		color_space->getCMYK(color, &cmyk);
		int Cc = qRound(colToDbl(cmyk.c) * 255);
		int Mc = qRound(colToDbl(cmyk.m) * 255);
		int Yc = qRound(colToDbl(cmyk.y) * 255);
		int Kc = qRound(colToDbl(cmyk.k) * 255);
		tmp.setColor(Cc, Mc, Yc, Kc);
		tmp.setSpotColor(true);
		QString nam = QString(((GfxSeparationColorSpace*)color_space)->getName()->getCString());
		fNam = m_doc->PageColors.tryAddColor(nam, tmp);
		*shade = qRound(colToDbl(color->c[0]) * 100);
	}
	else
	{
		GfxRGB rgb;
		color_space->getRGB(color, &rgb);
		int Rc = qRound(colToDbl(rgb.r) * 255);
		int Gc = qRound(colToDbl(rgb.g) * 255);
		int Bc = qRound(colToDbl(rgb.b) * 255);
		tmp.setColorRGB(Rc, Gc, Bc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	//	qDebug() << "update fill color other colorspace" << color_space->getMode() << "treating as rgb" << Rc << Gc << Bc;
	}
	if (fNam == namPrefix+tmp.name())
		m_importedColors->append(fNam);
	return fNam;
}

QString SlaOutputDev::getAnnotationColor(AnnotColor *color)
{
	QString fNam;
	QString namPrefix = "FromPDF";
	ScColor tmp;
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	if (color->getSpace() == AnnotColor::colorTransparent)
		return CommonStrings::None;
	else if (color->getSpace() == AnnotColor::colorRGB)
	{
		const double *color_data = color->getValues();
		int Rc = qRound(color_data[0] * 255);
		int Gc = qRound(color_data[1] * 255);
		int Bc = qRound(color_data[2] * 255);
		tmp.setColorRGB(Rc, Gc, Bc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	}
	else if (color->getSpace() == AnnotColor::colorCMYK)
	{
		const double *color_data = color->getValues();
		int Cc = qRound(color_data[0] * 255);
		int Mc = qRound(color_data[1] * 255);
		int Yc = qRound(color_data[2] * 255);
		int Kc = qRound(color_data[3] * 255);
		tmp.setColor(Cc, Mc, Yc, Kc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	}
	else if (color->getSpace() == AnnotColor::colorGray)
	{
		const double *color_data = color->getValues();
		int Kc = 255 - qRound(color_data[0] * 255);
		tmp.setColor(0, 0, 0, Kc);
		fNam = m_doc->PageColors.tryAddColor(namPrefix+tmp.name(), tmp);
	}
	if (fNam == namPrefix+tmp.name())
		m_importedColors->append(fNam);
	return fNam;
}

QString SlaOutputDev::convertPath(GfxPath *path)
{
    if (! path)
		return QString();

    QString output;
	pathIsClosed = false;

	for (int i = 0; i < path->getNumSubpaths(); ++i)
	{
        GfxSubpath * subpath = path->getSubpath(i);
		if (subpath->getNumPoints() > 0)
		{
			output += QString("M %1 %2").arg(subpath->getX(0)).arg(subpath->getY(0));
            int j = 1;
			while (j < subpath->getNumPoints())
			{
				if (subpath->getCurve(j))
				{
					output += QString("C %1 %2 %3 %4 %5 %6")
                              .arg(subpath->getX(j)).arg(subpath->getY(j))
                              .arg(subpath->getX(j + 1)).arg(subpath->getY(j + 1))
                              .arg(subpath->getX(j + 2)).arg(subpath->getY(j + 2));
                    j += 3;
				}
				else
				{
					output += QString("L %1 %2").arg(subpath->getX(j)).arg(subpath->getY(j));
                    ++j;
                }
            }
			if (subpath->isClosed())
			{
                output += QString("Z");
				pathIsClosed = true;
            }
        }
	}
	return output;
}

void SlaOutputDev::getPenState(GfxState *state)
{
	switch (state->getLineCap())
	{
		case 0:
			PLineEnd = Qt::FlatCap;
			break;
		case 1:
			PLineEnd = Qt::RoundCap;
			break;
		case 2:
			PLineEnd = Qt::SquareCap;
			break;
	}
	switch (state->getLineJoin())
	{
		case 0:
			PLineJoin = Qt::MiterJoin;
			break;
		case 1:
			PLineJoin = Qt::RoundJoin;
			break;
		case 2:
			PLineJoin = Qt::BevelJoin;
			break;
	}
	double lw = state->getLineWidth();
	double *dashPattern;
	int dashLength;
	state->getLineDash(&dashPattern, &dashLength, &DashOffset);
	QVector<double> pattern(dashLength);
	for (int i = 0; i < dashLength; ++i)
	{
		pattern[i] = dashPattern[i] / lw;
	}
	DashValues = pattern;
}

int SlaOutputDev::getBlendMode(GfxState *state)
{
	int mode = 0;
	switch (state->getBlendMode())
	{
		default:
		case gfxBlendNormal:
			mode = 0;
			break;
		case gfxBlendDarken:
			mode = 1;
			break;
		case gfxBlendLighten:
			mode = 2;
			break;
		case gfxBlendMultiply:
			mode = 3;
			break;
		case gfxBlendScreen:
			mode = 4;
			break;
		case gfxBlendOverlay:
			mode = 5;
			break;
		case gfxBlendHardLight:
			mode = 6;
			break;
		case gfxBlendSoftLight:
			mode = 7;
			break;
		case gfxBlendDifference:
			mode = 8;
			break;
		case gfxBlendExclusion:
			mode = 9;
			break;
		case gfxBlendColorDodge:
			mode = 10;
			break;
		case gfxBlendColorBurn:
			mode = 11;
			break;
		case gfxBlendHue:
			mode = 12;
			break;
		case gfxBlendSaturation:
			mode = 13;
			break;
		case gfxBlendColor:
			mode = 14;
			break;
		case gfxBlendLuminosity:
			mode = 15;
			break;
	}
	return mode;
}

void SlaOutputDev::applyMask(PageItem *ite)
{
	if (m_groupStack.count() != 0)
	{
		if (!m_groupStack.top().maskName.isEmpty())
		{
			ite->setPatternMask(m_groupStack.top().maskName);
			if (m_groupStack.top().alpha)
			{
				if (m_groupStack.top().inverted)
					ite->setMaskType(8);
				else
					ite->setMaskType(3);
			}
			else
			{
				if (m_groupStack.top().inverted)
					ite->setMaskType(7);
				else
					ite->setMaskType(6);
			}
		}
	}
	// Code for updating our Progressbar, needs to be called this way, as we have no
	// possibility to get the current fileposition.
	updateGUICounter++;
	if (updateGUICounter > 20)
	{
		qApp->processEvents();
		updateGUICounter = 0;
	}
}

void SlaOutputDev::pushGroup(QString maskName, GBool forSoftMask, GBool alpha, bool inverted)
{
	groupEntry gElements;
	gElements.forSoftMask = forSoftMask;
	gElements.alpha = alpha;
	gElements.inverted = inverted;
	gElements.maskName = maskName;
	m_groupStack.push(gElements);
}

QString SlaOutputDev::UnicodeParsedString(GooString *s1)
{
	if ( !s1 || s1->getLength() == 0 )
		return QString();
	GBool isUnicode;
	int i;
	Unicode u;
	QString result;
	if ((s1->getChar(0) & 0xff) == 0xfe && (s1->getLength() > 1 && (s1->getChar(1) & 0xff) == 0xff))
	{
		isUnicode = gTrue;
		i = 2;
		result.reserve((s1->getLength() - 2) / 2);
	}
	else
	{
		isUnicode = gFalse;
		i = 0;
		result.reserve(s1->getLength());
	}
	while (i < s1->getLength())
	{
		if (isUnicode)
		{
			u = ((s1->getChar(i) & 0xff) << 8) | (s1->getChar(i+1) & 0xff);
			i += 2;
		}
		else
		{
			u = s1->getChar(i) & 0xff;
			++i;
		}
		result += QChar( u );
	}
	return result;
}

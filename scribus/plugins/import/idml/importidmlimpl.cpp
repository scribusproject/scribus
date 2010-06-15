/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "importidmlimpl.h"

#include <QByteArray>
#include <QCursor>
#include <QDrag>
#include <QFile>
#include <QMimeData>
#include <QRegExp>
#include <QTextCodec>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QMessageBox>
#include <QPointF>

#include "commonstrings.h"
#include "docpreference.h"
#include "ui/customfdialog.h"
#include "fileunzip.h"
#include "importidml.h"
#include "loadsaveplugin.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "pageitem_imageframe.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pagesize.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "ui/propertiespalette.h"
#include "rawimage.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scribusdoc.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "sctextstream.h"
#include "selection.h"
#include "textwriter.h"
#include "undomanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"


// Initialize members here, if any
ImportIdmlImpl::ImportIdmlImpl() : QObject(0)
{
}

bool ImportIdmlImpl::run(const QString & target, ScribusDoc* doc)
{
	//Do the bulk of your work here
	m_Doc=doc;
	QString fnameIn = target;
	//	bool success = false;
	bool showProgress;
	//cancel = false;
	double x,y,b,h;
	//	bool ret = false;
	QFileInfo fi = QFileInfo(fnameIn);
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	if(!ScCore->usingGUI())
	showProgress=false;
	/* Implement Progress Dialog Later */
	
	x = 0.0;
	y = 0.0;
	b = 0.0;
	h = 0.0;
	
	/**
	Here, first we parse the idml document, in the order designmap.xml,preferences.xml, fonts.xml, styles.xml, gradiant.xml, all the stories will be transformed into Story Objects, and finally all the master spreads and spreads.
	We set the document page width, height etc from preferences.xml, styles are defined and added into the list.
	
	QMap are maintained with QString, Object for each item which is referred later in the document using the self attribute. This includes all styles, stories etc.
	*/
	FileUnzip* idmlFile = new FileUnzip(target);
	desmapPath = idmlFile->getFile("designmap.xml");
	fontPath = idmlFile->getFile("Resources/Fonts.xml");
	stylePath = idmlFile->getFile("Resources/Styles.xml");
	graphicPath = idmlFile->getFile("Resources/Graphic.xml");
	preferPath = idmlFile->getFile("Resources/Preferences.xml");
	if(!preferPath.isEmpty())
	{
		root=loadXmlFile(preferPath);
		bool success = preference.processFile(root);
		root.clear();
		if(!success)
			return false;
	}
	docWidth = preference.preference.PageWidth.toDouble();
	docHeight = preference.preference.PageHeight.toDouble();
	double leftMargin = preference.marginPreference.left.toDouble();
	double rightMargin = preference.marginPreference.right.toDouble();
	double topMargin = preference.marginPreference.top.toDouble();
	double bottomMargin = preference.marginPreference.bottom.toDouble();
	m_Doc=ScCore->primaryMainWindow()->doFileNew(docWidth, docHeight, topMargin, leftMargin, rightMargin, bottomMargin, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
	Elements.clear();
	m_Doc->GroupCounter = 1;
	m_Doc->LastAuto = 0;
	m_Doc->PageColors.clear();
	m_Doc->Layers.clear();
	layerMap.clear();
	layerID = -1;

	if(!desmapPath.isNull())
	{
		bool success = processDesignMap();
		for( int i=0;i<storyList.count();i++ )
		storyPath.append(idmlFile->getFile(storyList.at(i)));
		for( int i=0;i<Spreads.count();i++ )
		spreadPath.append(idmlFile->getFile(Spreads.at(i)));
		for( int i=0;i<MasterSpreads.count();i++ )
		mspreadPath.append(idmlFile->getFile(MasterSpreads.at(i)));
		if(!success)
			return false;
	}
	if(!fontPath.isNull())
	{
		bool success = processFont();
		root.clear();
		if(!success)
			return false;
	}
	if(!stylePath.isEmpty())
	{
		bool success = processStyles();
		root.clear();
		if(!success)
			return false;
	}
	if(!storyPath.isEmpty())
	{
		bool success = processStory();
		root.clear();
		if(!success)
			return false;
	}
	if(!spreadPath.isEmpty())
	{
		bool success = processSpreads();
		root.clear();
		if(!success)
			return false;
	}
	return true;
}

// Function for loading xml file for parsing

QDomElement ImportIdmlImpl::loadXmlFile(QString &filename)
{
	QFile file(filename);
	qDebug()<<"File Name = "<<filename;
	QDomDocument docu;
	QDomElement rootElement;
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	docu.setContent(&file);
	rootElement = docu.documentElement();
	qDebug()<<"root tag = "<<rootElement.tagName();
	file.close();
	return rootElement;
}

bool ImportIdmlImpl::processDesignMap()
{
	QDomElement designmapRoot=loadXmlFile(desmapPath);
	if(designmapRoot.tagName() != "Document")
	return false;
	else
	{
		/*get the attributes of Document Tag before going into its children */
		//docAttributes(designmapRoot);
		node = designmapRoot.firstChild();
		while(!node.isNull())
		{
			element = node.toElement();
			if(!element.isNull())
			{
				if(element.tagName()=="Layer")
				{
					ScLayer newLayer;
					handleLayer(newLayer,element);
					m_Doc->Layers.append(newLayer);
				}
				if(element.tagName()=="Section")
				{
					handleSection(m_Doc,element);
				}
				if(element.tagName()=="idPkg:Spread")
					getSpreadSrc(element);
				if(element.tagName()=="idPkg:MasterSpread")
					getMSpreadSrc(element);
				if(element.tagName()=="idPkg:Story")
					getStorySrc(element);
			}
			node = node.nextSibling();
		}
		return true;
	}
}

void docAttributes(QDomElement& element)
{
}

/* Function to handle the layers */

void ImportIdmlImpl::handleLayer(ScLayer& layer,QDomElement& element)
{
	QString self = element.attribute("Self","");
	layer.Name=element.attribute("Name","");
	layer.isEditable=(element.attribute("Locked","")=="true")?true:false;
	layer.isPrintable=(element.attribute("Printable","")=="true")?true:false;
	layer.flowControl=(element.attribute("IgnoreWrap","")=="true")?true:false;
	layer.isViewable=(element.attribute("Visible","")=="true")?true:false;
	layer.outlineMode=(element.attribute("ShowGuides","")=="true")?true:false;
	layer.ID = layerID + 1;
	//descending down to properties of layer
	QDomNode n = node.firstChild().firstChild();
	QDomElement lcolor = n.toElement();
	layer.markerColor=lcolor.text();
	layerMap.insert(self,layer.ID);
}

void ImportIdmlImpl::handleSection(ScribusDoc* doc, QDomElement& element)
{
}

void ImportIdmlImpl::getMSpreadSrc(QDomElement& element)
{
	MasterSpreads.append(element.attribute("src",""));
}

void ImportIdmlImpl::getSpreadSrc(QDomElement& element)
{
	Spreads.append(element.attribute("src",""));
}

void ImportIdmlImpl::getStorySrc(QDomElement& element)
{
	storyList.append(element.attribute("src",""));
}

bool ImportIdmlImpl::processFont()
{
	return true;
}

bool ImportIdmlImpl::processStyles()
{
	QDomElement stylesRoot=loadXmlFile(stylePath);
	if(stylesRoot.tagName() != "idPkg:Styles")
	return false;
	else
	{
		node=stylesRoot.firstChild();
		while(!node.isNull())
		{
			element=node.toElement();
			if(!element.isNull())
			{
				if(element.tagName()=="RootParagraphStyleGroup")
				if(!importPStyles(node))
					return false;
				if(element.tagName()=="RootCharacterStyleGroup")
					if(!importCStyles(node))
						return false;
			}
			node=node.nextSibling();
		}
	}
	return true;
}

bool ImportIdmlImpl::importPStyles(QDomNode &node)
{
	node=node.firstChild();
	while(!node.isNull())
	{
		QDomElement elem = node.toElement();
		if(!elem.isNull())
		{
			if(elem.tagName()=="ParagraphStyle")
			{
				readParagraphStyle(m_Doc, node, vg, *m_AvailableFonts);
				StyleSet<ParagraphStyle>tmp;
				tmp.create(vg);
				m_Doc->redefineStyles(tmp, false);
			}
			if(elem.tagName()=="ParagraphStyleGroup")
			{
				readParagraphStyle(m_Doc,node.firstChild(),vg,*m_AvailableFonts);
				StyleSet<ParagraphStyle>tmp;
				tmp.create(vg);
				m_Doc->redefineStyles(tmp, false);
			}
			if(elem.tagName()=="CharacterStyle")
			{
				readCharacterStyle(m_Doc, node, cs);
				StyleSet<CharStyle> temp;
				temp.create(cs);
				m_Doc->redefineCharStyles(temp, false);
			}
			if(elem.tagName()=="CharacterStyleGroup")
			{
				readCharacterStyle(m_Doc, node.firstChild(), cs);
				StyleSet<CharStyle> temp;
				temp.create(cs);
				m_Doc->redefineCharStyles(temp,false);
			}
		}
		node=node.nextSibling();
	}
	return true;
}

void ImportIdmlImpl::readParagraphStyle(ScribusDoc *doc, QDomNode node,  ParagraphStyle &newStyle, SCFonts &fonts)
{
	/* Parsing out the attributes of ParagraphStyle those needed to form a paragraph style of Scribus */
	QDomElement elem=node.toElement();
	QStringList features;
	newStyle.erase();
	newStyle.setName(elem.attribute("Name",""));
	newStyle.setDefaultStyle(false);
	newStyle.setLeftMargin(elem.attribute("LeftIndent","").toDouble());
	newStyle.setRightMargin(elem.attribute("RightMargin","").toDouble());
	newStyle.setFirstIndent(elem.attribute("FirstLineIndent","").toDouble());
	//newStyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(elem.attribute("Justification","")));
	newStyle.setGapBefore(elem.attribute("SpaceBefore","").toDouble());
	newStyle.setGapAfter(elem.attribute("SpaceAfter","").toDouble());
	newStyle.setDropCapLines(elem.attribute("DropCapLines","").toInt());
	newStyle.setMinWordTracking(elem.attribute("MinimumWordSpacing","").toDouble());
	newStyle.setMinGlyphExtension(elem.attribute("MinimumGlyphScaling","").toDouble());
	newStyle.setMaxGlyphExtension(elem.attribute("MaximumGlyphScaling","").toDouble());

	newStyle.charStyle().setFontSize((qRound(elem.attribute("PointSize","").toDouble()) * 10));
	newStyle.charStyle().setWordTracking(elem.attribute("DesiredWordSpacing","").toDouble());
	newStyle.charStyle().setTracking(elem.attribute("Tracking","").toDouble());

	if(elem.attribute("Underline","")=="true")
	features.append(CharStyle::UNDERLINE);
	if(elem.attribute("StrikeThru","")=="true")
	features.append(CharStyle::STRIKETHROUGH);
	/*if(elem.attribute("Capitalization","")=="")
		features.append(CharStyle::ALLCAPS);
	if(elem.attribute("Capitalization","")=="")
		features.append(CharStyle::SMALLCAPS);*/
	newStyle.charStyle().setFeatures(features);

	QString fontStyle = elem.attribute("FontStyle","");
	newStyle.charStyle().setScaleH(qRound((elem.attribute("HorizontalScale","").toDouble()) * 10));
	newStyle.charStyle().setScaleV(qRound((elem.attribute("VerticalScale","").toDouble()) * 10));
	newStyle.charStyle().setBaselineOffset(qRound( (elem.attribute("BaselineShift","").toDouble()) * 10));
	newStyle.charStyle().setStrokeColor(elem.attribute("StrokeColor",""));
	newStyle.charStyle().setFillColor(elem.attribute("FillColor",""));
	newStyle.charStyle().setFillShade((elem.attribute("FillTint","").toDouble()) * 10);
	newStyle.charStyle().setStrokeShade((elem.attribute("StrokeTint","").toDouble()) * 10);
	newStyle.charStyle().setUnderlineOffset(qRound((elem.attribute("UnderlineOffset","").toDouble()) * 10));
	newStyle.charStyle().setUnderlineWidth(qRound((elem.attribute("UnderlineWidth","").toDouble()) * 10));
	newStyle.charStyle().setStrikethruOffset(qRound((elem.attribute("StrikeThroughOffset","").toDouble())* 10));
	newStyle.charStyle().setStrikethruWidth(qRound((elem.attribute("StrikeThroughWidth","").toDouble()) * 10));
	newStyle.charStyle().setLanguage(elem.attribute("AppliedLanguage",""));

	node = node.firstChild();
	QList<ParagraphStyle::TabRecord> tbs;
	newStyle.resetTabValues();
	while(!node.isNull())
	{
		QDomElement e = node.toElement();
		if(!e.isNull())
		{
			if(e.tagName()=="BasedOn")
			newStyle.setParent(e.text());
			if(e.tagName()=="AppliedFont")
			{
				newStyle.charStyle().setFont(m_AvailableFonts->findFont(e.text()+fontStyle, doc));
			}
			if(e.tagName()=="TabList")
			{
				ParagraphStyle::TabRecord tb;
				node = node.firstChild();
				while(!node.isNull())
				{
					QDomElement tabElements = node.toElement();
					if(!tabElements.isNull())
					{
						if(tabElements.tagName()=="Position")
						tb.tabPosition = tabElements.text().toDouble();
						if(tabElements.tagName()=="AlignmentCharacter")
						{
							QString tbCh   = tabElements.text();
							if(tbCh.contains("$ID/",Qt::CaseSensitive))
								tbCh = tbCh.split("$ID/",QString::SkipEmptyParts,Qt::CaseSensitive).at(1);
							tb.tabFillChar = tbCh.isEmpty() ? QChar() : tbCh[0];
							tbs.append(tb);
							if (tbs.count() > 0)
								newStyle.setTabValues(tbs);
						}/*
						tb.tabType     = attrs2.valueAsInt("Type") */
					}
					node = node.nextSibling();
				}
			}
		}
		node = node.nextSibling();
	}
	fixLegacyParStyle(newStyle);
}

void ImportIdmlImpl::readCharacterStyle(ScribusDoc *doc, QDomNode node, CharStyle& newStyle)
{
	QDomElement elem = node.toElement();
	QStringList features;
	
	newStyle.erase();
	newStyle.setDefaultStyle(false);
	newStyle.setName(elem.attribute("Name",""));
	newStyle.setFontSize((qRound(elem.attribute("PointSize","").toDouble()) * 10));
	newStyle.setWordTracking(elem.attribute("DesiredWordSpacing","").toDouble());
	newStyle.setTracking(elem.attribute("Tracking","").toDouble());

	if(elem.attribute("Underline","")=="true")
	features.append(CharStyle::UNDERLINE);
	if(elem.attribute("StrikeThru","")=="true")
	features.append(CharStyle::STRIKETHROUGH);
	/*if(elem.attribute("Capitalization","")=="")
		features.append(CharStyle::ALLCAPS);
	if(elem.attribute("Capitalization","")=="")
		features.append(CharStyle::SMALLCAPS);*/
	newStyle.setFeatures(features);
	
	QString fontStyle = elem.attribute("FontStyle","");
	newStyle.setScaleH(qRound((elem.attribute("HorizontalScale","").toDouble()) * 10));
	newStyle.setScaleV(qRound((elem.attribute("VerticalScale","").toDouble()) * 10));
	newStyle.setBaselineOffset(qRound( (elem.attribute("BaselineShift","").toDouble()) * 10));
	newStyle.setStrokeColor(elem.attribute("StrokeColor",""));
	newStyle.setFillColor(elem.attribute("FillColor",""));
	newStyle.setFillShade((elem.attribute("FillTint","").toDouble()) * 10);
	newStyle.setStrokeShade((elem.attribute("StrokeTint","").toDouble()) * 10);
	newStyle.setUnderlineOffset(qRound((elem.attribute("UnderlineOffset","").toDouble()) * 10));
	newStyle.setUnderlineWidth(qRound((elem.attribute("UnderlineWidth","").toDouble()) * 10));
	newStyle.setStrikethruOffset(qRound((elem.attribute("StrikeThroughOffset","").toDouble())* 10));
	newStyle.setStrikethruWidth(qRound((elem.attribute("StrikeThroughWidth","").toDouble()) * 10));
	newStyle.setLanguage(elem.attribute("AppliedLanguage",""));
	
	node = node.firstChild();
	while(!node.isNull())
	{
		QDomElement e = node.toElement();
		if(!e.isNull())
		{
			if(e.tagName()=="BasedOn")
				newStyle.setParent(e.text());
			if(e.tagName()=="AppliedFont")
				newStyle.setFont(m_AvailableFonts->findFont(e.text()+fontStyle, doc));
		}
		node=node.nextSibling();
	}
	fixLegacyCStyle(newStyle);
}

/*!
This is a function taken from fileloader of Scribus 150 fileformat. Please check out the file for necessary details
*/

void ImportIdmlImpl::fixLegacyParStyle(ParagraphStyle& pstyle) 
{
	if (pstyle.lineSpacing() <= -16000)
		pstyle.resetLineSpacing();
	if (pstyle.leftMargin() <= -16000)
		pstyle.resetLeftMargin();
	if (pstyle.rightMargin() <= -16000)
		pstyle.resetRightMargin();
	if (pstyle.firstIndent() <= -16000)
		pstyle.resetFirstIndent();
	if (pstyle.alignment() < 0)
		pstyle.resetAlignment();
	if (pstyle.gapBefore() <= -16000)
		pstyle.resetGapBefore();
	if (pstyle.gapAfter() <= -16000)
		pstyle.resetGapAfter();
	if (pstyle.dropCapLines() < 0)
		pstyle.resetDropCapLines();
	if (pstyle.dropCapOffset() <= -16000)
		pstyle.resetDropCapOffset();
	fixLegacyCStyle(pstyle.charStyle());
}

/*!
This is a function taken from fileloader of Scribus 150 fileformat. Please check out the file for necessary details
*/

void fixLegacyCStyle(CharStyle& cstyle) 
{
	if (! cstyle.font().usable())
		cstyle.resetFont();
	if (cstyle.fontSize() <= -16000 / 10)
		cstyle.resetFontSize();
	//		if (cstyle.effects() == 65535)
	//			cstyle.resetEffects();
	if (cstyle.fillColor().isEmpty())
		cstyle.resetFillColor();
	if (cstyle.fillShade() <= -16000)
		cstyle.resetFillShade();
	if (cstyle.strokeColor().isEmpty())
		cstyle.resetStrokeColor();
	if (cstyle.strokeShade() <= -16000)
		cstyle.resetStrokeShade();
	if (cstyle.shadowXOffset() <= -16000 / 10)
		cstyle.resetShadowXOffset();
	if (cstyle.shadowYOffset() <= -16000 / 10)
		cstyle.resetShadowYOffset();
	if (cstyle.outlineWidth() <= -16000 / 10)
		cstyle.resetOutlineWidth();
	if (cstyle.underlineOffset() <= -16000 / 10)
		cstyle.resetUnderlineOffset();
	if (cstyle.underlineWidth() <= -16000 / 10)
		cstyle.resetUnderlineWidth();
	if (cstyle.strikethruOffset() <= -16000 / 10)
		cstyle.resetStrikethruOffset();
	if (cstyle.strikethruWidth() <= -16000 / 10)
		cstyle.resetStrikethruWidth();
	if (cstyle.scaleH() <= -16000 / 10)
		cstyle.resetScaleH();
	if (cstyle.scaleV() <= -16000 / 10)
		cstyle.resetScaleV();
	if (cstyle.baselineOffset() <= -16000 / 10)
		cstyle.resetBaselineOffset();
	if (cstyle.tracking() <= -16000 / 10)
		cstyle.resetTracking();
}

bool ImportIdmlImpl::processSpreads()
{
	int count = spreadPath.count();
	for( int i=0;i<count;i++ )
	{
		//loop to load files one by one and process it!
		QString path = spreadPath.at(i);
		QDomElement spreadRoot = loadXmlFile(path);
		if(spreadRoot.tagName()=="idPkg:Spread")
			continue;
		else
		{
			readSpread(spreadRoot.firstChild());
		}
	}
	return true;
}


bool ImportIdmlImpl::processStory()
{
	int count = storyList.count();
	for(int i=0;i<count;i++)
	{
		QString path = storyList.at(i);
		QDomElement storyRoot = loadXmlFile(path);
		if(storyRoot.tagName() != "idPkg:Story")
			continue;
		else
		{
			readStoryObjects(storyRoot.firstChild());
		}
	}
	return true;
}

bool ImportIdmlImpl::readStoryObjects(QDomNode node)
{
	while(!node.isNull())
	{
		QDomElement elem = node.toElement();
		if(!elem.isNull())
		{
			if(elem.tagName()=="Story")
			{
				Story story;
				QString Self = elem.attribute("Self","");
				readComTextProps(node,story.properties); //this will read common text properties represented as attributes/elements in the <story>
				QDomNode descend = node.firstChild();
				while(!descend.isNull())
				{
					if(descend.toElement().tagName()=="ParagraphStyleRange")
					{
						PStyleRange p;
						readComTextProps(descend,p.properties);
						if(descend.firstChild().toElement().tagName() == "CharacterStyleRange")
							p.cstyleRange.append(readCStyleRange(descend.firstChild()));
					}
					descend = descend.nextSibling();
				}
				storyMap.insert(Self,story);
			}
		}
		node=node.nextSibling();
	}
	return true;
}

void ImportIdmlImpl::readSpread(QDomNode node)
{
	/* Reading the attributes first */
	
	QTransform ItemTransform;
	QDomElement elem = node.toElement();
	bool showMasterItems = (elem.attribute("ShowMasterItems","")=="true")?true:false;
	int pageCount = elem.attribute("PageCount","").toInt();
	node=node.firstChild();
	while( !node.isNull() )
	{
		elem = node.toElement();
		if( !elem.isNull() )
		{
			if(elem.tagName()=="Page")
			{
				addPage(m_Doc,node);
			}
			if( elem.tagName()=="Rectangle")
			{
				addRectangle(m_Doc,node);
			}
			if( elem.tagName()=="Oval")
			{
				
			}
			if(elem.tagName()=="Polygon")
			{
				addPolygon(m_Doc,node);
			}
			if(elem.tagName()=="GraphicLine")
			{
				addGraphicLine(m_Doc,node);
			}
			if(elem.tagName()=="TextFrame")
			{
				addTextFrame(m_Doc,node);
			}
			if(elem.tagName()=="Group")
			{
			}
		}
		node = node.nextSibling();
	}
	
}

void ImportIdmlImpl::addTextFrame(ScribusDoc* doc, QDomNode node)
{
	QString Self;
	Story parentStory;
	bool Locked = true;
	QString FillColor, FillTint, StrokeColor, ItemLayer;
	QTransform itemTransform;
	QDomElement elem = node.toElement();
	if( !elem.isNull())
	{
		Self = elem.attribute("Self","");
		Locked = (elem.attribute("Locked","")=="true")?true:false;
		parentStory = storyMap.value(elem.attribute("ParentStory",""));
		FillColor = elem.attribute("FillColor","");
		if(FillColor.isEmpty())
			FillColor = CommonStrings::None;
		FillTint = elem.attribute("FillTint","");
		StrokeColor = elem.attribute("StrokeColor","");
		if(StrokeColor.isEmpty())
			StrokeColor = CommonStrings::None;
		ItemLayer = elem.attribute("ItemLayer","");
		itemTransform =  applyTransform(elem.attribute("ItemTranform",""));
		/* Try to add gradiant feature */
	}
	//Reading the properties
	QList<PathPoint> pathPoints;
	node = node.firstChild().firstChild();
	while(!node.isNull())
	{
		QDomElement e = node.toElement();
		if(!e.isNull())
		{
			if(e.tagName()=="PathGeometry")
			{
				if(node.firstChild().toElement().tagName()=="GeometryPathType")
				{
					QDomNode path = node.firstChild().firstChild();
					while(!node.isNull())
					{
						QDomElement p = node.toElement();
						if(!e.isNull())
						{
							pathPoints.append(readPathPoint(p));
						}
						node = node.nextSibling();
					}
				}
			}
		}
	}
	
	double x = pathPoints.at(0).Anchor.x();
	double y = pathPoints.at(0).Anchor.y();
	double w = findWidth(pathPoints.at(2).Anchor.x(),pathPoints.at(2).Anchor.y(),pathPoints.at(3).Anchor.x(),pathPoints.at(3).Anchor.y());
	double h = findHeight(pathPoints.at(0).Anchor.x(),pathPoints.at(0).Anchor.y(),pathPoints.at(1).Anchor.x(),pathPoints.at(1).Anchor.y());
	
	/** Now we have all the necessary things for defining a textitem in Scribus, here we go!
	*/
	
	int z = doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, x, y, w, h,0 , FillColor, StrokeColor, true);
	currItem = doc->Items->at(z);
	currItem->setLocked(Locked);
	currItem->setLayer(layerMap.value(ItemLayer));
	TextWriter *writer = new TextWriter(m_Doc);
	writer->open(currItem);
	/* Appending the text according to the paragraph style range elements */
	
	for(int i=0; i<parentStory.pstyleRange.count(); i++)
	{
		int count = parentStory.pstyleRange.at(i).cstyleRange.count();
		for(int j=0; j<count; j++)
		{
			if(!(parentStory.pstyleRange.at(i).cstyleRange.at(j).properties.AppliedCStyle.isEmpty()))
			writer->setCharStyle(parentStory.pstyleRange.at(i).cstyleRange.at(j).properties.AppliedCStyle);
			if(!(parentStory.pstyleRange.at(i).cstyleRange.at(j).properties.AppliedPStyle.isEmpty()))
			writer->setStyle(parentStory.pstyleRange.at(i).cstyleRange.at(j).properties.AppliedPStyle);
			QStringList list = parentStory.pstyleRange.at(i).cstyleRange.at(j).Content.split(QChar(13),QString::KeepEmptyParts);
			QStringList::Iterator it;
			for( it=list.begin();it!=list.end();++it)
			{
				writer->appendText(*it);
				writer->appendText(SpecialChars::PARSEP);
			}
		}
	}
}

PathPoint ImportIdmlImpl::readPathPoint(QDomElement e)
{
	PathPoint p;
	QStringList l = e.attribute("PathPointAnchor","").split(" ");
	p.Anchor.setX(l.at(0).toDouble());
	p.Anchor.setY(l.at(1).toDouble());
	l=e.attribute("LeftDirection","").split(" ");
	p.Left.setX(l.at(0).toDouble());
	p.Left.setY(l.at(1).toDouble());
	l=e.attribute("RightDirection","").split(" ");
	p.Right.setX(l.at(0).toDouble());
	p.Right.setY(l.at(1).toDouble());
	return p;
}
/**
\brief A Funtion to find width 
\return width as double if successful, else return -1
*/

double ImportIdmlImpl::findWidth(double x1, double y1, double x2, double y2)
{
	if(x1==x2)
		return (y1>y2)?(y1-y2):(y2-y1);
	return -1;
}

double ImportIdmlImpl::findHeight(double x1, double y1, double x2, double y2)
{
	if(y1==y2)
		return (x1>x2)?(x1-x2):(x2-x1);
	return -1;
}

QTransform itemTransform(QString transform)
{
	QStringList list = transform.split(" ");
	
	qreal a = list.at(0).toDouble();
	qreal b = list.at(1).toDouble();
	qreal c = list.at(3).toDouble();
	qreal d = list.at(4).toDouble();
	qreal e = list.at(5).toDouble();
	qreal f = list.at(6).toDouble();
	/* Adding the values directly */
	QTransform transformation(a,b,0,c,d,0,e,f,1);
	return transformation;
}

void ImportIdmlImpl::addPage(ScribusDoc *doc, QDomNode node)
{
	/** We've set an integer variable to hold the current page number status. Hence when we meet this function, we will increment this variable and pass it to the addPage function of m_Doc
	*/
	pageNumber = pageNumber+1;
	QDomElement elem = node.toElement();
	QString appliedMaster;
	Page* newPage;
	if(!elem.isNull())
	{
		appliedMaster = elem.attribute("AppliedMaster","");
	}
	if(!appliedMaster.isNull())
		newPage=doc->addPage(pageNumber,appliedMaster);
	else
		newPage=doc->addPage(pageNumber);
	newPage->setInitialHeight(docHeight);
	newPage->setInitialWidth(docWidth);
	if(docWidth > docHeight)
		newPage->setOrientation(1);
	else
		newPage->setOrientation(0);
	node=node.firstChild();
	while(!node.isNull())
	{
		QDomElement e = node.toElement();
		if(!e.isNull())
		{
			if(e.tagName()=="MarginPreference")
			{
				newPage->initialMargins.Left=e.attribute("Left","").toDouble();
				newPage->initialMargins.Right=e.attribute("Right","").toDouble();
				newPage->initialMargins.Top=e.attribute("Top","").toDouble();
				newPage->initialMargins.Bottom=e.attribute("Bottom","").toDouble();
				newPage->Margins.Top    = newPage->initialMargins.Top;
				newPage->Margins.Bottom = newPage->initialMargins.Bottom;
			}
		}
		node=node.nextSibling();
	}
}

CStyleRange ImportIdmlImpl::readCStyleRange(QDomNode node)
{
	CStyleRange c;
	readComTextProps(node,c.properties);
	while(!node.isNull())
	{
		QDomElement elem = node.toElement();
		if(!elem.isNull())
		{
			if(elem.tagName()=="Content")
			{
				c.Content.append(elem.text());
			}
			if(elem.tagName()=="Br")
				c.Content.append(QChar(13));
		}
		node=node.nextSibling();
	}
	return c;
}

void ImportIdmlImpl::readComTextProps(QDomNode node, CommonTextProps& property)
{
	/**
	First, read all the properties defined as attributes, then descend to those defined as elements
	*/
	QDomElement element = node.toElement();
	property.AppliedCStyle=element.attribute("AppliedCharacterStyle","");
	property.AppliedLanguage=element.attribute("AppliedLanguage","");
	property.AppliedPStyle=element.attribute("AppliedParagraphStyle","");
	property.BaselineShift=element.attribute("BaselineShift","").toDouble();
	//	property.Capital=element.attribute("Capitalization","");
	property.DesiredGlyphScale=element.attribute("DesiredGlyphScale","").toDouble();
	property.DesiredWordSpace=element.attribute("DesiredWordSpacing","").toDouble();
	property.DropCapChars=element.attribute("DropCapCharacters","").toDouble();
	property.DropCapLines=element.attribute("DropCapLines","").toDouble();
	property.FillColor=element.attribute("FillColor","");
	property.FillTint=element.attribute("FillTint","").toDouble();
	property.FirstLineIndent=element.attribute("FirstLineIndent","").toDouble();
	property.FontStyle=element.attribute("FontStyle","");
	property.GradFillAngle=element.attribute("GradiantFillAngle","").toDouble();
	property.GradFillLength=element.attribute("GradiantFillLength","").toDouble();
	property.GradStrokeAngle=element.attribute("GradiantStrokeAngle","").toDouble();
	property.GradStrokeLength=element.attribute("GradiantStrokeLength","").toDouble();
	property.HorizontalScale=element.attribute("HorizontalScale","").toDouble();
	//	property.justify=element.attribute("Justification","");
	property.StrikeThru=(element.attribute("StrikeThrough","")=="true")?true:false;
	property.StrikeThruOffset=element.attribute("StrikeThroughOffset","").toDouble();
	property.StrikeThruWeight=element.attribute("StrikeThroughWeight","").toDouble();
	property.StrokeColor=element.attribute("StrokeColor","");
	property.StrokeTint=element.attribute("StrokeTint","").toDouble();
	property.Tracking=element.attribute("Tracking","").toDouble();
	property.Underline=(element.attribute("Underline","")=="true")?true:false;
	property.UnderlineOffset=element.attribute("UnderlineOffset","").toDouble();
	property.UnderlineWeight =element.attribute("UnderlineWeight","").toDouble();
	
	/** Properties represented as elements 
	*/
	node=node.firstChild();
	while(!node.isNull())
	{
		QDomElement elem = node.toElement();
		if(!elem.isNull())
		{
			if(elem.tagName()=="AppliedFont")
			{
				property.AppliedFont=elem.text();
			}
		}
		node=node.nextSibling();
	}
}


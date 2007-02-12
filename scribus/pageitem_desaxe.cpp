


#include "pageitem.h"
#include "util.h"
#include "desaxe/simple_actions.h"
#include "desaxe/saxiohelper.h"
#include "pageitem_imageframe.h"
#include "pageitem_textframe.h"
#include "pageitem_pathtext.h"
#include "pageitem_polyline.h"
#include "pageitem_polygon.h"
#include "pageitem_line.h"
#include "scribusdoc.h"


using namespace desaxe;


Xml_string toXMLString(const QValueList<double>& doublelist)
{
	QString result = "";
	QValueList<double>::const_iterator dax;
	for (dax = doublelist.begin(); dax != doublelist.end(); ++dax)
		result += toXMLString(*dax) + " ";
	return result;
}


QValueList<double> parseDoubleList(Xml_string str)
{
	QStringList strlist = QStringList::split(' ', str);
	QValueList<double> result;
	for (QStringList::iterator it=strlist.begin(); it != strlist.end(); ++it)
		result.append(parseDouble(*it));
	return result;
}


Xml_string toXMLString(const QValueList<int>& intlist)
{
	QString result = "";
	QValueList<int>::const_iterator dax;
	for (dax = intlist.begin(); dax != intlist.end(); ++dax)
		result += toXMLString(*dax) + " ";
	return result;
}


QValueList<int> parseIntList(Xml_string str)
{
	QStringList strlist = QStringList::split(' ', str);
	QValueList<int> result;
	for (QStringList::iterator it=strlist.begin(); it != strlist.end(); ++it)
		result.append(parseInt(*it));
	return result;
}


QValueStack<int> parseIntStack(Xml_string str)
{
	QStringList strlist = QStringList::split(' ', str);
	QValueStack<int> result;
	for (QStringList::iterator it=strlist.begin(); it != strlist.end(); ++it)
		result.append(parseInt(*it));
	return result;
}


FPointArray parseSVG(Xml_string str)
{
	FPointArray result;
	result.parseSVG(str);
	return result;
}


static Xml_attr PageItemAttributes(const PageItem* item)
{
	Xml_attr result;
	
	result.insert("id", toXMLString(const_cast<PageItem*>(item)->getUId()));
	result.insert("name", item->itemName());
	const PageItem* nxt = item->nextInChain(); 
	if (nxt)
		result.insert("nextframe",  toXMLString(nxt->getUId())); 
	result.insert("layer", toXMLString(item->LayerNr));
	result.insert("level", toXMLString(item->ItemNr));
	result.insert("itemtype", toXMLString(item->itemType()));
	result.insert("ownpage", toXMLString(item->OwnPage));
	
	result.insert("is-bookmark", toXMLString(item->isPDFBookmark()));
	result.insert("printable", toXMLString(item->printEnabled()));
	result.insert("locked", toXMLString(item->locked()));
	result.insert("size-locked", toXMLString(item->sizeLocked()));
	
	result.insert("width", toXMLString(item->width()));
	result.insert("height", toXMLString(item->height()));
	result.insert("xorigin", toXMLString(item->xPos()));
	result.insert("yorigin", toXMLString(item->yPos()));
	//	result.insert("XPOS",item->xPos() - doc->currentPage()->xOffset());
	//	result.insert("YPOS",item->yPos() - doc->currentPage()->yOffset());
	result.insert("rotation", toXMLString(item->rotation()));
	result.insert("fill-color", item->fillColor());
	result.insert("fill-shade", toXMLString(item->fillShade()));
	result.insert("fill-transparency", toXMLString(item->fillTransparency()));
	result.insert("fill-blendmode", toXMLString(item->fillBlendmode()));
	result.insert("fill-gradient-type", toXMLString(item->gradientType()));
	result.insert("fill-evenodd", toXMLString(item->fillEvenOdd()));
	result.insert("do-overprint", toXMLString(item->overprint()));
	
	result.insert("line-color", item->lineColor());
	result.insert("line-shade", toXMLString(item->lineShade()));
	result.insert("line-transparency", toXMLString(item->lineTransparency()));
	result.insert("line-blendmode", toXMLString(item->lineBlendmode()));
	result.insert("line-width", toXMLString(item->lineWidth()));
	result.insert("line-style", toXMLString(item->PLineArt));
	result.insert("line-cap", toXMLString(item->PLineEnd));
	result.insert("line-join", toXMLString(item->PLineJoin));
	result.insert("line-customstyle", item->customLineStyle());
	result.insert("line-start-arrow", toXMLString(item->startArrowIndex()));
	result.insert("line-end-arrow", toXMLString(item->endArrowIndex()));

	result.insert("frame-type", toXMLString(item->frameType()));
	result.insert("frame-shape", toXMLString(item->shape()));
	result.insert("frame-has-default-shape", toXMLString(item->hasDefaultShape()));	
	result.insert("corner-radius", toXMLString(item->cornerRadius()));	
	result.insert("frame-contour", toXMLString(item->contour()));
	result.insert("text-flow-mode", toXMLString((int) item->textFlowMode() ));
	
	result.insert("left-distance", toXMLString(item->textToFrameDistLeft()));
	result.insert("top-distance", toXMLString(item->textToFrameDistTop()));
	result.insert("bottom-distance", toXMLString(item->textToFrameDistBottom()));
	result.insert("right-distance", toXMLString(item->textToFrameDistRight()));

	result.insert("text-autoframes", toXMLString(item->isAutoFrame()));
	result.insert("text-columns", toXMLString(item->columns()));
	result.insert("text-columngap", toXMLString(item->columnGap()));
	result.insert("text-reversed", toXMLString(item->reversed()));
	result.insert("pathtext-type", toXMLString(item->pathTextType()));
	result.insert("pathtext-flipped", toXMLString(item->flipPathText()));
	result.insert("pathtext-baseoffset", toXMLString(item->pathTextBaseOffset()));
	result.insert("pathtext-showpath", toXMLString(item->pathTextShowFrame()));

	result.insert("image-scale-framesize", toXMLString(item->fitImageToFrame()));
	result.insert("image-scale-proportional", toXMLString(item->keepAspectRatio()));
	result.insert("image-x-scale", toXMLString(item->imageXScale()));
	result.insert("image-y-scale", toXMLString(item->imageYScale()));
	result.insert("image-x-position", toXMLString(item->imageXOffset()));
	result.insert("image-y-position", toXMLString(item->imageYOffset()));
	//	result.insert("image-x-dpi", toXMLString(item->imageXDpi()));
	//	result.insert("image-y-dpi", toXMLString(item->imageYDpi()));
	result.insert("image-shown", toXMLString(item->imageShown()));
	result.insert("image-use-embedded", toXMLString(item->useEmbeddedImageProfile()));
	result.insert("image-embedded-profile", item->embeddedImageProfile());
	result.insert("image-flipped-hor", toXMLString(item->imageFlippedH()));
	result.insert("image-flipped-vert", toXMLString(item->imageFlippedV()));
	result.insert("cms-profile", item->cmsProfile());
	result.insert("cms-intent", toXMLString(item->cmsRenderingIntent()));
	
	result.insert("isGroupControl", toXMLString(item->controlsGroup()));
	if (item->isGroupControl)
	{
		if (item->groupsLastItem != 0)
			result.insert("groupsLastItem", toXMLString(item->groupsLastItem->getUId()));
	}
	QString idreflist = "";
	QValueStack<int>::const_iterator it;
	for (it = item->groups().begin(); it != item->groups().end(); ++it)
		idreflist += toXMLString( item->doc()->DocItems.at(*it)->getUId()) + " ";	
	result.insert("groups", toXMLString(idreflist));
	
	result.insert("isTableItem", toXMLString(item->isTableItem));
	result.insert("TopLine", toXMLString(item->TopLine));
	result.insert("LeftLine", toXMLString(item->LeftLine));
	result.insert("RightLine", toXMLString(item->RightLine));
	result.insert("BottomLine", toXMLString(item->BottomLine));
	if (item->isTableItem)
	{
		if (item->TopLink != 0)
			result.insert("TopLINK", toXMLString(item->TopLink->getUId()));
		if (item->LeftLink != 0)
			result.insert("LeftLINK", toXMLString(item->LeftLink->getUId()));
		if (item->RightLink != 0)
			result.insert("RightLINK", toXMLString(item->RightLink->getUId()));
		if (item->BottomLink != 0)
			result.insert("BottomLINK", toXMLString(item->BottomLink->getUId()));
	}
	
//	result.insert("ANNAME", !item->AutoName ? item->itemName() : QString(""));  // not used
	
	if ((item->itemType()==PageItem::ImageFrame || item->itemType()==PageItem::TextFrame) && (!item->externalFile().isEmpty()))
		result.insert("image-file", Path2Relative(item->externalFile()));
	if (!item->fileIconPressed().isEmpty())
		result.insert("icon-pressed-file", Path2Relative(item->fileIconPressed()));
	if (!item->fileIconRollover().isEmpty())
		result.insert("icon-rollover-file", Path2Relative(item->fileIconRollover()));	
	
	if (item->dashes().count() > 0)
		result.insert("line-dashes", toXMLString(item->dashes()));
	if (item->dashOffset() != 0.0)
		result.insert("line-dash-offset", toXMLString(item->dashOffset()));
	return result;
}	

void PageItem::saxx(SaxHandler& handler) const
{
	Xml_attr att(PageItemAttributes(this));
	handler.begin("item", att);

	if (effectsInUse.count() != 0)
	{
		for (uint a = 0; a < effectsInUse.count(); ++a)
		{
			Xml_attr imeff;
			imeff.insert("Code", toXMLString((*effectsInUse.at(a)).effectCode));
			imeff.insert("Param", (*effectsInUse.at(a)).effectParameters);
			handler.begin("ImageEffect", imeff);
			handler.end("ImageEffect");
		}
	}
	
	if ((itemType()==ImageFrame || itemType()==TextFrame) && !Pfile.isEmpty() 
		&& pixm.imgInfo.layerInfo.count() != 0 && pixm.imgInfo.isRequest)
	{
		QMap<int, ImageLoadRequest>::const_iterator it2;
		for (it2 = pixm.imgInfo.RequestProps.begin(); it2 != pixm.imgInfo.RequestProps.end(); ++it2)
		{
			Xml_attr psd;
			psd.insert("Layer", toXMLString(it2.key()));
			psd.insert("Visible", toXMLString(it2.data().visible));
			psd.insert("useMask", toXMLString(it2.data().useMask));
			psd.insert("Opacity", toXMLString(it2.data().opacity));
			psd.insert("Blend", it2.data().blend);
			handler.begin("PSDLayer", psd);
			handler.end("PSDLayer");
		}
	}
	if (gradientType() != 0)
	{
		if (gradientType() == 8)
		{
			Xml_attr patt;
			patt.insert("pattern", pattern());
			double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation;
			const_cast<PageItem*>(this)->
				patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
			patt.insert("pScaleX", toXMLString(patternScaleX));
			patt.insert("pScaleY", toXMLString(patternScaleY));
			patt.insert("pOffsetX", toXMLString(patternOffsetX));
			patt.insert("pOffsetY", toXMLString(patternOffsetY));
			patt.insert("pRotation", toXMLString(patternRotation));
			handler.begin("Pattern", patt);
			handler.end("Pattern");
		}
		else
		{
			Xml_attr gradient;
			gradient.insert("GRSTARTX", toXMLString(GrStartX));
			gradient.insert("GRSTARTY", toXMLString(GrStartY));
			gradient.insert("GRENDX", toXMLString(GrEndX));
			gradient.insert("GRENDY", toXMLString(GrEndY));
			handler.begin("Gradient", gradient);
			QPtrVector<VColorStop> cstops = fill_gradient.colorStops();
			for (uint cst = 0; cst < const_cast<VGradient&>(fill_gradient).Stops(); ++cst) //FIXME make const
			{
				Xml_attr itcl;
				itcl.insert("RAMP", toXMLString(cstops.at(cst)->rampPoint));
				itcl.insert("NAME", cstops.at(cst)->name);
				itcl.insert("SHADE", toXMLString(cstops.at(cst)->shade));
				itcl.insert("TRANS", toXMLString(cstops.at(cst)->opacity));
				handler.begin("CStop",itcl);
				handler.end("CStop");
			}
			handler.end("Gradient");
		}
	}
	
	// TODO: PI attributes...
	
	if (isAnnotation())
	{
		annotation().saxx(handler);
	}
	if(itemText.length() > 0)
	{
		itemText.saxx(handler);
	}
	handler.end("item");
}


class CreatePageItem_body : public Generator_body<PageItem>
{
	void begin (const Xml_string /*tagname*/, Xml_attr attr)
	{
		PageItem* result;
		ScribusDoc* doc = this->dig->top<ScribusDoc>();
		
		int type = parseInt(attr["itemtype"]);
		double xpos = parseDouble(attr["xorigin"]);
		double ypos = parseDouble(attr["yorigin"]);
		double width = parseDouble(attr["width"]);
		double height = parseDouble(attr["height"]);
		double linewidth = parseDouble(attr["line-width"]);
		QString fillC = attr["fill-color"];
		QString lineC = attr["line-color"];
		switch (type)
		{
			case PageItem::ImageFrame:
				result = new PageItem_ImageFrame(doc, xpos, ypos, width, height, linewidth, fillC, lineC);
				break;
			case PageItem::TextFrame:
				result = new PageItem_TextFrame(doc, xpos, ypos, width, height, linewidth, fillC, lineC);
				break;
			case PageItem::PathText:
				result = new PageItem_PathText(doc, xpos, ypos, width, height, linewidth, fillC, lineC);
				break;
			case PageItem::PolyLine:
				result = new PageItem_PolyLine(doc, xpos, ypos, width, height, linewidth, fillC, lineC);
				break;
			case PageItem::Polygon:
				result = new PageItem_Polygon(doc, xpos, ypos, width, height, linewidth, fillC, lineC);
				break;
			case PageItem::Line:
				result = new PageItem_Line(doc, xpos, ypos, width, height, linewidth, fillC, lineC);
				break;
			default:
				result = NULL;
				break;
		}
		this->dig->push(result);
	}
};

class CreatePageItem : public MakeGenerator<CreatePageItem_body, PageItem>
{};


void PageItem::desaxeRules(Xml_string prefixPattern, Digester& ruleset)
{
	Xml_string itemPrefix(prefixPattern + "/item");
	
	// the generator CreatePageItem currently *requires* the Scribusdoc at the top of the stack
	ruleset.addRule(itemPrefix, CreatePageItem() );

	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setLayer, "layer", &parseInt )); // set to activelayer in constructor
//	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>("id", toXMLString(const_cast<PageItem*>(item)->getUId())); // set automatically
	static const QString dummy(""); // otherwise the next line gives a warning: default-initialization of 'const QString& desaxe::SetAttributeWithConversion_body<PageItem, const QString&>::default_', which has reference type ...
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,const QString&>( & PageItem::setItemName, "name", dummy ));    // ... which could be ignored, but without is nicer
//	ruleset.addRule(itemPrefix, ?(?"nextframe",  toXMLString(item->nextInChain()->getUId())); 
//	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & ?PageItem::setLevel, "level", &parseInt ));  // set automatically
//	ruleset.addRule(itemPrefix, SetAttribute("itemtype", toXMLString(item->itemType()));  // in createPageItem()
//	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>(?, "ownpage", &parseInt);  // set to current page in constructor
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setIsPDFBookmark,  "is-bookmark", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setPrintEnabled, "printable", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setLocked, "locked", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setSizeLocked, "size-locked", &parseBool ));
	
//	ruleset.addRule("width", toXMLString(item->width()));  // in createPageItem()
//	ruleset.addRule("height", toXMLString(item->height()));  // in createPageItem()
//	ruleset.addRule("xorigin", toXMLString(item->xPos()));  // in createPageItem()
//	ruleset.addRule("yorigin", toXMLString(item->yPos()));  // in createPageItem()
	//	ruleset.addRule("XPOS",item->xPos() - doc->currentPage()->xOffset());
	//	ruleset.addRule("YPOS",item->yPos() - doc->currentPage()->yOffset());
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setRotation, "rotation", &parseDouble ));
//	ruleset.addRule(itemPrefix, SetAttribute<PageItem,const QString&>("fill-color", item->fillColor());  // in createPageItem()
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setFillShade, "fill-shade", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setFillTransparency, "fill-transparency", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setFillBlendmode, "fill-blendmode", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setGradientType, "gradient-type", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,PageItem::TextFlowMode>( & PageItem::setTextFlowMode, "text-flow-mode", &parseEnum<PageItem::TextFlowMode> ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setFillEvenOdd, "frame-fill-evenodd", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setOverprint, "do-overprint", &parseBool ));
	
//	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>("line-color", item->lineColor());  // in createPageItem()
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setLineShade, "line-shade", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setLineTransparency, "line-transparency", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setLineBlendmode, "line-blendmode", &parseInt ));
//	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setLineWidth, "line-width", &parseInt ));  // in createPageItem()
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,PenStyle>( & PageItem::setLineStyle, "line-style", &parseEnum<PenStyle> ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,PenCapStyle>( & PageItem::setLineEnd, "line-cap", &parseEnum<PenCapStyle> ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,PenJoinStyle>( & PageItem::setLineJoin, "line-join", &parseEnum<PenJoinStyle> ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,const QString&>( & PageItem::setCustomLineStyle, "line-customstyle", dummy )); // see comment above for setItemName
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setStartArrowIndex, "line-start-arrow", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setEndArrowIndex, "line-end-arrow", &parseInt ));
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setFrameType, "frame-type", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,FPointArray>( & PageItem::setShape, "frame-shape", &parseSVG ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setHasDefaultShape, "frame-has-default-shape", &parseBool ));	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setCornerRadius, "corner-radius", &parseDouble ));	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,FPointArray>( & PageItem::setContour, "frame-contour", &parseSVG ));
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setTextToFrameDistLeft, "left-distance", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setTextToFrameDistTop, "top-distance", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setTextToFrameDistBottom, "bottom-distance", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setTextToFrameDistRight, "right-distance", &parseDouble ));
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setIsAutoFrame, "text-autoframe", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setColumns, "text-columns", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setColumnGap, "text-columngap", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setReversed, "text-reversed", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setPathTextType, "pathtext-type", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setFlipPathText, "pathtext-flipped", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setPathTextBaseOffset, "pathtext-baseoffset", &parseDouble));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setPathTextShowFrame, "pathtext-showpath", &parseBool ));
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setFitImageToFrame, "image-scale-framesize", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setKeepAspectRatio, "image-scale-proportional", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setImageXScale, "image-x-scale", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setImageYScale, "image-y-scale", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setImageXOffset, "image-x-position", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setImageYOffset, "image-y-position", &parseDouble ));
	//	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>("image-x-dpi", &parseDouble ));  //NYI
	//	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>("image-y-dpi", &parseDouble ));  //NYI
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setImageShown, "image-shown", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setUseEmbeddedImageProfile, "image-use-embedded", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setEmbeddedImageProfile, "image-embedded-profile" ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setImageFlippedH, "image-flipped-hor", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setImageFlippedV, "image-flipped-vert", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setCmsProfile, "cms-profile" ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setCmsRenderingIntent, "cms-intent", &parseInt ));

	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setControlsGroup, "isGroupControl", &parseBool ));
//			ruleset.addRule("groupsLastItem", toXMLString(item->groupsLastItem->ItemNr - item->ItemNr));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,QValueStack<int> >( & PageItem::setGroups, "groups", &parseIntStack ));

/*	
	ruleset.addRule("isTableItem", toXMLString(item->isTableItem));
	ruleset.addRule("TopLine", toXMLString(item->TopLine));
	ruleset.addRule("LeftLine", toXMLString(item->LeftLine));
	ruleset.addRule("RightLine", toXMLString(item->RightLine));
	ruleset.addRule("BottomLine", toXMLString(item->BottomLine));
	if (item->isTableItem)
	{
		if (item->TopLink != 0)
			ruleset.addRule("TopLINK", toXMLString(item->TopLink->ItemNr));
		else
			ruleset.addRule("TopLINK", "-1");
		if (item->LeftLink != 0)
			ruleset.addRule("LeftLINK", toXMLString(item->LeftLink->ItemNr));
		else
			ruleset.addRule("LeftLINK", "-1");
		if (item->RightLink != 0)
			ruleset.addRule("RightLINK", toXMLString(item->RightLink->ItemNr));
		else
			ruleset.addRule("RightLINK", "-1");
		if (item->BottomLink != 0)
			ruleset.addRule("BottomLINK", toXMLString(item->BottomLink->ItemNr));
		else
			ruleset.addRule("BottomLINK", "-1");
		ruleset.addRule("OwnLINK", toXMLString(item->ItemNr));
	}
	
					*/
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setExternalFile, "image-file" ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setFileIconPressed, "icon-pressed-file" ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setFileIconRollover, "icon-rollover-file" ));
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,QValueList<double> >( & PageItem::setDashes, "line-dashes", &parseDoubleList ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setDashOffset, "line-dash-offset", &parseDouble ));
}
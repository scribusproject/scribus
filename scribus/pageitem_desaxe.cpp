/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "desaxe/saxiohelper.h"
#include "desaxe/simple_actions.h"
#include "pageitem.h"
#include "pageitem_imageframe.h"
#include "pageitem_line.h"
#include "pageitem_pathtext.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pageitem_textframe.h"
#include "pageitem_latexframe.h"
#include "scribusdoc.h"
#include "util_color.h"
#include "util_math.h"
#include "util.h"

#include <QDebug>

using namespace desaxe;



FPointArray parseSVG(const Xml_string& str)
{
	FPointArray result;
	result.parseSVG(str);
	return result;
}


static Xml_attr PageItemXMLAttributes(const PageItem* item)
{
	Xml_attr result;
	
	result.insert("id", "obj" + toXMLString(const_cast<PageItem*>(item)->getUId()));
	if (!item->AutoName)
		result.insert("name", item->itemName());
	const PageItem* nxt = item->nextInChain(); 
	if (nxt)
		result.insert("nextframe", "obj" + toXMLString(nxt->getUId())); 
	result.insert("layer", toXMLString(item->LayerNr));
	result.insert("level", toXMLString(item->ItemNr));
	result.insert("itemtype", toXMLString(item->realItemType()));
	
	result.insert("is-annotation", toXMLString(item->isAnnotation()));
	result.insert("is-bookmark", toXMLString(item->isPDFBookmark()));
	result.insert("printable", toXMLString(item->printEnabled()));
	result.insert("locked", toXMLString(item->locked()));
	result.insert("size-locked", toXMLString(item->sizeLocked()));
	
	result.insert("width", toXMLString(item->width()));
	result.insert("height", toXMLString(item->height()));
	result.insert("ownpage", toXMLString(item->OwnPage));
	Page* page = (item->OwnPage >= 0) ? item->doc()->Pages->at(item->OwnPage) : NULL;
	if (page)
	{
		result.insert("xorigin", toXMLString(item->xPos() - page->xOffset()));
		result.insert("yorigin", toXMLString(item->yPos() - page->yOffset()));
	}
	else
	{
		result.insert("xorigin", toXMLString(item->xPos()));
		result.insert("yorigin", toXMLString(item->yPos()));
	}
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
	
	if (item->groups().count() > 0)
	{
		QString grouplist = "";
		QStack<int>::const_iterator it;
		for (it = item->groups().begin(); it != item->groups().end(); ++it)
			grouplist += toXMLString( *it ) + " ";	
		result.insert("groups", toXMLString(grouplist));
		result.insert("isGroupControl", toXMLString(item->controlsGroup()));
		if (item->isGroupControl)
		{
			if (item->groupsLastItem != 0)
				result.insert("groupsLastItem", "obj" + toXMLString(item->groupsLastItem->getUId()));
		}
	}
	
	result.insert("isTableItem", toXMLString(item->isTableItem));
	if (item->isTableItem)
	{
		result.insert("TopLine", toXMLString(item->TopLine));
		result.insert("LeftLine", toXMLString(item->LeftLine));
		result.insert("RightLine", toXMLString(item->RightLine));
		result.insert("BottomLine", toXMLString(item->BottomLine));
		if (item->TopLink != 0)
			result.insert("TopLINK", "obj" + toXMLString(item->TopLink->getUId()));
		if (item->LeftLink != 0)
			result.insert("LeftLINK", "obj" + toXMLString(item->LeftLink->getUId()));
		if (item->RightLink != 0)
			result.insert("RightLINK", "obj" + toXMLString(item->RightLink->getUId()));
		if (item->BottomLink != 0)
			result.insert("BottomLINK", "obj" + toXMLString(item->BottomLink->getUId()));
	}
	
//	result.insert("ANNAME", !item->AutoName ? item->itemName() : QString(""));  // not used
	const PageItem_LatexFrame *latexframe = NULL;
	if (item->realItemType() == PageItem::LatexFrame)
		latexframe = dynamic_cast<const PageItem_LatexFrame*>(item);
	
	if ((item->itemType()==PageItem::ImageFrame || item->itemType()==PageItem::TextFrame) && (!item->externalFile().isEmpty()) && !latexframe)
		result.insert("image-file", Path2Relative(item->externalFile()));
	if (!item->fileIconPressed().isEmpty())
		result.insert("icon-pressed-file", Path2Relative(item->fileIconPressed()));
	if (!item->fileIconRollover().isEmpty())
		result.insert("icon-rollover-file", Path2Relative(item->fileIconRollover()));	
	if (latexframe) {
		result.insert("latex-dpi", toXMLString(latexframe->dpi()));
		result.insert("latex-configfile", latexframe->configFile());
		result.insert("latex-use-preamble", toXMLString(latexframe->usePreamble()));
	}
	
	if (item->dashes().count() > 0)
		result.insert("line-dashes", toXMLString(item->dashes()));
	if (item->dashOffset() != 0.0)
		result.insert("line-dash-offset", toXMLString(item->dashOffset()));
	return result;
}	


void PageItem::saxx(SaxHandler& handler, const Xml_string& elemtag) const
{
	Xml_attr att(PageItemXMLAttributes(this));
	Xml_attr empty;
//	qDebug(QString("PageItem::saxx %1 %2").arg((ulong) &handler));
	handler.begin(elemtag, att);

	if (effectsInUse.count() != 0)
	{
		for (int a = 0; a < effectsInUse.count(); ++a)
		{
			Xml_attr imeff;
			imeff.insert("Code", toXMLString(effectsInUse.at(a).effectCode));
			imeff.insert("Param", effectsInUse.at(a).effectParameters);
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
			psd.insert("Visible", toXMLString(it2.value().visible));
			psd.insert("useMask", toXMLString(it2.value().useMask));
			psd.insert("Opacity", toXMLString(it2.value().opacity));
			psd.insert("Blend", it2.value().blend);
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
			gradient.insert("GRTYPE", toXMLString(GrType));
			gradient.insert("GRSTARTX", toXMLString(GrStartX));
			gradient.insert("GRSTARTY", toXMLString(GrStartY));
			gradient.insert("GRENDX", toXMLString(GrEndX));
			gradient.insert("GRENDY", toXMLString(GrEndY));
			handler.begin("Gradient", gradient);
			QList<VColorStop*> cstops = fill_gradient.colorStops();
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
	if (prevInChain() == NULL && itemText.length() > 0)
	{
		itemText.saxx(handler, "text-content");
	}

	const PageItem_LatexFrame *latexframe = NULL;
	if (this->realItemType() == PageItem::LatexFrame)
		latexframe = dynamic_cast<const PageItem_LatexFrame*>(this);
	if (latexframe) {
		handler.begin("latex-source", empty);
		QMapIterator<QString, QString> i(latexframe->editorProperties);
		while (i.hasNext()) {
			Xml_attr property;
			i.next();
			property.insert("name", i.key());
			property.insert("value", i.value());
			handler.begin("property", property);
			handler.end("property");
		}
		handler.chars(latexframe->formula());
		handler.end("latex-source");
	}
	handler.end(elemtag);
}



class CreatePageItem_body : public Generator_body<PageItem>
{
	void begin (const Xml_string& /*tagname*/, Xml_attr attr)
	{
		PageItem* result;
		ScribusDoc* doc = this->dig->lookup<ScribusDoc>("<scribusdoc>");
		
		PageItem::ItemType type = parseEnum<PageItem::ItemType>(attr["itemtype"]);
		PageItem::ItemFrameType frametype = PageItem::Unspecified; // parseEnum<PageItem::ItemFrameType>(attr["frame-type"]);
		// int ownpage = parseInt(attr["ownpage"]);
		// doc->setCurrentPage(doc->Pages->at(ownpage));
		double xpos = parseDouble(attr["xorigin"]) + doc->currentPage()->xOffset();
		double ypos = parseDouble(attr["yorigin"]) + doc->currentPage()->yOffset();
		double width = parseDouble(attr["width"]);
		double height = parseDouble(attr["height"]);
		double linewidth = parseDouble(attr["line-width"]);
		QString fillC = attr["fill-color"];
		QString lineC = attr["line-color"];
		int nr = doc->itemAdd(type, frametype, xpos, ypos, width, height, linewidth, fillC, lineC, true);
		result = doc->Items->at(nr);
		this->dig->push(result);
//		qDebug(QString("pushed item: %1,%2 params %3,%4").arg(result->xPos()).arg(result->yPos()).arg(xpos).arg(ypos));
	}
};

class CreatePageItem : public MakeGenerator<CreatePageItem_body, PageItem>
{};



///   PageItem StoryText -> PageItem StoryText
class SetItemText_body : public Action_body
{
	void end (const Xml_string& /*tagname*/)
	{
		PageItem* item = this->dig->top<PageItem>(1);
		StoryText* story = this->dig->top<StoryText>(0);
		item->itemText.clear();
		item->itemText.setDefaultStyle(story->defaultStyle());
		item->itemText.append(*story);  // story has no document attached, so we dont want to assign here
	}
};

class SetItemText : public MakeAction<SetItemText_body>
{};



class Gradient_body : public Action_body
{
	void begin (const Xml_string& tagName, Xml_attr attr)
	{
//		qDebug(QString("pageitem_desaxe: gradient %1").arg(tagName));
		if (tagName=="CStop")
		{
//			ScribusDoc* doc = this->dig->lookup<ScribusDoc>("<scribusdoc>");
			PageItem* item = this->dig->top<PageItem>();
			QString name = attr["NAME"];
			double ramp = parseDouble(attr["RAMP"]);
			int shade = parseInt(attr["SHADE"]);
			double opa = parseDouble(attr["TRANS"]);
			// Hack : at this stage, colors may still not exists in document and SetColor would create it, 
			// so use the dummy brown and update manually gradient colors in Serializer
			// item->fill_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
			item->fill_gradient.addStop( QColor(150, 100, 50) , ramp, 0.5, opa, name, shade);
		}
		if (tagName=="Gradient")
		{
			PageItem* item = this->dig->top<PageItem>();
			item->GrType = parseInt(attr["GRTYPE"]);
			item->GrStartX = parseDouble(attr["GRSTARTX"]);
			item->GrStartY = parseDouble(attr["GRSTARTY"]);
			item->GrEndX = parseDouble(attr["GRENDX"]);
			item->GrEndY = parseDouble(attr["GRENDY"]);
			item->fill_gradient.clearStops();
		}
	}
};

class Gradient : public MakeAction<Gradient_body>
{};



class Pattern_body : public Action_body
{
	void begin (const Xml_string& tagName, Xml_attr attr)
	{
		PageItem* item = this->dig->top<PageItem>();
		
		double patternScaleX = parseDouble(attr["pScaleX"]);
		double patternScaleY = parseDouble(attr["pScaleY"]);
		double patternOffsetX = parseDouble(attr["pOffsetX"]);
		double patternOffsetY = parseDouble(attr["pOffsetY"]);
		double patternRotation = parseDouble(attr["pRotation"]);
//		qDebug(QString("pageitem_desaxe: pattern %6: *(%1,%2) +(%3,%4) °%5").arg(patternScaleX).arg(patternScaleY).arg(patternOffsetX).arg(patternOffsetY).arg(patternRotation).arg(attr["pattern"]));

		item->setPattern(attr["pattern"]);
		item->setPatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
	}
};

class Pattern : public MakeAction<Pattern_body>
{};



class ImageEffectsAndLayers_body : public Action_body
{
	void begin (const Xml_string& tagName, Xml_attr attr)
	{
//		qDebug(QString("pageitem_desaxe: effects/layers %1").arg(tagName));
		if (tagName=="ImageEffect")
		{
			PageItem* obj = this->dig->top<PageItem>();
			struct ImageEffect ef;
			ef.effectParameters = attr["Param"];
			ef.effectCode = parseInt(attr["Code"]);
			obj->effectsInUse.append(ef);
		}
		if (tagName == "PSDLayer")
		{
			PageItem* obj = this->dig->top<PageItem>();
			struct ImageLoadRequest loadingInfo;
			loadingInfo.blend = attr["Blend"];
			loadingInfo.opacity = parseInt(attr["Opacity"]);
			loadingInfo.visible = parseBool(attr["Visible"]);
			loadingInfo.useMask = parseBool(attr["useMask"]);
			obj->pixm.imgInfo.RequestProps.insert(parseInt(attr["Layer"]), loadingInfo);
			obj->pixm.imgInfo.isRequest = true;
		}
	}
};

class ImageEffectsAndLayers : public MakeAction<ImageEffectsAndLayers_body>
{};



class LoadPicture_body : public Action_body
{
	void end (const Xml_string& /*tagname*/)
	{
		PageItem* item = this->dig->top<PageItem>();
		if (item->itemType() == PageItem::ImageFrame)
			item->loadImage(item->externalFile(), true);
	}
};

class LoadPicture : public MakeAction<LoadPicture_body>
{};



class AdjustGroupIds_body : public Action_body
{
	void begin (const Xml_string& tagname, Xml_attr attr)
	{
		if (tagname != PageItem::saxxDefaultElem)
		{
			ScribusDoc* doc = this->dig->lookup<ScribusDoc>("<scribusdoc>");
			minGroup = doc->GroupCounter;
			maxGroup = minGroup - 1;
		}
	}

	void end (const Xml_string& tagname)
	{
		if (tagname != PageItem::saxxDefaultElem)
		{
			ScribusDoc* doc = this->dig->lookup<ScribusDoc>("<scribusdoc>");
			doc->GroupCounter = maxGroup + 1;
		}
		else {
			PageItem* item = this->dig->top<PageItem>();
			QStack<int> groups;
			for (int i=0; i < item->groups().count(); ++i)
			{
				int newGroup = minGroup + item->groups()[i];
				if (newGroup > maxGroup)
					maxGroup = newGroup;
//				qDebug(QString("group id %1 -> %2").arg(item->groups()[i]).arg(newGroup));
				groups.append(newGroup);
			}
			item->setGroups(groups);
		}
	}
	
private:
	int minGroup;
	int maxGroup;
};

class AdjustGroupIds : public MakeAction<AdjustGroupIds_body>
{};


class LatexSource_body : public Action_body
{
	public:	
		void chars(const Xml_string& txt)
		{
			if (this->dig->top<PageItem>()->realItemType() == PageItem::LatexFrame)
			{
				PageItem_LatexFrame* obj = dynamic_cast<PageItem_LatexFrame *> (this->dig->top<PageItem>());
				obj->setFormula(txt);
			}
		}
};

struct  LatexSource : public MakeAction<LatexSource_body> 
{};


class LatexParams_body : public Action_body
{
	public:	
		void begin(const Xml_string& tag, Xml_attr attr) 
		{
			if (this->dig->top<PageItem>()->realItemType() == PageItem::LatexFrame)
			{
				PageItem_LatexFrame* obj = dynamic_cast<PageItem_LatexFrame *> (this->dig->top<PageItem>());
				obj->setConfigFile(attr["latex-configfile"]);
				obj->setDpi(parseInt(attr["latex-dpi"]));
				obj->setUsePreamble(parseBool(attr["latex-use-preamble"]));
			}
		}
};

struct  LatexParams : public MakeAction<LatexParams_body> 
{};



const Xml_string PageItem::saxxDefaultElem("item");



void PageItem::desaxeRules(const Xml_string& prefixPattern, Digester& ruleset, Xml_string elemtag)
{
	Xml_string itemPrefix(Digester::concat(prefixPattern, elemtag));
	
	// the generator CreatePageItem *requires* the Scribusdoc stored as "<scribusdoc>"
	ruleset.addRule(itemPrefix, CreatePageItem() );

	ruleset.addRule(itemPrefix, IdRef<PageItem>() );
//	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>("id", toXMLString(const_cast<PageItem*>(item)->getUId())); // set automatically
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setLayer, "layer", &parseInt )); // set to activelayer in constructor
	static const QString dummy(""); // otherwise the next line gives a warning: default-initialization of 'const QString& desaxe::SetAttributeWithConversion_body<PageItem, const QString&>::default_', which has reference type ...
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,const QString&>( & PageItem::setItemName, "name", dummy ));    // ... which could be ignored, but without is nicer
	ruleset.addRule(itemPrefix, PatchIdRefAttribute<PageItem, PageItem>( & PageItem::link, "nextframe" ));
//	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & ?PageItem::setLevel, "level", &parseInt ));  // set automatically
//	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::??, "itemtype", &parseInt ));  // automatically in createPageItem()
//	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>(?, "ownpage", &parseInt);  // set to current page in constructor
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setIsAnnotation,  "is-annotation", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setIsPDFBookmark,  "is-bookmark", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setPrintEnabled, "printable", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setLocked, "locked", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setSizeLocked, "size-locked", &parseBool ));
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setWidth, "width", &parseDouble ));  // also in createPageItem()
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setHeight, "height", &parseDouble ));  // also in createPageItem()
//	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setXPos, "xorigin", &parseDouble ));  // also in createPageItem()
//	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setYPos, "yorigin", &parseDouble ));  // also in createPageItem()
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setRotation, "rotation", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,const QString&>( & PageItem::setFillColor, "fill-color", dummy ));  // also in createPageItem()
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setFillShade, "fill-shade", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setFillTransparency, "fill-transparency", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setFillBlendmode, "fill-blendmode", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setGradientType, "fill-gradient-type", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,PageItem::TextFlowMode>( & PageItem::setTextFlowMode, "text-flow-mode", &parseEnum<PageItem::TextFlowMode> ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setFillEvenOdd, "frame-fill-evenodd", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setOverprint, "do-overprint", &parseBool ));
	
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,const QString&>( & PageItem::setLineColor, "line-color", dummy ));  // also in createPageItem()
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setLineShade, "line-shade", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setLineTransparency, "line-transparency", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setLineBlendmode, "line-blendmode", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setLineWidth, "line-width", &parseDouble ));  // also in createPageItem()
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,Qt::PenStyle>( & PageItem::setLineStyle, "line-style", &parseEnum<Qt::PenStyle> ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,Qt::PenCapStyle>( & PageItem::setLineEnd, "line-cap", &parseEnum<Qt::PenCapStyle> ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,Qt::PenJoinStyle>( & PageItem::setLineJoin, "line-join", &parseEnum<Qt::PenJoinStyle> ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,const QString&>( & PageItem::setCustomLineStyle, "line-customstyle", dummy )); // see comment above for setItemName
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setStartArrowIndex, "line-start-arrow", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setEndArrowIndex, "line-end-arrow", &parseInt ));
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setFrameType, "frame-type", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem, FPointArray>( & PageItem::setShape, "frame-shape", &parseSVG ));
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
	ruleset.addRule(itemPrefix, PatchIdRefAttribute<PageItem, PageItem>( & PageItem::setGroupsLastItem, "groupsLastItem" ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,QStack<int> >( & PageItem::setGroups, "groups", &parseIntStack ));

	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setIsTableItem, "isTableItem", &parseBool ));
	ruleset.addRule(itemPrefix, PatchIdRefAttribute<PageItem, PageItem>( & PageItem::setTopLink, "TopLink" ));
	ruleset.addRule(itemPrefix, PatchIdRefAttribute<PageItem, PageItem>( & PageItem::setLeftLink, "LeftLink" ));
	ruleset.addRule(itemPrefix, PatchIdRefAttribute<PageItem, PageItem>( & PageItem::setRightLink, "RightLink" ));
	ruleset.addRule(itemPrefix, PatchIdRefAttribute<PageItem, PageItem>( & PageItem::setBottomLink, "BottomLink" ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setHasTopLine, "TopLine", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setHasLeftLine, "LeftLine", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setHasRightLine, "RightLine", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setHasBottomLine, "BottomLine", &parseBool ));
	
	ruleset.addRule(Digester::concat(itemPrefix, "Pattern"), Pattern()); 
	
	Gradient gradient;
	Xml_string gradientPrefix(Digester::concat(itemPrefix, "Gradient"));
	ruleset.addRule(gradientPrefix, gradient);
	ruleset.addRule(Digester::concat(gradientPrefix, "CStop"), gradient);
		
	ImageEffectsAndLayers effectsAndLayers;
	ruleset.addRule(Digester::concat(itemPrefix, "ImageEffect"), effectsAndLayers);
	ruleset.addRule(Digester::concat(itemPrefix, "PSDLayer"), effectsAndLayers);
	
	Annotation::desaxeRules(itemPrefix, ruleset);
	ruleset.addRule(Digester::concat(itemPrefix, Annotation::saxxDefaultElem), 
					SetterWithConversion<PageItem,const Annotation&,Annotation>( & PageItem::setAnnotation ));

	// TODO: obj attributes
	
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setExternalFile, "image-file" ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setFileIconPressed, "icon-pressed-file" ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setFileIconRollover, "icon-rollover-file" ));
	ruleset.addRule("latex-source", LatexSource());
	ruleset.addRule(itemPrefix, LatexParams());
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,QList<double> >( & PageItem::setDashes, "line-dashes", &parseDoubleList ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setDashOffset, "line-dash-offset", &parseDouble ));

	StoryText::desaxeRules(itemPrefix, ruleset, "text-content");
	Xml_string storyPrefix = Digester::concat(itemPrefix, "text-content");
	ruleset.addRule(storyPrefix, SetItemText());
	
	ruleset.addRule(itemPrefix, LoadPicture());
	
	AdjustGroupIds adjustGroupIds;
	ruleset.addRule("/", adjustGroupIds);
	ruleset.addRule(PageItem::saxxDefaultElem, adjustGroupIds);
	
}

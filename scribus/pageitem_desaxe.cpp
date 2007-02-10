


#include "pageitem.h"
#include "util.h"
//#include "desaxe/simple_actions.h"
#include "desaxe/saxiohelper.h"

using namespace desaxe;


// little helpers
static const char* yesNo(bool what)
{
	return what? "yes" : "no";
}


static Xml_attr PageItemAttributes(const PageItem* item)
{
	Xml_attr result;
	
	result.insert("id", QString::number(const_cast<PageItem*>(item)->getUId()));
	result.insert("name", item->itemName());
	const PageItem* nxt = item->nextInChain(); 
	if (nxt)
		result.insert("nextframe",  QString::number(nxt->getUId())); 
	result.insert("layer", QString::number(item->LayerNr));
	result.insert("level", QString::number(item->ItemNr));
	result.insert("itemtype", QString::number(item->itemType()));
	result.insert("ownpage", QString::number(item->OwnPage));
	
	result.insert("is-bookmark", yesNo(item->isBookmark));
	result.insert("printable", yesNo(item->printEnabled()));
	result.insert("locked", yesNo(item->locked()));
	result.insert("size-locked", yesNo(item->sizeLocked()));
	
	result.insert("width", QString::number(item->width()));
	result.insert("height", QString::number(item->height()));
	result.insert("xorigin", QString::number(item->xPos()));
	result.insert("yorigin", QString::number(item->yPos()));
	//	result.insert("XPOS",item->xPos() - doc->currentPage()->xOffset());
	//	result.insert("YPOS",item->yPos() - doc->currentPage()->yOffset());
	result.insert("rotation", QString::number(item->rotation()));
	result.insert("fill-color", item->fillColor());
	result.insert("fill-shade", QString::number(item->fillShade()));
	result.insert("fill-transparency", QString::number(item->fillTransparency()));
	result.insert("fill-blendmode", QString::number(item->fillBlendmode()));
	result.insert("gradient-type", QString::number(item->GrType));
	result.insert("text-flow-mode", QString::number((int) item->textFlowMode() ));
	result.insert("frame-fill-rule", QString::number(static_cast<int>(item->fillRule)));
	result.insert("do-overprint", yesNo(item->doOverprint));
	
	result.insert("line-color", item->lineColor());
	result.insert("line-shade", QString::number(item->lineShade()));
	result.insert("line-transparency", QString::number(item->lineTransparency()));
	result.insert("line-blendmode", QString::number(item->lineBlendmode()));
	result.insert("line-width", QString::number(item->lineWidth()));
	result.insert("line-style", QString::number(item->PLineArt));
	result.insert("line-cap", QString::number(item->PLineEnd));
	result.insert("line-join", QString::number(item->PLineJoin));
	result.insert("line-customstyle", item->NamedLStyle);
	result.insert("line-start-arrow", QString::number(item->startArrowIndex()));
	result.insert("line-end-arrow", QString::number(item->endArrowIndex()));

	result.insert("frame-type", QString::number(item->FrameType));
	result.insert("frame-shape", toXMLString(item->PoLine));
	result.insert("frame-has-default-shape", yesNo(! item->ClipEdited));	
	result.insert("corner-radius", QString::number(item->cornerRadius()));	
	result.insert("frame-contour", toXMLString(item->ContourLine));
	
	result.insert("left-distance", QString::number(item->textToFrameDistLeft()));
	result.insert("top-distance", QString::number(item->textToFrameDistTop()));
	result.insert("bottom-distance", QString::number(item->textToFrameDistBottom()));
	result.insert("right-distance", QString::number(item->textToFrameDistRight()));

	result.insert("text-autoframes", yesNo(item->isAutoText));
	result.insert("text-columns", QString::number(item->columns()));
	result.insert("text-columngap", QString::number(item->columnGap()));
	result.insert("text-reversed", yesNo(item->reversed()));
	result.insert("text-path-type", QString::number(item->textPathType));
	result.insert("text-path-flipped", yesNo(item->textPathFlipped));
	result.insert("text-path-baseoffset", QString::number(item->BaseOffs));
	result.insert("text-path-showpath", yesNo(item->PoShow));

	result.insert("image-scale-framesize", yesNo(item->ScaleType));
	result.insert("image-scale-proportional", yesNo(item->AspectRatio));
	result.insert("image-x-scale", QString::number(item->imageXScale()));
	result.insert("image-y-scale", QString::number(item->imageYScale()));
	result.insert("image-x-position", QString::number(item->imageXOffset()));
	result.insert("image-y-position", QString::number(item->imageYOffset()));
	//	result.insert("image-x-dpi", QString::number(item->imageXDpi()));
	//	result.insert("image-y-dpi", QString::number(item->imageYDpi()));
	result.insert("image-shown", yesNo(item->imageShown()));
	result.insert("image-use-embedded", yesNo(item->UseEmbedded));
	result.insert("image-embedded-profile", item->EmProfile);
	result.insert("image-flipped-hor", yesNo(item->imageFlippedH()));
	result.insert("image-flipped-vert", yesNo(item->imageFlippedV()));
	result.insert("cms-profile", item->IProfile);
	result.insert("cms-intent", QString::number(item->IRender));
	
	result.insert("isGroupControl", yesNo(item->isGroupControl));
	if (item->isGroupControl)
	{
		if (item->groupsLastItem != 0)
			result.insert("groupsLastItem", QString::number(item->groupsLastItem->ItemNr - item->ItemNr));
	}
	result.insert("NUMGROUP", QString::number(item->Groups.count()));
	QString glp = "";
	QValueStack<int>::const_iterator nx;
	for (nx = item->Groups.begin(); nx != item->Groups.end(); ++nx)
		glp += QString::number(*nx) + " ";
	result.insert("GROUPS", glp);
	
	result.insert("isTableItem", yesNo(item->isTableItem));
	result.insert("TopLine", yesNo(item->TopLine));
	result.insert("LeftLine", yesNo(item->LeftLine));
	result.insert("RightLine", yesNo(item->RightLine));
	result.insert("BottomLine", yesNo(item->BottomLine));
	if (item->isTableItem)
	{
		if (item->TopLink != 0)
			result.insert("TopLINK", QString::number(item->TopLink->ItemNr));
		else
			result.insert("TopLINK", "-1");
		if (item->LeftLink != 0)
			result.insert("LeftLINK", QString::number(item->LeftLink->ItemNr));
		else
			result.insert("LeftLINK", "-1");
		if (item->RightLink != 0)
			result.insert("RightLINK", QString::number(item->RightLink->ItemNr));
		else
			result.insert("RightLINK", "-1");
		if (item->BottomLink != 0)
			result.insert("BottomLINK", QString::number(item->BottomLink->ItemNr));
		else
			result.insert("BottomLINK", "-1");
		result.insert("OwnLINK", QString::number(item->ItemNr));
	}
	
//	result.insert("ANNAME", !item->AutoName ? item->itemName() : QString(""));
	if ((item->itemType()==PageItem::ImageFrame || item->itemType()==PageItem::TextFrame) && (!item->Pfile.isEmpty()))
		result.insert("PFILE", Path2Relative(item->Pfile));
	else
		result.insert("PFILE","");
	if (!item->Pfile2.isEmpty())
		result.insert("PFILE2", Path2Relative(item->Pfile2));
	else
		result.insert("PFILE2","");
	if (!item->Pfile3.isEmpty())
		result.insert("PFILE3", Path2Relative(item->Pfile3));
	else
		result.insert("PFILE3","");
	result.insert("NUMDASH", QString::number(item->DashValues.count()));
	QString dlp = "";
	QValueList<double>::const_iterator dax;
	for (dax = item->DashValues.begin(); dax != item->DashValues.end(); ++dax)
		dlp += QString::number(*dax) + " ";
	result.insert("DASHS", dlp);
	result.insert("DASHOFF", QString::number(item->DashOffset));
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
			imeff.insert("Code", QString::number((*effectsInUse.at(a)).effectCode));
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
			psd.insert("Layer", QString::number(it2.key()));
			psd.insert("Visible", yesNo(it2.data().visible));
			psd.insert("useMask", yesNo(it2.data().useMask));
			psd.insert("Opacity", QString::number(it2.data().opacity));
			psd.insert("Blend", it2.data().blend);
			handler.begin("PSDLayer", psd);
			handler.end("PSDLayer");
		}
	}
	if (GrType != 0)
	{
		if (GrType == 8)
		{
			Xml_attr patt;
			patt.insert("pattern", pattern());
			double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation;
			const_cast<PageItem*>(this)->
				patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
			patt.insert("pScaleX", QString::number(patternScaleX));
			patt.insert("pScaleY", QString::number(patternScaleY));
			patt.insert("pOffsetX", QString::number(patternOffsetX));
			patt.insert("pOffsetY", QString::number(patternOffsetY));
			patt.insert("pRotation", QString::number(patternRotation));
			handler.begin("Pattern", patt);
			handler.end("Pattern");
		}
		else
		{
			Xml_attr gradient;
			gradient.insert("GRSTARTX", QString::number(GrStartX));
			gradient.insert("GRSTARTY", QString::number(GrStartY));
			gradient.insert("GRENDX", QString::number(GrEndX));
			gradient.insert("GRENDY", QString::number(GrEndY));
			handler.begin("Gradient", gradient);
			QPtrVector<VColorStop> cstops = fill_gradient.colorStops();
			for (uint cst = 0; cst < const_cast<VGradient&>(fill_gradient).Stops(); ++cst) //FIXME make const
			{
				Xml_attr itcl;
				itcl.insert("RAMP", QString::number(cstops.at(cst)->rampPoint));
				itcl.insert("NAME", cstops.at(cst)->name);
				itcl.insert("SHADE", QString::number(cstops.at(cst)->shade));
				itcl.insert("TRANS", QString::number(cstops.at(cst)->opacity));
				handler.begin("CStop",itcl);
				handler.end("CStop");
			}
			handler.end("Gradient");
		}
	}

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

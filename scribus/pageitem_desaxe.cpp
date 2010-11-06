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
#ifdef HAVE_OSG
	#include "pageitem_osgframe.h"
#endif
#include "scribusdoc.h"
#include "sctextstream.h"
#include "util_color.h"
#include "util_math.h"
#include "util.h"

#include <QDebug>

using namespace desaxe;

eRenderIntent parseIntent(const Xml_string& str)
{
	return (eRenderIntent) str.toInt();
}

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
	result.insert("layer", toXMLString(item->LayerID));
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
	//Page* page = (item->OwnPage >= 0) ? item->doc()->Pages->at(item->OwnPage) : NULL;
	Page* page = item->doc()->currentPage(); //#6175
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
	result.insert("line-start-arrow-scale", toXMLString(item->startArrowScale()));
	result.insert("line-end-arrow-scale", toXMLString(item->endArrowScale()));

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
	result.insert("first-line-offset", toXMLString(item->firstLineOffset()));

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
	result.insert("image-rotation", toXMLString(item->imageRotation()));
	result.insert("image-clip", toXMLString(item->pixm.imgInfo.usedPath));
	if (item->OverrideCompressionMethod)
	{
		result.insert("COMPRESSIONMETHODOVER", toXMLString(item->OverrideCompressionMethod));
		result.insert("COMPRESSIONMETHOD", toXMLString(item->CompressionMethodIndex));
	}
	if (item->OverrideCompressionQuality)
	{
		result.insert("COMPRESSIONQUALITYOVER", toXMLString(item->OverrideCompressionQuality));
		result.insert("COMPRESSIONQUALITY", toXMLString(item->CompressionQualityIndex));
	}
	//	result.insert("image-x-dpi", toXMLString(item->imageXDpi()));
	//	result.insert("image-y-dpi", toXMLString(item->imageYDpi()));
	result.insert("image-shown", toXMLString(item->imageShown()));
	result.insert("image-use-embedded", toXMLString(item->useEmbeddedImageProfile()));
	result.insert("image-embedded-profile", item->embeddedImageProfile());
	result.insert("image-flipped-hor", toXMLString(item->imageFlippedH()));
	result.insert("image-flipped-vert", toXMLString(item->imageFlippedV()));
	result.insert("pagenumber", toXMLString(item->pixm.imgInfo.actualPageNumber));
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
	if (item->itemType() == PageItem::Symbol)
		result.insert("pattern", item->pattern());
	if ((item->itemType()==PageItem::ImageFrame || item->itemType()==PageItem::TextFrame) && (!item->externalFile().isEmpty()) && !latexframe)
	{
		if (item->isInlineImage)
		{
			result.insert("image-file", "");
			result.insert("isInlineImage", toXMLString(item->isInlineImage));
			QFileInfo inlFi(item->Pfile);
			result.insert("inlineImageExt", inlFi.suffix());
			QFile inFil(item->Pfile);
			if (inFil.open(QIODevice::ReadOnly))
			{
				QByteArray ba = qCompress(inFil.readAll()).toBase64();
				result.insert("ImageData", QString(ba));
				inFil.close();
			}
		}
		else
			result.insert("image-file", Path2Relative(item->externalFile(), QDir::homePath()));
	}
#ifdef HAVE_OSG
	if (item->realItemType() == PageItem::OSGFrame)
	{
		if (!item->Pfile.isEmpty())
		{
			const PageItem_OSGFrame *osgframe = dynamic_cast<const PageItem_OSGFrame*>(item);
			result.insert("modelFile", Path2Relative(osgframe->modelFile, QDir::homePath()));
			result.insert("currentViewName", osgframe->currentView);
		}
	}
#endif
	if (!item->fileIconPressed().isEmpty())
		result.insert("icon-pressed-file", Path2Relative(item->fileIconPressed(), QDir::homePath()));
	if (!item->fileIconRollover().isEmpty())
		result.insert("icon-rollover-file", Path2Relative(item->fileIconRollover(), QDir::homePath()));
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
//	qDebug() << QString("PageItem::saxx %1 %2").arg((ulong) &handler);
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
	if (!strokePattern().isEmpty())
	{
		Xml_attr patt;
		patt.insert("patternS", strokePattern());
		patt.insert("pScaleXS", toXMLString(patternStrokeScaleX));
		patt.insert("pScaleYS", toXMLString(patternStrokeScaleY));
		patt.insert("pOffsetXS", toXMLString(patternStrokeOffsetX));
		patt.insert("pOffsetYS", toXMLString(patternStrokeOffsetY));
		patt.insert("pRotationS", toXMLString(patternStrokeRotation));
		patt.insert("pSkewXS",   toXMLString(patternStrokeSkewX));
		patt.insert("pSkewYS",   toXMLString(patternStrokeSkewY));
		patt.insert("pMirrorXS", toXMLString(patternStrokeMirrorX));
		patt.insert("pMirrorYS", toXMLString(patternStrokeMirrorY));
		patt.insert("pSpaceS", toXMLString(patternStrokeSpace));
		patt.insert("pAtPathS",  toXMLString(patternStrokePath));
		handler.begin("PatternStroke", patt);
		handler.end("PatternStroke");
	}
	if (!patternMask().isEmpty())
	{
		Xml_attr patt;
		patt.insert("patternM", patternMask());
		patt.insert("pScaleXM", toXMLString(patternMaskScaleX));
		patt.insert("pScaleYM", toXMLString(patternMaskScaleY));
		patt.insert("pOffsetXM", toXMLString(patternMaskOffsetX));
		patt.insert("pOffsetYM", toXMLString(patternMaskOffsetY));
		patt.insert("pRotationM", toXMLString(patternMaskRotation));
		patt.insert("pSkewXM", toXMLString(patternMaskSkewX));
		patt.insert("pSkewYM", toXMLString(patternMaskSkewY));
		patt.insert("pMirrorXM", toXMLString(patternMaskMirrorX));
		patt.insert("pMirrorYM", toXMLString(patternMaskMirrorY));
		handler.begin("PatternMask", patt);
		handler.end("PatternMask");
	}
	if (gradientType() != 0)
	{
		if (gradientType() == 8)
		{
			Xml_attr patt;
			patt.insert("pattern", pattern());
			patt.insert("pScaleX", toXMLString(patternScaleX));
			patt.insert("pScaleY", toXMLString(patternScaleY));
			patt.insert("pOffsetX", toXMLString(patternOffsetX));
			patt.insert("pOffsetY", toXMLString(patternOffsetY));
			patt.insert("pRotation", toXMLString(patternRotation));
			patt.insert("pSkewX", toXMLString(patternSkewX));
			patt.insert("pSkewY", toXMLString(patternSkewY));
			patt.insert("pMirrorX", toXMLString(patternMirrorX));
			patt.insert("pMirrorY", toXMLString(patternMirrorY));
			handler.begin("Pattern", patt);
			handler.end("Pattern");
		}
		else if (gradientType() == 11)
		{
			Xml_attr gradientGM;
			gradientGM.insert("GRTYPE", toXMLString(GrType));
			gradientGM.insert("GMAY", toXMLString(meshGradientArray[0].count()));
			gradientGM.insert("GMAX", toXMLString(meshGradientArray.count()));
			handler.begin("Mesh", gradientGM);
			for (int grow = 0; grow < meshGradientArray.count(); grow++)
			{
				for (int gcol = 0; gcol < meshGradientArray[grow].count(); gcol++)
				{
					Xml_attr itcm;
					meshPoint mp = meshGradientArray[grow][gcol];
					itcm.insert("GX", toXMLString(mp.gridPoint.x()));
					itcm.insert("GY", toXMLString(mp.gridPoint.y()));
					itcm.insert("CTX", toXMLString(mp.controlTop.x()));
					itcm.insert("CTY", toXMLString(mp.controlTop.y()));
					itcm.insert("CBX", toXMLString(mp.controlBottom.x()));
					itcm.insert("CBY", toXMLString(mp.controlBottom.y()));
					itcm.insert("CLX", toXMLString(mp.controlLeft.x()));
					itcm.insert("CLY", toXMLString(mp.controlLeft.y()));
					itcm.insert("CRX", toXMLString(mp.controlRight.x()));
					itcm.insert("CRY", toXMLString(mp.controlRight.y()));
					itcm.insert("NAME", toXMLString(mp.colorName));
					itcm.insert("SHADE", toXMLString(mp.shade));
					itcm.insert("TRANS", toXMLString(mp.transparency));
					handler.begin("MPoints",itcm);
					handler.end("MPoints");
				}
			}
			handler.end("Mesh");
		}
		else
		{
			Xml_attr gradientV;
			gradientV.insert("GRTYPE", toXMLString(GrType));
			gradientV.insert("GRSTARTX", toXMLString(GrStartX));
			gradientV.insert("GRSTARTY", toXMLString(GrStartY));
			gradientV.insert("GRENDX", toXMLString(GrEndX));
			gradientV.insert("GRENDY", toXMLString(GrEndY));
			gradientV.insert("GRFOCALX", toXMLString(GrFocalX));
			gradientV.insert("GRFOCALY", toXMLString(GrFocalY));
			gradientV.insert("GRSCALE", toXMLString(GrScale));
			gradientV.insert("GRSKEW", toXMLString(GrSkew));
			gradientV.insert("GRNAME", toXMLString(gradient()));
			gradientV.insert("GRC1X"   , toXMLString(GrControl1.x()));
			gradientV.insert("GRC1Y"   , toXMLString(GrControl1.y()));
			gradientV.insert("GRCOLP1" , toXMLString(GrColorP1));
			gradientV.insert("GRC2X"   , toXMLString(GrControl2.x()));
			gradientV.insert("GRC2Y"   , toXMLString(GrControl2.y()));
			gradientV.insert("GRCOLP2" , toXMLString(GrColorP2));
			gradientV.insert("GRC3X"   , toXMLString(GrControl3.x()));
			gradientV.insert("GRC3Y"   , toXMLString(GrControl3.y()));
			gradientV.insert("GRCOLP3" , toXMLString(GrColorP3));
			gradientV.insert("GRC4X"   , toXMLString(GrControl4.x()));
			gradientV.insert("GRC4Y"   , toXMLString(GrControl4.y()));
			gradientV.insert("GRC5X"   , toXMLString(GrControl5.x()));
			gradientV.insert("GRC5Y"   , toXMLString(GrControl5.y()));
			gradientV.insert("GRCOLP4" , toXMLString(GrColorP4));
			gradientV.insert("GRCOLT1", toXMLString(GrCol1transp));
			gradientV.insert("GRCOLT2", toXMLString(GrCol2transp));
			gradientV.insert("GRCOLT3", toXMLString(GrCol3transp));
			gradientV.insert("GRCOLT4", toXMLString(GrCol4transp));
			gradientV.insert("GRCOLS1", toXMLString(GrCol1Shade));
			gradientV.insert("GRCOLS2", toXMLString(GrCol2Shade));
			gradientV.insert("GRCOLS3", toXMLString(GrCol3Shade));
			gradientV.insert("GRCOLS4", toXMLString(GrCol4Shade));
			handler.begin("Gradient", gradientV);
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
	if (GrTypeStroke > 0)
	{
		Xml_attr gradientV;
		gradientV.insert("GRTYPES", toXMLString(GrTypeStroke));
		gradientV.insert("GRSTARTXS", toXMLString(GrStrokeStartX));
		gradientV.insert("GRSTARTYS", toXMLString(GrStrokeStartY));
		gradientV.insert("GRENDXS", toXMLString(GrStrokeEndX));
		gradientV.insert("GRENDYS", toXMLString(GrStrokeEndY));
		gradientV.insert("GRFOCALXS", toXMLString(GrStrokeFocalX));
		gradientV.insert("GRFOCALYS", toXMLString(GrStrokeFocalY));
		gradientV.insert("GRSCALES", toXMLString(GrStrokeScale));
		gradientV.insert("GRSKEWS", toXMLString(GrStrokeSkew));
		gradientV.insert("GRNAMES", toXMLString(strokeGradient()));
		handler.begin("GradientS", gradientV);
		QList<VColorStop*> cstops = fill_gradient.colorStops();
		for (uint cst = 0; cst < const_cast<VGradient&>(fill_gradient).Stops(); ++cst) //FIXME make const
		{
			Xml_attr itcl;
			itcl.insert("RAMP", toXMLString(cstops.at(cst)->rampPoint));
			itcl.insert("NAME", cstops.at(cst)->name);
			itcl.insert("SHADE", toXMLString(cstops.at(cst)->shade));
			itcl.insert("TRANS", toXMLString(cstops.at(cst)->opacity));
			handler.begin("S_CStop",itcl);
			handler.end("S_CStop");
		}
		handler.end("GradientS");
	}
	if (GrMask > 0)
	{
		Xml_attr gradientV;
		gradientV.insert("GRTYPEM", toXMLString(GrMask));
		if (mask_gradient.type() == VGradient::radial)
			gradientV.insert("GRMK", "1");
		else
			gradientV.insert("GRMK", "0");
		gradientV.insert("GRSTARTXM", toXMLString(GrMaskStartX));
		gradientV.insert("GRSTARTYM", toXMLString(GrMaskStartY));
		gradientV.insert("GRENDXM", toXMLString(GrMaskEndX));
		gradientV.insert("GRENDYM", toXMLString(GrMaskEndY));
		gradientV.insert("GRFOCALXM", toXMLString(GrMaskFocalX));
		gradientV.insert("GRFOCALYM", toXMLString(GrMaskFocalY));
		gradientV.insert("GRSCALEM", toXMLString(GrMaskScale));
		gradientV.insert("GRSKEWM", toXMLString(GrMaskSkew));
		gradientV.insert("GRNAMEM", toXMLString(gradientMask()));
		handler.begin("GradientM", gradientV);
		QList<VColorStop*> cstops = mask_gradient.colorStops();
		for (uint cst = 0; cst < const_cast<VGradient&>(mask_gradient).Stops(); ++cst) //FIXME make const
		{
			Xml_attr itcl;
			itcl.insert("RAMP", toXMLString(cstops.at(cst)->rampPoint));
			itcl.insert("NAME", cstops.at(cst)->name);
			itcl.insert("SHADE", toXMLString(cstops.at(cst)->shade));
			itcl.insert("TRANS", toXMLString(cstops.at(cst)->opacity));
			handler.begin("M_CStop",itcl);
			handler.end("M_CStop");
		}
		handler.end("GradientM");
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
	if (latexframe)
	{
		handler.begin("latex-source", empty);
		QMapIterator<QString, QString> i(latexframe->editorProperties);
		while (i.hasNext())
		{
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
#ifdef HAVE_OSG
	if (this->realItemType() == PageItem::OSGFrame)
	{
		const PageItem_OSGFrame *osgitem = dynamic_cast<const PageItem_OSGFrame*>(this);
		if (!osgitem->Pfile.isEmpty())
		{
			QHashIterator<QString, PageItem_OSGFrame::viewDefinition> itv(osgitem->viewMap);
			while (itv.hasNext())
			{
				Xml_attr osg;
				itv.next();
				osg.insert("viewName", toXMLString(itv.key()));
				osg.insert("angleFOV", toXMLString(itv.value().angleFOV));
				QString tmp;
				QString trackM = "";
				for (uint matx = 0; matx < 4; ++matx)
				{
					for (uint maty = 0; maty < 4; ++maty)
					{
						trackM += tmp.setNum(itv.value().trackerMatrix(matx, maty))+" ";
					}
				}
				osg.insert("trackM", trackM);
				QString trackC = "";
				trackC += tmp.setNum(itv.value().trackerCenter[0])+" ";
				trackC += tmp.setNum(itv.value().trackerCenter[1])+" ";
				trackC += tmp.setNum(itv.value().trackerCenter[2]);
				osg.insert("trackC", trackC);
				QString cameraP = "";
				cameraP += tmp.setNum(itv.value().cameraPosition[0])+" ";
				cameraP += tmp.setNum(itv.value().cameraPosition[1])+" ";
				cameraP += tmp.setNum(itv.value().cameraPosition[2]);
				osg.insert("cameraP", cameraP);
				QString cameraU = "";
				cameraU += tmp.setNum(itv.value().cameraUp[0])+" ";
				cameraU += tmp.setNum(itv.value().cameraUp[1])+" ";
				cameraU += tmp.setNum(itv.value().cameraUp[2]);
				osg.insert("cameraU", cameraU);
				osg.insert("trackerDist", toXMLString(itv.value().trackerDist));
				osg.insert("trackerSize", toXMLString(itv.value().trackerSize));
				osg.insert("illumination", toXMLString(itv.value().illumination));
				osg.insert("rendermode", toXMLString(itv.value().rendermode));
				osg.insert("trans", toXMLString(itv.value().addedTransparency));
				osg.insert("colorAC", toXMLString(itv.value().colorAC.name()));
				osg.insert("colorFC", toXMLString(itv.value().colorFC.name()));
				handler.begin("OSGViews", osg);
				handler.end("OSGViews");
			}
		}
	}
#endif
	handler.end(elemtag);
}

#ifdef HAVE_OSG
class OSGViews_body : public Action_body
{
	void begin (const Xml_string& tagName, Xml_attr attr)
	{
		if (tagName=="OSGViews")
		{
			PageItem_OSGFrame* osgframe = dynamic_cast<PageItem_OSGFrame *> (this->dig->top<PageItem>());
			struct PageItem_OSGFrame::viewDefinition defaultView;
			defaultView.angleFOV = parseDouble(attr["angleFOV"]);
			QString tmp = "";
			tmp = attr["trackM"];
			ScTextStream fp(&tmp, QIODevice::ReadOnly);
			double m1, m2, m3, m4;
			double m5, m6, m7, m8;
			double m9, m10, m11, m12;
			double m13, m14, m15, m16;
			fp >> m1 >> m2 >> m3 >> m4;
			fp >> m5 >> m6 >> m7 >> m8;
			fp >> m9 >> m10 >> m11 >> m12;
			fp >> m13 >> m14 >> m15 >> m16;
			defaultView.trackerMatrix.set(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16);
			tmp = "";
			tmp = attr["trackC"];
			ScTextStream fp2(&tmp, QIODevice::ReadOnly);
			double v1, v2, v3;
			fp2 >> v1 >> v2 >> v3;
			defaultView.trackerCenter.set(v1, v2, v3);
			tmp = "";
			tmp = attr["cameraP"];
			ScTextStream fp3(&tmp, QIODevice::ReadOnly);
			fp3 >> v1 >> v2 >> v3;
			defaultView.cameraPosition.set(v1, v2, v3);
			tmp = "";
			tmp = attr["cameraU"];
			ScTextStream fp4(&tmp, QIODevice::ReadOnly);
			fp4 >> v1 >> v2 >> v3;
			defaultView.cameraUp.set(v1, v2, v3);
			defaultView.trackerDist = parseDouble(attr["trackerDist"]);
			defaultView.trackerSize = parseDouble(attr["trackerSize"]);
			defaultView.illumination = static_cast<PageItem_OSGFrame::LightType>(parseInt(attr["illumination"]));
			defaultView.rendermode = static_cast<PageItem_OSGFrame::RenderType>(parseInt(attr["rendermode"]));
			defaultView.addedTransparency = parseDouble(attr["trans"]);
			defaultView.colorAC = QColor(attr["colorAC"]);
			defaultView.colorFC = QColor(attr["colorFC"]);
			QString name = attr["viewName"];
			osgframe->viewMap.insert(name, defaultView);
		}
	}
};

class OSGViews : public MakeAction<OSGViews_body>
{};
#endif

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
//		qDebug() << QString("pushed item: %1,%2 params %3,%4").arg(result->xPos()).arg(result->yPos()).arg(xpos).arg(ypos);
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
//		qDebug() << QString("pageitem_desaxe: gradient %1").arg(tagName);
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
		if (tagName=="S_CStop")
		{
			PageItem* item = this->dig->top<PageItem>();
			QString name = attr["NAME"];
			double ramp = parseDouble(attr["RAMP"]);
			int shade = parseInt(attr["SHADE"]);
			double opa = parseDouble(attr["TRANS"]);
			item->stroke_gradient.addStop( QColor(150, 100, 50) , ramp, 0.5, opa, name, shade);
		}
		if (tagName=="M_CStop")
		{
			PageItem* item = this->dig->top<PageItem>();
			QString name = attr["NAME"];
			double ramp = parseDouble(attr["RAMP"]);
			int shade = parseInt(attr["SHADE"]);
			double opa = parseDouble(attr["TRANS"]);
			item->mask_gradient.addStop( QColor(150, 100, 50) , ramp, 0.5, opa, name, shade);
		}
		if (tagName=="GradientS")
		{
			PageItem* item = this->dig->top<PageItem>();
			item->GrTypeStroke = parseInt(attr["GRTYPES"]);
			item->GrStrokeStartX = parseDouble(attr["GRSTARTXS"]);
			item->GrStrokeStartY = parseDouble(attr["GRSTARTYS"]);
			item->GrStrokeEndX = parseDouble(attr["GRENDXS"]);
			item->GrStrokeEndY = parseDouble(attr["GRENDYS"]);
			item->GrStrokeFocalX = parseDouble(attr["GRFOCALXS"]);
			item->GrStrokeFocalY = parseDouble(attr["GRFOCALYS"]);
			item->GrStrokeScale = parseDouble(attr["GRSCALES"]);
			item->GrStrokeSkew = parseDouble(attr["GRSKEWS"]);
			item->setStrokeGradient(attr["GRNAMES"]);
			item->stroke_gradient.clearStops();
		}
		if (tagName=="GradientM")
		{
			PageItem* item = this->dig->top<PageItem>();
			item->GrMask = parseInt(attr["GRTYPEM"]);
			item->GrMaskStartX = parseDouble(attr["GRSTARTXM"]);
			item->GrMaskStartY = parseDouble(attr["GRSTARTYM"]);
			item->GrMaskEndX = parseDouble(attr["GRENDXM"]);
			item->GrMaskEndY = parseDouble(attr["GRENDYM"]);
			item->GrMaskFocalX = parseDouble(attr["GRFOCALXM"]);
			item->GrMaskFocalY = parseDouble(attr["GRFOCALYM"]);
			item->GrMaskScale = parseDouble(attr["GRSCALEM"]);
			item->GrMaskSkew = parseDouble(attr["GRSKEWM"]);
			item->setGradientMask(attr["GRNAMEM"]);
			int grM = parseInt(attr["GRMK"]);
			if (grM == 0)
				item->mask_gradient = VGradient(VGradient::linear);
			else
				item->mask_gradient = VGradient(VGradient::radial);
			item->mask_gradient.clearStops();
		}
		if (tagName=="Gradient")
		{
			PageItem* item = this->dig->top<PageItem>();
			item->GrType = parseInt(attr["GRTYPE"]);
			item->GrStartX = parseDouble(attr["GRSTARTX"]);
			item->GrStartY = parseDouble(attr["GRSTARTY"]);
			item->GrEndX = parseDouble(attr["GRENDX"]);
			item->GrEndY = parseDouble(attr["GRENDY"]);
			item->GrFocalX = parseDouble(attr["GRFOCALX"]);
			item->GrFocalY = parseDouble(attr["GRFOCALY"]);
			item->GrScale = parseDouble(attr["GRSCALE"]);
			item->GrSkew = parseDouble(attr["GRSKEW"]);
			item->setGradient(attr["GRNAME"]);
			item->fill_gradient.clearStops();
			item->GrControl1 = FPoint(parseDouble(attr["GRC1X"]), parseDouble(attr["GRC1Y"]));
			item->GrControl2 = FPoint(parseDouble(attr["GRC2X"]), parseDouble(attr["GRC2Y"]));
			item->GrControl3 = FPoint(parseDouble(attr["GRC3X"]), parseDouble(attr["GRC3Y"]));
			item->GrControl4 = FPoint(parseDouble(attr["GRC4X"]), parseDouble(attr["GRC4Y"]));
			item->GrControl5 = FPoint(parseDouble(attr["GRC5X"]), parseDouble(attr["GRC5Y"]));
			item->GrColorP1  = attr["GRCOLP1"];
			item->GrColorP2  = attr["GRCOLP2"];
			item->GrColorP3  = attr["GRCOLP3"];
			item->GrColorP4  = attr["GRCOLP4"];
			item->GrCol1transp = parseDouble(attr["GRCOLT1"]);
			item->GrCol2transp = parseDouble(attr["GRCOLT2"]);
			item->GrCol3transp = parseDouble(attr["GRCOLT3"]);
			item->GrCol4transp = parseDouble(attr["GRCOLT4"]);
			item->GrCol1Shade = parseDouble(attr["GRCOLS1"]);
			item->GrCol2Shade = parseDouble(attr["GRCOLS2"]);
			item->GrCol3Shade = parseDouble(attr["GRCOLS3"]);
			item->GrCol4Shade = parseDouble(attr["GRCOLS4"]);
		}
		if (tagName=="MPoints")
		{
			PageItem* item = this->dig->top<PageItem>();
			meshPoint mp;
			mp.colorName     = attr["NAME"];
			mp.shade         = parseInt(attr["SHADE"]);
			mp.transparency  = parseDouble(attr["TRANS"]);
			mp.gridPoint     = FPoint(parseDouble(attr["GX"]), parseDouble(attr["GY"]));
			mp.controlTop    = FPoint(parseDouble(attr["CTX"]), parseDouble(attr["CTY"]));
			mp.controlBottom = FPoint(parseDouble(attr["CBX"]), parseDouble(attr["CBY"]));
			mp.controlLeft   = FPoint(parseDouble(attr["CLX"]), parseDouble(attr["CLY"]));
			mp.controlRight  = FPoint(parseDouble(attr["CRX"]), parseDouble(attr["CRY"]));
			mp.color         = QColor(150, 100, 50);
			item->meshGradientArray[item->GrStartX][item->GrStartY] = mp;
			item->GrStartY++;
			if (item->GrStartY == item->meshGradientArray[item->GrStartX].count())
			{
				item->GrStartY = 0;
				item->GrStartX++;
			}
		}
		if (tagName=="Mesh")
		{
			PageItem* item = this->dig->top<PageItem>();
			item->meshGradientArray.clear();
			item->GrType = parseInt(attr["GRTYPE"]);
			int mGArrayRows = parseInt(attr["GMAX"]);
			int mGArrayCols = parseInt(attr["GMAY"]);
			item->GrStartX = 0;
			item->GrStartY = 0;
			for (int mgr = 0; mgr < mGArrayRows; mgr++)
			{
				QList<meshPoint> ml;
				for (int mgc = 0; mgc < mGArrayCols; mgc++)
				{
					meshPoint mp;
					ml.append(mp);
				}
				item->meshGradientArray.append(ml);
			}
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
		double patternSkewX = parseDouble(attr["pSkewX"]);
		double patternSkewY = parseDouble(attr["pSkewY"]);
		bool mirrorX = parseBool(attr["pMirrorX"]);
		bool mirrorY = parseBool(attr["pMirrorY"]);
		item->setPattern(attr["pattern"]);
		item->setPatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
		item->setPatternFlip(mirrorX, mirrorY);
	}
};

class Pattern : public MakeAction<Pattern_body>
{};

class PatternStroke_body : public Action_body
{
	void begin (const Xml_string& tagName, Xml_attr attr)
	{
		PageItem* item = this->dig->top<PageItem>();
		
		double patternScaleX = parseDouble(attr["pScaleXS"]);
		double patternScaleY = parseDouble(attr["pScaleYS"]);
		double patternOffsetX = parseDouble(attr["pOffsetXS"]);
		double patternOffsetY = parseDouble(attr["pOffsetYS"]);
		double patternRotation = parseDouble(attr["pRotationS"]);
		double patternSkewX = parseDouble(attr["pSkewXS"]);
		double patternSkewY = parseDouble(attr["pSkewYS"]);
		double patternSpace = parseDouble(attr["pSpaceS"]);
		bool mirrorX = parseBool(attr["pMirrorXS"]);
		bool mirrorY = parseBool(attr["pMirrorYS"]);
		bool atPath = parseBool(attr["pAtPathS"]);
		item->setStrokePattern(attr["patternS"]);
		item->setStrokePatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace);
		item->setStrokePatternFlip(mirrorX, mirrorY);
		item->setStrokePatternToPath(atPath);
	}
};

class PatternStroke : public MakeAction<PatternStroke_body>
{};

class PatternMask_body : public Action_body
{
	void begin (const Xml_string& tagName, Xml_attr attr)
	{
		PageItem* item = this->dig->top<PageItem>();
		
		double patternScaleX = parseDouble(attr["pScaleXM"]);
		double patternScaleY = parseDouble(attr["pScaleYM"]);
		double patternOffsetX = parseDouble(attr["pOffsetXM"]);
		double patternOffsetY = parseDouble(attr["pOffsetYM"]);
		double patternRotation = parseDouble(attr["pRotationM"]);
		double patternSkewX = parseDouble(attr["pSkewXM"]);
		double patternSkewY = parseDouble(attr["pSkewYM"]);
		bool mirrorX = parseBool(attr["pMirrorXM"]);
		bool mirrorY = parseBool(attr["pMirrorYM"]);
		item->setPatternMask(attr["patternM"]);
		item->maskTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
		item->setMaskFlip(mirrorX, mirrorY);
	}
};

class PatternMask : public MakeAction<PatternMask_body>
{};



class ImageEffectsAndLayers_body : public Action_body
{
	void begin (const Xml_string& tagName, Xml_attr attr)
	{
//		qDebug() << QString("pageitem_desaxe: effects/layers %1").arg(tagName);
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
	void begin (const Xml_string& tagName, Xml_attr attr)
	{
		clipPath = attr["image-clip"];
	}
	void end (const Xml_string& /*tagname*/)
	{
		PageItem* item = this->dig->top<PageItem>();
		if (item->itemType() == PageItem::ImageFrame)
			item->loadImage(item->externalFile(), true);
		if (item->pixm.imgInfo.PDSpathData.contains(clipPath))
		{
			item->imageClip = item->pixm.imgInfo.PDSpathData[clipPath].copy();
			item->pixm.imgInfo.usedPath = clipPath;
			QTransform cl;
			cl.translate(item->imageXOffset()*item->imageXScale(), item->imageYOffset()*item->imageYScale());
			cl.scale(item->imageXScale(), item->imageYScale());
			item->imageClip.map(cl);
			item->loadImage(item->externalFile(), true);
		}
	}
private:
	QString clipPath;
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
		else
		{
			PageItem* item = this->dig->top<PageItem>();
			QStack<int> groups;
			for (int i=0; i < item->groups().count(); ++i)
			{
				int newGroup = minGroup + item->groups()[i];
				if (newGroup > maxGroup)
					maxGroup = newGroup;
//				qDebug() << QString("group id %1 -> %2").arg(item->groups()[i]).arg(newGroup);
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

class LatexParamsExtra_body : public Action_body
{
	void begin (const Xml_string& tagName, Xml_attr attr)
	{
		if (tagName=="property")
		{
			if (this->dig->top<PageItem>()->realItemType() == PageItem::LatexFrame)
			{
				PageItem_LatexFrame* obj = dynamic_cast<PageItem_LatexFrame *> (this->dig->top<PageItem>());
				QString name  = attr["name"];
				QString value = attr["value"];
				obj->editorProperties[name] = value;
			}
		}
	}
};

class LatexParamsExtra : public MakeAction<LatexParamsExtra_body>
{};


#ifdef HAVE_OSG
class OSGParams_body : public Action_body
{
	public:	
		void begin(const Xml_string& tag, Xml_attr attr) 
		{
			if (this->dig->top<PageItem>()->realItemType() == PageItem::OSGFrame)
			{
				PageItem_OSGFrame* osgframe = dynamic_cast<PageItem_OSGFrame *> (this->dig->top<PageItem>());
				osgframe->setExternalModelFile(attr["modelFile"]);
				osgframe->currentView = attr["currentViewName"];
				osgframe->loadModel();
			}
		}
};

struct  OSGParams : public MakeAction<OSGParams_body> 
{};
#endif

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
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setStartArrowScale, "line-start-arrow-scale", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setEndArrowScale, "line-end-arrow-scale", &parseInt ));
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setFrameType, "frame-type", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem, FPointArray>( & PageItem::setShape, "frame-shape", &parseSVG ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setHasDefaultShape, "frame-has-default-shape", &parseBool ));	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setCornerRadius, "corner-radius", &parseDouble ));	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,FPointArray>( & PageItem::setContour, "frame-contour", &parseSVG ));
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setTextToFrameDistLeft, "left-distance", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setTextToFrameDistTop, "top-distance", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setTextToFrameDistBottom, "bottom-distance", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setTextToFrameDistRight, "right-distance", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,FirstLineOffsetPolicy>( & PageItem::setFirstLineOffset, "first-line-offset", &parseEnum ));

	
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
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setImageRotation, "image-rotation", &parseDouble ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setImagePagenumber, "pagenumber", &parseInt ));
	//	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>("image-x-dpi", &parseDouble ));  //NYI
	//	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>("image-y-dpi", &parseDouble ));  //NYI
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setImageShown, "image-shown", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setUseEmbeddedImageProfile, "image-use-embedded", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setEmbeddedImageProfile, "image-embedded-profile" ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setImageFlippedH, "image-flipped-hor", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setImageFlippedV, "image-flipped-vert", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setCmsProfile, "cms-profile" ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem, eRenderIntent>( & PageItem::setCmsRenderingIntent, "cms-intent", &parseIntent ));
	
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setOverrideCompressionMethod, "COMPRESSIONMETHODOVER", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setOverrideCompressionQuality, "COMPRESSIONQUALITYOVER", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setCompressionMethodIndex, "COMPRESSIONMETHOD", &parseInt ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,int>( & PageItem::setCompressionQualityIndex, "COMPRESSIONQUALITY", &parseInt ));

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
	ruleset.addRule(Digester::concat(itemPrefix, "PatternStroke"), PatternStroke());
	ruleset.addRule(Digester::concat(itemPrefix, "PatternMask"), PatternMask());
	
	Gradient gradient;
	Xml_string gradientPrefix(Digester::concat(itemPrefix, "Gradient"));
	ruleset.addRule(gradientPrefix, gradient);
	ruleset.addRule(Digester::concat(gradientPrefix, "CStop"), gradient);
	Gradient gradientS;
	Xml_string gradientPrefixS(Digester::concat(itemPrefix, "GradientS"));
	ruleset.addRule(gradientPrefixS, gradientS);
	ruleset.addRule(Digester::concat(gradientPrefixS, "S_CStop"), gradientS);
	Gradient gradientM;
	Xml_string gradientPrefixM(Digester::concat(itemPrefix, "GradientM"));
	ruleset.addRule(gradientPrefixM, gradientM);
	ruleset.addRule(Digester::concat(gradientPrefixM, "M_CStop"), gradientM);
	Gradient gradientMG;
	Xml_string gradientPrefixMG(Digester::concat(itemPrefix, "Mesh"));
	ruleset.addRule(gradientPrefixMG, gradientMG);
	ruleset.addRule(Digester::concat(gradientPrefixMG, "MPoints"), gradientMG);
		
	ImageEffectsAndLayers effectsAndLayers;
	ruleset.addRule(Digester::concat(itemPrefix, "ImageEffect"), effectsAndLayers);
	ruleset.addRule(Digester::concat(itemPrefix, "PSDLayer"), effectsAndLayers);
	
	Annotation::desaxeRules(itemPrefix, ruleset);
	ruleset.addRule(Digester::concat(itemPrefix, Annotation::saxxDefaultElem), 
					SetterWithConversion<PageItem,const Annotation&,Annotation>( & PageItem::setAnnotation ));

	// TODO: obj attributes
	
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,const QString&>( & PageItem::setPattern, "pattern", dummy ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setExternalFile, "image-file" ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,bool>( & PageItem::setImageInline, "isInlineImage", &parseBool ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setInlineExt, "inlineImageExt" ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setInlineData, "ImageData" ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setFileIconPressed, "icon-pressed-file" ));
	ruleset.addRule(itemPrefix, SetAttribute<PageItem,QString>( & PageItem::setFileIconRollover, "icon-rollover-file" ));
	ruleset.addRule("latex-source", LatexSource());
	ruleset.addRule(itemPrefix, LatexParams());
	LatexParamsExtra laExtra;
	Xml_string latexPrefixMG(Digester::concat(itemPrefix, "latex-source"));
	ruleset.addRule(latexPrefixMG, laExtra);
	ruleset.addRule(Digester::concat(latexPrefixMG, "property"), laExtra);
#ifdef HAVE_OSG
	ruleset.addRule(itemPrefix, OSGParams());
	OSGViews osgViewset;
	ruleset.addRule(Digester::concat(itemPrefix, "OSGViews"), osgViewset);
#endif
	
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,QVector<double> >( & PageItem::setDashes, "line-dashes", &parseDoubleVector ));
	ruleset.addRule(itemPrefix, SetAttributeWithConversion<PageItem,double>( & PageItem::setDashOffset, "line-dash-offset", &parseDouble ));

	StoryText::desaxeRules(itemPrefix, ruleset, "text-content");
	Xml_string storyPrefix = Digester::concat(itemPrefix, "text-content");
	ruleset.addRule(storyPrefix, SetItemText());
	
	ruleset.addRule(itemPrefix, LoadPicture());
	
//	AdjustGroupIds adjustGroupIds;
//	ruleset.addRule("/", adjustGroupIds);
//	ruleset.addRule(PageItem::saxxDefaultElem, adjustGroupIds);
	
}

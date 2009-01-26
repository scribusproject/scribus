/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdvar.h"



#include "scribus.h"
#include "scribusdoc.h"
#include "pageitem.h"
#include <QList>

using boost::python::list;
using boost::python::throw_error_already_set;

PageItem & findItemByName(QList<PageItem*> & items, const QString name)
{
	for (
		QList<PageItem*>::iterator it(items.begin()) ;
		it != items.end() ;
		++it)
	{
		if ( (*it)->itemName() == name)
			return *(*it);
	}
	PyErr_SetString(PyExc_KeyError, "Item not found");
	throw_error_already_set();
}

PageItem & getItem(const QString name)
{
	return findItemByName(ScCore->primaryMainWindow()->doc->DocItems, name);
}

list getItemNames()
{
	list l;
	QList<PageItem*>& items( ScCore->primaryMainWindow()->doc->DocItems );
	for (
		QList<PageItem*>::iterator it(items.begin()) ;
		it != items.end() ;
		++it)
	{
		l.append((*it)->itemName());
	}
	return l;
}

void export_PageItem()
{
	using namespace boost::python;

	def("getItemNames", getItemNames);
	def("getItem", getItem, return_internal_reference<>());

	{
		scope p = class_<PageItem,boost::noncopyable>(
				"PageItem",
				"A Scribus canvas object",
				no_init)
			.def("clearContents", &PageItem::clearContents)
			.def("AdjustPictScale", &PageItem::AdjustPictScale)
			//TODO Needs: ObjAttrVector, Problem: pointer return
			//.def("getObjectAttributes", &PageItem::getObjectAttributes,
			//TODO Needs: ObjectAttribute
			.def("getObjectAttribute", &PageItem::getObjectAttribute)
			.def("setObjectAttributes", &PageItem::setObjectAttributes)
			.def("SetFrameShape", &PageItem::SetFrameShape)
			.def("SetRectFrame", &PageItem::SetRectFrame)
			.def("SetOvalFrame", &PageItem::SetOvalFrame)
			.def("SetFrameRound", &PageItem::SetFrameRound)
			.def("SetPolyClip", &PageItem::SetPolyClip)
			.def("getBoundingRect", &PageItem::getBoundingRect)
			.def("pointWithinItem", &PageItem::pointWithinItem)
			//.def("SetZeichAttr", &PageItem::SetZeichAttr) // WTF?
			.def("SetFarbe", &PageItem::SetFarbe)
			.def_readwrite("AspectRatio", &PageItem::AspectRatio)
			.def_readwrite("AutoName", &PageItem::AutoName)	
			.def_readwrite("BottomLine", &PageItem::BottomLine)
			.def_readwrite("ChangedMasterItem", &PageItem::ChangedMasterItem)
			.def_readwrite("ClipEdited", &PageItem::ClipEdited)
			.def_readwrite("Dirty", &PageItem::Dirty)
			.def_readwrite("fillRule", &PageItem::fillRule)
			.def_readwrite("Frame", &PageItem::Frame)
			.def_readwrite("FrameOnly", &PageItem::FrameOnly)
			.def_readwrite("HasSel", &PageItem::HasSel)
			.def_readwrite("isAutoText", &PageItem::isAutoText)
			.def_readwrite("isBookmark", &PageItem::isBookmark)
			.def_readwrite("isEmbedded", &PageItem::isEmbedded)
			.def_readwrite("isRaster", &PageItem::isRaster)
			.def_readwrite("isSingleSel", &PageItem::isSingleSel)
			.def_readwrite("isTableItem", &PageItem::isTableItem)
			.def_readwrite("LeftLine", &PageItem::LeftLine)
			.def_readwrite("PicArt", &PageItem::PicArt)
			.def_readwrite("PictureIsAvailable", &PageItem::PictureIsAvailable)
			.def_readwrite("PoShow", &PageItem::PoShow)
			.def_readwrite("Redrawn", &PageItem::Redrawn)
			.def_readwrite("RightLine", &PageItem::RightLine)
			.def_readwrite("ScaleType", &PageItem::ScaleType)
			.def_readwrite("Sizing", &PageItem::Sizing)
			.def_readwrite("Tinput", &PageItem::Tinput)
			.def_readwrite("toPixmap", &PageItem::toPixmap)
			.def_readwrite("TopLine", &PageItem::TopLine)
			.def_readwrite("UseEmbedded", &PageItem::UseEmbedded)
			.def_readwrite("BaseOffs", &PageItem::BaseOffs)
			.def_readwrite("BBoxH", &PageItem::BBoxH)
			.def_readwrite("BBoxX", &PageItem::BBoxX)
			.def_readwrite("BoundingH", &PageItem::BoundingH)
			.def_readwrite("BoundingW", &PageItem::BoundingW)
			.def_readwrite("BoundingX", &PageItem::BoundingX)
			.def_readwrite("BoundingY", &PageItem::BoundingY)
			.def_readwrite("ColGap", &PageItem::ColGap)
			.def_readwrite("CurX", &PageItem::CurX)
			.def_readwrite("CurY", &PageItem::CurY)
			.def_readwrite("DashOffset", &PageItem::DashOffset)
			.def_readwrite("gHeight", &PageItem::gHeight)
			.def_readwrite("GrEndX", &PageItem::GrEndX)
			.def_readwrite("GrEndY", &PageItem::GrEndY)
			.def_readwrite("GrStartX", &PageItem::GrStartX)
			.def_readwrite("GrStartY", &PageItem::GrStartY)
			.def_readwrite("gWidth", &PageItem::gWidth)
			.def_readwrite("gXpos", &PageItem::gXpos)
			.def_readwrite("gYpos", &PageItem::gYpos)
			.def_readwrite("LineSp", &PageItem::LineSp)
			.def_readwrite("OldB", &PageItem::OldB)
			.def_readwrite("OldB2", &PageItem::OldB2)
			.def_readwrite("OldH", &PageItem::OldH)
			.def_readwrite("OldH2", &PageItem::OldH2)
			.def_readwrite("OldPwidth", &PageItem::OldPwidth)
			.def_readwrite("Pwidth", &PageItem::Pwidth)
			.def_readwrite("ContourLine", &PageItem::ContourLine)
			.def_readwrite("imageClip", &PageItem::imageClip)
			.def_readwrite("PoLine", &PageItem::PoLine)
			.def_readwrite("BottomLinkID", &PageItem::BottomLinkID)
			.def_readwrite("Cols", &PageItem::Cols)
			.def_readwrite("CPos", &PageItem::CPos)
			.def_readwrite("ExtraV", &PageItem::ExtraV)
			.def_readwrite("FrameType", &PageItem::FrameType)
			.def_readwrite("GrType", &PageItem::GrType)
			.def_readwrite("IRender", &PageItem::IRender)
			.def_readwrite("LayerNr", &PageItem::LayerNr)
			.def_readwrite("LeftLinkID", &PageItem::LeftLinkID)
			.def_readwrite("LineSpMode", &PageItem::LineSpMode)
			.def_readwrite("NextIt", &PageItem::NextIt)
			.def_readwrite("NextPg", &PageItem::NextPg)
			.def_readwrite("oldOwnPage", &PageItem::oldOwnPage)
			.def_readwrite("OrigH", &PageItem::OrigH)
			.def_readwrite("OrigW", &PageItem::OrigW)
			.def_readwrite("OwnPage", &PageItem::OwnPage)
			.def_readwrite("RightLinkID", &PageItem::RightLinkID)
			.def_readwrite("ShTxtFill", &PageItem::ShTxtFill)
			.def_readwrite("ShTxtStroke", &PageItem::ShTxtStroke)
			.def_readwrite("textAlignment", &PageItem::textAlignment)
			.def_readwrite("TopLinkID", &PageItem::TopLinkID)
			.def_readwrite("TxtBase", &PageItem::TxtBase)
			.def_readwrite("TxtOutline", &PageItem::TxtOutline)
			.def_readwrite("TxtScale", &PageItem::TxtScale)
			.def_readwrite("TxtScaleV", &PageItem::TxtScaleV)
			.def_readwrite("TxtShadowX", &PageItem::TxtShadowX)
			.def_readwrite("TxtShadowY", &PageItem::TxtShadowY)
			.def_readwrite("TxtStrikePos", &PageItem::TxtStrikePos)
			.def_readwrite("TxtStrikeWidth", &PageItem::TxtStrikeWidth)
			.def_readwrite("TxTStyle", &PageItem::TxTStyle)
			.def_readwrite("TxtUnderPos", &PageItem::TxtUnderPos)
			.def_readwrite("TxtUnderWidth", &PageItem::TxtUnderWidth)
			.def_readwrite("*BackBox", &PageItem::BackBox)
			.def_readwrite("BottomLink", &PageItem::BottomLink)
			.def_readwrite("LeftLink", &PageItem::LeftLink)
			.def_readwrite("*NextBox", &PageItem::NextBox)
			.def_readwrite("RightLink", &PageItem::RightLink)
			.def_readwrite("TopLink", &PageItem::TopLink)
			.def_readwrite("PLineEnd", &PageItem::PLineEnd)
			.def_readwrite("PLineJoin", &PageItem::PLineJoin)
			.def_readwrite("PLineArt", &PageItem::PLineArt)
			.def_readwrite("Clip", &PageItem::Clip)
			.def_readwrite("itemText", &PageItem::itemText)
			.def_readwrite("EmProfile", &PageItem::EmProfile)
			.def_readwrite("IProfile", &PageItem::IProfile)
			.def_readwrite("Language", &PageItem::Language)
			.def_readwrite("NamedLStyle", &PageItem::NamedLStyle)
			.def_readwrite("OnMasterPage", &PageItem::OnMasterPage)
			.def_readwrite("Pfile", &PageItem::Pfile)
			.def_readwrite("Pfile2", &PageItem::Pfile2)
			.def_readwrite("Pfile3", &PageItem::Pfile3)
			.def_readwrite("TxtFill", &PageItem::TxtFill)
			.def_readwrite("TxtStroke", &PageItem::TxtStroke)
			.def_readwrite("DashValues", &PageItem::DashValues)
			.def_readwrite("effectsInUse", &PageItem::effectsInUse)
			.def_readwrite("TabValues", &PageItem::TabValues)
			.def_readwrite("Segments", &PageItem::Segments)
			.def_readwrite("Groups", &PageItem::Groups)
			.def_readwrite("pixm", &PageItem::pixm)
			.def_readwrite("ItemNr", &PageItem::ItemNr)
			.def_readwrite("MaxChars", &PageItem::MaxChars)
			.def_readwrite("fill_gradient", &PageItem::fill_gradient)
			.add_property("xPos", &PageItem::xPos, &PageItem::setXPos)
			.add_property("yPos", &PageItem::yPos, &PageItem::setYPos)
			.def("moveBy", &PageItem::moveBy)
			.add_property("width", &PageItem::width, &PageItem::setWidth)
			.add_property("height", &PageItem::height, &PageItem::setHeight)
			.def("resizeBy", &PageItem::resizeBy)
			// TODO: Weird error
			//.def("rotation", &PageItem::rotation)
			.def("setRotation", &PageItem::setRotation)
			.def("rotateBy", &PageItem::rotateBy)
			.add_property("selected", &PageItem::isSelected, &PageItem::setSelected)
			.add_property("imageXscale", &PageItem::imageXScale, &PageItem::setImageXScale)
			.add_property("imageYscale", &PageItem::imageYScale, &PageItem::setImageYScale)
			.add_property("imageXOffset", &PageItem::imageXOffset, &PageItem::setImageXOffset)
			.add_property("imageYOffset", &PageItem::imageYOffset, &PageItem::setImageYOffset)
			.def("moveImageXYOffsetBy", &PageItem::moveImageXYOffsetBy)
			.add_property("reversed", &PageItem::reversed, &PageItem::setReversed)
			// TODO: Weird error
			//.def("cornerRadius", &PageItem::cornerRadius)
			.def("setCornerRadius", &PageItem::setCornerRadius)
			.add_property("textToFrameDistLeft", &PageItem::textToFrameDistLeft, &PageItem::setTextToFrameDistLeft)
			.add_property("textToFrameDistRight", &PageItem::textToFrameDistRight, &PageItem::setTextToFrameDistRight)
			.add_property("textToFrameDistTop", &PageItem::textToFrameDistTop, &PageItem::setTextToFrameDistTop)
			.add_property("textToFrameDistBottom", &PageItem::textToFrameDistBottom, &PageItem::setTextToFrameDistBottom)
			.def("setTextToFrameDist", &PageItem::setTextToFrameDist)
			.add_property("itemName", &PageItem::itemName, &PageItem::setItemName)
			.add_property("fillColor", &PageItem::fillColor, &PageItem::setFillColor)
			.add_property("fillShade", &PageItem::fillShade, &PageItem::setFillShade)
			.add_property("fillTransparency", &PageItem::fillTransparency, &PageItem::setFillTransparency)
			.add_property("lineColor", &PageItem::lineColor, &PageItem::setLineColor)
			.add_property("lineShade", &PageItem::lineShade, &PageItem::setLineShade)
			.add_property("lineTransparency", &PageItem::lineTransparency, &PageItem::setLineTransparency)
			.add_property("setLineQColor", &PageItem::setLineQColor, &PageItem::setFillQColor)
			.def("lineStyle", &PageItem::lineStyle)
			// TODO: weird error
			//.def("setLineStyle", &PageItem::setLineStyle)
			// TODO: weird error
			//.def("lineWidth", &PageItem::lineWidth)
			.def("setLineWidth", &PageItem::setLineWidth)
			.add_property("lineEnd", &PageItem::lineEnd, &PageItem::setLineEnd)
			.add_property("lineJoin", &PageItem::lineJoin, &PageItem::setLineJoin)
			.add_property("customLineStyle", &PageItem::customLineStyle, &PageItem::setCustomLineStyle)
			.add_property("startArrowIndex", &PageItem::startArrowIndex, &PageItem::setStartArrowIndex)
			.add_property("endArrowIndex", &PageItem::endArrowIndex, &PageItem::setEndArrowIndex)
			.add_property("imageFlippedH", &PageItem::imageFlippedH, &PageItem::setImageFlippedH)
			.def("flipImageH", &PageItem::flipImageH)
			.add_property("imageFlippedV", &PageItem::imageFlippedV, &PageItem::setImageFlippedV)
			.def("flipImageV", &PageItem::flipImageV)
			.def("setImageScalingMode", &PageItem::setImageScalingMode)
			.def("toggleLock", &PageItem::toggleLock)
			.add_property("locked", &PageItem::locked, &PageItem::setLocked)
			.def("toggleSizeLock", &PageItem::toggleSizeLock)
			.add_property("sizeLocked", &PageItem::sizeLocked, &PageItem::setSizeLocked)
			.add_property("font", &PageItem::font,  &PageItem::setFont)
			.add_property("fontSize", &PageItem::fontSize, &PageItem::setFontSize)
			.add_property("fontHeight", &PageItem::fontHeight, &PageItem::setFontHeight)
			.add_property("fontWidth", &PageItem::fontWidth, &PageItem::setFontWidth)
			.add_property("fontFillColor", &PageItem::fontFillColor, &PageItem::setFontFillColor)
			.add_property("fontFillShade", &PageItem::fontFillShade, &PageItem::setFontFillShade)
			.add_property("fontStrokeColor", &PageItem::fontStrokeColor, &PageItem::setFontStrokeColor)
			.add_property("fontStrokeShade", &PageItem::fontStrokeShade, &PageItem::setFontStrokeShade)
			.add_property("fontEffects", &PageItem::fontEffects, &PageItem::setFontEffects)
			.add_property("kerning", &PageItem::kerning, &PageItem::setKerning)
			// TODO: weird error
			//.def("lineSpacing", &PageItem::lineSpacing)
			.def("setLineSpacing", &PageItem::setLineSpacing)
			.add_property("language", &PageItem::language, &PageItem::setLanguage)
			.add_property("textFlowMode", &PageItem::textFlowMode, &PageItem::setTextFlowMode)
			.def("itemType", &PageItem::itemType)
			.def("convertTo", &PageItem::convertTo)
			.def("setLayer", &PageItem::setLayer)
			.add_property("printable", &PageItem::printable, &PageItem::setPrintable)
			.def("loadImage", &PageItem::loadImage)
			.add_property("isAnnotation", &PageItem::isAnnotation, &PageItem::setIsAnnotation)
			.def("annotation", &PageItem::annotation, return_internal_reference<>());

		enum_<PageItem::ItemType>("ItemType")
			.value("ImageFrame", PageItem::ImageFrame)
			.value("ImageFrame", PageItem::ImageFrame)
			.value("TextFrame", PageItem::TextFrame)
			.value("Line", PageItem::Line)
			.value("Polygon", PageItem::Polygon)
			.value("PolyLine", PageItem::PolyLine)
			.value("PathText", PageItem::PathText)
			.export_values();

		enum_<PageItem::ItemFrameType>("ItemFrameType")
			.value("Unspecified", PageItem::Unspecified)
			.value("Rectangle", PageItem::Rectangle)
			.value("Ellipse", PageItem::Ellipse)
			.value("Round", PageItem::Round)
			.value("Other", PageItem::Other)
			.export_values();

		class_<PageItem::TabRecord>("TabRecord")
			.def_readwrite("tabPosition", &PageItem::TabRecord::tabPosition)
			.def_readwrite("tabType", &PageItem::TabRecord::tabType)
			.add_property("tabFillChar", make_getter(&PageItem::TabRecord::tabFillChar, return_value_policy<return_by_value>()),
										 make_setter(&PageItem::TabRecord::tabFillChar, return_value_policy<return_by_value>()));

	} // end scope p
}



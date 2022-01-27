/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdvar.h"

#include "scribusstructs.h"


using namespace boost::python;

/**
 * @brief Wrap the ParagraphStyle class
 */
void export_ParagraphStyle()
{
	class_<ParagraphStyle>("ParagraphStyle")
//		.def_readwrite("Vname", &ParagraphStyle::Vname)
		.add_property("Vname", make_getter(&ParagraphStyle::Vname, return_value_policy<return_by_value>()),
		                       make_setter(&ParagraphStyle::Vname, return_value_policy<return_by_value>()))
		.def_readwrite("LineSpaMode", &ParagraphStyle::LineSpaMode)
		.def_readwrite("LineSpa", &ParagraphStyle::LineSpa)
		.def_readwrite("textAlignment", &ParagraphStyle::textAlignment)
		.def_readwrite("Indent", &ParagraphStyle::Indent)
		.def_readwrite("First", &ParagraphStyle::First)
		.def_readwrite("gapBefore", &ParagraphStyle::gapBefore)
		.def_readwrite("gapAfter", &ParagraphStyle::gapAfter)
//		.def_readwrite("Font", &ParagraphStyle::Font)
		.add_property("Font", make_getter(&ParagraphStyle::Font, return_value_policy<return_by_value>()),
		                      make_setter(&ParagraphStyle::Font, return_value_policy<return_by_value>()))
		.def_readwrite("FontSize", &ParagraphStyle::FontSize)
		.def_readwrite("TabValues", &ParagraphStyle::TabValues)
		.def_readwrite("Drop", &ParagraphStyle::Drop)
		.def_readwrite("DropLin", &ParagraphStyle::DropLin)
		.def_readwrite("DropDist", &ParagraphStyle::DropDist)
		.def_readwrite("FontEffect", &ParagraphStyle::FontEffect)
//		.def_readwrite("FColor", &ParagraphStyle::FColor)
		.add_property("FColor", make_getter(&ParagraphStyle::FColor, return_value_policy<return_by_value>()),
		                        make_setter(&ParagraphStyle::FColor, return_value_policy<return_by_value>()))
		.def_readwrite("FShade", &ParagraphStyle::FShade)
//		.def_readwrite("SColor", &ParagraphStyle::SColor)
		.add_property("SColor", make_getter(&ParagraphStyle::SColor, return_value_policy<return_by_value>()),
		                        make_setter(&ParagraphStyle::SColor, return_value_policy<return_by_value>()))
		.def_readwrite("SShade", &ParagraphStyle::SShade)
		.def_readwrite("BaseAdj", &ParagraphStyle::BaseAdj)
		.def_readwrite("txtShadowX", &ParagraphStyle::txtShadowX)
		.def_readwrite("txtShadowY", &ParagraphStyle::txtShadowY)
		.def_readwrite("txtOutline", &ParagraphStyle::txtOutline)
		.def_readwrite("txtUnderPos", &ParagraphStyle::txtUnderPos)
		.def_readwrite("txtUnderWidth", &ParagraphStyle::txtUnderWidth)
		.def_readwrite("txtStrikePos", &ParagraphStyle::txtStrikePos)
		.def_readwrite("txtStrikeWidth", &ParagraphStyle::txtStrikeWidth)
		.def_readwrite("scaleH", &ParagraphStyle::scaleH)
		.def_readwrite("scaleV", &ParagraphStyle::scaleV)
		.def_readwrite("baseOff", &ParagraphStyle::baseOff)
		.def_readwrite("kernVal", &ParagraphStyle::kernVal)
		;
}

void export_ScribusStructs()
{
	export_ParagraphStyle();
}



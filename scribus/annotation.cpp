/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "annotation.h"
#include "util.h"
#include "util_text.h"
#include "util_math.h"

void Annotation::saxx(SaxHandler& handler, const Xml_string& elemtag) const
{
	Xml_attr att;
	att.insert("ANTYPE", QString::number(Type()));
	att.insert("ANACTION", Action());
	att.insert("ANEACT", E_act());
	att.insert("ANXACT", X_act());
	att.insert("ANDACT", D_act());
	att.insert("ANFOACT", Fo_act());
	att.insert("ANBLACT", Bl_act());
	att.insert("ANKACT", K_act());
	att.insert("ANFACT", F_act());
	att.insert("ANVACT", V_act());
	att.insert("ANCACT", C_act());
	if (ActionType() == 8)
		att.insert("ANEXTERN", Extern());
	else
		att.insert("ANEXTERN", Path2Relative(Extern()));
	att.insert("ANZIEL", QString::number(Ziel()));
	att.insert("ANACTYP", QString::number(ActionType()));
	att.insert("ANTOOLTIP", ToolTip());
	att.insert("ANBWID", QString::number(Bwid()));
	att.insert("ANBSTY", QString::number(Bsty()));
	att.insert("ANFEED", QString::number(Feed()));
	att.insert("ANFLAG", QString::number(Flag()));
	att.insert("ANFONT", QString::number(Font()));
	att.insert("ANFORMAT", QString::number(Format()));
	att.insert("ANROLL", RollOver());
	att.insert("ANDOWN", Down());
	att.insert("ANVIS", QString::number(Vis()));
	att.insert("ANMC", QString::number(MaxChar()));
	att.insert("ANCHK", IsChk() ? "yes" : "no");
	att.insert("ANAA", AAact() ? "yes" : "no");
	att.insert("ANCHKS", QString::number(ChkStil()));
	att.insert("ANBCOL", borderColor());
	att.insert("ANHTML", HTML() ? "yes" : "no");
	att.insert("ANICON", UseIcons() ? "yes" : "no");
	att.insert("ANPLACE", QString::number(IPlace()));
	att.insert("ANSCALE", QString::number(ScaleW()));
	
	handler.beginEnd(elemtag, att);
}

const Xml_string Annotation::saxxDefaultElem("pdfannotation");


class ParseAnnotation_body : public Action_body
{
public:
	void begin(const Xml_string& tagname, Xml_attr attr)
	{
		Annotation* ann = this->dig->top<Annotation>();
		ann->setType(parseInt(attr["ANTYPE"]));
		ann->setAction(attr["ANACTION"]);
		ann->setE_act(attr["ANEACT"]);
		ann->setX_act(attr["ANXACT"]);
		ann->setD_act(attr["ANDACT"]);
		ann->setFo_act(attr["ANFOACT"]);
		ann->setBl_act(attr["ANBLACT"]);
		ann->setK_act(attr["ANKACT"]);
		ann->setF_act(attr["ANFACT"]);
		ann->setV_act(attr["ANVACT"]);
		ann->setC_act(attr["ANCACT"]);
		ann->setExtern(attr["ANEXTERN"]);
		ann->setZiel(parseInt(attr["ANZIEL"]));
		ann->setActionType(parseInt(attr["ANACTYP"]));
		ann->setToolTip(attr["ANTOOLTIP"]);
		ann->setBwid(parseInt(attr["ANBWID"]));
		ann->setBsty(parseInt(attr["ANBSTY"]));
		ann->setFeed(parseInt(attr["ANFEED"]));
		ann->setFlag(parseInt(attr["ANFLAG"]));
		ann->setFont(parseInt(attr["ANFONT"]));
		ann->setFormat(parseInt(attr["ANFORMAT"]));
		ann->setRollOver(attr["ANROLL"]);
		ann->setDown(attr["ANDOWN"]);
		ann->setVis(parseInt(attr["ANVIS"]));
		ann->setMaxChar(parseInt(attr["ANMC"]));
		ann->setIsChk(parseBool(attr["ANCHK"]));
		ann->setAAact(parseBool(attr["ANAA"]));
		ann->setChkStil(parseInt(attr["ANCHKS"]));
		ann->setBorderColor(attr["ANBCOL"]);
		ann->setHTML(parseBool(attr["ANHTML"]));
		ann->setUseIcons(parseBool(attr["ANICON"]));
		ann->setIPlace(parseInt(attr["ANPLACE"]));
		ann->setScaleW(parseInt(attr["ANSCALE"]));
	}
};

class ParseAnnotation : public MakeAction<ParseAnnotation_body>
{};


void Annotation::desaxeRules(const Xml_string& prefixPattern, desaxe::Digester& ruleset, Xml_string elemtag)
{
	Xml_string annPrefix(Digester::concat(prefixPattern, elemtag));
	ruleset.addRule(annPrefix, Factory<Annotation>());
	ruleset.addRule(annPrefix, ParseAnnotation());
}

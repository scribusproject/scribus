


#include "annotation.h"
#include "util.h"

void Annotation::saxx(SaxHandler& handler, Xml_string elemtag) const
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

void Annotation::desaxeRules(Xml_string prefixPattern, desaxe::Digester& ruleset, Xml_string elemtag)
{}

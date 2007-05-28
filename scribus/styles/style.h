

#ifndef STYLES_STYLE
#define STYLES_STYLE

#include "../style.h"
#include "desaxe/digester.h"
#include "desaxe/simple_actions.h"
#include "desaxe/saxiohelper.h"


using namespace desaxe;

template<class SUBSTYLE>
inline
void  Style::desaxeRules(const Xml_string& prefixPattern, desaxe::Digester& ruleset, Xml_string elemtag)
{
	Xml_string stylePrefix(Digester::concat(prefixPattern,elemtag));
	ruleset.addRule(stylePrefix, SetAttributeWithConversion<SUBSTYLE, const QString&>( & SUBSTYLE::setName, "name", &parse<const QString&>, QString::null));
	ruleset.addRule(stylePrefix, SetAttributeWithConversion<SUBSTYLE, const QString&>( & SUBSTYLE::setParent, "parent", &parse<const QString&>, QString::null));
	ruleset.addRule(stylePrefix, SetAttributeWithConversion<SUBSTYLE, const QString&>( & SUBSTYLE::setShortcut, "shortcut", &parse<const QString&>, QString::null));
}

#endif

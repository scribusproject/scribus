/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <qvaluelist.h>
#include <qobject.h>
#include "sctextstruct.h"
#include "scfonts.h"


QString CharStyle::asString() const
{
	QString result;
	if (cfont != &Foi::NONE)
		result += QObject::tr("font %1 ").arg(cfont->scName());
	if (csize != NOVALUE)
		result += QObject::tr("size %1 ").arg(csize);
	if (cstyle != NOVALUE)
		result += QObject::tr("+style ");
	if (ccolor != NOCOLOR || cstroke != NOCOLOR || cshade != NOVALUE || cshade2 != NOVALUE)
		result += QObject::tr("+color ");
	if (cunderwidth != NOVALUE || cunderpos != NOVALUE)
		result += cunderwidth > 0 ? QObject::tr("+underline ") : QObject::tr("-underline ");
	if (cstrikewidth != NOVALUE || cstrikepos != NOVALUE)
		result += cstrikewidth > 0 ? QObject::tr("+strikeout ") : QObject::tr("-strikeout ");
	if (cshadowx != NOVALUE || cshadowy != NOVALUE)
		result += cshadowx != 0 && cshadowy != 0? QObject::tr("+shadow ") : QObject::tr("-shadow ");
	if (coutline != NOVALUE)
		result += coutline > 0 ? QObject::tr("+outline ") : QObject::tr("-outline ");
	if (cextra != NOVALUE)
		result += cextra > 0 ? QObject::tr("+tracking %1").arg(cextra) : QObject::tr("-tracking ");
	if (cbase != NOVALUE)
		result += QObject::tr("+baseline %1").arg(cbase);
	if (cscale != NOVALUE || cscalev != NOVALUE)
		result += QObject::tr("+stretch ");
	if (parent() != NULL)
		result += QObject::tr("parent= %1").arg(parent()->name()=="" ? QObject::tr("unnamed") : parent()->name());
	return result.stripWhiteSpace();
}


ScText::~ScText() 
{
	// delete the linked list if present
	GlyphLayout * more = glyph.more;
	while (more) {
		glyph.more = glyph.more->more;
		delete more;
		more = glyph.more;
	}
	if (parstyle)
		delete parstyle;
	//if (cembedded) delete cembedded
}

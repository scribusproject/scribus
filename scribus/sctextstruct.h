/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCTEXTSTRUCT_H
#define SCTEXTSTRUCT_H

#ifdef HAVE_CONFIG_H
#include "scconfig.h"
#endif

#include <qstring.h>

class Foi;
class PageItem;

/* Struktur fuer Pageitem Text */
struct ScText
{
	bool cselect;
	int csize;
	short cshade;
	short cshade2;
	short cstyle;
/**	Meaning of the values in cstyle
	1			= Superscript
	2 			= Subscript
	4 			= Outline
	8 			= Underline
	16 		= Strikethru
	32 		= All Caps
	64 		= Small Caps
	128 		= Hyphenation possible here (Smart Hyphen)
	256 	= Shadowed
	512 	= Underline Words
	1024 	= free, not used in the moment
	2048 	= Char is a DropCap
	4096 	= internal use in PageItem (Suppresses spaces when in Block alignment)
	8192 	= Smart Hyphen visible at line end
	16384	= Start of Line
*/
	short cab;
	short cscale;
	short cscalev;
	short cbase;
	short cshadowx;
	short cshadowy;
	short coutline;
	short cunderpos;
	short cunderwidth;
	short cstrikepos;
	short cstrikewidth;
	short cextra;
	float xp;
	float yp;
	float PtransX;
	float PtransY;
	float PRot;
	Foi* cfont;
	PageItem* cembedded;
	QString ccolor;
	QString cstroke;
	QString ch;
 /** Special Characters used:
 	QChar(24) =	Non breaking Hyphen
 	QChar(25) =	Marks an inline Object
 	QChar(26) =	Column Break
 	QChar(27) =	Frame Break
 	QChar(28) =	New Line, doesn't break Paragraph
 	QChar(29) =	Non breaking Space
 	QChar(30) =	Automatic Pagenumbering  */
};

#endif // SCTEXTSTRUCT_H


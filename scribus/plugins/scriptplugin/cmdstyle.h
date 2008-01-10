/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
02.01.2008: Joachim Neu - joachim_neu@web.de - http://www.joachim-neu.de
*/
#ifndef CMDSTYLE_H
#define CMDSTYLE_H

// Pulls in <Python.h> first
#include "cmdvar.h"
//Added by qt3to4:
#include <QPixmap>

/** Style-related Commands */

/* PARAGRAPH STYLES */

/*! docstring */
PyDoc_STRVAR(scribus_createparagraphstyle__doc__,
QT_TR_NOOP("createParagraphStyle(...)\n\n\
Creates a paragraph style. This function takes the following keyword parameters:\n\n\
\"name\" [required] -> specifies the name of the paragraphstyle to create\n\n\
linespacingmode [optional] -> specifies the linespacing mode; possible modes are:\n\n\
fixed linespacing:          0\n\n\
automatic linespacing:      1\n\n\
baseline grid linespacing:  2\n\n\
linespacing [optional] -> specifies the linespacing if using fixed linespacing\n\n\
alignment [optional] -> specifies the alignment of the paragraph\n\n\
-> left:     0\n\n\
-> center:   1\n\n\
-> right:    2\n\n\
-> justify:  3\n\n\
-> extend:   4\n\n\
leftmargin [optional], rightmargin [optional] -> specify the margin\n\n\
gapbefore [optional], gapafter [optional] -> specify the gaps to the heading and following paragraphs\n\n\
firstindent [optional] -> the indent of the first line\n\n\
hasdropcap [optional] -> specifies if there are caps (1 = yes, 0 = no)\n\n\
dropcaplines [optional] -> height (in lines) of the caps if used\n\n\
dropcapoffset [optional] -> offset of the caps if used\n\n\
\"charstyle\" [optional] -> char style to use\n\n\
"));
/*! 02.01.2007 - 05.01.2007 : Joachim Neu : Create a paragraph style.
  		Special thanks go to avox for helping me! */
PyObject *scribus_createparagraphstyle(PyObject * /* self */, PyObject* args, PyObject* keywords);

/* CHAR STYLES */

/*! docstring */
PyDoc_STRVAR(scribus_createcharstyle__doc__,
QT_TR_NOOP("createCharStyle(...)\n\n\
Creates a character style. This function takes the following keyword parameters:\n\n\
\"name\" [required] -> name of the char style to create\n\n\
\"font\" [optional] -> name of the font to use\n\n\
fontsize [optional] -> font size to set (double)\n\n\
\"features\" [optional] -> nearer typographic details can be defined by a string that might contain the following phrases comma-seperated (without spaces!):\n\n\
-> inherit\n\n\
-> bold\n\n\
-> italic\n\n\
-> underline\n\n\
-> underlinewords\n\n\
-> strike\n\n\
-> superscript\n\n\
-> subscript\n\n\
-> outline\n\n\
-> shadowed\n\n\
-> allcaps\n\n\
-> smallcaps\n\n\
\"fillcolor\" [optional], \"fillshade\" [optional] -> specify fill options\n\n\
\"strokecolor\" [optional], \"strokeshade\" [optional] -> specify stroke options\n\n\
baselineoffset [optional] -> offset of the baseline\n\n\
shadowxoffset [optional], shadowyoffset [optional] -> offset of the shadow if used\n\n\
outlinewidth [optional] -> width of the outline if used\n\n\
underlineoffset [optional], underlinewidth [optional] -> underline options if used\n\n\
strikethruoffset [optional], strikethruwidth [optional] -> strikethru options if used\n\n\
scaleh [optional], scalev [optional] -> scale of the chars\n\n\
tracking [optional] -> tracking of the text\n\n\
\"language\" [optional] -> language code\n\n\
"));
/*! 03.01.2007 - 05.01.2007 : Joachim Neu : Create a char style. 
  		Special thanks go to avox for helping me! */
PyObject *scribus_createcharstyle(PyObject * /* self */, PyObject* args, PyObject* keywords);

#endif


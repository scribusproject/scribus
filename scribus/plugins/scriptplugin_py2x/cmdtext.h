/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDTEXT_H
#define CMDTEXT_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/*! Text frame handling */

/*! docstring */
PyDoc_STRVAR(scribus_getfontsize__doc__,
QT_TR_NOOP("getFontSize([\"name\"]) -> float\n\
\n\
Returns the font size in points for the text frame \"name\". If this text\n\
frame has some text selected the value assigned to the first character of\n\
the selection is returned.\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Get font size */
PyObject *scribus_getfontsize(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getfont__doc__,
QT_TR_NOOP("getFont([\"name\"]) -> string\n\
\n\
Returns the font name for the text frame \"name\". If this text frame\n\
has some text selected the value assigned to the first character\n\
of the selection is returned. If \"name\" is not given the currently\n\
selected item is used.\n\
"));
/*! Get font */
PyObject *scribus_getfont(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getfontfeatures__doc__,
QT_TR_NOOP("getFontFeatures([\"name\"]) -> string\n\
\n\
Returns the font features for the text frame \"name\". If this text frame\n\
has some text selected the value assigned to the first character\n\
of the selection is returned. If \"name\" is not given the currently\n\
selected item is used.\n\
"));
/*! Get fontfeatures */
PyObject *scribus_getfontfeatures(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettextcolor__doc__,
	QT_TR_NOOP("getTextColor([\"name\"]) -> string\n\
\n\
Returns the name of the text color used for text frame \"name\".\n\
If this text frame has some text selected the value assigned\n\
to the first character of the selection is returned.\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Returns color of the line */
PyObject *scribus_gettextcolor(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettextshade__doc__,
	QT_TR_NOOP("getTextShade([\"name\"]) -> integer\n\
\n\
Returns the shade of text color used for text frame \"name\".\n\
If this text frame has some text selected the value assigned\n\
to the first character of the selection is returned.\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Returns color of the line */
PyObject *scribus_gettextshade(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettextlength__doc__,
QT_TR_NOOP("getTextLength([\"name\"]) -> integer\n\
\n\
Returns the length of the text in the text frame \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Get text size */
PyObject *scribus_gettextlength(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettextlines__doc__,
QT_TR_NOOP("getTextLines([\"name\"]) -> integer\n\
\n\
Returns the number of lines of the text in the text frame \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
As this function depends on text layout being up-to-date, you may need to call \n\
layoutText() or layoutTextChain() before calling this function in order to get \n\
expected result.\n\
"));
/*! Get text lines */
PyObject *scribus_gettextlines(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getframetext__doc__,
QT_TR_NOOP("getFrameText([\"name\"]) -> string\n\
\n\
Returns the text visible in text frame \"name\". If this text frame has some text\n\
selected, the selected text is returned. If \"name\" is not given the currently\n\
selected item is used.\n\
\n\
This function returns only the text visible in specified frame. If you need to \n\
retrieve the text contained in a text chain, use getAllText() instead.\n\
\n\
As this function depends on text layout being up-to-date, you may need to call \n\
layoutText() or layoutTextChain() before calling this function in order to get \n\
expected result.\n\
"));
PyDoc_STRVAR(scribus_gettext__doc__,
QT_TR_NOOP("getText([\"name\"]) -> string\n\
\n\
Deprecated. Use getFrameText() instead.\n\
"));
/*! Get frame text */
PyObject *scribus_getframetext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getalltext__doc__,
QT_TR_NOOP("getAllText([\"name\"]) -> string\n\
\n\
Returns the text of the text frame \"name\" and of all text frames which are\n\
linked with this frame. If this textframe has some text selected, the selected\n\
text is returned. If \"name\" is not given the currently selected item is\n\
used.\n\
"));
/*! Get all text */
PyObject *scribus_getalltext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinespace__doc__,
QT_TR_NOOP("getLineSpacing([\"name\"]) -> float\n\
\n\
Returns the line spacing (\"leading\") of the text frame \"name\" expressed in\n\
points. If \"name\" is not given the currently selected item is used.\n\
"));
/*! Get line space */
PyObject *scribus_getlinespace(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettextdistances__doc__,
QT_TR_NOOP("getTextDistances([\"name\"]) -> tuple\n\
\n\
Returns the text distances of the text frame \"name\" expressed in points. The\n\
distances are returned as a tuple like (left, right, top, bottom). If \"name\"\n\
is not given the currently selected item is used.\n\
"));
/*! Get text distances */
PyObject *scribus_gettextdistances(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettextverticalalignment__doc__,
	QT_TR_NOOP("getTextVerticalAlignment([\"name\"]) -> integer\n\
\n\
Gets the vertical alignment of text inside text frame \"name\". If \"name\" is not\n\
given the currently selected item is used.\n\
"));
/*! Get columns */
PyObject *scribus_gettextverticalalignment(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcolumngap__doc__,
QT_TR_NOOP("getColumnGap([\"name\"]) -> float\n\
\n\
Returns the column gap size of the text frame \"name\" expressed in points. If\n\
\"name\" is not given the currently selected item is used.\n\
"));
/*! Get column gap */
PyObject *scribus_getcolumngap(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcolumns__doc__,
QT_TR_NOOP("getColumns([\"name\"]) -> integer\n\
\n\
Gets the number of columns of the text frame \"name\". If \"name\" is not\n\
given the currently selected item is used.\n\
"));
/*! Get columns */
PyObject *scribus_getcolumns(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settext__doc__,
QT_TR_NOOP("setText(\"text\", [\"name\"])\n\
\n\
Sets the text of the text frame \"name\" to the text of the string \"text\".\n\
Text must be UTF8 encoded - use e.g. unicode(text, 'iso-8859-2'). See the FAQ\n\
for more details. If \"name\" is not given the currently selected item is\n\
used.\n\
"));
/*! Set text */
PyObject *scribus_settext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_inserttext__doc__,
QT_TR_NOOP("insertText(\"text\", pos, [\"name\"])\n\
\n\
Inserts the text \"text\" at the position \"pos\" into the text frame \"name\".\n\
Text must be UTF encoded (see setText() as reference) The first character has an\n\
index of 0. Inserting text at position -1 appends it to the frame. If \"name\" is\n\
not given the currently selected item is used.\n\
\n\
For performance reason, this function does not update text layout in any way. \n\
As a consequence, you may need to call layoutText() or layoutTextChain() at appropriate \n\
times after calling this function and before calling functions such as getFrameText() \n\
or getTextLines().\n\
\n\
May throw IndexError for an insertion out of bounds.\n\
"));
/*! Insert text */
PyObject *scribus_inserttext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_inserthtmltext__doc__,
QT_TR_NOOP("insertHTMLText(\"file\", [\"name\"])\n\
\n\
Inserts the text from \"file\" into the text frame \"name\".\n\
Text must be UTF encoded (see setText() as reference). If \"name\" is\n\
not given the currently selected Item is used.\n\
"));
/*! Insert HTML text */
PyObject *scribus_inserthtmltext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_layouttext__doc__,
QT_TR_NOOP("layoutText([\"name\"])\n\
\n\
Relayout text in the text frame \"name\". If \"name\" is not given the currently\n\
selected item is used.\n\
"));
/*! Delete text */
PyObject *scribus_layouttext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_layouttextchain__doc__,
QT_TR_NOOP("layoutTextChain([\"name\"])\n\
\n\
Relayout the whole text chain whom the text frame \"name\" belongs.\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Delete text */
PyObject *scribus_layouttextchain(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfont__doc__,
QT_TR_NOOP("setFont(\"font\", [\"name\"])\n\
\n\
Sets the font of the text frame \"name\" to \"font\". If there is some text\n\
selected only the selected text is changed.  If \"name\" is not given the\n\
currently selected item is used.\n\
\n\
May throw ValueError if the font cannot be found.\n\
"));
/*! Set font */
PyObject *scribus_setfont(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfontfeatures__doc__,
QT_TR_NOOP("setFontFeatures(\"fontfeature\", [\"name\"])\n\
\n\
Sets the font features of the text frame \"name\" to \"fontfeature\". If there is some text\n\
selected only the selected text is changed.  If \"name\" is not given the\n\
currently selected item is used.\n\
\n\
May throw ValueError if the font cannot be found.\n\
"));
/*! Set font features */
PyObject *scribus_setfontfeatures(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfontsize__doc__,
QT_TR_NOOP("setFontSize(size, [\"name\"])\n\
\n\
Sets the font size of the text frame \"name\" to \"size\". \"size\" is treated\n\
as a value in points. If there is some text selected only the selected text is\n\
changed. \"size\" must be in the range 1 to 512. If \"name\" is not given the\n\
currently selected item is used.\n\
\n\
May throw ValueError for a font size that's out of bounds.\n\
"));
/*! Set font size */
PyObject *scribus_setfontsize(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinespace__doc__,
QT_TR_NOOP("setLineSpacing(size, [\"name\"])\n\
\n\
Sets the line spacing (\"leading\") of the text frame \"name\" to \"size\".\n\
\"size\" is a value in points. If \"name\" is not given the currently selected\n\
item is used.\n\
\n\
May throw ValueError if the line spacing is out of bounds.\n\
"));
/*! Set line space */
PyObject *scribus_setlinespace(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinespacemode__doc__,
QT_TR_NOOP("setLineSpacingMode(mode, [\"name\"])\n\
\n\
Sets the line spacing mode of the text frame \"name\" to \"mode\".\n\
If \"name\" is not given the currently selected\n\
item is used.\n\
Mode values are the same as in createParagraphStyle.\n\
\n\
May throw ValueError if the mode is out of bounds.\n\
"));
/*! Set line space mode */
PyObject *scribus_setlinespacemode(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextdistances__doc__,
QT_TR_NOOP("setTextDistances(left, right, top, bottom, [\"name\"])\n\
\n\
Sets the text distances of the text frame \"name\" to the values \"left\"\n\
\"right\", \"top\" and \"bottom\". If \"name\" is not given the currently\n\
selected item is used.\n\
\n\
May throw ValueError if any of the distances are out of bounds (must be positive).\n\
"));
/*! Set text distances */
PyObject *scribus_settextdistances(PyObject */*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolumngap__doc__,
QT_TR_NOOP("setColumnGap(size, [\"name\"])\n\
\n\
Sets the column gap of the text frame \"name\" to the value \"size\". If\n\
\"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError if the column gap is out of bounds (must be positive).\n\
"));
/*! Set column gap */
PyObject *scribus_setcolumngap(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolumns__doc__,
QT_TR_NOOP("setColumns(nr, [\"name\"])\n\
\n\
Sets the number of columns of the text frame \"name\" to the integer \"nr\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May throw ValueError if number of columns is not at least one.\n\
"));
/*! Set columns */
PyObject *scribus_setcolumns(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setalign__doc__,
QT_TR_NOOP("setTextAlignment(align, [\"name\"])\n\
\n\
Sets the text alignment of the text frame \"name\" to the specified alignment.\n\
If \"name\" is not given the currently selected item is used. \"align\" should\n\
be one of the ALIGN_ constants defined in this module - see dir(scribus).\n\
\n\
May throw ValueError for an invalid alignment constant.\n\
"));
/*! Set alignt */
PyObject *scribus_setalignment(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextverticalalignment__doc__,
	QT_TR_NOOP("setTextVerticalAlignment(align, [\"name\"])\n\
\n\
Sets the vertical alignment of text inside text frame \"name\" to the specified\n\
alignment. If \"name\" is not given the currently selected item is used. \"align\" \n\
should be one of the ALIGNV constants defined in this module - see dir(scribus).\n\
\n\
May throw ValueError for an invalid alignment constant.\n\
"));
/*! Set vertical alignment */
PyObject *scribus_settextverticalalignment(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setdirection__doc__,
QT_TR_NOOP("setTextDirection(direction, [\"name\"])\n\
\n\
Sets the text direction of the text frame \"name\" to the specified direction.\n\
If \"name\" is not given the currently selected item is used. \"direction\" should\n\
be one of the DIRECTION_ constants defined in this module - see dir(scribus).\n\
\n\
May throw ValueError for an invalid direction constant.\n\
"));
/*! Set direction */
PyObject *scribus_setdirection(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_selectframetext__doc__,
QT_TR_NOOP("selectFrameText(start, count, [\"name\"])\n\
\n\
Selects \"count\" characters of text in the text frame \"name\" starting from the\n\
character \"start\". Character counting starts at 0.\n\
If \"count\" is zero, any text selection will be cleared.\n\
If \"count\" is -1, the selection will extend to the end of the frame.\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
This function only acts on the text visible in the specified frame. If you need to \n\
work on the text contained in a text chain, use selectText() instead.\n\
As this function depends on text layout being up-to-date, you may need to call \n\
layoutText() or layoutTextChain() before calling this function in order to get \n\
expected result.\n\
\n\
May throw IndexError if the selection is outside the bounds of the text.\n\
"));
/*! Select frame text */
PyObject *scribus_selectframetext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_selecttext__doc__,
QT_TR_NOOP("selectText(start, count, [\"name\"])\n\
\n\
Selects \"count\" characters of text in the text frame \"name\" starting from the\n\
character \"start\". Character counting starts at 0. If \"count\" is zero, any\n\
text selection will be cleared.  If \"name\" is not given the currently\n\
selected item is used.\n\
\n\
May throw IndexError if the selection is outside the bounds of the text.\n\
"));
/*! Select text */
PyObject *scribus_selecttext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deletetext__doc__,
QT_TR_NOOP("deleteText([\"name\"])\n\
\n\
Deletes any text in the text frame \"name\". If there is some text selected,\n\
only the selected text will be deleted. If \"name\" is not given the currently\n\
selected item is used.\n\
"));
/*! Delete text */
PyObject *scribus_deletetext(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextfill__doc__,
QT_TR_NOOP("setTextColor(\"color\", [\"name\"])\n\
\n\
Sets the text color of the text frame \"name\" to the color \"color\". If there\n\
is some text selected only the selected text is changed. If \"name\" is not\n\
given the currently selected item is used.\n\
"));
/*! Set text fill */
PyObject *scribus_settextfill(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextstroke__doc__,
QT_TR_NOOP("setTextStroke(\"color\", [\"name\"])\n\
\n\
Set \"color\" of the text stroke. If \"name\" is not given the currently\n\
selected item is used.\n\
"));
/*! Set text stroke */
PyObject *scribus_settextstroke(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextscalingv__doc__,
QT_TR_NOOP("setTextScalingV(scale, [\"name\"])\n\
\n\
Sets the vertical character scaling of the object \"name\" to \"scale\" in percent.\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Set text shde */
PyObject *scribus_settextscalingv(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextscalingh__doc__,
QT_TR_NOOP("setTextScalingH(scale, [\"name\"])\n\
\n\
Sets the horizontal character scaling of the object \"name\" to \"scale\" in percent.\n\
If \"name\" is not given the currently selected item is used.\n\
"));
/*! Set text shde */
PyObject *scribus_settextscalingh(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextshade__doc__,
QT_TR_NOOP("setTextShade(shade, [\"name\"])\n\
\n\
Sets the shading of the text color of the object \"name\" to \"shade\". If\n\
there is some text selected only the selected text is changed. \"shade\" must\n\
be an integer value in the range from 0 (lightest) to 100 (full color\n\
intensity). If \"name\" is not given the currently selected item is\n\
used.\n\
"));
/*! Set text shde */
PyObject *scribus_settextshade(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_linktextframes__doc__,
QT_TR_NOOP("linkTextFrames(\"fromname\", \"toname\")\n\
\n\
Link two text frames. The frame named \"fromname\" is linked to the\n\
frame named \"toname\". The target frame must be an empty text frame\n\
and must not link to or be linked from any other frames already.\n\
\n\
May throw ScribusException if linking rules are violated.\n\
"));
/**
 Link text frames via Scripter.
 02/22/04 (petr vanek)
 TODO: PDF bookmarks handling
 */
PyObject *scribus_linktextframes(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_unlinktextframes__doc__,
QT_TR_NOOP("unlinkTextFrames(\"name\")\n\
\n\
Remove the specified (named) object from the text frame flow/linkage. If the\n\
frame was in the middle of a chain, the previous and next frames will be\n\
connected, eg 'a->b->c' becomes 'a->c' when you unlinkTextFrames(b)'\n\
\n\
May throw ScribusException if linking rules are violated.\n\
"));
/**
 (Un)Link text frames via Scripter.
 02/22/04 (petr vanek)
 TODO: PDF bookmarks handling
 */
PyObject *scribus_unlinktextframes(PyObject * self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_outlinetext__doc__,
QT_TR_NOOP("traceText([\"name\"])\n\
\n\
Convert the text frame \"name\" to outlines. If \"name\" is not given the\n\
currently selected item is used."));
PyDoc_STRVAR(scribus_tracetext__doc__,
QT_TR_NOOP("traceText([\"name\"])\n\
\n\
Deprecated, use outlineText() instead."));
/**
 Trace text frames via Scripter.
 2004-09-07 (Craig Ringer)
*/
PyObject *scribus_outlinetext(PyObject * self, PyObject* args);

PyDoc_STRVAR(scribus_istextoverflowing__doc__,
QT_TR_NOOP("textOverflows([\"name\", nolinks]) -> integer\n\
\n\
Returns 1 if there are overflowing characters in text frame \"name\", 0 if not.\n\
If is nolinks set to non zero value it takes only one frame - it doesn't\n\
use text frame linking. Without this parameter it search all linking chain.\n\
\n\
May raise WrongFrameTypeError if the target frame is not an text frame\n"));
/** Check if is the text frame overflowind or not
 2005-02-04 petr vanek
 */
PyObject *scribus_istextoverflowing(PyObject * self, PyObject* args, PyObject* kw);

PyDoc_STRVAR(scribus_hyphenatetext__doc__,
QT_TR_NOOP("hyphenateText([\"name\"]) -> bool\n\
\n\
Does hyphenation on text frame \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May raise WrongFrameTypeError if the target frame is not a text frame\n\
"));
/*
	Does hyphenation on the given text frame
	08.12.2007: Joachim Neu
*/
PyObject *scribus_hyphenatetext(PyObject *, PyObject* args);

PyDoc_STRVAR(scribus_dehyphenatetext__doc__,
QT_TR_NOOP("dehyphenateText([\"name\"]) -> bool\n\
\n\
Does dehyphenation on text frame \"name\".\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May raise WrongFrameTypeError if the target frame is not a text frame\n\
"));
/*
	Does dehyphenation on the given text frame
	13.12.2007: Joachim Neu
*/
PyObject *scribus_dehyphenatetext(PyObject *, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setpdfbookmark__doc__,
QT_TR_NOOP("setPDFBookmark(\"toggle\", [\"name\"])\n\
\n\
Sets whether (toggle = 1) the text frame \"name\" is a bookmark nor not.\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May raise WrongFrameTypeError if the target frame is not a text frame\n\
"));
/*! Set PDF bookmark */
PyObject *scribus_setpdfbookmark(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_ispdfbookmark__doc__,
 QT_TR_NOOP("isPDFBookmark([\"name\"]) -> bool\n\
\n\
Returns true if the text frame \"name\" is a PDF bookmark.\n\
If \"name\" is not given the currently selected item is used.\n\
\n\
May raise WrongFrameTypeError if the target frame is not a text frame\n\
"));
/*! Is PDF bookmark? */
PyObject *scribus_ispdfbookmark(PyObject * /*self*/, PyObject* args);

#endif

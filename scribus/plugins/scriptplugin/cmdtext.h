#ifndef CMDTEXT_H
#define CMDTEXT_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/*! Text frame handling */

/*! docstring */
PyDoc_STRVAR(scribus_getfontsize__doc__,
    "getFontSize([\"name\"]) -> float\n\n\
Returns the fontsize for the textframe \"name\". If this\
textframe has some text selected the value assigned to\
the first character of the selection is returned.\
If \"name\" is not given the currently selected item is used.");
/*! Get font size */
PyObject *scribus_getfontsize(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getfont__doc__,
    "getFont([\"name\"]) -> string\n\n\
Returns the font for the textframe \"name\". If this\
textframe has some text selected the value assigned to the\
first character of the selection is returned. If \"name\"\
is not given the currently selected Item is used.");
/*! Get font */
PyObject *scribus_getfont(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettextsize__doc__,
    "getTextLength([\"name\"]) -> integer\n\n\
Returns the length of the text in the textframe \"name\".\
If \"name\" is not given the currently selected item is used.");
/*! Get text size */
PyObject *scribus_gettextsize(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getframetext__doc__,
    "getText([\"name\"]) -> string\n\n\
Returns the text of the textframe \"name\". If this textframe\
has some text selected, this text is returned. If \"name\" is\
not given the currently selected item is used.");
/*! Get frame text */
PyObject *scribus_getframetext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettext__doc__,
    "getAllText([\"name\"]) -> string\n\n\
Returns the text of the textframe \"name\" and of all\
textframes which are linked with this frame. If this textframe\
has some text selected, this text is returned. If \"name\"\
is not given the currently selected Item is used.");
/*! Get all text */
PyObject *scribus_gettext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinespace__doc__,
    "getLineSpacing([\"name\"]) -> float\n\n\
Gets the linespacing of the text frame \"name\" expressed\
in points. If \"name\" is not given the currently selected\
item is used.");
/*! Get line space */
PyObject *scribus_getlinespace(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcolumngap__doc__,
    "getColumnGap([\"name\"]) -> float\n\n\
Gets the column gap of the text frame \"name\" expressed\
in points. If \"name\" is not given the currently selected\
item is used.");
/*! Get column gap */
PyObject *scribus_getcolumngap(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcolumns__doc__,
    "getColumns([\"name\"]) -> integer\n\n\
Gets the number of columns of the text frame \"name\".\
If \"name\" is not given the currently selected item is used.");
/*! Get columns */
PyObject *scribus_getcolumns(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setboxtext__doc__,
    "setText(\"text\", [\"name\"])\n\n\
Sets the text of the text frame \"name\" to the text of the\
string \"text\". Text must be UTF8 encoded - use e.g. unicode(text, 'iso-8859-2').\
If \"name\" is not given the currently selected item is used.");
/*! Set text */
PyObject *scribus_setboxtext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_inserttext__doc__,
    "insertText(\"text\", pos, [\"name\"])\n\n\
Inserts the text \"text\" at the position \"pos\" into the textframe.\
Text must be UTF encoded (see setText() as reference)\
The first character has an index of 0. \"name\" If \"name\" is not\
given the currently selected Item is used.");
/*! Insert text */
PyObject *scribus_inserttext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfont__doc__,
    "setFont(\"font\", [\"name\"])\n\n\
Sets the font of the text frame \"name\" to \"font\", if there\
is some text selected only the selected text is changed.\
If \"name\" is not given the currently selected item is used.");
/*! Set font */
PyObject *scribus_setfont(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfontsize__doc__,
    "setFontSize(size, [\"name\"])\n\n\
Sets the fontsize of the text frame \"name\" to the pointsize\
\"size\", if there is some text selected only the selected Text\
is changed. \"size\" must be in the Range 1 to 512. If \"name\"\
is not given the currently selected item is used.");
/*! Set font size */
PyObject *scribus_setfontsize(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinespace__doc__,
    "setLineSpacing(size, [\"name\"])\n\n\
Sets the linespacing of the text frame \"name\" to the pointsize\
\"size\". If \"name\" is not given the currently selected item is used.");
/*! Set line space */
PyObject *scribus_setlinespace(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolumngap__doc__,
    "setColumnGap(size, [\"name\"])\n\n\
Sets the column gap of the text frame \"name\" to the value \"size\".\
If \"name\" is not given the currently selected item is used.");
/*! Set column gap */
PyObject *scribus_setcolumngap(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolumns__doc__,
    "setColumns(nr, [\"name\"])\n\n\
Sets the number of columns of the text frame \"name\"\
to the value \"nr\". If \"name\" is not given the currently\
selected item is used.");
/*! Set columns */
PyObject *scribus_setcolumns(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setalign__doc__,
    "setTextAlignment(align, [\"name\"])\n\n\
Sets the text alignment of the text frame \"name\" to the specified\
alignment. If \"name\" is not given the currently selected item is\
used. \"align\" can have the defined constants - see ALIGN_<type>.");
/*! Set alignt */
PyObject *scribus_setalign(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_selecttext__doc__,
    "selectText(start, count, [\"name\"])\n\n\
Selects \"count\" characters text of the text frame \"name\"\
starting from the character \"start\". Character counting starts\
at 0. If \"count\" is zero, any text selection will be cleared.\
If \"name\" is not given the currently selected item is used.");
/*! Select text */
PyObject *scribus_selecttext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deletetext__doc__,
    "deleteText([\"name\"])\n\n\
Deletes the text of the text frame \"name\". If there is some\
text selected, this text will be deleted. If \"name\" is not\
given the currently selected Item is used.");
/*! Delete text */
PyObject *scribus_deletetext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextfill__doc__,
    "setTextColor(\"color\", [\"name\"])\n\n\
Sets the text color of the object \"name\" to the color\
\"color\", if there is some text selected only the selected\
text is changed. If \"name\" is not given the currently\
selected item is used.");
/*! Set text fill */
PyObject *scribus_settextfill(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextstroke__doc__,
    "setTextStroke(\"color\", [\"name\"])\n\n\
Set \"color\" of the text stroke. If \"name\" is not given the currently\
selected item is used.");
/*! Set text stroke */
PyObject *scribus_settextstroke(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextshade__doc__,
    "setTextShade(shade, [\"name\"])\n\n\
Sets the shading of the text color of the object \"name\" to \"shade\",\
if there is some text selected only the selected text is changed. \"shade\"\
must be an integer value in the range from 0 (lightest) to 100 (full\
color intensity). If \"name\" is not given the currently selected\
item is used.");
/*! Set text shde */
PyObject *scribus_settextshade(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_linktextframes__doc__,
    "linkTextFrames(\"name1\", \"name2\")\n\n\
Create the linked text frames. Parameters are the object names.");
/**
 Link text frames via Scripter.
 02/22/04 (petr vanek)
 TODO: PDF bookmarks handling
 */
PyObject *scribus_linktextframes(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_unlinktextframes__doc__,
    "linkTextFrames(\"name\")\n\n\
Remove the specified (named) object from the text frame flow/linkage.");
/**
 (Un)Link text frames via Scripter.
 02/22/04 (petr vanek)
 TODO: PDF bookmarks handling
 */
PyObject *scribus_unlinktextframes(PyObject * self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_tracetext__doc__,
    "traceText([\"name\"])\n\n\
Convert the text frame \"name\" to outlines. If \"name\" is not\
given the currently selected item is used.");
/**
 Trace text frames via Scripter.
 2004-09-07 (Craig Ringer)
*/
PyObject *scribus_tracetext(PyObject * self, PyObject* args);

#endif


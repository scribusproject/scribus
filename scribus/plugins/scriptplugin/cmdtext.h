#ifndef CMDTEXT_H
#define CMDTEXT_H

// docstrings
#ifndef PyDoc_STR
#define PyDoc_VAR(name)         static char name[]
#define PyDoc_STR(str)          (str)
#define PyDoc_STRVAR(name, str) PyDoc_VAR(name) = PyDoc_STR(str)
#endif


/*! Text frame handling */

/*! docstring */
PyDoc_STRVAR(scribus_getfontsize__doc__,
    "getFontSize()\n\n\
    Returns the Fontsize for the Textframe \"name\". If this\
    Textframe has some Text selected the Value assigned to\
    the first Character of the Selection is returned.\
    If \"name\" is not given the currently selected Item is used.");
/*! Get font size */
PyObject *scribus_getfontsize(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getfont__doc__,
    "getFont()\n\n\
    Returns the Font for the Textframe \"name\". If this\
    Textframe has some Text selected the Value assigned to the\
    first Character of the Selection is returned. If \"name\"\
    is not given the currently selected Item is used.");
/*! Get font */
PyObject *scribus_getfont(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettextsize__doc__,
    "getTextLength()\n\n\
    Returns the Length of the Text in the Textframe \"name\".\
    If \"name\" is not given the currently selected Item is used.");
/*! Get text size */
PyObject *scribus_gettextsize(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getframetext__doc__,
    "getText()\n\n\
    Returns the Text of the Textframe \"name\". If this Textframe\
    has some Text selected, this Text is returned. If \"name\" is\
    not given the currently selected Item is used.");
/*! Get frame text */
PyObject *scribus_getframetext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_gettext__doc__,
    "getAllText()\n\n\
    Returns the Text of the Textframe \"name\" and of all\
    Textframes which are linked with this Frame. If this Textframe\
    has some Text selected, this Text is returned. If \"name\"\
    is not given the currently selected Item is used.");
/*! Get all text */
PyObject *scribus_gettext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getlinespace__doc__,
    "getLineSpacing()\n\n\
    Gets the Linespacing of the Text Frame \"name\" expressed\
    in Points. If \"name\" is not given the currently selected\
    Item is used.");
/*! Get line space */
PyObject *scribus_getlinespace(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcolumngap__doc__,
    "getColumnGap()\n\n\
    Gets the Column Gap of the Text Frame \"name\" expressed\
    in Points. If \"name\" is not given the currently selected\
    Item is used.");
/*! Get column gap */
PyObject *scribus_getcolumngap(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getcolumns__doc__,
    "getColumns()\n\n\
    Gets the number of Columns of the Text Frame \"name\".\
    If \"name\" is not given the currently selected Item is used.");
/*! Get columns */
PyObject *scribus_getcolumns(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setboxtext__doc__,
    "setText()\n\n\
    Sets the Text of the Text Frame \"name\" to the Text of the\
    String \"text\". If \"name\" is not given the currently\
    selected Item is used.");
/*! Set text */
PyObject *scribus_setboxtext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_inserttext__doc__,
    "insertText()\n\n\
    Inserts the Text \"text\" at the Position \"pos\" into the Textframe\
    The first Character has an Index of 0. \"name\" If \"name\" is not\
    given the currently selected Item is used.");
/*! Insert text */
PyObject *scribus_inserttext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfont__doc__,
    "setFont()\n\n\
    Sets the Font of the Text Frame \"name\" to \"font\", if there\
    is some Text selected only the selected Text is changed.\
    If \"name\" is not given the currently selected Item is used.");
/*! Set font */
PyObject *scribus_setfont(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setfontsize__doc__,
    "setFontSize()\n\n\
    Sets the Fontsize of the Text Frame \"name\" to the Pointsize\
    \"size\", if there is some Text selected only the selected Text\
    is changed. \"size\" must be in the Range 1 to 512. If \"name\"\
    is not given the currently selected Item is used.");
/*! Set font size */
PyObject *scribus_setfontsize(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setlinespace__doc__,
    "setLineSpacing()\n\n\
    Sets the Linespacing of the Text Frame \"name\" to the Pointsize\
    \"size\". If \"name\" is not given the currently selected Item is used.");
/*! Set line space */
PyObject *scribus_setlinespace(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolumngap__doc__,
    "setColumnGap()\n\n\
    Sets the Column Gap of the Text Frame \"name\" to the Value \"size\".\
    If \"name\" is not given the currently selected Item is used.");
/*! Set column gap */
PyObject *scribus_setcolumngap(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setcolumns__doc__,
    "setColumns()\n\n\
    Sets the number of Columns of the Text Frame \"name\"\
    to the Value \"nr\". If \"name\" is not given the currently\
    selected Item is used.");
/*! Set columns */
PyObject *scribus_setcolumns(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setalign__doc__,
    "setTextAlignment()\n\n\
    Sets the Textalignment of the Text Frame \"name\" to the specified\
    Alignment. If \"name\" is not given the currently selected Item is\
    used. \"align\" can have the defined constants.");
/*! Set alignt */
PyObject *scribus_setalign(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_selecttext__doc__,
    "selectText()\n\n\
    Selects \"count\" Characters Text of the Textframe \"name\"\
    starting from the Character \"start\". Character Counting starts\
    at 0. If \"count\" is zero, any Text Selection will be cleared.\
    If \"name\" is not given the currently selected Item is used.");
/*! Select text */
PyObject *scribus_selecttext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_deletetext__doc__,
    "deleteText()\n\n\
    Deletes the Text of the Textframe \"name\". If there is some\
    Text selected, this Text will be deleted. If \"name\" is not\
    given the currently selected Item is used.");
/*! Delete text */
PyObject *scribus_deletetext(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextfill__doc__,
    "setTextColor()\n\n\
    Sets the Text Color of the Object \"name\" to the Color\
    \"color\", if there is some Text selected only the selected\
    Text is changed. If \"name\" is not given the currently\
    selected Item is used.");
/*! Set text fill */
PyObject *scribus_settextfill(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextstroke__doc__,
    "setTextStroke()\n\n\
    TODO: docstring");
/*! Set text stroke */
PyObject *scribus_settextstroke(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_settextshade__doc__,
    "setTextShade()\n\n\
    Sets the shading of the Text Color of the Object \"name\" to \"shade\",\
    if there is some Text selected only the selected Text is changed. \"shade\"\
    must be an Integer Value in the range from 0 (lightest) to 100 (full\
    Color intensity). If \"name\" is not given the currently selected\
    Item is used.");
/*! Set text shde */
PyObject *scribus_settextshade(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_linktextframes__doc__,
    "linkTextFrames()\n\n\
    Create the linked text frames. Parameters are the object names.");
/**
 Link text frames via Scripter.
 02/22/04 (petr vanek)
 TODO: PDF bookmarks handling
 */
PyObject *scribus_linktextframes(PyObject *self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_unlinktextframes__doc__,
    "linkTextFrames()\n\n\
    Remove the specified (named) object from the text frame flows/linkage.");
/**
 (Un)Link text frames via Scripter.
 02/22/04 (petr vanek)
 TODO: PDF bookmarks handling
 */
PyObject *scribus_unlinktextframes(PyObject * self, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_tracetext__doc__,
    "traceText()\n\n\
    TODO: docstring");
/**
 Trace text frames via Scripter.
 2004-09-07 (Craig Ringer)
*/
PyObject *scribus_tracetext(PyObject * self, PyObject* args);

#endif


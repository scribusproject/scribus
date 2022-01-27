/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDANNOTATIONS_H
#define CMDANNOTATIONS_H

// Pulls in <Python.h> first
#include "cmdvar.h"

/*! Text frame pdf annotation handling */

/*! docstring */
PyDoc_STRVAR(scribus_isannotated__doc__,
 QT_TR_NOOP("isAnnotated([\"name\"],[\"deannotate=False\"])\n\
\n\
Queries the item to see if it has a Pdf annotation.\n\
\n\
Arguments:\n\
\"name\" uses the currently selected item if not given.\n\
\n\
Keyword Arguments:\n\
\"deannotate\" if set to True will turn off the annotation flag\n\
\n\
Returns:\n\
A tuple with a string at 0 that indicates\n\
what type of pdf annotation it is.  A dictionary is\n\
in index 1 that contains data the function was able to gather.\n\
If the item is not a pdf annotation returns None.\n\
Passing the keyword parameter deannotate=True returns None.\n\
\n\
May raise WrongFrameTypeError if the target frame is not a text frame\n\
\
"));
/*!returns a tuple if the text frame is annotated */
PyObject *scribus_isannotated(PyObject * /*self*/, PyObject* args, PyObject *keywds);




PyDoc_STRVAR(scribus_setfileannotation__doc__,
 QT_TR_NOOP("setFileAnnotation(path, page, x, y, [\"name\"]),[\"absolute=True\"])\n\
\n\
Turns a text frame into a absolute or relative link annotation.\n\
Arguments:\n\
\"path\" is the absolute or relative path to the file.\n\
\"page\" is the page that it links to.\n\
\"x\" and \"y\" are the x and y coordinates of the page.\n\
\"name\" uses the currently selected item if not given.\n\
\n\
Keyword arguments:\n\
\"absolute\" if set to False will make this a relative path.\n\
True is its default.\n\
\n\
Returns:\n\
None\n\
\n\
May raise WrongFrameTypeError if the target frame is not a text frame\n\
"));
/*!adds a file annotation to text frame*/
PyObject *scribus_setfileannotation(PyObject * /*self*/, PyObject* args, PyObject *keywds);




/*! docstring */
PyDoc_STRVAR(scribus_setlinkannotation__doc__,
 QT_TR_NOOP("setLinkAnnotation(page,x,y,[\"name\"])\n\
\n\
Turns a text fame into a link that gotos a page in the document.\n\
\n\
Arguments:\n\
\"page\" is the page the link will take you to.\n\
Must be 1 and cannot be greater than the number of pages in the document.\n\
\"x\" and \"y\" are the x and y coordinates of the page.\n\
\"name\" uses the currently selected item if not given.\n\
\n\
Returns:\n\
None\n\
\n\
May raise WrongFrameTypeError if the target frame is not a text frame\n\
"));
/*!adds a link annotation to a text frame  */
PyObject *scribus_setlinkannotation(PyObject * /*self*/, PyObject* args);


/*! docstring */
PyDoc_STRVAR(scribus_seturiannotation__doc__,
 QT_TR_NOOP("setURIAnnotation(uri,[\"name\"])\n\
\n\
Turns a text fame into a uri link that gotos the uri specified.\n\
\n\
Arguments:\n\
\"uri\" is the uri that the link will be set to.\n\
\"name\" uses the currently selected item if not given.\n\
\n\
Returns:\n\
None\n\
\n\
May raise WrongFrameTypeError if the target frame is not a text frame\n\
"));
/*!adds a uri annotation to a text frame */
PyObject *scribus_seturiannotation(PyObject * /*self*/, PyObject* args);


/*! docstring */
PyDoc_STRVAR(scribus_settextannotation__doc__,
 QT_TR_NOOP("setTextAnnotation(icon,isopen,[\"name\"])\n\
\n\
Turns a text fame into a text annotation.\n\
\n\
Arguments:\n\
\"icon\" must be 0-8.  The values correspond to:(\n\
0 \"Note\", 1 \"Comment\", 2 \"Key\",\n\
3 \"Help\", 4 \"NewParagraph\", 5 \"Paragraph\",\n\
6 \"Insert\",7 \"Cross\", 8 \"Circle\")n\
\"isopen\" is True or False.\n\
\"name\" uses the currently selected item if not given.\n\
\n\
Returns:\n\
None\n\
\n\
May raise WrongFrameTypeError if the target frame is not a text frame\n\
"));
/*!adds a text annotation to a text frame  */
PyObject *scribus_settextannotation(PyObject * /*self*/, PyObject* args);



/*! docstring */
PyDoc_STRVAR(scribus_createpdfannotation__doc__,
 QT_TR_NOOP("createPdfAnnotation(which,x,y,w,h,[\"name\"])\n\
\n\
Creates a pdf annotation.\n\
\n\
Arguments:\n\
\"which\" is one of the following:\n\
(0 PDFBUTTON, 1 PDFRADIOBUTTON, 2 PDFTEXTFIELD,\n\
3 PDFCHECKBOX, 4 PDFCOMBOBOX, 5 PDFLISTBOX,\n\
6 PDFTEXTANNOTATION, 7 PDFLINKANNOTATION, 8 PDF3DANNOTATION)\n\
\"x\" and \"y\" are the coordinates where it will be placed.\n\
\"w\" is its width.\n\
\"h\" is its height.\n\
On systems without OSG installed a runtime error will be raised.\n\
\"name\" should be a unique identifier for the object\n\
because you need this name for further referencing of that object.\n\
If \"name\" is not given Scribus will create one for you.\n\
\n\
Returns:\n\
The name of the newly created annotation.\n\
\n\
May raise NameExistsError if you explicitly pass a name that's already used.\n\
\n\
"));
/*!creates a pdf annotation and a text frame.*/
PyObject *scribus_createpdfannotation(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_getjsactionscript__doc__,
	QT_TR_NOOP("getJSActionScript(which,[\"name\"])\n\
\n\
Gets the JavaScript action for a particular event\n\
\"which\" is one of the following:\n\
(0 Mouse Up, 1 Mouse Down, 2 Mouse Enter,\n\
3 Mouse Exit, 4 Focus In, 5 Focus Out,\n\
6 Selection Change, 7 Field Format,\n\
8 Field Validate, 9 Field Calculate)\n\
\"name\" uses the currently selected item if not given.\n\
Page item must be an annotation or an error will be raised.\n\
Returns:\n\
Returns a string if object's action type is Javascript, \n\
NONE otherwise.\n\
"));
/*!gets the JavaScript for an annotation action.*/
PyObject *scribus_getjsactionscript(PyObject * /*self*/, PyObject* args);

/*! docstring */
PyDoc_STRVAR(scribus_setjsactionscript__doc__,
 QT_TR_NOOP("setJSActionScript(which,script,[\"name\"])\n\
\n\
Sets the JavaScript action for a particular event.\n\
Also sets the annotation's action to JavaScript.\n\
\"which\" is one of the following:\n\
(0 Mouse Up, 1 Mouse Down, 2 Mouse Enter,\n\
3 Mouse Exit, 4 Focus In, 5 Focus Out,\n\
6 Selection Change, 7 Field Format,\n\
8 Field Validate, 9 Field Calculate)\n\
\"script\" is the JavaScript set to the action.\n\
\"name\" uses the currently selected item if not given.\n\
Page item must be an annotation or an error will be raised.\n\
Returns:\n\
None\n\
"));
/*!sets the JavaScript on an annotation action.*/
PyObject *scribus_setjsactionscript(PyObject * /*self*/, PyObject* args);

#endif

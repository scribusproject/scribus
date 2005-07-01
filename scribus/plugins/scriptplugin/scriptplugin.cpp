/***************************************************************************
                          scriptplugin.cpp  -  description
                             -------------------
    begin                : Thu Oct 3 08:00:00 CEST 2002
    copyright            : (C) 2002 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "scriptplugin.h"

#include "cmddialog.h"
#include "cmddoc.h"
#include "cmdpage.h"
#include "cmdobj.h"
#include "cmdgetprop.h"
#include "cmdsetprop.h"
#include "cmdtext.h"
#include "cmdmani.h"
#include "cmdcolor.h"
#include "cmdmisc.h"
#include "cmdutil.h"
#include "objprinter.h"
#include "objpdffile.h"
#include "objimageexport.h"
#include "macro.h"
#include "macromanager.h"
#include "extmacro.h"
#include "scripterprefs.h"
#include "guiapp.h"
#include "customfdialog.h"
#include "helpbrowser.h"
#include "mpalette.h"
#include "scriptercore.h"

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qdom.h>
#include <qtextstream.h>
#include <cstdlib>

// Exceptions; visible from cmdvar.h, set up in initscribus()
PyObject* ScribusException;
PyObject* NoDocOpenError;
PyObject* WrongFrameTypeError;
PyObject* NoValidObjectError;
PyObject* NotFoundError;
PyObject* NameExistsError;
PyObject* AccessDeniedError;

// Script console globals declared `extern' in cmdvar.h
QString RetString;
QString InValue;
int RetVal;

// Scripter core instance
ScripterCore* scripterCore;

QString Name()
{
	return QObject::tr("S&cripter Manual...");
}

int Type()
{
	return 4;
}

int ID()
{
	return 8;
}

void InitPlug(QWidget *d, ScribusApp *plug)
{
	// push the QApplication instance we've been passed into the Carrier global
	Carrier = plug;
	// init Python
	QString cm;
	Py_Initialize();
	if (PyUnicode_SetDefaultEncoding("utf-8"))
	{
		qDebug("Failed to set default encoding to utf-8.\n");
		PyErr_Clear();
	}
	RetVal = 0;
	// Init the class that manages the scripter and its gui
	scripterCore = new ScripterCore(d);
	if (!scripterCore)
		qDebug("scripterCore failed to initialize");
	// Set up the `scribus' module in the main interpreter
	initscribus(Carrier);
	// If Python extensions are enabled, load the macro manager.
	if (scripterCore->enableExtPython)
		MacroManager::instance();
	// and run the start-up script, if any
	scripterCore->runStartupScript();
}

void CleanUpPlug()
{
	MacroManager::deleteInstance();
	if (scripterCore)
		delete(scripterCore);
	Py_Finalize();
}

void Run(QWidget *d, ScribusApp *plug)
{
	QString pfad = DOCDIR;
	QString pfad2;
	pfad2 = QDir::convertSeparators(pfad + "en/Scripter/index.html");
	HelpBrowser *dia = new HelpBrowser(d, QObject::tr("Online Reference"), plug->GuiLanguage, "scripter");
	dia->show();
}

// This function builds a Python wrapper function called newName around the
// python function called oldName. The wrapper function prints a warning, then
// calls oldName with all passed arguments and returns the result from oldName.
// The wrapper is stored in the module dictionary passed, so it appears in the
// `scribus' module and will be imported by 'from scribus import'. A docstring
// is provided to direct the user to the correct function.
// By default the warning gets output only on the first use of the function in a given
// interpreter instance, but user scripts can change this.
void deprecatedFunctionAlias(PyObject* scribusdict, char* oldName, char* newName)
{
	// Build the Python code to create the wrapper function
	QString wrapperFunc = "";
	wrapperFunc += QString("def %1(*args, **kwargs):\n").arg(newName);
	wrapperFunc += QString("    \"\"\"Deprecated alias for function %1 - see help(%2).\"\"\"\n").arg(oldName).arg(oldName);
	wrapperFunc += QString("    warnings.warn(\"Warning, script function %1 is deprecated, use %2 instead.\\n\",exceptions.DeprecationWarning)\n").arg(newName).arg(oldName);
	wrapperFunc += QString("    return %1(*args,**kwargs)\n").arg(oldName);
	QCString wsData = wrapperFunc.latin1();	//this should probably be utf8 now
	// And run it in the namespace of the scribus module
	PyObject* result = PyRun_String(wsData, Py_file_input, scribusdict, scribusdict);
	// NULL is returned if an exception is set. We don't care about any other return value and
	// can ignore it.
	if (result == NULL)
	{
		qDebug("Failed to alias %s to %s in Python scripter - exception raised!", oldName, newName);
		PyErr_Print();
	}
	// Because 'result' may be NULL, not a PyObject*, we must call PyXDECREF not Py_DECREF
	Py_XDECREF(result);
}

// Copy constants by coping the reference to them. Equivalent to
// scribus.__dict__[newName] = scribus.__dict__[oldName].
void constantAlias(PyObject* scribusdict, const char* oldName, const char* newName)
{
	/* Work around the braindead Python/C API not using 'const' for
	 * strings it won't change. */
	char* newNameTemp = strdup(newName);
	char* oldNameTemp = strdup(oldName);
	assert(newNameTemp != NULL);
	assert(oldNameTemp != NULL);
	/* We use PyMapping_GetItemString because unlike PyDict_GetItemString it
	 * returns a new (not borrowed) reference, while PyDict_SetItemString
	 * consumes the reference. That means we don't have to Py_INCREF anything.
	 */
	PyObject* item = PyMapping_GetItemString(scribusdict, oldNameTemp);
	assert(item != NULL);
	PyDict_SetItemString(scribusdict, newNameTemp, item);
	free(newNameTemp);
	free(oldNameTemp);
}

/****************************************************************************************/
/*                                                                                      */
/*   Definitions of the Python commands                                                 */
/*                                                                                      */
/****************************************************************************************/

static PyObject *scribus_retval(PyObject* /*self*/, PyObject* args)
{
	char *Name = NULL;
	int retV = 0;
	if (!PyArg_ParseTuple(args, (char*)"si", &Name, &retV))
		return NULL;
	// Because sysdefaultencoding is not utf-8, Python is returning utf-8 encoded
	// 8-bit char* strings. Make sure Qt understands that the input is utf-8 not
	// the default local encoding (usually latin-1) by using QString::fromUtf8()
	RetString = QString::fromUtf8(Name);
	RetVal = retV;
	return PyInt_FromLong(0L);
}

static PyObject *scribus_getval(PyObject* /*self*/)
{
	return PyString_FromString(InValue.utf8().data());
}

/*!
 * Translate a docstring. Small helper function for use with the
 * PyMethodDef struct.
 */
char* tr(const char* docstringConstant)
{
	// Alas, there's a lot of wasteful string copying going on
	// here.
	QString translated = QObject::tr(docstringConstant, "scripter docstring");
	/*
	 * Python doesn't support 'unicode' object docstrings in the PyMethodDef,
	 * and has no way to specify what encoding docstrings are in. The passed C
	 * strings passed are made into 'str' objects as-is. These are interpreted
	 * as being in the Python sysdefaultencoding, usually 'ascii', when used.
	 * We now set systemdefaultencoding to 'utf-8' ...  so we're going to pass
	 * Python an 8-bit utf-8 encoded string in a char* .  With
	 * sysdefaultencoding set correctly, Python will interpret it correctly and
	 * we'll have our unicode docstrings. It's not as ugly a hack as it sounds,
	 * you just have to remember that C and Python strings can both be
	 * considered 8-bit strings of binary data that can be later interpreted as
	 * a text string in a particular text encoding.
	 */
	QCString utfTranslated = translated.utf8();
	const char* trch = utfTranslated.data();
	return strdup(trch);
}

/* Now we're using the more pythonic convention for names:
 * class - ClassName
 * procedure/function/method - procedureName
 * etc. */
PyMethodDef scribus_methods[] = {
	// 2004/10/03 pv - aliases with common Python syntax - ClassName methodName
	// 2004-11-06 cr - move aliasing to dynamically generated wrapper functions, sort methoddef
	{const_cast<char*>("changeColor"), scribus_setcolor, METH_VARARGS, tr(scribus_setcolor__doc__)},
	{const_cast<char*>("closeDoc"), (PyCFunction)scribus_closedoc, METH_NOARGS, tr(scribus_closedoc__doc__)},
	{const_cast<char*>("createBezierLine"), scribus_bezierline, METH_VARARGS, tr(scribus_bezierline__doc__)},
	{const_cast<char*>("createEllipse"), scribus_newellipse, METH_VARARGS, tr(scribus_newellipse__doc__)},
	{const_cast<char*>("createImage"), scribus_newimage, METH_VARARGS, tr(scribus_newimage__doc__)},
	{const_cast<char*>("createLayer"), scribus_createlayer, METH_VARARGS, tr(scribus_createlayer__doc__)},
	{const_cast<char*>("createLine"), scribus_newline, METH_VARARGS, tr(scribus_newline__doc__)},
	{const_cast<char*>("createPathText"), scribus_pathtext, METH_VARARGS, tr(scribus_pathtext__doc__)},
	{const_cast<char*>("createPolygon"), scribus_polygon, METH_VARARGS, tr(scribus_polygon__doc__)},
	{const_cast<char*>("createPolyLine"), scribus_polyline, METH_VARARGS, tr(scribus_polyline__doc__)},
	{const_cast<char*>("createRect"), scribus_newrect, METH_VARARGS, tr(scribus_newrect__doc__)},
	{const_cast<char*>("createText"), scribus_newtext, METH_VARARGS, tr(scribus_newtext__doc__)},
	{const_cast<char*>("currentPage"), (PyCFunction)scribus_actualpage, METH_NOARGS, tr(scribus_actualpage__doc__)},
	{const_cast<char*>("defineColor"), scribus_newcolor, METH_VARARGS, tr(scribus_newcolor__doc__)},
	{const_cast<char*>("deleteColor"), scribus_delcolor, METH_VARARGS, tr(scribus_delcolor__doc__)},
	{const_cast<char*>("deleteLayer"), scribus_removelayer, METH_VARARGS, tr(scribus_removelayer__doc__)},
	{const_cast<char*>("deleteObject"), scribus_deleteobj, METH_VARARGS, tr(scribus_deleteobj__doc__)},
	{const_cast<char*>("deletePage"), scribus_deletepage, METH_VARARGS, tr(scribus_deletepage__doc__)},
	{const_cast<char*>("deleteText"), scribus_deletetext, METH_VARARGS, tr(scribus_deletetext__doc__)},
	{const_cast<char*>("deselectAll"), (PyCFunction)scribus_deselect, METH_NOARGS, tr(scribus_deselect__doc__)},
	{const_cast<char*>("docChanged"), scribus_docchanged, METH_VARARGS, tr(scribus_docchanged__doc__)},
	{const_cast<char*>("fileDialog"), (PyCFunction)scribus_filedia, METH_VARARGS|METH_KEYWORDS, tr(scribus_filedia__doc__)},
	{const_cast<char*>("getActiveLayer"), (PyCFunction)scribus_getactlayer, METH_NOARGS, tr(scribus_getactlayer__doc__)},
	{const_cast<char*>("getAllObjects"), scribus_getallobj, METH_VARARGS, tr(scribus_getallobj__doc__)},
	{const_cast<char*>("getAllStyles"), (PyCFunction)scribus_getstylenames, METH_NOARGS, tr(scribus_getstylenames__doc__)},
	{const_cast<char*>("getAllText"), scribus_gettext, METH_VARARGS, tr(scribus_gettext__doc__)},
	{const_cast<char*>("getColorNames"), (PyCFunction)scribus_colornames, METH_NOARGS, tr(scribus_colornames__doc__)},
	{const_cast<char*>("getColor"), scribus_getcolor, METH_VARARGS, tr(scribus_getcolor__doc__)},
	{const_cast<char*>("getColorAsRGB"), scribus_getcolorasrgb, METH_VARARGS, tr(scribus_getcolorasrgb__doc__)},
	{const_cast<char*>("getColumnGap"), scribus_getcolumngap, METH_VARARGS, tr(scribus_getcolumngap__doc__)},
	{const_cast<char*>("getColumns"), scribus_getcolumns, METH_VARARGS, tr(scribus_getcolumns__doc__)},
	{const_cast<char*>("getCornerRadius"), scribus_getcornerrad, METH_VARARGS, tr(scribus_getcornerrad__doc__)},
	{const_cast<char*>("getFillColor"), scribus_getfillcolor, METH_VARARGS, tr(scribus_getfillcolor__doc__)},
	{const_cast<char*>("getFillShade"), scribus_getfillshade, METH_VARARGS, tr(scribus_getfillshade__doc__)},
	{const_cast<char*>("getFontNames"), (PyCFunction)scribus_fontnames, METH_NOARGS, tr(scribus_fontnames__doc__)},
	{const_cast<char*>("getFont"), scribus_getfont, METH_VARARGS, tr(scribus_getfont__doc__)},
	{const_cast<char*>("getFontSize"), scribus_getfontsize, METH_VARARGS, tr(scribus_getfontsize__doc__)},
	{const_cast<char*>("getGuiLanguage"), (PyCFunction)scribus_getlanguage, METH_NOARGS, tr(scribus_getlanguage__doc__)},
	{const_cast<char*>("getHGuides"), (PyCFunction)scribus_getHguides, METH_NOARGS, tr(scribus_getHguides__doc__)},
	{const_cast<char*>("getImageFile"), scribus_getimgname, METH_VARARGS, tr(scribus_getimgname__doc__)},
	{const_cast<char*>("getImageScale"), scribus_getimgscale, METH_VARARGS, tr(scribus_getimgscale__doc__)},
	{const_cast<char*>("getLayers"), (PyCFunction)scribus_getlayers, METH_NOARGS, tr(scribus_getlayers__doc__)},
	{const_cast<char*>("getLineCap"), scribus_getlineend, METH_VARARGS, tr(scribus_getlineend__doc__)},
	{const_cast<char*>("getLineColor"), scribus_getlinecolor, METH_VARARGS, tr(scribus_getlinecolor__doc__)},
	{const_cast<char*>("getLineJoin"), scribus_getlinejoin, METH_VARARGS, tr(scribus_getlinejoin__doc__)},
	{const_cast<char*>("getLineShade"), scribus_getlineshade, METH_VARARGS, tr(scribus_getlineshade__doc__)},
	{const_cast<char*>("getLineSpacing"), scribus_getlinespace, METH_VARARGS, tr(scribus_getlinespace__doc__)},
	{const_cast<char*>("getLineStyle"), scribus_getlinestyle, METH_VARARGS, tr(scribus_getlinestyle__doc__)},
	{const_cast<char*>("getLineWidth"), scribus_getlinewidth, METH_VARARGS, tr(scribus_getlinewidth__doc__)},
	{const_cast<char*>("getPageItems"), (PyCFunction)scribus_getpageitems, METH_NOARGS, tr(scribus_getpageitems__doc__)},
	{const_cast<char*>("getPageMargins"), (PyCFunction)scribus_getpagemargins, METH_NOARGS, tr(scribus_getpagemargins__doc__)},
	{const_cast<char*>("getPageSize"), (PyCFunction)scribus_pagedimension, METH_NOARGS, tr(scribus_pagedimension__doc__)}, // just an alias to PageDimension()
	{const_cast<char*>("getPosition"), scribus_getposi, METH_VARARGS, tr(scribus_getposi__doc__)},
	{const_cast<char*>("getRotation"), scribus_getrotation, METH_VARARGS, tr(scribus_getrotation__doc__)},
	{const_cast<char*>("getSelectedObject"), scribus_getselobjnam, METH_VARARGS, tr(scribus_getselobjnam__doc__)},
	{const_cast<char*>("getSize"), scribus_getsize, METH_VARARGS, tr(scribus_getsize__doc__)},
	{const_cast<char*>("getTextColor"), scribus_getlinecolor, METH_VARARGS, tr(scribus_getlinecolor__doc__)},
	{const_cast<char*>("getTextLength"), scribus_gettextsize, METH_VARARGS, tr(scribus_gettextsize__doc__)},
	{const_cast<char*>("getText"), scribus_getframetext, METH_VARARGS, tr(scribus_getframetext__doc__)},
	{const_cast<char*>("getTextShade"), scribus_getlineshade, METH_VARARGS, tr(scribus_getlineshade__doc__)},
	{const_cast<char*>("getUnit"), (PyCFunction)scribus_getunit, METH_NOARGS, tr(scribus_getunit__doc__)},
	{const_cast<char*>("getVGuides"), (PyCFunction)scribus_getVguides, METH_NOARGS, tr(scribus_getVguides__doc__)},
	{const_cast<char*>("getXFontNames"), (PyCFunction)scribus_xfontnames, METH_NOARGS, tr(scribus_xfontnames__doc__)},
	{const_cast<char*>("gotoPage"), scribus_gotopage, METH_VARARGS, tr(scribus_gotopage__doc__)},
	{const_cast<char*>("groupObjects"), scribus_groupobj, METH_VARARGS, tr(scribus_groupobj__doc__)},
	{const_cast<char*>("haveDoc"), (PyCFunction)scribus_havedoc, METH_NOARGS, tr(scribus_havedoc__doc__)},
	{const_cast<char*>("insertText"), scribus_inserttext, METH_VARARGS, tr(scribus_inserttext__doc__)},
	{const_cast<char*>("isLayerPrintable"), scribus_glayerprint, METH_VARARGS, tr(scribus_glayerprint__doc__)},
	{const_cast<char*>("isLayerVisible"), scribus_glayervisib, METH_VARARGS, tr(scribus_glayervisib__doc__)},
	{const_cast<char*>("isLocked"), scribus_islocked, METH_VARARGS, tr(scribus_islocked__doc__)},
	{const_cast<char*>("linkTextFrames"), scribus_linktextframes, METH_VARARGS, tr(scribus_linktextframes__doc__)},
	{const_cast<char*>("loadImage"), scribus_loadimage, METH_VARARGS, tr(scribus_loadimage__doc__)},
	{const_cast<char*>("loadStylesFromFile"), scribus_loadstylesfromfile, METH_VARARGS, tr(scribus_loadstylesfromfile__doc__)},
	{const_cast<char*>("lockObject"), scribus_lockobject, METH_VARARGS, tr(scribus_lockobject__doc__)},
	{const_cast<char*>("messagebarText"), scribus_messagebartext, METH_VARARGS, tr(scribus_messagebartext__doc__)},
	{const_cast<char*>("messageBox"), (PyCFunction)scribus_messdia, METH_VARARGS|METH_KEYWORDS, tr(scribus_messdia__doc__)},
	{const_cast<char*>("moveObjectAbs"), scribus_moveobjabs, METH_VARARGS, tr(scribus_moveobjabs__doc__)},
	{const_cast<char*>("moveObject"), scribus_moveobjrel, METH_VARARGS, tr(scribus_moveobjrel__doc__)},
	{const_cast<char*>("newDocDialog"), (PyCFunction)scribus_newdocdia, METH_NOARGS, tr(scribus_newdocdia__doc__)},
	{const_cast<char*>("newDoc"), scribus_newdoc, METH_VARARGS, tr(scribus_newdoc__doc__)},
	{const_cast<char*>("newPage"), scribus_newpage, METH_VARARGS, tr(scribus_newpage__doc__)},
	{const_cast<char*>("objectExists"),scribus_objectexists, METH_VARARGS, tr(scribus_objectexists__doc__)},
	{const_cast<char*>("openDoc"), scribus_opendoc, METH_VARARGS, tr(scribus_opendoc__doc__)},
	{const_cast<char*>("pageCount"), (PyCFunction)scribus_pagecount, METH_NOARGS, tr(scribus_pagecount__doc__)},
	{const_cast<char*>("pageDimension"), (PyCFunction)scribus_pagedimension, METH_NOARGS, tr(scribus_pagedimension__doc__)},
	{const_cast<char*>("progressReset"), (PyCFunction)scribus_progressreset, METH_NOARGS, tr(scribus_progressreset__doc__)},
	{const_cast<char*>("progressSet"), scribus_progresssetprogress, METH_VARARGS, tr(scribus_progresssetprogress__doc__)},
	{const_cast<char*>("progressTotal"), scribus_progresssettotalsteps, METH_VARARGS, tr(scribus_progresssettotalsteps__doc__)},
	{const_cast<char*>("redrawAll"), (PyCFunction)scribus_redraw, METH_NOARGS, tr(scribus_redraw__doc__)},
	{const_cast<char*>("renderFont"), (PyCFunction)scribus_renderfont, METH_KEYWORDS, tr(scribus_renderfont__doc__)},
	{const_cast<char*>("replaceColor"), scribus_replcolor, METH_VARARGS, tr(scribus_replcolor__doc__)},
	{const_cast<char*>("rotateObjectAbs"), scribus_rotobjabs, METH_VARARGS, tr(scribus_rotobjabs__doc__)},
	{const_cast<char*>("rotateObject"), scribus_rotobjrel, METH_VARARGS, tr(scribus_rotobjrel__doc__)},
	{const_cast<char*>("saveDocAs"), scribus_savedocas, METH_VARARGS, tr(scribus_savedocas__doc__)},
	{const_cast<char*>("saveDoc"), (PyCFunction)scribus_savedoc, METH_NOARGS, tr(scribus_savedoc__doc__)},
	{const_cast<char*>("savePageAsEPS"), scribus_savepageeps, METH_VARARGS, tr(scribus_savepageeps__doc__)},
	{const_cast<char*>("scaleGroup"), scribus_scalegroup, METH_VARARGS, tr(scribus_scalegroup__doc__)},
	{const_cast<char*>("scaleImage"), scribus_scaleimage, METH_VARARGS, tr(scribus_scaleimage__doc__)},
	{const_cast<char*>("selectionCount"), (PyCFunction)scribus_selcount, METH_NOARGS, tr(scribus_selcount__doc__)},
	{const_cast<char*>("selectObject"), scribus_selectobj, METH_VARARGS, tr(scribus_selectobj__doc__)},
	{const_cast<char*>("selectText"), scribus_selecttext, METH_VARARGS, tr(scribus_selecttext__doc__)},
	{const_cast<char*>("sentToLayer"), scribus_senttolayer, METH_VARARGS, tr(scribus_senttolayer__doc__)},
	{const_cast<char*>("setActiveLayer"), scribus_setactlayer, METH_VARARGS, tr(scribus_setactlayer__doc__)},
	{const_cast<char*>("setPDFBookmark"), scribus_setpdfbookmark, METH_VARARGS, tr(scribus_setpdfbookmark__doc__)},
	{const_cast<char*>("isPDFBookmark"), scribus_ispdfbookmark, METH_VARARGS, tr(scribus_ispdfbookmark__doc__)},
	{const_cast<char*>("setColumnGap"), scribus_setcolumngap, METH_VARARGS, tr(scribus_setcolumngap__doc__)},
	{const_cast<char*>("setColumns"), scribus_setcolumns, METH_VARARGS, tr(scribus_setcolumns__doc__)},
	{const_cast<char*>("setCornerRadius"), scribus_setcornerrad, METH_VARARGS, tr(scribus_setcornerrad__doc__)},
	{const_cast<char*>("setCursor"), scribus_setcursor, METH_VARARGS, tr(scribus_setcursor__doc__)},
	{const_cast<char*>("setDocType"), scribus_setdoctype, METH_VARARGS, tr(scribus_setdoctype__doc__)},
	{const_cast<char*>("setFillColor"), scribus_setfillcolor, METH_VARARGS, tr(scribus_setfillcolor__doc__)},
	{const_cast<char*>("setFillShade"), scribus_setfillshade, METH_VARARGS, tr(scribus_setfillshade__doc__)},
	{const_cast<char*>("setFont"), scribus_setfont, METH_VARARGS, tr(scribus_setfont__doc__)},
	{const_cast<char*>("setFontSize"), scribus_setfontsize, METH_VARARGS, tr(scribus_setfontsize__doc__)},
	{const_cast<char*>("setGradientFill"), scribus_setgradfill, METH_VARARGS, tr(scribus_setgradfill__doc__)},
	{const_cast<char*>("setHGuides"), scribus_setHguides, METH_VARARGS, tr(scribus_setHguides__doc__)},
	{const_cast<char*>("setInfo"), scribus_setinfo, METH_VARARGS, tr(scribus_setinfo__doc__)},
	{const_cast<char*>("setLayerPrintable"), scribus_layerprint, METH_VARARGS, tr(scribus_layerprint__doc__)},
	{const_cast<char*>("setLayerVisible"), scribus_layervisible, METH_VARARGS, tr(scribus_layervisible__doc__)},
	{const_cast<char*>("setLineCap"), scribus_setlineend, METH_VARARGS, tr(scribus_setlineend__doc__)},
	{const_cast<char*>("setLineColor"), scribus_setlinecolor, METH_VARARGS, tr(scribus_setlinecolor__doc__)},
	{const_cast<char*>("setLineJoin"), scribus_setlinejoin, METH_VARARGS, tr(scribus_setlinejoin__doc__)},
	{const_cast<char*>("setLineShade"), scribus_setlineshade, METH_VARARGS, tr(scribus_setlineshade__doc__)},
	{const_cast<char*>("setLineSpacing"), scribus_setlinespace, METH_VARARGS, tr(scribus_setlinespace__doc__)},
	{const_cast<char*>("setLineStyle"), scribus_setlinestyle, METH_VARARGS, tr(scribus_setlinestyle__doc__)},
	{const_cast<char*>("setLineWidth"), scribus_setlinewidth, METH_VARARGS, tr(scribus_setlinewidth__doc__)},
	{const_cast<char*>("setMargins"), scribus_setmargins, METH_VARARGS, tr(scribus_setmargins__doc__)},
	{const_cast<char*>("setMultiLine"), scribus_setmultiline, METH_VARARGS, tr(scribus_setmultiline__doc__)},
	// duplicity? {"setMultiLine", scribus_setmultiline, METH_VARARGS, "TODO: docstring"},
	{const_cast<char*>("setRedraw"), scribus_setredraw, METH_VARARGS, tr(scribus_setredraw__doc__)},
	// missing? {"setSelectedObject", scribus_setselobjnam, METH_VARARGS, "Returns the Name of the selecteted Object. \"nr\" if given indicates the Number of the selected Object, e.g. 0 means the first selected Object, 1 means the second selected Object and so on."},
	{const_cast<char*>("setScaleImageToFrame"), (PyCFunction)scribus_setscaleimagetoframe, METH_KEYWORDS, tr(scribus_setscaleimagetoframe__doc__)},
	{const_cast<char*>("setStyle"), scribus_setstyle, METH_VARARGS, tr(scribus_setstyle__doc__)},
	{const_cast<char*>("setTextAlignment"), scribus_setalign, METH_VARARGS, tr(scribus_setalign__doc__)},
	{const_cast<char*>("setTextColor"), scribus_settextfill, METH_VARARGS, tr(scribus_settextfill__doc__)},
	{const_cast<char*>("setText"), scribus_setboxtext, METH_VARARGS, tr(scribus_setboxtext__doc__)},
	{const_cast<char*>("setTextShade"), scribus_settextshade, METH_VARARGS, tr(scribus_settextshade__doc__)},
	{const_cast<char*>("setTextStroke"), scribus_settextstroke, METH_VARARGS, tr(scribus_settextstroke__doc__)},
	{const_cast<char*>("setUnit"), scribus_setunit, METH_VARARGS, tr(scribus_setunit__doc__)},
	{const_cast<char*>("setVGuides"), scribus_setVguides, METH_VARARGS, tr(scribus_setVguides__doc__)},
	{const_cast<char*>("sizeObject"), scribus_sizeobjabs, METH_VARARGS, tr(scribus_sizeobjabs__doc__)},
	{const_cast<char*>("statusMessage"), scribus_messagebartext, METH_VARARGS, tr(scribus_messagebartext__doc__)},
	{const_cast<char*>("textFlowsAroundFrame"), scribus_textflow, METH_VARARGS, tr(scribus_textflow__doc__)},
	{const_cast<char*>("traceText"), scribus_tracetext, METH_VARARGS, tr(scribus_tracetext__doc__)},
	{const_cast<char*>("unGroupObject"), scribus_ungroupobj, METH_VARARGS, tr(scribus_ungroupobj__doc__)},
	{const_cast<char*>("unlinkTextFrames"), scribus_unlinktextframes, METH_VARARGS, tr(scribus_unlinktextframes__doc__)},
	{const_cast<char*>("valueDialog"), scribus_valdialog, METH_VARARGS, tr(scribus_valdialog__doc__)},
	// Functions for use by extension scripts
	{const_cast<char*>("register_macro_callable"), (PyCFunction)register_macro_callable, METH_KEYWORDS, tr(register_macro_callable__doc__)},
	{const_cast<char*>("register_macro_code"), (PyCFunction)register_macro_code, METH_KEYWORDS, tr(register_macro_code__doc__)},
	{const_cast<char*>("unregister_macro"), (PyCFunction)unregister_macro, METH_KEYWORDS, const_cast<char*>("Un-register a macro")},
	// Private functions for use by the scripter API its self
	{const_cast<char*>("_retval"), scribus_retval, METH_VARARGS, const_cast<char*>("Scribus internal API for console.")},
	{const_cast<char*>("_getval"), (PyCFunction)scribus_getval, METH_NOARGS, const_cast<char*>("Scribus internal API for console.")},
	{NULL, (PyCFunction)(0), 0, NULL} /* sentinel */
};

void initscribus(ScribusApp *pl)
{
	if (!scripterCore)
	{
		qDebug("initscribus called but no scripterCore");
		return;
	}
	PyObject *m, *d;
	PyImport_AddModule((char*)"scribus");
	PyType_Ready(&Printer_Type);
	PyType_Ready(&PDFfile_Type);
	PyType_Ready(&ImageExport_Type);
	m = Py_InitModule((char*)"scribus", scribus_methods);
	Py_INCREF(&Printer_Type);
	PyModule_AddObject(m, (char*)"Printer", (PyObject *) &Printer_Type);
	Py_INCREF(&PDFfile_Type);
	PyModule_AddObject(m, (char*)"PDFfile", (PyObject *) &PDFfile_Type);
	Py_INCREF(&ImageExport_Type);
	PyModule_AddObject(m, (char*)"ImageExport", (PyObject *) &ImageExport_Type);
	d = PyModule_GetDict(m);

	// Set up the module exceptions
	// common exc.
	ScribusException = PyErr_NewException((char*)"scribus.ScribusException", NULL, NULL);
	Py_INCREF(ScribusException);
	PyModule_AddObject(m, (char*)"ScribusException", ScribusException);
	// no doc open
	NoDocOpenError = PyErr_NewException((char*)"scribus.NoDocOpenError", ScribusException, NULL);
	Py_INCREF(NoDocOpenError);
	PyModule_AddObject(m, (char*)"NoDocOpenError", NoDocOpenError);
	// wrong type of frame for operation
	WrongFrameTypeError = PyErr_NewException((char*)"scribus.WrongFrameTypeError", ScribusException, NULL);
	Py_INCREF(WrongFrameTypeError);
	PyModule_AddObject(m, (char*)"WrongFrameTypeError", WrongFrameTypeError);
	// Couldn't find named object, or no named object and no selection
	NoValidObjectError = PyErr_NewException((char*)"scribus.NoValidObjectError", ScribusException, NULL);
	Py_INCREF(NoValidObjectError);
	PyModule_AddObject(m, (char*)"NoValidObjectError", NoValidObjectError);
	// Couldn't find the specified resource - font, color, etc.
	NotFoundError = PyErr_NewException((char*)"scribus.NotFoundError", ScribusException, NULL);
	Py_INCREF(NotFoundError);
	PyModule_AddObject(m, (char*)"NotFoundError", NotFoundError);
	// Tried to create an object with the same name as one that already exists
	NameExistsError = PyErr_NewException((char*)"scribus.NameExistsError", ScribusException, NULL);
	Py_INCREF(NameExistsError);
	PyModule_AddObject(m, (char*)"NameExistsError", NameExistsError);
	// Tried to run an extension only function with python extensions disabled or from a normal script
	AccessDeniedError = PyErr_NewException((char*)"scribus.AccessDeniedError", ScribusException, NULL);
	Py_INCREF(AccessDeniedError);
	PyModule_AddObject(m, (char*)"AccessDeniedError", AccessDeniedError);
	// Done with exception setup

	// CONSTANTS
	PyDict_SetItemString(d, const_cast<char*>("UNIT_POINTS"), Py_BuildValue(const_cast<char*>("i"), 0));
	PyDict_SetItemString(d, const_cast<char*>("UNIT_MILLIMETERS"), Py_BuildValue(const_cast<char*>("i"), 1));
	PyDict_SetItemString(d, const_cast<char*>("UNIT_INCHES"), Py_BuildValue(const_cast<char*>("i"), 2));
	PyDict_SetItemString(d, const_cast<char*>("UNIT_PICAS"), Py_BuildValue(const_cast<char*>("i"), 3));
	PyDict_SetItemString(d, const_cast<char*>("PORTRAIT"), Py_BuildValue(const_cast<char*>("i"), 0));
	PyDict_SetItemString(d, const_cast<char*>("LANDSCAPE"), Py_BuildValue(const_cast<char*>("i"), 1));
	PyDict_SetItemString(d, const_cast<char*>("NOFACINGPAGES"), Py_BuildValue(const_cast<char*>("i"), 0));
	PyDict_SetItemString(d, const_cast<char*>("FACINGPAGES"),  Py_BuildValue(const_cast<char*>("i"), 1));
	PyDict_SetItemString(d, const_cast<char*>("FIRSTPAGERIGHT"), Py_BuildValue(const_cast<char*>("i"), 0));
	PyDict_SetItemString(d, const_cast<char*>("FIRSTPAGELEFT"), Py_BuildValue(const_cast<char*>("i"), 1));
	PyDict_SetItemString(d, const_cast<char*>("ALIGN_LEFT"), Py_BuildValue(const_cast<char*>("i"), 0));
	PyDict_SetItemString(d, const_cast<char*>("ALIGN_RIGHT"), Py_BuildValue(const_cast<char*>("i"), 2));
	PyDict_SetItemString(d, const_cast<char*>("ALIGN_CENTERED"), Py_BuildValue(const_cast<char*>("i"), 1));
	PyDict_SetItemString(d, const_cast<char*>("ALIGN_BLOCK"), Py_BuildValue(const_cast<char*>("i"), 4));
	PyDict_SetItemString(d, const_cast<char*>("ALIGN_FORCED"), Py_BuildValue(const_cast<char*>("i"), 4));
	PyDict_SetItemString(d, const_cast<char*>("FILL_NOG"), Py_BuildValue(const_cast<char*>("i"), 0));
	PyDict_SetItemString(d, const_cast<char*>("FILL_HORIZONTALG"), Py_BuildValue(const_cast<char*>("i"), 1));
	PyDict_SetItemString(d, const_cast<char*>("FILL_VERTICALG"), Py_BuildValue(const_cast<char*>("i"), 2));
	PyDict_SetItemString(d, const_cast<char*>("FILL_DIAGONALG"), Py_BuildValue(const_cast<char*>("i"), 3));
	PyDict_SetItemString(d, const_cast<char*>("FILL_CROSSDIAGONALG"), Py_BuildValue(const_cast<char*>("i"), 4));
	PyDict_SetItemString(d, const_cast<char*>("FILL_RADIALG"), Py_BuildValue(const_cast<char*>("i"), 5));
	PyDict_SetItemString(d, const_cast<char*>("LINE_SOLID"), Py_BuildValue(const_cast<char*>("i"), Qt::SolidLine));
	PyDict_SetItemString(d, const_cast<char*>("LINE_DASH"), Py_BuildValue(const_cast<char*>("i"), Qt::DashLine));
	PyDict_SetItemString(d, const_cast<char*>("LINE_DOT"), Py_BuildValue(const_cast<char*>("i"), Qt::DotLine));
	PyDict_SetItemString(d, const_cast<char*>("LINE_DASHDOT"), Py_BuildValue(const_cast<char*>("i"), Qt::DashDotLine));
	PyDict_SetItemString(d, const_cast<char*>("LINE_DASHDOTDOT"), Py_BuildValue(const_cast<char*>("i"), Qt::DashDotDotLine));
	PyDict_SetItemString(d, const_cast<char*>("JOIN_MITTER"), Py_BuildValue(const_cast<char*>("i"), Qt::MiterJoin));
	PyDict_SetItemString(d, const_cast<char*>("JOIN_BEVEL"), Py_BuildValue(const_cast<char*>("i"), Qt::BevelJoin));
	PyDict_SetItemString(d, const_cast<char*>("JOIN_ROUND"), Py_BuildValue(const_cast<char*>("i"), Qt::RoundJoin));
	PyDict_SetItemString(d, const_cast<char*>("CAP_FLAT"), Py_BuildValue(const_cast<char*>("i"), Qt::FlatCap));
	PyDict_SetItemString(d, const_cast<char*>("CAP_SQUARE"), Py_BuildValue(const_cast<char*>("i"), Qt::SquareCap));
	PyDict_SetItemString(d, const_cast<char*>("CAP_ROUND"), Py_BuildValue(const_cast<char*>("i"), Qt::RoundCap));
	PyDict_SetItemString(d, const_cast<char*>("BUTTON_NONE"), Py_BuildValue(const_cast<char*>("i"), QMessageBox::NoButton));
	PyDict_SetItemString(d, const_cast<char*>("BUTTON_OK"), Py_BuildValue(const_cast<char*>("i"), QMessageBox::Ok));
	PyDict_SetItemString(d, const_cast<char*>("BUTTON_CANCEL"), Py_BuildValue(const_cast<char*>("i"), QMessageBox::Cancel));
	PyDict_SetItemString(d, const_cast<char*>("BUTTON_YES"), Py_BuildValue(const_cast<char*>("i"), QMessageBox::Yes));
	PyDict_SetItemString(d, const_cast<char*>("BUTTON_NO"), Py_BuildValue(const_cast<char*>("i"), QMessageBox::No));
	PyDict_SetItemString(d, const_cast<char*>("BUTTON_ABORT"), Py_BuildValue(const_cast<char*>("i"), QMessageBox::Abort));
	PyDict_SetItemString(d, const_cast<char*>("BUTTON_RETRY"), Py_BuildValue(const_cast<char*>("i"), QMessageBox::Retry));
	PyDict_SetItemString(d, const_cast<char*>("BUTTON_IGNORE"), Py_BuildValue(const_cast<char*>("i"), QMessageBox::Ignore));
	PyDict_SetItemString(d, const_cast<char*>("ICON_NONE"), Py_BuildValue(const_cast<char*>("i"), QMessageBox::NoIcon));
	PyDict_SetItemString(d, const_cast<char*>("ICON_INFORMATION"), Py_BuildValue(const_cast<char*>("i"), QMessageBox::Information));
	PyDict_SetItemString(d, const_cast<char*>("ICON_WARNING"), Py_BuildValue(const_cast<char*>("i"), QMessageBox::Warning));
	PyDict_SetItemString(d, const_cast<char*>("ICON_CRITICAL"), Py_BuildValue(const_cast<char*>("i"), QMessageBox::Critical));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_A0"), Py_BuildValue(const_cast<char*>("(ff)"), 2380.0, 3368.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_A1"), Py_BuildValue(const_cast<char*>("(ff)"), 1684.0, 2380.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_A2"), Py_BuildValue(const_cast<char*>("(ff)"), 1190.0, 1684.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_A3"), Py_BuildValue(const_cast<char*>("(ff)"), 842.0, 1190.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_A4"), Py_BuildValue(const_cast<char*>("(ff)"), 595.0, 842.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_A5"), Py_BuildValue(const_cast<char*>("(ff)"), 421.0, 595.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_A6"), Py_BuildValue(const_cast<char*>("(ff)"), 297.0, 421.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_A7"), Py_BuildValue(const_cast<char*>("(ff)"), 210.0, 297.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_A8"), Py_BuildValue(const_cast<char*>("(ff)"), 148.0, 210.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_A9"), Py_BuildValue(const_cast<char*>("(ff)"), 105.0, 148.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_B0"), Py_BuildValue(const_cast<char*>("(ff)"), 2836.0, 4008.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_B1"), Py_BuildValue(const_cast<char*>("(ff)"), 2004.0, 2836.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_B2"), Py_BuildValue(const_cast<char*>("(ff)"), 1418.0, 2004.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_B3"), Py_BuildValue(const_cast<char*>("(ff)"), 1002.0, 1418.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_B4"), Py_BuildValue(const_cast<char*>("(ff)"), 709.0, 1002.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_B5"), Py_BuildValue(const_cast<char*>("(ff)"), 501.0, 709.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_B6"), Py_BuildValue(const_cast<char*>("(ff)"), 355.0, 501.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_B7"), Py_BuildValue(const_cast<char*>("(ff)"), 250.0, 355.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_B8"), Py_BuildValue(const_cast<char*>("(ff)"), 178.0, 250.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_B9"), Py_BuildValue(const_cast<char*>("(ff)"), 125.0, 178.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_B10"), Py_BuildValue(const_cast<char*>("(ff)"), 89.0, 125.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_C5E"), Py_BuildValue(const_cast<char*>("(ff)"), 462.0, 649.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_COMM10E"), Py_BuildValue(const_cast<char*>("(ff)"), 298.0, 683.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_DLE"),  Py_BuildValue(const_cast<char*>("(ff)"), 312.0, 624.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_EXECUTIVE"), Py_BuildValue(const_cast<char*>("(ff)"), 542.0, 720.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_FOLIO"), Py_BuildValue(const_cast<char*>("(ff)"), 595.0, 935.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_LEDGER"), Py_BuildValue(const_cast<char*>("(ff)"), 1224.0, 792.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_LEGAL"), Py_BuildValue(const_cast<char*>("(ff)"), 612.0, 1008.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_LETTER"), Py_BuildValue(const_cast<char*>("(ff)"), 612.0, 792.0));
	PyDict_SetItemString(d, const_cast<char*>("PAPER_TABLOID"), Py_BuildValue(const_cast<char*>("(ff)"), 792.0, 1224.0));

	// Export the Scribus version into the module namespace so scripts know what they're running in
	PyDict_SetItemString(d, const_cast<char*>("scribus_version"), PyString_FromString(const_cast<char*>(VERSION)));
	// Now build a version tuple like that provided by Python in sys.version_info
	// The tuple is of the form (major, minor, patchlevel, extraversion, reserved)
	QRegExp version_re("(\\d+)\\.(\\d+)\\.(\\d+)(.*)");
	int pos = version_re.search(QString(VERSION));
	// We ignore errors, causing the scribus_version_info attribute to simply not be created.
	// This will make acceses raise AttrbuteError.
	if (pos > -1)
	{
		int majorVersion = version_re.cap(1).toInt();
		int minorVersion = version_re.cap(2).toInt();
		int patchVersion = version_re.cap(3).toInt();
		QString extraVersion = version_re.cap(4);
		PyObject* versionTuple = Py_BuildValue(const_cast<char*>("(iiisi)"),\
				majorVersion, minorVersion, patchVersion, (const char*)extraVersion.utf8(), 0);
		if (versionTuple != NULL)
				PyDict_SetItemString(d, const_cast<char*>("scribus_version_info"), versionTuple);
		else
				qDebug("Failed to build version tuple for version string '%s' in scripter", VERSION);
	}
	else
			qDebug("Couldn't parse version string '%s' in scripter", VERSION);

	// Push some build defines into the scribus module so scripts can see them.
	PyDict_SetItemString(d, const_cast<char*>("LIBDIR"),
		PyString_FromString(const_cast<char*>(LIBDIR)));
	PyDict_SetItemString(d, const_cast<char*>("SAMPLESDIR"),
		PyString_FromString(const_cast<char*>(SAMPLESDIR)));
	PyDict_SetItemString(d, const_cast<char*>("SCRIPTSDIR"),
		PyString_FromString(const_cast<char*>(SCRIPTSDIR)));
	PyDict_SetItemString(d, const_cast<char*>("ICONDIR"),
		PyString_FromString(const_cast<char*>(ICONDIR)));
	PyDict_SetItemString(d, const_cast<char*>("DOCDIR"),
		PyString_FromString(const_cast<char*>(DOCDIR)));
	PyDict_SetItemString(d, const_cast<char*>("TEMPLATEDIR"),
		PyString_FromString(const_cast<char*>(TEMPLATEDIR)));
	PyDict_SetItemString(d, const_cast<char*>("PLUGINDIR"),
		PyString_FromString(const_cast<char*>(PLUGINDIR)));

	Carrier = pl;
	// Function aliases for compatibility
	// We need to import the __builtins__, warnings and exceptions modules to be able to run
	// the generated Python functions from inside the `scribus' module's context.
	// This code makes it possible to extend the `scribus' module by running Python code
	// from C in other ways too.
	PyObject* builtinModule = PyImport_ImportModuleEx(const_cast<char*>("__builtin__"),
			d, d, Py_BuildValue(const_cast<char*>("[]")));
	if (builtinModule == NULL)
	{
		qDebug("Failed to import __builtin__ module. Something is probably broken with your Python.");
		return;
	}
	PyDict_SetItemString(d, const_cast<char*>("__builtin__"), builtinModule);
	PyObject* exceptionsModule = PyImport_ImportModuleEx(const_cast<char*>("exceptions"),
			d, d, Py_BuildValue(const_cast<char*>("[]")));
	if (exceptionsModule == NULL)
	{
		qDebug("Failed to import exceptions module. Something is probably broken with your Python.");
		return;
	}
	PyDict_SetItemString(d, const_cast<char*>("exceptions"), exceptionsModule);
	PyObject* warningsModule = PyImport_ImportModuleEx(const_cast<char*>("warnings"),
			d, d, Py_BuildValue(const_cast<char*>("[]")));
	if (warningsModule == NULL)
	{
		qDebug("Failed to import warnings module. Something is probably broken with your Python.");
		return;
	}
	PyDict_SetItemString(d, const_cast<char*>("warnings"), warningsModule);
	// Now actually add the aliases
	if (scripterCore->legacyAliases)
	{
		deprecatedFunctionAlias(d, const_cast<char*>("changeColor"), const_cast<char*>("ChangeColor"));
		deprecatedFunctionAlias(d, const_cast<char*>("closeDoc"), const_cast<char*>("CloseDoc"));
		deprecatedFunctionAlias(d, const_cast<char*>("createBezierLine"), const_cast<char*>("CreateBezierLine"));
		deprecatedFunctionAlias(d, const_cast<char*>("createEllipse"), const_cast<char*>("CreateEllipse"));
		deprecatedFunctionAlias(d, const_cast<char*>("createImage"), const_cast<char*>("CreateImage"));
		deprecatedFunctionAlias(d, const_cast<char*>("createLayer"), const_cast<char*>("CreateLayer"));
		deprecatedFunctionAlias(d, const_cast<char*>("createLine"), const_cast<char*>("CreateLine"));
		deprecatedFunctionAlias(d, const_cast<char*>("createPathText"), const_cast<char*>("CreatePathText"));
		deprecatedFunctionAlias(d, const_cast<char*>("createPolygon"), const_cast<char*>("CreatePolygon"));
		deprecatedFunctionAlias(d, const_cast<char*>("createPolyLine"), const_cast<char*>("CreatePolyLine"));
		deprecatedFunctionAlias(d, const_cast<char*>("createRect"), const_cast<char*>("CreateRect"));
		deprecatedFunctionAlias(d, const_cast<char*>("createText"), const_cast<char*>("CreateText"));
		deprecatedFunctionAlias(d, const_cast<char*>("currentPage"), const_cast<char*>("CurrentPage"));
		deprecatedFunctionAlias(d, const_cast<char*>("defineColor"), const_cast<char*>("DefineColor"));
		deprecatedFunctionAlias(d, const_cast<char*>("deleteColor"), const_cast<char*>("DeleteColor"));
		deprecatedFunctionAlias(d, const_cast<char*>("deleteLayer"), const_cast<char*>("DeleteLayer"));
		deprecatedFunctionAlias(d, const_cast<char*>("deleteObject"), const_cast<char*>("DeleteObject"));
		deprecatedFunctionAlias(d, const_cast<char*>("deletePage"), const_cast<char*>("DeletePage"));
		deprecatedFunctionAlias(d, const_cast<char*>("deleteText"), const_cast<char*>("DeleteText"));
		deprecatedFunctionAlias(d, const_cast<char*>("deselectAll"), const_cast<char*>("DeselectAll"));
		deprecatedFunctionAlias(d, const_cast<char*>("docChanged"), const_cast<char*>("DocChanged"));
		deprecatedFunctionAlias(d, const_cast<char*>("fileDialog"), const_cast<char*>("FileDialog"));
		deprecatedFunctionAlias(d, const_cast<char*>("getActiveLayer"), const_cast<char*>("GetActiveLayer"));
		deprecatedFunctionAlias(d, const_cast<char*>("getAllObjects"), const_cast<char*>("GetAllObjects"));
		deprecatedFunctionAlias(d, const_cast<char*>("getAllStyles"), const_cast<char*>("GetAllStyles"));
		deprecatedFunctionAlias(d, const_cast<char*>("getAllText"), const_cast<char*>("GetAllText"));
		deprecatedFunctionAlias(d, const_cast<char*>("getColor"), const_cast<char*>("GetColor"));
		deprecatedFunctionAlias(d, const_cast<char*>("getColorNames"), const_cast<char*>("GetColorNames"));
		deprecatedFunctionAlias(d, const_cast<char*>("getColumnGap"), const_cast<char*>("GetColumnGap"));
		deprecatedFunctionAlias(d, const_cast<char*>("getColumns"), const_cast<char*>("GetColumns"));
		deprecatedFunctionAlias(d, const_cast<char*>("getCornerRadius"), const_cast<char*>("GetCornerRadius"));
		deprecatedFunctionAlias(d, const_cast<char*>("getFillColor"), const_cast<char*>("GetFillColor"));
		deprecatedFunctionAlias(d, const_cast<char*>("getFillShade"), const_cast<char*>("GetFillShade"));
		deprecatedFunctionAlias(d, const_cast<char*>("getFont"), const_cast<char*>("GetFont"));
		deprecatedFunctionAlias(d, const_cast<char*>("getFontNames"), const_cast<char*>("GetFontNames"));
		deprecatedFunctionAlias(d, const_cast<char*>("getFontSize"), const_cast<char*>("GetFontSize"));
		deprecatedFunctionAlias(d, const_cast<char*>("getGuiLanguage"), const_cast<char*>("GetGuiLanguage"));
		deprecatedFunctionAlias(d, const_cast<char*>("getHGuides"), const_cast<char*>("GetHGuides"));
		deprecatedFunctionAlias(d, const_cast<char*>("getImageFile"), const_cast<char*>("GetImageFile"));
		deprecatedFunctionAlias(d, const_cast<char*>("getImageScale"), const_cast<char*>("GetImageScale"));
		deprecatedFunctionAlias(d, const_cast<char*>("getLayers"), const_cast<char*>("GetLayers"));
		deprecatedFunctionAlias(d, const_cast<char*>("getLineCap"), const_cast<char*>("GetLineCap"));
		deprecatedFunctionAlias(d, const_cast<char*>("getLineColor"), const_cast<char*>("GetLineColor"));
		deprecatedFunctionAlias(d, const_cast<char*>("getLineJoin"), const_cast<char*>("GetLineJoin"));
		deprecatedFunctionAlias(d, const_cast<char*>("getLineShade"), const_cast<char*>("GetLineShade"));
		deprecatedFunctionAlias(d, const_cast<char*>("getLineSpacing"), const_cast<char*>("GetLineSpacing"));
		deprecatedFunctionAlias(d, const_cast<char*>("getLineStyle"), const_cast<char*>("GetLineStyle"));
		deprecatedFunctionAlias(d, const_cast<char*>("getLineWidth"), const_cast<char*>("GetLineWidth"));
		deprecatedFunctionAlias(d, const_cast<char*>("getPageItems"), const_cast<char*>("GetPageItems"));
		deprecatedFunctionAlias(d, const_cast<char*>("getPageMargins"), const_cast<char*>("GetPageMargins"));
		deprecatedFunctionAlias(d, const_cast<char*>("getPageSize"), const_cast<char*>("GetPageSize"));
		deprecatedFunctionAlias(d, const_cast<char*>("getPosition"), const_cast<char*>("GetPosition"));
		deprecatedFunctionAlias(d, const_cast<char*>("getRotation"), const_cast<char*>("GetRotation"));
		deprecatedFunctionAlias(d, const_cast<char*>("getSelectedObject"), const_cast<char*>("GetSelectedObject"));
		deprecatedFunctionAlias(d, const_cast<char*>("getSize"), const_cast<char*>("GetSize"));
		deprecatedFunctionAlias(d, const_cast<char*>("getText"), const_cast<char*>("GetText"));
		deprecatedFunctionAlias(d, const_cast<char*>("getTextColor"), const_cast<char*>("GetTextColor"));
		deprecatedFunctionAlias(d, const_cast<char*>("getTextLength"), const_cast<char*>("GetTextLength"));
		deprecatedFunctionAlias(d, const_cast<char*>("getTextShade"), const_cast<char*>("GetTextShade"));
		deprecatedFunctionAlias(d, const_cast<char*>("getUnit"), const_cast<char*>("GetUnit"));
		deprecatedFunctionAlias(d, const_cast<char*>("getVGuides"), const_cast<char*>("GetVGuides"));
		deprecatedFunctionAlias(d, const_cast<char*>("getXFontNames"), const_cast<char*>("GetXFontNames"));
		deprecatedFunctionAlias(d, const_cast<char*>("gotoPage"), const_cast<char*>("GotoPage"));
		deprecatedFunctionAlias(d, const_cast<char*>("groupObjects"), const_cast<char*>("GroupObjects"));
		deprecatedFunctionAlias(d, const_cast<char*>("haveDoc"), const_cast<char*>("HaveDoc"));
		deprecatedFunctionAlias(d, const_cast<char*>("insertText"), const_cast<char*>("InsertText"));
		deprecatedFunctionAlias(d, const_cast<char*>("isLayerPrintable"), const_cast<char*>("IsLayerPrintable"));
		deprecatedFunctionAlias(d, const_cast<char*>("isLayerVisible"), const_cast<char*>("IsLayerVisible"));
		deprecatedFunctionAlias(d, const_cast<char*>("isLocked"), const_cast<char*>("IsLocked"));
		deprecatedFunctionAlias(d, const_cast<char*>("linkTextFrames"), const_cast<char*>("LinkTextFrames"));
		deprecatedFunctionAlias(d, const_cast<char*>("loadImage"), const_cast<char*>("LoadImage"));
		deprecatedFunctionAlias(d, const_cast<char*>("loadStylesFromFile"), const_cast<char*>("LoadStylesFromFile"));
		deprecatedFunctionAlias(d, const_cast<char*>("lockObject"), const_cast<char*>("LockObject"));
		deprecatedFunctionAlias(d, const_cast<char*>("messagebarText"), const_cast<char*>("MessagebarText"));
		deprecatedFunctionAlias(d, const_cast<char*>("messageBox"), const_cast<char*>("MessageBox"));
		deprecatedFunctionAlias(d, const_cast<char*>("moveObject"), const_cast<char*>("MoveObject"));
		deprecatedFunctionAlias(d, const_cast<char*>("moveObjectAbs"), const_cast<char*>("MoveObjectAbs"));
		deprecatedFunctionAlias(d, const_cast<char*>("newDoc"), const_cast<char*>("NewDoc"));
		deprecatedFunctionAlias(d, const_cast<char*>("newDocDialog"), const_cast<char*>("NewDocDialog"));
		deprecatedFunctionAlias(d, const_cast<char*>("newPage"), const_cast<char*>("NewPage"));
		deprecatedFunctionAlias(d, const_cast<char*>("objectExists"), const_cast<char*>("ObjectExists"));
		deprecatedFunctionAlias(d, const_cast<char*>("openDoc"), const_cast<char*>("OpenDoc"));
		deprecatedFunctionAlias(d, const_cast<char*>("pageCount"), const_cast<char*>("PageCount"));
		deprecatedFunctionAlias(d, const_cast<char*>("pageDimension"), const_cast<char*>("PageDimension"));
		deprecatedFunctionAlias(d, const_cast<char*>("progressReset"), const_cast<char*>("ProgressReset"));
		deprecatedFunctionAlias(d, const_cast<char*>("progressSet"), const_cast<char*>("ProgressSet"));
		deprecatedFunctionAlias(d, const_cast<char*>("progressTotal"), const_cast<char*>("ProgressTotal"));
		deprecatedFunctionAlias(d, const_cast<char*>("redrawAll"), const_cast<char*>("RedrawAll"));
		deprecatedFunctionAlias(d, const_cast<char*>("renderFont"), const_cast<char*>("RenderFont"));
		deprecatedFunctionAlias(d, const_cast<char*>("replaceColor"), const_cast<char*>("ReplaceColor"));
		deprecatedFunctionAlias(d, const_cast<char*>("rotateObject"), const_cast<char*>("RotateObject"));
		deprecatedFunctionAlias(d, const_cast<char*>("rotateObjectAbs"), const_cast<char*>("RotateObjectAbs"));
		deprecatedFunctionAlias(d, const_cast<char*>("saveDoc"), const_cast<char*>("SaveDoc"));
		deprecatedFunctionAlias(d, const_cast<char*>("saveDocAs"), const_cast<char*>("SaveDocAs"));
		deprecatedFunctionAlias(d, const_cast<char*>("savePageAsEPS"), const_cast<char*>("SavePageAsEPS"));
		deprecatedFunctionAlias(d, const_cast<char*>("scaleGroup"), const_cast<char*>("ScaleGroup"));
		deprecatedFunctionAlias(d, const_cast<char*>("scaleImage"), const_cast<char*>("ScaleImage"));
		deprecatedFunctionAlias(d, const_cast<char*>("selectionCount"), const_cast<char*>("SelectionCount"));
		deprecatedFunctionAlias(d, const_cast<char*>("selectObject"), const_cast<char*>("SelectObject"));
		deprecatedFunctionAlias(d, const_cast<char*>("selectText"), const_cast<char*>("SelectText"));
		deprecatedFunctionAlias(d, const_cast<char*>("sentToLayer"), const_cast<char*>("SentToLayer"));
		deprecatedFunctionAlias(d, const_cast<char*>("setActiveLayer"), const_cast<char*>("SetActiveLayer"));
		deprecatedFunctionAlias(d, const_cast<char*>("setColumnGap"), const_cast<char*>("SetColumnGap"));
		deprecatedFunctionAlias(d, const_cast<char*>("setColumns"), const_cast<char*>("SetColumns"));
		deprecatedFunctionAlias(d, const_cast<char*>("setCornerRadius"), const_cast<char*>("SetCornerRadius"));
		deprecatedFunctionAlias(d, const_cast<char*>("setCursor"), const_cast<char*>("SetCursor"));
		deprecatedFunctionAlias(d, const_cast<char*>("setDocType"), const_cast<char*>("SetDocType"));
		deprecatedFunctionAlias(d, const_cast<char*>("setFillColor"), const_cast<char*>("SetFillColor"));
		deprecatedFunctionAlias(d, const_cast<char*>("setFillShade"), const_cast<char*>("SetFillShade"));
		deprecatedFunctionAlias(d, const_cast<char*>("setFont"), const_cast<char*>("SetFont"));
		deprecatedFunctionAlias(d, const_cast<char*>("setFontSize"), const_cast<char*>("SetFontSize"));
		deprecatedFunctionAlias(d, const_cast<char*>("setGradientFill"), const_cast<char*>("SetGradientFill"));
		deprecatedFunctionAlias(d, const_cast<char*>("setHGuides"), const_cast<char*>("SetHGuides"));
		deprecatedFunctionAlias(d, const_cast<char*>("setInfo"), const_cast<char*>("SetInfo"));
		deprecatedFunctionAlias(d, const_cast<char*>("setLayerPrintable"), const_cast<char*>("SetLayerPrintable"));
		deprecatedFunctionAlias(d, const_cast<char*>("setLayerVisible"), const_cast<char*>("SetLayerVisible"));
		deprecatedFunctionAlias(d, const_cast<char*>("setLineCap"), const_cast<char*>("SetLineCap"));
		deprecatedFunctionAlias(d, const_cast<char*>("setLineColor"), const_cast<char*>("SetLineColor"));
		deprecatedFunctionAlias(d, const_cast<char*>("setLineJoin"), const_cast<char*>("SetLineJoin"));
		deprecatedFunctionAlias(d, const_cast<char*>("setLineShade"), const_cast<char*>("SetLineShade"));
		deprecatedFunctionAlias(d, const_cast<char*>("setLineSpacing"), const_cast<char*>("SetLineSpacing"));
		deprecatedFunctionAlias(d, const_cast<char*>("setLineStyle"), const_cast<char*>("SetLineStyle"));
		deprecatedFunctionAlias(d, const_cast<char*>("setLineWidth"), const_cast<char*>("SetLineWidth"));
		deprecatedFunctionAlias(d, const_cast<char*>("setMargins"), const_cast<char*>("SetMargins"));
		deprecatedFunctionAlias(d, const_cast<char*>("setMultiLine"), const_cast<char*>("SetMultiLine"));
		deprecatedFunctionAlias(d, const_cast<char*>("setMultiLine"), const_cast<char*>("SetMultiLine"));
		deprecatedFunctionAlias(d, const_cast<char*>("setRedraw"), const_cast<char*>("SetRedraw"));
		deprecatedFunctionAlias(d, const_cast<char*>("setSelectedObject"), const_cast<char*>("SetSelectedObject"));
		deprecatedFunctionAlias(d, const_cast<char*>("setStyle"), const_cast<char*>("SetStyle"));
		deprecatedFunctionAlias(d, const_cast<char*>("setText"), const_cast<char*>("SetText"));
		deprecatedFunctionAlias(d, const_cast<char*>("setTextAlignment"), const_cast<char*>("SetTextAlignment"));
		deprecatedFunctionAlias(d, const_cast<char*>("setTextColor"), const_cast<char*>("SetTextColor"));
		deprecatedFunctionAlias(d, const_cast<char*>("setTextShade"), const_cast<char*>("SetTextShade"));
		deprecatedFunctionAlias(d, const_cast<char*>("setTextStroke"), const_cast<char*>("SetTextStroke"));
		deprecatedFunctionAlias(d, const_cast<char*>("setUnit"), const_cast<char*>("SetUnit"));
		deprecatedFunctionAlias(d, const_cast<char*>("setVGuides"), const_cast<char*>("SetVGuides"));
		deprecatedFunctionAlias(d, const_cast<char*>("sizeObject"), const_cast<char*>("SizeObject"));
		deprecatedFunctionAlias(d, const_cast<char*>("statusMessage"), const_cast<char*>("StatusMessage"));
		deprecatedFunctionAlias(d, const_cast<char*>("textFlowsAroundFrame"), const_cast<char*>("TextFlowsAroundFrame"));
		deprecatedFunctionAlias(d, const_cast<char*>("traceText"), const_cast<char*>("TraceText"));
		deprecatedFunctionAlias(d, const_cast<char*>("unGroupObject"), const_cast<char*>("UnGroupObject"));
		deprecatedFunctionAlias(d, const_cast<char*>("unlinkTextFrames"), const_cast<char*>("UnlinkTextFrames"));
		deprecatedFunctionAlias(d, const_cast<char*>("valueDialog"), const_cast<char*>("ValueDialog"));
	}
	// end function aliases
	// legacy constants - alas, we can't print warnings when these
	// are used.
	if (scripterCore->legacyAliases)
	{
		constantAlias(d, "UNIT_POINTS", "Points");
		constantAlias(d, "UNIT_MILLIMETERS", "Millimeters");
		constantAlias(d, "UNIT_INCHES", "Inches");
		constantAlias(d, "UNIT_PICAS", "Picas");
		constantAlias(d, "PORTRAIT", "Portrait");
		constantAlias(d, "LANDSCAPE", "Landscape");
		constantAlias(d, "NOFACINGPAGES", "NoFacingPages");
		constantAlias(d, "FACINGPAGES", "FacingPages");
		constantAlias(d, "FIRSTPAGERIGHT", "FirstPageRight");
		constantAlias(d, "FIRSTPAGELEFT", "FirstPageLeft");
		constantAlias(d, "ALIGN_LEFT", "LeftAlign");
		constantAlias(d, "ALIGN_RIGHT", "RightAlign");
		constantAlias(d, "ALIGN_CENTERED", "Centered");
		constantAlias(d, "ALIGN_FORCED", "Forced");
		constantAlias(d, "FILL_NOG", "NoGradient");
		constantAlias(d, "FILL_HORIZONTALG", "HorizontalGradient");
		constantAlias(d, "FILL_VERTICALG", "VerticalGradient");
		constantAlias(d, "FILL_DIAGONALG", "DiagonalGradient");
		constantAlias(d, "FILL_CROSSDIAGONALG", "CrossDiagonalGradient");
		constantAlias(d, "FILL_RADIALG", "RadialGradient");
		constantAlias(d, "LINE_SOLID", "SolidLine");
		constantAlias(d, "LINE_DASH", "DashLine");
		constantAlias(d, "LINE_DOT", "DotLine");
		constantAlias(d, "LINE_DASHDOT", "DashDotLine");
		constantAlias(d, "LINE_DASHDOTDOT", "DashDotDotLine");
		constantAlias(d, "JOIN_MITTER", "MiterJoin");
		constantAlias(d, "JOIN_BEVEL", "BevelJoin");
		constantAlias(d, "JOIN_ROUND", "RoundJoin");
		constantAlias(d, "CAP_FLAT", "FlatCap");
		constantAlias(d, "CAP_SQUARE", "SquareCap");
		constantAlias(d, "CAP_ROUND", "RoundCap");
		constantAlias(d, "BUTTON_NONE", "NoButton");
		constantAlias(d, "BUTTON_OK", "Ok");
		constantAlias(d, "BUTTON_CANCEL", "Cancel");
		constantAlias(d, "BUTTON_YES", "Yes");
		constantAlias(d, "BUTTON_NO", "No");
		constantAlias(d, "BUTTON_ABORT", "Abort");
		constantAlias(d, "BUTTON_RETRY", "Retry");
		constantAlias(d, "BUTTON_IGNORE", "Ignore");
		constantAlias(d, "ICON_NONE", "NoIcon");
		constantAlias(d, "ICON_INFORMATION", "Information");
		constantAlias(d, "ICON_WARNING", "Warning");
		constantAlias(d, "ICON_CRITICAL", "Critical");
		constantAlias(d, "PAPER_A0", "Paper_A0");
		constantAlias(d, "PAPER_A1", "Paper_A1");
		constantAlias(d, "PAPER_A2", "Paper_A2");
		constantAlias(d, "PAPER_A3", "Paper_A3");
		constantAlias(d, "PAPER_A4", "Paper_A4");
		constantAlias(d, "PAPER_A5", "Paper_A5");
		constantAlias(d, "PAPER_A6", "Paper_A6");
		constantAlias(d, "PAPER_A7", "Paper_A7");
		constantAlias(d, "PAPER_A8", "Paper_A8");
		constantAlias(d, "PAPER_A9", "Paper_A9");
		constantAlias(d, "PAPER_B0", "Paper_B0");
		constantAlias(d, "PAPER_B1", "Paper_B1");
		constantAlias(d, "PAPER_B2", "Paper_B2");
		constantAlias(d, "PAPER_B3", "Paper_B3");
		constantAlias(d, "PAPER_B4", "Paper_B4");
		constantAlias(d, "PAPER_B5", "Paper_B5");
		constantAlias(d, "PAPER_B6", "Paper_B6");
		constantAlias(d, "PAPER_B7", "Paper_B7");
		constantAlias(d, "PAPER_B8", "Paper_B8");
		constantAlias(d, "PAPER_B9", "Paper_B9");
		constantAlias(d, "PAPER_B10", "Paper_B10");
		constantAlias(d, "PAPER_C5E", "Paper_C5E");
		constantAlias(d, "PAPER_COMM10E", "Paper_Comm10E");
		constantAlias(d, "PAPER_DLE", "Paper_DLE");
		constantAlias(d, "PAPER_EXECUTIVE", "Paper_Executive");
		constantAlias(d, "PAPER_FOLIO", "Paper_Folio");
		constantAlias(d, "PAPER_LEDGER", "Paper_Ledger");
		constantAlias(d, "PAPER_LEGAL", "Paper_Legal");
		constantAlias(d, "PAPER_LETTER", "Paper_Letter");
		constantAlias(d, "PAPER_TABLOID", "Paper_Tabloid");
	}
	// end of deprecated cosntants

	// Create the module-level docstring. This can be a proper unicode string, unlike
	// the others, because we can just create a Unicode object and insert it in our
	// module dictionary.
	QString docstring = QObject::tr("Scribus Python interface module\n\
\n\
This module is the Python interface for Scribus. It provides functions\n\
to control scribus and to manipulate objects on the canvas. Each\n\
function is documented individually below.\n\
\n\
A few things are common across most of the interface.\n\
\n\
Most functions operate on frames. Frames are identified by their name,\n\
a string - they are not real Python objects. Many functions take an\n\
optional (non-keyword) parameter, a frame name.\n\
Many exceptions are also common across most functions. These are\n\
not currently documented in the docstring for each function.\n\
- Many functions will raise a NoDocOpenError if you try to use them\
without a document to operate on.\n\
- If you do not pass a frame name to a function that requires one,\
the function will use the currently selected frame, if any, or\
raise a NoValidObjectError if it can't find anything to operate\
on.\n\
- Many functions will raise WrongFrameTypeError if you try to use them\n\
on a frame type that they do not make sense with. For example, setting\n\
the text colour on a graphics frame doesn't make sense, and will result\n\
in this exception being raised.\n\
- Errors resulting from calls to the underlying Python API will be\n\
passed through unaltered. As such, the list of exceptions thrown by\n\
any function as provided here and in its docstring is incomplete.\n\
\n\
Details of what exceptions each function may throw are provided on the\n\
function's documentation.");

	// Py_UNICODE is a typedef for unsigned short
	PyObject* uniDocStr = Py_BuildValue(const_cast<char*>("u"), (Py_UNICODE*)(docstring.ucs2()));
	if (uniDocStr == NULL)
		qDebug("Failed to create module-level docstring object!");
	else
		PyDict_SetItemString(d, const_cast<char*>("__doc__"), uniDocStr);
	Py_DECREF(uniDocStr);
}


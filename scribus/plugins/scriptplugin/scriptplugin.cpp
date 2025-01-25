/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

// include cmdvar.h first, as it pulls in <Python.h>
#include "cmdannotations.h"
#include "cmdvar.h"
#include "cmdcell.h"
#include "cmdcolor.h"
#include "cmddialog.h"
#include "cmddoc.h"
#include "cmdgetprop.h"
#include "cmdgetsetprop.h"
#include "cmdmani.h"
#include "cmdmisc.h"
#include "cmdobj.h"
#include "cmdpage.h"
#include "cmdsetprop.h"
#include "cmdtable.h"
#include "cmdtext.h"
#include "cmdutil.h"
#include "cmdstyle.h"
#include "guiapp.h"
#include "objimageexport.h"
#include "objpdffile.h"
#include "objprinter.h"
#include "prefs_scripter.h"
#include "scconfig.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scribusstructs.h"
#include "scriptercore.h"
#include "scriptplugin.h"
#include "svgimport.h"
#include "ui/customfdialog.h"
#include "ui/helpbrowser.h"
#include "ui/propertiespalette.h"
#include "ui/scmwmenumanager.h"
#include "units.h"

#include "api/api_application.h"

#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>
#include <QPixmap>
#include <QWidget>
#include <QString>
//Added by qt3to4:
#include <QByteArray>
#include <cstdlib>
#include <iostream>


#ifdef HAVE_SCRIPTER2
extern void scripter2_init();
#endif

// Exceptions; visible from cmdvar.h, set up in initscribus()
PyObject* ScribusException;
PyObject* NoDocOpenError;
PyObject* WrongFrameTypeError;
PyObject* NoValidObjectError;
PyObject* NotFoundError;
PyObject* NameExistsError;

// Other extern variables defined in cmdvar.h
PyObject* wrappedMainWindow;
PyObject* wrappedQApp;
ScripterCore* scripterCore;


int scriptplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* scriptplugin_getPlugin()
{
	scripterCore = nullptr;
	ScriptPlugin* plug = new ScriptPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void scriptplugin_freePlugin(ScPlugin* plugin)
{
	ScriptPlugin* plug = dynamic_cast<ScriptPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ScriptPlugin::ScriptPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

ScriptPlugin::~ScriptPlugin() = default;

void ScriptPlugin::languageChange()
{
	if (scripterCore)
		scripterCore->languageChange();
}

void ScriptPlugin::addToMainWindowMenu(ScribusMainWindow* mw)
{
	if (scripterCore)
		scripterCore->addToMainWindowMenu(mw);
}

QString ScriptPlugin::fullTrName() const
{
	return QObject::tr("Scripter");
}

const ScActionPlugin::AboutData* ScriptPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8(
			"Petr Van\xc4\x9bk <petr@scribus.info>, "
			"Franz Schmid <franz@scribus.info>, "
			"Craig Ringer <craig@scribus.info>");
	about->shortDescription = tr("Embedded Python scripting support for Python 3.x.");
	// about->description = tr("Write me!")
	// about->releaseDate
	// about->copyright
	// about->license
	return about;
}

void ScriptPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool ScriptPlugin::initPlugin()
{
#if defined(Q_OS_WIN)
	QString pyHome = QApplication::applicationDirPath() + "/python";
	if (QDir(pyHome).exists())
	{
		QString ph = QDir::toNativeSeparators(pyHome);
		pythonHome.resize(2 * ph.length() + 2);
		memcpy(pythonHome.data(), ph.utf16(), 2 * ph.length() + 2);
		Py_SetPythonHome((const wchar_t*) pythonHome.constData());
	}
#endif

	scripterCore = new ScripterCore(ScCore->primaryMainWindow());
	Q_CHECK_PTR(scripterCore);

	PyImport_AppendInittab("scribus", &PyInit_scribus);
	Py_Initialize();

	//initscribus(ScCore->primaryMainWindow());
	
#ifdef HAVE_SCRIPTER2
	scripter2_init();
#endif
	scripterCore->setupMainInterpreter();
	scripterCore->initExtensionScripts();
	return true;
}

bool ScriptPlugin::cleanupPlugin()
{
	if (scripterCore)
	{
		delete scripterCore;
		scripterCore = nullptr;
	}
	Py_Finalize();
	return true;
}

bool ScriptPlugin::newPrefsPanelWidget(QWidget* parent, Prefs_Pane*& panel)
{
	panel = new Prefs_Scripter(parent);
	Q_CHECK_PTR(panel);
	connect(panel, SIGNAL(prefsChanged()), scripterCore, SLOT(updateSyntaxHighlighter()));
	return true;
}

/*  TEMPORARILY DISABLED
void run()
{
	QString pfad = ScPaths::instance().docDir();
	QString pfad2;
	pfad2 = QDir::toNativeSeparators(pfad + "en/Scripter/index.html");
	HelpBrowser *dia = new HelpBrowser(0, QObject::tr("Online Reference"), ScCore->primaryMainWindow()->getGuiLanguage(), "scripter");
	dia->show();
}
*/


/****************************************************************************************/
/*                                                                                      */
/*   Definitions of the Python commands                                                 */
/*                                                                                      */
/****************************************************************************************/

/*static */PyObject *scribus_retval(PyObject* /*self*/, PyObject* args)
{
	char *Name = nullptr;
	if (!PyArg_ParseTuple(args, "s", &Name))
		return nullptr;
	// Because sysdefaultencoding is not utf-8, Python is returning utf-8 encoded
	// 8-bit char* strings. Make sure Qt understands that the input is utf-8 not
	// the default local encoding (usually latin-1) by using QString::fromUtf8()
	/*RetString = QString::fromUtf8(Name);
	RetVal = retV;*/
	scripterCore->returnString = QString::fromUtf8(Name);
	return PyLong_FromLong(0L);
}

/*static */PyObject *scribus_getval(PyObject* /*self*/)
{
	return PyUnicode_FromString(scripterCore->inValue.toUtf8().data());
}

/*! \brief Translate a docstring. Small helper function for use with the
 * PyMethodDef struct.
 */
char* tr(const char* docstringConstant)
{
	// Alas, there's a lot of wasteful string copying going on
	// here.
	QString translated = QObject::tr(docstringConstant, "scripter docstring");
	// pv - hack for ugly formatting in console removing
	translated.replace("\n\n", "<P>");
	translated.replace('\n', " ");
	translated.replace("<P>", "\n\n");
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
	//QCString utfTranslated = translated.utf8();
	QByteArray trch = translated.toUtf8();
	char* utfstr = strdup(trch.data());
	if (!utfstr)
		// Complain, but then return NULL anyway. Python will treat NULL as
		// "no value" so that's fine.
		qDebug("scriptplugin.cpp:tr() - strdup() failure");
	return utfstr;
}

/* Now we're using the more pythonic convention for names:
 * class - ClassName
 * procedure/function/method - procedureName
 * etc. */
PyMethodDef scribus_methods[] = {
	// 2004/10/03 pv - aliases with common Python syntax - ClassName methodName
	// 2004-11-06 cr - move aliasing to dynamically generated wrapper functions, sort methoddef
	{ "applyMasterPage", scribus_applymasterpage, METH_VARARGS, tr(scribus_applymasterpage__doc__)},
	{ "changeColor", scribus_setcolor, METH_VARARGS, tr(scribus_setcolor__doc__)},
	{ "changeColorCMYK", scribus_setcolorcmyk, METH_VARARGS, tr(scribus_setcolorcmyk__doc__)},
	{ "changeColorCMYKFloat", scribus_setcolorcmykfloat, METH_VARARGS, tr(scribus_setcolorcmykfloat__doc__)},
	{ "changeColorLab", scribus_setcolorlab, METH_VARARGS, tr(scribus_setcolorlab__doc__) },
	{ "changeColorRGB", scribus_setcolorrgb, METH_VARARGS, tr(scribus_setcolorrgb__doc__)},
	{ "changeColorRGBFloat", scribus_setcolorrgbfloat, METH_VARARGS, tr(scribus_setcolorrgbfloat__doc__)},
	{ "closeDoc", (PyCFunction) scribus_closedoc, METH_NOARGS, tr(scribus_closedoc__doc__)},
	{ "closeMasterPage", (PyCFunction) scribus_closemasterpage, METH_NOARGS, tr(scribus_closemasterpage__doc__)},
	{ "createBezierLine", scribus_createbezierline, METH_VARARGS, tr(scribus_createbezierline__doc__)},
	{ "createCharStyle", (PyCFunction) scribus_createcharstyle, METH_VARARGS|METH_KEYWORDS, tr(scribus_createcharstyle__doc__)},
	{ "createCustomLineStyle", scribus_createcustomlinestyle, METH_VARARGS, tr(scribus_createcustomlinestyle__doc__)},
	{ "createEllipse", scribus_createellipse, METH_VARARGS, tr(scribus_createellipse__doc__)},
	{ "createImage", scribus_createimage, METH_VARARGS, tr(scribus_createimage__doc__)},
	{ "createLayer", scribus_createlayer, METH_VARARGS, tr(scribus_createlayer__doc__)},
	{ "createLine", scribus_createline, METH_VARARGS, tr(scribus_createline__doc__)},
	{ "createMasterPage", scribus_createmasterpage, METH_VARARGS, tr(scribus_createmasterpage__doc__)},
	{ "createParagraphStyle", (PyCFunction) scribus_createparagraphstyle, METH_VARARGS|METH_KEYWORDS, tr(scribus_createparagraphstyle__doc__)},
	{ "createPathText", scribus_createpathtext, METH_VARARGS, tr(scribus_createpathtext__doc__)},
	{ "createPolyLine", scribus_createpolyline, METH_VARARGS, tr(scribus_createpolyline__doc__)},
	{ "createPolygon", scribus_createpolygon, METH_VARARGS, tr(scribus_createpolygon__doc__)},
	{ "createRect", scribus_createrect, METH_VARARGS, tr(scribus_createrect__doc__)},
	{ "createTable", scribus_createtable, METH_VARARGS, tr(scribus_createtable__doc__)},
	{ "createText", scribus_createtext, METH_VARARGS, tr(scribus_createtext__doc__)},
	{ "currentPage", (PyCFunction) scribus_currentpage, METH_NOARGS, tr(scribus_currentpage__doc__)},
	{ "currentPageNumber", (PyCFunction) scribus_currentpage, METH_NOARGS, tr(scribus_currentpage__doc__)},
	{ "currentPageNumberForSection", (PyCFunction) scribus_currentpagenumberforsection, METH_NOARGS, tr(scribus_currentpagenumberforsection__doc__)},
	{ "defineColor", scribus_newcolor, METH_VARARGS, tr(scribus_newcolor__doc__)},
	{ "defineColorCMYK", scribus_newcolorcmyk, METH_VARARGS, tr(scribus_newcolorcmyk__doc__)},
	{ "defineColorCMYKFloat", scribus_newcolorcmykfloat, METH_VARARGS, tr(scribus_newcolorcmykfloat__doc__)},
	{ "defineColorLab", scribus_newcolorlab, METH_VARARGS, tr(scribus_newcolorlab__doc__) },
	{ "defineColorRGB", scribus_newcolorrgb, METH_VARARGS, tr(scribus_newcolorrgb__doc__)},
	{ "defineColorRGBFloat", scribus_newcolorrgb, METH_VARARGS, tr(scribus_newcolorrgbfloat__doc__)},
	{ "dehyphenateText", scribus_dehyphenatetext, METH_VARARGS, tr(scribus_dehyphenatetext__doc__)},
	{ "deleteColor", scribus_deletecolor, METH_VARARGS, tr(scribus_deletecolor__doc__)},
	{ "deleteLayer", scribus_deletelayer, METH_VARARGS, tr(scribus_deletelayer__doc__)},
	{ "deleteMasterPage", scribus_deletemasterpage, METH_VARARGS, tr(scribus_deletemasterpage__doc__)},
	{ "deleteObject", scribus_deleteobject, METH_VARARGS, tr(scribus_deleteobject__doc__)},
	{ "deletePage", scribus_deletepage, METH_VARARGS, tr(scribus_deletepage__doc__)},
	{ "deleteText", scribus_deletetext, METH_VARARGS, tr(scribus_deletetext__doc__)},
	{ "deselectAll", (PyCFunction) scribus_deselectall, METH_NOARGS, tr(scribus_deselectall__doc__)},
	{ "docChanged", scribus_docchanged, METH_VARARGS, tr(scribus_docchanged__doc__)},
	{ "editMasterPage", scribus_editmasterpage, METH_VARARGS, tr(scribus_editmasterpage__doc__)},
	{ "fileDialog", (PyCFunction) scribus_filedialog, METH_VARARGS|METH_KEYWORDS, tr(scribus_filedialog__doc__)},
	{ "fileQuit", scribus_filequit, METH_VARARGS, tr(scribus_filequit__doc__)},
	{ "flipObject", scribus_flipobject, METH_VARARGS, tr(scribus_flipobject__doc__)},
	{ "getActiveLayer", (PyCFunction) scribus_getactivelayer, METH_NOARGS, tr(scribus_getactivelayer__doc__)},
	{ "getAllObjects", (PyCFunction) scribus_getallobjects, METH_VARARGS|METH_KEYWORDS, tr(scribus_getallobjects__doc__)},
	{ "getAllStyles", (PyCFunction) scribus_getparagraphstyles, METH_NOARGS, tr(scribus_getallstyles__doc__)}, //Deprecated
	{ "getAllText", scribus_getalltext, METH_VARARGS, tr(scribus_getalltext__doc__)},
	{ "getBaseLine", (PyCFunction) scribus_getbaseline, METH_NOARGS, tr(scribus_getbaseline__doc__)},
	{ "getBleeds", scribus_getbleeds, METH_NOARGS, tr(scribus_getbleeds__doc__)},
	{ "getBoundingBox", scribus_getboundingbox, METH_VARARGS, tr(scribus_getboundingbox__doc__)},
	{ "getCellColumnSpan", scribus_getcellcolumnspan, METH_VARARGS, tr(scribus_getcellcolumnspan__doc__)},
	{ "getCellFillColor", scribus_getcellfillcolor, METH_VARARGS, tr(scribus_getcellfillcolor__doc__)},
	{ "getCellRowSpan", scribus_getcellrowspan, METH_VARARGS, tr(scribus_getcellrowspan__doc__)},
	{ "getCellStyle", scribus_getcellstyle, METH_VARARGS, tr(scribus_getcellstyle__doc__)},
	{ "getCellStyles", (PyCFunction) scribus_getcellstyles, METH_VARARGS, tr(scribus_getcellstyles__doc__)},
	{ "getCellText", scribus_getcelltext, METH_VARARGS, tr(scribus_getcelltext__doc__)},
	{ "getCharStyles", (PyCFunction) scribus_getcharstyles, METH_NOARGS, tr(scribus_getcharstyles__doc__)},
	{ "getCharacterStyle", scribus_getcharacterstyle, METH_VARARGS, tr(scribus_getcharacterstyle__doc__) },
	{ "getColor", scribus_getcolor, METH_VARARGS, tr(scribus_getcolor__doc__)},
	{ "getColorAsRGB", scribus_getcolorasrgb, METH_VARARGS, tr(scribus_getcolorasrgb__doc__)},
	{ "getColorAsRGBFloat", scribus_getcolorasrgbfloat, METH_VARARGS, tr(scribus_getcolorasrgbfloat__doc__)},
	{ "getColorFloat", scribus_getcolorfloat, METH_VARARGS, tr(scribus_getcolorfloat__doc__)},
	{ "getColorNames", (PyCFunction) scribus_getcolornames, METH_NOARGS, tr(scribus_getcolornames__doc__)},
	{ "getColumnGap", scribus_getcolumngap, METH_VARARGS, tr(scribus_getcolumngap__doc__)},
	{ "getColumns", scribus_getcolumns, METH_VARARGS, tr(scribus_getcolumns__doc__)},
	{ "getColumnGuides", (PyCFunction) scribus_getColumnGuides, METH_VARARGS|METH_KEYWORDS, tr(scribus_getColumnGuides__doc__)},
	{ "getCornerRadius", scribus_getcornerradius, METH_VARARGS, tr(scribus_getcornerradius__doc__)},
	{ "getCurrentPageSize", (PyCFunction) scribus_getcurrentpagesize, METH_NOARGS, tr(scribus_getcurrentpagesize__doc__)},
	{ "getCustomLineStyle", scribus_getcustomlinestyle, METH_VARARGS, tr(scribus_getcustomlinestyle__doc__)},
	{ "getDocName", (PyCFunction) scribus_getdocname, METH_NOARGS, tr(scribus_getdocname__doc__)},
	{ "getFillBlendmode", scribus_getfillblendmode, METH_VARARGS, tr(scribus_getfillblendmode__doc__)},
	{ "getFillColor", scribus_getfillcolor, METH_VARARGS, tr(scribus_getfillcolor__doc__)},
	{ "getFillShade", scribus_getfillshade, METH_VARARGS, tr(scribus_getfillshade__doc__)},
	{ "getFillTransparency", scribus_getfilltransparency, METH_VARARGS, tr(scribus_getfilltransparency__doc__)},
	{ "getFirstLineOffset", scribus_getfirstlineoffset, METH_VARARGS, tr(scribus_getfirstlineoffset__doc__)},
	{ "getFirstLinkedFrame", scribus_getfirstlinkedframe, METH_VARARGS, tr(scribus_getfirstlinkedframe__doc__)},
	{ "getFont", scribus_getfont, METH_VARARGS, tr(scribus_getfont__doc__)},
	{ "getFontFeatures", scribus_getfontfeatures, METH_VARARGS, tr(scribus_getfontfeatures__doc__)},
	{ "getFontNames", (PyCFunction) scribus_getfontnames, METH_NOARGS, tr(scribus_getfontnames__doc__)},
	{ "getFontSize", scribus_getfontsize, METH_VARARGS, tr(scribus_getfontsize__doc__)},
	{ "getFrameText", scribus_getframetext, METH_VARARGS, tr(scribus_getframetext__doc__)},
	{ "getFrameSelectedTextRange", scribus_getframeselectedtextrange, METH_VARARGS, tr(scribus_getframeselectedtextrange__doc__)},
	{ "getGradientStop", scribus_getgradstop, METH_VARARGS, tr(scribus_getgradstop__doc__)},
	{ "getGradientStopsCount", scribus_getgradstopscount, METH_VARARGS, tr(scribus_getgradstopscount__doc__)},
	{ "getGradientVector", scribus_getgradvector, METH_VARARGS, tr(scribus_getgradvector__doc__)},
	{ "getGroupItems", (PyCFunction) scribus_getGroupItems, METH_VARARGS|METH_KEYWORDS, tr(scribus_getGroupItems__doc__)},
	{ "getGuiLanguage", (PyCFunction) scribus_getlanguage, METH_NOARGS, tr(scribus_getlanguage__doc__)},
	{ "getHGuides", (PyCFunction) scribus_getHguides, METH_NOARGS, tr(scribus_getHguides__doc__)},
	{ "getImageColorSpace", scribus_getimagecolorspace, METH_VARARGS, tr(scribus_getimagecolorspace__doc__) },
	{ "getImageFile", scribus_getimagefile, METH_VARARGS, tr(scribus_getimagefile__doc__)},
	{ "getImageOffset", scribus_getimageoffset, METH_VARARGS, tr(scribus_getimageoffset__doc__)},
	{ "getImageScale", scribus_getimagescale, METH_VARARGS, tr(scribus_getimagescale__doc__)},
	{ "getInfo", (PyCFunction) scribus_getinfo, METH_VARARGS, tr(scribus_getinfo__doc__)},
	{ "getItemPageNumber", scribus_getitempagenumber, METH_VARARGS, tr(scribus_getitempagenumber__doc__)},
	{ "getLastLinkedFrame", scribus_getlastlinkedframe, METH_VARARGS, tr(scribus_getlastlinkedframe__doc__)},
	{ "getLayerBlendmode", scribus_getlayerblendmode, METH_VARARGS, tr(scribus_getlayerblendmode__doc__)},
	{ "getLayerTransparency", scribus_getlayertransparency, METH_VARARGS, tr(scribus_getlayertransparency__doc__)},
	{ "getLayers", (PyCFunction) scribus_getlayers, METH_NOARGS, tr(scribus_getlayers__doc__)},
	{ "getLineBlendmode", scribus_getlineblendmode, METH_VARARGS, tr(scribus_getlineblendmode__doc__)},
	{ "getLineCap", scribus_getlinecap, METH_VARARGS, tr(scribus_getlinecap__doc__)},
	{ "getLineColor", scribus_getlinecolor, METH_VARARGS, tr(scribus_getlinecolor__doc__)},
	{ "getLineJoin", scribus_getlinejoin, METH_VARARGS, tr(scribus_getlinejoin__doc__)},
	{ "getLineShade", scribus_getlineshade, METH_VARARGS, tr(scribus_getlineshade__doc__)},
	{ "getLineSpacing", scribus_getlinespacing, METH_VARARGS, tr(scribus_getlinespacing__doc__)},
	{ "getLineSpacingMode", scribus_getlinespacingmode, METH_VARARGS, tr(scribus_getlinespacingmode__doc__)},
	{ "getLineStyle", scribus_getlinestyle, METH_VARARGS, tr(scribus_getlinestyle__doc__)},
	{ "getLineStyles", (PyCFunction) scribus_getlinestyles, METH_VARARGS, tr(scribus_getlinestyles__doc__)},
	{ "getLineTransparency", scribus_getlinetransparency, METH_VARARGS, tr(scribus_getlinetransparency__doc__)},
	{ "getLineWidth", scribus_getlinewidth, METH_VARARGS, tr(scribus_getlinewidth__doc__)},
	{ "getMargins", scribus_getmargins, METH_NOARGS, tr(scribus_getmargins__doc__) },
	{ "getMasterPage", scribus_getmasterpage, METH_VARARGS, tr(scribus_getmasterpage__doc__)},
	{ "getNextLinkedFrame", scribus_getnextlinkedframe, METH_VARARGS, tr(scribus_getnextlinkedframe__doc__)},
	{ "getObjectAttributes", scribus_getobjectattributes, METH_VARARGS, tr(scribus_getobjectattributes__doc__)},
	{ "getObjectType", scribus_getobjecttype, METH_VARARGS, tr(scribus_getobjecttype__doc__)},
	{ "getPageItems", (PyCFunction) scribus_getpageitems, METH_NOARGS, tr(scribus_getpageitems__doc__)},
	{ "getPageMargins", (PyCFunction) scribus_getpagemargins, METH_NOARGS, tr(scribus_getpagemargins__doc__)},
	{ "getPageNMargins", scribus_getpagenmargins, METH_VARARGS, tr(scribus_getpagenmargins__doc__)},
	{ "getPageNSize", scribus_getpagensize, METH_VARARGS, tr(scribus_getpagensize__doc__)},
	{ "getPageSize", (PyCFunction) scribus_getpagesize, METH_NOARGS, tr(scribus_getpagesize__doc__)},
	{ "getPageType", (PyCFunction) scribus_getpagetype, METH_VARARGS, tr(scribus_getpagetype__doc__)},
	{ "getParagraphStyle", scribus_getparagraphstyle, METH_VARARGS, tr(scribus_getparagraphstyle__doc__) },
	{ "getParagraphStyles", (PyCFunction) scribus_getparagraphstyles, METH_NOARGS, tr(scribus_getparagraphstyles__doc__)},
	{ "getPosition", scribus_getposition, METH_VARARGS, tr(scribus_getposition__doc__)},
	{ "getPrevLinkedFrame", scribus_getprevlinkedframe, METH_VARARGS, tr(scribus_getprevlinkedframe__doc__)},
	{ "getRotation", scribus_getrotation, METH_VARARGS, tr(scribus_getrotation__doc__)},
	{ "getRowGuides", (PyCFunction) scribus_getRowGuides, METH_VARARGS|METH_KEYWORDS, tr(scribus_getRowGuides__doc__)},
	{ "getSelectedObject", scribus_getselectedobject, METH_VARARGS, tr(scribus_getselectedobject__doc__)},
	{ "getSelectedTextRange", scribus_getselectedtextrange, METH_VARARGS, tr(scribus_getselectedtextrange__doc__)},
	{ "getSize", scribus_getsize, METH_VARARGS, tr(scribus_getsize__doc__)},
	{ "getStyle", scribus_getparagraphstyle, METH_VARARGS, tr(scribus_getstyle__doc__) }, // Deprecated
	{ "getTableColumnWidth", scribus_gettablecolumnwidth, METH_VARARGS, tr(scribus_gettablecolumnwidth__doc__)},
	{ "getTableColumns", scribus_gettablecolumns, METH_VARARGS, tr(scribus_gettablecolumns__doc__)},
	{ "getTableFillColor", scribus_gettablefillcolor, METH_VARARGS, tr(scribus_gettablefillcolor__doc__)},
	{ "getTableRowHeight", scribus_gettablerowheight, METH_VARARGS, tr(scribus_gettablerowheight__doc__)},
	{ "getTableRows", scribus_gettablerows, METH_VARARGS, tr(scribus_gettablerows__doc__)},
	{ "getTableStyle", scribus_gettablestyle, METH_VARARGS, tr(scribus_gettablestyle__doc__)},
	{ "getTableStyles", (PyCFunction) scribus_gettablestyles, METH_VARARGS, tr(scribus_gettablestyles__doc__)},
	{ "getText", scribus_getframetext, METH_VARARGS, tr(scribus_gettext__doc__)}, // Deprecated
	{ "getTextColor", scribus_gettextcolor, METH_VARARGS, tr(scribus_gettextcolor__doc__)},
	{ "getTextDistances", scribus_gettextdistances, METH_VARARGS, tr(scribus_gettextdistances__doc__)},
	{ "getTextFlowMode", scribus_gettextflowmode, METH_VARARGS, tr(scribus_gettextflowmode__doc__)},
	{ "getTextLength", scribus_gettextlength, METH_VARARGS, tr(scribus_gettextlength__doc__)},
	{ "getTextLines", scribus_gettextlines, METH_VARARGS, tr(scribus_gettextlines__doc__)},
	{ "getTextShade", scribus_gettextshade, METH_VARARGS, tr(scribus_gettextshade__doc__)},
	{ "getTextVerticalAlignment", scribus_gettextverticalalignment, METH_VARARGS, tr(scribus_gettextverticalalignment__doc__)},
	{ "getTracking", scribus_gettracking, METH_VARARGS, tr(scribus_gettracking__doc__)},
	{ "getUnit", (PyCFunction) scribus_getunit, METH_NOARGS, tr(scribus_getunit__doc__)},
	{ "getVisualBoundingBox", scribus_getvisualboundingbox, METH_VARARGS, tr(scribus_getvisualboundingbox__doc__) },
	{ "getWordTracking", scribus_getwordtracking, METH_VARARGS, tr(scribus_getwordtracking__doc__)},
	{ "pointsToDocUnit", scribus_pointstodocunit, METH_VARARGS, tr(scribus_pointstodocunit__doc__)},
	{ "docUnitToPoints", scribus_docunittopoints, METH_VARARGS, tr(scribus_docunittopoints__doc__)},
	{ "stringValueToPoints", scribus_stringvaluetopoints, METH_VARARGS, tr(scribus_stringvaluetopoints__doc__)},
	{ "getVGuides", (PyCFunction) scribus_getVguides, METH_NOARGS, tr(scribus_getVguides__doc__)},
	{ "getXFontNames", (PyCFunction) scribus_xfontnames, METH_NOARGS, tr(scribus_xfontnames__doc__)},
	{ "gotoPage", scribus_gotopage, METH_VARARGS, tr(scribus_gotopage__doc__)},
	{ "groupObjects", (PyCFunction) scribus_groupobjects, METH_VARARGS, tr(scribus_groupobjects__doc__)},
	{ "haveDoc", (PyCFunction) scribus_havedoc, METH_NOARGS, tr(scribus_havedoc__doc__)},
	{ "hyphenateText", scribus_hyphenatetext, METH_VARARGS, tr(scribus_hyphenatetext__doc__)},
	{ "importPage", scribus_importpage, METH_VARARGS, tr(scribus_importpage__doc__)},
	{ "insertHtmlText", scribus_inserthtmltext, METH_VARARGS, tr(scribus_inserthtmltext__doc__)},
	{ "insertTableColumns", scribus_inserttablecolumns, METH_VARARGS, tr(scribus_inserttablecolumns__doc__)},
	{ "insertTableRows", scribus_inserttablerows, METH_VARARGS, tr(scribus_inserttablerows__doc__)},
	{ "insertText", scribus_inserttext, METH_VARARGS, tr(scribus_inserttext__doc__)},
	{ "isLayerFlow", scribus_islayerflow, METH_VARARGS, tr(scribus_islayerflow__doc__)},
	{ "isLayerLocked", scribus_islayerlocked, METH_VARARGS, tr(scribus_islayerlocked__doc__)},
	{ "isLayerOutlined", scribus_islayeroutlined, METH_VARARGS, tr(scribus_islayeroutlined__doc__)},
	{ "isLayerPrintable", scribus_islayerprintable, METH_VARARGS, tr(scribus_islayerprintable__doc__)},
	{ "isLayerVisible", scribus_islayervisible, METH_VARARGS, tr(scribus_islayervisible__doc__)},
	{ "isLocked", scribus_islocked, METH_VARARGS, tr(scribus_islocked__doc__)},
	{ "isPDFBookmark", scribus_ispdfbookmark, METH_VARARGS, tr(scribus_ispdfbookmark__doc__)},
	{ "isSpotColor", scribus_isspotcolor, METH_VARARGS, tr(scribus_isspotcolor__doc__)},
	{ "layoutText", scribus_layouttext, METH_VARARGS, tr(scribus_layouttext__doc__)},
	{ "layoutTextChain", scribus_layouttextchain, METH_VARARGS, tr(scribus_layouttextchain__doc__)},
	{ "linkTextFrames", scribus_linktextframes, METH_VARARGS, tr(scribus_linktextframes__doc__)},
	{ "loadImage", scribus_loadimage, METH_VARARGS, tr(scribus_loadimage__doc__)},
	{ "loadStylesFromFile", scribus_loadstylesfromfile, METH_VARARGS, tr(scribus_loadstylesfromfile__doc__)},
	{ "lockObject", scribus_lockobject, METH_VARARGS, tr(scribus_lockobject__doc__)},
	{ "lowerActiveLayer", (PyCFunction) scribus_loweractivelayer, METH_NOARGS, tr(scribus_loweractivelayer__doc__)},
	{ "masterPageNames", (PyCFunction) scribus_masterpagenames, METH_NOARGS, tr(scribus_masterpagenames__doc__)},
	{ "mergeTableCells", scribus_mergetablecells, METH_VARARGS, tr(scribus_mergetablecells__doc__)},
	{ "messageBox", (PyCFunction) scribus_messagebox, METH_VARARGS|METH_KEYWORDS, tr(scribus_messagebox__doc__)},
	{ "messagebarText", scribus_statusmessage, METH_VARARGS, tr(scribus_statusmessage__doc__)}, // Deprecated
	{ "moveObject", scribus_moveobjectrel, METH_VARARGS, tr(scribus_moveobjectrel__doc__)},
	{ "moveObjectAbs", scribus_moveobjectabs, METH_VARARGS, tr(scribus_moveobjectabs__doc__)},
	{ "moveSelectionToBack", (PyCFunction) scribus_moveselectiontoback, METH_NOARGS, tr(scribus_moveselectiontoback__doc__) },
	{ "moveSelectionToFront", (PyCFunction) scribus_moveselectiontofront, METH_NOARGS, tr(scribus_moveselectiontofront__doc__) },
	{ "newDoc", scribus_newdoc, METH_VARARGS, tr(scribus_newdoc__doc__)},
	{ "newDocDialog", (PyCFunction) scribus_newdocdialog, METH_NOARGS, tr(scribus_newdocdialog__doc__)},
	{ "newDocument", scribus_newdocument, METH_VARARGS, tr(scribus_newdocument__doc__)},
	{ "newPage", scribus_newpage, METH_VARARGS, tr(scribus_newpage__doc__)},
	{ "newStyleDialog", scribus_newstyledialog, METH_NOARGS, tr(scribus_newstyledialog__doc__)},
	{ "objectExists",scribus_objectexists, METH_VARARGS, tr(scribus_objectexists__doc__)},
	{ "openDoc", scribus_opendoc, METH_VARARGS, tr(scribus_opendoc__doc__)},
	{ "outlineText", scribus_outlinetext, METH_VARARGS, tr(scribus_outlinetext__doc__)},
	{ "pageCount", (PyCFunction) scribus_pagecount, METH_NOARGS, tr(scribus_pagecount__doc__)},
	{ "pageDimension", (PyCFunction) scribus_getpagesize, METH_NOARGS, "Obsolete function. Don't use it."},
	{ "placeEPS", scribus_placevec, METH_VARARGS, tr(scribus_placeeps__doc__)},
	{ "placeODG", scribus_placevec, METH_VARARGS, tr(scribus_placeodg__doc__)},
	{ "placeSVG", scribus_placevec, METH_VARARGS, tr(scribus_placesvg__doc__)},
	{ "placeSXD", scribus_placevec, METH_VARARGS, tr(scribus_placesxd__doc__)},
	{ "placeVectorFile", scribus_placevec, METH_VARARGS, tr(scribus_placevec__doc__)},
	{ "progressReset", (PyCFunction) scribus_progressreset, METH_NOARGS, tr(scribus_progressreset__doc__)},
	{ "progressSet", scribus_progresssetprogress, METH_VARARGS, tr(scribus_progresssetprogress__doc__)},
	{ "progressTotal", scribus_progresssettotalsteps, METH_VARARGS, tr(scribus_progresssettotalsteps__doc__)},
	{ "raiseActiveLayer", (PyCFunction) scribus_raiseactivelayer, METH_NOARGS, tr(scribus_raiseactivelayer__doc__)},
	{ "readPDFOptions", (PyCFunction) scribus_readpdfoptions, METH_VARARGS, tr(scribus_readpdfoptions__doc__)},
	{ "redrawAll", (PyCFunction) scribus_redraw, METH_NOARGS, tr(scribus_redraw__doc__)},
	{ "removeTableColumns", scribus_removetablecolumns, METH_VARARGS, tr(scribus_removetablecolumns__doc__)},
	{ "removeTableRows", scribus_removetablerows, METH_VARARGS, tr(scribus_removetablerows__doc__)},
	{ "renderFont", (PyCFunction) scribus_renderfont, METH_VARARGS|METH_KEYWORDS, tr(scribus_renderfont__doc__)},
	{ "replaceColor", scribus_replcolor, METH_VARARGS, tr(scribus_replcolor__doc__)},
	{ "resizeTableColumn", scribus_resizetablecolumn, METH_VARARGS, tr(scribus_resizetablecolumn__doc__)},
	{ "resizeTableRow", scribus_resizetablerow, METH_VARARGS, tr(scribus_resizetablerow__doc__)},
	{ "revertDoc", (PyCFunction) scribus_revertdoc, METH_NOARGS, tr(scribus_revertdoc__doc__)},
	{ "rotateObject", scribus_rotateobjectrel, METH_VARARGS, tr(scribus_rotateobjectrel__doc__)},
	{ "rotateObjectAbs", (PyCFunction) scribus_setrotation, METH_VARARGS|METH_KEYWORDS, tr(scribus_setrotation__doc__)}, // Deprecated, alias to setRotation
	{ "saveDoc", (PyCFunction) scribus_savedoc, METH_NOARGS, tr(scribus_savedoc__doc__)},
	{ "saveDocAs", scribus_savedocas, METH_VARARGS, tr(scribus_savedocas__doc__)},
	{ "savePDFOptions", (PyCFunction) scribus_savepdfoptions, METH_VARARGS, tr(scribus_savepdfoptions__doc__)},
	{ "savePageAsEPS", scribus_savepageeps, METH_VARARGS, tr(scribus_savepageeps__doc__)},
	{ "scaleGroup", scribus_scalegroup, METH_VARARGS, tr(scribus_scalegroup__doc__)},
	{ "scaleImage", scribus_scaleimage, METH_VARARGS, tr(scribus_scaleimage__doc__)},
	{ "scrollDocument", scribus_scrolldocument, METH_VARARGS, tr(scribus_scrolldocument__doc__) },
	{ "selectFrameText", scribus_selectframetext, METH_VARARGS, tr(scribus_selectframetext__doc__)},
	{ "selectObject", scribus_selectobject, METH_VARARGS, tr(scribus_selectobject__doc__)},
	{ "selectText", scribus_selecttext, METH_VARARGS, tr(scribus_selecttext__doc__)},
	{ "selectionCount", (PyCFunction) scribus_selectioncount, METH_NOARGS, tr(scribus_selectioncount__doc__)},
	{ "sendToLayer", scribus_sendtolayer, METH_VARARGS, tr(scribus_sendtolayer__doc__)},
	{ "sentToLayer", scribus_sendtolayer, METH_VARARGS, tr(scribus_sendtolayer__doc__)}, // Deprecated, alias to sentToLayer
	{ "setActiveLayer", scribus_setactivelayer, METH_VARARGS, tr(scribus_setactivelayer__doc__)},
	{ "setBaseLine", scribus_setbaseline, METH_VARARGS, tr(scribus_setbaseline__doc__)},
	{ "setBleeds", scribus_setbleeds, METH_VARARGS, tr(scribus_setbleeds__doc__)},
	{ "setCellBottomBorder", scribus_setcellbottomborder, METH_VARARGS, tr(scribus_setcellbottomborder__doc__)},
	{ "setCellBottomPadding", scribus_setcellbottompadding, METH_VARARGS, tr(scribus_setcellbottompadding__doc__)},
	{ "setCellFillColor", scribus_setcellfillcolor, METH_VARARGS, tr(scribus_setcellfillcolor__doc__)},
	{ "setCellLeftBorder", scribus_setcellleftborder, METH_VARARGS, tr(scribus_setcellleftborder__doc__)},
	{ "setCellLeftPadding", scribus_setcellleftpadding, METH_VARARGS, tr(scribus_setcellleftpadding__doc__)},
	{ "setCellRightBorder", scribus_setcellrightborder, METH_VARARGS, tr(scribus_setcellrightborder__doc__)},
	{ "setCellRightPadding", scribus_setcellrightpadding, METH_VARARGS, tr(scribus_setcellrightpadding__doc__)},
	{ "setCellStyle", scribus_setcellstyle, METH_VARARGS, tr(scribus_setcellstyle__doc__)},
	{ "setCellText", scribus_setcelltext, METH_VARARGS, tr(scribus_setcelltext__doc__)},
	{ "setCellTopBorder", scribus_setcelltopborder, METH_VARARGS, tr(scribus_setcelltopborder__doc__)},
	{ "setCellTopPadding", scribus_setcelltoppadding, METH_VARARGS, tr(scribus_setcelltoppadding__doc__)},
	{ "setCharacterStyle", scribus_setcharstyle, METH_VARARGS, tr(scribus_setcharstyle__doc__) },
	{ "setColumnGap", scribus_setcolumngap, METH_VARARGS, tr(scribus_setcolumngap__doc__)},
	{ "setColumns", scribus_setcolumns, METH_VARARGS, tr(scribus_setcolumns__doc__)},
	{ "setColumnGuides", (PyCFunction) scribus_setColumnGuides, METH_VARARGS|METH_KEYWORDS, tr(scribus_setColumnGuides__doc__)},
	{ "setCornerRadius", scribus_setcornerradius, METH_VARARGS, tr(scribus_setcornerradius__doc__)},
	{ "setCurrentPageSize", scribus_setcurrentpagesize, METH_VARARGS, tr(scribus_setcurrentpagesize__doc__) },
	{ "setCursor", scribus_setcursor, METH_VARARGS, tr(scribus_setcursor__doc__)},
	{ "setCustomLineStyle", scribus_setcustomlinestyle, METH_VARARGS, tr(scribus_setcustomlinestyle__doc__)},
	{ "setDocType", scribus_setdoctype, METH_VARARGS, tr(scribus_setdoctype__doc__)},
	{ "setEditMode", (PyCFunction) scribus_seteditmode, METH_NOARGS, tr(scribus_seteditmode__doc__)},
	{ "setFillBlendmode", scribus_setfillblend, METH_VARARGS, tr(scribus_setfillblend__doc__)},
	{ "setFillColor", scribus_setfillcolor, METH_VARARGS, tr(scribus_setfillcolor__doc__)},
	{ "setFillShade", scribus_setfillshade, METH_VARARGS, tr(scribus_setfillshade__doc__)},
	{ "setFillTransparency", scribus_setfilltrans, METH_VARARGS, tr(scribus_setfilltrans__doc__)},
	{ "setFirstLineOffset", scribus_setfirstlineoffset, METH_VARARGS, tr(scribus_setfirstlineoffset__doc__)},
	{ "setFont", scribus_setfont, METH_VARARGS, tr(scribus_setfont__doc__)},
	{ "setFontFeatures", scribus_setfontfeatures, METH_VARARGS, tr(scribus_setfontfeatures__doc__)},
	{ "setFontSize", scribus_setfontsize, METH_VARARGS, tr(scribus_setfontsize__doc__)},
	{ "setGradientFill", scribus_setgradfill, METH_VARARGS, tr(scribus_setgradfill__doc__)},
	{ "setGradientStop", scribus_setgradstop, METH_VARARGS, tr(scribus_setgradstop__doc__)},
	{ "setGradientVector", scribus_setgradvector, METH_VARARGS, tr(scribus_setgradvector__doc__) },
	{ "setHGuides", scribus_setHguides, METH_VARARGS, tr(scribus_setHguides__doc__)},
	{ "setImageBrightness", scribus_setimagebrightness, METH_VARARGS, tr(scribus_setimagebrightness__doc__)},
	{ "setImageGrayscale", scribus_setimagegrayscale, METH_VARARGS, tr(scribus_setimagegrayscale__doc__)},
	{ "setImageOffset", scribus_setimageoffset, METH_VARARGS, tr(scribus_setimageoffset__doc__)},
	{ "setImageScale", scribus_setimagescale, METH_VARARGS, tr(scribus_setimagescale__doc__)},
	{ "setInfo", scribus_setinfo, METH_VARARGS, tr(scribus_setinfo__doc__)},
	{ "setItemName", scribus_setitemname, METH_VARARGS, tr(scribus_setitemname__doc__)},
	{ "setNormalMode", (PyCFunction) scribus_setnormalmode, METH_NOARGS, tr(scribus_setnormalmode__doc__)},
	{ "setLayerBlendmode", scribus_setlayerblendmode, METH_VARARGS, tr(scribus_setlayerblendmode__doc__)},
	{ "setLayerFlow", scribus_setlayerflow, METH_VARARGS, tr(scribus_setlayerflow__doc__)},
	{ "setLayerLocked", scribus_setlayerlocked, METH_VARARGS, tr(scribus_setlayerlocked__doc__)},
	{ "setLayerOutlined", scribus_setlayeroutlined, METH_VARARGS, tr(scribus_setlayeroutlined__doc__)},
	{ "setLayerPrintable", scribus_setlayerprintable, METH_VARARGS, tr(scribus_setlayerprintable__doc__)},
	{ "setLayerTransparency", scribus_setlayertransparency, METH_VARARGS, tr(scribus_setlayertransparency__doc__)},
	{ "setLayerVisible", scribus_setlayervisible, METH_VARARGS, tr(scribus_setlayervisible__doc__)},
	{ "setLineBlendmode", scribus_setlineblend, METH_VARARGS, tr(scribus_setlineblend__doc__)},
	{ "setLineCap", scribus_setlinecap, METH_VARARGS, tr(scribus_setlinecap__doc__)},
	{ "setLineColor", scribus_setlinecolor, METH_VARARGS, tr(scribus_setlinecolor__doc__)},
	{ "setLineJoin", scribus_setlinejoin, METH_VARARGS, tr(scribus_setlinejoin__doc__)},
	{ "setLineShade", scribus_setlineshade, METH_VARARGS, tr(scribus_setlineshade__doc__)},
	{ "setLineSpacing", scribus_setlinespacing, METH_VARARGS, tr(scribus_setlinespacing__doc__)},
	{ "setLineSpacingMode", scribus_setlinespacingmode, METH_VARARGS, tr(scribus_setlinespacingmode__doc__)},
	{ "setLineStyle", scribus_setlinestyle, METH_VARARGS, tr(scribus_setlinestyle__doc__)},
	{ "setLineTransparency", scribus_setlinetrans, METH_VARARGS, tr(scribus_setlinetrans__doc__)},
	{ "setLineWidth", scribus_setlinewidth, METH_VARARGS, tr(scribus_setlinewidth__doc__)},
	{ "setMargins", scribus_setmargins, METH_VARARGS, tr(scribus_setmargins__doc__)},
	{ "setMultiLine", scribus_setmultiline, METH_VARARGS, tr(scribus_setmultiline__doc__)},
	{ "setNewName", scribus_setitemname, METH_VARARGS, tr(scribus_setitemname__doc__)}, // Deprecated, was in fact never documented
	{ "setObjectAttributes", scribus_setobjectattributes, METH_VARARGS, tr(scribus_setobjectattributes__doc__)},
	{ "setPDFBookmark", scribus_setpdfbookmark, METH_VARARGS, tr(scribus_setpdfbookmark__doc__)},
	{ "setParagraphStyle", scribus_setparagraphstyle, METH_VARARGS, tr(scribus_setparagraphstyle__doc__)},
	{ "setRedraw", scribus_setredraw, METH_VARARGS, tr(scribus_setredraw__doc__)},
	{ "setRotation", (PyCFunction) scribus_setrotation, METH_VARARGS|METH_KEYWORDS, tr(scribus_setrotation__doc__)},
	{ "setRowGuides", (PyCFunction) scribus_setRowGuides, METH_VARARGS|METH_KEYWORDS, tr(scribus_setRowGuides__doc__)},
	{ "setScaleFrameToImage", (PyCFunction) scribus_setscaleframetoimage, METH_VARARGS, tr(scribus_setscaleframetoimage__doc__)},
	{ "setScaleImageToFrame", (PyCFunction) scribus_setscaleimagetoframe, METH_VARARGS|METH_KEYWORDS, tr(scribus_setscaleimagetoframe__doc__)},
	{ "setSpotColor", scribus_setspotcolor, METH_VARARGS, tr(scribus_setspotcolor__doc__)},
	{ "setStyle", scribus_setparagraphstyle, METH_VARARGS, tr(scribus_setstyle__doc__)}, // Deprecated
	{ "setTableBottomBorder", scribus_settablebottomborder, METH_VARARGS, tr(scribus_settablebottomborder__doc__)},
	{ "setTableFillColor", scribus_settablefillcolor, METH_VARARGS, tr(scribus_settablefillcolor__doc__)},
	{ "setTableLeftBorder", scribus_settableleftborder, METH_VARARGS, tr(scribus_settableleftborder__doc__)},
	{ "setTableRightBorder", scribus_settablerightborder, METH_VARARGS, tr(scribus_settablerightborder__doc__)},
	{ "setTableStyle", scribus_settablestyle, METH_VARARGS, tr(scribus_settablestyle__doc__)},
	{ "setTableTopBorder", scribus_settabletopborder, METH_VARARGS, tr(scribus_settabletopborder__doc__)},
	{ "setText", scribus_settext, METH_VARARGS, tr(scribus_settext__doc__)},
	{ "setTextAlignment", scribus_settextalignment, METH_VARARGS, tr(scribus_settextalignment__doc__)},
	{ "setTextColor", scribus_settextfill, METH_VARARGS, tr(scribus_settextfill__doc__)},
	{ "setTextDirection", scribus_setdirection, METH_VARARGS, tr(scribus_setdirection__doc__)},
	{ "setTextDistances", scribus_settextdistances, METH_VARARGS, tr(scribus_settextdistances__doc__)},
	{ "setTextFlowMode", scribus_settextflowmode, METH_VARARGS, tr(scribus_settextflowmode__doc__)},
	{ "setTextScalingH", scribus_settextscalingh, METH_VARARGS, tr(scribus_settextscalingh__doc__)},
	{ "setTextScalingV", scribus_settextscalingv, METH_VARARGS, tr(scribus_settextscalingv__doc__)},
	{ "setTextShade", scribus_settextshade, METH_VARARGS, tr(scribus_settextshade__doc__)},
	{ "setTextStroke", scribus_settextstroke, METH_VARARGS, tr(scribus_settextstroke__doc__)},
	{ "setTextVerticalAlignment", scribus_settextverticalalignment, METH_VARARGS, tr(scribus_settextverticalalignment__doc__)},
	{ "setTracking", scribus_settracking, METH_VARARGS, tr(scribus_settracking__doc__)},
	{ "setUnit", scribus_setunit, METH_VARARGS, tr(scribus_setunit__doc__)},
	{ "setVGuides", scribus_setVguides, METH_VARARGS, tr(scribus_setVguides__doc__)},
	{ "setWordTracking", scribus_setwordtracking, METH_VARARGS, tr(scribus_setwordtracking__doc__)},
	{ "sizeObject", scribus_sizeobject, METH_VARARGS, tr(scribus_sizeobject__doc__)},
	{ "statusMessage", scribus_statusmessage, METH_VARARGS, tr(scribus_statusmessage__doc__)},
	{ "textFlowMode", scribus_settextflowmode, METH_VARARGS, tr(scribus_textflowmode__doc__)}, // Deprecated
	{ "textOverflows", (PyCFunction) scribus_istextoverflowing, METH_VARARGS|METH_KEYWORDS, tr(scribus_istextoverflowing__doc__) },
	{ "traceText", scribus_outlinetext, METH_VARARGS, tr(scribus_tracetext__doc__)},
	{ "unGroupObject", scribus_ungroupobjects, METH_VARARGS, tr(scribus_ungroupobjects__doc__)}, // Deprecated, now alias for unGroupObjects()
	{ "unGroupObjects", scribus_ungroupobjects, METH_VARARGS, tr(scribus_ungroupobjects__doc__)},
	{ "unlinkTextFrames", scribus_unlinktextframes, METH_VARARGS, tr(scribus_unlinktextframes__doc__)},
	{ "valueDialog", scribus_valuedialog, METH_VARARGS, tr(scribus_valuedialog__doc__)},
	{ "zoomDocument", scribus_zoomdocument, METH_VARARGS, tr(scribus_zoomdocument__doc__)},
	// Property magic
	{ "getPropertyCType", (PyCFunction) scribus_propertyctype, METH_VARARGS|METH_KEYWORDS, tr(scribus_propertyctype__doc__)},
	{ "getPropertyNames", (PyCFunction) scribus_getpropertynames, METH_VARARGS|METH_KEYWORDS, tr(scribus_getpropertynames__doc__)},
	{ "getProperty", (PyCFunction) scribus_getproperty, METH_VARARGS|METH_KEYWORDS, tr(scribus_getproperty__doc__)},
	{ "setProperty", (PyCFunction) scribus_setproperty, METH_VARARGS|METH_KEYWORDS, tr(scribus_setproperty__doc__)},
// 	{ "getChildren", (PyCFunction) scribus_getchildren, METH_VARARGS|METH_KEYWORDS, tr(scribus_getchildren__doc__)},
// 	{ "getChild", (PyCFunction) scribus_getchild, METH_VARARGS|METH_KEYWORDS, tr(scribus_getchild__doc__)},
	// by Christian Hausknecht
	{ "copyObject", scribus_copyobject, METH_VARARGS, tr(scribus_copyobject__doc__)}, // Deprecated
	{ "copyObjects", scribus_copyobjects, METH_VARARGS, tr(scribus_copyobjects__doc__)},
	{ "duplicateObject", scribus_duplicateobject, METH_VARARGS, tr(scribus_duplicateobject__doc__)}, // Deprecated
	{ "duplicateObjects", scribus_duplicateobjects, METH_VARARGS, tr(scribus_duplicateobjects__doc__)},
	{ "pasteObject", scribus_pasteobject, METH_NOARGS, tr(scribus_pasteobject__doc__)}, // Deprecated
	{ "pasteObjects", scribus_pasteobjects, METH_NOARGS, tr(scribus_pasteobjects__doc__)},
	// by Tj (hacker@iam.tj>
	{ "combinePolygons", (PyCFunction) scribus_combinepolygons, METH_NOARGS, tr(scribus_combinepolygons__doc__)},
	// Internal methods - Not for public use
	{ "retval", (PyCFunction) scribus_retval, METH_VARARGS, "Scribus internal." },
	{ "getval", (PyCFunction) scribus_getval, METH_NOARGS, "Scribus internal." },
	{ "setLinkAnnotation", scribus_setlinkannotation, METH_VARARGS,tr(scribus_setlinkannotation__doc__)},
	{ "setFileAnnotation", (PyCFunction) scribus_setfileannotation, METH_VARARGS|METH_KEYWORDS,tr(scribus_setfileannotation__doc__)},
	{ "setURIAnnotation", scribus_seturiannotation, METH_VARARGS,tr(scribus_seturiannotation__doc__)},
	{ "setTextAnnotation", scribus_settextannotation, METH_VARARGS,tr(scribus_settextannotation__doc__)},
	{ "createPdfAnnotation", scribus_createpdfannotation, METH_VARARGS,tr(scribus_createpdfannotation__doc__)},
	{ "isAnnotated",(PyCFunction) scribus_isannotated, METH_VARARGS|METH_KEYWORDS,tr(scribus_isannotated__doc__)},
	{ "setJSActionScript", scribus_setjsactionscript, METH_VARARGS,tr(scribus_setjsactionscript__doc__)},
	{ "getJSActionScript", scribus_getjsactionscript, METH_VARARGS,tr(scribus_getjsactionscript__doc__)},
	{nullptr, (PyCFunction)(nullptr), 0, nullptr} /* sentinel */
};

struct scribus_module_state
{
    PyObject *error;
};
#define GETSTATE(m) ((struct scribus_module_state*) PyModule_GetState(m))

static int scribus_extension_traverse(PyObject *m, visitproc visit, void *arg)
{
	Py_VISIT(GETSTATE(m)->error);
	return 0;
}

static int scribus_extension_clear(PyObject *m)
{
	Py_CLEAR(GETSTATE(m)->error);
	return 0;
}

static struct PyModuleDef scribus_module_def = {
        PyModuleDef_HEAD_INIT,
        "scribus",
        nullptr,
        sizeof(struct scribus_module_state),
        scribus_methods,
        nullptr,
        scribus_extension_traverse,
        scribus_extension_clear,
        nullptr
};

void initscribus_failed(const char* fileName, int lineNo)
{
	qDebug("Scripter setup failed (%s:%i)", fileName, lineNo);
	if (PyErr_Occurred())
		PyErr_Print();
}

PyObject* PyInit_scribus(void)
{
	ScribusMainWindow* mainWin = ScCore->primaryMainWindow();
	if (!scripterCore)
	{
		qWarning("scriptplugin: Tried to init scribus module, but no scripter core. Aborting.");
		return nullptr;
	}

	int result;
	PyObject *m, *d;

	PyType_Ready(&Printer_Type);
	PyType_Ready(&PDFfile_Type);
	PyType_Ready(&ImageExport_Type);

	m = PyModule_Create(&scribus_module_def);

	Py_INCREF(&Printer_Type);
	result = PyModule_AddObject(m, "Printer", (PyObject *) &Printer_Type);
	if (result != 0)
		qDebug("scriptplugin: Could not create scribus.Printer module");
	Py_INCREF(&PDFfile_Type);
	result = PyModule_AddObject(m, "PDFfile", (PyObject *) &PDFfile_Type);
	if (result != 0)
		qDebug("scriptplugin: Could not create scribus.PDFfile module");
	Py_INCREF(&ImageExport_Type);
	PyModule_AddObject(m, "ImageExport", (PyObject *) &ImageExport_Type);
	if (result != 0)
		qDebug("scriptplugin: Could not create scribus.ImageExport module");
	d = PyModule_GetDict(m);

	// Set up the module exceptions
	// common exc.
	ScribusException = PyErr_NewException("scribus.ScribusException", nullptr, nullptr);
	Py_INCREF(ScribusException);
	PyModule_AddObject(m, "ScribusException", ScribusException);
	// no doc open
	NoDocOpenError = PyErr_NewException("scribus.NoDocOpenError", ScribusException, nullptr);
	Py_INCREF(NoDocOpenError);
	PyModule_AddObject(m, "NoDocOpenError", NoDocOpenError);
	// wrong type of frame for operation
	WrongFrameTypeError = PyErr_NewException("scribus.WrongFrameTypeError", ScribusException, nullptr);
	Py_INCREF(WrongFrameTypeError);
	PyModule_AddObject(m, "WrongFrameTypeError", WrongFrameTypeError);
	// Couldn't find named object, or no named object and no selection
	NoValidObjectError = PyErr_NewException("scribus.NoValidObjectError", ScribusException, nullptr);
	Py_INCREF(NoValidObjectError);
	PyModule_AddObject(m, "NoValidObjectError", NoValidObjectError);
	// Couldn't find the specified resource - font, color, etc.
	NotFoundError = PyErr_NewException("scribus.NotFoundError", ScribusException, nullptr);
	Py_INCREF(NotFoundError);
	PyModule_AddObject(m, "NotFoundError", NotFoundError);
	// Tried to create an object with the same name as one that already exists
	NameExistsError = PyErr_NewException("scribus.NameExistsError", ScribusException, nullptr);
	Py_INCREF(NameExistsError);
	PyModule_AddObject(m, "NameExistsError", NameExistsError);
	// Done with exception setup

	// CONSTANTS
	// Units
	PyDict_SetItemString(d, "UNIT_POINTS", PyLong_FromLong(unitIndexFromString("pt")));
	PyDict_SetItemString(d, "UNIT_MILLIMETERS", PyLong_FromLong(unitIndexFromString("mm")));
	PyDict_SetItemString(d, "UNIT_INCHES", PyLong_FromLong(unitIndexFromString("in")));
	PyDict_SetItemString(d, "UNIT_PICAS", PyLong_FromLong(unitIndexFromString("p")));
	PyDict_SetItemString(d, "UNIT_CENTIMETRES", PyLong_FromLong(unitIndexFromString("cm")));
	PyDict_SetItemString(d, "UNIT_CICERO", PyLong_FromLong(unitIndexFromString("c")));
	PyDict_SetItemString(d, "UNIT_PT", PyLong_FromLong(unitIndexFromString("pt")));
	PyDict_SetItemString(d, "UNIT_MM", PyLong_FromLong(unitIndexFromString("mm")));
	PyDict_SetItemString(d, "UNIT_IN", PyLong_FromLong(unitIndexFromString("in")));
	PyDict_SetItemString(d, "UNIT_P", PyLong_FromLong(unitIndexFromString("p")));
	PyDict_SetItemString(d, "UNIT_CM", PyLong_FromLong(unitIndexFromString("cm")));
	PyDict_SetItemString(d, "UNIT_C", PyLong_FromLong(unitIndexFromString("c")));
	// Page orientation
	PyDict_SetItemString(d, "PORTRAIT", Py_BuildValue("i", portraitPage));
	PyDict_SetItemString(d, "LANDSCAPE", Py_BuildValue("i", landscapePage));
	// Page layout
	PyDict_SetItemString(d, "NOFACINGPAGES", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "FACINGPAGES",  Py_BuildValue("i", 1));
	// First page position
	PyDict_SetItemString(d, "FIRSTPAGERIGHT", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "FIRSTPAGELEFT", Py_BuildValue("i", 0));
	// Text horizontal alignment
	PyDict_SetItemString(d, "ALIGN_LEFT", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "ALIGN_RIGHT", Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "ALIGN_CENTERED", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "ALIGN_BLOCK", Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "ALIGN_FORCED", Py_BuildValue("i", 4));
	// Text vertical alignment
	PyDict_SetItemString(d, "ALIGNV_TOP", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "ALIGNV_CENTERED", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "ALIGNV_BOTTOM", Py_BuildValue("i", 2));
	// Text direction
	PyDict_SetItemString(d, "DIRECTION_LTR", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "DIRECTION_RTL", Py_BuildValue("i", 1));
	// First line offset
	PyDict_SetItemString(d, "FLOP_REALGLYPHHEIGHT", Py_BuildValue("i", (int) FLOPRealGlyphHeight));
	PyDict_SetItemString(d, "FLOP_FONTASCENT", Py_BuildValue("i", (int) FLOPFontAscent));
	PyDict_SetItemString(d, "FLOP_LINESPACING", Py_BuildValue("i", (int) FLOPLineSpacing));
	PyDict_SetItemString(d, "FLOP_BASELINEGRID", Py_BuildValue("i", (int) FLOPBaselineGrid));
	// Fill mode
	PyDict_SetItemString(d, "FILL_NOG", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "FILL_HORIZONTALG", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "FILL_VERTICALG", Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "FILL_DIAGONALG", Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "FILL_CROSSDIAGONALG", Py_BuildValue("i", 4));
	PyDict_SetItemString(d, "FILL_RADIALG", Py_BuildValue("i", 5));
	// Stroke type
	PyDict_SetItemString(d, "LINE_SOLID", Py_BuildValue("i", Qt::SolidLine));
	PyDict_SetItemString(d, "LINE_DASH", Py_BuildValue("i", Qt::DashLine));
	PyDict_SetItemString(d, "LINE_DOT", Py_BuildValue("i", Qt::DotLine));
	PyDict_SetItemString(d, "LINE_DASHDOT", Py_BuildValue("i", Qt::DashDotLine));
	PyDict_SetItemString(d, "LINE_DASHDOTDOT", Py_BuildValue("i", Qt::DashDotDotLine));
	// Line join type
	PyDict_SetItemString(d, "JOIN_MITTER", Py_BuildValue("i", Qt::MiterJoin));
	PyDict_SetItemString(d, "JOIN_BEVEL", Py_BuildValue("i", Qt::BevelJoin));
	PyDict_SetItemString(d, "JOIN_ROUND", Py_BuildValue("i", Qt::RoundJoin));
	// Line cap type
	PyDict_SetItemString(d, "CAP_FLAT", Py_BuildValue("i", Qt::FlatCap));
	PyDict_SetItemString(d, "CAP_SQUARE", Py_BuildValue("i", Qt::SquareCap));
	PyDict_SetItemString(d, "CAP_ROUND", Py_BuildValue("i", Qt::RoundCap));
	// QMessageBox buttons
	PyDict_SetItemString(d, "BUTTON_NONE", Py_BuildValue("i", QMessageBox::NoButton));
	PyDict_SetItemString(d, "BUTTON_OK", Py_BuildValue("i", QMessageBox::Ok));
	PyDict_SetItemString(d, "BUTTON_CANCEL", Py_BuildValue("i", QMessageBox::Cancel));
	PyDict_SetItemString(d, "BUTTON_YES", Py_BuildValue("i", QMessageBox::Yes));
	PyDict_SetItemString(d, "BUTTON_NO", Py_BuildValue("i", QMessageBox::No));
	PyDict_SetItemString(d, "BUTTON_ABORT", Py_BuildValue("i", QMessageBox::Abort));
	PyDict_SetItemString(d, "BUTTON_RETRY", Py_BuildValue("i", QMessageBox::Retry));
	PyDict_SetItemString(d, "BUTTON_IGNORE", Py_BuildValue("i", QMessageBox::Ignore));
	PyDict_SetItemString(d, "BUTTON_DEFAULT", Py_BuildValue("i", QMessageBox::Default));
	PyDict_SetItemString(d, "BUTTON_ESCAPE", Py_BuildValue("i", QMessageBox::Escape));
	// QMessageBox icons
	PyDict_SetItemString(d, "ICON_NONE", Py_BuildValue("i", QMessageBox::NoIcon));
	PyDict_SetItemString(d, "ICON_INFORMATION", Py_BuildValue("i", QMessageBox::Information));
	PyDict_SetItemString(d, "ICON_WARNING", Py_BuildValue("i", QMessageBox::Warning));
	PyDict_SetItemString(d, "ICON_CRITICAL", Py_BuildValue("i", QMessageBox::Critical));
	// Paper formats
	PyDict_SetItemString(d, "PAPER_A0", Py_BuildValue("(ff)", 2380.0, 3368.0));
	PyDict_SetItemString(d, "PAPER_A1", Py_BuildValue("(ff)", 1684.0, 2380.0));
	PyDict_SetItemString(d, "PAPER_A2", Py_BuildValue("(ff)", 1190.0, 1684.0));
	PyDict_SetItemString(d, "PAPER_A3", Py_BuildValue("(ff)", 842.0, 1190.0));
	PyDict_SetItemString(d, "PAPER_A4", Py_BuildValue("(ff)", 595.0, 842.0));
	PyDict_SetItemString(d, "PAPER_A5", Py_BuildValue("(ff)", 421.0, 595.0));
	PyDict_SetItemString(d, "PAPER_A6", Py_BuildValue("(ff)", 297.0, 421.0));
	PyDict_SetItemString(d, "PAPER_A7", Py_BuildValue("(ff)", 210.0, 297.0));
	PyDict_SetItemString(d, "PAPER_A8", Py_BuildValue("(ff)", 148.0, 210.0));
	PyDict_SetItemString(d, "PAPER_A9", Py_BuildValue("(ff)", 105.0, 148.0));
	PyDict_SetItemString(d, "PAPER_A0_MM", Py_BuildValue("(ff)", 841.0, 1189.0));
	PyDict_SetItemString(d, "PAPER_A1_MM", Py_BuildValue("(ff)", 594.0, 841.0));
	PyDict_SetItemString(d, "PAPER_A2_MM", Py_BuildValue("(ff)", 420.0, 594.0));
	PyDict_SetItemString(d, "PAPER_A3_MM", Py_BuildValue("(ff)", 297.0, 420.0));
	PyDict_SetItemString(d, "PAPER_A4_MM", Py_BuildValue("(ff)", 210.0, 297.0));
	PyDict_SetItemString(d, "PAPER_A5_MM", Py_BuildValue("(ff)", 148.0, 210.0));
	PyDict_SetItemString(d, "PAPER_A6_MM", Py_BuildValue("(ff)", 105.0, 148.0));
	PyDict_SetItemString(d, "PAPER_A7_MM", Py_BuildValue("(ff)", 74.0, 105.0));
	PyDict_SetItemString(d, "PAPER_A8_MM", Py_BuildValue("(ff)", 52.0, 74.0));
	PyDict_SetItemString(d, "PAPER_A9_MM", Py_BuildValue("(ff)", 37.0, 52.0));
	PyDict_SetItemString(d, "PAPER_B0", Py_BuildValue("(ff)", 2836.0, 4008.0));
	PyDict_SetItemString(d, "PAPER_B1", Py_BuildValue("(ff)", 2004.0, 2836.0));
	PyDict_SetItemString(d, "PAPER_B2", Py_BuildValue("(ff)", 1418.0, 2004.0));
	PyDict_SetItemString(d, "PAPER_B3", Py_BuildValue("(ff)", 1002.0, 1418.0));
	PyDict_SetItemString(d, "PAPER_B4", Py_BuildValue("(ff)", 709.0, 1002.0));
	PyDict_SetItemString(d, "PAPER_B5", Py_BuildValue("(ff)", 501.0, 709.0));
	PyDict_SetItemString(d, "PAPER_B6", Py_BuildValue("(ff)", 355.0, 501.0));
	PyDict_SetItemString(d, "PAPER_B7", Py_BuildValue("(ff)", 250.0, 355.0));
	PyDict_SetItemString(d, "PAPER_B8", Py_BuildValue("(ff)", 178.0, 250.0));
	PyDict_SetItemString(d, "PAPER_B9", Py_BuildValue("(ff)", 125.0, 178.0));
	PyDict_SetItemString(d, "PAPER_B10", Py_BuildValue("(ff)", 89.0, 125.0));
	PyDict_SetItemString(d, "PAPER_B0_MM", Py_BuildValue("(ff)", 1000.0, 1414.0));
	PyDict_SetItemString(d, "PAPER_B1_MM", Py_BuildValue("(ff)", 707.0, 1000.0));
	PyDict_SetItemString(d, "PAPER_B2_MM", Py_BuildValue("(ff)", 500.0, 707.0));
	PyDict_SetItemString(d, "PAPER_B3_MM", Py_BuildValue("(ff)", 353.0, 500.0));
	PyDict_SetItemString(d, "PAPER_B4_MM", Py_BuildValue("(ff)", 250.0, 353.0));
	PyDict_SetItemString(d, "PAPER_B5_MM", Py_BuildValue("(ff)", 176.0, 250.0));
	PyDict_SetItemString(d, "PAPER_B6_MM", Py_BuildValue("(ff)", 125.0, 176.0));
	PyDict_SetItemString(d, "PAPER_B7_MM", Py_BuildValue("(ff)", 88.0, 125.0));
	PyDict_SetItemString(d, "PAPER_B8_MM", Py_BuildValue("(ff)", 62.0, 88.0));
	PyDict_SetItemString(d, "PAPER_B9_MM", Py_BuildValue("(ff)", 44.0, 62.0));
	PyDict_SetItemString(d, "PAPER_B10_MM", Py_BuildValue("(ff)", 31.0, 44.0));
	PyDict_SetItemString(d, "PAPER_C5E", Py_BuildValue("(ff)", 462.0, 649.0));
	PyDict_SetItemString(d, "PAPER_COMM10E", Py_BuildValue("(ff)", 298.0, 683.0));
	PyDict_SetItemString(d, "PAPER_DLE",  Py_BuildValue("(ff)", 312.0, 624.0));
	PyDict_SetItemString(d, "PAPER_EXECUTIVE", Py_BuildValue("(ff)", 542.0, 720.0));
	PyDict_SetItemString(d, "PAPER_FOLIO", Py_BuildValue("(ff)", 595.0, 935.0));
	PyDict_SetItemString(d, "PAPER_LEDGER", Py_BuildValue("(ff)", 1224.0, 792.0));
	PyDict_SetItemString(d, "PAPER_LEGAL", Py_BuildValue("(ff)", 612.0, 1008.0));
	PyDict_SetItemString(d, "PAPER_LETTER", Py_BuildValue("(ff)", 612.0, 792.0));
	PyDict_SetItemString(d, "PAPER_TABLOID", Py_BuildValue("(ff)", 792.0, 1224.0));
	// PageItem types
	PyDict_SetItemString(d, "ITEMTYPE_ITEMTYPE1",  Py_BuildValue("i", (int) PageItem::ItemType1));
	PyDict_SetItemString(d, "ITEMTYPE_IMAGEFRAME", Py_BuildValue("i", (int) PageItem::ImageFrame));
	PyDict_SetItemString(d, "ITEMTYPE_ITEMTYPE3",  Py_BuildValue("i", (int) PageItem::ItemType3));
	PyDict_SetItemString(d, "ITEMTYPE_TEXTFRAME",  Py_BuildValue("i", (int) PageItem::TextFrame));
	PyDict_SetItemString(d, "ITEMTYPE_LINE",       Py_BuildValue("i", (int) PageItem::Line));
	PyDict_SetItemString(d, "ITEMTYPE_POLYGON",    Py_BuildValue("i", (int) PageItem::Polygon));
	PyDict_SetItemString(d, "ITEMTYPE_POLYLINE",   Py_BuildValue("i", (int) PageItem::PolyLine));
	PyDict_SetItemString(d, "ITEMTYPE_PATHTEXT",   Py_BuildValue("i", (int) PageItem::PathText));
	PyDict_SetItemString(d, "ITEMTYPE_LATEXFRAME", Py_BuildValue("i", (int) PageItem::LatexFrame));
	PyDict_SetItemString(d, "ITEMTYPE_OSGFRAME",   Py_BuildValue("i", (int) PageItem::OSGFrame));
	PyDict_SetItemString(d, "ITEMTYPE_SYMBOL",     Py_BuildValue("i", (int) PageItem::Symbol));
	PyDict_SetItemString(d, "ITEMTYPE_GROUP",      Py_BuildValue("i", (int) PageItem::Group));
	PyDict_SetItemString(d, "ITEMTYPE_REGULARPOLYGON", Py_BuildValue("i", (int) PageItem::RegularPolygon));
	PyDict_SetItemString(d, "ITEMTYPE_ARC",        Py_BuildValue("i", (int) PageItem::Arc));
	PyDict_SetItemString(d, "ITEMTYPE_SPIRAL",     Py_BuildValue("i", (int) PageItem::Spiral));
	PyDict_SetItemString(d, "ITEMTYPE_TABLE",      Py_BuildValue("i", (int) PageItem::Table));
	PyDict_SetItemString(d, "ITEMTYPE_NOTEFRAME",  Py_BuildValue("i", (int) PageItem::NoteFrame));
	PyDict_SetItemString(d, "ITEMTYPE_MULTIPLE",   Py_BuildValue("i", (int) PageItem::Multiple));
	// Colorspaces
	PyDict_SetItemString(d, "CSPACE_UNDEFINED", Py_BuildValue("i", -1));
	PyDict_SetItemString(d, "CSPACE_RGB", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "CSPACE_CMYK", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "CSPACE_GRAY", Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "CSPACE_DUOTONE", Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "CSPACE_MONOCHROME", Py_BuildValue("i", 4));
	// Blend modes
	PyDict_SetItemString(d, "NORMAL", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "DARKEN", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "LIGHTEN", Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "MULTIPLY", Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "SCREEN", Py_BuildValue("i", 4));
	PyDict_SetItemString(d, "OVERLAY", Py_BuildValue("i", 5));
	PyDict_SetItemString(d, "HARD_LIGHT", Py_BuildValue("i", 6));
	PyDict_SetItemString(d, "SOFT_LIGHT", Py_BuildValue("i", 7));
	PyDict_SetItemString(d, "DIFFERENCE", Py_BuildValue("i", 8));
	PyDict_SetItemString(d, "EXCLUSION", Py_BuildValue("i", 9));
	PyDict_SetItemString(d, "COLOR_DODGE", Py_BuildValue("i", 10));
	PyDict_SetItemString(d, "COLOR_BURN", Py_BuildValue("i", 11));
	PyDict_SetItemString(d, "HUE", Py_BuildValue("i", 12));
	PyDict_SetItemString(d, "SATURATION", Py_BuildValue("i", 13));
	PyDict_SetItemString(d, "COLOR", Py_BuildValue("i", 14));
	PyDict_SetItemString(d, "LUMINOSITY", Py_BuildValue("i", 15));
	// Preset page layouts
	PyDict_SetItemString(d, "PAGE_1", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "PAGE_2", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "PAGE_3", Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "PAGE_4", Py_BuildValue("i", 3));
	// Print languages
	PyDict_SetItemString(d, "PRNLANG_POSTSCRIPT1", Py_BuildValue("i", (int) PrintLanguage::PostScript1));
	PyDict_SetItemString(d, "PRNLANG_POSTSCRIPT2", Py_BuildValue("i", (int) PrintLanguage::PostScript2));
	PyDict_SetItemString(d, "PRNLANG_POSTSCRIPT3", Py_BuildValue("i", (int) PrintLanguage::PostScript3));
	PyDict_SetItemString(d, "PRNLANG_WINDOWSGDI",  Py_BuildValue("i", (int) PrintLanguage::WindowsGDI));
	PyDict_SetItemString(d, "PRNLANG_PDF",         Py_BuildValue("i", (int) PrintLanguage::PDF));
	// Tab alignment
	PyDict_SetItemString(d, "TAB_LEFT", Py_BuildValue("i", 0));
	PyDict_SetItemString(d, "TAB_RIGHT", Py_BuildValue("i", 1));
	PyDict_SetItemString(d, "TAB_PERIOD", Py_BuildValue("i", 2));
	PyDict_SetItemString(d, "TAB_COMMA", Py_BuildValue("i", 3));
	PyDict_SetItemString(d, "TAB_CENTER", Py_BuildValue("i", 4));
	// Basepoint / AnchorPoint
	PyDict_SetItemString(d, "BASEPOINT_TOPLEFT", Py_BuildValue("i", (int) AnchorPoint::TopLeft));
	PyDict_SetItemString(d, "BASEPOINT_TOP", Py_BuildValue("i", (int) AnchorPoint::Top));
	PyDict_SetItemString(d, "BASEPOINT_TOPRIGHT", Py_BuildValue("i", (int) AnchorPoint::TopRight));
	PyDict_SetItemString(d, "BASEPOINT_LEFT", Py_BuildValue("i", (int) AnchorPoint::Left));
	PyDict_SetItemString(d, "BASEPOINT_CENTER", Py_BuildValue("i", (int) AnchorPoint::Center));
	PyDict_SetItemString(d, "BASEPOINT_RIGHT", Py_BuildValue("i", (int) AnchorPoint::Right));
	PyDict_SetItemString(d, "BASEPOINT_BOTTOMLEFT", Py_BuildValue("i", (int) AnchorPoint::BottomLeft));
	PyDict_SetItemString(d, "BASEPOINT_BOTTOM", Py_BuildValue("i", (int) AnchorPoint::Bottom));
	PyDict_SetItemString(d, "BASEPOINT_BOTTOMRIGHT", Py_BuildValue("i", (int) AnchorPoint::BottomRight));

	// Measurement units understood by Scribus's units.cpp functions are exported as constant conversion
	// factors to be used from Python.
	for (int i = 0; i <= unitGetMaxIndex()-2; ++i)
	{
		PyObject* value = PyFloat_FromDouble(unitGetRatioFromIndex(i));
		if (!value)
		{
			initscribus_failed(__FILE__, __LINE__);
			return nullptr;
		}
		// `in' is a reserved word in Python so we must replace it
		PyObject* name;
		if (unitGetUntranslatedStrFromIndex(i) == "in")
			name = PyUnicode_FromString("inch");
		else
			name = PyUnicode_FromString(unitGetUntranslatedStrFromIndex(i).toUtf8().constData());
		if (!name)
		{
			initscribus_failed(__FILE__, __LINE__);
			return nullptr;
		}
		if (PyDict_SetItem(d, name, value))
		{
			initscribus_failed(__FILE__, __LINE__);
			return nullptr;
		}
	}

	// Export the Scribus version into the module namespace so scripts know what they're running in
	PyDict_SetItemString(d, "SCRIBUS_VERSION", PyUnicode_FromString(ScribusAPI::getVersion().toLatin1().constData()));
	// Now build a version tuple like that provided by Python in sys.version_info
	// The tuple is of the form (major, minor, patchlevel, extraversion, reserved)
	int majorVersion = ScribusAPI::getVersionMajor();
	int minorVersion = ScribusAPI::getVersionMinor();
	int patchVersion = ScribusAPI::getVersionPatch();
	QString extraVersion = ScribusAPI::getVersionSuffix();

	PyObject* versionTuple = Py_BuildValue("(iiisi)", majorVersion, minorVersion, patchVersion, (const char*)extraVersion.toUtf8(), 0);
	if (versionTuple != nullptr)
		PyDict_SetItemString(d, "SCRIBUS_VERSION_INFO", versionTuple);
	else
		qDebug("Failed to build version tuple for version string '%s' in scripter", VERSION);


	// Function aliases for compatibility
	// We need to import the __builtins__, warnings and exceptions modules to be able to run
	// the generated Python functions from inside the `scribus' module's context.
	// This code makes it possible to extend the `scribus' module by running Python code
	// from C in other ways too.
	PyObject* builtinModule = PyImport_ImportModuleEx("builtins", d, d, Py_BuildValue("[]"));
	if (builtinModule == nullptr)
	{
		qDebug("Failed to import builtins module. Something is probably broken with your Python.");
		return nullptr;
	}
	PyDict_SetItemString(d, "builtins", builtinModule);
	PyObject* warningsModule = PyImport_ImportModuleEx("warnings", d, d, Py_BuildValue("[]"));
	if (warningsModule == nullptr)
	{
		qDebug("Failed to import warnings module. Something is probably broken with your Python.");
		return nullptr;
	}
	PyDict_SetItemString(d, "warnings", warningsModule);
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
- Many functions will raise a NoDocOpenError if you try to use them\n\
without a document to operate on.\n\
- If you do not pass a frame name to a function that requires one,\n\
the function will use the currently selected frame, if any, or\n\
raise a NoValidObjectError if it can't find anything to operate\n\
on.\n\
- Many functions will raise WrongFrameTypeError if you try to use them\n\
on a frame type that they do not make sense with. For example, setting\n\
the text color on a graphics frame doesn't make sense, and will result\n\
in this exception being raised.\n\
- Errors resulting from calls to the underlying Python API will be\n\
passed through unaltered. As such, the list of exceptions thrown by\n\
any function as provided here and in its docstring is incomplete.\n\
\n\
Details of what exceptions each function may throw are provided on the\n\
function's documentation, though as with most Python code this list\n\
is not exhaustive due to exceptions from called functions.\n\
");

	PyObject* docStr = PyUnicode_FromString(docstring.toUtf8().data());
	if (!docStr)
		qDebug("Failed to create module-level docstring (couldn't make str)");
	else
		PyDict_SetItemString(d, "__doc__", docStr);

	// Wrap up pointers to the the QApp and main window and push them out
	// to Python.
	wrappedQApp = wrapQObject(qApp);
	if (!wrappedQApp)
	{
		qDebug("Failed to wrap up QApp");
		PyErr_Print();
	}
	// Push it into the module dict, stealing a ref in the process
	PyDict_SetItemString(d, "qApp", wrappedQApp);
	Py_DECREF(wrappedQApp);
	wrappedQApp = nullptr;

	wrappedMainWindow = wrapQObject(mainWin);
	if (!wrappedMainWindow)
	{
		qDebug("Failed to wrap up ScribusMainWindow");
		PyErr_Print();
	}
	// Push it into the module dict, stealing a ref in the process
	PyDict_SetItemString(d, "mainWindow", wrappedMainWindow);
	Py_DECREF(wrappedMainWindow);
	wrappedMainWindow = nullptr;

	return m;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void scriptplugindocwarnings()
{
	QStringList s;
	s << imgexp__doc__
	  << imgexp_alltypes__doc__
	  << imgexp_dpi__doc__
	  << imgexp_filename__doc__
	  << imgexp_quality__doc__
	  << imgexp_save__doc__
	  << imgexp_saveas__doc__
	  << imgexp_scale__doc__
	  << imgexp_transparentBkgnd__doc__
	  << imgexp_type__doc__
	  << pdffile__doc__
	  << printer__doc__;
}

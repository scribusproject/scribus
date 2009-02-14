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
#include "cmdvar.h"

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
#include "cmdtext.h"
#include "cmdutil.h"
#include "cmdstyle.h"
#include "customfdialog.h"
#include "guiapp.h"
#include "helpbrowser.h"
#include "menumanager.h"
#include "objimageexport.h"
#include "objpdffile.h"
#include "objprinter.h"
#include "propertiespalette.h"
#include "scconfig.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scribusstructs.h"
#include "scriptercore.h"
#include "scripterprefsgui.h"
#include "scriptplugin.h"
#include "svgimport.h"
#include "units.h"
#include "util_icon.h"

#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>
#include <QRegExp>
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
	scripterCore=0;
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

ScriptPlugin::ScriptPlugin() : ScPersistentPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

ScriptPlugin::~ScriptPlugin() {};

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

const QString ScriptPlugin::fullTrName() const
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
	about->shortDescription = tr("Embedded Python scripting support.");
	// about->description = tr("Write me!")
	// about->version
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
	QString cm;
	Py_Initialize();
	if (PyUnicode_SetDefaultEncoding("utf-8"))
	{
		qDebug("Failed to set default encoding to utf-8.\n");
		PyErr_Clear();
	}

	scripterCore = new ScripterCore(ScCore->primaryMainWindow());
	Q_CHECK_PTR(scripterCore);
	initscribus(ScCore->primaryMainWindow());
#ifdef HAVE_SCRIPTER2
	scripter2_init();
#endif
	scripterCore->setupMainInterpreter();
	scripterCore->initExtensionScripts();
	scripterCore->runStartupScript();
	return true;
}

bool ScriptPlugin::cleanupPlugin()
{
	if (scripterCore)
	{
		delete scripterCore;
		scripterCore = NULL;
	}
	Py_Finalize();
	return true;
}

bool ScriptPlugin::newPrefsPanelWidget(QWidget* parent,
									   PrefsPanel*& panel,
									   QString& caption,
									   QPixmap& icon)
{
	panel = new ScripterPrefsGui(parent);
	Q_CHECK_PTR(panel);
	caption = tr("Scripter");
	icon = loadIcon("python.png");
	return true;
}

/*  TEMPORARILY DISABLED
void run()
{
	QString pfad = ScPaths::instance().docDir();
	QString pfad2;
	pfad2 = QDir::convertSeparators(pfad + "en/Scripter/index.html");
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
	char *Name = NULL;
	if (!PyArg_ParseTuple(args, (char*)"s", &Name))
		return NULL;
	// Because sysdefaultencoding is not utf-8, Python is returning utf-8 encoded
	// 8-bit char* strings. Make sure Qt understands that the input is utf-8 not
	// the default local encoding (usually latin-1) by using QString::fromUtf8()
	/*RetString = QString::fromUtf8(Name);
	RetVal = retV;*/
	scripterCore->returnString = QString::fromUtf8(Name);
	return PyInt_FromLong(0L);
}

/*static */PyObject *scribus_getval(PyObject* /*self*/)
{
	return PyString_FromString(scripterCore->inValue.toUtf8().data());
}

/*! \brief Translate a docstring. Small helper function for use with the
 * PyMethodDef struct.
 */
char* tr(const char* docstringConstant)
{
	// Alas, there's a lot of wasteful string copying going on
	// here.
	QString translated = QObject::tr(docstringConstant, "scripter docstring");
	// pv - hack for ugly formating in console removing
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
	{const_cast<char*>("changeColor"), scribus_setcolor, METH_VARARGS, tr(scribus_setcolor__doc__)},
	{const_cast<char*>("closeDoc"), (PyCFunction)scribus_closedoc, METH_NOARGS, tr(scribus_closedoc__doc__)},
	{const_cast<char*>("closeMasterPage"), (PyCFunction)scribus_closemasterpage, METH_NOARGS, tr(scribus_closemasterpage__doc__)},
	{const_cast<char*>("createBezierLine"), scribus_bezierline, METH_VARARGS, tr(scribus_bezierline__doc__)},
	{const_cast<char*>("createEllipse"), scribus_newellipse, METH_VARARGS, tr(scribus_newellipse__doc__)},
	{const_cast<char*>("createImage"), scribus_newimage, METH_VARARGS, tr(scribus_newimage__doc__)},
	{const_cast<char*>("createLayer"), scribus_createlayer, METH_VARARGS, tr(scribus_createlayer__doc__)},
	{const_cast<char*>("createLine"), scribus_newline, METH_VARARGS, tr(scribus_newline__doc__)},
	{const_cast<char*>("createMasterPage"), scribus_createmasterpage, METH_VARARGS, tr(scribus_createmasterpage__doc__)},
	{const_cast<char*>("createPathText"), scribus_pathtext, METH_VARARGS, tr(scribus_pathtext__doc__)},
	{const_cast<char*>("createPolygon"), scribus_polygon, METH_VARARGS, tr(scribus_polygon__doc__)},
	{const_cast<char*>("createPolyLine"), scribus_polyline, METH_VARARGS, tr(scribus_polyline__doc__)},
	{const_cast<char*>("createRect"), scribus_newrect, METH_VARARGS, tr(scribus_newrect__doc__)},
	{const_cast<char*>("createText"), scribus_newtext, METH_VARARGS, tr(scribus_newtext__doc__)},
	{const_cast<char*>("createParagraphStyle"), (PyCFunction)scribus_createparagraphstyle, METH_KEYWORDS, tr(scribus_createparagraphstyle__doc__)},
	{const_cast<char*>("createCharStyle"), (PyCFunction)scribus_createcharstyle, METH_KEYWORDS, tr(scribus_createcharstyle__doc__)},
	{const_cast<char*>("currentPage"), (PyCFunction)scribus_actualpage, METH_NOARGS, tr(scribus_actualpage__doc__)},
	{const_cast<char*>("defineColor"), scribus_newcolor, METH_VARARGS, tr(scribus_newcolor__doc__)},
	{const_cast<char*>("deleteColor"), scribus_delcolor, METH_VARARGS, tr(scribus_delcolor__doc__)},
	{const_cast<char*>("deleteLayer"), scribus_removelayer, METH_VARARGS, tr(scribus_removelayer__doc__)},
	{const_cast<char*>("deleteMasterPage"), scribus_deletemasterpage, METH_VARARGS, tr(scribus_deletemasterpage__doc__)},
	{const_cast<char*>("deleteObject"), scribus_deleteobj, METH_VARARGS, tr(scribus_deleteobj__doc__)},
	{const_cast<char*>("deletePage"), scribus_deletepage, METH_VARARGS, tr(scribus_deletepage__doc__)},
	{const_cast<char*>("deleteText"), scribus_deletetext, METH_VARARGS, tr(scribus_deletetext__doc__)},
	{const_cast<char*>("deselectAll"), (PyCFunction)scribus_deselect, METH_NOARGS, tr(scribus_deselect__doc__)},
	{const_cast<char*>("docChanged"), scribus_docchanged, METH_VARARGS, tr(scribus_docchanged__doc__)},
	{const_cast<char*>("editMasterPage"), scribus_editmasterpage, METH_VARARGS, tr(scribus_editmasterpage__doc__)},
	{const_cast<char*>("fileDialog"), (PyCFunction)scribus_filedia, METH_VARARGS|METH_KEYWORDS, tr(scribus_filedia__doc__)},
	{const_cast<char*>("getActiveLayer"), (PyCFunction)scribus_getactlayer, METH_NOARGS, tr(scribus_getactlayer__doc__)},
	{const_cast<char*>("getAllObjects"), scribus_getallobj, METH_VARARGS, tr(scribus_getallobj__doc__)},
	{const_cast<char*>("getAllStyles"), (PyCFunction)scribus_getstylenames, METH_NOARGS, tr(scribus_getstylenames__doc__)},
	{const_cast<char*>("getAllText"), scribus_gettext, METH_VARARGS, tr(scribus_gettext__doc__)},
	{const_cast<char*>("getColorNames"), (PyCFunction)scribus_colornames, METH_NOARGS, tr(scribus_colornames__doc__)},
	{const_cast<char*>("getColor"), scribus_getcolor, METH_VARARGS, tr(scribus_getcolor__doc__)},
	{const_cast<char*>("getColorAsRGB"), scribus_getcolorasrgb, METH_VARARGS, tr(scribus_getcolorasrgb__doc__)},
	{const_cast<char*>("isSpotColor"), scribus_isspotcolor, METH_VARARGS, tr(scribus_isspotcolor__doc__)},
	{const_cast<char*>("setSpotColor"), scribus_setspotcolor, METH_VARARGS, tr(scribus_setspotcolor__doc__)},
	{const_cast<char*>("getTextDistances"), scribus_gettextdistances, METH_VARARGS, tr(scribus_gettextdistances__doc__)},
	{const_cast<char*>("getColumnGap"), scribus_getcolumngap, METH_VARARGS, tr(scribus_getcolumngap__doc__)},
	{const_cast<char*>("getColumns"), scribus_getcolumns, METH_VARARGS, tr(scribus_getcolumns__doc__)},
	{const_cast<char*>("getCornerRadius"), scribus_getcornerrad, METH_VARARGS, tr(scribus_getcornerrad__doc__)},
	{const_cast<char*>("getFillColor"), scribus_getfillcolor, METH_VARARGS, tr(scribus_getfillcolor__doc__)},
	{const_cast<char*>("getFillShade"), scribus_getfillshade, METH_VARARGS, tr(scribus_getfillshade__doc__)},
	{const_cast<char*>("getFillBlendmode"), scribus_getfillblend, METH_VARARGS, tr(scribus_getfillblend__doc__)},
	{const_cast<char*>("getFillTransparency"), scribus_getfilltrans, METH_VARARGS, tr(scribus_getfilltrans__doc__)},
	{const_cast<char*>("getFontNames"), (PyCFunction)scribus_fontnames, METH_NOARGS, tr(scribus_fontnames__doc__)},
	{const_cast<char*>("getFont"), scribus_getfont, METH_VARARGS, tr(scribus_getfont__doc__)},
	{const_cast<char*>("getFontSize"), scribus_getfontsize, METH_VARARGS, tr(scribus_getfontsize__doc__)},
	{const_cast<char*>("getGuiLanguage"), (PyCFunction)scribus_getlanguage, METH_NOARGS, tr(scribus_getlanguage__doc__)},
	{const_cast<char*>("getHGuides"), (PyCFunction)scribus_getHguides, METH_NOARGS, tr(scribus_getHguides__doc__)},
	{const_cast<char*>("getImageFile"), scribus_getimgname, METH_VARARGS, tr(scribus_getimgname__doc__)},
	{const_cast<char*>("getImageScale"), scribus_getimgscale, METH_VARARGS, tr(scribus_getimgscale__doc__)},
	{const_cast<char*>("getLayers"), (PyCFunction)scribus_getlayers, METH_NOARGS, tr(scribus_getlayers__doc__)},
	{const_cast<char*>("getLayerBlendmode"), scribus_glayerblend, METH_VARARGS, tr(scribus_glayerblend__doc__)},
	{const_cast<char*>("getLayerTransparency"), scribus_glayertrans, METH_VARARGS, tr(scribus_glayertrans__doc__)},
	{const_cast<char*>("getLineCap"), scribus_getlineend, METH_VARARGS, tr(scribus_getlineend__doc__)},
	{const_cast<char*>("getLineColor"), scribus_getlinecolor, METH_VARARGS, tr(scribus_getlinecolor__doc__)},
	{const_cast<char*>("getLineShade"), scribus_getlineshade, METH_VARARGS, tr(scribus_getlineshade__doc__)},
	{const_cast<char*>("getLineBlendmode"), scribus_getlineblend, METH_VARARGS, tr(scribus_getlineblend__doc__)},
	{const_cast<char*>("getLineTransparency"), scribus_getlinetrans, METH_VARARGS, tr(scribus_getlinetrans__doc__)},
	{const_cast<char*>("getLineJoin"), scribus_getlinejoin, METH_VARARGS, tr(scribus_getlinejoin__doc__)},
	{const_cast<char*>("getLineSpacing"), scribus_getlinespace, METH_VARARGS, tr(scribus_getlinespace__doc__)},
	{const_cast<char*>("getLineStyle"), scribus_getlinestyle, METH_VARARGS, tr(scribus_getlinestyle__doc__)},
	{const_cast<char*>("getLineWidth"), scribus_getlinewidth, METH_VARARGS, tr(scribus_getlinewidth__doc__)},
	{const_cast<char*>("getPageItems"), (PyCFunction)scribus_getpageitems, METH_NOARGS, tr(scribus_getpageitems__doc__)},
	{const_cast<char*>("getPageMargins"), (PyCFunction)scribus_getpagemargins, METH_NOARGS, tr(scribus_getpagemargins__doc__)},
	{const_cast<char*>("getPageType"), (PyCFunction)scribus_pageposition, METH_VARARGS, tr(scribus_pageposition__doc__)},
	{const_cast<char*>("getPageSize"), (PyCFunction)scribus_pagedimension, METH_NOARGS, tr(scribus_pagedimension__doc__)}, // just an alias to PageDimension()
	{const_cast<char*>("importPage"), scribus_importpage, METH_VARARGS, tr(scribus_importpage__doc__)},
	{const_cast<char*>("getPosition"), scribus_getposi, METH_VARARGS, tr(scribus_getposi__doc__)},
	{const_cast<char*>("getRotation"), scribus_getrotation, METH_VARARGS, tr(scribus_getrotation__doc__)},
	{const_cast<char*>("getSelectedObject"), scribus_getselobjnam, METH_VARARGS, tr(scribus_getselobjnam__doc__)},
	{const_cast<char*>("getSize"), scribus_getsize, METH_VARARGS, tr(scribus_getsize__doc__)},
	{const_cast<char*>("getTextColor"), scribus_getlinecolor, METH_VARARGS, tr(scribus_getlinecolor__doc__)},
	{const_cast<char*>("getTextLength"), scribus_gettextsize, METH_VARARGS, tr(scribus_gettextsize__doc__)},
	{const_cast<char*>("getTextLines"), scribus_gettextlines, METH_VARARGS, tr(scribus_gettextlines__doc__)},
	{const_cast<char*>("getText"), scribus_getframetext, METH_VARARGS, tr(scribus_getframetext__doc__)},
	{const_cast<char*>("getTextShade"), scribus_getlineshade, METH_VARARGS, tr(scribus_getlineshade__doc__)},
	{const_cast<char*>("getUnit"), (PyCFunction)scribus_getunit, METH_NOARGS, tr(scribus_getunit__doc__)},
	{const_cast<char*>("getVGuides"), (PyCFunction)scribus_getVguides, METH_NOARGS, tr(scribus_getVguides__doc__)},
	{const_cast<char*>("getXFontNames"), (PyCFunction)scribus_xfontnames, METH_NOARGS, tr(scribus_xfontnames__doc__)},
	{const_cast<char*>("gotoPage"), scribus_gotopage, METH_VARARGS, tr(scribus_gotopage__doc__)},
	{const_cast<char*>("groupObjects"), scribus_groupobj, METH_VARARGS, tr(scribus_groupobj__doc__)},
	{const_cast<char*>("haveDoc"), (PyCFunction)scribus_havedoc, METH_NOARGS, tr(scribus_havedoc__doc__)},
	{const_cast<char*>("placeSVG"), scribus_placesvg, METH_VARARGS, tr(scribus_placesvg__doc__)},
	{const_cast<char*>("placeEPS"), scribus_placeeps, METH_VARARGS, tr(scribus_placeeps__doc__)},
	{const_cast<char*>("placeSXD"), scribus_placesxd, METH_VARARGS, tr(scribus_placesxd__doc__)},
	{const_cast<char*>("placeODG"), scribus_placeodg, METH_VARARGS, tr(scribus_placeodg__doc__)},
	{const_cast<char*>("insertText"), scribus_inserttext, METH_VARARGS, tr(scribus_inserttext__doc__)},
	{const_cast<char*>("isLayerPrintable"), scribus_glayerprint, METH_VARARGS, tr(scribus_glayerprint__doc__)},
	{const_cast<char*>("isLayerVisible"), scribus_glayervisib, METH_VARARGS, tr(scribus_glayervisib__doc__)},
	{const_cast<char*>("isLayerLocked"), scribus_glayerlock, METH_VARARGS, tr(scribus_glayerlock__doc__)},
	{const_cast<char*>("isLayerOutlined"), scribus_glayeroutline, METH_VARARGS, tr(scribus_glayeroutline__doc__)},
	{const_cast<char*>("isLayerFlow"), scribus_glayerflow, METH_VARARGS, tr(scribus_glayerflow__doc__)},
	{const_cast<char*>("isLocked"), scribus_islocked, METH_VARARGS, tr(scribus_islocked__doc__)},
	{const_cast<char*>("linkTextFrames"), scribus_linktextframes, METH_VARARGS, tr(scribus_linktextframes__doc__)},
	{const_cast<char*>("loadImage"), scribus_loadimage, METH_VARARGS, tr(scribus_loadimage__doc__)},
	{const_cast<char*>("loadStylesFromFile"), scribus_loadstylesfromfile, METH_VARARGS, tr(scribus_loadstylesfromfile__doc__)},
	{const_cast<char*>("lockObject"), scribus_lockobject, METH_VARARGS, tr(scribus_lockobject__doc__)},
	{const_cast<char*>("masterPageNames"), (PyCFunction)scribus_masterpagenames, METH_NOARGS, tr(scribus_masterpagenames__doc__)},
	{const_cast<char*>("messagebarText"), scribus_messagebartext, METH_VARARGS, tr(scribus_messagebartext__doc__)},
	{const_cast<char*>("messageBox"), (PyCFunction)scribus_messdia, METH_VARARGS|METH_KEYWORDS, tr(scribus_messdia__doc__)},
	{const_cast<char*>("moveObjectAbs"), scribus_moveobjabs, METH_VARARGS, tr(scribus_moveobjabs__doc__)},
	{const_cast<char*>("moveObject"), scribus_moveobjrel, METH_VARARGS, tr(scribus_moveobjrel__doc__)},
	{const_cast<char*>("newDocDialog"), (PyCFunction)scribus_newdocdia, METH_NOARGS, tr(scribus_newdocdia__doc__)},
	{const_cast<char*>("newDoc"), scribus_newdoc, METH_VARARGS, tr(scribus_newdoc__doc__)},
	{const_cast<char*>("newDocument"), scribus_newdocument, METH_VARARGS, tr(scribus_newdocument__doc__)},
	{const_cast<char*>("newPage"), scribus_newpage, METH_VARARGS, tr(scribus_newpage__doc__)},
	{const_cast<char*>("newStyleDialog"), scribus_newstyledialog, METH_NOARGS, tr(scribus_newstyledialog__doc__)},
	{const_cast<char*>("objectExists"),scribus_objectexists, METH_VARARGS, tr(scribus_objectexists__doc__)},
	{const_cast<char*>("openDoc"), scribus_opendoc, METH_VARARGS, tr(scribus_opendoc__doc__)},
	{const_cast<char*>("pageCount"), (PyCFunction)scribus_pagecount, METH_NOARGS, tr(scribus_pagecount__doc__)},
	{const_cast<char*>("pageDimension"), (PyCFunction)scribus_pagedimension, METH_NOARGS, "Obsolete function. Don't use it."},
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
	{const_cast<char*>("setImageScale"), scribus_setimagescale, METH_VARARGS, tr(scribus_setimagescale__doc__)},
	{const_cast<char*>("selectionCount"), (PyCFunction)scribus_selcount, METH_NOARGS, tr(scribus_selcount__doc__)},
	{const_cast<char*>("selectObject"), scribus_selectobj, METH_VARARGS, tr(scribus_selectobj__doc__)},
	{const_cast<char*>("selectText"), scribus_selecttext, METH_VARARGS, tr(scribus_selecttext__doc__)},
	{const_cast<char*>("sentToLayer"), scribus_senttolayer, METH_VARARGS, tr(scribus_senttolayer__doc__)},
	{const_cast<char*>("setActiveLayer"), scribus_setactlayer, METH_VARARGS, tr(scribus_setactlayer__doc__)},
	{const_cast<char*>("setPDFBookmark"), scribus_setpdfbookmark, METH_VARARGS, tr(scribus_setpdfbookmark__doc__)},
	{const_cast<char*>("isPDFBookmark"), scribus_ispdfbookmark, METH_VARARGS, tr(scribus_ispdfbookmark__doc__)},
	{const_cast<char*>("setTextDistances"), scribus_settextdistances, METH_VARARGS, tr(scribus_settextdistances__doc__)},
	{const_cast<char*>("setColumnGap"), scribus_setcolumngap, METH_VARARGS, tr(scribus_setcolumngap__doc__)},
	{const_cast<char*>("setColumns"), scribus_setcolumns, METH_VARARGS, tr(scribus_setcolumns__doc__)},
	{const_cast<char*>("setCornerRadius"), scribus_setcornerrad, METH_VARARGS, tr(scribus_setcornerrad__doc__)},
	{const_cast<char*>("setCursor"), scribus_setcursor, METH_VARARGS, tr(scribus_setcursor__doc__)},
	{const_cast<char*>("setDocType"), scribus_setdoctype, METH_VARARGS, tr(scribus_setdoctype__doc__)},
	{const_cast<char*>("setFillColor"), scribus_setfillcolor, METH_VARARGS, tr(scribus_setfillcolor__doc__)},
	{const_cast<char*>("setFillTransparency"), scribus_setfilltrans, METH_VARARGS, tr(scribus_setfilltrans__doc__)},
	{const_cast<char*>("setFillBlendmode"), scribus_setfillblend, METH_VARARGS, tr(scribus_setfillblend__doc__)},
	{const_cast<char*>("setFillShade"), scribus_setfillshade, METH_VARARGS, tr(scribus_setfillshade__doc__)},
	{const_cast<char*>("setFont"), scribus_setfont, METH_VARARGS, tr(scribus_setfont__doc__)},
	{const_cast<char*>("setFontSize"), scribus_setfontsize, METH_VARARGS, tr(scribus_setfontsize__doc__)},
	{const_cast<char*>("setGradientFill"), scribus_setgradfill, METH_VARARGS, tr(scribus_setgradfill__doc__)},
	{const_cast<char*>("setGradientStop"), scribus_setgradstop, METH_VARARGS, tr(scribus_setgradstop__doc__)},
	{const_cast<char*>("setHGuides"), scribus_setHguides, METH_VARARGS, tr(scribus_setHguides__doc__)},
	{const_cast<char*>("setInfo"), scribus_setinfo, METH_VARARGS, tr(scribus_setinfo__doc__)},
	{const_cast<char*>("setLayerPrintable"), scribus_layerprint, METH_VARARGS, tr(scribus_layerprint__doc__)},
	{const_cast<char*>("setLayerVisible"), scribus_layervisible, METH_VARARGS, tr(scribus_layervisible__doc__)},
	{const_cast<char*>("setLayerLocked"), scribus_layerlock, METH_VARARGS, tr(scribus_layerlock__doc__)},
	{const_cast<char*>("setLayerOutlined"), scribus_layeroutline, METH_VARARGS, tr(scribus_layeroutline__doc__)},
	{const_cast<char*>("setLayerFlow"), scribus_layerflow, METH_VARARGS, tr(scribus_layerflow__doc__)},
	{const_cast<char*>("setLayerBlendmode"), scribus_layerblend, METH_VARARGS, tr(scribus_layerblend__doc__)},
	{const_cast<char*>("setLayerTransparency"), scribus_layertrans, METH_VARARGS, tr(scribus_layertrans__doc__)},
	{const_cast<char*>("setLineCap"), scribus_setlineend, METH_VARARGS, tr(scribus_setlineend__doc__)},
	{const_cast<char*>("setLineColor"), scribus_setlinecolor, METH_VARARGS, tr(scribus_setlinecolor__doc__)},
	{const_cast<char*>("setLineTransparency"), scribus_setlinetrans, METH_VARARGS, tr(scribus_setlinetrans__doc__)},
	{const_cast<char*>("setLineBlendmode"), scribus_setlineblend, METH_VARARGS, tr(scribus_setlineblend__doc__)},
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
	{const_cast<char*>("hyphenateText"), scribus_hyphenatetext, METH_VARARGS, tr(scribus_hyphenatetext__doc__)},
	{const_cast<char*>("dehyphenateText"), scribus_dehyphenatetext, METH_VARARGS, tr(scribus_dehyphenatetext__doc__)},
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
	{const_cast<char*>("textFlowMode"), scribus_textflow, METH_VARARGS, tr(scribus_textflow__doc__)},
	{const_cast<char*>("traceText"), scribus_tracetext, METH_VARARGS, tr(scribus_tracetext__doc__)},
	{const_cast<char*>("unGroupObject"), scribus_ungroupobj, METH_VARARGS, tr(scribus_ungroupobj__doc__)},
	{const_cast<char*>("unlinkTextFrames"), scribus_unlinktextframes, METH_VARARGS, tr(scribus_unlinktextframes__doc__)},
	{const_cast<char*>("valueDialog"), scribus_valdialog, METH_VARARGS, tr(scribus_valdialog__doc__)},
	{const_cast<char*>("textOverflows"), (PyCFunction)scribus_istextoverflowing, METH_KEYWORDS, tr(scribus_istextoverflowing__doc__)},
	{const_cast<char*>("zoomDocument"), scribus_zoomdocument, METH_VARARGS, tr(scribus_zoomdocument__doc__)},
	{const_cast<char*>("scrollDocument"), scribus_scrolldocument, METH_VARARGS, tr(scribus_scrolldocument__doc__)},
	{const_cast<char*>("moveSelectionToBack"), (PyCFunction)scribus_moveselectiontoback, METH_NOARGS, tr(scribus_moveselectiontoback__doc__)},
	{const_cast<char*>("moveSelectionToFront"), (PyCFunction)scribus_moveselectiontofront, METH_NOARGS, tr(scribus_moveselectiontofront__doc__)},
	// Property magic
	{const_cast<char*>("getPropertyCType"), (PyCFunction)scribus_propertyctype, METH_KEYWORDS, tr(scribus_propertyctype__doc__)},
	{const_cast<char*>("getPropertyNames"), (PyCFunction)scribus_getpropertynames, METH_KEYWORDS, tr(scribus_getpropertynames__doc__)},
	{const_cast<char*>("getProperty"), (PyCFunction)scribus_getproperty, METH_KEYWORDS, tr(scribus_getproperty__doc__)},
	{const_cast<char*>("setProperty"), (PyCFunction)scribus_setproperty, METH_KEYWORDS, tr(scribus_setproperty__doc__)},
// 	{const_cast<char*>("getChildren"), (PyCFunction)scribus_getchildren, METH_KEYWORDS, tr(scribus_getchildren__doc__)},
// 	{const_cast<char*>("getChild"), (PyCFunction)scribus_getchild, METH_KEYWORDS, tr(scribus_getchild__doc__)},
	// by Christian Hausknecht
	{const_cast<char*>("duplicateObject"), scribus_duplicateobject, METH_VARARGS, tr(scribus_duplicateobject__doc__)},
	// Internal methods - Not for public use
	{const_cast<char*>("retval"), (PyCFunction)scribus_retval, METH_VARARGS, const_cast<char*>("Scribus internal.")},
	{const_cast<char*>("getval"), (PyCFunction)scribus_getval, METH_NOARGS, const_cast<char*>("Scribus internal.")},
	{NULL, (PyCFunction)(0), 0, NULL} /* sentinel */
};

void initscribus_failed(const char* fileName, int lineNo)
{
	qDebug("Scripter setup failed (%s:%i)", fileName, lineNo);
	if (PyErr_Occurred())
		PyErr_Print();
	return;
}

void initscribus(ScribusMainWindow *pl)
{
	if (!scripterCore)
	{
		qWarning("scriptplugin: Tried to init scribus module, but no scripter core. Aborting.");
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
	// Done with exception setup

	// CONSTANTS
	PyDict_SetItemString(d, const_cast<char*>("UNIT_POINTS"), PyInt_FromLong(unitIndexFromString("pt")));
	PyDict_SetItemString(d, const_cast<char*>("UNIT_MILLIMETERS"), PyInt_FromLong(unitIndexFromString("mm")));
	PyDict_SetItemString(d, const_cast<char*>("UNIT_INCHES"), PyInt_FromLong(unitIndexFromString("in")));
	PyDict_SetItemString(d, const_cast<char*>("UNIT_PICAS"), PyInt_FromLong(unitIndexFromString("p")));
	PyDict_SetItemString(d, const_cast<char*>("UNIT_CENTIMETRES"), PyInt_FromLong(unitIndexFromString("cm")));
	PyDict_SetItemString(d, const_cast<char*>("UNIT_CICERO"), PyInt_FromLong(unitIndexFromString("c")));
	PyDict_SetItemString(d, const_cast<char*>("PORTRAIT"), Py_BuildValue(const_cast<char*>("i"), portraitPage));
	PyDict_SetItemString(d, const_cast<char*>("LANDSCAPE"), Py_BuildValue(const_cast<char*>("i"), landscapePage));
	PyDict_SetItemString(d, const_cast<char*>("NOFACINGPAGES"), Py_BuildValue(const_cast<char*>("i"), 0));
	PyDict_SetItemString(d, const_cast<char*>("FACINGPAGES"),  Py_BuildValue(const_cast<char*>("i"), 1));
	PyDict_SetItemString(d, const_cast<char*>("FIRSTPAGERIGHT"), Py_BuildValue(const_cast<char*>("i"), 1));
	PyDict_SetItemString(d, const_cast<char*>("FIRSTPAGELEFT"), Py_BuildValue(const_cast<char*>("i"), 0));
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
	PyDict_SetItemString(d, const_cast<char*>("NORMAL"), Py_BuildValue(const_cast<char*>("i"), 0));
	PyDict_SetItemString(d, const_cast<char*>("DARKEN"), Py_BuildValue(const_cast<char*>("i"), 1));
	PyDict_SetItemString(d, const_cast<char*>("LIGHTEN"), Py_BuildValue(const_cast<char*>("i"), 2));
	PyDict_SetItemString(d, const_cast<char*>("MULTIPLY"), Py_BuildValue(const_cast<char*>("i"), 3));
	PyDict_SetItemString(d, const_cast<char*>("SCREEN"), Py_BuildValue(const_cast<char*>("i"), 4));
	PyDict_SetItemString(d, const_cast<char*>("OVERLAY"), Py_BuildValue(const_cast<char*>("i"), 5));
	PyDict_SetItemString(d, const_cast<char*>("HARD_LIGHT"), Py_BuildValue(const_cast<char*>("i"), 6));
	PyDict_SetItemString(d, const_cast<char*>("SOFT_LIGHT"), Py_BuildValue(const_cast<char*>("i"), 7));
	PyDict_SetItemString(d, const_cast<char*>("DIFFERENCE"), Py_BuildValue(const_cast<char*>("i"), 8));
	PyDict_SetItemString(d, const_cast<char*>("EXCLUSION"), Py_BuildValue(const_cast<char*>("i"), 9));
	PyDict_SetItemString(d, const_cast<char*>("COLOR_DODGE"), Py_BuildValue(const_cast<char*>("i"), 10));
	PyDict_SetItemString(d, const_cast<char*>("COLOR_BURN"), Py_BuildValue(const_cast<char*>("i"), 11));
	PyDict_SetItemString(d, const_cast<char*>("HUE"), Py_BuildValue(const_cast<char*>("i"), 12));
	PyDict_SetItemString(d, const_cast<char*>("SATURATION"), Py_BuildValue(const_cast<char*>("i"), 13));
	PyDict_SetItemString(d, const_cast<char*>("COLOR"), Py_BuildValue(const_cast<char*>("i"), 14));
	PyDict_SetItemString(d, const_cast<char*>("LUMINOSITY"), Py_BuildValue(const_cast<char*>("i"), 15));
	// preset page layouts
	PyDict_SetItemString(d, const_cast<char*>("PAGE_1"), Py_BuildValue(const_cast<char*>("i"), 0));
	PyDict_SetItemString(d, const_cast<char*>("PAGE_2"), Py_BuildValue(const_cast<char*>("i"), 1));
	PyDict_SetItemString(d, const_cast<char*>("PAGE_3"), Py_BuildValue(const_cast<char*>("i"), 2));
	PyDict_SetItemString(d, const_cast<char*>("PAGE_4"), Py_BuildValue(const_cast<char*>("i"), 3));

	// Measurement units understood by Scribus's units.cpp functions are exported as constant conversion
	// factors to be used from Python.
	for (int i = 0; i <= unitGetMaxIndex(); ++i)
	{
		PyObject* value = PyFloat_FromDouble(unitGetRatioFromIndex(i));
		if (!value)
		{
			initscribus_failed(__FILE__, __LINE__);
			return;
		}
		// `in' is a reserved word in Python so we must replace it
		PyObject* name;
		if (unitGetStrFromIndex(i) == "in")
			name = PyString_FromString("inch");
		else
			name = PyString_FromString(unitGetStrFromIndex(i).toAscii().constData());
		if (!name)
		{
			initscribus_failed(__FILE__, __LINE__);
			return;
		}
		if (PyDict_SetItem(d, name, value))
		{
			initscribus_failed(__FILE__, __LINE__);
			return;
		}
	}

	// Export the Scribus version into the module namespace so scripts know what they're running in
	PyDict_SetItemString(d, const_cast<char*>("scribus_version"), PyString_FromString(const_cast<char*>(VERSION)));
	// Now build a version tuple like that provided by Python in sys.version_info
	// The tuple is of the form (major, minor, patchlevel, extraversion, reserved)
	QRegExp version_re("(\\d+)\\.(\\d+)\\.(\\d+)(.*)");
	int pos = version_re.indexIn(QString(VERSION));
	// We ignore errors, causing the scribus_version_info attribute to simply not be created.
	// This will make acceses raise AttrbuteError.
	if (pos > -1)
	{
		int majorVersion = version_re.cap(1).toInt();
		int minorVersion = version_re.cap(2).toInt();
		int patchVersion = version_re.cap(3).toInt();
		QString extraVersion = version_re.cap(4);
		PyObject* versionTuple = Py_BuildValue(const_cast<char*>("(iiisi)"),\
				majorVersion, minorVersion, patchVersion, (const char*)extraVersion.toUtf8(), 0);
		if (versionTuple != NULL)
			PyDict_SetItemString(d, const_cast<char*>("scribus_version_info"), versionTuple);
		else
			qDebug("Failed to build version tuple for version string '%s' in scripter", VERSION);
	}
	else
		qDebug("Couldn't parse version string '%s' in scripter", VERSION);

// 	ScMW = pl;
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

	PyObject* docStr = PyString_FromString(docstring.toUtf8().data());
	if (!docStr)
		qDebug("Failed to create module-level docstring (couldn't make str)");
	else
	{
		PyObject* uniDocStr = PyUnicode_FromEncodedObject(docStr, "utf-8", NULL);
		Py_DECREF(docStr);
		docStr = NULL;
		if (!uniDocStr)
			qDebug("Failed to create module-level docstring object (couldn't make unicode)");
		else
			PyDict_SetItemString(d, const_cast<char*>("__doc__"), uniDocStr);
		Py_DECREF(uniDocStr);
		uniDocStr = NULL;
	}

	// Wrap up pointers to the the QApp and main window and push them out
	// to Python.
	wrappedQApp = wrapQObject(qApp);
	if (!wrappedQApp)
	{
		qDebug("Failed to wrap up QApp");
		PyErr_Print();
	}
	// Push it into the module dict, stealing a ref in the process
	PyDict_SetItemString(d, const_cast<char*>("qApp"), wrappedQApp);
	Py_DECREF(wrappedQApp);
	wrappedQApp = NULL;

	wrappedMainWindow = wrapQObject(pl);
	if (!wrappedMainWindow)
	{
		qDebug("Failed to wrap up ScMW");
		PyErr_Print();
	}
	// Push it into the module dict, stealing a ref in the process
	PyDict_SetItemString(d, const_cast<char*>("mainWindow"), wrappedMainWindow);
	Py_DECREF(wrappedMainWindow);
	wrappedMainWindow = NULL;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void scriptplugindocwarnings()
{
    QStringList s;
    s <<printer__doc__<<pdffile__doc__<<imgexp__doc__<<imgexp_dpi__doc__<<imgexp_scale__doc__ <<imgexp_quality__doc__<<imgexp_filename__doc__<<imgexp_type__doc__<<imgexp_alltypes__doc__ << imgexp_save__doc__ << imgexp_saveas__doc__;
}

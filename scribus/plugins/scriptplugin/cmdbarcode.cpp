/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdbarcode.h"

#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

#include "cmdutil.h"
#include "pyesstring.h"
#include "scpaths.h"
#include "scraction.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribus.h"
#include "util_ghostscript.h"


// Run GS on the barcode PS file to capture any BWIPP error message.
// Only called after a failed import to extract the error detail.
static QString getBwippError()
{
	QString psFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "bcode.ps");
	QString errFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "bcode_err.txt");

	QStringList gargs;
	gargs.append("-dDEVICEWIDTHPOINTS=1");
	gargs.append("-dDEVICEHEIGHTPOINTS=1");
	gargs.append("-r36");
	gargs.append("-sOutputFile=%nulldevice%");
	gargs.append(psFile);

	callGS(gargs, QString(), errFile);

	QFile f(errFile);
	if (!f.open(QIODevice::ReadOnly))
		return {};
	QTextStream ts(&f);
	QString err = ts.readAll();
	f.close();
	QFile::remove(errFile);

	QRegularExpression rx("[\\r\\n]+BWIPP ERROR: [^\\s]+ (.*)[\\r\\n$]+",
						  QRegularExpression::InvertedGreedinessOption);
	QRegularExpressionMatch match = rx.match(err);
	if (match.hasMatch())
		return match.captured(1).trimmed();
	return {};
}


PyObject *scribus_createbarcode(PyObject* /* self */, PyObject* args)
{
	PyESString encoder, data, options, name;
	double x, y;

	if (!PyArg_ParseTuple(args, "esesesdd|es",
			"utf-8", encoder.ptr(),
			"utf-8", data.ptr(),
			"utf-8", options.ptr(),
			&x, &y,
			"utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	if (!ScCore->haveGS())
	{
		PyErr_SetString(ScribusException, QObject::tr("Ghostscript is not available.", "python error").toUtf8());
		return nullptr;
	}

	ScribusMainWindow* mw = ScCore->primaryMainWindow();
	ScribusDoc* doc = mw->doc;

	if (!mw->scrActions.contains("BarcodeGenerator"))
	{
		PyErr_SetString(ScribusException, QObject::tr("Barcode plugin is not available.", "python error").toUtf8());
		return nullptr;
	}

	// Store parameters for the barcode generator
	mw->pluginEditParams["bwipp-encoder"] = QString::fromUtf8(encoder.c_str());
	mw->pluginEditParams["bwipp-content"] = QString::fromUtf8(data.c_str());
	mw->pluginEditParams["bwipp-options"] = QString::fromUtf8(options.c_str());
	mw->pluginEditParams["_scriptedX"] = QString::number(pageUnitXToDocX(x));
	mw->pluginEditParams["_scriptedY"] = QString::number(pageUnitYToDocY(y));
	mw->pluginEditItem = nullptr;
	mw->pluginEditSilent = true;

	int itemsBefore = doc->Items->count();
	mw->scrActions["BarcodeGenerator"]->trigger();
	int itemsAfter = doc->Items->count();

	if (itemsAfter <= itemsBefore)
	{
		QString bwippErr = getBwippError();
		QString msg = bwippErr.isEmpty()
			? QObject::tr("Barcode generation failed.", "python error")
			: QObject::tr("Barcode generation failed: %1", "python error").arg(bwippErr);
		PyErr_SetString(ScribusException, msg.toUtf8());
		return nullptr;
	}

	PageItem* newItem = doc->Items->last();

	if (name.length() > 0)
	{
		QString objName = QString::fromUtf8(name.c_str());
		if (!ItemExists(objName))
			newItem->setItemName(objName);
	}

	return PyUnicode_FromString(newItem->itemName().toUtf8());
}

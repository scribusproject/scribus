/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmddoc.h"
#include "cmdutil.h"
#include "datamergesource.h"
#include "documentchecker.h"
#include "documentinformation.h"
#include "dynamicvariable.h"
#include "marks.h"
#include "pageitem.h"
#include "pyesstring.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "undomanager.h"
#include "util.h"
#include "units.h"

#include <QApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QSet>
#include <QVector>

namespace
{
QString dynamicVariableId(ScribusDoc* doc, const QString& identifier)
{
	if (DynamicVariableResolver::isBuiltInId(identifier))
		return DynamicVariableResolver::isKnownBuiltInId(identifier) ? identifier : QString();
	for (const DynamicVariable& variable : DynamicVariableResolver::builtInVariables())
	{
		if (variable.type == identifier)
			return variable.id;
	}
	if (doc->dynamicVariable(identifier))
		return identifier;
	return doc->dynamicVariableIdByName(identifier);
}

QString userDynamicVariableId(ScribusDoc* doc, const QString& identifier)
{
	const QString id = dynamicVariableId(doc, identifier);
	return (id.isEmpty() || DynamicVariableResolver::isBuiltInId(id)) ? QString() : id;
}

PyObject* dynamicVariableNotFound(const QString& identifier)
{
	PyErr_SetString(NotFoundError, QObject::tr("Dynamic variable '%1' was not found.", "python error").arg(identifier).toUtf8().constData());
	return nullptr;
}

PyObject* recordsToPython(const DataMergeSource& source, qsizetype limit)
{
	const qsizetype recordCount = limit < 0 ? source.recordCount() : qMin(limit, static_cast<qsizetype>(source.recordCount()));
	PyObject* result = PyList_New(recordCount);
	if (!result)
		return nullptr;
	for (qsizetype rowIndex = 0; rowIndex < recordCount; ++rowIndex)
	{
		PyObject* record = PyDict_New();
		if (!record)
		{
			Py_DECREF(result);
			return nullptr;
		}
		const QStringList& row = source.row(rowIndex);
		for (int column = 0; column < source.fields().size(); ++column)
		{
			const QByteArray keyUtf8 = source.fields().at(column).toUtf8();
			const QByteArray valueUtf8 = row.value(column).toUtf8();
			PyObject* key = PyUnicode_FromStringAndSize(keyUtf8.constData(), keyUtf8.size());
			PyObject* value = PyUnicode_FromStringAndSize(valueUtf8.constData(), valueUtf8.size());
			if (!key || !value || PyDict_SetItem(record, key, value) < 0)
			{
				Py_XDECREF(key);
				Py_XDECREF(value);
				Py_DECREF(record);
				Py_DECREF(result);
				return nullptr;
			}
			Py_DECREF(key);
			Py_DECREF(value);
		}
		PyList_SET_ITEM(result, rowIndex, record);
	}
	return result;
}
}

PyObject *scribus_newdocument(PyObject* /* self */, PyObject* args)
{
	double topMargin, bottomMargin, leftMargin, rightMargin;
	double pageWidth, pageHeight;
	int orientation, firstPageNr, unit, pagesType, firstPageOrder, numPages;
	int bindingDirection = 0;

	PyObject *p, *m;

	if ((!PyArg_ParseTuple(args, "OOiiiiii|i", &p, &m, &orientation,
											&firstPageNr, &unit,
											&pagesType,
											&firstPageOrder,
											&numPages, &bindingDirection)) ||
						(!PyArg_ParseTuple(p, "dd", &pageWidth, &pageHeight)) ||
						(!PyArg_ParseTuple(m, "dddd", &leftMargin, &rightMargin,
												&topMargin, &bottomMargin)))
		return nullptr;
	if (numPages <= 0)
		numPages = 1;
	if (pagesType == 0)
	{
		firstPageOrder = 0;
	}
	if (pagesType < firstPageOrder)
	{
		PyErr_SetString(ScribusException, QObject::tr("firstPageOrder is bigger than allowed.","python error").toUtf8().constData());
		return nullptr;
	}


	pageWidth  = value2pts(pageWidth, unit);
	pageHeight = value2pts(pageHeight, unit);
	if (orientation == 1)
	{
		double x = pageWidth;
		pageWidth = pageHeight;
		pageHeight = x;
	}
	leftMargin   = value2pts(leftMargin, unit);
	rightMargin  = value2pts(rightMargin, unit);
	topMargin    = value2pts(topMargin, unit);
	bottomMargin = value2pts(bottomMargin, unit);

	bool ret = ScCore->primaryMainWindow()->doFileNew(pageWidth, pageHeight,
								topMargin, leftMargin, rightMargin, bottomMargin,
								// autoframes. It's disabled in python
								// columnDistance, numberCols, autoframes,
								0, 1, false,
								pagesType, unit, firstPageOrder,
								orientation, firstPageNr, QSizeF(), true,
								numPages, true, 0, bindingDirection);
	ScCore->primaryMainWindow()->doc->setPageSetFirstPage(pagesType, firstPageOrder);

	return PyLong_FromLong(static_cast<long>(ret));
}

PyObject *scribus_newdoc(PyObject* /* self */, PyObject* args)
{
	qDebug("WARNING: newDoc() procedure is obsolete, it will be removed in a forthcoming release. Use newDocument() instead.");
	double b, h, lr, tpr, btr, rr, ebr;
	int unit, ds, fsl, fNr, ori;
	PyObject *p, *m;
	if ((!PyArg_ParseTuple(args, "OOiiiii", &p, &m, &ori, &fNr, &unit, &ds, &fsl)) ||
	        (!PyArg_ParseTuple(p, "dd", &b, &h)) ||
	        (!PyArg_ParseTuple(m, "dddd", &lr, &rr, &tpr, &btr)))
		return nullptr;
	b = value2pts(b, unit);
	h = value2pts(h, unit);
	if (ori == 1)
	{
		ebr = b;
		b = h;
		h = ebr;
	}
	/*! \todo Obsolete! In the case of no facing pages use only firstpageleft
	scripter is not new-page-size ready.
	What is it: don't allow to use wrong FSL constant in the case of
	onesided document. */
	if (ds != 1 && fsl > 0)
		fsl = 0;
	// end of hack

	tpr = value2pts(tpr, unit);
	lr  = value2pts(lr, unit);
	rr  = value2pts(rr, unit);
	btr = value2pts(btr, unit);
	bool ret = ScCore->primaryMainWindow()->doFileNew(b, h, tpr, lr, rr, btr, 0, 1, false, ds, unit, fsl, ori, fNr, QSizeF(), true);
	//	qApp->processEvents();
	return PyLong_FromLong(static_cast<long>(ret));
}

PyObject *scribus_getbleeds(PyObject */* self */, PyObject* /*args*/)
{
	if (!checkHaveDocument())
		return nullptr;

	const MarginStruct& bleeds = ScCore->primaryMainWindow()->doc->bleedsVal();

	return Py_BuildValue("(dddd)",
		PointToValue(bleeds.left()),
		PointToValue(bleeds.right()),
		PointToValue(bleeds.top()),
		PointToValue(bleeds.bottom()));
}

PyObject *scribus_setbleeds(PyObject */* self */, PyObject *args)
{
	double lr, tpr, btr, rr;
	if (!PyArg_ParseTuple(args, "dddd", &lr, &rr, &tpr, &btr))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	MarginStruct bleeds(ValueToPoint(tpr), ValueToPoint(lr), ValueToPoint(btr), ValueToPoint(rr));

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	currentDoc->setBleeds(bleeds);
	currentView->reformPages();
	currentDoc->setModified(true);
	currentView->DrawNew();
	Py_RETURN_NONE;
}

PyObject* scribus_getmargins(PyObject*/* self */, PyObject* /*args*/)
{
	if (!checkHaveDocument())
		return nullptr;

	const MarginStruct& margins = ScCore->primaryMainWindow()->doc->marginsVal();

	return Py_BuildValue("(dddd)",
		PointToValue(margins.left()),
		PointToValue(margins.right()),
		PointToValue(margins.top()),
		PointToValue(margins.bottom()));
}

PyObject *scribus_setmargins(PyObject* /* self */, PyObject* args)
{
	double lr, tpr, btr, rr;
	if (!PyArg_ParseTuple(args, "dddd", &lr, &rr, &tpr, &btr))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	MarginStruct margins(ValueToPoint(tpr), ValueToPoint(lr), ValueToPoint(btr), ValueToPoint(rr));

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	currentDoc->setMargins(margins);
	currentView->reformPages();
	currentDoc->setModified(true);
	currentView->GotoPage(currentDoc->currentPageNumber());
	currentView->DrawNew();

	Py_RETURN_NONE;
}

PyObject* scribus_getbaseline(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;

	const GuidesPrefs& guides = ScCore->primaryMainWindow()->doc->guidesPrefs();

	return Py_BuildValue("(dd)",
		PointToValue(guides.valueBaselineGrid),
		PointToValue(guides.offsetBaselineGrid));
}

PyObject *scribus_setbaseline(PyObject* /* self */, PyObject* args)
{
	double grid, offset;
	if (!PyArg_ParseTuple(args, "dd", &grid, &offset))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	currentDoc->guidesPrefs().valueBaselineGrid = ValueToPoint(grid);
	currentDoc->guidesPrefs().offsetBaselineGrid = ValueToPoint(offset);
	//currentView->reformPages();
	currentDoc->setModified(true);
	//currentView->GotoPage(currentDoc->currentPageNumber());
	currentView->DrawNew();

	Py_RETURN_NONE;
}

PyObject *scribus_closedoc(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->doc->setModified(false);
	bool ret = ScCore->primaryMainWindow()->slotFileClose();
	QApplication::processEvents();
	return PyLong_FromLong(static_cast<long>(ret));
}

PyObject *scribus_havedoc(PyObject* /* self */)
{
	return PyLong_FromLong(static_cast<long>(ScCore->primaryMainWindow()->HaveDoc));
}

PyObject *scribus_opendoc(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	bool ret = ScCore->primaryMainWindow()->loadDoc(QString::fromUtf8(name.c_str()));
	if (!ret)
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to open document: %1","python error").arg(name.c_str()).toUtf8().constData());
		return nullptr;
	}
	return PyBool_FromLong(static_cast<long>(true));
//	Py_INCREF(Py_True); // compatibility: return true, not none, on success
//	return Py_True;
//	Py_RETURN_TRUE;
}

PyObject *scribus_savedoc(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->slotFileSave();
	Py_RETURN_NONE;
}

PyObject *scribus_revertdoc(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->slotFileRevert();
	Py_RETURN_NONE;
}

PyObject *scribus_getdocname(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	if (! ScCore->primaryMainWindow()->doc->hasName)
	{
		return PyUnicode_FromString("");
	}
	return PyUnicode_FromString(ScCore->primaryMainWindow()->doc->documentFileName().toUtf8());
}

PyObject *scribus_savedocas(PyObject* /* self */, PyObject* args)
{
	PyESString fileName;
	if (!PyArg_ParseTuple(args, "es", "utf-8", fileName.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	bool ret = ScCore->primaryMainWindow()->DoFileSave(QString::fromUtf8(fileName.c_str()));
	if (!ret)
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to save document.","python error").toUtf8().constData());
		return nullptr;
	}
	return PyBool_FromLong(static_cast<long>(true));
//	Py_INCREF(Py_True); // compatibility: return true, not none, on success
//	return Py_True;
//	Py_RETURN_TRUE;
}

PyObject *scribus_setinfo(PyObject* /* self */, PyObject* args)
{
	char *Author;
	char *Title;
	char *Desc;
	// z means string, but None becomes a nullptr value. QString()
	// will correctly handle nullptr.
	if (!PyArg_ParseTuple(args, "zzz", &Author, &Title, &Desc))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	DocumentInformation& docInfo = ScCore->primaryMainWindow()->doc->documentInfo();
	docInfo.setAuthor(QString::fromUtf8(Author));
	docInfo.setTitle(QString::fromUtf8(Title));
	docInfo.setComments(QString::fromUtf8(Desc));
	ScCore->primaryMainWindow()->slotDocCh();

	Py_RETURN_NONE;
}

PyObject *scribus_getinfo(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	if (! ScCore->primaryMainWindow()->doc->hasName)
	{
		return PyUnicode_FromString("");
	}

	const DocumentInformation& docInfo = ScCore->primaryMainWindow()->doc->documentInfo();
	return Py_BuildValue("(sss)",
				docInfo.author().toUtf8().data(),
				docInfo.title().toUtf8().data(),
				docInfo.comments().toUtf8().data());
}

PyObject *scribus_setunit(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if ((e < UNITMIN) || (e > UNITMAX))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Unit out of range. Use one of the scribus.UNIT_* constants.","python error").toUtf8().constData());
		return nullptr;
	}
	ScCore->primaryMainWindow()->slotChangeUnit(e);

	Py_RETURN_NONE;
}

PyObject *scribus_getunit(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	return PyLong_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->unitIndex()));
}

PyObject *scribus_pointstodocunit(PyObject* /* self */, PyObject *args)
{
    double points;
    if (!PyArg_ParseTuple(args, "d", &points))
        return nullptr;
    if (!checkHaveDocument())
        return nullptr;

    return Py_BuildValue("d", PointToValue(points));
}

PyObject *scribus_docunittopoints(PyObject* /* self */, PyObject *args)
{
    double value;
    if (!PyArg_ParseTuple(args, "d", &value))
        return nullptr;
    if (!checkHaveDocument())
        return nullptr;

    return Py_BuildValue("d", ValueToPoint(value));
}

PyObject *scribus_stringvaluetopoints(PyObject* /* self */, PyObject *args)
{
    PyESString strValue;
    if (!PyArg_ParseTuple(args, "es", "utf-8", strValue.ptr()))
        return nullptr;

    QString qv = QString::fromUtf8(strValue.c_str());

    int uIdx = unitIndexFromString(qv);
    double value = unitValueFromString(qv);
    double points = value / unitGetRatioFromIndex(uIdx);

    return Py_BuildValue("d", points);
}

PyObject *scribus_loadstylesfromfile(PyObject* /* self */, PyObject *args)
{
	PyESString fileName;
	if (!PyArg_ParseTuple(args, "es", "utf-8", fileName.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->doc->loadStylesFromFile(QString::fromUtf8(fileName.c_str()));

	Py_RETURN_NONE;
}

PyObject *scribus_setdoctype(PyObject* /* self */, PyObject* args)
{
	int fp, fsl;
	if (!PyArg_ParseTuple(args, "ii", &fp, &fsl))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;

	if (currentDoc->pagePositioning() == fp)
		currentDoc->setPageSetFirstPage(currentDoc->pagePositioning(), fsl);
	currentView->reformPages();
	currentView->GotoPage(currentDoc->currentPageNumber()); // is this needed?
	currentView->DrawNew();   // is this needed?
	//CB TODO ScCore->primaryMainWindow()->pagePalette->RebuildPage(); // is this needed?
	ScCore->primaryMainWindow()->slotDocCh();

	Py_RETURN_NONE;
}

PyObject *scribus_closemasterpage(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->view->hideMasterPage();

	Py_RETURN_NONE;
}

PyObject *scribus_masterpagenames(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	const ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	PyObject* names = PyList_New(currentDoc->MasterPages.count());
	QMap<QString,int>::const_iterator it(currentDoc->MasterNames.constBegin());
	QMap<QString,int>::const_iterator itEnd(currentDoc->MasterNames.constEnd());
	int n = 0;
	for ( ; it != itEnd; ++it )
	{
		PyList_SET_ITEM(names, n++, PyUnicode_FromString(it.key().toUtf8().data()) );
	}
	return names;
}

PyObject *scribus_editmasterpage(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	const QString masterPageName(name.c_str());
	const QMap<QString,int>& masterNames(ScCore->primaryMainWindow()->doc->MasterNames);
	const QMap<QString,int>::const_iterator it(masterNames.find(masterPageName));
	if ( it == masterNames.constEnd() )
	{
		PyErr_SetString(PyExc_ValueError, "Master page not found");
		return nullptr;
	}
	ScCore->primaryMainWindow()->view->showMasterPage(*it);

	Py_RETURN_NONE;
}

PyObject* scribus_createmasterpage(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	const QString masterPageName(name.c_str());

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	if (currentDoc->MasterNames.contains(masterPageName))
	{
		PyErr_SetString(PyExc_ValueError, "Master page already exists");
		return nullptr;
	}
	currentDoc->addMasterPage(currentDoc->MasterPages.count(), masterPageName);

	Py_RETURN_NONE;
}

PyObject* scribus_createfacingmasterpair(PyObject* /* self */, PyObject* args)
{
	PyESString leftNameUtf8;
	PyESString rightNameUtf8;
	if (!PyArg_ParseTuple(args, "eses", "utf-8", leftNameUtf8.ptr(), "utf-8", rightNameUtf8.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	const QString leftName = QString::fromUtf8(leftNameUtf8.c_str()).trimmed();
	const QString rightName = QString::fromUtf8(rightNameUtf8.c_str()).trimmed();
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	if (currentDoc->pageSets()[currentDoc->pagePositioning()].Columns != 2)
	{
		PyErr_SetString(PyExc_ValueError, "Facing master pairs require a facing-page document");
		return nullptr;
	}
	if (leftName.isEmpty() || rightName.isEmpty() || leftName == rightName)
	{
		PyErr_SetString(PyExc_ValueError, "Left and right master page names must be different and non-empty");
		return nullptr;
	}
	if (currentDoc->MasterNames.contains(leftName) || currentDoc->MasterNames.contains(rightName))
	{
		PyErr_SetString(PyExc_ValueError, "A master page with one of these names already exists");
		return nullptr;
	}
	if (!currentDoc->addMasterPagePair(leftName, rightName))
	{
		PyErr_SetString(PyExc_RuntimeError, "Could not create the facing master pair");
		return nullptr;
	}

	PyObject* leftNameObject = PyUnicode_FromString(leftName.toUtf8().constData());
	PyObject* rightNameObject = PyUnicode_FromString(rightName.toUtf8().constData());
	return Py_BuildValue("(NN)", leftNameObject, rightNameObject);
}

PyObject* scribus_deletemasterpage(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	const QString masterPageName(name.c_str());

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	if (!currentDoc->MasterNames.contains(masterPageName))
	{
		PyErr_SetString(PyExc_ValueError, "Master page does not exist");
		return nullptr;
	}
	if (masterPageName == "Normal")
	{
		PyErr_SetString(PyExc_ValueError, "Can not delete the Normal master page");
		return nullptr;
	}
	bool oldMode = currentDoc->masterPageMode();
	currentDoc->setMasterPageMode(true);
	ScCore->primaryMainWindow()->deletePage2(currentDoc->MasterNames[masterPageName]);
	currentDoc->setMasterPageMode(oldMode);

	Py_RETURN_NONE;
}

PyObject *scribus_getmasterpage(PyObject* /* self */, PyObject* args)
{
	int e;
	if (!PyArg_ParseTuple(args, "i", &e))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	e--;

	const ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	if ((e < 0) || (e > static_cast<int>(currentDoc->Pages->count())-1))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range: '%1'.","python error").arg(e+1).toUtf8().constData());
		return nullptr;
	}
	return PyUnicode_FromString(currentDoc->DocPages.at(e)->masterPageName().toUtf8());
}

PyObject* scribus_applymasterpage(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	int page = 0;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", name.ptr(), &page))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	const QString masterPageName(name.c_str());

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	if (!currentDoc->MasterNames.contains(masterPageName))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Master page does not exist: '%1'","python error").arg(masterPageName).toUtf8().constData());
		return nullptr;
	}
	if ((page < 1) || (page > static_cast<int>(currentDoc->Pages->count())))
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Page number out of range: %1.","python error").arg(page).toUtf8().constData());
		return nullptr;
	}

	if (!currentDoc->applyMasterPage(masterPageName, page-1))
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to apply masterpage '%1' on page: %2","python error").arg(masterPageName).arg(page).toUtf8().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject* scribus_exportdocumentcheck(PyObject* /* self */, PyObject* args, PyObject* kw)
{
	PyESString targetFilenameArg;
	PyESString checkProfileNameArg;
	bool showNonPrintingLayerErrors = false;
	char *kwargs[] = {const_cast<char*>("jsonFilename"), const_cast<char*>("checkProfileName"),
		const_cast<char*>("nonPrintingLayers"), nullptr};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "|es$esp", kwargs,
			"utf-8", targetFilenameArg.ptr(), "utf-8", checkProfileNameArg.ptr(),
			&showNonPrintingLayerErrors))
		return nullptr;

	if (!checkHaveDocument())
		return nullptr;
	const QString targetFileName(targetFilenameArg.c_str());
	const QString checkProfileName(checkProfileNameArg.c_str());

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	if (checkProfileName.isEmpty())
		DocumentChecker::checkDocument(currentDoc);
	else
		DocumentChecker::checkDocument(currentDoc, checkProfileName);

	// "Standard" Errors
	// (Taken from PreflightError in scribusstruct.h)
	QMap<PreflightError, QString> errorsList = {
		{PreflightError::MissingGlyph, "MissingGlyph"},
		{PreflightError::TextOverflow, "TextOverflow"},
		{PreflightError::ObjectNotOnPage, "ObjectNotOnPage"},
		{PreflightError::MissingImage, "MissingImage"},
		{PreflightError::ImageDPITooLow, "ImageDPITooLow"},
		{PreflightError::Transparency, "Transparency"},
		{PreflightError::PDFAnnotField, "PDFAnnotField"},
		{PreflightError::PlacedPDF, "PlacedPDF"},
		{PreflightError::ImageDPITooHigh, "ImageDPITooHigh"},
		{PreflightError::ImageIsGIF, "ImageIsGIF"},
		{PreflightError::BlendMode, "BlendMode"},
		{PreflightError::WrongFontInAnnotation, "WrongFontInAnnotation"},
		{PreflightError::NotCMYKOrSpot, "NotCMYKOrSpot"},
		{PreflightError::DeviceColorsAndOutputIntent, "DeviceColorsAndOutputIntent"},
		{PreflightError::FontNotEmbedded, "FontNotEmbedded"},
		{PreflightError::EmbeddedFontIsOpenType, "EmbeddedFontIsOpenType"},
		{PreflightError::OffConflictLayers, "OffConflictLayers"},
		{PreflightError::PartFilledImageFrame, "PartFilledImageFrame"},
		{PreflightError::MarksChanged, "MarksChanged"},
		{PreflightError::AppliedMasterDifferentSide, "AppliedMasterDifferentSide"},
		{PreflightError::EmptyTextFrame, "EmptyTextFrame"},
		{PreflightError::ImageHasProgressiveEncoding, "ImageHasProgressiveEncoding"},
		{PreflightError::MissingStyle, "MissingStyle"},
		{PreflightError::BrokenCrossReference, "BrokenCrossReference"},
	};
	// Custom Errors
	// "DocumentModifiedAfterMarksUpdate"

	QJsonObject json;

	if (currentDoc->notesChanged())
	{
		json["marks"] = QJsonObject{{"", "DocumentModifiedAfterMarksUpdate"}};
	}
	else
	{
		json["marks"] = QJsonObject{};
	}

	QJsonArray jsonLayers;
	for (const auto& [layerId, layerErrors]: currentDoc->docLayerErrors.asKeyValueRange())
	{
		for (const auto& [key, errorLevel]: layerErrors.asKeyValueRange())
		{
			jsonLayers.push_back(QJsonObject{{"layer", currentDoc->layerName(layerId)}, {"error", errorsList.value(key)}});
		}
	}
	json["layers"] = jsonLayers;

	QMap<int, QVector<QPair<QString, QString>>> pagesWithErrors;

	for (auto [pageItem, itemError]: currentDoc->masterItemErrors.asKeyValueRange())
	{
		if (!showNonPrintingLayerErrors && !currentDoc->layerPrintable(pageItem->m_layerID))
			continue;
		const int pageNumber = pageItem->OwnPage;
		for (auto [errorCode, errorLevel]: itemError.asKeyValueRange())
			pagesWithErrors[pageNumber].push_back({pageItem->itemName(), errorsList.value(errorCode)});
	}

	QJsonObject jsonMasterPages;
	for (auto [pageNumber, value]: pagesWithErrors.asKeyValueRange())
	{
		if (pageNumber < 0 || pageNumber >= currentDoc->MasterPages.count())
			continue;
		QJsonArray pageItems;
		for (const auto& [item, error]: value)
		{
			pageItems.push_back(QJsonObject{{{"item", item}, {"error", error}}});
		}
		jsonMasterPages[currentDoc->MasterPages.at(pageNumber)->pageName()] = pageItems;
	}
	json["masterPages"] = jsonMasterPages;

	pagesWithErrors.clear();

	for (auto [pageNumber, pageErrors]: currentDoc->pageErrors.asKeyValueRange())
	{
		pagesWithErrors[pageNumber] = {};

		for (auto [errorCode, value]: pageErrors.asKeyValueRange())
		{
			pagesWithErrors[pageNumber].push_back({"", errorsList.value(errorCode)});
		}
	}
	QJsonArray jsonFreeItems;
	for (auto [pageItem, itemErrors]: currentDoc->docItemErrors.asKeyValueRange())
	{
		if (!showNonPrintingLayerErrors && !currentDoc->layerPrintable(pageItem->m_layerID))
			continue;
		if (pageItem->OwnPage == -1)
		{
			jsonFreeItems.push_back(pageItem->itemName());
			continue;
		}
		for (auto [errorCode, errorLevel]: itemErrors.asKeyValueRange())
			pagesWithErrors[pageItem->OwnPage].push_back({pageItem->itemName(), errorsList.value(errorCode)});
	}

	QJsonObject jsonPages;
	for (auto [pageNumber, value]: pagesWithErrors.asKeyValueRange())
	{
		QJsonArray pageItems;
		for (const auto& [item, error]: value)
		{
			pageItems.push_back(QJsonObject{{{"item", item}, {"error", error}}});
		}
		jsonPages[QString::number(pageNumber + 1)] = pageItems;
	}
	json["pages"] = jsonPages;

	QJsonObject jsonStyles;
	for (auto [styleName, styleErrors] : currentDoc->docStyleErrors.asKeyValueRange())
	{
		QJsonArray styleErrorList;
		for (auto [errorCode, value] : styleErrors.asKeyValueRange())
			styleErrorList.push_back(errorsList.value(errorCode));
		jsonStyles[styleName] = styleErrorList;
	}
	json["styles"] = jsonStyles;

	json["freeItems"] = jsonFreeItems;

	if (targetFileName.isEmpty())
		return PyUnicode_FromString(QJsonDocument(json).toJson().constData());

	QFile saveFile(targetFileName);
	if (!saveFile.open(QIODevice::WriteOnly))
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to open the file '%1' for writing","python error").arg(targetFileName).toUtf8().constData());
		return nullptr;
	}
	saveFile.write(QJsonDocument(json).toJson());

	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmddocdocwarnings()
{
	QStringList s;
	s << scribus_applymasterpage__doc__
	  << scribus_closedoc__doc__
	  << scribus_closemasterpage__doc__
	  << scribus_createmasterpage__doc__
	  << scribus_deletemasterpage__doc__
	  << scribus_editmasterpage__doc__
	  << scribus_getbaseline__doc__ 
	  << scribus_getbleeds__doc__ 
	  << scribus_getdocname__doc__
	  << scribus_getinfo__doc__
	  << scribus_getmargins__doc__
	  << scribus_getmasterpage__doc__
	  << scribus_getunit__doc__ 
	  << scribus_havedoc__doc__
	  << scribus_loadstylesfromfile__doc__
	  << scribus_masterpagenames__doc__ 
	  << scribus_newdoc__doc__ 
	  << scribus_newdocument__doc__
	  << scribus_opendoc__doc__
	  << scribus_revertdoc__doc__
	  << scribus_savedoc__doc__
	  << scribus_savedocas__doc__
	  << scribus_setbaseline__doc__
	  << scribus_setbleeds__doc__
	  << scribus_setdoctype__doc__ 
	  << scribus_setinfo__doc__
	  << scribus_setmargins__doc__
	  << scribus_setunit__doc__;
}

PyObject *scribus_getrtl(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	return PyBool_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->isRTL()));
}

PyObject *scribus_setrtl(PyObject* /* self */, PyObject* args)
{
	int rtl = 0;
	if (!PyArg_ParseTuple(args, "p", &rtl))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	currentDoc->setRTL(rtl != 0);
	currentDoc->setModified(true);
	Py_RETURN_NONE;
}

PyObject *scribus_createcrossreferencetarget(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	PyESString objectName;
	int position = -1;
	if (!PyArg_ParseTuple(args, "es|esi", "utf-8", name.ptr(), "utf-8", objectName.ptr(), &position))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	PageItem* item = GetUniqueItem(QString::fromUtf8(objectName.c_str()));
	if (!item)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot insert a cross-reference target into a non-text frame.", "python error").toUtf8().constData());
		return nullptr;
	}
	if (position < -1 || position > item->itemText.length())
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Insert index out of bounds.", "python error").toUtf8().constData());
		return nullptr;
	}

	const QString targetName = QString::fromUtf8(name.c_str()).trimmed();
	if (targetName.isEmpty() || currentDoc->crossReferenceTarget(targetName))
	{
		PyErr_SetString(NameExistsError, QObject::tr("A cross-reference target named '%1' already exists, or the name is empty.", "python error").arg(targetName).toUtf8().constData());
		return nullptr;
	}
	Mark* target = currentDoc->insertCrossReferenceTarget(targetName, item, position);
	if (!target)
	{
		PyErr_SetString(ScribusException, QObject::tr("The cross-reference target could not be inserted.", "python error").toUtf8().constData());
		return nullptr;
	}
	return PyUnicode_FromString(target->label.toUtf8().constData());
}

PyObject *scribus_deletecrossreferencetarget(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const QString targetName = QString::fromUtf8(name.c_str()).trimmed();
	if (!currentDoc->crossReferenceTarget(targetName))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Cross-reference target '%1' was not found.", "python error").arg(targetName).toUtf8().constData());
		return nullptr;
	}
	if (!currentDoc->deleteCrossReferenceTarget(targetName))
	{
		PyErr_SetString(ScribusException, QObject::tr("The cross-reference target could not be deleted.", "python error").toUtf8().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_insertcrossreference(PyObject* /* self */, PyObject* args)
{
	PyESString targetName;
	PyESString objectName;
	PyESString label;
	PyESString formatName;
	PyESString prefix;
	PyESString suffix;
	int position = -1;
	if (!PyArg_ParseTuple(args, "es|esieseseses", "utf-8", targetName.ptr(), "utf-8", objectName.ptr(), &position,
		"utf-8", label.ptr(), "utf-8", formatName.ptr(), "utf-8", prefix.ptr(), "utf-8", suffix.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	PageItem* item = GetUniqueItem(QString::fromUtf8(objectName.c_str()));
	if (!item)
		return nullptr;
	if (!item->isTextFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot insert a cross-reference into a non-text frame.", "python error").toUtf8().constData());
		return nullptr;
	}
	if (position < -1 || position > item->itemText.length())
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Insert index out of bounds.", "python error").toUtf8().constData());
		return nullptr;
	}

	const QString requestedTarget = QString::fromUtf8(targetName.c_str()).trimmed();
	if (!currentDoc->crossReferenceTarget(requestedTarget))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Cross-reference target '%1' was not found.", "python error").arg(requestedTarget).toUtf8().constData());
		return nullptr;
	}
	const QString requestedFormat = QString::fromUtf8(formatName.c_str()).trimmed().toLower();
	CrossReferenceFormat format = CrossReferencePageNumber;
	if (requestedFormat == QLatin1String("paragraph") || requestedFormat == QLatin1String("paragraph-text"))
		format = CrossReferenceParagraphText;
	else if (!requestedFormat.isEmpty() && requestedFormat != QLatin1String("page") && requestedFormat != QLatin1String("page-number"))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cross-reference format must be 'page' or 'paragraph'.", "python error").toUtf8().constData());
		return nullptr;
	}
	Mark* reference = currentDoc->insertCrossReference(requestedTarget, item, position,
		QString::fromUtf8(label.c_str()), format, QString::fromUtf8(prefix.c_str()), QString::fromUtf8(suffix.c_str()));
	if (!reference)
	{
		PyErr_SetString(ScribusException, QObject::tr("The page reference could not be inserted.", "python error").toUtf8().constData());
		return nullptr;
	}
	return PyUnicode_FromString(reference->label.toUtf8().constData());
}

PyObject *scribus_gotocrossreferencetarget(PyObject* /* self */, PyObject* args)
{
	PyESString referenceName;
	if (!PyArg_ParseTuple(args, "es", "utf-8", referenceName.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const QString requestedReference = QString::fromUtf8(referenceName.c_str()).trimmed();
	Mark* reference = currentDoc->getMark(requestedReference, MARK2MarkType);
	if (!reference)
	{
		PyErr_SetString(NotFoundError, QObject::tr("Cross-reference '%1' was not found.", "python error").arg(requestedReference).toUtf8().constData());
		return nullptr;
	}
	Mark* target = currentDoc->crossReferenceDestination(reference);
	if (!target)
	{
		PyErr_SetString(NotFoundError, QObject::tr("The target of cross-reference '%1' was not found.", "python error").arg(requestedReference).toUtf8().constData());
		return nullptr;
	}
	if (!currentDoc->navigateToMark(target))
	{
		PyErr_SetString(ScribusException, QObject::tr("The target of cross-reference '%1' is not placed in document text.", "python error").arg(requestedReference).toUtf8().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_getcrossreferencetext(PyObject* /* self */, PyObject* args)
{
	PyESString targetName;
	if (!PyArg_ParseTuple(args, "es", "utf-8", targetName.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const QString requestedTarget = QString::fromUtf8(targetName.c_str()).trimmed();
	if (!currentDoc->crossReferenceTarget(requestedTarget))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Cross-reference target '%1' was not found.", "python error").arg(requestedTarget).toUtf8().constData());
		return nullptr;
	}
	return PyUnicode_FromString(currentDoc->crossReferenceParagraphText(requestedTarget).toUtf8().constData());
}

PyObject *scribus_getcrossreferencepage(PyObject* /* self */, PyObject* args)
{
	PyESString targetName;
	if (!PyArg_ParseTuple(args, "es", "utf-8", targetName.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const QString requestedTarget = QString::fromUtf8(targetName.c_str()).trimmed();
	if (!currentDoc->crossReferenceTarget(requestedTarget))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Cross-reference target '%1' was not found.", "python error").arg(requestedTarget).toUtf8().constData());
		return nullptr;
	}
	return PyUnicode_FromString(currentDoc->crossReferencePageNumber(requestedTarget).toUtf8().constData());
}

PyObject *scribus_listcrossreferencetargets(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	const QStringList targets = ScCore->primaryMainWindow()->doc->marksLabelsList(MARKAnchorType);
	PyObject* list = PyList_New(targets.size());
	if (!list)
		return nullptr;
	for (int i = 0; i < targets.size(); ++i)
		PyList_SET_ITEM(list, i, PyUnicode_FromString(targets.at(i).toUtf8().constData()));
	return list;
}

PyObject *scribus_renamecrossreferencetarget(PyObject* /* self */, PyObject* args)
{
	PyESString oldName;
	PyESString newName;
	if (!PyArg_ParseTuple(args, "eses", "utf-8", oldName.ptr(), "utf-8", newName.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const QString previousName = QString::fromUtf8(oldName.c_str()).trimmed();
	const QString targetName = QString::fromUtf8(newName.c_str()).trimmed();
	if (!currentDoc->crossReferenceTarget(previousName))
	{
		PyErr_SetString(NotFoundError, QObject::tr("Cross-reference target '%1' was not found.", "python error").arg(previousName).toUtf8().constData());
		return nullptr;
	}
	if (targetName.isEmpty() || (targetName != previousName && currentDoc->crossReferenceTarget(targetName)))
	{
		PyErr_SetString(NameExistsError, QObject::tr("A cross-reference target named '%1' already exists, or the name is empty.", "python error").arg(targetName).toUtf8().constData());
		return nullptr;
	}
	if (!currentDoc->renameCrossReferenceTarget(previousName, targetName))
	{
		PyErr_SetString(ScribusException, QObject::tr("The cross-reference target could not be renamed.", "python error").toUtf8().constData());
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject *scribus_createvariable(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	PyESString value;
	if (!PyArg_ParseTuple(args, "eses", "utf-8", name.ptr(), "utf-8", value.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const QString variableName = QString::fromUtf8(name.c_str());
	const QString id = currentDoc->addDynamicVariable(variableName, QString::fromUtf8(value.c_str()));
	if (id.isEmpty())
	{
		PyErr_SetString(NameExistsError, QObject::tr("A dynamic variable named '%1' already exists, or the name is empty or reserved.", "python error").arg(variableName).toUtf8().constData());
		return nullptr;
	}
	currentDoc->changed();
	return PyUnicode_FromString(id.toUtf8().constData());
}

PyObject *scribus_createrunningheadervariable(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	PyESString paragraphStyle;
	PyESString mode;
	PyESString textCase;
	PyESString fallback;
	int removeTrailingPunctuation = 0;
	if (!PyArg_ParseTuple(args, "eseses|espes", "utf-8", name.ptr(), "utf-8", paragraphStyle.ptr(), "utf-8", mode.ptr(),
		"utf-8", textCase.ptr(), &removeTrailingPunctuation, "utf-8", fallback.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const QString variableName = QString::fromUtf8(name.c_str());
	const QString styleName = QString::fromUtf8(paragraphStyle.c_str());
	const auto headerMode = DynamicVariableResolver::runningHeaderModeFromString(QString::fromUtf8(mode.c_str()));
	QString textCaseName = QString::fromUtf8(textCase.c_str());
	if (textCaseName.isEmpty())
		textCaseName = DynamicVariableResolver::AsEnteredCase;
	const auto headerTextCase = DynamicVariableResolver::runningHeaderTextCaseFromString(textCaseName);
	QString fallbackName = QString::fromUtf8(fallback.c_str());
	if (fallbackName.isEmpty())
		fallbackName = DynamicVariableResolver::NoFallback;
	const auto headerFallback = DynamicVariableResolver::runningHeaderFallbackFromString(fallbackName);
	const QString id = currentDoc->addRunningHeaderVariable(variableName, styleName, headerMode, headerTextCase,
		removeTrailingPunctuation != 0, headerFallback);
	if (id.isEmpty())
	{
		PyErr_SetString(ScribusException, QObject::tr("The running header name, paragraph style, mode, fallback, or text formatting is invalid or already in use.", "python error").toUtf8().constData());
		return nullptr;
	}
	currentDoc->changed();
	return PyUnicode_FromString(id.toUtf8().constData());
}

PyObject *scribus_deletevariable(PyObject* /* self */, PyObject* args)
{
	PyESString identifier;
	if (!PyArg_ParseTuple(args, "es", "utf-8", identifier.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const QString requested = QString::fromUtf8(identifier.c_str());
	const QString id = userDynamicVariableId(currentDoc, requested);
	if (id.isEmpty())
		return dynamicVariableNotFound(requested);
	currentDoc->removeDynamicVariable(id);
	currentDoc->changed();
	Py_RETURN_NONE;
}

PyObject *scribus_getvariable(PyObject* /* self */, PyObject* args)
{
	PyESString identifier;
	PyESString objectName;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", identifier.ptr(), "utf-8", objectName.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	PageItem* contextFrame = nullptr;
	const QString contextName = QString::fromUtf8(objectName.c_str());
	if (!contextName.isEmpty())
	{
		contextFrame = GetUniqueItem(contextName);
		if (!contextFrame)
			return nullptr;
	}
	const QString requested = QString::fromUtf8(identifier.c_str());
	const QString id = dynamicVariableId(currentDoc, requested);
	if (id.isEmpty())
		return dynamicVariableNotFound(requested);
	return PyUnicode_FromString(currentDoc->resolveDynamicVariable(id, contextFrame).toUtf8().constData());
}

PyObject *scribus_insertvariable(PyObject* /* self */, PyObject* args)
{
	PyESString identifier;
	PyESString objectName;
	int position = -1;
	if (!PyArg_ParseTuple(args, "es|esi", "utf-8", identifier.ptr(), "utf-8", objectName.ptr(), &position))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	PageItem* item = GetUniqueItem(QString::fromUtf8(objectName.c_str()));
	if (!item)
		return nullptr;
	if (!item->isTextFrame() && !item->isPathText())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Cannot insert a dynamic variable into a non-text frame.", "python error").toUtf8().constData());
		return nullptr;
	}
	if (position < -1 || position > item->itemText.length())
	{
		PyErr_SetString(PyExc_IndexError, QObject::tr("Insert index out of bounds.", "python error").toUtf8().constData());
		return nullptr;
	}

	const QString requested = QString::fromUtf8(identifier.c_str());
	const QString id = dynamicVariableId(currentDoc, requested);
	if (id.isEmpty())
		return dynamicVariableNotFound(requested);

	Mark* mark = currentDoc->getDynamicVariableMark(id);
	if (!mark)
	{
		QString label;
		if (DynamicVariableResolver::isBuiltInId(id))
			label = DynamicVariableResolver::displayNameForType(DynamicVariableResolver::typeForId(id));
		else
			label = currentDoc->dynamicVariable(id)->name;
		getUniqueName(label, currentDoc->marksLabelsList(MARKVariableTextType), QStringLiteral("_"));
		MarkData data;
		data.itemName = item->itemName();
		data.variableId = id;
		data.text = currentDoc->resolveDynamicVariable(id, item);
		mark = currentDoc->newMark();
		mark->setValues(label, item->OwnPage, MARKVariableTextType, data);
	}
	if (position < 0)
		position = item->itemText.length();
	item->itemText.insertMark(mark, position);
	item->invalidateLayout();
	currentDoc->changed();
	currentDoc->flag_updateMarksLabels = true;
	return PyUnicode_FromString(id.toUtf8().constData());
}

PyObject *scribus_listvariables(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	const ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const auto& variables = currentDoc->dynamicVariables();
	PyObject* list = PyList_New(variables.size());
	if (!list)
		return nullptr;
	int index = 0;
	for (auto it = variables.constBegin(); it != variables.constEnd(); ++it)
	{
		const DynamicVariable& variable = it.value();
		PyObject* tuple = Py_BuildValue("(sss)", variable.id.toUtf8().constData(), variable.name.toUtf8().constData(), variable.value.toUtf8().constData());
		if (!tuple)
		{
			Py_DECREF(list);
			return nullptr;
		}
		PyList_SET_ITEM(list, index++, tuple);
	}
	return list;
}

PyObject *scribus_renamevariable(PyObject* /* self */, PyObject* args)
{
	PyESString identifier;
	PyESString newName;
	if (!PyArg_ParseTuple(args, "eses", "utf-8", identifier.ptr(), "utf-8", newName.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const QString requested = QString::fromUtf8(identifier.c_str());
	const QString id = userDynamicVariableId(currentDoc, requested);
	if (id.isEmpty())
		return dynamicVariableNotFound(requested);
	const DynamicVariable variable = *currentDoc->dynamicVariable(id);
	if (!currentDoc->updateDynamicVariable(id, QString::fromUtf8(newName.c_str()), variable.value))
	{
		PyErr_SetString(NameExistsError, QObject::tr("The new dynamic variable name is empty or already in use.", "python error").toUtf8().constData());
		return nullptr;
	}
	currentDoc->changed();
	Py_RETURN_NONE;
}

PyObject *scribus_setvariable(PyObject* /* self */, PyObject* args)
{
	PyESString identifier;
	PyESString value;
	if (!PyArg_ParseTuple(args, "eses", "utf-8", identifier.ptr(), "utf-8", value.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const QString requested = QString::fromUtf8(identifier.c_str());
	const QString id = userDynamicVariableId(currentDoc, requested);
	if (id.isEmpty())
		return dynamicVariableNotFound(requested);
	const DynamicVariable variable = *currentDoc->dynamicVariable(id);
	if (variable.type != DynamicVariableResolver::UserDefined)
	{
		PyErr_SetString(ScribusException, QObject::tr("The value of computed dynamic variable '%1' cannot be set.", "python error").arg(variable.name).toUtf8().constData());
		return nullptr;
	}
	if (!currentDoc->updateDynamicVariable(id, variable.name, QString::fromUtf8(value.c_str())))
	{
		PyErr_SetString(ScribusException, QObject::tr("The dynamic variable could not be updated.", "python error").toUtf8().constData());
		return nullptr;
	}
	currentDoc->changed();
	Py_RETURN_NONE;
}

PyObject *scribus_applydatarecord(PyObject* /* self */, PyObject* args)
{
	PyObject* record = nullptr;
	int strict = 1;
	if (!PyArg_ParseTuple(args, "O|p", &record, &strict))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (!PyDict_Check(record))
	{
		PyErr_SetString(PyExc_TypeError, "Data record must be a dictionary of string keys and string values.");
		return nullptr;
	}

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	struct Binding { QString id; QString name; QString value; };
	QVector<Binding> bindings;
	QSet<QString> seenIds;
	PyObject* key = nullptr;
	PyObject* value = nullptr;
	Py_ssize_t position = 0;
	while (PyDict_Next(record, &position, &key, &value))
	{
		if (!PyUnicode_Check(key) || !PyUnicode_Check(value))
		{
			PyErr_SetString(PyExc_TypeError, "Data record keys and values must be strings.");
			return nullptr;
		}
		Py_ssize_t keyLength = 0;
		Py_ssize_t valueLength = 0;
		const char* keyUtf8 = PyUnicode_AsUTF8AndSize(key, &keyLength);
		const char* valueUtf8 = PyUnicode_AsUTF8AndSize(value, &valueLength);
		if (!keyUtf8 || !valueUtf8)
			return nullptr;
		const QString identifier = QString::fromUtf8(keyUtf8, keyLength);
		const QString resolvedId = dynamicVariableId(currentDoc, identifier);
		const QString id = DynamicVariableResolver::isBuiltInId(resolvedId) ? QString() : resolvedId;
		const DynamicVariable* variable = id.isEmpty() ? nullptr : currentDoc->dynamicVariable(id);
		if (!variable || variable->type != DynamicVariableResolver::UserDefined)
		{
			if (!strict && resolvedId.isEmpty())
				continue;
			PyErr_SetString(ScribusException, QObject::tr("Data field '%1' does not match a writable user-defined variable.", "python error").arg(identifier).toUtf8().constData());
			return nullptr;
		}
		if (seenIds.contains(id))
		{
			PyErr_SetString(ScribusException, QObject::tr("Data record refers to variable '%1' more than once.", "python error").arg(variable->name).toUtf8().constData());
			return nullptr;
		}
		seenIds.insert(id);
		bindings.append({id, variable->name, QString::fromUtf8(valueUtf8, valueLength)});
	}

	UndoTransaction transaction;
	if (bindings.size() > 1 && UndoManager::undoEnabled())
		transaction = UndoManager::instance()->beginTransaction(currentDoc->getUName(), Um::IDocument, QObject::tr("Apply Data Record"));
	for (const Binding& binding : bindings)
	{
		// All bindings were validated before the first document mutation.
		currentDoc->updateDynamicVariable(binding.id, binding.name, binding.value);
	}
	if (transaction)
		transaction.commit();
	if (!bindings.isEmpty())
		currentDoc->changed();
	return PyLong_FromSsize_t(bindings.size());
}

PyObject *scribus_loaddatasource(PyObject* /* self */, PyObject* args)
{
	PyESString path;
	PyESString requestedFormat;
	Py_ssize_t limit = -1;
	if (!PyArg_ParseTuple(args, "es|esn", "utf-8", path.ptr(), "utf-8", requestedFormat.ptr(), &limit))
		return nullptr;
	if (limit < -1)
	{
		PyErr_SetString(PyExc_ValueError, "The record limit must be -1 or greater.");
		return nullptr;
	}
	DataMergeSource source;
	QString error;
	if (!source.load(QString::fromUtf8(path.c_str()), QString::fromUtf8(requestedFormat.c_str()), &error))
	{
		PyErr_SetString(ScribusException, error.toUtf8().constData());
		return nullptr;
	}
	return recordsToPython(source, limit);
}

PyObject *scribus_exportdatamergepdfs(PyObject* /* self */, PyObject* args)
{
	PyESString path;
	PyESString directory;
	PyESString prefix;
	PyESString fileNameField;
	PyObject* requestedMapping = Py_None;
	int firstRecord = 1;
	int lastRecord = -1;
	int failOnPreflight = 0;
	if (!PyArg_ParseTuple(args, "eses|Oesiiesp", "utf-8", path.ptr(), "utf-8", directory.ptr(),
		&requestedMapping, "utf-8", prefix.ptr(), &firstRecord, &lastRecord,
		"utf-8", fileNameField.ptr(), &failOnPreflight))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (requestedMapping != Py_None && !PyDict_Check(requestedMapping))
	{
		PyErr_SetString(PyExc_TypeError, "Mapping must be a dictionary of source fields to user-variable names or IDs.");
		return nullptr;
	}
	DataMergeSource source;
	QString error;
	if (!source.load(QString::fromUtf8(path.c_str()), QString(), &error))
	{
		PyErr_SetString(ScribusException, error.toUtf8().constData());
		return nullptr;
	}
	ScribusMainWindow* mainWindow = ScCore->primaryMainWindow();
	ScribusDoc* currentDoc = mainWindow->doc;
	QMap<QString, QString> mapping;
	if (requestedMapping == Py_None)
	{
		for (const QString& field : source.fields())
		{
			const QString id = userDynamicVariableId(currentDoc, field);
			const DynamicVariable* variable = id.isEmpty() ? nullptr : currentDoc->dynamicVariable(id);
			if (variable && variable->type == DynamicVariableResolver::UserDefined)
				mapping.insert(field, id);
		}
	}
	else
	{
		PyObject* key = nullptr;
		PyObject* value = nullptr;
		Py_ssize_t position = 0;
		while (PyDict_Next(requestedMapping, &position, &key, &value))
		{
			if (!PyUnicode_Check(key) || !PyUnicode_Check(value))
			{
				PyErr_SetString(PyExc_TypeError, "Mapping field names and variable identifiers must be strings.");
				return nullptr;
			}
			Py_ssize_t fieldLength = 0;
			Py_ssize_t identifierLength = 0;
			const char* fieldUtf8 = PyUnicode_AsUTF8AndSize(key, &fieldLength);
			const char* identifierUtf8 = PyUnicode_AsUTF8AndSize(value, &identifierLength);
			if (!fieldUtf8 || !identifierUtf8)
				return nullptr;
			const QString field = QString::fromUtf8(fieldUtf8, fieldLength);
			const QString identifier = QString::fromUtf8(identifierUtf8, identifierLength);
			const QString id = userDynamicVariableId(currentDoc, identifier);
			const DynamicVariable* variable = id.isEmpty() ? nullptr : currentDoc->dynamicVariable(id);
			if (!variable || variable->type != DynamicVariableResolver::UserDefined)
			{
				PyErr_SetString(ScribusException, QObject::tr("'%1' is not a writable user-defined variable.", "python error").arg(identifier).toUtf8().constData());
				return nullptr;
			}
			mapping.insert(field, id);
		}
	}
	DataMergeBatchResult result;
	DataMergeBatchOptions options;
	options.firstRecord = firstRecord;
	options.lastRecord = lastRecord;
	options.fileNameField = QString::fromUtf8(fileNameField.c_str());
	options.failOnPreflight = failOnPreflight != 0;
	if (!DataMergeBatchExporter::exportPdfs(mainWindow, source, mapping, QString::fromUtf8(directory.c_str()),
		QString::fromUtf8(prefix.c_str()), result, {}, options))
	{
		QString message = result.error;
		if (!result.files.isEmpty())
			message += QObject::tr(" %1 completed PDF(s) remain in the output folder.", "python error").arg(result.files.size());
		PyErr_SetString(ScribusException, message.toUtf8().constData());
		return nullptr;
	}
	PyObject* files = PyList_New(result.files.size());
	if (!files)
		return nullptr;
	for (int index = 0; index < result.files.size(); ++index)
	{
		const QByteArray pathUtf8 = result.files.at(index).toUtf8();
		PyObject* file = PyUnicode_FromStringAndSize(pathUtf8.constData(), pathUtf8.size());
		if (!file)
		{
			Py_DECREF(files);
			return nullptr;
		}
		PyList_SET_ITEM(files, index, file);
	}
	return files;
}

PyObject *scribus_setrunningheadervariable(PyObject* /* self */, PyObject* args)
{
	PyESString identifier;
	PyESString name;
	PyESString paragraphStyle;
	PyESString mode;
	PyESString textCase;
	PyESString fallback;
	int removeTrailingPunctuation = -1;
	if (!PyArg_ParseTuple(args, "eseseses|espes", "utf-8", identifier.ptr(), "utf-8", name.ptr(),
		"utf-8", paragraphStyle.ptr(), "utf-8", mode.ptr(), "utf-8", textCase.ptr(), &removeTrailingPunctuation,
		"utf-8", fallback.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	const QString requested = QString::fromUtf8(identifier.c_str());
	const QString id = userDynamicVariableId(currentDoc, requested);
	if (id.isEmpty())
		return dynamicVariableNotFound(requested);
	const DynamicVariable* variable = currentDoc->dynamicVariable(id);
	if (!variable || variable->type != DynamicVariableResolver::RunningHeader)
	{
		PyErr_SetString(ScribusException, QObject::tr("Dynamic variable '%1' is not a running header.", "python error").arg(requested).toUtf8().constData());
		return nullptr;
	}
	const auto headerMode = DynamicVariableResolver::runningHeaderModeFromString(QString::fromUtf8(mode.c_str()));
	QString textCaseName = QString::fromUtf8(textCase.c_str());
	if (textCaseName.isEmpty())
		textCaseName = variable->runningHeaderTextCase;
	const auto headerTextCase = DynamicVariableResolver::runningHeaderTextCaseFromString(textCaseName);
	const bool removePunctuation = removeTrailingPunctuation < 0
		? variable->removeTrailingPunctuation : removeTrailingPunctuation != 0;
	QString fallbackName = QString::fromUtf8(fallback.c_str());
	if (fallbackName.isEmpty())
		fallbackName = variable->runningHeaderFallback;
	const auto headerFallback = DynamicVariableResolver::runningHeaderFallbackFromString(fallbackName);
	if (!currentDoc->updateRunningHeaderVariable(id, QString::fromUtf8(name.c_str()),
		QString::fromUtf8(paragraphStyle.c_str()), headerMode, headerTextCase, removePunctuation, headerFallback))
	{
		PyErr_SetString(ScribusException, QObject::tr("The running header name, paragraph style, mode, fallback, or text formatting is invalid or already in use.", "python error").toUtf8().constData());
		return nullptr;
	}
	currentDoc->changed();
	Py_RETURN_NONE;
}

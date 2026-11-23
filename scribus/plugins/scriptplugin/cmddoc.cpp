/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmddoc.h"
#include "cmdutil.h"
#include "documentchecker.h"
#include "documentinformation.h"
#include "pyesstring.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "units.h"

#include <QApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

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
		{PreflightError::DuplicateLayerName, "DuplicateLayerName"},
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

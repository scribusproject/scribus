/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFileInfo>
#include <QQueue>

#include "cmdmani.h"
#include "cmdutil.h"
#include "embeddedimageextractor.h"
#include "filewatcher.h"
#include "imagealphacontour.h"
#include "imagecmykbatch.h"
#include "imagecmykconversion.h"
#include "imagelinkreplacement.h"
#include "pyesstring.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "sctextstream.h"
#include "selection.h"
#include "undomanager.h"

PyObject *scribus_loadimage(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	PyESString image;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", image.ptr(), "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Target is not an image frame.","python error").toUtf8().constData());
		return nullptr;
	}
	ScCore->primaryMainWindow()->doc->loadPict(QString::fromUtf8(image.c_str()), item);
	Py_RETURN_NONE;
}

PyObject *scribus_relinkimage(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	PyESString image;
	if (!PyArg_ParseTuple(args, "es|es", "utf-8", image.ptr(), "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Target is not an image frame.", "python error").toUtf8().constData());
		return nullptr;
	}
	if (item->isImageInline())
	{
		PyErr_SetString(ScribusException, QObject::tr("Embedded images cannot be relinked.", "python error").toUtf8().constData());
		return nullptr;
	}

	return PyBool_FromLong(item->relinkImage(QString::fromUtf8(image.c_str()), false));
}

PyObject *scribus_replaceimagelinks(PyObject* /* self */, PyObject* args)
{
	PyESString source;
	PyESString replacement;
	PyESString scopeName;
	int dryRun = 0;
	if (!PyArg_ParseTuple(args, "eses|pes", "utf-8", source.ptr(), "utf-8", replacement.ptr(),
		&dryRun, "utf-8", scopeName.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	const QString scope = QString::fromUtf8(scopeName.c_str());
	ImageLinkReplacementScope replacementScope = ImageLinkReplacementScope::EntireDocument;
	if (scope == QLatin1String("page"))
		replacementScope = ImageLinkReplacementScope::CurrentPage;
	else if (scope == QLatin1String("masters"))
		replacementScope = ImageLinkReplacementScope::MasterPages;
	else if (!scope.isEmpty() && scope != QLatin1String("document"))
	{
		PyErr_SetString(PyExc_ValueError, "scope must be document, page or masters");
		return nullptr;
	}
	const ImageLinkReplacementResult result = replaceImageLinks(ScCore->primaryMainWindow()->doc,
		QString::fromUtf8(source.c_str()), QString::fromUtf8(replacement.c_str()),
		replacementScope, dryRun != 0);
	return Py_BuildValue("(iii)", result.matched, result.replaced, result.failed);
}

PyObject *scribus_generateimagealphacontour(PyObject* /* self */, PyObject* args)
{
	int threshold = 128;
	double padding = 0.0;
	int enableWrap = 1;
	PyESString name;
	if (!PyArg_ParseTuple(args, "|idpes", &threshold, &padding, &enableWrap, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem* item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (!item)
		return nullptr;
	QString error;
	if (!generateImageAlphaContour(item, threshold, padding, enableWrap != 0, &error))
	{
		PyErr_SetString(ScribusException, error.toUtf8().constData());
		return nullptr;
	}
	Py_RETURN_TRUE;
}

PyObject *scribus_generateimagecontour(PyObject* /* self */, PyObject* args)
{
	const char* sourceName = "alpha";
	ImageContourOptions options;
	int wrap = 1;
	PyESString name;
	if (!PyArg_ParseTuple(args, "|sididpes", &sourceName, &options.threshold,
		&options.padding, &options.smoothing, &options.simplification,
		&wrap, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	const QString source = QString::fromUtf8(sourceName).toLower();
	if (source == QLatin1String("alpha"))
		options.source = ImageContourSource::Alpha;
	else if (source == QLatin1String("clip"))
		options.source = ImageContourSource::ImageClippingPath;
	else if (source == QLatin1String("luminance"))
		options.source = ImageContourSource::Luminance;
	else if (source == QLatin1String("edge"))
		options.source = ImageContourSource::ContrastEdge;
	else
	{
		PyErr_SetString(PyExc_ValueError, "source must be alpha, clip, luminance or edge");
		return nullptr;
	}
	options.enableWrap = wrap != 0;
	PageItem* item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (!item)
		return nullptr;
	QString error;
	if (!generateImageContour(item, options, &error))
	{
		PyErr_SetString(ScribusException, error.toUtf8().constData());
		return nullptr;
	}
	Py_RETURN_TRUE;
}

PyObject *scribus_exportimageascmykcopy(PyObject* /* self */, PyObject* args)
{
	PyESString destination;
	PyESString name;
	int relink = 0;
	if (!PyArg_ParseTuple(args, "es|esp", "utf-8", destination.ptr(), "utf-8", name.ptr(), &relink))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem* item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (!item)
		return nullptr;
	if (!item->isImageFrame() || item->isLatexFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Target is not an image frame.", "python error").toUtf8().constData());
		return nullptr;
	}
	if (relink && item->isImageInline())
	{
		PyErr_SetString(ScribusException, QObject::tr("Embedded images cannot be relinked by this export.", "python error").toUtf8().constData());
		return nullptr;
	}
	const QString destinationPath = QFileInfo(QString::fromUtf8(destination.c_str())).absoluteFilePath();
	QString error;
	if (!exportImageAsCMYKCopy(item, destinationPath, &error))
	{
		PyErr_SetString(ScribusException, error.toUtf8().constData());
		return nullptr;
	}
	if (relink && !item->relinkImage(destinationPath, false, true))
		Py_RETURN_FALSE;
	Py_RETURN_TRUE;
}

PyObject *scribus_batchexportimagesascmyk(PyObject* /* self */, PyObject* args)
{
	PyESString directory;
	PyESString sourceProfile;
	PyESString destinationProfile;
	int intent = -1;
	int blackPoint = -1;
	int backup = 1;
	int relink = 0;
	int dryRun = 0;
	if (!PyArg_ParseTuple(args, "es|esesiippp", "utf-8", directory.ptr(),
		"utf-8", sourceProfile.ptr(), "utf-8", destinationProfile.ptr(),
		&intent, &blackPoint, &backup, &relink, &dryRun))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (intent < -1 || intent > 3 || blackPoint < -1 || blackPoint > 1)
	{
		PyErr_SetString(PyExc_ValueError, "Invalid rendering intent or black-point compensation value");
		return nullptr;
	}
	ImageCMYKBatchOptions options;
	options.color.sourceProfileName = QString::fromUtf8(sourceProfile.c_str());
	options.color.destinationProfileName = QString::fromUtf8(destinationProfile.c_str());
	if (intent >= 0)
		options.color.renderingIntent = static_cast<eRenderIntent>(intent);
	if (blackPoint >= 0)
		options.color.blackPointCompensation = blackPoint != 0;
	options.copyOriginals = backup != 0;
	options.relink = relink != 0;
	options.dryRun = dryRun != 0;
	const ImageCMYKBatchResult result = runImageCMYKBatch(ScCore->primaryMainWindow()->doc,
		QFileInfo(QString::fromUtf8(directory.c_str())).absoluteFilePath(), options);
	if (!result.error.isEmpty())
	{
		PyErr_SetString(ScribusException, result.error.toUtf8().constData());
		return nullptr;
	}
	return Py_BuildValue("(iiiis)", result.ready, result.exported, result.relinked,
		result.failed, result.reportPath.toUtf8().constData());
}

PyObject *scribus_embedimage(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "|es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame() || item->isLatexFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Target is not an image frame.", "python error").toUtf8().constData());
		return nullptr;
	}
	if (item->isImageInline())
		Py_RETURN_TRUE;
	if (!item->imageIsAvailable || item->Pfile.isEmpty())
		Py_RETURN_FALSE;

	const QString oldPath = item->Pfile;
	if (ScCore->fileWatcher->isWatching(oldPath))
		ScCore->fileWatcher->removeFile(oldPath);
	item->makeImageInline();
	if (!item->isImageInline())
	{
		ScCore->fileWatcher->addFile(oldPath);
		Py_RETURN_FALSE;
	}

	ScCore->fileWatcher->addFile(item->Pfile);
	const bool flipHorizontal = item->imageFlippedH();
	const bool flipVertical = item->imageFlippedV();
	ScCore->primaryMainWindow()->doc->loadPict(item->Pfile, item, true);
	item->setImageFlippedH(flipHorizontal);
	item->setImageFlippedV(flipVertical);
	Py_RETURN_TRUE;
}

PyObject *scribus_extractembeddedimage(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	PyESString destination;
	int relink = 0;
	int overwrite = 0;
	if (!PyArg_ParseTuple(args, "es|ppes", "utf-8", destination.ptr(), &relink, &overwrite, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame() || item->isLatexFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Target is not an image frame.", "python error").toUtf8().constData());
		return nullptr;
	}
	if (!item->isImageInline() || !item->imageIsAvailable)
		Py_RETURN_FALSE;

	const QString destinationPath = QFileInfo(QString::fromUtf8(destination.c_str())).absoluteFilePath();
	QString error;
	if (!copyEmbeddedImageBytes(item->Pfile, destinationPath, overwrite != 0, &error))
	{
		PyErr_SetString(ScribusException, error.toUtf8().constData());
		return nullptr;
	}
	if (relink && !item->relinkExtractedImage(destinationPath, false))
		Py_RETURN_FALSE;
	Py_RETURN_TRUE;
}

PyObject *scribus_isimageembedded(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "|es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame() || item->isLatexFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Target is not an image frame.", "python error").toUtf8().constData());
		return nullptr;
	}
	return PyBool_FromLong(item->isImageInline());
}

PyObject *scribus_scaleimage(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toUtf8().constData());
		return nullptr;
	}

	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*currentDoc->m_Selection);
	bool hadOrigSelection = !tempSelection.isEmpty();

	currentDoc->m_Selection->clear();
	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);

	// scale
	currentDoc->itemSelection_SetImageScale(x, y); //CB why when this is done above?
	currentDoc->updatePic();

	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection = tempSelection;

	Py_RETURN_NONE;
}

PyObject *scribus_setimagepage(PyObject* /* self */, PyObject* args)
{
	if (!checkHaveDocument())
		return nullptr;

	PyESString name;
	int page;
	if (!PyArg_ParseTuple(args, "i|es", &page, "utf-8", name.ptr()))
		return nullptr;

	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toUtf8().constData());
		return nullptr;
	}

	if (page < 0 || page > item->pixm.imgInfo.numberOfPages)
	{
		PyErr_SetString(ScribusException, QObject::tr("The image has %1 pages: cannot switch to page %2.","python error").arg(item->pixm.imgInfo.numberOfPages).arg(page).toUtf8().constData());
		return nullptr;
	}

	item->pixm.imgInfo.actualPageNumber = page;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	currentDoc->updatePic();

	Py_RETURN_NONE;
}

PyObject *scribus_setimagepreviewresolution(PyObject* /* self */, PyObject* args)
{
	if (!checkHaveDocument())
		return nullptr;

	PyESString name;
	int resolutionType;
	if (!PyArg_ParseTuple(args, "i|es", &resolutionType, "utf-8", name.ptr()))
		return nullptr;

	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toUtf8().constData());
		return nullptr;
	}

	if (resolutionType < 0 || resolutionType > 2)
	{
		PyErr_SetString(ScribusException, QObject::tr("The resolution shall be one of: IMAGE_PREVIEW_RESOLUTION_FULL, IMAGE_PREVIEW_RESOLUTION_NORMAL, IMAGE_PREVIEW_RESOLUTION_LOW. %1 is an invalid value.", "python error").arg(resolutionType).toUtf8().constData());
		return nullptr;
	}

	item->pixm.imgInfo.lowResType = resolutionType;
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	currentDoc->updatePic();

	Py_RETURN_NONE;
}

PyObject *scribus_setimagescale(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toUtf8().constData());
		return nullptr;
	}

	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*currentDoc->m_Selection);
	bool hadOrigSelection = !tempSelection.isEmpty();

	currentDoc->m_Selection->clear();
	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);

	// scale
	double newScaleX = x / item->pixm.imgInfo.xres * 72.0;
	double newScaleY = y / item->pixm.imgInfo.yres * 72.0;
	currentDoc->itemSelection_SetImageScale(newScaleX, newScaleY); //CB why when this is done above?
	currentDoc->updatePic();

	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection = tempSelection;

	Py_RETURN_NONE;
}

PyObject *scribus_setimagerotation(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	double deg;
	if (!PyArg_ParseTuple(args, "d|es", &deg, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toUtf8().constData());
		return nullptr;
	}

	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*currentDoc->m_Selection);
	bool hadOrigSelection = !tempSelection.isEmpty();

	currentDoc->m_Selection->clear();
	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);

	currentDoc->itemSelection_SetImageRotation(360 - deg);
	currentDoc->updatePic();

	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection = tempSelection;

	Py_RETURN_NONE;
}

PyObject *scribus_setimageoffset(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toUtf8().constData());
		return nullptr;
	}

	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*ScCore->primaryMainWindow()->doc->m_Selection);
	bool hadOrigSelection = !tempSelection.isEmpty();

	currentDoc->m_Selection->clear();
	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);

	// offset
	double newOffsetX = x / ((item->imageXScale() != 0.0) ? item->imageXScale() : 1);
	double newOffsetY = y / ((item->imageYScale() != 0.0) ? item->imageYScale() : 1);
	currentDoc->itemSelection_SetImageOffset(newOffsetX, newOffsetY); //CB why when this is done above?
	currentDoc->updatePic();

	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection = tempSelection;

	Py_RETURN_NONE;
}

PyObject *scribus_setimagebrightness(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	double n;
	if (!PyArg_ParseTuple(args, "d|es", &n, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toUtf8().constData());
		return nullptr;
	}

	ImageEffect ef;
	ef.effectCode = ImageEffect::EF_BRIGHTNESS;
	ScTextStream fp(&ef.effectParameters, QIODevice::WriteOnly);
	fp << n;

	item->effectsInUse.append(ef);
	item->pixm.applyEffect(item->effectsInUse, ScCore->primaryMainWindow()->doc->PageColors, false);
	
	ScCore->primaryMainWindow()->doc->updatePic();
	Py_RETURN_NONE;
}

PyObject *scribus_setimagegrayscale(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "|es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toUtf8().constData());
		return nullptr;
	}

	ImageEffect ef;
	ef.effectCode = ImageEffect::EF_GRAYSCALE;

	item->effectsInUse.append(ef);
	item->pixm.applyEffect(item->effectsInUse, ScCore->primaryMainWindow()->doc->PageColors, false);
	
	ScCore->primaryMainWindow()->doc->updatePic();
	Py_RETURN_NONE;
}

PyObject *scribus_moveobjectrel(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;

	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*currentDoc->m_Selection);
	bool hadOrigSelection = !tempSelection.isEmpty();

	currentDoc->m_Selection->clear();
	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);
	// Move the item, or items
	if (currentDoc->m_Selection->count() > 1)
	{
		currentView->startGroupTransaction(Um::Move, "", Um::IMove);
		currentDoc->moveGroup(ValueToPoint(x), ValueToPoint(y));
		currentView->endGroupTransaction();
	}
	else
	{
		currentDoc->moveItem(ValueToPoint(x), ValueToPoint(y), item);
	}
	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection = tempSelection;
	Py_RETURN_NONE;
}

PyObject *scribus_moveobjectabs(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;

	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*currentDoc->m_Selection);
	bool hadOrigSelection = !tempSelection.isEmpty();

	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);
	// Move the item, or items
	if (currentDoc->m_Selection->count() > 1)
	{
		currentView->startGroupTransaction(Um::Move, "", Um::IMove);
		double x2, y2, w, h;
		currentDoc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
		currentDoc->moveGroup(pageUnitXToDocX(x) - x2, pageUnitYToDocY(y) - y2);
		currentView->endGroupTransaction();
	}
	else
		currentDoc->moveItem(pageUnitXToDocX(x) - item->xPos(), pageUnitYToDocY(y) - item->yPos(), item);
	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection = tempSelection;

	Py_RETURN_NONE;
}

PyObject *scribus_rotateobjectrel(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	double x;
	if (!PyArg_ParseTuple(args, "d|es", &x, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	ScCore->primaryMainWindow()->doc->rotateItem(item->rotation() - x, item);
	Py_RETURN_NONE;
}

PyObject *scribus_setrotation(PyObject* /* self */, PyObject* args, PyObject* kw)
{
	PyESString name;
	double rotation;
	int basepoint = (int) AnchorPoint::None;
	char *kwargs[] = {
					  const_cast<char *>("rotation"),
					  const_cast<char *>("name"),
					  const_cast<char *>("basepoint"),
					  nullptr};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "d|esi", kwargs, &rotation, "utf-8", name.ptr(), &basepoint))
		return nullptr;

	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;

	AnchorPoint oldBasePoint = AnchorPoint::None;
	if (basepoint != (int) AnchorPoint::None)
	{
		oldBasePoint = ScCore->primaryMainWindow()->doc->rotationMode();
		ScCore->primaryMainWindow()->doc->setRotationMode(static_cast<AnchorPoint>(basepoint));
	}

	ScCore->primaryMainWindow()->doc->rotateItem(rotation * -1.0, item);

	if (basepoint != (int) AnchorPoint::None)
	{
		ScCore->primaryMainWindow()->doc->setRotationMode(oldBasePoint);
	}

	Py_RETURN_NONE;
}

PyObject *scribus_sizeobject(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	double x, y;
	if (!PyArg_ParseTuple(args, "dd|es", &x, &y, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	ScCore->primaryMainWindow()->doc->sizeItem(ValueToPoint(x), ValueToPoint(y), item);
	Py_RETURN_NONE;
}

PyObject *scribus_groupobjects(PyObject* /* self */, PyObject* args)
{
	PyObject *il = nullptr;
	if (!PyArg_ParseTuple(args, "|O", &il))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (il == nullptr && ScCore->primaryMainWindow()->doc->m_Selection->count() < 2)
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("Need selection or argument list of items to group", "python error").toUtf8().constData());
		return nullptr;
	}
	Selection *tempSelection = nullptr;
	Selection *finalSelection = nullptr;
	//uint ap = ScCore->primaryMainWindow()->doc->currentPage()->pageNr();
	// If we were passed a list of items to group...
	if (il != nullptr)
	{
		Py_ssize_t len = PyList_Size(il);
		tempSelection = new Selection(ScCore->primaryMainWindow(), false);
		for (Py_ssize_t i = 0; i < len; i++)
		{
			// FIXME: We might need to explicitly get this string as utf8
			// but as sysdefaultencoding is utf8 it should be a no-op to do
			// so anyway.
			const char* name = PyUnicode_AsUTF8(PyList_GetItem(il, i));
			PageItem *ic = GetUniqueItem(QString::fromUtf8(name));
			if (ic == nullptr)
			{
				delete tempSelection;
				return nullptr;
			}
			tempSelection->addItem (ic);
		}
		finalSelection = tempSelection;
	}
	else
		finalSelection = ScCore->primaryMainWindow()->doc->m_Selection;
	if (finalSelection->count() < 2)
	{
		// We can't very well group only one item
		PyErr_SetString(NoValidObjectError, QObject::tr("Cannot group less than two items", "python error").toUtf8().constData());
		finalSelection = nullptr;
		delete tempSelection;
		return nullptr;
	}

	const PageItem* group = ScCore->primaryMainWindow()->doc->itemSelection_GroupObjects(false, false, finalSelection);
	finalSelection = nullptr;
	delete tempSelection;
	
	return (group ? PyUnicode_FromString(group->itemName().toUtf8()) : nullptr);
}

PyObject *scribus_ungroupobjects(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "|es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *i = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (i == nullptr)
		return nullptr;

	ScribusMainWindow* currentWin = ScCore->primaryMainWindow();
	currentWin->view->deselectItems();
	currentWin->view->selectItem(i);
	currentWin->UnGroupObj();

	Py_RETURN_NONE;
}

PyObject *scribus_scalegroup(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	double sc;
	if (!PyArg_ParseTuple(args, "d|es", &sc, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (sc == 0.0)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot scale by 0%.","python error").toUtf8().constData());
		return nullptr;
	}
	PageItem *i = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (i == nullptr)
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;

	currentView->deselectItems();
	currentView->selectItem(i);
	currentView->startGroupTransaction(Um::Resize, "", Um::IResize);
	currentDoc->scaleGroup(sc, sc);
	currentView->endGroupTransaction();

	Py_RETURN_NONE;
}

PyObject *scribus_getGroupItems(PyObject * /*self*/, PyObject* args, PyObject* kw)
{
	if (!checkHaveDocument())
		return nullptr;

	PyESString name;
	unsigned int recursive = 0;
	unsigned int type = 0;
	char* kwargs[] = {const_cast<char*>("name"), const_cast<char*>("recursive"), const_cast<char*>("type"), nullptr};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "|espi", kwargs, "utf-8", name.ptr(), &recursive, &type))
		return nullptr;

	PageItem *pageItem = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (pageItem == nullptr)
		return nullptr;

	if (!pageItem->isGroup())
		return nullptr;

	auto items = PyList_New(0);

	QQueue<PageItem*> queue;
	queue.enqueue(pageItem);

	while (!queue.isEmpty())
	{
		const auto pageItem = queue.dequeue();
		for (auto groupItem: std::as_const(pageItem->groupItemList))
		{
			if (type == 0 || groupItem->itemType() == type)
			{
				auto itemObj = Py_BuildValue("(sii)",
					groupItem->itemName().toUtf8().constData(),
					groupItem->itemType(),
					groupItem->uniqueNr
				);
				PyList_Append(items, itemObj);
			}
			if (recursive && groupItem->isGroup())
				queue.enqueue(groupItem);
		}
	}

	return items;
}

PyObject *scribus_getselectedobject(PyObject* /* self */, PyObject* args)
{
	int i = 0;
	if (!PyArg_ParseTuple(args, "|i", &i))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	Selection * selection = ScCore->primaryMainWindow()->doc->m_Selection;
	if ((i < selection->count()) && (i > -1))
		return PyUnicode_FromString(selection->itemAt(i)->itemName().toUtf8());
	// FIXME: Should probably return None if no selection?
	return PyUnicode_FromString("");
}

PyObject *scribus_selectioncount(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	return PyLong_FromLong(static_cast<long>(ScCore->primaryMainWindow()->doc->m_Selection->count()));
}

PyObject *scribus_selectobject(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *i = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (i == nullptr)
		return nullptr;
	ScCore->primaryMainWindow()->view->selectItem(i);
	Py_RETURN_NONE;
}

PyObject *scribus_deselectall(PyObject* /* self */)
{
	if (!checkHaveDocument())
		return nullptr;
	ScCore->primaryMainWindow()->view->deselectItems();
	Py_RETURN_NONE;
}

PyObject *scribus_setexportableobject(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	int exp = 1;
	if (!PyArg_ParseTuple(args, "esi", "utf-8", name.ptr(), &exp))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (name.isEmpty())
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Cannot have an empty item name.","python error").toUtf8().constData());
		return nullptr;
	}
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	item->setPrintEnabled(static_cast<bool>(exp));
	if (item->locked())
		return PyLong_FromLong(1);
	return PyLong_FromLong(0);
}

PyObject *scribus_isexportable(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "|es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (item->printEnabled())
		return PyBool_FromLong(1);
	return PyBool_FromLong(0);
}

PyObject *scribus_lockobject(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "|es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	// FIXME: Rather than toggling the lock, we should probably let the user set the lock state
	// and instead provide a different function like toggleLock()
	item->toggleLock();
	if (item->locked())
		return PyLong_FromLong(1);
	return PyLong_FromLong(0);
}

PyObject *scribus_islocked(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	if (!PyArg_ParseTuple(args, "|es", "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (item->locked())
		return PyBool_FromLong(1);
	return PyBool_FromLong(0);
}

PyObject *scribus_setscaleframetoimage(PyObject* /* self */, PyObject* args)
{
	if (!checkHaveDocument())
		return nullptr;
	PyESString name;
	if (!PyArg_ParseTuple(args, "|es", "utf-8", name.ptr()))
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QObject::tr("Specified item not an image frame.","python error").toUtf8().constData());
		return nullptr;
	}
	Selection *sel = new Selection(ScCore->primaryMainWindow());
	sel->addItem(item);
	ScCore->primaryMainWindow()->doc->itemSelection_AdjustFrametoImageSize(sel);
	delete sel;

	Py_RETURN_NONE;
}

PyObject *scribus_setscaleimagetoframe(PyObject* /* self */, PyObject* args, PyObject* kw)
{
	PyESString name;
	long int scaleToFrame = 0;
	long int proportional = 1;
	char* kwargs[] = {const_cast<char*>("scaletoframe"),
		const_cast<char*>("proportional"), const_cast<char*>("name"), nullptr};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "i|ies", kwargs, &scaleToFrame, &proportional, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	if (!item->isImageFrame())
	{
		PyErr_SetString(ScribusException, QObject::tr("Specified item not an image frame.","python error").toUtf8().constData());
		return nullptr;
	}
	// Set the item to scale if appropriate. ScaleType 1 is free
	// scale, 0 is scale to frame.
	item->ScaleType = scaleToFrame == 0;
	// Now, if the user has chosen to set the proportional mode,
	// set it. 1 is proportional, 0 is free aspect.
	if (proportional != -1)
		item->AspectRatio = proportional > 0;

	item->adjustPictScale();
	item->update();

	Py_RETURN_NONE;
}
PyObject *scribus_flipobject(PyObject* /* self */, PyObject* args)
{
	PyESString name;
	double h, v;
	if (!PyArg_ParseTuple(args, "dd|es", &h, &v, "utf-8", name.ptr()))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(name.c_str()));
	if (item == nullptr)
		return nullptr;
	
	// Grab the old selection - but use it only where is there any
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;
	Selection tempSelection(*currentDoc->m_Selection);
	bool hadOrigSelection = !tempSelection.isEmpty();

	currentDoc->m_Selection->clear();
	// Clear the selection
	currentView->deselectItems();
	// Select the item, which will also select its group if
	// there is one.
	currentView->selectItem(item);

	// flip
	if (h == 1)
		currentDoc->itemSelection_FlipH();
	if (v == 1)
		currentDoc->itemSelection_FlipV();
	// Now restore the selection.
	currentView->deselectItems();
	if (hadOrigSelection)
		*currentDoc->m_Selection = tempSelection;

	Py_RETURN_NONE;
}

PyObject *scribus_combinepolygons(PyObject * /* self */)
{
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	Selection* curSelection = currentDoc->m_Selection;
	if (curSelection->count() <= 1)
		Py_RETURN_NONE;

	for (int i = 0; i < curSelection->count(); ++i)
	{
		const PageItem* it = currentDoc->m_Selection->itemAt(i);
		if (!it->isPolygon() && !it->isPolyLine())
		{
			PyErr_SetString(WrongFrameTypeError, QObject::tr("Selection must contain only shapes or bezier curves.", "python error").toUtf8().constData());
			return nullptr;
		}
	}

	currentDoc->itemSelection_UniteItems(nullptr);

	Py_RETURN_NONE;
}

PyObject *scribus_seteditmode(PyObject * /* self */)
{
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	if (currentDoc->m_Selection->count() < 1)
	{
		PyErr_SetString(NoValidObjectError, QString("No item selected.").toUtf8().constData());
		return nullptr;
	}

	auto currItem = currentDoc->m_Selection->itemAt(0);
	if (!currItem->isTextFrame() && !currItem->isImageFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QString("Only image and text frames are supported.").toUtf8().constData());
		return nullptr;
	}

	ScCore->primaryMainWindow()->view->requestMode(modeEdit);

	Py_RETURN_NONE;
}

PyObject *scribus_setnormalmode(PyObject * /* self */)
{
	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;

	if (currentDoc->m_Selection->count() < 1)
	{
		PyErr_SetString(NoValidObjectError, QString("No item selected.").toUtf8().constData());
		return nullptr;
	}

	auto currItem = currentDoc->m_Selection->itemAt(0);
	if (!currItem->isTextFrame() && !currItem->isImageFrame())
	{
		PyErr_SetString(WrongFrameTypeError, QString("Only image and text frames are supported.").toUtf8().constData());
		return nullptr;
	}

	ScCore->primaryMainWindow()->view->requestMode(modeNormal);

	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdmanidocwarnings()
{
	QStringList s;
	s << scribus_combinepolygons__doc__
	  << scribus_deselectall__doc__
	  << scribus_flipobject__doc__
	  << scribus_getGroupItems__doc__
	  << scribus_getselectedobject__doc__
	  << scribus_groupobjects__doc__
	  << scribus_isexportable__doc__
	  << scribus_islocked__doc__
	  << scribus_loadimage__doc__
	  << scribus_lockobject__doc__
      << scribus_moveobjectabs__doc__
	  << scribus_moveobjectrel__doc__
	  << scribus_rotateobjectabs__doc__
	  << scribus_rotateobjectrel__doc__
	  << scribus_scalegroup__doc__
	  << scribus_scaleimage__doc__
	  << scribus_selectioncount__doc__
	  << scribus_selectobject__doc__
	  << scribus_seteditmode__doc__
	  << scribus_setexportableobject__doc__
	  << scribus_setimagebrightness__doc__
	  << scribus_setimagegrayscale__doc__
	  << scribus_setimageoffset__doc__
	  << scribus_setimagepage__doc__
	  << scribus_setimagepreviewresolution__doc__
	  << scribus_setimagescale__doc__
	  << scribus_setnormalmode__doc__
	  << scribus_setrotation__doc__
	  << scribus_setscaleframetoimage__doc__
	  << scribus_setscaleimagetoframe__doc__
	  << scribus_sizeobject__doc__ 
	  << scribus_ungroupobjects__doc__;
}

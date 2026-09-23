/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "imagealphacontour.h"

#include <QPainterPathStroker>
#include <QTransform>

#include "pageitem.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "undostate.h"
#include "undotransaction.h"

bool generateImageAlphaContour(PageItem* item, int threshold, double padding,
	bool enableWrap, QString* error)
{
	if (!item || !item->isImageFrame() || item->isLatexFrame() || !item->imageIsAvailable
		|| !item->isRaster)
	{
		if (error)
			*error = QObject::tr("Select a loaded raster image frame.");
		return false;
	}
	if (threshold < 1 || threshold > 255 || padding < 0 || padding > 1000)
	{
		if (error)
			*error = QObject::tr("Alpha threshold must be 1–255 and padding must be 0–1000 points.");
		return false;
	}
	const QImage* image = item->pixm.qImagePtr();
	if (!image || image->isNull() || !image->hasAlphaChannel())
	{
		if (error)
			*error = QObject::tr("This image has no alpha channel.");
		return false;
	}
	QPainterPath silhouette = imageAlphaSilhouette(*image, threshold);
	if (silhouette.isEmpty())
	{
		if (error)
			*error = QObject::tr("The alpha channel has no usable transparent silhouette.");
		return false;
	}
	const double originalWidth = item->OrigW > 0 ? item->OrigW : image->width();
	const double originalHeight = item->OrigH > 0 ? item->OrigH : image->height();
	const double sourceScaleX = originalWidth / image->width();
	const double sourceScaleY = originalHeight / image->height();
	QTransform imageToFrame;
	imageToFrame.translate(item->imageXOffset() * item->imageXScale(),
		item->imageYOffset() * item->imageYScale());
	imageToFrame.rotate(item->imageRotation());
	imageToFrame.scale(item->imageXScale() * sourceScaleX,
		item->imageYScale() * sourceScaleY);
	silhouette = imageToFrame.map(silhouette);
	QTransform flips;
	if (item->imageFlippedH())
	{
		flips.translate(item->width(), 0);
		flips.scale(-1, 1);
	}
	if (item->imageFlippedV())
	{
		flips.translate(0, item->height());
		flips.scale(1, -1);
	}
	silhouette = flips.map(silhouette);
	silhouette = silhouette.intersected(item->PoLine.toQPainterPath(true));
	if (silhouette.isEmpty())
	{
		if (error)
			*error = QObject::tr("The visible alpha silhouette does not intersect the frame.");
		return false;
	}
	if (padding > 0)
	{
		QPainterPathStroker stroke;
		stroke.setWidth(padding * 2);
		stroke.setJoinStyle(Qt::RoundJoin);
		silhouette = silhouette.united(stroke.createStroke(silhouette));
	}
	FPointArray contour;
	contour.fromQPainterPath(silhouette, true);
	if (contour.empty())
	{
		if (error)
			*error = QObject::tr("The generated alpha contour could not be converted to an editable path.");
		return false;
	}

	UndoTransaction transaction;
	if (UndoManager::undoEnabled())
	{
		transaction = UndoManager::instance()->beginTransaction(item->getUName(), item->getUPixmap(),
			QObject::tr("Generate image alpha contour"), QString(), Um::IBorder);
		auto* state = new ScOldNewState<FPointArray>(QObject::tr("Generate image alpha contour"),
			QString(), Um::IBorder);
		state->set("GENERATE_ALPHA_CONTOUR");
		state->setStates(item->ContourLine.copy(), contour.copy());
		UndoManager::instance()->action(item, state);
	}
	item->ContourLine = contour;
	item->ClipEdited = true;
	if (enableWrap)
		item->setTextFlowMode(PageItem::TextFlowUsesContourLine);
	item->checkTextFlowInteractions();
	item->doc()->regionsChanged()->update(QRectF());
	item->doc()->changed();
	item->update();
	if (transaction)
		transaction.commit();
	return true;
}

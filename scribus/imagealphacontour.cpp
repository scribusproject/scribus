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

bool buildImageContour(PageItem* item, const ImageContourOptions& options,
	QPainterPath* path, QString* error)
{
	if (!path || !item || !item->isImageFrame() || item->isLatexFrame() || !item->imageIsAvailable
		|| !item->isRaster)
	{
		if (error)
			*error = QObject::tr("Select a loaded raster image frame.");
		return false;
	}
	if (options.threshold < 1 || options.threshold > 255 || options.padding < 0
		|| options.padding > 1000 || options.smoothing < 0 || options.smoothing > 50
		|| options.simplification < 0 || options.simplification > 8)
	{
		if (error)
			*error = QObject::tr("Contour threshold, clearance, smoothing or simplification is outside its allowed range.");
		return false;
	}
	if (options.source == ImageContourSource::ImageClippingPath)
	{
		if (item->imageClip.empty())
		{
			if (error)
				*error = QObject::tr("This image has no active clipping path.");
			return false;
		}
		*path = item->imageClip.toQPainterPath(true);
	}
	else
	{
		const QImage* image = item->pixm.qImagePtr();
		if (!image || image->isNull()
			|| (options.source == ImageContourSource::Alpha && !image->hasAlphaChannel()))
		{
			if (error)
				*error = QObject::tr("This image has no usable pixels for the selected contour source.");
			return false;
		}
		QImage sampled = *image;
		if (options.simplification > 0)
		{
			const double factor = 1.0 + options.simplification;
			sampled = image->scaled(qMax(1, int(image->width() / factor)),
				qMax(1, int(image->height() / factor)), Qt::IgnoreAspectRatio,
				Qt::SmoothTransformation);
		}
		QPainterPath silhouette;
		switch (options.source)
		{
			case ImageContourSource::Alpha:
				silhouette = imageAlphaSilhouette(sampled, options.threshold);
				break;
			case ImageContourSource::Luminance:
				silhouette = imageLuminanceSilhouette(sampled, options.threshold);
				break;
			case ImageContourSource::ContrastEdge:
				silhouette = imageContrastSilhouette(sampled, options.threshold);
				break;
			case ImageContourSource::ImageClippingPath:
				break;
		}
		if (silhouette.isEmpty())
		{
			if (error)
				*error = QObject::tr("The selected contour source has no usable foreground silhouette.");
			return false;
		}
		const double originalWidth = item->OrigW > 0 ? item->OrigW : image->width();
		const double originalHeight = item->OrigH > 0 ? item->OrigH : image->height();
		const double sourceScaleX = originalWidth / sampled.width();
		const double sourceScaleY = originalHeight / sampled.height();
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
		*path = flips.map(silhouette);
	}
	*path = path->intersected(item->PoLine.toQPainterPath(true));
	if (path->isEmpty())
	{
		if (error)
			*error = QObject::tr("The visible alpha silhouette does not intersect the frame.");
		return false;
	}
	if (options.smoothing > 0 || options.padding > 0)
	{
		QPainterPathStroker stroke;
		stroke.setWidth((options.smoothing + options.padding) * 2);
		stroke.setJoinStyle(Qt::RoundJoin);
		*path = path->united(stroke.createStroke(*path)).simplified();
	}
	return true;
}

bool generateImageContour(PageItem* item, const ImageContourOptions& options, QString* error)
{
	QPainterPath silhouette;
	if (!buildImageContour(item, options, &silhouette, error))
		return false;
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
	if (options.enableWrap)
		item->setTextFlowMode(PageItem::TextFlowUsesContourLine);
	item->checkTextFlowInteractions();
	item->doc()->regionsChanged()->update(QRectF());
	item->doc()->changed();
	item->update();
	if (transaction)
		transaction.commit();
	return true;
}

bool generateImageAlphaContour(PageItem* item, int threshold, double padding,
	bool enableWrap, QString* error)
{
	ImageContourOptions options;
	options.threshold = threshold;
	options.padding = padding;
	options.enableWrap = enableWrap;
	return generateImageContour(item, options, error);
}

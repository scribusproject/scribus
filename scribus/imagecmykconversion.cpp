/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "imagecmykconversion.h"

#include <QFileInfo>
#include <QObject>

#include "cmsettings.h"
#include "imagecmykexport.h"
#include "pageitem.h"
#include "scimage.h"
#include "scribuscore.h"
#include "scribusdoc.h"

namespace
{
bool fail(QString* error, const QString& message)
{
	if (error)
		*error = message;
	return false;
}
}

bool exportImageAsCMYKCopy(PageItem* item, const QString& destination, QString* error)
{
	return exportImageAsCMYKCopy(item, destination, ImageCMYKConversionOptions(), error);
}

bool canExportImageAsCMYKCopy(PageItem* item,
	const ImageCMYKConversionOptions& options, QString* error)
{
	if (!item || !item->isImageFrame() || item->isLatexFrame() || !item->imageIsAvailable
		|| !item->isRaster || item->pixm.imgInfo.colorspace != ColorSpaceRGB)
		return fail(error, QObject::tr("Choose an available RGB raster image frame."));
	ScribusDoc* doc = item->doc();
	if (!doc || !doc->HasCMS)
		return fail(error, QObject::tr("A valid CMYK output profile and enabled color management are required."));
	ScColorProfile profile = doc->DocPrinterProf;
	if (!options.destinationProfileName.isEmpty())
	{
		if (!ScCore->PrinterProfiles.contains(options.destinationProfileName))
			return fail(error, QObject::tr("The selected CMYK output profile is not installed."));
		profile = doc->colorEngine.openProfileFromFile(
			ScCore->PrinterProfiles.value(options.destinationProfileName).file);
	}
	if (!profile || profile.colorSpace() != ColorSpace_Cmyk)
		return fail(error, QObject::tr("A valid CMYK output profile is required."));
	if (!options.sourceProfileName.isEmpty() && !ScCore->InputProfiles.contains(options.sourceProfileName))
		return fail(error, QObject::tr("The selected RGB source profile is not installed."));
	if (!item->effectsInUse.isEmpty())
		return fail(error, QObject::tr("This frame uses image effects. Exporting without those effects would change its appearance."));
	ScImage alphaProbe;
	alphaProbe.imgInfo.RequestProps = item->pixm.imgInfo.RequestProps;
	alphaProbe.imgInfo.isRequest = item->pixm.imgInfo.isRequest;
	QByteArray alpha;
	if (!alphaProbe.getAlpha(item->Pfile, item->pixm.imgInfo.actualPageNumber,
		alpha, false, true, 300) || !alpha.isEmpty())
		return fail(error, QObject::tr("This image has transparency or its alpha channel could not be checked. Transparent images are not supported yet."));
	return true;
}

bool exportImageAsCMYKCopy(PageItem* item, const QString& destination,
	const ImageCMYKConversionOptions& options, QString* error)
{
	if (!canExportImageAsCMYKCopy(item, options, error))
		return false;
	ScribusDoc* doc = item->doc();
	ScColorProfile destinationProfile = doc->DocPrinterProf;
	if (!options.destinationProfileName.isEmpty())
		destinationProfile = doc->colorEngine.openProfileFromFile(
			ScCore->PrinterProfiles.value(options.destinationProfileName).file);

	const QFileInfo destinationInfo(destination);
	const QString suffix = destinationInfo.suffix().toLower();
	if (suffix != QLatin1String("tif") && suffix != QLatin1String("tiff"))
		return fail(error, QObject::tr("Choose a .tif or .tiff filename for the converted image."));
	if (destinationInfo.exists() || destinationInfo.isSymLink())
		return fail(error, QObject::tr("The destination already exists; converted images never replace files."));
	const QString sourcePath = item->Pfile;
	const int sourcePage = item->pixm.imgInfo.actualPageNumber;

	QByteArray profileBytes;
	if (!destinationProfile.save(profileBytes) || profileBytes.isEmpty())
		return fail(error, QObject::tr("The document's CMYK output profile could not be saved."));

	ScImage converted;
	converted.imgInfo.RequestProps = item->pixm.imgInfo.RequestProps;
	converted.imgInfo.isRequest = item->pixm.imgInfo.isRequest;
	CMSettings cms(doc, options.sourceProfileName.isEmpty() ? item->cmsProfile()
		: options.sourceProfileName, static_cast<eRenderIntent>(item->cmsRenderingIntent()));
	cms.setUseEmbeddedProfile(options.sourceProfileName.isEmpty() && item->useEmbeddedImageProfile());
	if (options.renderingIntent)
		cms.setImageRenderingIntent(*options.renderingIntent);
	if (options.blackPointCompensation)
		cms.setBlackPointCompensation(*options.blackPointCompensation);
	cms.setOutputProfile(destinationProfile);
	bool realCMYK = false;
	if (!converted.loadPicture(sourcePath, sourcePage, cms, ScImage::OutputProfile, 300, &realCMYK))
		return fail(error, QObject::tr("The source image could not be loaded for CMYK conversion."));
	if (!realCMYK)
		return fail(error, QObject::tr("The output profile did not produce CMYK image data."));
	if (converted.qImage().format() != QImage::Format_ARGB32)
		return fail(error, QObject::tr("The converted image has an unsupported pixel format (%1).")
			.arg(static_cast<int>(converted.qImage().format())));

	return writeCMYKTiffCopy(destination, converted.qImage(), profileBytes,
		converted.imgInfo.xres, converted.imgInfo.yres, error);
}

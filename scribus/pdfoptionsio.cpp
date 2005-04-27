#include "pdfoptionsio.h"
#include "scribusstructs.h"

const int PDFOptionsIO::formatVersion = 1300;

PDFOptionsIO::PDFOptionsIO(PDFOptions& opts) :
	doc("PDFSettings"),
	error()
{
	this->opts = &opts;
}

// writeTo(QString) is implemented separately to writeTo(QTextStream)
// because we don't want to clobber the output file until we know the
// data has been generated ok, and we can't avoid clobbering the file
// to create a QTextStream().
bool PDFOptionsIO::writeTo(QString outFileName)
{
	QString xml = buildXMLString();
	if (xml == QString::null)
		return false;
	QFile f(outFileName);
	if (!f.open(IO_WriteOnly|IO_Truncate))
	{
		error = QObject::tr("Couldn't open output file");
		return false;
	}
	QTextStream ts(&f);
	ts.setEncoding(QTextStream::UnicodeUTF8);
	ts << xml;
	error = QString::null;
	return true;
}

bool PDFOptionsIO::writeTo(QTextStream& outStream)
{
	if (!outStream.device()->isWritable())
	{
		error = QObject::tr("Output stream not writeable");
		return false;
	}
	QString xml = buildXMLString();
	if (xml == QString::null)
		return false;
	outStream << xml;
	error = QString::null;
	return true;
}

// Construct and return an XML string representing the settings
QString PDFOptionsIO::buildXMLString()
{
	// Verify to make sure our settings are sane
	QString vrfyError;
	PDFOptions::VerifyResults vr = opts->verify(&vrfyError);
	if (vr != PDFOptions::Verify_NoError)
	{
		error = QObject::tr("Verification of settings failed: %1").arg(vrfyError);
		return QString::null;
	}
	// Build the document. Initial implementation uses QDom.
	root = doc.createElement("ScribusPDFOpts");
	root.setAttribute("version", formatVersion);
	doc.appendChild(root);
	// Fill the guts of the document
	buildSettings();
	// We're done - return a string containing the document XML
	QString xml = doc.toString();
	xml.prepend("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	return xml;
}

// Build up the stored settings into the passed DOM objects
void PDFOptionsIO::buildSettings()
{
	addElem(root, "thumbnails", opts->Thumbnails);
	addElem(root, "articles", opts->Articles);
	addElem(root, "useLayers", opts->useLayers);
	addElem(root, "compress", opts->Compress);
	addElem(root, "compressMethod", opts->CompressMethod);
	addElem(root, "quality", opts->Quality);
	addElem(root, "recalcPic", opts->RecalcPic);
	addElem(root, "bookmarks", opts->Bookmarks);
	addElem(root, "picRes", opts->PicRes);
	QString pdfVersString;
	switch (opts->Version)
	{
		case PDFOptions::PDFVersion_X3:
			pdfVersString = "X3";
			break;
		default:
			pdfVersString = QString::number(opts->Version);
			break;
	}
	addElem(root, "pdfVersion", pdfVersString);
	addElem(root, "resolution", opts->Resolution);
	addElem(root, "binding", opts->Binding);
	addList(root, "embedFonts", opts->EmbedList);
	addList(root, "subsetFonts", opts->SubsetList);
	addElem(root, "mirrorH", opts->MirrorH);
	addElem(root, "mirrorV", opts->MirrorV);
	addElem(root, "rotateDegrees", opts->RotateDeg);
	addElem(root, "presentMode", opts->PresentMode);
	addPresentationData();
	addElem(root, "filename", opts->Datei);
	addElem(root, "isGrayscale", opts->isGrayscale);
	addElem(root, "useRGB", opts->UseRGB);
	addElem(root, "useProfiles", opts->UseProfiles);
	addElem(root, "useProfiles2", opts->UseProfiles2);
	addElem(root, "useLPI", opts->UseLPI);
	addLPISettings();
	addElem(root, "solidProf", opts->SolidProf);
	addElem(root, "sComp", opts->SComp);
	addElem(root, "imageProf", opts->ImageProf);
	addElem(root, "embeddedI", opts->EmbeddedI);
	addElem(root, "intent2", opts->Intent2);
	addElem(root, "printProf", opts->PrintProf);
	addElem(root, "info", opts->Info);
	addElem(root, "intent", opts->Intent);
	addElem(root, "bleedTop", opts->BleedTop);
	addElem(root, "bleedLeft", opts->BleedLeft);
	addElem(root, "bleedRight", opts->BleedRight);
	addElem(root, "bleedBottom", opts->BleedBottom);
	addElem(root, "encrypt", opts->Encrypt);
	addElem(root, "passOwner", opts->PassOwner);
	addElem(root, "passUser", opts->PassUser);
	addElem(root, "permissions", opts->Permissions);
}

// Convenience functions to add a single-attribute element
// of a particular type to the tree at a given point.

void PDFOptionsIO::addElem(QDomElement& addTo, QString name, bool value)
{
	QDomElement elem = doc.createElement(name);
	elem.setAttribute("value",value);
	addTo.appendChild(elem);
}

void PDFOptionsIO::addElem(QDomElement& addTo, QString name, QString value)
{
	QDomElement elem = doc.createElement(name);
	elem.setAttribute("value",value);
	addTo.appendChild(elem);
}

void PDFOptionsIO::addElem(QDomElement& addTo, QString name, int value)
{
	QDomElement elem = doc.createElement(name);
	elem.setAttribute("value",value);
	addTo.appendChild(elem);
}

void PDFOptionsIO::addElem(QDomElement& addTo, QString name, double value)
{
	QDomElement elem = doc.createElement(name);
	elem.setAttribute("value",value);
	addTo.appendChild(elem);
}

// Save a QValueList<String> or QStringList as a list of
// <item value=""> elements
void PDFOptionsIO::addList(QDomElement& addTo, QString name, QValueList<QString>& value)
{
	// List base element has no attributes, only children
	QDomElement listbase = doc.createElement(name);
	addTo.appendChild(listbase);
	QValueList<QString>::iterator it;
	for (it = value.begin(); it != value.end(); ++it)
		addElem(listbase, "item", *(it));
}

// save PDFOptions::PresentVals
void PDFOptionsIO::addPresentationData()
{
	// PDFOptions::PresentVals is a QValueList<PDFPresentationData>
	//
	// XML structure will be like this:
	// <presentationSettings>
	//   <presentationSettingsEntry>
	//     <pageEffectDuration value="0"/>
	//     <pageViewDuration value="0"/>
	//     <effectType value="0"/>
	//     <dm value="0"/>
	//     <m value="0"/>
	//     <di value="0"/>
	//   </presentationSettingsEntry>
	//   <presentationSettingsEntry>
	//     ...
	//   </presentationSettingsEntry>
	//   ...
	// </presentationSettings>
	QDomElement presentationSettings = doc.createElement("presentationSettings");
	root.appendChild(presentationSettings);
	QValueList<PDFPresentationData>::iterator it;
	for (it = opts->PresentVals.begin(); it != opts->PresentVals.end(); ++it)
	{
		// Settings entry has no attributes, only children
		QDomElement psEntry = doc.createElement("presentationSettingsEntry");
		presentationSettings.appendChild(psEntry);
		// Children:
		addElem(psEntry, "pageEffectDuration", (*it).pageEffectDuration);
		addElem(psEntry, "pageViewDuration", (*it).pageViewDuration);
		addElem(psEntry, "effectType", (*it).effectType);
		addElem(psEntry, "dm", (*it).Dm);
		addElem(psEntry, "m", (*it).M);
		addElem(psEntry, "di", (*it).Di);
	}
}

// Save PDFOptions::LPISettings
void PDFOptionsIO::addLPISettings()
{
	// PDFOptions::LPISettings is a QMap<QString,LPIData>
	//
	// XML structure will be like this:
	// <lpiSettings>
	//   <lpiSettingsEntry name="blah">
	//     <frequency value="0">
	//     <angle value="0">
	//     <spotFunc value="0">
	//   </lpiSettingsEntry>
	//   <lpiSettingsEntry name="blah2">
	//     ...
	//   </lpiSettingsEntry>
	//   ...
	// </lpiSettings>
	QDomElement lpiSettings = doc.createElement("lpiSettings");
	root.appendChild(lpiSettings);
	QMap<QString,LPIData>::iterator it;
	for (it = opts->LPISettings.begin(); it != opts->LPISettings.end(); ++it)
	{
		// Settings entry has no attributes, only children
		QDomElement lpiEntry = doc.createElement("lpiSettingsEntry");
		lpiEntry.setAttribute("name", it.key());
		lpiSettings.appendChild(lpiEntry);
		// Children:
		addElem(lpiEntry, "frequency", (*it).Frequency);
		addElem(lpiEntry, "angle", (*it).Angle);
		addElem(lpiEntry, "spotFunc", (*it).SpotFunc);
	}

}

bool PDFOptionsIO::readFrom(QTextStream& inStream)
{
	if (!inStream.device()->isReadable())
		return false;
	// TODO: implement this method
	return false;
}

// overload of bool readFrom(QTextStream& inStream)
bool PDFOptionsIO::readFrom(QString inFileName)
{
	QTextStream ts(inFileName, IO_ReadOnly);
	return readFrom(ts);
}

const QString& PDFOptionsIO::lastError() const
{
	return error;
}

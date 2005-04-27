#include "pdfoptions.h"
#include "scribusstructs.h"

PDFOptions::VerifyResults PDFOptions::verify()
{
	return verify(static_cast<QString*>(NULL));
}

PDFOptions::VerifyResults PDFOptions::verify(QString* /*problemDescription*/)
{
	// TODO: implement this method
	return Verify_OtherError;
}




const int PDFOptionsIO::formatVersion = 1300;

PDFOptionsIO::PDFOptionsIO(PDFOptions& opts) :
	doc("PDFSettings")
{
	this->opts = &opts;
}

// overload of bool PDFOptions::writeTo(QTextStream& outStream)
bool PDFOptionsIO::writeTo(QString outFileName)
{
	QTextStream ts(outFileName, IO_WriteOnly|IO_Truncate);
	return writeTo(ts);
}

// overload of bool PDFOptions::writeTo(QTextStream& outStream)
bool PDFOptionsIO::writeTo(FILE* outFilePtr)
{
	QTextStream ts(outFilePtr, IO_WriteOnly|IO_Truncate);
	return writeTo(ts);
}

bool PDFOptionsIO::writeTo(QTextStream& outStream)
{
	if (!outStream.device()->isWritable())
		return false;
	// Verify to make sure our settings are sane
	PDFOptions::VerifyResults vr = opts->verify();
	if (vr != PDFOptions::Verify_NoError)
	{
		qDebug("PDFOptionsIO::writeTo(): "
				"verify() failed [%i]", vr);
		return false;
	}
	// Build the document. Initial implementation uses QDom.
	root = doc.createElement("ScribusPDFOpts");
	root.setAttribute("version", formatVersion);
	doc.appendChild(root);
	// Fill the guts of the document
	if (!buildSettings())
	{
		qDebug("PDFOptionsIO::writeTo(): "
				"couldn't build settings file");
		return false;
	}
	// We're done - save it to the output stream
	QString xml = doc.toString();
	outStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	outStream << doc.toString();
	return true;
}

// Build up the stored settings into the passed DOM objects
bool PDFOptionsIO::buildSettings()
{
	addElem("thumbnails", opts->Thumbnails);
	addElem("articles", opts->Articles);
	addElem("useLayers", opts->useLayers);
	addElem("compress", opts->Compress);
	addElem("compressMethod", opts->CompressMethod);
	addElem("quality", opts->Quality);
	addElem("recalcPic", opts->RecalcPic);
	addElem("bookmarks", opts->Bookmarks);
	addElem("picRes", opts->PicRes);
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
	addElem("pdfVersion", pdfVersString);
	addElem("resolution", opts->Resolution);
	addElem("binding", opts->Binding);
	// EmbedList
	// SubsetList
	addElem("mirrorH", opts->MirrorH);
	addElem("mirrorV", opts->MirrorV);
	addElem("rotateDegrees", opts->RotateDeg);
	addElem("presentMode", opts->PresentMode);
	// PresentVals
	addElem("filename", opts->Datei);
	addElem("isGrayscale", opts->isGrayscale);
	addElem("useRGB", opts->UseRGB);
	addElem("useProfiles", opts->UseProfiles);
	addElem("useProfiles2", opts->UseProfiles2);
	addElem("useLPI", opts->UseLPI);
	// LPISettings
	addElem("solidProf", opts->SolidProf);
	addElem("sComp", opts->SComp);
	addElem("imageProf", opts->ImageProf);
	addElem("embeddedI", opts->EmbeddedI);
	addElem("intent2", opts->Intent2);
	addElem("printProf", opts->PrintProf);
	addElem("info", opts->Info);
	addElem("intent", opts->Intent);
	addElem("bleedTop", opts->BleedTop);
	addElem("bleedLeft", opts->BleedLeft);
	addElem("bleedRight", opts->BleedRight);
	addElem("bleedBottom", opts->BleedBottom);
	addElem("encrypt", opts->Encrypt);
	addElem("passOwner", opts->PassOwner);
	addElem("passUser", opts->PassUser);
	addElem("permissions", opts->Permissions);
	return true;
}

// Convenience functions to add a single-attribute element
// of a particular type to the tree.

void PDFOptionsIO::addElem(QString name, bool value)
{
	QDomElement elem = doc.createElement(name);
	elem.setAttribute("value",value);
	root.appendChild(elem);
}

void PDFOptionsIO::addElem(QString name, QString value)
{
	QDomElement elem = doc.createElement(name);
	elem.setAttribute("value",value);
	root.appendChild(elem);
}

void PDFOptionsIO::addElem(QString name, int value)
{
	QDomElement elem = doc.createElement(name);
	elem.setAttribute("value",value);
	root.appendChild(elem);
}

void PDFOptionsIO::addElem(QString name, double value)
{
	QDomElement elem = doc.createElement(name);
	elem.setAttribute("value",value);
	root.appendChild(elem);
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

// overload of bool readFrom(QTextStream& inStream)
bool PDFOptionsIO::readFrom(FILE* inFilePtr)
{
	QTextStream ts(inFilePtr, IO_ReadOnly);
	return readFrom(ts);
}

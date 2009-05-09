/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pdfoptionsio.h"
#include "scribusstructs.h"

#include <QApplication>
#include <QList>
#include <QTextStream>

#include "scclocale.h"

const int PDFOptionsIO::formatVersion = 1300;

PDFOptionsIO::PDFOptionsIO(PDFOptions& opts) :
	m_doc(),
	m_root(),
	m_includePasswords(false),
	m_error()
{
	this->m_opts = &opts;
}

// writeTo(QString) is implemented separately to writeTo(QTextStream)
// because we don't want to clobber the output file until we know the
// data has been generated ok, and we can't avoid clobbering the file
// to create a QTextStream().
bool PDFOptionsIO::writeTo(QString outFileName, bool includePasswords)
{
	m_includePasswords = includePasswords;
	QString xml(buildXMLString());
	if (xml.isNull())
		return false;
	QFile f(outFileName);
	if (!f.open(QIODevice::WriteOnly|QIODevice::Truncate))
	{
		m_error = QObject::tr("Could not open output file %1")
			.arg(qApp->translate("QFile",f.errorString().toLatin1().constData()));
		return false;
	}
	QTextStream ts(&f);
	ts.setCodec("UTF-8");
	ts << xml;
	m_includePasswords = false; // just to be paranoid
	m_error = QString::null;
	return true;
}

bool PDFOptionsIO::writeTo(QIODevice& outDevice, bool includePasswords)
{
	m_includePasswords = includePasswords;
	if (!outDevice.isWritable())
	{
		m_error = QObject::tr("Output stream not writeable");
		return false;
	}
	QString xml = buildXMLString();
	if (xml.isNull())
		return false;
	QTextStream ts(&outDevice);
	ts.setCodec("UTF-8");
	ts << xml;
	m_includePasswords = false; // just to be paranoid
	m_error = QString::null;
	return true;
}

// Construct and return an XML string representing the settings
QString PDFOptionsIO::buildXMLString()
{
	// Verify to make sure our settings are sane
	QString vrfyError;
	// Make sure the options are sane
	PDFOptions::VerifyResults vr = m_opts->verify(&vrfyError);
	if (vr != PDFOptions::Verify_NoError)
	{
		m_error = QObject::tr("Verification of settings failed: %1").arg(vrfyError);
		return QString::null;
	}
	// Build the document. Initial implementation uses QDom.
	m_doc = QDomDocument();
	m_root = m_doc.createElement("ScribusPDFOptions");
	m_root.setAttribute("version", formatVersion);
	m_doc.appendChild(m_root);
	// Fill the guts of the document
	buildSettings();
	// We're done - return a string containing the document XML
	QString xml = m_doc.toString();
	xml.prepend(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<!DOCTYPE ScribusPDFOptions SYSTEM 'scribuspdfoptions.dtd'>\n"
		);
	return xml;
}

// Build up the stored settings into the passed DOM objects
void PDFOptionsIO::buildSettings()
{
	addElem(m_root, "firstUse", m_opts->firstUse);
	addElem(m_root, "thumbnails", m_opts->Thumbnails);
	addElem(m_root, "articles", m_opts->Articles);
	addElem(m_root, "useLayers", m_opts->useLayers);
	addElem(m_root, "compress", m_opts->Compress);
	addElem(m_root, "compressMethod", m_opts->CompressMethod);
	addElem(m_root, "quality", m_opts->Quality);
	addElem(m_root, "recalcPic", m_opts->RecalcPic);
	addElem(m_root, "bookmarks", m_opts->Bookmarks);
	addElem(m_root, "picRes", m_opts->PicRes);
	addElem(m_root, "embedPDF", m_opts->embedPDF);
	QString pdfVersString;
	switch (m_opts->Version)
	{
		case PDFOptions::PDFVersion_X3:
			pdfVersString = "X3";
			break;
		default:
			pdfVersString = QString::number(m_opts->Version);
			break;
	}
	addElem(m_root, "pdfVersion", pdfVersString);
	addElem(m_root, "resolution", m_opts->Resolution);
	addElem(m_root, "binding", m_opts->Binding);
	addList(m_root, "embedFonts", m_opts->EmbedList);
	addList(m_root, "subsetFonts", m_opts->SubsetList);
	addElem(m_root, "mirrorH", m_opts->MirrorH);
	addElem(m_root, "mirrorV", m_opts->MirrorV);
	addElem(m_root, "rotateDegrees", m_opts->RotateDeg);
	addElem(m_root, "presentMode", m_opts->PresentMode);
	addPresentationData();
	addElem(m_root, "filename", m_opts->fileName);
	addElem(m_root, "isGrayscale", m_opts->isGrayscale);
	addElem(m_root, "useRGB", m_opts->UseRGB);
	addElem(m_root, "useProfiles", m_opts->UseProfiles);
	addElem(m_root, "useProfiles2", m_opts->UseProfiles2);
	addElem(m_root, "useLPI", m_opts->UseLPI);
	addElem(m_root, "useSpotColors", m_opts->UseSpotColors);
	addElem(m_root, "doMultiFile", m_opts->doMultiFile);
	addLPISettings();
	addElem(m_root, "solidProf", m_opts->SolidProf);
	addElem(m_root, "sComp", m_opts->SComp);
	addElem(m_root, "imageProf", m_opts->ImageProf);
	addElem(m_root, "embeddedI", m_opts->EmbeddedI);
	addElem(m_root, "intent2", m_opts->Intent2);
	addElem(m_root, "printProf", m_opts->PrintProf);
	addElem(m_root, "info", m_opts->Info);
	addElem(m_root, "intent", m_opts->Intent);
	addElem(m_root, "bleedTop", m_opts->bleeds.Top);
	addElem(m_root, "bleedLeft", m_opts->bleeds.Left);
	addElem(m_root, "bleedRight", m_opts->bleeds.Right);
	addElem(m_root, "bleedBottom", m_opts->bleeds.Bottom);
	addElem(m_root, "encrypt", m_opts->Encrypt);
	addElem(m_root, "passOwner", m_includePasswords ? m_opts->PassOwner : "");
	addElem(m_root, "passUser", m_includePasswords ? m_opts->PassUser : "");
	addElem(m_root, "permissions", m_opts->Permissions);
}

// Convenience functions to add a single-attribute element
// of a particular type to the tree at a given point.

void PDFOptionsIO::addElem(QDomElement& addTo, QString name, bool value)
{
	QDomElement elem = m_doc.createElement(name);
	elem.setAttribute("value", value ? "true" : "false" );
	addTo.appendChild(elem);
}

void PDFOptionsIO::addElem(QDomElement& addTo, QString name, QString value)
{
	QDomElement elem = m_doc.createElement(name);
	elem.setAttribute("value",value);
	addTo.appendChild(elem);
}

void PDFOptionsIO::addElem(QDomElement& addTo, QString name, int value)
{
	QDomElement elem = m_doc.createElement(name);
	elem.setAttribute("value",value);
	addTo.appendChild(elem);
}

void PDFOptionsIO::addElem(QDomElement& addTo, QString name, double value)
{
	QDomElement elem = m_doc.createElement(name);
	elem.setAttribute("value",value);
	addTo.appendChild(elem);
}

// Save a QValueList<String> or QStringList as a list of
// <item value=""> elements
void PDFOptionsIO::addList(QDomElement& addTo, QString name, QList<QString>& value)
{
	// List base element has no attributes, only children
	QDomElement listbase = m_doc.createElement(name);
	addTo.appendChild(listbase);
	QList<QString>::iterator it;
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
	QDomElement presentationSettings = m_doc.createElement("presentationSettings");
	m_root.appendChild(presentationSettings);
	QList<PDFPresentationData>::iterator it;
	for (it = m_opts->PresentVals.begin(); it != m_opts->PresentVals.end(); ++it)
	{
		// Settings entry has no attributes, only children
		QDomElement psEntry = m_doc.createElement("presentationSettingsEntry");
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
	QDomElement lpiSettings = m_doc.createElement("lpiSettings");
	m_root.appendChild(lpiSettings);
	QMap<QString,LPIData>::iterator it;
	for (it = m_opts->LPISettings.begin(); it != m_opts->LPISettings.end(); ++it)
	{
		// Settings entry has no attributes, only children
		QDomElement lpiEntry = m_doc.createElement("lpiSettingsEntry");
		lpiEntry.setAttribute("name", it.key());
		lpiSettings.appendChild(lpiEntry);
		// Children:
		addElem(lpiEntry, "frequency", (*it).Frequency);
		addElem(lpiEntry, "angle", (*it).Angle);
		addElem(lpiEntry, "spotFunc", (*it).SpotFunc);
	}

}

// overload of bool readFrom(QTextStream& inStream)
bool PDFOptionsIO::readFrom(QString inFileName)
{
	QFile f(inFileName);
	if (!f.open(QIODevice::ReadOnly))
	{
		m_error = QObject::tr("Could not open input file %1")
			.arg(qApp->translate("QFile",f.errorString().toLatin1().constData()));
		return false;
	}
	return readFrom(f);
}

bool PDFOptionsIO::readFrom(QIODevice& inDevice)
{
	if (!inDevice.isReadable())
		return false;
	QString domError;
	int errorLine, errorColumn;
	if (!m_doc.setContent(&inDevice, &domError, &errorLine, &errorColumn))
	{
		m_error = QObject::tr("Unable to read settings XML:")
			.arg(QObject::tr("%1 (line %2 col %3)", "Load PDF settings")
				.arg(domError).arg(errorLine).arg(errorColumn)
			);
		return false;
	}
	if (!readSettings())
		// m_error should already be set
		return false;
	m_error = QString::null;
	return true;
}

// Helper for readFrom(...) to read settings from the DOM
bool PDFOptionsIO::readSettings()
{
	// Get the root element
	m_root = m_doc.documentElement();
	if (m_root.isNull())
	{
		m_error = QObject::tr("Unable to read settings XML: %1")
			.arg(QObject::tr("null root node", "Load PDF settings"));
	}
	// and start processing elements
	if (!readElem(m_root, "firstUse", &m_opts->firstUse))
		return false;
	if (!readElem(m_root, "thumbnails", &m_opts->Thumbnails))
		return false;
	if (!readElem(m_root, "articles", &m_opts->Articles))
		return false;
	if (!readElem(m_root, "useLayers", &m_opts->useLayers))
		return false;
	if (!readElem(m_root, "compress", &m_opts->Compress))
		return false;
	if (!readElem(m_root, "compressMethod", (int*) &m_opts->CompressMethod))
		return false;
	if (!readElem(m_root, "quality", &m_opts->Quality))
		return false;
	if (!readElem(m_root, "recalcPic", &m_opts->RecalcPic))
		return false;
	if (!readElem(m_root, "bookmarks", &m_opts->Bookmarks))
		return false;
	if (!readElem(m_root, "picRes", &m_opts->PicRes))
		return false;
	if (!readElem(m_root, "embedPDF", &m_opts->embedPDF))
		m_opts->embedPDF = false;
	readPDFVersion();
	if (!readElem(m_root, "resolution", &m_opts->Resolution))
		return false;
	if (!readElem(m_root, "binding", &m_opts->Binding))
		return false;
	if (!readList(m_root, "embedFonts", &m_opts->EmbedList))
		return false;
	if (!readList(m_root, "subsetFonts", &m_opts->SubsetList))
		return false;
	if (!readElem(m_root, "mirrorH", &m_opts->MirrorH))
		return false;
	if (!readElem(m_root, "mirrorV", &m_opts->MirrorV))
		return false;
	if (!readElem(m_root, "rotateDegrees", &m_opts->RotateDeg))
		return false;
	if (!readElem(m_root, "presentMode", &m_opts->PresentMode))
		return false;
	if (!readPresentationData())
		return false;
	if (!readElem(m_root, "filename", &m_opts->fileName))
		return false;
	if (!readElem(m_root, "isGrayscale", &m_opts->isGrayscale))
		return false;
	if (!readElem(m_root, "useRGB", &m_opts->UseRGB))
		return false;
	if (!readElem(m_root, "useProfiles", &m_opts->UseProfiles))
		return false;
	if (!readElem(m_root, "useProfiles2", &m_opts->UseProfiles2))
		return false;
	if (!readElem(m_root, "useLPI", &m_opts->UseLPI))
		return false;
	if (!readElem(m_root, "useSpotColors", &m_opts->UseSpotColors))
		return false;
	if (!readElem(m_root, "doMultiFile", &m_opts->doMultiFile))
		return false;
	if (!readLPISettings())
		return false;
	if (!readElem(m_root, "solidProf", &m_opts->SolidProf))
		return false;
	if (!readElem(m_root, "sComp", &m_opts->SComp))
		return false;
	if (!readElem(m_root, "imageProf", &m_opts->ImageProf))
		return false;
	if (!readElem(m_root, "embeddedI", &m_opts->EmbeddedI))
		return false;
	if (!readElem(m_root, "intent2", &m_opts->Intent2))
		return false;
	if (!readElem(m_root, "printProf", &m_opts->PrintProf))
		return false;
	if (!readElem(m_root, "info", &m_opts->Info))
		return false;
	if (!readElem(m_root, "intent", &m_opts->Intent))
		return false;
	if (!readElem(m_root, "bleedTop", &m_opts->bleeds.Top))
		return false;
	if (!readElem(m_root, "bleedLeft", &m_opts->bleeds.Left))
		return false;
	if (!readElem(m_root, "bleedRight", &m_opts->bleeds.Right))
		return false;
	if (!readElem(m_root, "bleedBottom", &m_opts->bleeds.Bottom))
		return false;
	if (!readElem(m_root, "encrypt", &m_opts->Encrypt))
		return false;
	if (!readElem(m_root, "passOwner", &m_opts->PassOwner))
		return false;
	if (!readElem(m_root, "passUser", &m_opts->PassUser))
		return false;
	if (!readElem(m_root, "permissions", &m_opts->Permissions))
		return false;
	return true;
}

bool PDFOptionsIO::readPDFVersion()
{
	QString pdfVersString;
	if (!readElem(m_root, "pdfVersion", &pdfVersString))
		return false;
	if (pdfVersString == "X3")
	{
		m_opts->Version = PDFOptions::PDFVersion_X3;
		return true;
	}
	else if (pdfVersString == "13")
	{
		m_opts->Version = PDFOptions::PDFVersion_13;
		return true;
	}
	else if (pdfVersString == "14")
	{
		m_opts->Version = PDFOptions::PDFVersion_14;
		return true;
	}
	else if (pdfVersString == "15")
	{
		m_opts->Version = PDFOptions::PDFVersion_15;
		return true;
	}
	else
	{
		m_error = QObject::tr("Unable to read settings XML:")
			.arg(QObject::tr("<pdfVersion> invalid", "Load PDF settings"));
		return false;
	}
}

// returns a null node on failure
QDomNode PDFOptionsIO::getUniqueNode(QDomElement& parent, QString name)
{
	QDomNodeList nodes = parent.elementsByTagName(name);
	if (nodes.count() != 1)
	{
		m_error = QObject::tr("Unable to read settings XML:")
			.arg(QObject::tr("found %1 <%2> nodes, need 1.", "Load PDF settings")
				.arg(nodes.count()).arg(name)
			);
		return QDomNode();
	}
	return nodes.item(0);
}

// Return the node as a QDomElement iff it is a QDomElement with
// a `value' attribute; otherwise return a null element.
QDomElement PDFOptionsIO::getValueElement(QDomNode& node, QString name, bool isValue)
{
	if (node.isNull())
	{
		m_error = QObject::tr("Unable to read settings XML:")
			.arg(QObject::tr("unexpected null <%2> node", "Load PDF settings")
				.arg(name)
			);
		return QDomNode().toElement();
	}
	if (!node.isElement())
	{
		m_error = QObject::tr("Unable to read settings XML:")
			.arg(QObject::tr("node <%1> not an element", "Load PDF settings")
				.arg(name)
			);
		return QDomNode().toElement();
	}
	QDomElement elem = node.toElement();
	if (elem.tagName() != name)
	{
		m_error = QObject::tr("Unable to read settings XML:")
			.arg(QString("Internal error: element named <%1> not expected <%2>")
					.arg(elem.tagName()).arg(name)
			);
		return QDomNode().toElement();
	}
	if (isValue)
	{
		// We need to check that it has a `value' attribute
		if (!elem.hasAttribute("value"))
		{
			m_error = QObject::tr("Unable to read settings XML:")
				.arg(QObject::tr("element <%1> lacks `value' attribute", "Load PDF settings")
					.arg(name)
				);
			return QDomNode().toElement();
		}
	}
	return elem;
}

bool PDFOptionsIO::readElem(QDomElement& parent, QString name, bool* value)
{
	QDomNode node = getUniqueNode(parent, name);
	QDomElement elem = getValueElement(node, name);
	if (elem.isNull())
		return false;
	QString elementText = elem.attribute("value");
	if (elementText == "true")
	{
		(*value) = true;
		return true;
	}
	else if (elementText == "false")
	{
		(*value) = false;
		return true;
	}
	else
	{
		m_error = QObject::tr("Unable to read settings XML:")
			.arg(QObject::tr("element <%1> value must be `true' or `false'", "Load PDF settings")
				.arg(name)
			);
		return false;
	}
}

bool PDFOptionsIO::readElem(QDomElement& parent, QString name, int* value)
{
	QDomNode node = getUniqueNode(parent, name);
	QDomElement elem = getValueElement(node, name);
	if (elem.isNull())
		return false;
	bool ok = false;
	int result = elem.attribute("value").toInt(&ok);
	if (ok)
		(*value) = result;
	return ok;
}

bool PDFOptionsIO::readElem(QDomElement& parent, QString name, double* value)
{
	QDomNode node = getUniqueNode(parent, name);
	QDomElement elem = getValueElement(node, name);
	if (elem.isNull())
		return false;
	bool ok = false;
	double result = ScCLocale::toDoubleC(elem.attribute("value"), &ok);
	if (ok)
		(*value) = result;
	return ok;
}

bool PDFOptionsIO::readElem(QDomElement& parent, QString name, QString* value)
{
	QDomNode node = getUniqueNode(parent, name);
	QDomElement elem = getValueElement(node, name);
	if (elem.isNull())
		return false;
	QString result = elem.attribute("value");
	bool ok = !result.isNull();
	if (ok)
		(*value) = result;
	return ok;
}

// Read a stringlist saved as a list of child <item value=""> elements
bool PDFOptionsIO::readList(QDomElement& parent, QString name, QList<QString>* value)
{
	QDomNode basenode = getUniqueNode(parent, name);
	QDomElement listbase = getValueElement(basenode, name, false);
	if (listbase.isNull())
		return false;
	QList<QString> list;
	for (QDomNode node = listbase.firstChild(); !node.isNull(); node = node.nextSibling())
	{
		QDomElement elem = getValueElement(node, "item");
		if (elem.isNull())
			return false;
		list.append(elem.attribute("value"));
	}
	(*value) = list;
	return true;
}

bool PDFOptionsIO::readPresentationData()
{
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
	QDomNode basenode = getUniqueNode(m_root, "presentationSettings");
	QDomElement pSettings = getValueElement(basenode, "presentationSettings", false);
	if (pSettings.isNull())
		return false;
	QList<PDFPresentationData> pList;
	for (QDomNode node = pSettings.firstChild(); !node.isNull(); node = node.nextSibling())
	{
		QDomElement elem = getValueElement(basenode, "presentationSettingsEntry", false);
		if (elem.isNull())
			return false;
		PDFPresentationData pres;
		if (!readElem(elem, "pageEffectDuration", &pres.pageEffectDuration))
			return false;
		if (!readElem(elem, "pageViewDuration", &pres.pageViewDuration))
			return false;
		if (!readElem(elem, "effectType", &pres.effectType))
			return false;
		if (!readElem(elem, "dm", &pres.Dm))
			return false;
		if (!readElem(elem, "m", &pres.M))
			return false;
		if (!readElem(elem, "di", &pres.Di))
			return false;
		pList.append(pres);
	}
	return true;
}

bool PDFOptionsIO::readLPISettings()
{
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
	QDomNode basenode = getUniqueNode(m_root, "lpiSettings");
	QDomElement lpiSettings = getValueElement(basenode, "lpiSettings", false);
	if (lpiSettings.isNull())
		return false;
	QMap<QString,LPIData> lpiMap;
	for (QDomNode node = lpiSettings.firstChild(); !node.isNull(); node = node.nextSibling())
	{
		QDomElement elem = getValueElement(basenode, "lpiSettingsEntry", false);
		if (elem.isNull())
			return false;
		QString name (elem.attribute("name"));
		if (name.isNull())
		{
			m_error = QObject::tr("Unable to read settings XML:")
				.arg(QObject::tr("element <lpiSettingsEntry> lacks `name' attribute", "Load PDF settings")
					.arg(name)
				);
			return false;
		}
		LPIData lpi;
		if (!readElem(elem, "frequency", &lpi.Frequency))
			return false;
		if (!readElem(elem, "angle", &lpi.Angle))
			return false;
		if (!readElem(elem, "spotFunc", &lpi.SpotFunc))
			return false;
		lpiMap[name] = lpi;
	}
	return true;
}

const QString& PDFOptionsIO::lastError() const
{
	return m_error;
}

/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "barcode.h"
//#include "barcode.moc"
#include "barcodegenerator.h"

Barcode::Barcode() : ScActionPlugin()
{
	languageChange();
}

Barcode::~Barcode() {};

void Barcode::languageChange()
{
	m_actionInfo.name = "Barcode Generator";
	m_actionInfo.text = tr("&Barcode...");
	m_actionInfo.menu = "Insert";
	m_actionInfo.enabledOnStartup = true;
	m_actionInfo.needsNumObjects = -1;
}

const QString Barcode::fullTrName() const
{
	return QObject::tr("Barcode Generator");
}

const ScActionPlugin::AboutData* Barcode::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Terry Burton - <tez@terryburton.co.uk>, Petr Van\xc4\x9bk <petr@scribus.info>");
	about->shortDescription = tr("Scribus frontend for Pure Postscript Barcode Writer");
	about->description = "Barcode Writer in Pure Postscript is an award-winning open source barcode maker, as used by NASA, that facilitates the printing of all major barcode symbologies entirely within level 2 PostScript, ideal for variable data printing. The complete process of generating printed barcodes is performed entirely within the printer (or print system) so that it is no longer the responsibility of your application or a library. There is no need for any barcode fonts and the flexibility offered by direct PostScript means you can avoid re-implementing barcode generator code, or migrating to new libraries, whenever your project language needs change.\nhttp://www.terryburton.co.uk/barcodewriter/";
	about->version = "Backend: 2006-01-17";
	// about->releaseDate
	about->copyright = QString::fromUtf8("Backend: Copyright (c) 2005 Terry Burton - tez@terryburton.co.uk\nFrontend: Copyright (c) 2005 Petr Van\xc4\x9bk - petr@scribus.info");
	about->license = "Backend: MIT/X-Consortium, Frontend: GPL";
	return about;
}

void Barcode::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool Barcode::run(ScribusDoc*, QString /*target*/ )
{
	BarcodeGenerator *bg = new BarcodeGenerator();
	Q_CHECK_PTR(bg);
	bg->exec();
	delete bg;
	return true;
}

int barcodegenerator_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* barcodegenerator_getPlugin()
{
	Barcode* plug = new Barcode();
	Q_CHECK_PTR(plug);
	return plug;
}

void barcodegenerator_freePlugin(ScPlugin* plugin)
{
	Barcode* plug = dynamic_cast<Barcode*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "barcode.h"
#include "barcodegenerator.h"
#include "scribusstructs.h"
#include "util_icon.h"

Barcode::Barcode() : ScActionPlugin()
{
	languageChange();
}

Barcode::~Barcode() {};

void Barcode::languageChange()
{
	m_actionInfo.name = "BarcodeGenerator";
	m_actionInfo.text = tr("Insert Barcode");
	m_actionInfo.menu = "Insert";
	m_actionInfo.menuAfterName = "toolsInsertRenderFrame";
	m_actionInfo.toolbar = "Tools";
	m_actionInfo.icon1 = loadIcon("16/insert-barcode.png");
	m_actionInfo.icon2 = loadIcon("22/insert-barcode.png");
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.forAppMode.append(modeNormal);
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
	about->shortDescription = tr("Scribus frontend for Pure PostScript Barcode Writer");
	about->description = "Barcode Writer in Pure PostScript is an award-winning open source barcode maker, as used by NASA, that facilitates the printing of all major barcode symbologies entirely within level 2 PostScript, ideal for variable data printing. The complete process of generating printed barcodes is performed entirely within the printer (or print system) so that it is no longer the responsibility of your application or a library. There is no need for any barcode fonts and the flexibility offered by direct PostScript means you can avoid re-implementing barcode generator code, or migrating to new libraries, whenever your project language needs change.\nhttp://www.terryburton.co.uk/barcodewriter/";
	about->version = "Backend: 2014-01-30";
	// about->releaseDate
	about->copyright = QString::fromUtf8("Backend: Copyright (c) 2004-2014 Terry Burton - tez@terryburton.co.uk\nFrontend: Copyright (c) 2005 Petr Van\xc4\x9bk - petr@scribus.info");
	about->license = "Backend: MIT/X-Consortium, Frontend: GPL";
	return about;
}

void Barcode::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool Barcode::run(ScribusDoc* doc, QString /*target*/ )
{
	if (!doc)
		return false;
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

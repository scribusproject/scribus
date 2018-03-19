/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "barcode.h"
#include "barcodegenerator.h"
#include "scribuscore.h"
#include "scribusstructs.h"
#include "iconmanager.h"
#include "scpaths.h"

Barcode::Barcode() : ScActionPlugin()
{
	languageChange();
}

Barcode::~Barcode() {};

void Barcode::languageChange()
{
	m_actionInfo.name = "BarcodeGenerator";
	m_actionInfo.text = tr("Barcode");
	m_actionInfo.helpText = tr("Insert a barcode");
	if (ScCore->haveGS())
	{
		m_actionInfo.menu = "Insert";
		m_actionInfo.menuAfterName = "toolsInsertRenderFrame";
		m_actionInfo.toolbar = "Tools";
	}
	m_actionInfo.icon1 = IconManager::instance()->loadPixmap("16/insert-barcode.png");
	m_actionInfo.icon2 = IconManager::instance()->loadPixmap("22/insert-barcode.png");
	m_actionInfo.enabledOnStartup = false;
//	m_actionInfo.forAppMode.append(modeNormal);
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
	about->shortDescription = tr("Scribus frontend for Barcode Writer in Pure PostScript");
	about->description = "Barcode Writer in Pure Postscript generates all barcode formats entirely within PostScript hence this plugin requires Ghostscript to be installed on your system. https://bwipp.terryburton.co.uk";

	// Extract the version information from BWIPP
	QFile f( ScPaths::instance().shareDir() + QString("/plugins/barcode.ps") );
	if(f.open(QIODevice::ReadOnly))
	{
		QTextStream ts(&f);
		QString bwipp = ts.read(150);
		f.close();
		QRegExp rx("\\n% Barcode Writer in Pure PostScript - Version ([\\d-]+)\\n");
		rx.indexIn(bwipp);
		about->version = "Backend: "+rx.cap(1);
	}
	else
		about->version = "Unable to open backend file";
	// about->releaseDate
	about->copyright = QString::fromUtf8("Backend: Copyright (c) 2004-2018 Terry Burton - tez@terryburton.co.uk\nFrontend: Copyright (c) 2005 Petr Van\xc4\x9bk - petr@scribus.info");
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
	if (!doc || !ScCore->haveGS())
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

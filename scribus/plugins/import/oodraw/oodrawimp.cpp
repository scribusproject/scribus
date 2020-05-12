/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QCursor>
#include <QDir>
#include <QDrag>
#include <QDomElement>
#include <QFile>
#include <QMessageBox>
#include <QMimeData>
#include <QString>
#include <QRegExp>
#include <QWidget>

#include "oodrawimp.h"

#include "scconfig.h"

#include "commonstrings.h"
#include "fpointarray.h"
#include "pageitem.h"
#include "pluginmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scmimedata.h"
#include "scraction.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "serializer.h"
#include "stylestack.h"
#include "third_party/zip/scribus_zip.h"
#include "ui/customfdialog.h"
#include "ui/propertiespalette.h"
#include "ui/scmessagebox.h"
#include "ui/scmwmenumanager.h"
#include "undomanager.h"
#include "util.h"
#include "util_math.h"



using namespace std;

int oodrawimp_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* oodrawimp_getPlugin()
{
	OODrawImportPlugin* plug = new OODrawImportPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void oodrawimp_freePlugin(ScPlugin* plugin)
{
	OODrawImportPlugin* plug = dynamic_cast<OODrawImportPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

OODrawImportPlugin::OODrawImportPlugin() :
	importAction(new ScrAction(ScrAction::DLL, "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file formats.
	registerFormats();
	languageChange();
}

OODrawImportPlugin::~OODrawImportPlugin()
{
	unregisterAll();
	// note: importAction is automatically deleted by Qt
}

void OODrawImportPlugin::languageChange()
{
	importAction->setText( tr("Import &OpenOffice.org Draw..."));
//	FileFormat* fmt = getFormatByExt("odg");
//	fmt->trName = tr("OpenDocument 1.0 Draw", "Import/export format name");
//	fmt->filter = tr("OpenDocument 1.0 Draw (*.odg *.ODG)");
	FileFormat* fmt2 = getFormatByExt("sxd");
	fmt2->trName = tr("OpenOffice.org 1.x Draw", "Import/export format name");
	fmt2->filter = tr("OpenOffice.org 1.x Draw (*.sxd *.SXD)");
}

QString OODrawImportPlugin::fullTrName() const
{
	return QObject::tr("OpenOffice.org Draw Importer");
}

const ScActionPlugin::AboutData* OODrawImportPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports OpenOffice.org Draw Files");
	about->description = tr("Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void OODrawImportPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void OODrawImportPlugin::registerFormats()
{
//	QString odtName = tr("OpenDocument 1.0 Draw", "Import/export format name");
//	FileFormat odtformat(this);
//	odtformat.trName = odtName; // Human readable name
//	odtformat.formatId = 0;
//	odtformat.filter = odtName + " (*.odg *.ODG)"; // QFileDialog filter
//	odtformat.fileExtensions = QStringList() << "odg";
//	odtformat.load = true;
//	odtformat.save = false;
//	odtformat.thumb = true;
//	odtformat.mimeTypes = QStringList("application/vnd.oasis.opendocument.graphics"); // MIME types
//	odtformat.priority = 64; // Priority
//	registerFormat(odtformat);

	QString sxdName = tr("OpenOffice.org 1.x Draw", "Import/export format name");
	FileFormat sxdformat(this);
	sxdformat.trName = sxdName; // Human readable name
	sxdformat.formatId = 0;
	sxdformat.filter = sxdName + " (*.sxd *.SXD)"; // QFileDialog filter
	sxdformat.fileExtensions = QStringList() << "sxd";
	sxdformat.load = true;
	sxdformat.save = false;
	sxdformat.thumb = true;
	sxdformat.mimeTypes = QStringList("application/vnd.sun.xml.draw"); // MIME types
	sxdformat.priority = 64; // Priority
	registerFormat(sxdformat);
}

bool OODrawImportPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	// TODO: try to identify .sxd / .odt files
	return true;
}

bool OODrawImportPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// For this plugin, right now "load" and "import" are the same thing
	return import(fileName, flags);
}

bool OODrawImportPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if (fileName.isEmpty())
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance().prefsFile->getPluginContext("OODrawImport");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), QObject::tr("OpenOffice.org Draw (*.sxd *.SXD);;All Files (*)"));
		if (diaf.exec())
		{
			fileName = diaf.selectedFile();
			prefs->set("wdir", fileName.left(fileName.lastIndexOf("/")));
		}
		else
			return true;
	}
	m_Doc=ScCore->primaryMainWindow()->doc;
	UndoTransaction activeTransaction;
	bool emptyDoc = (m_Doc == nullptr);
	bool hasCurrentPage = (m_Doc && m_Doc->currentPage());
	TransactionSettings trSettings;
	trSettings.targetName   = hasCurrentPage ? m_Doc->currentPage()->getUName() : "";
	trSettings.targetPixmap = Um::IImageFrame;
	trSettings.actionName   = Um::ImportOOoDraw;
	trSettings.description  = fileName;
	trSettings.actionPixmap = Um::IImportOOoDraw;
	OODPlug dia(m_Doc);
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(false);
	if (UndoManager::undoEnabled())
		activeTransaction = UndoManager::instance()->beginTransaction(trSettings);
	bool importDone = dia.import(fileName, trSettings, flags);
	if (activeTransaction)
		activeTransaction.commit();
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(true);
	if (dia.importCanceled)
	{
		if ((!importDone) || (dia.importFailed))
			ScMessageBox::warning(ScCore->primaryMainWindow(), CommonStrings::trWarning, tr("The file could not be imported"));
		else if (dia.unsupported)
			ScMessageBox::warning(ScCore->primaryMainWindow(), CommonStrings::trWarning, tr("This file contains some unsupported features"));
	}
	return importDone;
}

QImage OODrawImportPlugin::readThumbnail(const QString& fileName)
{
	if (fileName.isEmpty())
		return QImage();
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = nullptr;
	OODPlug *dia = new OODPlug(m_Doc);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}

OODPlug::OODPlug(ScribusDoc* doc)
{
	m_Doc=doc;
	unsupported = false;
	interactive = false;
	importFailed = false;
	importCanceled = true;
	importedColors.clear();
	tmpSel=new Selection(this, false);
}

QImage OODPlug::readThumbnail(const QString& fileName)
{
	QByteArray f, f2, f3;
	if (!QFile::exists(fileName))
		return QImage();
	ScZipHandler* fun = new ScZipHandler();
	if (!fun->open(fileName))
	{
		delete fun;
		return QImage();
	}
	if (fun->contains("styles.xml"))
		fun->read("styles.xml", f);
	if (fun->contains("content.xml"))
		fun->read("content.xml", f2);
	if (fun->contains("meta.xml"))
		fun->read("meta.xml", f3);
	delete fun;
	HaveMeta = inpMeta.setContent(f3);
	QString docname = fileName.right(fileName.length() - fileName.lastIndexOf("/") - 1);
	docname = docname.left(docname.lastIndexOf("."));
	if (f.isEmpty())
		return QImage();
	if (f2.isEmpty())
		return QImage();
	if (!inpStyles.setContent(f))
		return QImage();
	if (!inpContents.setContent(f2))
		return QImage();
	QString CurDirP = QDir::currentPath();
	QFileInfo efp(fileName);
	QDir::setCurrent(efp.path());
	bool isOODraw2 = false;
	QDomNode drawPagePNode;
	QList<PageItem*> Elements;
	createStyleMap( inpStyles );
	QDomElement docElem = inpContents.documentElement();
	QDomNode automaticStyles = docElem.namedItem( "office:automatic-styles" );
	if (!automaticStyles.isNull())
		insertStyles( automaticStyles.toElement() );
	QDomNode body = docElem.namedItem( "office:body" );
	QDomNode drawPage = body.namedItem( "draw:page" );
	if (drawPage.isNull())
	{
		QDomNode offDraw = body.namedItem( "office:drawing" );
		drawPage = offDraw.namedItem( "draw:page" );
		if (drawPage.isNull())
			return QImage();
		isOODraw2 = true;
		drawPagePNode = body.namedItem( "office:drawing" );
	}
	else
		drawPagePNode = body;
	StyleStack::Mode mode = isOODraw2 ? StyleStack::OODraw2x : StyleStack::OODraw1x;
	m_styleStack.setMode( mode );
	QDomElement dp = drawPage.toElement();
	QDomElement *master = m_styles[dp.attribute( "draw:master-page-name" )];
	QDomElement *style = nullptr;
	QDomElement properties;
	if (isOODraw2)
	{
		style = m_styles.value(master->attribute( "style:page-layout-name" ), nullptr);
		if (style)
			properties = style->namedItem("style:page-layout-properties" ).toElement();
	}
	else
	{
		style = m_styles.value(master->attribute( "style:page-master-name" ), nullptr);
		if (style)
			properties = style->namedItem( "style:properties" ).toElement();
	}
	double width = !properties.attribute( "fo:page-width" ).isEmpty() ? parseUnit(properties.attribute( "fo:page-width" ) ) : 550.0;
	double height = !properties.attribute( "fo:page-height" ).isEmpty() ? parseUnit(properties.attribute( "fo:page-height" ) ) : 841.0;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(width, height, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), nullptr);
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	if (!m_Doc->PageColors.contains("Black"))
		m_Doc->PageColors.insert("Black", ScColor(0, 0, 0, 255));
	QDomNode drawPag = drawPagePNode.firstChild();
	QDomElement dpg = drawPag.toElement();
	m_styleStack.clear();
	fillStyleStack( dpg );
	QList<PageItem*> el = parseGroup( dpg );
	for (int ec = 0; ec < el.count(); ++ec)
		Elements.append(el.at(ec));
	tmpSel->clear();
	QImage tmpImage = QImage();
	if (Elements.count() > 0)
	{
		if (Elements.count() > 1)
			m_Doc->groupObjectsList(Elements);
		m_Doc->DoDrawing = true;
		m_Doc->m_Selection->delaySignalsOn();
		for (int dre=0; dre<Elements.count(); ++dre)
		{
			tmpSel->addItem(Elements.at(dre), true);
		}
		tmpSel->setGroupRect();
		double xs = tmpSel->width();
		double ys = tmpSel->height();
		tmpImage = Elements.at(0)->DrawObj_toImage(500);
		tmpImage.setText("XSize", QString("%1").arg(xs));
		tmpImage.setText("YSize", QString("%1").arg(ys));
		m_Doc->m_Selection->delaySignalsOff();
		m_Doc->setLoading(false);
	}
	m_Doc->scMW()->setScriptRunning(false);
	delete m_Doc;
	QDir::setCurrent(CurDirP);
	return tmpImage;
}

bool OODPlug::import(const QString& fileName, const TransactionSettings& trSettings, int flags)
{
	bool importDone = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	QByteArray f, f2, f3;
	if (!QFile::exists(fileName))
		return false;
	ScZipHandler* fun = new ScZipHandler();
	if (!fun->open(fileName))
	{
		delete fun;
		return false;
	}
	if (fun->contains("styles.xml"))
		fun->read("styles.xml", f);
	if (fun->contains("content.xml"))
		fun->read("content.xml", f2);
	if (fun->contains("meta.xml"))
		fun->read("meta.xml", f3);
	delete fun;
	HaveMeta = inpMeta.setContent(f3);
	QString docname = fileName.right(fileName.length() - fileName.lastIndexOf("/") - 1);
	docname = docname.left(docname.lastIndexOf("."));
	if (f.isEmpty())
		return false;
	if (f2.isEmpty())
		return false;
	if (!inpStyles.setContent(f))
		return false;
	if (!inpContents.setContent(f2))
		return false;
	QString CurDirP = QDir::currentPath();
	QFileInfo efp(fileName);
	QDir::setCurrent(efp.path());
	importDone = convert(trSettings, flags);
	QDir::setCurrent(CurDirP);
	return importDone;
}

bool OODPlug::convert(const TransactionSettings& trSettings, int flags)
{
	bool ret = false;
	bool isOODraw2 = false;
	QDomNode drawPagePNode;
	int PageCounter = 0;
	QList<PageItem*> Elements;
	createStyleMap( inpStyles );
	QDomElement docElem = inpContents.documentElement();
	QDomNode automaticStyles = docElem.namedItem( "office:automatic-styles" );
	if (!automaticStyles.isNull() )
		insertStyles( automaticStyles.toElement() );
	QDomNode body = docElem.namedItem( "office:body" );
	QDomNode drawPage = body.namedItem( "draw:page" );
	if (drawPage.isNull())
	{
		QDomNode offDraw = body.namedItem( "office:drawing" );
		drawPage = offDraw.namedItem( "draw:page" );
		if (drawPage.isNull())
		{
			ScMessageBox::warning( m_Doc->scMW(), CommonStrings::trWarning, tr("This document does not seem to be an OpenOffice Draw file.") );
			return false;
		}
		isOODraw2 = true;
		drawPagePNode = body.namedItem( "office:drawing" );
	}
	else
		drawPagePNode = body;
	StyleStack::Mode mode = isOODraw2 ? StyleStack::OODraw2x : StyleStack::OODraw1x;
	m_styleStack.setMode( mode );
	QDomElement dp = drawPage.toElement();
	QDomElement *master = m_styles[dp.attribute( "draw:master-page-name" )];
	QDomElement *style = nullptr;
	QDomElement properties;
	if (isOODraw2)
	{
		style = m_styles.value(master->attribute( "style:page-layout-name" ), nullptr);
		if (style)
			properties = style->namedItem("style:page-layout-properties" ).toElement();
	}
	else
	{
		style = m_styles.value(master->attribute( "style:page-master-name" ), nullptr);
		if (style)
			properties = style->namedItem( "style:properties" ).toElement();
	}
	double width = !properties.attribute( "fo:page-width" ).isEmpty() ? parseUnit(properties.attribute( "fo:page-width" ) ) : 550.0;
	double height = !properties.attribute( "fo:page-height" ).isEmpty() ? parseUnit(properties.attribute( "fo:page-height" ) ) : 841.0;
	if (!interactive || (flags & LoadSavePlugin::lfInsertPage))
		m_Doc->setPage(width, height, 0, 0, 0, 0, 0, 0, false, false);
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(width, height, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
		}
	}
	if ((ret) || (!interactive))
	{
		if (width > height)
			m_Doc->setPageOrientation(1);
		else
			m_Doc->setPageOrientation(0);
		m_Doc->setPageSize("Custom");
		QDomNode mpg;
		QDomElement metaElem = inpMeta.documentElement();
		QDomElement mp = metaElem.namedItem( "office:meta" ).toElement();
		mpg = mp.namedItem( "dc:title" );
		if (!mpg.isNull())
			m_Doc->documentInfo().setTitle(mpg.toElement().text());
		mpg = mp.namedItem( "meta:initial-creator" );
		if (!mpg.isNull())
			m_Doc->documentInfo().setAuthor(mpg.toElement().text());
		mpg = mp.namedItem( "dc:description" );
		if (!mpg.isNull())
			m_Doc->documentInfo().setComments(mpg.toElement().text());
		mpg = mp.namedItem( "dc:language" );
		if (!mpg.isNull())
			m_Doc->documentInfo().setLangInfo(mpg.toElement().text());
		mpg = mp.namedItem( "meta:creation-date" );
		if (!mpg.isNull())
			m_Doc->documentInfo().setDate(mpg.toElement().text());
		mpg = mp.namedItem( "dc:creator" );
		if (!mpg.isNull())
			m_Doc->documentInfo().setContrib(mpg.toElement().text());
		mpg = mp.namedItem( "meta:keywords" );
		if (!mpg.isNull())
		{
			QString Keys = "";
			for (QDomNode n = mpg.firstChild(); !n.isNull(); n = n.nextSibling())
			{
				Keys += n.toElement().text()+", ";
			}
			if (Keys.length() > 2)
				m_Doc->documentInfo().setKeywords(Keys.left(Keys.length()-2));
		}
	}
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
		m_Doc->view()->deselectItems();
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
		m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->setScriptRunning(true);
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	if (!m_Doc->PageColors.contains("Black"))
		m_Doc->PageColors.insert("Black", ScColor(0, 0, 0, 255));
	for (QDomNode drawPag = drawPagePNode.firstChild(); !drawPag.isNull(); drawPag = drawPag.nextSibling())
	{
		QDomElement dpg = drawPag.toElement();
		if (!interactive)
		{
			m_Doc->addPage(PageCounter);
			m_Doc->view()->addPage(PageCounter);
		}
		PageCounter++;
		m_styleStack.clear();
		fillStyleStack( dpg );
		QList<PageItem*> el = parseGroup( dpg );
		for (int ec = 0; ec < el.count(); ++ec)
			Elements.append(el.at(ec));
		if ((interactive) && (PageCounter == 1))
			break;
	}
	tmpSel->clear();
//	if ((Elements.count() > 1) && (interactive))
	if (Elements.count() == 0)
	{
		importFailed = true;
		if (importedColors.count() != 0)
		{
			for (int cd = 0; cd < importedColors.count(); cd++)
			{
				m_Doc->PageColors.remove(importedColors[cd]);
			}
		}
	}
	if (Elements.count() > 1)
		m_Doc->groupObjectsList(Elements);
	m_Doc->DoDrawing = true;
	m_Doc->scMW()->setScriptRunning(false);
	if (interactive)
		m_Doc->setLoading(false);
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	if ((Elements.count() > 0) && (!ret) && (interactive))
	{
		if (flags & LoadSavePlugin::lfScripted)
		{
			bool loadF = m_Doc->isLoading();
			m_Doc->setLoading(false);
			m_Doc->changed();
			m_Doc->setLoading(loadF);
			if (!(flags & LoadSavePlugin::lfLoadAsPattern))
			{
				m_Doc->m_Selection->delaySignalsOn();
				for (int dre=0; dre<Elements.count(); ++dre)
				{
					m_Doc->m_Selection->addItem(Elements.at(dre), true);
				}
				m_Doc->m_Selection->delaySignalsOff();
				m_Doc->m_Selection->setGroupRect();
				m_Doc->view()->updatesOn(true);
			}
			importCanceled = false;
		}
		else
		{
			m_Doc->DragP = true;
			m_Doc->DraggedElem = nullptr;
			m_Doc->DragElements.clear();
			m_Doc->m_Selection->delaySignalsOn();
			for (int dre=0; dre<Elements.count(); ++dre)
			{
				tmpSel->addItem(Elements.at(dre), true);
			}
			tmpSel->setGroupRect();
			ScElemMimeData* md = ScriXmlDoc::writeToMimeData(m_Doc, tmpSel);
			m_Doc->itemSelection_DeleteItem(tmpSel);
			m_Doc->view()->updatesOn(true);
			m_Doc->m_Selection->delaySignalsOff();
			// We must copy the TransationSettings object as it is owned
			// by handleObjectImport method afterwards
			TransactionSettings* transacSettings = new TransactionSettings(trSettings);
			m_Doc->view()->handleObjectImport(md, transacSettings);
			m_Doc->DragP = false;
			m_Doc->DraggedElem = nullptr;
			m_Doc->DragElements.clear();
		}
	}
	else
	{
		bool loadF = m_Doc->isLoading();
		m_Doc->setLoading(false);
		m_Doc->changed();
		m_Doc->reformPages();
		if (!(flags & LoadSavePlugin::lfLoadAsPattern))
			m_Doc->view()->updatesOn(true);
		m_Doc->setLoading(loadF);
	}
	qApp->restoreOverrideCursor();
	return true;
}

QList<PageItem*> OODPlug::parseGroup(const QDomElement &e)
{
	OODrawStyle oostyle;
	FPointArray ImgClip;
	QList<PageItem*> elements, cElements;
//	double BaseX = m_Doc->currentPage()->xOffset();
//	double BaseY = m_Doc->currentPage()->yOffset();
	storeObjectStyles(e);
	parseStyle(oostyle, e);
//	QString drawID = e.attribute("draw:name");
//	int zn = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, BaseX, BaseY, 1, 1, 0, CommonStrings::None, CommonStrings::None);
//	PageItem *neu = m_Doc->Items->at(zn);
	for (QDomNode n = e.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		QDomElement b = n.toElement();
		if (b.isNull() )
			continue;
		QList<PageItem*> el = parseElement(b);
		for (int ec = 0; ec < el.count(); ++ec)
			cElements.append(el.at(ec));
	}
	if (cElements.count() < 2)
	{
		for (int gr = 0; gr < cElements.count(); ++gr)
		{
			elements.append(cElements.at(gr));
		}
	}
	else
	{
		PageItem *neu = m_Doc->groupObjectsList(cElements);
		elements.append(neu);
		if (!e.attribute("id").isEmpty() )
			neu->setItemName(e.attribute("id"));
		else
			neu->setItemName( tr("Group%1").arg(m_Doc->GroupCounter));
	}
	return elements;
}

QList<PageItem*> OODPlug::parseElement(const QDomElement &e)
{
	QList<PageItem*> GElements;
	QString STag = e.tagName();
	if (STag == "draw:g")
	{
		GElements = parseGroup(e);
		return GElements;
	}
	if (STag == "draw:rect")
		GElements = parseRect(e);
	else if (STag == "draw:circle" || STag == "draw:ellipse")
		GElements = parseEllipse(e);
	else if (STag == "draw:line")
		GElements = parseLine(e);
	else if (STag == "draw:polygon")
		GElements = parsePolygon(e);
	else if (STag == "draw:polyline")
		GElements = parsePolyline(e);
	else if (STag == "draw:path")
		GElements = parsePath(e);
	else if (STag == "draw:text-box")
		GElements = parseTextBox(e);
	else if (STag == "draw:frame")
		GElements = parseFrame(e);
	else if (STag == "draw:connector")
		GElements = parseConnector(e);
	else
	{
		// warn if unsupported feature are encountered
		unsupported = true;
		qDebug("Not supported yet: %s", STag.toLocal8Bit().data());
	}
	return GElements;
}

QList<PageItem*> OODPlug::parseRect(const QDomElement &e)
{
	OODrawStyle style;
	QList<PageItem*> elements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double x = parseUnit(e.attribute("svg:x"));
	double y = parseUnit(e.attribute("svg:y")) ;
	double w = parseUnit(e.attribute("svg:width"));
	double h = parseUnit(e.attribute("svg:height"));
	double corner = parseUnit(e.attribute("draw:corner-radius"));
	storeObjectStyles(e);
	parseStyle(style, e);
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, BaseX+x, BaseY+y, w, h, style.strokeWidth, style.fillColor, style.strokeColor);
	PageItem* ite = m_Doc->Items->at(z);
	if (corner != 0)
	{
		ite->setCornerRadius(corner);
		ite->SetFrameRound();
		m_Doc->setRedrawBounding(ite);
	}
	ite = finishNodeParsing(e, ite, style);
	elements.append(ite);
	return elements;
}

QList<PageItem*> OODPlug::parseEllipse(const QDomElement &e)
{
	OODrawStyle style;
	QList<PageItem*> elements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double x = parseUnit(e.attribute("svg:x"));
	double y = parseUnit(e.attribute("svg:y")) ;
	double w = parseUnit(e.attribute("svg:width"));
	double h = parseUnit(e.attribute("svg:height"));
	storeObjectStyles(e);
	parseStyle(style, e);
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, BaseX+x, BaseY+y, w, h, style.strokeWidth, style.fillColor, style.strokeColor);
	PageItem* ite = m_Doc->Items->at(z);
	ite = finishNodeParsing(e, ite, style);
	elements.append(ite);
	return elements;
}

QList<PageItem*> OODPlug::parseLine(const QDomElement &e)
{
	OODrawStyle style;
	QList<PageItem*> elements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double x1 = e.attribute( "svg:x1" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:x1" ) );
	double y1 = e.attribute( "svg:y1" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:y1" ) );
	double x2 = e.attribute( "svg:x2" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:x2" ) );
	double y2 = e.attribute( "svg:y2" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:y2" ) );
	storeObjectStyles(e);
	parseStyle(style, e);
	int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, BaseX, BaseY, 10, 10, style.strokeWidth, CommonStrings::None, style.strokeColor);
	PageItem* ite = m_Doc->Items->at(z);
	ite->PoLine.resize(4);
	ite->PoLine.setPoint(0, FPoint(x1, y1));
	ite->PoLine.setPoint(1, FPoint(x1, y1));
	ite->PoLine.setPoint(2, FPoint(x2, y2));
	ite->PoLine.setPoint(3, FPoint(x2, y2));
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(), wh.y());
	ite->ClipEdited = true;
	ite->FrameType = 3;
	if (!e.hasAttribute("draw:transform"))
	{
		ite->Clip = flattenPath(ite->PoLine, ite->Segments);
		m_Doc->adjustItemSize(ite);
	}
	ite = finishNodeParsing(e, ite, style);
	elements.append(ite);
	return elements;
}

QList<PageItem*> OODPlug::parsePolygon(const QDomElement &e)
{
	OODrawStyle style;
	QList<PageItem*> elements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	storeObjectStyles(e);
	parseStyle(style, e);
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, 10, 10, style.strokeWidth, style.fillColor, style.strokeColor);
	PageItem* ite = m_Doc->Items->at(z);
	ite->PoLine.resize(0);
	appendPoints(&ite->PoLine, e, true);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(), wh.y());
	ite->ClipEdited = true;
	ite->FrameType = 3;
	if (!e.hasAttribute("draw:transform"))
	{
		ite->Clip = flattenPath(ite->PoLine, ite->Segments);
		m_Doc->adjustItemSize(ite);
	}
	ite = finishNodeParsing(e, ite, style);
	elements.append(ite);
	return elements;
}

QList<PageItem*> OODPlug::parsePolyline(const QDomElement &e)
{
	OODrawStyle style;
	QList<PageItem*> elements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	storeObjectStyles(e);
	parseStyle(style, e);
	int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, BaseX, BaseY, 10, 10, style.strokeWidth, CommonStrings::None, style.strokeColor);
	PageItem* ite = m_Doc->Items->at(z);
	ite->PoLine.resize(0);
	appendPoints(&ite->PoLine, e, false);
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(), wh.y());
	ite->ClipEdited = true;
	ite->FrameType = 3;
	if (!e.hasAttribute("draw:transform"))
	{
		ite->Clip = flattenPath(ite->PoLine, ite->Segments);
		m_Doc->adjustItemSize(ite);
	}
	ite = finishNodeParsing(e, ite, style);
	elements.append(ite);
	return elements;
}

QList<PageItem*> OODPlug::parsePath(const QDomElement &e)
{
	OODrawStyle style;
	FPointArray pArray;
	QList<PageItem*> elements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	storeObjectStyles(e);
	parseStyle(style, e);
	PageItem::ItemType itype = parseSVG(e.attribute("svg:d"), &pArray) ? PageItem::PolyLine : PageItem::Polygon;
	int z = m_Doc->itemAdd(itype, PageItem::Unspecified, BaseX, BaseY, 10, 10, style.strokeWidth, style.fillColor, style.strokeColor);
	PageItem* ite = m_Doc->Items->at(z);
	ite->PoLine.resize(0);
	ite->PoLine = pArray;
	if (ite->PoLine.size() < 4)
	{
// 		m_Doc->m_Selection->addItem(ite);
		tmpSel->addItem(ite);
// 		m_Doc->itemSelection_DeleteItem();
		m_Doc->itemSelection_DeleteItem(tmpSel);
	}
	else
	{
		QTransform mat;
		double x = parseUnit(e.attribute("svg:x"));
		double y = parseUnit(e.attribute("svg:y")) ;
		double w = parseUnit(e.attribute("svg:width"));
		double h = parseUnit(e.attribute("svg:height"));
		double vx = 0;
		double vy = 0;
		double vw = 1;
		double vh = 1;
		parseViewBox(e, &vx, &vy, &vw, &vh);
		double sx = (vw != 0.0) ? (w / vw) : w;
		double sy = (vh != 0.0) ? (h / vh) : h;
		mat.translate(x, y);
		mat.scale(sx, sy);
		ite->PoLine.map(mat);
		FPoint wh = getMaxClipF(&ite->PoLine);
		ite->setWidthHeight(wh.x(), wh.y());
		ite->ClipEdited = true;
		ite->FrameType = 3;
		if (!e.hasAttribute("draw:transform"))
		{
			ite->Clip = flattenPath(ite->PoLine, ite->Segments);
			m_Doc->adjustItemSize(ite);
		}
		ite = finishNodeParsing(e, ite, style);
		elements.append(ite);
	}
	return elements;
}

QList<PageItem*> OODPlug::parseTextBox(const QDomElement &e)
{
	OODrawStyle style;
	QList<PageItem*> elements;
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double x = parseUnit(e.attribute("svg:x"));
	double y = parseUnit(e.attribute("svg:y")) ;
	double w = parseUnit(e.attribute("svg:width"));
	double h = parseUnit(e.attribute("svg:height"));
	storeObjectStyles(e);
	parseStyle(style, e);
	int z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, BaseX+x, BaseY+y, w, h+(h*0.1), style.strokeWidth, CommonStrings::None, style.strokeColor);
	PageItem* ite = m_Doc->Items->at(z);
	ite->setFillColor(style.fillColor);
	ite->setLineColor(style.strokeColor);
	ite = finishNodeParsing(e, ite, style);
	elements.append(ite);
	return elements;
}

QList<PageItem*> OODPlug::parseFrame(const QDomElement &e)
{
	OODrawStyle oostyle;
	QList<PageItem*> elements;
	QString drawID = e.attribute("draw:name");
	double BaseX = m_Doc->currentPage()->xOffset();
	double BaseY = m_Doc->currentPage()->yOffset();
	double x = parseUnit(e.attribute("svg:x"));
	double y = parseUnit(e.attribute("svg:y")) ;
	double w = parseUnit(e.attribute("svg:width"));
	double h = parseUnit(e.attribute("svg:height"));
	storeObjectStyles(e);
	parseStyle(oostyle, e);
	QDomNode n = e.firstChild();
	QString STag2 = n.toElement().tagName();
	if (STag2 == "draw:text-box")
	{
		int z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, BaseX+x, BaseY+y, w, h+(h*0.1), oostyle.strokeWidth, CommonStrings::None, oostyle.strokeColor);
		PageItem* ite = m_Doc->Items->at(z);
		ite->setTextToFrameDist(0.0, 0.0, 0.0, 0.0);
		ite->setFillTransparency(oostyle.fillTrans);
		ite->setLineTransparency(oostyle.strokeTrans);
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		if (!drawID.isEmpty())
			ite->setItemName(drawID);
		ite = parseTextP(n.toElement(), ite);
		elements.append(ite);
	}
	return elements;
}

QList<PageItem*> OODPlug::parseConnector(const QDomElement &e)
{
	QList<PageItem*> elements;
	if (e.hasAttribute("svg:x1") && e.hasAttribute("svg:x2") && e.hasAttribute("svg:y1") && e.hasAttribute("svg:y2"))
	{
		elements = parseLine(e);
	}
	else
	{
		unsupported = true;
		qDebug("an unsupported form of connector was found");
	}
	return elements;
}

void OODPlug::parseStyle(OODrawStyle& oostyle, const QDomElement &e)
{
	oostyle.haveGradient = false;
	oostyle.gradient.clearStops();
	if (m_styleStack.hasAttribute("draw:stroke") )
	{
		if (m_styleStack.attribute( "draw:stroke" ) == "none" )
			oostyle.strokeWidth = 0.0;
		else
		{
			if (m_styleStack.hasAttribute("svg:stroke-width"))
			{
				oostyle.strokeWidth = parseUnit(m_styleStack.attribute("svg:stroke-width"));
				if (oostyle.strokeWidth == 0)
					oostyle.strokeWidth = 1;
			}
			if (m_styleStack.hasAttribute("svg:stroke-color"))
				oostyle.strokeColor = parseColor(m_styleStack.attribute("svg:stroke-color"));
			if (m_styleStack.hasAttribute( "svg:stroke-opacity" ) )
				oostyle.strokeTrans = 1.0 - ScCLocale::toDoubleC(m_styleStack.attribute( "svg:stroke-opacity" ).remove( '%' )) / 100.0;
			if (m_styleStack.attribute( "draw:stroke" ) == "dash" )
			{
				QString style = m_styleStack.attribute( "draw:stroke-dash" );
				if (style == "Ultrafine Dashed")
					oostyle.dashes << 1.4 << 1.4;
				else if (style == "Fine Dashed" )
					oostyle.dashes << 14.4 << 14.4;
				else if (style == "Fine Dotted")
					oostyle.dashes << 13 << 13;
				else if (style == "Ultrafine 2 Dots 3 Dashes")
					oostyle.dashes << 1.45 << 3.6 << 1.45 << 3.6 << 7.2 << 3.6 << 7.2 << 3.6 << 7.2 << 3.6;
				else if (style == "Line with Fine Dots")
				{
					oostyle.dashes << 56.9 << 4.31;
					for (int dd = 0; dd < 10; ++ dd)
					{
						oostyle.dashes << 8.6 << 4.31;
					}
				}
				else if (style == "2 Dots 1 Dash" )
					oostyle.dashes << 2.8 << 5.75 << 2.8 << 5.75 << 5.75 << 5.75;
			}
		}
	}
	if (m_styleStack.hasAttribute( "draw:fill" ) )
	{
		QString fill = m_styleStack.attribute( "draw:fill" );
		if (fill == "solid" )
		{
			if (m_styleStack.hasAttribute( "draw:fill-color" ) )
				oostyle.fillColor = parseColor( m_styleStack.attribute("draw:fill-color"));
			if (m_styleStack.hasAttribute( "draw:transparency" ) )
				oostyle.fillTrans = 1.0 - ScCLocale::toDoubleC(m_styleStack.attribute( "draw:transparency" ).remove( '%' )) / 100.0;
		}
		else if (fill == "gradient" )
		{
			oostyle.haveGradient = true;
			oostyle.gradientAngle = 0;
			oostyle.gradient.clearStops();
			oostyle.gradient.setRepeatMethod( VGradient::none );
			QString style = m_styleStack.attribute( "draw:fill-gradient-name" );
			QDomElement* draw = m_draws[style];
			if (draw )
			{
				double border = 0.0;
				int shadeS = 100;
				int shadeE = 100;
				if (draw->hasAttribute( "draw:border" ) )
					border += ScCLocale::toDoubleC(draw->attribute( "draw:border" ).remove( '%' )) / 100.0;
				if (draw->hasAttribute( "draw:start-intensity" ) )
					shadeS = draw->attribute( "draw:start-intensity" ).remove( '%' ).toInt();
				if (draw->hasAttribute( "draw:end-intensity" ) )
					shadeE = draw->attribute( "draw:end-intensity" ).remove( '%' ).toInt();
				QString type = draw->attribute( "draw:style" );
				if (type == "linear" || type == "axial" )
				{
					oostyle.gradient.setType( VGradient::linear );
					oostyle.gradientAngle = ScCLocale::toDoubleC(draw->attribute( "draw:angle" )) / 10;
					oostyle.gradientType = 1;
				}
				else if (type == "radial" || type == "ellipsoid" )
				{
					if (draw->hasAttribute( "draw:cx" ) )
						oostyle.gradientPointX = ScCLocale::toDoubleC(draw->attribute( "draw:cx" ).remove( '%' )) / 100.0;
					else
						oostyle.gradientPointX = 0.5;
					if (draw->hasAttribute( "draw:cy" ) )
						oostyle.gradientPointY = ScCLocale::toDoubleC(draw->attribute( "draw:cy" ).remove( '%' )) / 100.0;
					else
						oostyle.gradientPointY = 0.5;
					oostyle.gradientType = 2;
				}
				QString c, c2;
				c = parseColor( draw->attribute( "draw:start-color" ) );
				c2 = parseColor( draw->attribute( "draw:end-color" ) );
				const ScColor& col1 = m_Doc->PageColors[c];
				const ScColor& col2 = m_Doc->PageColors[c2];
				if (((oostyle.gradientAngle > 90) && (oostyle.gradientAngle < 271)) || (oostyle.gradientType == 2))
				{
					const ScColor& col1 = m_Doc->PageColors[c];
					const ScColor& col2 = m_Doc->PageColors[c2];
					oostyle.gradient.addStop( ScColorEngine::getShadeColor(col2, m_Doc, shadeE), 0.0, 0.5, 1, c2, shadeE );
					oostyle.gradient.addStop( ScColorEngine::getShadeColor(col1, m_Doc, shadeS), 1.0 - border, 0.5, 1, c, shadeS );
				}
				else
				{
					oostyle.gradient.addStop( ScColorEngine::getShadeColor(col1, m_Doc, shadeS), border, 0.5, 1, c, shadeS );
					oostyle.gradient.addStop( ScColorEngine::getShadeColor(col2, m_Doc, shadeE), 1.0, 0.5, 1, c2, shadeE );
				}
			}
		}
	}
}

void OODPlug::parseCharStyle(CharStyle& style, const QDomElement &e)
{
	if (m_styleStack.hasAttribute("fo:font-size"))
	{
		QString fs = m_styleStack.attribute("fo:font-size").remove( "pt" );
		int FontSize = (int) (ScCLocale::toFloatC(fs) * 10.0);
		style.setFontSize(FontSize);
	}
}

void OODPlug::parseParagraphStyle(ParagraphStyle& style, const QDomElement &e)
{
	if (m_styleStack.hasAttribute("fo:text-align"))
	{
		QString attValue = m_styleStack.attribute("fo:text-align");
		if (attValue == "left")
			style.setAlignment(ParagraphStyle::LeftAligned);
		if (attValue == "center")
			style.setAlignment(ParagraphStyle::Centered);
		if (attValue == "right")
			style.setAlignment(ParagraphStyle::RightAligned);
	}
	if (m_styleStack.hasAttribute("fo:font-size"))
	{
		QString fs = m_styleStack.attribute("fo:font-size").remove( "pt" );
		int FontSize = (int) (ScCLocale::toFloatC(fs) * 10.0);
		style.charStyle().setFontSize(FontSize);
		style.setLineSpacing((FontSize + FontSize * 0.2) / 10.0);
	}
}

PageItem* OODPlug::parseTextP (const QDomElement& elm, PageItem* item)
{
	for ( QDomNode n = elm.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		if (!n.hasAttributes() && !n.hasChildNodes())
			continue;
		QDomElement e = n.toElement();
		if (e.text().isEmpty())
			continue;
		storeObjectStyles(e);
		item->itemText.insertChars(-1, SpecialChars::PARSEP);
		item = parseTextSpans(e, item);
	}
	return item;
}

PageItem* OODPlug::parseTextSpans(const QDomElement& elm, PageItem* item)
{
	bool firstSpan = true;
	for ( QDomNode n = elm.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		QString chars;
		QDomElement e = n.toElement();
		if (n.isElement() && (e.tagName() == "text:span"))
		{
			chars = e.text().simplified();
			storeObjectStyles(e);
		}
		if (n.isText())
		{
			QDomText t = n.toText();
			chars = t.data().simplified();
		}
		if (chars.isEmpty())
			continue;
		int pos = item->itemText.length();
		if (firstSpan && (m_styleStack.hasAttribute("fo:text-align") || m_styleStack.hasAttribute("fo:font-size")))
		{
			ParagraphStyle newStyle;
			parseParagraphStyle(newStyle, n.isElement() ? e : elm);
			item->itemText.applyStyle(-1, newStyle);
		}
		item->itemText.insertChars( -2, chars);
		if (!firstSpan && m_styleStack.hasAttribute("fo:font-size"))
		{
			CharStyle newStyle;
			parseCharStyle(newStyle, n.isElement() ? e : elm);
			item->itemText.applyCharStyle(pos, chars.length(), newStyle);
		}
		if (!item->asPolyLine() && !item->asTextFrame())
			item = m_Doc->convertItemTo(item, PageItem::TextFrame);
		firstSpan = false;
	}
	return item;
}

PageItem* OODPlug::finishNodeParsing(const QDomElement &elm, PageItem* item, OODrawStyle& oostyle)
{
	item->setTextToFrameDist(0.0, 0.0, 0.0, 0.0);
	item->itemText.trim();
//	bool firstPa = false;
	QString drawID = elm.attribute("draw:name");
	item = parseTextP(elm, item);
	item->setFillTransparency(oostyle.fillTrans);
	item->setLineTransparency(oostyle.strokeTrans);
	if (oostyle.dashes.count() != 0)
		item->DashValues = oostyle.dashes;
	if (!drawID.isEmpty())
		item->setItemName(drawID);
	if (elm.hasAttribute("draw:transform"))
	{
		parseTransform(&item->PoLine, elm.attribute("draw:transform"));
		item->ClipEdited = true;
		item->FrameType = 3;
		FPoint wh = getMaxClipF(&item->PoLine);
		item->setWidthHeight(wh.x(), wh.y());
		item->Clip = flattenPath(item->PoLine, item->Segments);
		m_Doc->adjustItemSize(item);
	}
	item->OwnPage = m_Doc->OnPage(item);
	//ite->setTextFlowMode(PageItem::TextFlowUsesFrameShape);
	item->setTextFlowMode(PageItem::TextFlowDisabled);
	if (oostyle.haveGradient)
	{
		item->GrType = 0;
		if (oostyle.gradient.stops() > 1)
		{
			item->fill_gradient = oostyle.gradient;
			if (oostyle.gradientType == 1)
			{
				bool flipped = false;
				double gradientAngle(oostyle.gradientAngle);
				if ((gradientAngle == 0) || (gradientAngle == 180) || (gradientAngle == 90) || (gradientAngle == 270))
				{
					if ((gradientAngle == 0) || (gradientAngle == 180))
					{
						item->GrType = 6;
						item->GrStartX = item->width() / 2.0;
						item->GrStartY = 0;
						item->GrEndX = item->width() / 2.0;
						item->GrEndY = item->height();
					}
					else if ((gradientAngle == 90) || (gradientAngle == 270))
					{
						item->GrType = 6;
						item->GrStartX = 0;
						item->GrStartY = item->height() / 2.0;
						item->GrEndX = item->width();
						item->GrEndY = item->height() / 2.0;
					}
				}
				else
				{
					if ((gradientAngle > 90) && (gradientAngle < 270))
						gradientAngle -= 180;
					else if ((gradientAngle > 270) && (gradientAngle < 360))
					{
						gradientAngle = 360 - gradientAngle;
						flipped = true;
					}
					double xpos;
					xpos = (item->width() / 2) * tan(gradientAngle* M_PI / 180.0) * (item->height() / item->width()) + (item->width() / 2);
					if ((xpos < 0) || (xpos > item->width()))
					{
						xpos = (item->height() / 2)- (item->height() / 2) * tan(gradientAngle* M_PI / 180.0) * (item->height() / item->width());
						if (flipped)
						{
							item->GrEndX = item->width();
							item->GrEndY = item->height() - xpos;
							item->GrStartX = 0;
							item->GrStartY = xpos;
						}
						else
						{
							item->GrEndY = xpos;
							item->GrEndX = item->width();
							item->GrStartX = 0;
							item->GrStartY = item->height() - xpos;
						}
					}
					else
					{
						item->GrEndX = xpos;
						item->GrEndY = item->height();
						item->GrStartX = item->width() - xpos;
						item->GrStartY = 0;
					}
					if (flipped)
					{
						item->GrEndX = item->width() - xpos;
						item->GrEndY = item->height();
						item->GrStartX = xpos;
						item->GrStartY = 0;
					}
					item->GrType = 6;
				}
			}
			if (oostyle.gradientType == 2)
			{
				item->GrType = 7;
				item->GrStartX = item->width() * oostyle.gradientPointX;
				item->GrStartY = item->height()* oostyle.gradientPointY;
				if (item->width() >= item->height())
				{
					item->GrEndX = item->width();
					item->GrEndY = item->height() / 2.0;
				}
				else
				{
					item->GrEndX = item->width() / 2.0;
					item->GrEndY = item->height();
				}
				//m_Doc->view()->updateGradientVectors(ite);
				item->updateGradientVectors();
			}
		}
		else
		{
			QList<VColorStop*> cstops = oostyle.gradient.colorStops();
			item->setFillColor(cstops.at(0)->name);
			item->setFillShade(cstops.at(0)->shade);
		}
	}
	return item;
}

void OODPlug::createStyleMap( QDomDocument &docstyles)
{
	QDomElement styles = docstyles.documentElement();
	if (styles.isNull())
		return;

	QDomNode fixedStyles = styles.namedItem( "office:styles" );
	if (!fixedStyles.isNull())
	{
		insertDraws( fixedStyles.toElement() );
		insertStyles( fixedStyles.toElement() );
	}
	QDomNode automaticStyles = styles.namedItem( "office:automatic-styles" );
	if (!automaticStyles.isNull())
		insertStyles( automaticStyles.toElement() );

	QDomNode masterStyles = styles.namedItem( "office:master-styles" );
	if (!masterStyles.isNull())
		insertStyles( masterStyles.toElement() );
}

void OODPlug::insertDraws(const QDomElement& styles)
{
	for (QDomNode n = styles.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		QDomElement e = n.toElement();
		if (!e.hasAttribute( "draw:name" ))
			continue;
		QString name = e.attribute( "draw:name" );
		m_draws.insert(name, new QDomElement(e) );
	}
}

void OODPlug::insertStyles( const QDomElement& styles )
{
	for (QDomNode n = styles.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		QDomElement e = n.toElement();
		if (!e.hasAttribute( "style:name" ))
			continue;
		QString name = e.attribute( "style:name" );
		m_styles.insert(name, new QDomElement(e));
	}
}

void OODPlug::fillStyleStack( const QDomElement& object )
{
	if (object.hasAttribute( "presentation:style-name" ))
		addStyles( m_styles.value(object.attribute( "presentation:style-name" ), nullptr) );
	if (object.hasAttribute( "draw:style-name" ))
		addStyles( m_styles.value(object.attribute( "draw:style-name" ), nullptr) );
	if (object.hasAttribute( "draw:text-style-name" ))
		addStyles( m_styles.value(object.attribute( "draw:text-style-name" ), nullptr) );
	if (object.hasAttribute( "text:style-name" ))
		addStyles( m_styles.value(object.attribute( "text:style-name" ), nullptr) );
}

void OODPlug::addStyles(const QDomElement* style)
{
	if (style)
	{
		if (style->hasAttribute( "style:parent-style-name" ))
			addStyles(m_styles.value(style->attribute( "style:parent-style-name" ), nullptr));
		m_styleStack.push(*style);
	}
}

void OODPlug::storeObjectStyles(const QDomElement& object)
{
	fillStyleStack(object);
}

double OODPlug::parseUnit(const QString &unit)
{
	QString unitval=unit;
	if (unit.isEmpty())
		return 0.0;
	if (unit.right( 2 ) == "pt")
		unitval.replace( "pt", "" );
	else if (unit.right( 2 ) == "cm")
		unitval.replace( "cm", "" );
	else if (unit.right( 2 ) == "mm")
		unitval.replace( "mm" , "" );
	else if (unit.right( 2 ) == "in")
		unitval.replace( "in", "" );
	else if (unit.right( 2 ) == "px")
		unitval.replace( "px", "" );
	double value = ScCLocale::toDoubleC(unitval);
	if (unit.right( 2 ) == "pt")
		{}/* value = value; */ // no change
	else if (unit.right( 2 ) == "cm")
		value = ( value / 2.54 ) * 72;
	else if (unit.right( 2 ) == "mm")
		value = ( value / 25.4 ) * 72;
	else if (unit.right( 2 ) == "in")
		value = value * 72;
	else if (unit.right( 2 ) == "px")
		{}/* value = value; */ // no change
	return value;
}

QString OODPlug::parseColor( const QString &s )
{
	QColor c;
	QString ret = CommonStrings::None;
	if (s.startsWith( "rgb(" ))
	{
		QString parse = s.trimmed();
		QStringList colors = parse.split( ',', QString::SkipEmptyParts );
		QString r = colors[0].right( ( colors[0].length() - 4 ) );
		QString g = colors[1];
		QString b = colors[2].left( ( colors[2].length() - 1 ) );
		if (r.contains( "%" ))
		{
			r.chop(1);
			r = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(r) ) / 100.0 ) ) );
		}
		if (g.contains( "%" ))
		{
			g.chop(1);
			g = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(g) ) / 100.0 ) ) );
		}
		if (b.contains( "%" ) )
		{
			b.chop(1);
			b = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(b) ) / 100.0 ) ) );
		}
		c = QColor(r.toInt(), g.toInt(), b.toInt());
	}
	else
		c.setNamedColor(s.trimmed());

	ScColor tmp;
	tmp.fromQColor(c);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString fNam = m_Doc->PageColors.tryAddColor("FromOODraw"+c.name(), tmp);
	if (fNam == "FromOODraw"+c.name())
		importedColors.append(fNam);
	ret = fNam;
	return ret;
}

void OODPlug::parseTransform(FPointArray *composite, const QString &transform)
{
	double dx, dy;
	QTransform result;
	QStringList subtransforms = transform.split(')', QString::SkipEmptyParts);
	QStringList::ConstIterator it = subtransforms.begin();
	QStringList::ConstIterator end = subtransforms.end();
	for (; it != end; ++it)
	{
		QStringList subtransform = (*it).split('(', QString::SkipEmptyParts);
		subtransform[0] = subtransform[0].trimmed().toLower();
		subtransform[1] = subtransform[1].simplified();
		QRegExp reg("[,( ]");
		QStringList params = subtransform[1].split(reg, QString::SkipEmptyParts);
		if (subtransform[0].startsWith(";") || subtransform[0].startsWith(","))
			subtransform[0] = subtransform[0].right(subtransform[0].length() - 1);
		if (subtransform[0] == "rotate")
		{
			result = QTransform();
			result.rotate(-parseUnit(params[0]) * 180 / M_PI);
			composite->map(result);
		}
		else if (subtransform[0] == "translate")
		{
			if (params.count() == 2)
			{
				dx = parseUnit(params[0]);
				dy = parseUnit(params[1]);
			}
			else
			{
				dx = parseUnit(params[0]);
				dy =0.0;
			}
			result = QTransform();
			result.translate(dx, dy);
			composite->map(result);
		}
		else if (subtransform[0] == "skewx")
		{
			result = QTransform();
			result.shear(-tan(ScCLocale::toDoubleC(params[0])), 0.0);
			composite->map(result);
		}
		else if (subtransform[0] == "skewy")
		{
			result = QTransform();
			result.shear(0.0, -tan(ScCLocale::toDoubleC(params[0])));
			composite->map(result);
		}
	}
}

void OODPlug::parseViewBox(const QDomElement& object, double *x, double *y, double *w, double *h)
{
	if (!object.attribute( "svg:viewBox" ).isEmpty())
	{
		QString viewbox( object.attribute( "svg:viewBox" ) );
		QStringList points = viewbox.replace( QRegExp(","), " ").simplified().split(' ', QString::SkipEmptyParts);
		*x = ScCLocale::toDoubleC(points[0]);
		*y = ScCLocale::toDoubleC(points[1]);
		*w = ScCLocale::toDoubleC(points[2]);
		*h = ScCLocale::toDoubleC(points[3]);
	}
}

void OODPlug::appendPoints(FPointArray *composite, const QDomElement& object, bool closePath)
{
	double x = parseUnit(object.attribute("svg:x"));
	double y = parseUnit(object.attribute("svg:y")) ;
	double w = parseUnit(object.attribute("svg:width"));
	double h = parseUnit(object.attribute("svg:height"));
	double vx = 0;
	double vy = 0;
	double vw = 1;
	double vh = 1;
	parseViewBox(object, &vx, &vy, &vw, &vh);
	double sx = (vw != 0.0) ? (w / vw) : w;
	double sy = (vh != 0.0) ? (h / vh) : h;
	QStringList ptList = object.attribute( "draw:points" ).split( ' ', QString::SkipEmptyParts );
	FPoint point, firstP;
	bool bFirst = true;
	for (QStringList::Iterator it = ptList.begin(); it != ptList.end(); ++it)
	{
		point = FPoint(ScCLocale::toDoubleC((*it).section( ',', 0, 0 )), ScCLocale::toDoubleC((*it).section( ',', 1, 1 )));
		if (bFirst)
		{
			composite->addPoint(point);
			composite->addPoint(point);
			firstP = point;
			bFirst = false;
		}
		else
		{
			composite->addPoint(point);
			composite->addPoint(point);
			composite->addPoint(point);
			composite->addPoint(point);
		}
	}
	if (closePath)
	{
		composite->addPoint(firstP);
		composite->addPoint(firstP);
	}
	QTransform mat;
	mat.translate(x, y);
	mat.scale(sx, sy);
	composite->map(mat);
}

const char * OODPlug::getCoord( const char *ptr, double &number )
{
	int integer, exponent;
	double decimal, frac;
	int sign, expsign;

	exponent = 0;
	integer = 0;
	frac = 1.0;
	decimal = 0;
	sign = 1;
	expsign = 1;

	// read the sign
	if (*ptr == '+')
		ptr++;
	else if (*ptr == '-')
	{
		ptr++;
		sign = -1;
	}

	// read the integer part
	while (*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
		integer = (integer * 10) + *(ptr++) - '0';
	if (*ptr == '.') // read the decimals
	{
		ptr++;
		while (*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
			decimal += (*(ptr++) - '0') * (frac *= 0.1);
	}

	if (*ptr == 'e' || *ptr == 'E') // read the exponent part
	{
		ptr++;

		// read the sign of the exponent
		if (*ptr == '+')
			ptr++;
		else if (*ptr == '-')
		{
			ptr++;
			expsign = -1;
		}

		exponent = 0;
		while (*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
		{
			exponent *= 10;
			exponent += *ptr - '0';
			ptr++;
		}
	}
	number = integer + decimal;
	number *= sign * pow( static_cast<double>(10), static_cast<double>( expsign * exponent ) );

	// skip the following space
	if (*ptr == ' ')
		ptr++;

	return ptr;
}

bool OODPlug::parseSVG(const QString &s, FPointArray *ite)
{
	QString d = s;
	d = d.replace( QRegExp( "," ), " ");
	bool ret = false;
	if (!d.isEmpty())
	{
		d = d.simplified();
		QByteArray data = d.toLatin1();
		const char *ptr = data.constData();
		const char *end = data.constData() + data.length() + 1;
		double contrlx, contrly, curx, cury, tox, toy, x1, y1, x2, y2, xc, yc;
		double px1, py1, px2, py2, px3, py3;
		bool relative;
		FirstM = true;
		char command = *(ptr++), lastCommand = ' ';
		curx = cury = contrlx = contrly = 0.0;
		while (ptr < end)
		{
			if (*ptr == ' ')
				ptr++;
			relative = false;
			switch (command)
			{
			case 'm':
				relative = true;
			case 'M':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					WasM = true;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					svgMoveTo(curx, cury );
					break;
				}
			case 'l':
				relative = true;
			case 'L':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'h':
				{
					ptr = getCoord( ptr, tox );
					curx = curx + tox;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'H':
				{
					ptr = getCoord( ptr, tox );
					curx = tox;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'v':
				{
					ptr = getCoord( ptr, toy );
					cury = cury + toy;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'V':
				{
					ptr = getCoord( ptr, toy );
					cury = toy;
					svgLineTo(ite,  curx, cury );
					break;
				}
			case 'z':
			case 'Z':
				{
					svgClosePath(ite);
					break;
				}
			case 'c':
				relative = true;
			case 'C':
				{
					ptr = getCoord( ptr, x1 );
					ptr = getCoord( ptr, y1 );
					ptr = getCoord( ptr, x2 );
					ptr = getCoord( ptr, y2 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					px1 = relative ? curx + x1 : x1;
					py1 = relative ? cury + y1 : y1;
					px2 = relative ? curx + x2 : x2;
					py2 = relative ? cury + y2 : y2;
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = relative ? curx + x2 : x2;
					contrly = relative ? cury + y2 : y2;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 's':
				relative = true;
			case 'S':
				{
					ptr = getCoord( ptr, x2 );
					ptr = getCoord( ptr, y2 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					px1 = 2 * curx - contrlx;
					py1 = 2 * cury - contrly;
					px2 = relative ? curx + x2 : x2;
					py2 = relative ? cury + y2 : y2;
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = relative ? curx + x2 : x2;
					contrly = relative ? cury + y2 : y2;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 'q':
				relative = true;
			case 'Q':
				{
					ptr = getCoord( ptr, x1 );
					ptr = getCoord( ptr, y1 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					px1 = relative ? (curx + 2 * (x1 + curx)) * (1.0 / 3.0) : (curx + 2 * x1) * (1.0 / 3.0);
					py1 = relative ? (cury + 2 * (y1 + cury)) * (1.0 / 3.0) : (cury + 2 * y1) * (1.0 / 3.0);
					px2 = relative ? ((curx + tox) + 2 * (x1 + curx)) * (1.0 / 3.0) : (tox + 2 * x1) * (1.0 / 3.0);
					py2 = relative ? ((cury + toy) + 2 * (y1 + cury)) * (1.0 / 3.0) : (toy + 2 * y1) * (1.0 / 3.0);
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = relative ? curx + x1 : (tox + 2 * x1) * (1.0 / 3.0);
					contrly = relative ? cury + y1 : (toy + 2 * y1) * (1.0 / 3.0);
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 't':
				relative = true;
			case 'T':
				{
					ptr = getCoord(ptr, tox);
					ptr = getCoord(ptr, toy);
					xc = 2 * curx - contrlx;
					yc = 2 * cury - contrly;
					px1 = relative ? (curx + 2 * xc) * (1.0 / 3.0) : (curx + 2 * xc) * (1.0 / 3.0);
					py1 = relative ? (cury + 2 * yc) * (1.0 / 3.0) : (cury + 2 * yc) * (1.0 / 3.0);
					px2 = relative ? ((curx + tox) + 2 * xc) * (1.0 / 3.0) : (tox + 2 * xc) * (1.0 / 3.0);
					py2 = relative ? ((cury + toy) + 2 * yc) * (1.0 / 3.0) : (toy + 2 * yc) * (1.0 / 3.0);
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = xc;
					contrly = yc;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 'a':
				relative = true;
			case 'A':
				{
					bool largeArc, sweep;
					double angle, rx, ry;
					ptr = getCoord( ptr, rx );
					ptr = getCoord( ptr, ry );
					ptr = getCoord( ptr, angle );
					ptr = getCoord( ptr, tox );
					largeArc = tox == 1;
					ptr = getCoord( ptr, tox );
					sweep = tox == 1;
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					calculateArc(ite, relative, curx, cury, angle, tox, toy, rx, ry, largeArc, sweep );
				}
			}
			lastCommand = command;
			if (*ptr == '+' || *ptr == '-' || (*ptr >= '0' && *ptr <= '9'))
			{
				// there are still coords in this command
				if (command == 'M')
					command = 'L';
				else if (command == 'm')
					command = 'l';
			}
			else
				command = *(ptr++);

			if (lastCommand != 'C' && lastCommand != 'c' &&
			        lastCommand != 'S' && lastCommand != 's' &&
			        lastCommand != 'Q' && lastCommand != 'q' &&
			        lastCommand != 'T' && lastCommand != 't')
			{
				contrlx = curx;
				contrly = cury;
			}
		}
		if ((lastCommand != 'z') && (lastCommand != 'Z'))
			ret = true;
		if (ite->size() > 2)
		{
			if ((ite->point(0).x() == ite->point(ite->size()-2).x()) && (ite->point(0).y() == ite->point(ite->size()-2).y()))
				ret = false;
		}
	}
	return ret;
}

void OODPlug::calculateArc(FPointArray *ite, bool relative, double &curx, double &cury, double angle, double x, double y, double r1, double r2, bool largeArcFlag, bool sweepFlag)
{
	double sin_th, cos_th;
	double a00, a01, a10, a11;
	double x0, y0, x1, y1, xc, yc;
	double d, sfactor, sfactor_sq;
	double th0, th1, th_arc;
	int i, n_segs;
	sin_th = sin(angle * (M_PI / 180.0));
	cos_th = cos(angle * (M_PI / 180.0));
	double dx;
	if (!relative)
		dx = (curx - x) / 2.0;
	else
		dx = -x / 2.0;
	double dy;
	if (!relative)
		dy = (cury - y) / 2.0;
	else
		dy = -y / 2.0;
	double _x1 =  cos_th * dx + sin_th * dy;
	double _y1 = -sin_th * dx + cos_th * dy;
	double Pr1 = r1 * r1;
	double Pr2 = r2 * r2;
	double Px = _x1 * _x1;
	double Py = _y1 * _y1;
	double check = Px / Pr1 + Py / Pr2;
	if (check > 1)
	{
		r1 = r1 * sqrt(check);
		r2 = r2 * sqrt(check);
	}
	a00 = cos_th / r1;
	a01 = sin_th / r1;
	a10 = -sin_th / r2;
	a11 = cos_th / r2;
	x0 = a00 * curx + a01 * cury;
	y0 = a10 * curx + a11 * cury;
	if (!relative)
		x1 = a00 * x + a01 * y;
	else
		x1 = a00 * (curx + x) + a01 * (cury + y);
	if (!relative)
		y1 = a10 * x + a11 * y;
	else
		y1 = a10 * (curx + x) + a11 * (cury + y);
	d = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
	sfactor_sq = 1.0 / d - 0.25;
	if (sfactor_sq < 0)
		sfactor_sq = 0;
	sfactor = sqrt(sfactor_sq);
	if (sweepFlag == largeArcFlag)
		sfactor = -sfactor;
	xc = 0.5 * (x0 + x1) - sfactor * (y1 - y0);
	yc = 0.5 * (y0 + y1) + sfactor * (x1 - x0);

	th0 = atan2(y0 - yc, x0 - xc);
	th1 = atan2(y1 - yc, x1 - xc);
	th_arc = th1 - th0;
	if (th_arc < 0 && sweepFlag)
		th_arc += 2 * M_PI;
	else if (th_arc > 0 && !sweepFlag)
		th_arc -= 2 * M_PI;
	n_segs = static_cast<int>(ceil(fabs(th_arc / (M_PI * 0.5 + 0.001))));
	for (i = 0; i < n_segs; i++)
	{
		{
			double sin_th, cos_th;
			double a00, a01, a10, a11;
			double x1, y1, x2, y2, x3, y3;
			double t;
			double th_half;
			double _th0 = th0 + i * th_arc / n_segs;
			double _th1 = th0 + (i + 1) * th_arc / n_segs;
			sin_th = sin(angle * (M_PI / 180.0));
			cos_th = cos(angle * (M_PI / 180.0));
			a00 = cos_th * r1;
			a01 = -sin_th * r2;
			a10 = sin_th * r1;
			a11 = cos_th * r2;
			th_half = 0.5 * (_th1 - _th0);
			t = (8.0 / 3.0) * sin(th_half * 0.5) * sin(th_half * 0.5) / sin(th_half);
			x1 = xc + cos(_th0) - t * sin(_th0);
			y1 = yc + sin(_th0) + t * cos(_th0);
			x3 = xc + cos(_th1);
			y3 = yc + sin(_th1);
			x2 = x3 + t * sin(_th1);
			y2 = y3 - t * cos(_th1);
			svgCurveToCubic(ite, a00 * x1 + a01 * y1, a10 * x1 + a11 * y1, a00 * x2 + a01 * y2, a10 * x2 + a11 * y2, a00 * x3 + a01 * y3, a10 * x3 + a11 * y3 );
		}
	}
	if (!relative)
		curx = x;
	else
		curx += x;
	if (!relative)
		cury = y;
	else
		cury += y;
}

void OODPlug::svgMoveTo(double x1, double y1)
{
	CurrX = x1;
	CurrY = y1;
	StartX = x1;
	StartY = y1;
	PathLen = 0;
}

void OODPlug::svgLineTo(FPointArray *i, double x1, double y1)
{
	if ((!FirstM) && (WasM))
	{
		i->setMarker();
		PathLen += 4;
	}
	FirstM = false;
	WasM = false;
	if (i->size() > 3)
	{
		const FPoint& b1 = i->point(i->size()-4);
		const FPoint& b2 = i->point(i->size()-3);
		const FPoint& b3 = i->point(i->size()-2);
		const FPoint& b4 = i->point(i->size()-1);
		FPoint n1 = FPoint(CurrX, CurrY);
		FPoint n2 = FPoint(x1, y1);
		if ((b1 == n1) && (b2 == n1) && (b3 == n2) && (b4 == n2))
			return;
	}
	i->addPoint(FPoint(CurrX, CurrY));
	i->addPoint(FPoint(CurrX, CurrY));
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x1, y1));
	CurrX = x1;
	CurrY = y1;
	PathLen += 4;
}

void OODPlug::svgCurveToCubic(FPointArray *i, double x1, double y1, double x2, double y2, double x3, double y3)
{
	if ((!FirstM) && (WasM))
	{
		i->setMarker();
		PathLen += 4;
	}
	FirstM = false;
	WasM = false;
	if (PathLen > 3)
	{
		const FPoint& b1 = i->point(i->size()-4);
		const FPoint& b2 = i->point(i->size()-3);
		const FPoint& b3 = i->point(i->size()-2);
		const FPoint& b4 = i->point(i->size()-1);
		FPoint n1 = FPoint(CurrX, CurrY);
		FPoint n2 = FPoint(x1, y1);
		FPoint n3 = FPoint(x3, y3);
		FPoint n4 = FPoint(x2, y2);
		if ((b1 == n1) && (b2 == n2) && (b3 == n3) && (b4 == n4))
			return;
	}
	i->addPoint(FPoint(CurrX, CurrY));
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x3, y3));
	i->addPoint(FPoint(x2, y2));
	CurrX = x3;
	CurrY = y3;
	PathLen += 4;
}

void OODPlug::svgClosePath(FPointArray *i)
{
	if (PathLen > 2)
	{
		if ((PathLen == 4) || (i->point(i->size()-2).x() != StartX) || (i->point(i->size()-2).y() != StartY))
		{
			i->addPoint(i->point(i->size()-2));
			i->addPoint(i->point(i->size()-3));
			i->addPoint(FPoint(StartX, StartY));
			i->addPoint(FPoint(StartX, StartY));
		}
	}
}

OODPlug::~OODPlug()
{
	delete tmpSel;
	// it's probably needed as QHash() dos not support autocleaning
	m_styles.clear();
	m_draws.clear();
}



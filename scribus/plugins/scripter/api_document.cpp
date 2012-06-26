/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QImageWriter>

#include "api_document.h"
#include "selection.h"
#include "api_color.h"
#include "api_layer.h"
#include "api_imageexport.h"
#include "api_printer.h"
#include "api_textitem.h"
#include "api_imageitem.h"
#include "util.h"
#include "utils.h"
#include "pageitem_textframe.h"
#include "undomanager.h"

DocumentAPI::DocumentAPI() : QObject(COLLECTOR)
{
	qDebug() << "DocumentAPI loaded";
	setObjectName("document"); // XXX: support other documents later
}



DocumentAPI::~DocumentAPI()
{
	qDebug() << "DocumentAPI deleted";
}

/**
 * Scripter.activeDocument.name
 */
QString DocumentAPI::getName()
{
	if (!check()) return NULL;
	if (! ScCore->primaryMainWindow()->doc->hasName)
	{
		return QString("");
	}
	return ScCore->primaryMainWindow()->doc->DocName;
}


/**
 * Scripter.activeDocument.available
 * Property
 * bool value if a document is active
 */
bool DocumentAPI::available()
{
	if (ScCore->primaryMainWindow()->HaveDoc)
		return true;
	return false;
}


/**
 * Scripter.activeDocument.check
 * returns True if found else False and raises error
 */
bool DocumentAPI::check()
{
	if (!available())
	{
		RAISE("Method call requires an open document, none found.");
		return false;
	}
	return true;
}


/**
 * Scripter.activeDocument.modified
 * Property
 * boolean flag if document is modified
 *
 * If a document is modified you will be asked to save it
 * on close or quit.
 */
void DocumentAPI::setModified(bool flag)
{
	if (!check()) return;
	ScCore->primaryMainWindow()->doc->setModified(flag);
}

bool DocumentAPI::modified()
{
	if (!check()) return NULL;
	return ScCore->primaryMainWindow()->doc->isModified();
}



/**
 * Scripter.activeDocument.close()
 *
 * closes the active document without asking save
 */
bool DocumentAPI::close()
{
	if (!check()) return NULL;
	setModified(false);
	bool ret = ScCore->primaryMainWindow()->slotFileClose();
	qApp->processEvents();
	return ret;
}

/**
 * Scripter.activeDocument.save()
 *
 * Saves the activeDocument
 */
bool DocumentAPI::save()
{
	if (!check()) return NULL;
	bool ret = ScCore->primaryMainWindow()->slotFileSave();
	qApp->processEvents();
	return ret;
}

/**
 * Scripter.activeDocument.saveAs(name)
 *
 * saves a document
 */
bool DocumentAPI::saveAs(QString name)
{
	if (!check()) return NULL;
	bool ret = ScCore->primaryMainWindow()->DoFileSave(name);
	if (!ret)
	{
		RAISE("Failed to save document.");
		return NULL;
	}
	return true;
}


/**
 * Scripter.activeDocument.pageCount()
 * returns number of pages in active document as int
 */
int DocumentAPI::pageCount()
{
	return ScCore->primaryMainWindow()->doc->Pages->count();
}



/**
 * Scripter.activeDocument.activePage
 * Property
 * Page object of active page in active Document
 */
QObject *DocumentAPI::activePage()
{
	return new PageAPI(this);
}


/**
 * Scripter.activeDocument.activeItem
 * Property
 * Currently active page item
 */
QObject *DocumentAPI::activeItem()
{
	if (ScCore->primaryMainWindow()->doc->m_Selection->count() == 0)
		return NULL;
	else
	{
		PageItem* item = ScCore->primaryMainWindow()->doc->m_Selection->itemAt(0);
		if(item->asTextFrame())
		{
			TextAPI *textItem = new TextAPI(item->asTextFrame());
			return textItem;
		}
		else if(item->asImageFrame())
		{
			ImageAPI *imageItem = new ImageAPI(item->asImageFrame());
			return imageItem;
		}
		else
		{
			ItemAPI *otherItem = new ItemAPI(item);
			return otherItem;
		}
	}
}

int DocumentAPI::selectionCount()
{
	if(!check())
		RAISE("No document open.");
	return ScCore->primaryMainWindow()->doc->m_Selection->count();
}


/**
 * Scripter.activeDocument.selection
 * Property
 * List of selected Item objects on active document
 */
QList<QVariant> DocumentAPI::selection()
{
	QList<QVariant> l;
	Selection *sel = ScCore->primaryMainWindow()->doc->m_Selection;
	for (int i=0; i < sel->count(); i++)
	{
		/**
		 * Checking whether it is a textframe. If yes, we are trying to cast 
		 * it onto TextWrapper class, which can effectively perform all 
		 * the text operations
		 */
		PageItem *item = sel->itemAt(i);
		if (item->asTextFrame())
		{
			TextAPI *textItem = new TextAPI(item->asTextFrame());
			l.append(qVariantFromValue((QObject *)(textItem)));
		}
		else if(item->asImageFrame())
		{
			ImageAPI *imageItem = new ImageAPI(item->asImageFrame());
			l.append(qVariantFromValue((QObject *)(imageItem)));
		}
		else
		{
			ItemAPI *otherItem = new ItemAPI(item);
			l.append(qVariantFromValue(
			             (QObject *)(otherItem)
			         ));
		}
	}
	return l;
}


/**
 * Scripter.activeDocument.dimensions
 * Property
 * Dimensions object of active page
 */
QObject *DocumentAPI::dimensions()
{
	return new Dimensions(this);
}

/**
 * Scripter.activeDocument.items
 * Property
 * List of all Item objects of active document
 */
QList<QVariant> DocumentAPI::items()
{
	QList<QVariant> l;
	for (int i = 0; i<ScCore->primaryMainWindow()->doc->Items->count(); ++i)
	{
		PageItem *item = ScCore->primaryMainWindow()->doc->Items->at(i);
		if (item->asTextFrame())
		{
			TextAPI *textItem = new TextAPI(item->asTextFrame());
			l.append(qVariantFromValue((QObject *)(textItem)));
		}
		else if(item->asImageFrame())
		{
			ImageAPI *imageItem = new ImageAPI(item->asImageFrame());
			l.append(qVariantFromValue((QObject *)(imageItem)));
		}
		else
		{
			ItemAPI *otherItem = new ItemAPI(item);
			l.append(qVariantFromValue(
			             (QObject *)(otherItem)
			         ));
		}
	}
	return l;
}
//FIXME is there a need to make these informations separate properties?
//jainbasil


void DocumentAPI::setInformation(QString author, QString title, QString desc)
{
	if (!check())
		return;
	ScCore->primaryMainWindow()->doc->documentInfo().setAuthor(author);
	ScCore->primaryMainWindow()->doc->documentInfo().setTitle(title);
	ScCore->primaryMainWindow()->doc->documentInfo().setComments(desc);
	ScCore->primaryMainWindow()->slotDocCh();
}


QList<QVariant> DocumentAPI::layers()
{
	QList<QVariant> l;
	for (int i = 0; i<ScCore->primaryMainWindow()->doc->Layers.count() ; ++i)
	{
		ScLayer *L = &(ScCore->primaryMainWindow()->doc->Layers[i]);
		LayerAPI *layer = new LayerAPI(L);
		l.append(qVariantFromValue((QObject *)(layer)));
	}
	return l;
}

QObject *DocumentAPI::newLayer(QString name)
{
	LayerAPI *l = new LayerAPI(ScCore->primaryMainWindow()->doc->Layers.newLayer(name));
	return (QObject *)(l);
}

void DocumentAPI::removeLayer(QString name)
{
	if (ScCore->primaryMainWindow()->doc->Layers.count() == 1)
	{
		RAISE("Cannot remove the last layer.");
	}
	bool found = false;
	for (int lam=0; lam < ScCore->primaryMainWindow()->doc->Layers.count(); ++lam)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[lam].Name == name)
		{
			ScLayer it2 = ScCore->primaryMainWindow()->doc->Layers.at(lam);
			int num2 = it2.ID;
			if (!num2)
			{
				return;
			}
			ScCore->primaryMainWindow()->doc->removeLayer(num2);
			ScCore->primaryMainWindow()->doc->Layers.removeLayerByID(num2);
			ScCore->primaryMainWindow()->doc->setActiveLayer(0);
			ScCore->primaryMainWindow()->changeLayer(0);
			found = true;
			break;
		}
	}
	if (!found)
	{
		RAISE("Layer not found.");
	}
}

QObject *DocumentAPI::getActiveLayer()
{
	for (int i = 0; i<ScCore->primaryMainWindow()->doc->Layers.count(); ++i)
	{
		if (ScCore->primaryMainWindow()->doc->Layers[i].Name == ScCore->primaryMainWindow()->doc->activeLayerName())
		{
			ScLayer *L = &(ScCore->primaryMainWindow()->doc->Layers[i]);
			return new LayerAPI(L);
		}
	}
	return NULL;
}

QString DocumentAPI::getActiveLayerName()
{
	return ScCore->primaryMainWindow()->doc->activeLayerName();
}

void DocumentAPI::setActiveLayer(QString name)
{
	bool found = ScCore->primaryMainWindow()->doc->setActiveLayer(name);
	if (found)
		ScCore->primaryMainWindow()->changeLayer(ScCore->primaryMainWindow()->doc->activeLayer());
	else
		RAISE("Layer not found.");
}

/**
 * Scripter.activeDocument.colors
 * Property
 * Colors of activeDocument
 */
QList<QVariant> DocumentAPI::colors()
{
	QList<QVariant> l;

	ColorList names = ScCore->primaryMainWindow()->doc->PageColors;
	ColorList::Iterator it;
	for (it = names.begin(); it != names.end(); ++it)
	{
		ScColor *value = &(names[it.key()]);
		ColorAPI *color = new ColorAPI(value, it.key());
		l.append(qVariantFromValue((QObject *)(color)));
	}
	return l;
}

QObject *DocumentAPI::getColor(QString name)
{
	return new ColorAPI(&(ScCore->primaryMainWindow()->doc->PageColors[name]), name);
}

QObject *DocumentAPI::newColorCMYK(QString name, int c, int m, int y, int k)
{
	if (name.isEmpty())
	{
		RAISE("Cannot create a color with an empty name");
	}
	if (ScCore->primaryMainWindow()->doc->PageColors.contains(name))
	{
		ScCore->primaryMainWindow()->doc->PageColors.insert(name, ScColor(c, m, y, k));
	}
	else
	{
		ScCore->primaryMainWindow()->doc->PageColors[name].setColor(c, m, y, k);
	}
	return getColor(name);
}

QObject *DocumentAPI::newColorRGB(QString name, int r, int g, int b)
{
	if (name.isEmpty())
	{
		RAISE("Cannot create a color with an empty name");
	}
	if (ScCore->primaryMainWindow()->doc->PageColors.contains(name))
	{
		ScCore->primaryMainWindow()->doc->PageColors.insert(name, ScColor(r, g, b));
	}
	else
	{
		ScCore->primaryMainWindow()->doc->PageColors[name].setColorRGB(r, g, b);
	}
	return getColor(name);
}

QList<QVariant> DocumentAPI::supportedImageTypes()
{
	QList<QVariant> l;
	QList<QByteArray> list = QImageWriter::supportedImageFormats();
	for (QList<QByteArray>::Iterator it = list.begin(); it != list.end(); ++it)
	{
		l.append(QString((*it)).toLatin1().constData());
	}
	return l;
}

bool DocumentAPI::exportAsImages(QString dirName, QString type, double scale, double quality, double dpi, bool overwrite)
{
	std::vector<int> pageNs;
	ImageExport *ie = new ImageExport(dirName, type, scale, quality, dpi, overwrite);
	parsePagesString("*", &pageNs, ScCore->primaryMainWindow()->doc->DocPages.count());
	bool result = ie->exportInterval(ScCore->primaryMainWindow()->doc, pageNs);
	return result;
}

QObject* DocumentAPI::Printer()
{
	return new PrinterAPI();
}

QList<QVariant> DocumentAPI::masterPages()
{
	QList<QVariant> names;
	if (!check())
		return names;
	QMap<QString,int>::const_iterator it(ScCore->primaryMainWindow()->doc->MasterNames.constBegin());
	QMap<QString,int>::const_iterator itEnd(ScCore->primaryMainWindow()->doc->MasterNames.constEnd());
	for (; it != itEnd; ++it)
	{
		names.append(it.key());
	}
	return names;
}

/**
 * Scripter.activeDocument.createMasterPage(name)
 */
void DocumentAPI::createMasterPage(QString name)
{
	if (!check()) return;
	if (ScCore->primaryMainWindow()->doc->MasterNames.contains(name))
	{
		RAISE("Master page already exists");
		return;
	}
	ScCore->primaryMainWindow()->doc->addMasterPage(ScCore->primaryMainWindow()->doc->MasterPages.count(), name);
}

/**
 * Scripter.activeDocument.deleteMasterPage(name)
 */
void DocumentAPI::deleteMasterPage(QString name)
{
	if (!check()) return;

	if (!ScCore->primaryMainWindow()->doc->MasterNames.contains(name))
	{
		RAISE("Master page does not exist");
		return;
	}
	if (name == "Normal")
	{
		RAISE("Can not delete the Normal master page");
		return;
	}
	bool oldMode = ScCore->primaryMainWindow()->doc->masterPageMode();
	ScCore->primaryMainWindow()->doc->setMasterPageMode(true);
	ScCore->primaryMainWindow()->deletePage2(ScCore->primaryMainWindow()->doc->MasterNames[name]);
	ScCore->primaryMainWindow()->doc->setMasterPageMode(oldMode);
}

void DocumentAPI::closeMasterPage(QString name)
{
	if (!check())
		return;
	ScCore->primaryMainWindow()->view->hideMasterPage();
}

void DocumentAPI::editMasterPage(QString name)
{
	if (!check())
		return;
	const QMap<QString,int>& masterNames(ScCore->primaryMainWindow()->doc->MasterNames);
	const QMap<QString,int>::const_iterator it(masterNames.find(name));
	if (it == masterNames.constEnd())
	{
		RAISE("Master page not found");
		return;
	}
	ScCore->primaryMainWindow()->view->showMasterPage(*it);
}

void DocumentAPI::loadStylesFromFile(QString name)
{
	if (!check())
		return;
	ScCore->primaryMainWindow()->doc->loadStylesFromFile(name);
}

QObject* DocumentAPI::selectItem(QString name)
{
	if (!check())
		RAISE("No document open");
	PageItem *i = GetUniqueItem(name);
	if (i == NULL)
	{
		RAISE("No item with this name");
	}
	ScCore->primaryMainWindow()->view->SelectItem(i);
	return activeItem();
}

void DocumentAPI::deleteItem(QString name)
{
	if (name.isEmpty())
	{
		RAISE("name cannot be empty.");
		return;
	}
	if (!check())
		return;
	PageItem *i = GetUniqueItem(name);
	if (i == NULL)
	{
		RAISE("Unable to find the item.");
		return;
	}
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(i);
	ScCore->primaryMainWindow()->doc->itemSelection_DeleteItem();
}

bool DocumentAPI::itemExists(QString name)
{
	if (name.isEmpty())
	{
		RAISE("Name cannot be empty");
		return false;
	}
	if (!check())
	{
		RAISE("No active document");
	}
	if (ItemExists(name))
		return true;
	return false;
}

QList< QVariant > DocumentAPI::styles()
{
	QList<QVariant> styleList;
	if (!check())
		RAISE("No active document.");
	for (int i=0; i < ScCore->primaryMainWindow()->doc->paragraphStyles().count(); ++i)
	{
		styleList.append(ScCore->primaryMainWindow()->doc->paragraphStyles()[i].name());
	}
	return styleList;
}

void DocumentAPI::deselectItems()
{
	if (!check())
		return;
	ScCore->primaryMainWindow()->view->Deselect();
}

QString DocumentAPI::groupItems(QList< QVariant > list)
{
	QString name;
	if (!check())
		RAISE("No document open");
	if (list.isEmpty() && ScCore->primaryMainWindow()->doc->m_Selection->count() < 2)
	{
		RAISE("Need selection or argument list of items to group");
	}
	Selection *tempSelection=0;
	Selection *finalSelection=0;
	//uint ap = ScCore->primaryMainWindow()->doc->currentPage()->pageNr();
	// If we were passed a list of items to group...
	if (!list.isEmpty())
	{
		int len = list.length();
		tempSelection = new Selection(ScCore->primaryMainWindow(), false);
		for (int i = 0; i < len; i++)
		{
			// FIXME: We might need to explicitly get this string as utf8
			// but as sysdefaultencoding is utf8 it should be a no-op to do
			// so anyway.
			name = list[i].toString();
			PageItem *ic = GetUniqueItem(name);
			if (ic == NULL)
			{
				delete tempSelection;
				return NULL;
			}
			tempSelection->addItem(ic, true);
		}
		finalSelection=tempSelection;
	}
	else
		finalSelection=ScCore->primaryMainWindow()->doc->m_Selection;
	if (finalSelection->count() < 2)
	{
		// We can't very well group only one item
		RAISE("Cannot group less than two items");
		finalSelection=0;
		delete tempSelection;
		return NULL;
	}

	const PageItem* group = ScCore->primaryMainWindow()->doc->itemSelection_GroupObjects(false, false, finalSelection);
	finalSelection=0;
	delete tempSelection;

	return (group ? group->itemName() : NULL);
}

void DocumentAPI::unGroupItems(QString name)
{
	if (!check())
		RAISE("No document open");
	PageItem *i = GetUniqueItem(name);
	if (i == NULL)
		RAISE("Item not found.");
	ScCore->primaryMainWindow()->view->Deselect();
	ScCore->primaryMainWindow()->view->SelectItem(i);
	ScCore->primaryMainWindow()->UnGroupObj();
}

void DocumentAPI::scaleGroup(double factor, QString name)
{
	if (!check())
		RAISE("No document open");
	if (factor == 0.0)
	{
		RAISE("Cannot scale by 0%.");
		return;
	}
	PageItem *i = GetUniqueItem(name);
	if (i == NULL)
		return;
	ScCore->primaryMainWindow()->view->Deselect();
	ScCore->primaryMainWindow()->view->SelectItem(i);
//	int h = ScCore->primaryMainWindow()->view->frameResizeHandle;
//	ScCore->primaryMainWindow()->view->frameResizeHandle = 1;
	ScCore->primaryMainWindow()->view->startGroupTransaction(Um::Resize, "", Um::IResize);
	ScCore->primaryMainWindow()->doc->scaleGroup(factor, factor);
	ScCore->primaryMainWindow()->view->endGroupTransaction();
}

Dimensions::Dimensions(QObject *parent) : QObject(COLLECTOR)
{
	qDebug() << "Dimensions created";
}

/**
 * Scripter.activeDocument.dimensions.width
 * Property
 * width of active document in points as double (read-only, for now)
 */
double Dimensions::width()
{
	return ScCore->primaryMainWindow()->doc->pageWidth();
}

/**
 * Scripter.activeDocument.dimensions.height
 * Property
 * height of active document in points as double (read-only, for now)
 */
double Dimensions::height()
{
	return ScCore->primaryMainWindow()->doc->pageHeight();
}

/**
 * Scripter.activeDocument.margins
 * Property
 * Margins object of active document
 */
QObject *DocumentAPI::margins()
{
	return new Margins(this);
}

Margins::Margins(QObject *parent) : QObject(COLLECTOR)
{
	qDebug() << "Margins created";
}

/**
 * Scripter.activeDocument.margins.top
 * Property
 * top-margin of active document as double
 */
double Margins::top()
{
	return ScCore->primaryMainWindow()->doc->margins()->Top;
}



/**
 * Scripter.activeDocument.margins.top
 * Property
 * left-margin of active document as double
 */
double Margins::left()
{
	return ScCore->primaryMainWindow()->doc->margins()->Left;
}



/**
 * Scripter.activeDocument.margins.top
 * Property
 * right-margin of active document as double
 */
double Margins::right()
{
	return ScCore->primaryMainWindow()->doc->margins()->Right;
}



/**
 * Scripter.activeDocument.margins.top
 * Property
 * top-margin of active document as double
 */
double Margins::bottom()
{
	return ScCore->primaryMainWindow()->doc->margins()->Bottom;
}


/**
 * Scripter.activeDocument.margins.set(lr, tpr, btr, rr)
 * args are double
 *
 * Use this method if you want to change more than one margin
 * at a time
 */

void Margins::set(double lr, double tpr, double btr, double rr)
{
	MarginStruct margins(tpr, lr, btr, rr);
	ScCore->primaryMainWindow()->doc->resetPage(ScCore->primaryMainWindow()->doc->pagePositioning(), &margins);
	ScCore->primaryMainWindow()->view->reformPages();
	ScCore->primaryMainWindow()->doc->setModified(true);
	ScCore->primaryMainWindow()->view->GotoPage(ScCore->primaryMainWindow()->doc->currentPageNumber());
	ScCore->primaryMainWindow()->view->DrawNew();
}



void Margins::setTop(double value)
{
	set(left(), value, bottom(), right());
}



void Margins::setLeft(double value)
{
	set(value, top(), bottom(), right());
}



void Margins::setRight(double value)
{
	set(left(), top(), bottom(), value);
}



void Margins::setBottom(double value)
{
	set(left(), top(), value, right());
}

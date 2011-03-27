/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribus134format.h"
#include "scribus134formatimpl.h"

#include "../../formatidlist.h"
#include "commonstrings.h"
#include "ui/missing.h"
#include "hyphenator.h"
#include "pageitem_latexframe.h"
#include "prefsmanager.h"
#include "scclocale.h"
#include "scconfig.h"
#include "sccolorengine.h"
#include "scpattern.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "sctextstream.h"
#include "scxmlstreamreader.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_math.h"
#include "util_color.h"
#include "util_layer.h"
#include "scgzfile.h"
#include <QCursor>
// #include <QDebug>
#include <QFileInfo>
#include <QList>
#include <QByteArray>
#include <QApplication>


// See scplugin.h and pluginmanager.{cpp,h} for detail on what these methods
// do. That documentatation is not duplicated here.
// Please don't implement the functionality of your plugin here; do that
// in scribus134formatimpl.h and scribus134formatimpl.cpp .

Scribus134Format::Scribus134Format() :
	LoadSavePlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file formats.
	languageChange();
}

Scribus134Format::~Scribus134Format()
{
	unregisterAll();
};

void Scribus134Format::languageChange()
{
	//(Re)register file formats.
	unregisterAll();
	registerFormats();
}

const QString Scribus134Format::fullTrName() const
{
	return QObject::tr("Scribus 1.3.4+ Support");
}

const ScActionPlugin::AboutData* Scribus134Format::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8(
			"Franz Schmid <franz@scribus.info>, "
			"The Scribus Team");
	about->shortDescription = tr("Scribus 1.3.4+ File Format Support");
	about->description = tr("Allows Scribus to read Scribus 1.3.4 and higher formatted files.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void Scribus134Format::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void Scribus134Format::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Scribus 1.3.4+ Document");
	fmt.formatId = FORMATID_SLA134IMPORT;
	fmt.load = true;
	fmt.save = false;
	fmt.colorReading = true;
	fmt.filter = fmt.trName + " (*.sla *.SLA *.sla.gz *.SLA.GZ *.scd *.SCD *.scd.gz *.SCD.GZ)";
	fmt.nameMatch = QRegExp("\\.(sla|scd)(\\.gz)?", Qt::CaseInsensitive);
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/x-scribus");
	fmt.fileExtensions = QStringList() << "sla" << "sla.gz" << "scd" << "scd.gz";
	fmt.priority = 64;
	registerFormat(fmt);
}

bool Scribus134Format::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	QByteArray docBytes("");
	if(fileName.right(2) == "gz")
	{
		if (!ScGzFile::readFromFile(fileName, docBytes, 4096))
		{
			// FIXME: Needs better error return
			return false;
		}
	}
	else
	{
		// Not gzip encoded, just load it
		loadRawText(fileName, docBytes);
	}
//	if (docBytes.left(16) == "<SCRIBUSUTF8NEW " && docBytes.left(35).contains("Version=\"1.3.4"))
//		return true;
	QRegExp regExp134("Version=\"1.3.[4-9]");
	QRegExp regExp140("Version=\"1.4.[0-9]");
	int startElemPos = docBytes.left(512).indexOf("<SCRIBUSUTF8NEW ");
	if (startElemPos >= 0)
	{
		bool is134 = ( regExp134.indexIn(docBytes.mid(startElemPos, 64)) >= 0 );
		bool is140 = ( regExp140.indexIn(docBytes.mid(startElemPos, 64)) >= 0 );
		return (is134 || is140);
	}
	return false;
}

QString Scribus134Format::readSLA(const QString & fileName)
{
	QByteArray docBytes("");
	if(fileName.right(2) == "gz")
	{
		if (!ScGzFile::readFromFile(fileName, docBytes))
		{
			// FIXME: Needs better error return
			return QString::null;
		}
	}
	else
	{
		// Not gzip encoded, just load it
		loadRawText(fileName, docBytes);
	}
	QString docText("");
	int startElemPos = docBytes.left(512).indexOf("<SCRIBUSUTF8NEW ");
	if (startElemPos >= 0)
	{
		QRegExp regExp134("Version=\"1.3.[4-9]");
		QRegExp regExp140("Version=\"1.4.[0-9]");
		bool is134 = ( regExp134.indexIn(docBytes.mid(startElemPos, 64)) >= 0 );
		bool is140 = ( regExp140.indexIn(docBytes.mid(startElemPos, 64)) >= 0 );
		if (is134 || is140)
			docText = QString::fromUtf8(docBytes);
		if (docText.endsWith(QChar(10)) || docText.endsWith(QChar(13)))
			docText.truncate(docText.length()-1);
	}
	if (docText.isEmpty())
		return QString::null;
	return docText;
}

void Scribus134Format::getReplacedFontData(bool & getNewReplacement, QMap<QString,QString> &getReplacedFonts, QList<ScFace> &getDummyScFaces)
{
	getNewReplacement=false;
	getReplacedFonts.clear();
}

bool Scribus134Format::loadFile(const QString & fileName, const FileFormat & /* fmt */, int /* flags */, int /* index */)
{
	if (m_Doc==0 || m_AvailableFonts==0)
	{
		Q_ASSERT(m_Doc==0 || m_AvailableFonts==0);
		return false;
	}
	ParagraphStyle vg;
	struct ScribusDoc::BookMa bok;
	QMap<int, ScribusDoc::BookMa> bookmarks;

	bool newVersion = false;

	QMap<int,int> TableID;
	QMap<int,int> TableIDM;
	QMap<int,int> TableIDF;
	QList<PageItem*> TableItems;
	QList<PageItem*> TableItemsM;
	QList<PageItem*> TableItemsF;
//	QMap<PageItem*, int> groupID;
//	QMap<PageItem*, int> groupIDM;
//	QMap<PageItem*, int> groupIDF;
	QStack< QList<PageItem*> > groupStack;
	QStack< QList<PageItem*> > groupStackF;
	QStack< QList<PageItem*> > groupStackM;
	QStack< QList<PageItem*> > groupStackP;
	QStack<int> groupStack2;
	
	QString f(readSLA(fileName));
	if (f.isEmpty())
	{
		setFileReadError();
		return false;
	}
	QString fileDir = QFileInfo(fileName).absolutePath();
	int firstPage = 0;
	int layerToSetActive = 0;
	ScColor lf = ScColor();
	
	if (m_mwProgressBar!=0)
	{
		m_mwProgressBar->setMaximum(f.length());
		m_mwProgressBar->setValue(0);
	}
	// Stop autosave timer,it will be restarted only if doc has autosave feature is enabled
	if (m_Doc->autoSaveTimer->isActive())
		m_Doc->autoSaveTimer->stop();
	
	groupRemap.clear();
	itemRemap.clear();
	itemNext.clear();
	itemCount = 0;
	itemRemapM.clear();
	itemNextM.clear();
	itemCountM = 0;
	itemRemapF.clear();
	itemNextF.clear();

	TableItems.clear();
	TableID.clear();
	TableItemsM.clear();
	TableIDM.clear();
	TableItemsF.clear();
	TableIDF.clear();

	m_Doc->GroupCounter = 1;
	m_Doc->LastAuto = 0;
	m_Doc->PageColors.clear();
	m_Doc->Layers.clear();

	bool firstElement = true;
	bool success = true;
	bool hasPageSets = false;
	int  progress = 0;

	ScXmlStreamReader reader(f);
	ScXmlStreamAttributes attrs;
	while(!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (tType != QXmlStreamReader::StartElement)
			continue;
		QStringRef tagName = reader.name();
		attrs = reader.scAttributes();

		if (m_mwProgressBar != 0)
		{
			int newProgress = qRound(reader.characterOffset() / (double) f.length() * 100);
			if (newProgress != progress)
			{
				m_mwProgressBar->setValue(reader.characterOffset());
				progress = newProgress;
			}
		}

		if (firstElement)
		{
			if (tagName != "SCRIBUSUTF8NEW")
			{
				success = false;
				break;
			}
			if (attrs.hasAttribute("Version"))
				newVersion = true;
			firstElement = false;
		}
		if (tagName == "DOCUMENT")
		{
			readDocAttributes(m_Doc, attrs);
			layerToSetActive = attrs.valueAsInt("ALAYER", 0);
			if (m_Doc->pagePositioning() == 0)
				firstPage = 0;
			else
			{
				if (attrs.valueAsInt("FIRSTLEFT", 0) == 1)
					firstPage = 0;
				else
					firstPage = 1;
			}
			if (attrs.hasAttribute("currentProfile"))
			{
				m_Doc->clearCheckerProfiles();
				m_Doc->setCurCheckProfile(attrs.valueAsString("currentProfile"));
			}
		}
		if (tagName == "CheckProfile")
		{
			success = readCheckProfile(m_Doc, attrs);
			if (!success) break;
		}
		if (tagName == "PageSets")
		{
			success = readPageSets(m_Doc, reader);
			if (!success) break;
			hasPageSets = true;
		}
		// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
		if (tagName == "COLOR" && attrs.valueAsString("NAME") != CommonStrings::None)
		{
			success = readColor(m_Doc->PageColors, attrs);
			if (!success) break;
		}
		if (tagName == "STYLE")
		{
			readParagraphStyle(m_Doc, reader, vg);
			StyleSet<ParagraphStyle>tmp;
			tmp.create(vg);
			m_Doc->redefineStyles(tmp, false);
		}
		if (tagName == "CHARSTYLE")
		{
			CharStyle cstyle;
			ScXmlStreamAttributes attrs = reader.scAttributes();
			readNamedCharacterStyleAttrs(m_Doc, attrs, cstyle);
			StyleSet<CharStyle> temp;
			temp.create(cstyle);
			m_Doc->redefineCharStyles(temp, false);
		}
		if (tagName == "JAVA")
		{
			QString name = attrs.valueAsString("NAME");
			if (!name.isEmpty())
				m_Doc->JavaScripts[name] = attrs.valueAsString("SCRIPT");
		}
		if (tagName == "LAYERS")
		{
			ScLayer newLayer;
			readLayers(newLayer, attrs);
			m_Doc->Layers.append(newLayer);
		}
		if (tagName == "Arrows")
		{
			success = readArrows(m_Doc, attrs);
			if (!success) break;
		}
		if (tagName == "MultiLine")
		{
			multiLine ml;
			QString mlName = attrs.valueAsString("Name");
			success = readMultiline(ml, reader);
			if (!success) break;
			if (!mlName.isEmpty())
			{
				m_Doc->MLineStyles.insert(mlName, ml);
			}
		}
		if (tagName == "Bookmark")
		{
			int bmElem = 0;
			struct ScribusDoc::BookMa bookmark;
			success = readBookMark(bookmark, bmElem, attrs);
			if (!success) break;
			bookmarks.insert(bmElem, bookmark);
		}
		if (tagName == "PDF")
		{
			success = readPDFOptions(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "Printer")
		{
			success = readPrinterOptions(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "DocItemAttributes")
		{
			success = readDocItemAttributes(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "TablesOfContents")
		{
			success = readTableOfContents(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "Sections")
		{
			success = readSections(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "HYPHEN")
		{
			success = readHyphen(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "PAGE" || tagName == "MASTERPAGE")
		{
			success = readPage(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "PAGEOBJECT" || tagName == "MASTEROBJECT" || tagName == "FRAMEOBJECT")
		{
			ItemInfo itemInfo;
			success = readObject(m_Doc, reader, itemInfo, fileDir, false);
			if (!success) break;

			// first of linked chain?
			if (tagName == "PAGEOBJECT")
			{
				if (itemInfo.nextItem != -1)
					itemNext[itemInfo.item->ItemNr] = itemInfo.nextItem;
			}
			else if (tagName == "MASTEROBJECT")
			{
				if (itemInfo.nextItem != -1)
					itemNextM[itemInfo.item->ItemNr] = itemInfo.nextItem;
			}
			/* not sure if we want that...
			else if (tagName == "FRAMEOBJECT")
			{
				if (itemInfo.nextItem != -1)
					itemNextF[itemInfo.item->ItemNr] = itemInfo.nextItem;
			}*/

			if (itemInfo.item->isTableItem)
			{
				if (tagName == "PAGEOBJECT")
				{
					TableItems.append(itemInfo.item);
					TableID.insert(itemInfo.ownLink, itemInfo.item->ItemNr);
				}
				else if (tagName == "FRAMEOBJECT")
				{
					TableItemsF.append(itemInfo.item);
					TableIDF.insert(itemInfo.ownLink, itemInfo.item->ItemNr);
				}
				else
				{
					TableItemsM.append(itemInfo.item);
					TableIDM.insert(itemInfo.ownLink, itemInfo.item->ItemNr);
				}
			}
			if (groupStack.count() > 0)
			{
				groupStack.top().append(itemInfo.item);
				while (static_cast<int>(itemInfo.item->ItemNr) == groupStack2.top())
				{
					if (tagName == "PAGEOBJECT")
						groupStackP.push(groupStack.pop());
					else if (tagName == "FRAMEOBJECT")
						groupStackF.push(groupStack.pop());
					else
						groupStackM.push(groupStack.pop());
					groupStack2.pop();
					if (groupStack2.count() == 0)
						break;
				}
			}
			if (itemInfo.isGroupFlag)
			{
				QList<PageItem*> GroupItems;
				GroupItems.append(itemInfo.item);
				groupStack.push(GroupItems);
				groupStack2.push(itemInfo.groupLastItem + itemInfo.item->ItemNr);
			}
		}
		if (tagName == "Pattern")
		{
			success = readPattern(m_Doc, reader, fileDir);
			if (!success) break;
		}
	}

	if (reader.hasError())
	{
		setDomParsingError(reader.errorString(), reader.lineNumber(), reader.columnNumber());
		return false;
	}

	QMap<int, ScribusDoc::BookMa>::Iterator it;
	for (it = bookmarks.begin(); it != bookmarks.end(); ++it)
	{
		int elem = it.key();
		if (elem < m_Doc->Items->count())
		{
			ScribusDoc::BookMa bookmark = it.value();
			bookmark.PageObject = m_Doc->Items->at(elem);
			m_Doc->BookMarks.append( bookmark );
		}
	}

	if (TableItemsF.count() != 0)
	{
		for (int ttc = 0; ttc < TableItemsF.count(); ++ttc)
		{
			PageItem* ta = TableItemsF.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = m_Doc->FrameItems.at(TableIDF[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = m_Doc->FrameItems.at(TableIDF[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = m_Doc->FrameItems.at(TableIDF[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = m_Doc->FrameItems.at(TableIDF[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	if (TableItemsM.count() != 0)
	{
		for (int ttc = 0; ttc < TableItemsM.count(); ++ttc)
		{
			PageItem* ta = TableItemsM.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = m_Doc->MasterItems.at(TableIDM[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = m_Doc->MasterItems.at(TableIDM[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = m_Doc->MasterItems.at(TableIDM[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = m_Doc->MasterItems.at(TableIDM[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	if (TableItems.count() != 0)
	{
		for (int ttc = 0; ttc < TableItems.count(); ++ttc)
		{
			PageItem* ta = TableItems.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = m_Doc->Items->at(TableID[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = m_Doc->Items->at(TableID[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = m_Doc->Items->at(TableID[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = m_Doc->Items->at(TableID[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	//CB Add this in to set this in the file in memory. Its saved, why not load it.
	//Will of course be replaced by per page settings although we still probably need a document default
	if (!hasPageSets)
	{
		m_Doc->setPageSetFirstPage(m_Doc->pagePositioning(), firstPage);
	}

	m_Doc->setMasterPageMode(false);
	m_Doc->reformPages();

	handleOldLayerBehavior(m_Doc);
	if (m_Doc->Layers.count() == 0)
	{
		ScLayer* nl = m_Doc->Layers.newLayer( QObject::tr("Background") );
		nl->flowControl  = false;
		layerToSetActive = nl->ID;
	}
	m_Doc->setActiveLayer(layerToSetActive);

	// reestablish textframe links
	if (itemNext.count() != 0)
	{
		QMap<int,int>::Iterator lc;
		for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
		{
			if (lc.value() >= 0)
			{
				PageItem * Its = m_Doc->DocItems.at(lc.key());
				PageItem * Itn = m_Doc->DocItems.at(lc.value());
				if (Itn->prevInChain() || Its->nextInChain()) 
				{
					qDebug() << "scribus134format: corruption in linked textframes detected";
					continue;
				}
				Its->link(Itn);
			}
		}
	}
	
	if (itemNextM.count() != 0)
	{
		QMap<int,int>::Iterator lc;
		for (lc = itemNextM.begin(); lc != itemNextM.end(); ++lc)
		{
			if (lc.value() >= 0)
			{
				PageItem * Its = m_Doc->MasterItems.at(lc.key());
				PageItem * Itn = m_Doc->MasterItems.at(lc.value());
				if (Itn->prevInChain() || Its->nextInChain()) 
				{
					qDebug() << "scribus134format: corruption in linked textframes detected";
					continue;
				}
				Its->link(Itn);
			}
		}
	}
	if (groupStackP.count() > 0)
	{
		while (groupStackP.count() > 0)
		{
			QList<PageItem*> gpL = groupStackP.pop();
			PageItem* gItem = gpL.takeFirst();
			for (int id = 0; id < gpL.count(); id++)
			{
				PageItem* cItem = gpL.at(id);
				cItem->gXpos = cItem->xPos() - gItem->xPos();
				cItem->gYpos = cItem->yPos() - gItem->yPos();
				if (gItem->rotation() != 0)
				{
					QTransform ma;
					ma.rotate(-gItem->rotation());
					FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
					cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
					cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
					cItem->setRotation(cItem->rotation() - gItem->rotation());
				}
				m_Doc->DocItems.removeOne(cItem);
			}
			gItem->groupItemList = gpL;
		}
	}
	if (groupStackF.count() > 0)
	{
		while (groupStackF.count() > 0)
		{
			QList<PageItem*> gpL = groupStackF.pop();
			PageItem* gItem = gpL.takeFirst();
			for (int id = 0; id < gpL.count(); id++)
			{
				PageItem* cItem = gpL.at(id);
				cItem->gXpos = cItem->xPos() - gItem->xPos();
				cItem->gYpos = cItem->yPos() - gItem->yPos();
				if (gItem->rotation() != 0)
				{
					QTransform ma;
					ma.rotate(-gItem->rotation());
					FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
					cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
					cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
					cItem->setRotation(cItem->rotation() - gItem->rotation());
				}
				m_Doc->FrameItems.removeOne(cItem);
			}
			gItem->groupItemList = gpL;
		}
	}
	if (groupStackM.count() > 0)
	{
		while (groupStackM.count() > 0)
		{
			QList<PageItem*> gpL = groupStackM.pop();
			PageItem* gItem = gpL.takeFirst();
			for (int id = 0; id < gpL.count(); id++)
			{
				PageItem* cItem = gpL.at(id);
				cItem->gXpos = cItem->xPos() - gItem->xPos();
				cItem->gYpos = cItem->yPos() - gItem->yPos();
				if (gItem->rotation() != 0)
				{
					QTransform ma;
					ma.rotate(-gItem->rotation());
					FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
					cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
					cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
					cItem->setRotation(cItem->rotation() - gItem->rotation());
				}
				m_Doc->MasterItems.removeOne(cItem);
			}
			gItem->groupItemList = gpL;
		}
	}
	m_Doc->renumberItemsInListOrder();
	
	// reestablish first/lastAuto
	m_Doc->FirstAuto = m_Doc->LastAuto;
	if (m_Doc->LastAuto)
	{
		while (m_Doc->LastAuto->nextInChain())
			m_Doc->LastAuto = m_Doc->LastAuto->nextInChain();
		while (m_Doc->FirstAuto->prevInChain())
			m_Doc->FirstAuto = m_Doc->FirstAuto->prevInChain();
	}

	// start auto save timer if needed
	if (m_Doc->autoSave()  && ScCore->usingGUI())
		m_Doc->autoSaveTimer->start(m_Doc->autoSaveTime());
	
	if (m_mwProgressBar!=0)
		m_mwProgressBar->setValue(reader.characterOffset());
	return true;
// 	return false;
}

// Low level plugin API
int scribus134format_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* scribus134format_getPlugin()
{
	Scribus134Format* plug = new Scribus134Format();
	Q_CHECK_PTR(plug);
	return plug;
}

void scribus134format_freePlugin(ScPlugin* plugin)
{
	Scribus134Format* plug = dynamic_cast<Scribus134Format*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}


namespace {
	const int NOVALUE = -16000;
	
	void fixLegacyCharStyle(CharStyle& cstyle) 
	{
		if (! cstyle.font().usable())
			cstyle.resetFont();
		if (cstyle.fontSize() <= -16000 / 10)
			cstyle.resetFontSize();
//		if (cstyle.effects() == 65535)
//			cstyle.resetEffects();
		if (cstyle.fillColor().isEmpty())
			cstyle.resetFillColor();
		if (cstyle.fillShade() <= -16000)
			cstyle.resetFillShade();
		if (cstyle.strokeColor().isEmpty())
			cstyle.resetStrokeColor();
		if (cstyle.strokeShade() <= -16000)
			cstyle.resetStrokeShade();
		if (cstyle.shadowXOffset() <= -16000 / 10)
			cstyle.resetShadowXOffset();
		if (cstyle.shadowYOffset() <= -16000 / 10)
			cstyle.resetShadowYOffset();
		if (cstyle.outlineWidth() <= -16000 / 10)
			cstyle.resetOutlineWidth();
		if (cstyle.underlineOffset() <= -16000 / 10)
			cstyle.resetUnderlineOffset();
		if (cstyle.underlineWidth() <= -16000 / 10)
			cstyle.resetUnderlineWidth();
		if (cstyle.strikethruOffset() <= -16000 / 10)
			cstyle.resetStrikethruOffset();
		if (cstyle.strikethruWidth() <= -16000 / 10)
			cstyle.resetStrikethruWidth();
		if (cstyle.scaleH() <= -16000 / 10)
			cstyle.resetScaleH();
		if (cstyle.scaleV() <= -16000 / 10)
			cstyle.resetScaleV();
		if (cstyle.baselineOffset() <= -16000 / 10)
			cstyle.resetBaselineOffset();
		if (cstyle.tracking() <= -16000 / 10)
			cstyle.resetTracking();
	}
	
	void fixLegacyParStyle(ParagraphStyle& pstyle) 
	{
		if (pstyle.lineSpacing() <= -16000)
			pstyle.resetLineSpacing();
		if (pstyle.leftMargin() <= -16000)
			pstyle.resetLeftMargin();
		if (pstyle.rightMargin() <= -16000)
			pstyle.resetRightMargin();
		if (pstyle.firstIndent() <= -16000)
			pstyle.resetFirstIndent();
		if (pstyle.alignment() < 0)
			pstyle.resetAlignment();
		if (pstyle.gapBefore() <= -16000)
			pstyle.resetGapBefore();
		if (pstyle.gapAfter() <= -16000)
			pstyle.resetGapAfter();
		if (pstyle.dropCapLines() < 0)
			pstyle.resetDropCapLines();
		if (pstyle.dropCapOffset() <= -16000)
			pstyle.resetDropCapOffset();
		fixLegacyCharStyle(pstyle.charStyle());
	}
	
}// namespace

void Scribus134Format::readDocAttributes(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	m_Doc->setPageSize(attrs.valueAsString("PAGESIZE"));
	m_Doc->setPageOrientation(attrs.valueAsInt("ORIENTATION", 0));
	m_Doc->FirstPnum  = attrs.valueAsInt("FIRSTNUM", 1);
	m_Doc->setPagePositioning(attrs.valueAsInt("BOOK", 0));

	m_Doc->setUsesAutomaticTextFrames( attrs.valueAsInt("AUTOTEXT") );
	m_Doc->PageSp  = attrs.valueAsInt("AUTOSPALTEN");
	m_Doc->PageSpa = attrs.valueAsDouble("ABSTSPALTEN");
	m_Doc->setUnitIndex( attrs.valueAsInt("UNITS", 0) );

	m_Doc->setHyphLanguage(attrs.valueAsString("LANGUAGE", ""));
	m_Doc->setHyphMinimumWordLength(attrs.valueAsInt("MINWORDLEN", 3));
	m_Doc->setHyphConsecutiveLines(attrs.valueAsInt("HYCOUNT", 2));

	if (attrs.hasAttribute("PAGEWIDTH"))
		m_Doc->setPageWidth(attrs.valueAsDouble("PAGEWIDTH"));
	else
		m_Doc->setPageWidth(attrs.valueAsDouble("PAGEWITH"));
	m_Doc->setPageHeight(attrs.valueAsDouble("PAGEHEIGHT"));
	m_Doc->margins()->Left  = qMax(0.0, attrs.valueAsDouble("BORDERLEFT"));
	m_Doc->margins()->Right = qMax(0.0, attrs.valueAsDouble("BORDERRIGHT"));
	m_Doc->margins()->Top   = qMax(0.0, attrs.valueAsDouble("BORDERTOP"));
	m_Doc->margins()->Bottom= qMax(0.0, attrs.valueAsDouble("BORDERBOTTOM"));
	m_Doc->setMarginPreset(attrs.valueAsInt("PRESET", 0));
	m_Doc->bleeds()->Top    = attrs.valueAsDouble("BleedTop", 0.0);
	m_Doc->bleeds()->Left   = attrs.valueAsDouble("BleedLeft", 0.0);
	m_Doc->bleeds()->Right  = attrs.valueAsDouble("BleedRight", 0.0);
	m_Doc->bleeds()->Bottom = attrs.valueAsDouble("BleedBottom", 0.0);
	m_Doc->setHyphAutomatic(attrs.valueAsBool("AUTOMATIC", true));
	m_Doc->setHyphAutoCheck(attrs.valueAsBool("AUTOCHECK", false));
	m_Doc->GuideLock = attrs.valueAsBool("GUIDELOCK", false);
	
	m_Doc->rulerXoffset = attrs.valueAsDouble("rulerXoffset", 0.0);
	m_Doc->rulerYoffset = attrs.valueAsDouble("rulerYoffset", 0.0);
	m_Doc->SnapGuides   = attrs.valueAsBool("SnapToGuides", false);
	m_Doc->useRaster    = attrs.valueAsBool("SnapToGrid", false);
	
	m_Doc->setAutoSave(attrs.valueAsBool("AutoSave", false));
	m_Doc->setAutoSaveTime(attrs.valueAsInt("AutoSaveTime", 600000));

	double leftScratch;
	// FIXME A typo in early 1.3cvs (MAR 05) means we must support loading of
	// FIXME 'ScatchLeft' for a while too. This can be removed in a few months.
	if (attrs.hasAttribute("ScatchLeft"))
		leftScratch = attrs.valueAsDouble("ScatchLeft", 100.0);
	else
		leftScratch = attrs.valueAsDouble("ScratchLeft", 100.0);
	m_Doc->scratch()->set(attrs.valueAsDouble("ScratchTop", 20.0), leftScratch,
						  attrs.valueAsDouble("ScratchBottom", 20.0),attrs.valueAsDouble("ScratchRight", 100.0));
	m_Doc->setPageGapHorizontal(attrs.valueAsDouble("GapHorizontal", -1.0));
	m_Doc->setPageGapVertical(attrs.valueAsDouble("GapVertical", -1.0));
	
	if (attrs.hasAttribute("PAGEC"))
		m_Doc->setPaperColor(QColor(attrs.valueAsString("PAGEC")));
	
	m_Doc->setMarginColored(attrs.valueAsBool("RANDF", false));

	readCMSSettings(doc, attrs);
	readDocumentInfo(doc, attrs);
	readGuideSettings(doc, attrs);
	readToolSettings(doc, attrs);
	readTypographicSettings(doc, attrs);
}

void Scribus134Format::readCMSSettings(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	doc->cmsSettings().SoftProofOn     = attrs.valueAsBool("DPSo", false);
	doc->cmsSettings().SoftProofFullOn = attrs.valueAsBool("DPSFo", false);
	doc->cmsSettings().CMSinUse   = attrs.valueAsBool("DPuse", false);
	doc->cmsSettings().GamutCheck = attrs.valueAsBool("DPgam", false);
	doc->cmsSettings().BlackPoint = attrs.valueAsBool("DPbla", true);
	doc->cmsSettings().DefaultMonitorProfile   = attrs.valueAsString("DPMo","");
	doc->cmsSettings().DefaultPrinterProfile   = attrs.valueAsString("DPPr","");
	doc->cmsSettings().DefaultImageRGBProfile  = attrs.valueAsString("DPIn","");
	doc->cmsSettings().DefaultImageCMYKProfile = attrs.valueAsString("DPInCMYK","");
	doc->cmsSettings().DefaultSolidColorRGBProfile = attrs.valueAsString("DPIn2","");
	if (attrs.hasAttribute("DPIn3"))
		doc->cmsSettings().DefaultSolidColorCMYKProfile = attrs.valueAsString("DPIn3","");
	else
		doc->cmsSettings().DefaultSolidColorCMYKProfile = attrs.valueAsString("DPPr","");
	doc->cmsSettings().DefaultIntentColors = (eRenderIntent) attrs.valueAsInt("DISc", 1);
	doc->cmsSettings().DefaultIntentImages = (eRenderIntent) attrs.valueAsInt("DIIm", 0);
}

void Scribus134Format::readDocumentInfo(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	DocumentInformation di;
	di.setAuthor(attrs.valueAsString("AUTHOR"));
	di.setComments(attrs.valueAsString("COMMENTS"));
	di.setKeywords(attrs.valueAsString("KEYWORDS",""));
	di.setTitle(attrs.valueAsString("TITLE"));
	di.setSubject(attrs.valueAsString("SUBJECT"));
	di.setPublisher(attrs.valueAsString("PUBLISHER", ""));
	di.setDate(attrs.valueAsString("DOCDATE", ""));
	di.setType(attrs.valueAsString("DOCTYPE", ""));
	di.setFormat(attrs.valueAsString("DOCFORMAT", ""));
	di.setIdent(attrs.valueAsString("DOCIDENT", ""));
	di.setSource(attrs.valueAsString("DOCSOURCE", ""));
	di.setLangInfo(attrs.valueAsString("DOCLANGINFO", ""));
	di.setRelation(attrs.valueAsString("DOCRELATION", ""));
	di.setCover(attrs.valueAsString("DOCCOVER", ""));
	di.setRights(attrs.valueAsString("DOCRIGHTS", ""));
	di.setContrib(attrs.valueAsString("DOCCONTRIB", ""));
	doc->setDocumentInfo(di);
}

void Scribus134Format::readGuideSettings(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	PrefsManager* prefsManager = PrefsManager::instance();
	doc->guidesPrefs().minorGridSpacing = attrs.valueAsDouble("MINGRID", prefsManager->appPrefs.guidesPrefs.minorGridSpacing);
	doc->guidesPrefs().majorGridSpacing = attrs.valueAsDouble("MAJGRID", prefsManager->appPrefs.guidesPrefs.majorGridSpacing);
	doc->guidesPrefs().gridShown    = attrs.valueAsBool("SHOWGRID", false);
	doc->guidesPrefs().guidesShown  =attrs.valueAsBool("SHOWGUIDES", true);
	doc->guidesPrefs().colBordersShown  = attrs.valueAsBool("showcolborders", false);
	doc->guidesPrefs().framesShown  = attrs.valueAsBool("SHOWFRAME", true);
	doc->guidesPrefs().layerMarkersShown = attrs.valueAsBool("SHOWLAYERM", false);
	doc->guidesPrefs().marginsShown = attrs.valueAsBool("SHOWMARGIN", true);
	doc->guidesPrefs().baselineGridShown    = attrs.valueAsBool("SHOWBASE", false);
	doc->guidesPrefs().showPic      = attrs.valueAsBool("SHOWPICT", true);
	doc->guidesPrefs().linkShown    = attrs.valueAsBool("SHOWLINK", false);
	doc->guidesPrefs().showControls = attrs.valueAsBool("SHOWControl", false);
	doc->guidesPrefs().rulerMode    = attrs.valueAsBool("rulerMode", true);
	doc->guidesPrefs().rulersShown  = attrs.valueAsBool("showrulers", true);
	doc->guidesPrefs().showBleed    = attrs.valueAsBool("showBleed", true);
	if (attrs.hasAttribute("MARGC"))
		doc->guidesPrefs().marginColor  = QColor(attrs.valueAsString("MARGC"));
	if (attrs.hasAttribute("MINORC"))
		doc->guidesPrefs().minorGridColor = QColor(attrs.valueAsString("MINORC"));
	if (attrs.hasAttribute("MAJORC"))
		doc->guidesPrefs().majorGridColor = QColor(attrs.valueAsString("MAJORC"));
	if (attrs.hasAttribute("GuideC"))
		doc->guidesPrefs().guideColor = QColor(attrs.valueAsString("GuideC"));
	if (attrs.hasAttribute("BaseC"))
		doc->guidesPrefs().baselineGridColor  = QColor(attrs.valueAsString("BaseC"));
	doc->guidesPrefs().guidePlacement   = attrs.valueAsBool("BACKG", true);
	doc->guidesPrefs().guideRad = attrs.valueAsDouble("GuideRad", 10.0);
	doc->guidesPrefs().grabRadius  = attrs.valueAsInt("GRAB", 4);
}

void Scribus134Format::readToolSettings(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	QString textFont = attrs.valueAsString("DFONT");
	m_AvailableFonts->findFont(textFont, doc);

	doc->itemToolPrefs().textFont = textFont;
	doc->itemToolPrefs().textSize = qRound(attrs.valueAsDouble("DSIZE", 12.0) * 10);
	doc->itemToolPrefs().textColumns   = attrs.valueAsInt("DCOL", 1);
	doc->itemToolPrefs().textColumnGap    = attrs.valueAsDouble("DGAP", 0.0);

	doc->itemToolPrefs().polyCorners   = attrs.valueAsInt("POLYC", 4);
	doc->itemToolPrefs().polyFactor   = attrs.valueAsDouble("POLYF", 0.5);
	doc->itemToolPrefs().polyRotation   = attrs.valueAsDouble("POLYR", 0.0);
	doc->itemToolPrefs().polyCurvature = attrs.valueAsDouble("POLYCUR", 0.0);
	doc->itemToolPrefs().polyUseFactor   = attrs.valueAsBool("POLYS", false);

	doc->itemToolPrefs().lineStartArrow = attrs.valueAsInt("StartArrow", 0);
	doc->itemToolPrefs().lineEndArrow   = attrs.valueAsInt("EndArrow", 0);
	doc->itemToolPrefs().imageScaleX      = attrs.valueAsDouble("PICTSCX", 1.0);
	doc->itemToolPrefs().imageScaleY      = attrs.valueAsDouble("PICTSCY", 1.0);
	doc->itemToolPrefs().imageScaleType   = attrs.valueAsBool("PSCALE", true);
	doc->itemToolPrefs().imageAspectRatio = attrs.valueAsBool("PASPECT", false);
	doc->itemToolPrefs().imageLowResType  = attrs.valueAsInt("HalfRes", 1);
	doc->itemToolPrefs().imageUseEmbeddedPath = attrs.valueAsBool("EmbeddedPath", false);
	if (attrs.hasAttribute("PEN"))
		doc->itemToolPrefs().shapeLineColor = attrs.valueAsString("PEN");
	if (attrs.hasAttribute("BRUSH"))
		doc->itemToolPrefs().shapeFillColor = attrs.valueAsString("BRUSH");
	if (attrs.hasAttribute("PENLINE"))
		doc->itemToolPrefs().lineColor = attrs.valueAsString("PENLINE");
	if (attrs.hasAttribute("PENTEXT"))
		doc->itemToolPrefs().textColor = attrs.valueAsString("PENTEXT");
	if (attrs.hasAttribute("StrokeText"))
		doc->itemToolPrefs().textStrokeColor = attrs.valueAsString("StrokeText");
	doc->itemToolPrefs().textFillColor  = attrs.valueAsString("TextBackGround", CommonStrings::None);
	doc->itemToolPrefs().textLineColor   = attrs.valueAsString("TextLineColor", CommonStrings::None);
	doc->itemToolPrefs().textFillColorShade =attrs.valueAsInt("TextBackGroundShade", 100);
	doc->itemToolPrefs().textLineColorShade   = attrs.valueAsInt("TextLineShade", 100);
	doc->itemToolPrefs().textShade    = attrs.valueAsInt("TextPenShade", 100);
	doc->itemToolPrefs().textStrokeShade = attrs.valueAsInt("TextStrokeShade", 100);
	doc->itemToolPrefs().shapeLineStyle    = static_cast<Qt::PenStyle>(attrs.valueAsInt("STIL"));
	doc->itemToolPrefs().lineStyle = static_cast<Qt::PenStyle>(attrs.valueAsInt("STILLINE"));
	doc->itemToolPrefs().shapeLineWidth      = attrs.valueAsDouble("WIDTH", 0.0);
	doc->itemToolPrefs().lineWidth  = attrs.valueAsDouble("WIDTHLINE", 1.0);
	doc->itemToolPrefs().shapeLineColorShade     = attrs.valueAsInt("PENSHADE", 100);
	doc->itemToolPrefs().lineColor  = attrs.valueAsInt("LINESHADE", 100);
	doc->itemToolPrefs().shapeFillColorShade      = attrs.valueAsInt("BRUSHSHADE", 100);
	doc->opToolPrefs().magMin      = attrs.valueAsInt("MAGMIN", 1);
	doc->opToolPrefs().magMax      = attrs.valueAsInt("MAGMAX", 3200);
	doc->opToolPrefs().magStep     = attrs.valueAsInt("MAGSTEP", 200);
	doc->opToolPrefs().dispX       = attrs.valueAsDouble("dispX", 10.0);
	doc->opToolPrefs().dispY       = attrs.valueAsDouble("dispY", 10.0);
	doc->opToolPrefs().constrain   = attrs.valueAsDouble("constrain", 15.0);
	//CB Reset doc zoom step value to 200% instead of old values.
	if (doc->opToolPrefs().magStep <= 100)
		doc->opToolPrefs().magStep = 200;
	doc->itemToolPrefs().textTabFillChar = attrs.valueAsString("TabFill","");
	doc->itemToolPrefs().textTabWidth   = attrs.valueAsDouble("TabWidth", 36.0);
	if (attrs.hasAttribute("CPICT"))
		doc->itemToolPrefs().imageFillColor = attrs.valueAsString("CPICT");
	doc->itemToolPrefs().imageFillColorShade = attrs.valueAsInt("PICTSHADE", 100);
}

void Scribus134Format::readTypographicSettings(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	doc->typographicPrefs().valueSuperScript   = attrs.valueAsInt("VHOCH");
	doc->typographicPrefs().scalingSuperScript = attrs.valueAsInt("VHOCHSC");
	doc->typographicPrefs().valueSubScript     = attrs.valueAsInt("VTIEF");
	doc->typographicPrefs().scalingSubScript   = attrs.valueAsInt("VTIEFSC");
	doc->typographicPrefs().valueSmallCaps     = attrs.valueAsInt("VKAPIT");
	doc->guidesPrefs().valueBaselineGrid      = attrs.valueAsDouble("BASEGRID", 12.0);
	doc->guidesPrefs().offsetBaselineGrid     = attrs.valueAsDouble("BASEO", 0.0);
	doc->typographicPrefs().autoLineSpacing    = attrs.valueAsInt("AUTOL", 20);
	doc->typographicPrefs().valueUnderlinePos  = attrs.valueAsInt("UnderlinePos", -1);
	doc->typographicPrefs().valueUnderlineWidth  = attrs.valueAsInt("UnderlineWidth", -1);
	doc->typographicPrefs().valueStrikeThruPos   = attrs.valueAsInt("StrikeThruPos", -1);
	doc->typographicPrefs().valueStrikeThruWidth = attrs.valueAsInt("StrikeThruWidth", -1);
}

bool Scribus134Format::readPageSets(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	struct PageSet pageS;
	ScXmlStreamAttributes attrs;

	doc->clearPageSets();
	while(!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		QStringRef tagName = reader.name();
		if(reader.isStartElement())
			attrs = reader.attributes();
		if (reader.isEndElement() && tagName == "PageSets")
			break;
		if(reader.isStartElement() && tagName == "Set")
		{
			ScXmlStreamAttributes attrs = reader.scAttributes();
			pageS.Name      = CommonStrings::untranslatePageSetString(attrs.valueAsString("Name"));
			pageS.FirstPage = attrs.valueAsInt("FirstPage", 0);
			pageS.Rows      = attrs.valueAsInt("Rows", 1);
			pageS.Columns   = attrs.valueAsInt("Columns", 1);
//			pageS.GapHorizontal = attrs.valueAsDouble("GapHorizontal", 0);
//			pageS.GapVertical   = attrs.valueAsDouble("GapVertical", 0);
//			pageS.GapBelow      = attrs.valueAsDouble("GapBelow", 0);
			pageS.pageNames.clear();
		}
		if(reader.isEndElement() && tagName == "Set")
		{
			//->Prefs doc->pageSets.append(pageS);
			doc->appendToPageSets(pageS);
			if ((doc->pageSets().count()-1 == doc->pagePositioning()) && ((doc->pageGapHorizontal() < 0) && (doc->pageGapVertical() < 0)))
			{
				doc->setPageGapHorizontal(attrs.valueAsDouble("GapHorizontal", 0.0));
				doc->setPageGapVertical(attrs.valueAsDouble("GapBelow", 0.0));
			}
		}
		if (reader.isStartElement() && tagName == "PageNames")
			pageS.pageNames.append(CommonStrings::untranslatePageSetLocString(attrs.valueAsString("Name")));
	}
	return !reader.hasError();
}

bool Scribus134Format::readCheckProfile(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	struct CheckerPrefs checkerSettings;
	QString profileName = attrs.valueAsString("Name");
	if (profileName.isEmpty())
		return true;
	checkerSettings.ignoreErrors      = attrs.valueAsBool("ignoreErrors", false);
	checkerSettings.autoCheck         = attrs.valueAsBool("autoCheck", true);
	checkerSettings.checkGlyphs       = attrs.valueAsBool("checkGlyphs", true);
	checkerSettings.checkOrphans      = attrs.valueAsBool("checkOrphans", true);
	checkerSettings.checkOverflow     = attrs.valueAsBool("checkOverflow", true);
	checkerSettings.checkPictures     = attrs.valueAsBool("checkPictures", true);
	checkerSettings.checkPartFilledImageFrames = attrs.valueAsBool("checkPartFilledImageFrames", false);
	checkerSettings.checkResolution   = attrs.valueAsBool("checkResolution", true);
	checkerSettings.checkTransparency = attrs.valueAsBool("checkTransparency", true);
	checkerSettings.minResolution     = attrs.valueAsDouble("minResolution", 72.0);
	checkerSettings.maxResolution     = attrs.valueAsDouble("maxResolution", 4800.0);
	checkerSettings.checkAnnotations  = attrs.valueAsBool("checkAnnotations", false);
	checkerSettings.checkRasterPDF    = attrs.valueAsBool("checkRasterPDF", true);
	checkerSettings.checkForGIF       = attrs.valueAsBool("checkForGIF", true);
	checkerSettings.ignoreOffLayers   = attrs.valueAsBool("ignoreOffLayers", false);
	checkerSettings.checkOffConflictLayers = attrs.valueAsBool("checkOffConflictLayers", false);
	doc->set1CheckerProfile(profileName, checkerSettings);
	return true;
}

bool Scribus134Format::readColor(ColorList& colors, ScXmlStreamAttributes& attrs)
{
	ScColor color;
	if (attrs.hasAttribute("CMYK"))
		color.setNamedColor(attrs.valueAsString("CMYK"));
	else
		color.fromQColor(QColor(attrs.valueAsString("RGB")));
	color.setSpotColor( attrs.valueAsBool("Spot", false) );
	color.setRegistrationColor( attrs.valueAsBool("Register", false) );
	QString name = attrs.valueAsString("NAME");
	colors.insert((name.isEmpty()) ? color.name() : name, color);
	return true;
}

void Scribus134Format::readCharacterStyleAttrs(ScribusDoc *doc, ScXmlStreamAttributes& attrs, CharStyle & newStyle)
{
	static const QString CPARENT("CPARENT");
	if (attrs.hasAttribute(CPARENT))
		newStyle.setParent(attrs.valueAsString(CPARENT));

	static const QString FONT("FONT");
	if (attrs.hasAttribute(FONT))
		newStyle.setFont(m_AvailableFonts->findFont(attrs.valueAsString(FONT), doc));
	
	static const QString FONTSIZE("FONTSIZE");
	if (attrs.hasAttribute(FONTSIZE))
		newStyle.setFontSize(qRound(attrs.valueAsDouble(FONTSIZE) * 10));
	
	static const QString FCOLOR("FCOLOR");
	if (attrs.hasAttribute(FCOLOR))
		newStyle.setFillColor(attrs.valueAsString(FCOLOR));
	
	static const QString KERN("KERN");
	if (attrs.hasAttribute(KERN))
		newStyle.setTracking(qRound(attrs.valueAsDouble(KERN) * 10));
	
	static const QString FSHADE("FSHADE");
	if (attrs.hasAttribute(FSHADE))
		newStyle.setFillShade(attrs.valueAsInt(FSHADE));
	
	static const QString EFFECTS("EFFECTS");
	if (attrs.hasAttribute(EFFECTS))
		newStyle.setFeatures(static_cast<StyleFlag>(attrs.valueAsInt(EFFECTS)).featureList());
	
	static const QString EFFECT("EFFECT");
	if (attrs.hasAttribute(EFFECT))
		newStyle.setFeatures(static_cast<StyleFlag>(attrs.valueAsInt(EFFECT)).featureList());
	
	static const QString FEATURES("FEATURES");
	if (attrs.hasAttribute(FEATURES))
		newStyle.setFeatures(attrs.valueAsString(FEATURES).split( " ", QString::SkipEmptyParts));
	
	static const QString SCOLOR("SCOLOR");
	if (attrs.hasAttribute(SCOLOR))
		newStyle.setStrokeColor(attrs.valueAsString(SCOLOR, CommonStrings::None));
	
	static const QString SSHADE("SSHADE");
	if (attrs.hasAttribute(SSHADE))
		newStyle.setStrokeShade(attrs.valueAsInt(SSHADE));
	
	static const QString SCALEH("SCALEH");
	if (attrs.hasAttribute(SCALEH))
		newStyle.setScaleH(qRound(attrs.valueAsDouble(SCALEH) * 10));
	
	static const QString SCALEV("SCALEV");
	if (attrs.hasAttribute(SCALEV))
		newStyle.setScaleV(qRound(attrs.valueAsDouble(SCALEV) * 10));
	
	static const QString BASEO("BASEO");
	if (attrs.hasAttribute(BASEO))
		newStyle.setBaselineOffset(qRound(attrs.valueAsDouble(BASEO) * 10));
	
	static const QString TXTSHX("TXTSHX");
	if (attrs.hasAttribute(TXTSHX))
		newStyle.setShadowXOffset(qRound(attrs.valueAsDouble(TXTSHX) * 10));
	
	static const QString TXTSHY("TXTSHY");
	if (attrs.hasAttribute(TXTSHY))
		newStyle.setShadowYOffset(qRound(attrs.valueAsDouble(TXTSHY) * 10));
	
	static const QString TXTOUT("TXTOUT");
	if (attrs.hasAttribute(TXTOUT))
		newStyle.setOutlineWidth(qRound(attrs.valueAsDouble(TXTOUT) * 10));
	
	static const QString TXTULP("TXTULP");
	if (attrs.hasAttribute(TXTULP))
		newStyle.setUnderlineOffset(qRound(attrs.valueAsDouble(TXTULP) * 10));
	
	static const QString TXTULW("TXTULW");
	if (attrs.hasAttribute(TXTULW))
		newStyle.setUnderlineWidth(qRound(attrs.valueAsDouble(TXTULW) * 10));
	
	static const QString TXTSTP("TXTSTP");
	if (attrs.hasAttribute(TXTSTP))
		newStyle.setStrikethruOffset(qRound(attrs.valueAsDouble(TXTSTP) * 10));
	
	static const QString TXTSTW("TXTSTW");
	if (attrs.hasAttribute(TXTSTW))
		newStyle.setStrikethruWidth(qRound(attrs.valueAsDouble(TXTSTW) * 10));

	static const QString LANGUAGE("LANGUAGE");
	if (attrs.hasAttribute(LANGUAGE))
		newStyle.setLanguage(attrs.valueAsString(LANGUAGE));

	static const QString SHORTCUT("SHORTCUT");
	if (attrs.hasAttribute(SHORTCUT))
		newStyle.setShortcut(attrs.valueAsString(SHORTCUT));

	static const QString WORDTRACK("wordTrack");
	if (attrs.hasAttribute(WORDTRACK))
		newStyle.setWordTracking(attrs.valueAsDouble(WORDTRACK));
}

void Scribus134Format::readNamedCharacterStyleAttrs(ScribusDoc *doc, ScXmlStreamAttributes& attrs, CharStyle & newStyle)
{
	static const QString CNAME("CNAME");
	if (attrs.hasAttribute(CNAME))
		newStyle.setName(attrs.valueAsString(CNAME));

	// The default style attribute must be correctly set before trying to assign a parent
	static const QString DEFAULTSTYLE("DefaultStyle");
	if (newStyle.hasName() && attrs.hasAttribute(DEFAULTSTYLE))
		newStyle.setDefaultStyle(attrs.valueAsInt(DEFAULTSTYLE));
	else if (newStyle.name() == CommonStrings::DefaultCharacterStyle || newStyle.name() == CommonStrings::trDefaultCharacterStyle)
		newStyle.setDefaultStyle(true);
	else
		newStyle.setDefaultStyle(false);

	readCharacterStyleAttrs(doc, attrs, newStyle);
}

void Scribus134Format::readParagraphStyle(ScribusDoc *doc, ScXmlStreamReader& reader, ParagraphStyle& newStyle)
{
	ScXmlStreamAttributes attrs = reader.scAttributes();

	newStyle.erase();
	newStyle.setName(attrs.valueAsString("NAME", ""));
	// The default style attribute must be correctly set before trying to assign a parent
	static const QString DEFAULTSTYLE("DefaultStyle");
	if (attrs.hasAttribute(DEFAULTSTYLE))
		newStyle.setDefaultStyle(attrs.valueAsInt(DEFAULTSTYLE));
	else if (newStyle.name() == CommonStrings::DefaultParagraphStyle || newStyle.name() == CommonStrings::trDefaultParagraphStyle)
		newStyle.setDefaultStyle(true);
	else
		newStyle.setDefaultStyle(false);
	QString parentStyle = attrs.valueAsString("PARENT", "");
	if (!parentStyle.isEmpty())
	{
		if (m_Doc->styleExists(parentStyle))
			newStyle.setParent(parentStyle);
		else
			newStyle.setParent(CommonStrings::DefaultParagraphStyle);
	}

	static const QString LINESPMode("LINESPMode");
	if (attrs.hasAttribute(LINESPMode))
		newStyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(attrs.valueAsInt(LINESPMode)));

	static const QString LINESP("LINESP");
	if (attrs.hasAttribute(LINESP))
		newStyle.setLineSpacing(attrs.valueAsDouble(LINESP));

	static const QString INDENT("INDENT");
	if (attrs.hasAttribute(INDENT))
		newStyle.setLeftMargin(attrs.valueAsDouble(INDENT));

	static const QString RMARGIN("RMARGIN");
	if (attrs.hasAttribute(RMARGIN))
		newStyle.setRightMargin(attrs.valueAsDouble(RMARGIN));

	static const QString FIRST("FIRST");
	if (attrs.hasAttribute(FIRST))
		newStyle.setFirstIndent(attrs.valueAsDouble(FIRST));

	static const QString ALIGN("ALIGN");
	if (attrs.hasAttribute(ALIGN))
		newStyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(attrs.valueAsInt(ALIGN)));

	static const QString VOR("VOR");
	if (attrs.hasAttribute(VOR))
		newStyle.setGapBefore(attrs.valueAsDouble(VOR));

	static const QString NACH("NACH");
	if (attrs.hasAttribute(NACH))
		newStyle.setGapAfter(attrs.valueAsDouble(NACH));

	static const QString DROP("DROP");
	if (attrs.hasAttribute(DROP))
		newStyle.setHasDropCap(static_cast<bool>(attrs.valueAsInt(DROP)));

	static const QString DROPLIN("DROPLIN");
	if (attrs.hasAttribute(DROPLIN))
		newStyle.setDropCapLines(attrs.valueAsInt(DROPLIN));

	static const QString DROPDIST("DROPDIST");
	if (attrs.hasAttribute(DROPDIST))
		newStyle.setDropCapOffset(attrs.valueAsDouble(DROPDIST));

	static const QString PSHORTCUT("PSHORTCUT");
	if (attrs.hasAttribute(PSHORTCUT))
		newStyle.setShortcut(attrs.valueAsString(PSHORTCUT));
	
	static const QString OpticalMargins("OpticalMargins");
	if (attrs.hasAttribute(OpticalMargins))
		newStyle.setOpticalMargins(attrs.valueAsInt(OpticalMargins));

	static const QString HyphenationMode("HyphenationMode");
	if (attrs.hasAttribute(HyphenationMode))
		newStyle.setHyphenationMode(attrs.valueAsInt(HyphenationMode));

	static const QString MinWordTrack("MinWordTrack");
	if (attrs.hasAttribute(MinWordTrack))
		newStyle.setMinWordTracking(attrs.valueAsDouble(MinWordTrack));

	static const QString NormWordTrack("NormWordTrack");
	if (attrs.hasAttribute(NormWordTrack))
		newStyle.charStyle().setWordTracking(attrs.valueAsDouble(NormWordTrack));

	static const QString MinGlyphShrink("MinGlyphShrink");
	if (attrs.hasAttribute(MinGlyphShrink))
		newStyle.setMinGlyphExtension(attrs.valueAsDouble(MinGlyphShrink));

	static const QString MaxGlyphExtend("MaxGlyphExtend");
	if (attrs.hasAttribute(MaxGlyphExtend))
		newStyle.setMaxGlyphExtension(attrs.valueAsDouble(MaxGlyphExtend));
	
	readCharacterStyleAttrs( doc, attrs, newStyle.charStyle());

	//	newStyle.tabValues().clear();
	int numTabs = attrs.valueAsInt("NUMTAB", 0);
	if (numTabs > 0)
	{
		QList<ParagraphStyle::TabRecord> tbs;
		ParagraphStyle::TabRecord tb;
		QString tmp = attrs.valueAsString("TABS");
		ScTextStream tgv(&tmp, QIODevice::ReadOnly);
		double xf, xf2;
		for (int cxv = 0; cxv < numTabs; cxv += 2)
		{
			tgv >> xf;
			tgv >> xf2;
			tb.tabPosition = xf2;
			tb.tabType = static_cast<int>(xf);
			tb.tabFillChar =  QChar();
			tbs.append(tb);
		}
		newStyle.setTabValues(tbs);
		tmp = "";
	}
	else
	{
		QList<ParagraphStyle::TabRecord> tbs;
		newStyle.resetTabValues();
		QStringRef thisTagName = reader.name();
		while (!reader.atEnd() && !reader.hasError())
		{
			reader.readNext();
			if (reader.isEndElement() && reader.name() == thisTagName)
				break;
			if (reader.isStartElement() && reader.name() == "Tabs")
			{
				ParagraphStyle::TabRecord tb;
				ScXmlStreamAttributes attrs2 = reader.scAttributes();
				tb.tabPosition = attrs2.valueAsDouble("Pos");
				tb.tabType     = attrs2.valueAsInt("Type");
				QString tbCh   = attrs2.valueAsString("Fill","");
				tb.tabFillChar = tbCh.isEmpty() ? QChar() : tbCh[0];
				tbs.append(tb);
			}
		}
		if (tbs.count() > 0)
			newStyle.setTabValues(tbs);
	}
	
	fixLegacyParStyle(newStyle);
}

void Scribus134Format::readLayers(ScLayer& layer, ScXmlStreamAttributes& attrs)
{
	int lId   = attrs.valueAsInt("NUMMER");
	int level = attrs.valueAsInt("LEVEL");
	layer = ScLayer( attrs.valueAsString("NAME"), level, lId);
	layer.isViewable   = attrs.valueAsInt("SICHTBAR");
	layer.isPrintable  = attrs.valueAsInt("DRUCKEN");
	layer.isEditable   = attrs.valueAsInt("EDIT", 1);
	layer.flowControl  = attrs.valueAsInt("FLOW", 1);
	layer.transparency = attrs.valueAsDouble("TRANS", 1.0);
	layer.blendMode    = attrs.valueAsInt("BLEND", 0);
	layer.outlineMode  = attrs.valueAsInt("OUTL", 0);
	if (attrs.hasAttribute("LAYERC"))
		layer.markerColor =  QColor(attrs.valueAsString("LAYERC","#000000"));
}

bool Scribus134Format::readArrows(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	double xa, ya;
	struct ArrowDesc arrow;
	arrow.name = attrs.valueAsString("Name");
	arrow.userArrow = true;
	QString tmp = attrs.valueAsString("Points");
	ScTextStream fp(&tmp, QIODevice::ReadOnly);
	unsigned int numPoints = attrs.valueAsUInt("NumPoints");
	for (uint cx = 0; cx < numPoints; ++cx)
	{
		fp >> xa;
		fp >> ya;
		arrow.points.addPoint(xa, ya);
	}
	doc->appendToArrowStyles(arrow);
	return true;
}

bool Scribus134Format::readMultiline(multiLine& ml, ScXmlStreamReader& reader)
{
	ml = multiLine();
	ScXmlStreamAttributes rattrs = reader.scAttributes();
	QStringRef tagName = reader.name();
	while(!reader.atEnd() && !reader.hasError())
	{
		ScXmlStreamReader::TokenType tType = reader.readNext();
		if (tType == ScXmlStreamReader::EndElement && reader.name() == tagName)
			break;
		if (tType == ScXmlStreamReader::StartElement && reader.name() == "SubLine")
		{
			struct SingleLine sl;
			ScXmlStreamAttributes attrs = reader.scAttributes();
			sl.Color    = attrs.valueAsString("Color");
			sl.Dash     = attrs.valueAsInt("Dash");
			sl.LineEnd  = attrs.valueAsInt("LineEnd");
			sl.LineJoin = attrs.valueAsInt("LineJoin");
			sl.Shade    = attrs.valueAsInt("Shade");
			sl.Width    = attrs.valueAsDouble("Width");
			ml.shortcut = attrs.valueAsString("Shortcut");
			ml.push_back(sl);
		}
	}
	return !reader.hasError();
}

bool Scribus134Format::readBookMark(ScribusDoc::BookMa& bookmark, int& elem, ScXmlStreamAttributes& attrs)
{
	elem = attrs.valueAsInt("Element");
	bookmark.PageObject = NULL;
	bookmark.Title  = attrs.valueAsString("Title");
	bookmark.Text   = attrs.valueAsString("Text");
	bookmark.Aktion = attrs.valueAsString("Aktion");
	bookmark.ItemNr = attrs.valueAsInt("ItemNr");
	bookmark.First  = attrs.valueAsInt("First");
	bookmark.Last   = attrs.valueAsInt("Last");
	bookmark.Prev   = attrs.valueAsInt("Prev");
	bookmark.Next   = attrs.valueAsInt("Next");
	bookmark.Parent = attrs.valueAsInt("Parent");
	return true;
}

bool Scribus134Format::readPDFOptions(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	ScXmlStreamAttributes attrs = reader.scAttributes();

	doc->pdfOptions().firstUse   = attrs.valueAsBool("firstUse", true);
	doc->pdfOptions().Articles   = attrs.valueAsBool("Articles");
	doc->pdfOptions().Thumbnails = attrs.valueAsBool("Thumbnails");
	doc->pdfOptions().Compress   = attrs.valueAsBool("Compress");
	doc->pdfOptions().CompressMethod = (PDFOptions::PDFCompression)attrs.valueAsInt("CMethod", 0);
	doc->pdfOptions().Quality    = attrs.valueAsInt("Quality", 0);
	doc->pdfOptions().RecalcPic  = attrs.valueAsBool("RecalcPic");
	doc->pdfOptions().Bookmarks  = attrs.valueAsBool("Bookmarks");
	doc->pdfOptions().MirrorH    = attrs.valueAsBool("MirrorH", false);
	doc->pdfOptions().MirrorV    = attrs.valueAsBool("MirrorV", false);
	doc->pdfOptions().RotateDeg  = attrs.valueAsInt("RotateDeg", 0);
	doc->pdfOptions().doClip     = attrs.valueAsBool("Clip", false);
	doc->pdfOptions().PresentMode = attrs.valueAsBool("PresentMode");
	doc->pdfOptions().PicRes     = attrs.valueAsInt("PicRes");
	// Fixme: check input pdf version
	doc->pdfOptions().Version    = (PDFOptions::PDFVersion) attrs.valueAsInt("Version");
	doc->pdfOptions().Resolution = attrs.valueAsInt("Resolution");
	doc->pdfOptions().Binding    = attrs.valueAsInt("Binding");
	doc->pdfOptions().fileName   = "";
	doc->pdfOptions().isGrayscale   = attrs.valueAsBool("Grayscale", false);
	doc->pdfOptions().UseRGB        = attrs.valueAsBool("RGBMode", false);
	doc->pdfOptions().UseProfiles   = attrs.valueAsBool("UseProfiles", false);
	doc->pdfOptions().UseProfiles2  = attrs.valueAsBool("UseProfiles2", false);
	doc->pdfOptions().Intent        = attrs.valueAsInt("Intent", 1);
	doc->pdfOptions().Intent2       = attrs.valueAsInt("Intent2", 1);
	doc->pdfOptions().SolidProf     = attrs.valueAsString("SolidP", "");
	doc->pdfOptions().ImageProf     = attrs.valueAsString("ImageP", "");
	doc->pdfOptions().PrintProf     = attrs.valueAsString("PrintP", "");
	doc->pdfOptions().Info          = attrs.valueAsString("InfoString", "");
	doc->pdfOptions().bleeds.Top    = attrs.valueAsDouble("BTop", 0.0);
	doc->pdfOptions().bleeds.Left   = attrs.valueAsDouble("BLeft", 0.0);
	doc->pdfOptions().bleeds.Right  = attrs.valueAsDouble("BRight", 0.0);
	doc->pdfOptions().bleeds.Bottom = attrs.valueAsDouble("BBottom", 0.0);
	doc->pdfOptions().useDocBleeds  = attrs.valueAsBool("useDocBleeds", true);
	doc->pdfOptions().cropMarks     = attrs.valueAsBool("cropMarks", false);
	doc->pdfOptions().bleedMarks    = attrs.valueAsBool("bleedMarks", false);
	doc->pdfOptions().registrationMarks = attrs.valueAsBool("registrationMarks", false);
	doc->pdfOptions().colorMarks    = attrs.valueAsBool("colorMarks", false);
	doc->pdfOptions().docInfoMarks  = attrs.valueAsBool("docInfoMarks", false);
	doc->pdfOptions().markOffset    = attrs.valueAsDouble("markOffset", 0.0);
	doc->pdfOptions().EmbeddedI     = attrs.valueAsBool("ImagePr", false);
	doc->pdfOptions().PassOwner     = attrs.valueAsString("PassOwner", "");
	doc->pdfOptions().PassUser      = attrs.valueAsString("PassUser", "");
	doc->pdfOptions().Permissions   = attrs.valueAsInt("Permissions", -4);
	doc->pdfOptions().Encrypt       = attrs.valueAsBool("Encrypt", false);
	doc->pdfOptions().useLayers     = attrs.valueAsBool("UseLayers", false);
	doc->pdfOptions().UseLPI        = attrs.valueAsBool("UseLpi", false);
	doc->pdfOptions().UseSpotColors = attrs.valueAsBool("UseSpotColors", true);
	doc->pdfOptions().doMultiFile   = attrs.valueAsBool("doMultiFile", false);
	doc->pdfOptions().displayBookmarks =  attrs.valueAsBool("displayBookmarks", false);
	doc->pdfOptions().displayFullscreen = attrs.valueAsBool("displayFullscreen", false);
	doc->pdfOptions().displayLayers = attrs.valueAsBool("displayLayers", false);
	doc->pdfOptions().displayThumbs = attrs.valueAsBool("displayThumbs", false);
	doc->pdfOptions().hideMenuBar   = attrs.valueAsBool("hideMenuBar", false);
	doc->pdfOptions().hideToolBar   = attrs.valueAsBool("hideToolBar", false);
	doc->pdfOptions().fitWindow     = attrs.valueAsBool("fitWindow", false);
	doc->pdfOptions().PageLayout    = attrs.valueAsInt("PageLayout", 0);
	doc->pdfOptions().openAction    = attrs.valueAsString("openAction", "");

	QStringRef tagName = reader.name();
	while(!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && (reader.name() == tagName))
			break;
		if (!reader.isStartElement())
			continue;
		QStringRef tName = reader.name();
		attrs = reader.scAttributes();
		if(tName == "LPI")
		{
			struct LPIData lpo;
			lpo.Angle     = attrs.valueAsInt("Angle");
			lpo.Frequency = attrs.valueAsInt("Frequency");
			lpo.SpotFunc  = attrs.valueAsInt("SpotFunction");
			doc->pdfOptions().LPISettings[attrs.valueAsString("Color")] = lpo;
		}
		if(tName == "Fonts")
		{
			QString fname = attrs.valueAsString("Name");
			if (!doc->pdfOptions().EmbedList.contains(fname))
				doc->pdfOptions().EmbedList.append(fname);
		}
		if(tName == "Subset")
		{
			QString sname = attrs.valueAsString("Name");
			if (!doc->pdfOptions().SubsetList.contains(sname))
				doc->pdfOptions().SubsetList.append(sname);
		}
		if(tName == "Effekte")
		{
			struct PDFPresentationData ef;
			ef.pageEffectDuration =  attrs.valueAsInt("pageEffectDuration");
			ef.pageViewDuration =  attrs.valueAsInt("pageViewDuration");
			ef.effectType = attrs.valueAsInt("effectType");
			ef.Dm = attrs.valueAsInt("Dm");
			ef.M  = attrs.valueAsInt("M");
			ef.Di = attrs.valueAsInt("Di");
			doc->pdfOptions().PresentVals.append(ef);
		}
	}
	return !reader.hasError();
}

bool Scribus134Format::readPrinterOptions(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	ScXmlStreamAttributes attrs = reader.scAttributes();
	doc->Print_Options.firstUse = attrs.valueAsBool("firstUse");
	doc->Print_Options.toFile   = attrs.valueAsBool("toFile");
	doc->Print_Options.useAltPrintCommand = attrs.valueAsBool("useAltPrintCommand");
	doc->Print_Options.outputSeparations  = attrs.valueAsBool("outputSeparations");
	doc->Print_Options.useSpotColors      = attrs.valueAsBool("useSpotColors");
	doc->Print_Options.useColor = attrs.valueAsBool("useColor");
	doc->Print_Options.mirrorH  = attrs.valueAsBool("mirrorH");
	doc->Print_Options.mirrorV  = attrs.valueAsBool("mirrorV");
	doc->Print_Options.useICC   = attrs.valueAsBool("useICC");
	doc->Print_Options.doGCR    = attrs.valueAsBool("doGCR");
	doc->Print_Options.doClip   = attrs.valueAsBool("doClip");
	doc->Print_Options.setDevParam  = attrs.valueAsBool("setDevParam");
	doc->Print_Options.useDocBleeds = attrs.valueAsBool("useDocBleeds");
	doc->Print_Options.cropMarks    = attrs.valueAsBool("cropMarks");
	doc->Print_Options.bleedMarks   = attrs.valueAsBool("bleedMarks");
	doc->Print_Options.registrationMarks = attrs.valueAsBool("registrationMarks");
	doc->Print_Options.colorMarks   = attrs.valueAsBool("colorMarks");
	doc->Print_Options.includePDFMarks = attrs.valueAsBool("includePDFMarks", "1");
	if (attrs.hasAttribute("PrintEngine"))
		doc->Print_Options.prnEngine = (PrintEngine) attrs.valueAsInt("PrintEngine", 3);
	else
		doc->Print_Options.prnEngine = (PrintEngine) attrs.valueAsInt("PSLevel", 3);
	doc->Print_Options.markOffset    = attrs.valueAsDouble("markOffset");
	doc->Print_Options.bleeds.Top    = attrs.valueAsDouble("BleedTop");
	doc->Print_Options.bleeds.Left   = attrs.valueAsDouble("BleedLeft");
	doc->Print_Options.bleeds.Right  = attrs.valueAsDouble("BleedRight");
	doc->Print_Options.bleeds.Bottom = attrs.valueAsDouble("BleedBottom");
	doc->Print_Options.printer  = attrs.valueAsString("printer");
	doc->Print_Options.filename = attrs.valueAsString("filename");
	doc->Print_Options.separationName = attrs.valueAsString("separationName");
	doc->Print_Options.printerCommand = attrs.valueAsString("printerCommand");
	doc->Print_Options.copies = 1;

	QStringRef tagName = reader.name();
	while(!reader.atEnd() && !reader.hasError())
	{
		ScXmlStreamReader::TokenType tType = reader.readNext();
		QStringRef tName = reader.name();
		if (tType == ScXmlStreamReader::StartElement && tName == "Separation")
			doc->Print_Options.allSeparations.append(reader.attributes().value("Name").toString());
		if (tType == ScXmlStreamReader::EndElement && tName == tagName)
			break;
	}
	return !reader.hasError();
}

bool Scribus134Format::readDocItemAttributes(ScribusDoc *doc, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();
	doc->clearItemAttributes();
	while(!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if(reader.isStartElement() && reader.name() == "ItemAttribute")
		{
			ScXmlStreamAttributes attrs = reader.scAttributes();
			ObjectAttribute objattr;
			objattr.name  = attrs.valueAsString("Name");
			objattr.type  = attrs.valueAsString("Type");
			objattr.value = attrs.valueAsString("Value");
			objattr.parameter      = attrs.valueAsString("Parameter");
			objattr.relationship   = attrs.valueAsString("Relationship");
			objattr.relationshipto = attrs.valueAsString("RelationshipTo");
			objattr.autoaddto = attrs.valueAsString("AutoAddTo");
			doc->appendToItemAttributes(objattr);
		}
	}
	return !reader.hasError();
}

bool Scribus134Format::readTableOfContents(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();
	m_Doc->clearTocSetups();
	while(!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if(reader.isStartElement() && reader.name() == "TableOfContents")
		{
			ScXmlStreamAttributes attrs = reader.scAttributes();
			ToCSetup tocsetup;
			tocsetup.name = attrs.valueAsString("Name");
			tocsetup.itemAttrName = attrs.valueAsString("ItemAttributeName");
			tocsetup.frameName    = attrs.valueAsString("FrameName");
			tocsetup.textStyle    = attrs.valueAsString("Style");
			tocsetup.listNonPrintingFrames = QVariant(attrs.valueAsString("ListNonPrinting")).toBool();
			QString numberPlacement = attrs.valueAsString("NumberPlacement");
			if (numberPlacement == "Beginning")
				tocsetup.pageLocation = Beginning;
			if (numberPlacement == "End")
				tocsetup.pageLocation = End;
			if (numberPlacement == "NotShown")
				tocsetup.pageLocation = NotShown;
			doc->appendToTocSetups(tocsetup);
		}
	}
	return !reader.hasError();
}

bool Scribus134Format::readSections(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();
	while(!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (reader.isStartElement() && reader.name() == "Section")
		{
			ScXmlStreamAttributes attrs = reader.scAttributes();
			struct DocumentSection newSection;
			newSection.number = attrs.valueAsInt("Number");
			newSection.name   = attrs.valueAsString("Name");
			newSection.fromindex = attrs.valueAsInt("From");
			newSection.toindex   = attrs.valueAsInt("To");
			QString type = attrs.valueAsString("Type");
			if (type == "Type_1_2_3")
				newSection.type=Type_1_2_3;
			if (type == "Type_i_ii_iii")
				newSection.type=Type_i_ii_iii;
			if (type == "Type_I_II_III")
				newSection.type=Type_I_II_III;
			if (type == "Type_a_b_c")
				newSection.type=Type_a_b_c;
			if (type == "Type_A_B_C")
				newSection.type=Type_A_B_C;
			if (type == "Type_None")
				newSection.type=Type_None;
			newSection.sectionstartindex = attrs.valueAsInt("Start");
			newSection.reversed = attrs.valueAsBool("Reversed");
			newSection.active   = attrs.valueAsBool("Active");
			doc->sections().insert(newSection.number, newSection);
		}
	}
	return !reader.hasError();
}

bool Scribus134Format::readHyphen(ScribusDoc *doc, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();
	while(!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (reader.isStartElement() && reader.name() == "EXCEPTION")
		{
			ScXmlStreamAttributes attrs = reader.scAttributes();
			QString word = attrs.valueAsString("WORD");
			QString hyph = attrs.valueAsString("HYPHENATED");
			doc->docHyphenator->specialWords.insert(word, hyph);
		}
		else if (reader.isStartElement() && reader.name() == "IGNORE")
		{
			ScXmlStreamAttributes attrs = reader.scAttributes();
			QString word = attrs.valueAsString("WORD");
			doc->docHyphenator->ignoredWords.insert(word);
		}
	}
	return !reader.hasError();
}

bool Scribus134Format::readPage(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();

	ScXmlStreamAttributes attrs = reader.scAttributes();
	int     pageNum  = attrs.valueAsInt("NUM");
	QString pageName = attrs.valueAsString("NAM", "");
	if (tagName == "MASTERPAGE" && pageName.isEmpty())
	{
		qDebug() << "scribus134format: corrupted masterpage with empty name detected";
		return true;
	}
	m_Doc->setMasterPageMode(!pageName.isEmpty());
	Page* newPage = pageName.isEmpty() ? doc->addPage(pageNum) : doc->addMasterPage(pageNum, pageName);

	newPage->LeftPg   = attrs.valueAsInt("LEFT", 0);
	QString mpName    = attrs.valueAsString("MNAM", "Normal");
	newPage->MPageNam = m_Doc->masterPageMode() ? QString("") : mpName;
	if (attrs.hasAttribute("Size"))
		newPage->m_pageSize = attrs.valueAsString("Size");
	if (attrs.hasAttribute("Orientation"))
		newPage->setOrientation(attrs.valueAsInt("Orientation"));
	newPage->setXOffset(attrs.valueAsDouble("PAGEXPOS"));
	newPage->setYOffset(attrs.valueAsDouble("PAGEYPOS"));
	if (attrs.hasAttribute("PAGEWIDTH"))
		newPage->setWidth(attrs.valueAsDouble("PAGEWIDTH"));
	else
		newPage->setWidth(attrs.valueAsDouble("PAGEWITH"));
	newPage->setHeight(attrs.valueAsDouble("PAGEHEIGHT"));
	newPage->setInitialHeight(newPage->height());
	newPage->setInitialWidth(newPage->width());
	newPage->initialMargins.Top    = qMax(0.0, attrs.valueAsDouble("BORDERTOP"));
	newPage->initialMargins.Bottom = qMax(0.0, attrs.valueAsDouble("BORDERBOTTOM"));
	newPage->initialMargins.Left   = qMax(0.0, attrs.valueAsDouble("BORDERLEFT"));
	newPage->initialMargins.Right  = qMax(0.0, attrs.valueAsDouble("BORDERRIGHT"));
	newPage->marginPreset   = attrs.valueAsInt("PRESET", 0);
	newPage->Margins.Top    = newPage->initialMargins.Top;
	newPage->Margins.Bottom = newPage->initialMargins.Bottom;
	m_Doc->setMasterPageMode(false);
	//m_Doc->Pages=&m_Doc->DocPages;
	// guides reading
	newPage->guides.setHorizontalAutoGap( attrs.valueAsDouble("AGhorizontalAutoGap", 0.0));
	newPage->guides.setVerticalAutoGap  ( attrs.valueAsDouble("AGverticalAutoGap", 0.0));
	newPage->guides.setHorizontalAutoCount( attrs.valueAsInt("AGhorizontalAutoCount", 0) );
	newPage->guides.setVerticalAutoCount  ( attrs.valueAsInt("AGverticalAutoCount", 0) );
	newPage->guides.setHorizontalAutoRefer( attrs.valueAsInt("AGhorizontalAutoRefer", 0) );
	newPage->guides.setVerticalAutoRefer  ( attrs.valueAsInt("AGverticalAutoRefer", 0) );
	GuideManagerIO::readVerticalGuides(attrs.valueAsString("VerticalGuides"),
			newPage,
			GuideManagerCore::Standard,
			attrs.hasAttribute("NumVGuides"));
	GuideManagerIO::readHorizontalGuides(attrs.valueAsString("HorizontalGuides"),
			newPage,
			GuideManagerCore::Standard,
			attrs.hasAttribute("NumHGuides"));
	GuideManagerIO::readSelection(attrs.valueAsString("AGSelection"), newPage);

	newPage->guides.addHorizontals(newPage->guides.getAutoHorizontals(newPage), GuideManagerCore::Auto);
	newPage->guides.addVerticals(newPage->guides.getAutoVerticals(newPage), GuideManagerCore::Auto);
	return true;
}

bool Scribus134Format::readObject(ScribusDoc* doc, ScXmlStreamReader& reader, ItemInfo& info, const QString& baseDir, bool loadPage)
{
	QStringRef tagName = reader.name();
	ScXmlStreamAttributes attrs = reader.scAttributes();

	if (!loadPage)
	{
		if (tagName == "PAGEOBJECT" || tagName =="FRAMEOBJECT" || tagName =="PatternItem")
			doc->setMasterPageMode(false);
		else
			doc->setMasterPageMode(true);
	}
	int pagenr = -1;
	if ((!attrs.value("OnMasterPage").isEmpty()) && (tagName == "MASTEROBJECT"))
	{
		doc->setCurrentPage(doc->MasterPages.at(doc->MasterNames[attrs.valueAsString("OnMasterPage")]));
		pagenr = -2;
	}

	PageItem* newItem = pasteItem(doc, attrs, baseDir, pagenr);
	newItem->setRedrawBounding();
	if (tagName == "MASTEROBJECT")
		newItem->OwnPage = doc->OnPage(newItem);
	else
		newItem->OwnPage = attrs.valueAsInt("OwnPage");
	if (tagName == "PAGEOBJECT")
		newItem->OnMasterPage = "";
	QString tmpf = attrs.valueAsString("IFONT", doc->itemToolPrefs().textFont);
	m_AvailableFonts->findFont(tmpf, doc);

//	newItem->Language = ScMW->GetLang(pg.attribute("LANGUAGE", doc->Language));
	newItem->isAutoText = attrs.valueAsBool("AUTOTEXT", false);
	newItem->isEmbedded = attrs.valueAsBool("isInline", false);
	newItem->gXpos   = attrs.valueAsDouble("gXpos", 0.0);
	newItem->gYpos   = attrs.valueAsDouble("gYpos", 0.0);
	newItem->gWidth  = attrs.valueAsDouble("gWidth", newItem->width());
	newItem->gHeight = attrs.valueAsDouble("gHeight", newItem->height());
	if (newItem->isAutoText)
		doc->LastAuto = newItem;

	if (tagName == "FRAMEOBJECT")
	{
		doc->FrameItems.append(doc->Items->takeAt(newItem->ItemNr));
		newItem->ItemNr = doc->FrameItems.count()-1;
	}

	info.item     = newItem;
	info.nextItem = attrs.valueAsInt("NEXTITEM", -1);
	info.ownLink  = newItem->isTableItem ? attrs.valueAsInt("OwnLINK", 0) : 0;
	info.groupLastItem = 0;

	info.isGroupFlag = attrs.valueAsBool("isGroupControl", 0);
	if (info.isGroupFlag)
		info.groupLastItem = attrs.valueAsInt("groupsLastItem", 0);

	bool layerFound = false;
	struct ImageLoadRequest loadingInfo;
	QList<ParagraphStyle::TabRecord> tabValues;

	LastStyles * lastStyle = new LastStyles();
	while(!reader.atEnd() && !reader.hasError())
	{
		ScXmlStreamReader::TokenType tType = reader.readNext();
		if (reader.isEndElement() && tagName == reader.name())
			break;
		if (tType != ScXmlStreamReader::StartElement)
			continue;
		QStringRef tName = reader.name();
		ScXmlStreamAttributes tAtt = reader.scAttributes();
		if (tName == "CSTOP")
		{
			QString name = tAtt.valueAsString("NAME");
			double ramp  = tAtt.valueAsDouble("RAMP", 0.0);
			int shade    = tAtt.valueAsInt("SHADE", 100);
			double opa   = tAtt.valueAsDouble("TRANS", 1.0);
			newItem->fill_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
		}

		if (tName == "ITEXT")
			readItemText(newItem, tAtt, lastStyle);
		else if (tName == "para")
		{
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::PARSEP);
			ParagraphStyle newStyle;
			PrefsManager* prefsManager = PrefsManager::instance();
			readParagraphStyle(doc, reader, newStyle);
			newItem->itemText.setStyle(newItem->itemText.length()-1, newStyle);
			newItem->itemText.setCharStyle(newItem->itemText.length()-1, 1, lastStyle->Style);
		}
		else if (tName == "trail")
		{
			ParagraphStyle newStyle;
			PrefsManager* prefsManager = PrefsManager::instance();
			readParagraphStyle(doc, reader, newStyle);
			newItem->itemText.setStyle(newItem->itemText.length(), newStyle);
		}
		else if (tName == "tab")
		{
			CharStyle newStyle;
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::TAB);
			readCharacterStyleAttrs(doc, tAtt, newStyle);
			newItem->itemText.setCharStyle(newItem->itemText.length()-1, 1, newStyle);
			lastStyle->StyleStart = newItem->itemText.length()-1;
			lastStyle->Style = newStyle;
		}
		else if (tName == "breakline")
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::LINEBREAK);
		else if (tName == "breakcol")
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::COLBREAK);
		else if (tName == "breakframe")
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::FRAMEBREAK);
		else if (tName == "nbhyphen")
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::NBHYPHEN);
		else if (tName == "nbspace")
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::NBSPACE);
		else if (tName == "zwnbspace")
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::ZWNBSPACE);
		else if (tName == "zwspace")
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::ZWSPACE);
		else if (tName == "var")
		{
			CharStyle newStyle;
			if (tAtt.value("name") == "pgno")
				newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::PAGENUMBER);
			else
				newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::PAGECOUNT);
			readCharacterStyleAttrs(doc, tAtt, newStyle);
			newItem->itemText.setCharStyle(newItem->itemText.length()-1, 1, newStyle);
			lastStyle->StyleStart = newItem->itemText.length()-1;
			lastStyle->Style = newStyle;
		}
		if (tName == "PageItemAttributes")
		{
			readPageItemAttributes(newItem, reader);
		}
		if (tName == "PSDLayer")
		{
			layerFound = true;
			loadingInfo.blend   = tAtt.valueAsString("Blend");
			loadingInfo.opacity = tAtt.valueAsInt("Opacity");
			loadingInfo.visible = tAtt.valueAsBool("Visible");
			loadingInfo.useMask = tAtt.valueAsBool("useMask", true);
			newItem->pixm.imgInfo.RequestProps.insert(tAtt.valueAsInt("Layer"), loadingInfo);
		}
		if (tName == "ImageEffect")
		{
			struct ImageEffect ef;
			ef.effectParameters = tAtt.valueAsString("Param");
			ef.effectCode = tAtt.valueAsInt("Code");
			newItem->effectsInUse.append(ef);
		}
		if (tName == "Tabs")
		{
			ParagraphStyle::TabRecord tb;
			tb.tabPosition = tAtt.valueAsDouble("Pos");
			tb.tabType     = tAtt.valueAsInt("Type");
			QString tbCh   = tAtt.valueAsString("Fill", "");
			tb.tabFillChar = tbCh.isEmpty() ? QChar() : tbCh[0];
			tabValues.append(tb);
		}
		if (tName == "LATEX")
		{
			if (newItem->asLatexFrame())
			{
				readLatexInfo(newItem->asLatexFrame(), reader);
			}
			else
			{
				while (!reader.atEnd() && !reader.hasError())
				{
					reader.readNext();
					if (reader.isEndElement() && reader.name() == tName)
						break;
				}
			}
		}
	}
	delete lastStyle;

	if (tabValues.count() > 0) 
	{
		ParagraphStyle newDefault(newItem->itemText.defaultStyle());
		newDefault.setTabValues(tabValues);
		newItem->itemText.setDefaultStyle(newDefault);
	}

	if (newItem->fill_gradient.Stops() == 0)
	{
		const ScColor& col1 = doc->PageColors[doc->itemToolPrefs().shapeFillColor];
		const ScColor& col2 = doc->PageColors[doc->itemToolPrefs().shapeLineColor];
		newItem->fill_gradient.addStop(ScColorEngine::getRGBColor(col1, doc), 0.0, 0.5, 1.0, doc->itemToolPrefs().shapeFillColor, 100);
		newItem->fill_gradient.addStop(ScColorEngine::getRGBColor(col2, doc), 1.0, 0.5, 1.0, doc->itemToolPrefs().shapeLineColor, 100);
	}

	if (newItem->asPathText())
	{
		newItem->updatePolyClip();
		newItem->Frame = true;
	}
	if (newItem->asImageFrame() || newItem->asLatexFrame())
	{
		if (!newItem->Pfile.isEmpty())
		{
			doc->loadPict(newItem->Pfile, newItem, false);
			if (layerFound)
			{
				newItem->pixm.imgInfo.isRequest = true;
				doc->loadPict(newItem->Pfile, newItem, true);
			}
		}
	}

	if (!loadPage)
		doc->setMasterPageMode(false);
	return !reader.hasError();
}

bool Scribus134Format::readPattern(ScribusDoc* doc, ScXmlStreamReader& reader, const QString& baseDir)
{
	ScPattern pat;
	ScXmlStreamAttributes attrs = reader.scAttributes();
	QString patternName = attrs.valueAsString("Name");
	bool success = true;

	if (patternName.isEmpty())
	{
		reader.readToElementEnd();
		return true;
	}

	QStack< QList<PageItem*> > groupStack;
	QStack< QList<PageItem*> > groupStackP;
	QStack<int> groupStack2;
	QMap<int,int> TableID2;
	QList<PageItem*> TableItems2;

	pat.setDoc(doc);
	pat.width   = attrs.valueAsDouble("width", 0.0);
	pat.height  = attrs.valueAsDouble("height", 0.0);
	pat.scaleX  = attrs.valueAsDouble("scaleX", 0.0);
	pat.scaleY  = attrs.valueAsDouble("scaleY", 0.0);
	pat.xoffset = attrs.valueAsDouble("xoffset", 0.0);
	pat.yoffset = attrs.valueAsDouble("yoffset", 0.0);
	
	uint itemCount1 = m_Doc->Items->count();
	bool savedAlignGrid = m_Doc->useRaster;
	bool savedAlignGuides = m_Doc->SnapGuides;
	m_Doc->useRaster = false;
	m_Doc->SnapGuides = false;

	QStringRef tagName = reader.name();
	while(!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (!reader.isStartElement() || reader.name() != "PatternItem") 
			continue;

		QStringRef tName = reader.name();
		ScXmlStreamAttributes tAtt = reader.attributes();
			
		ItemInfo itemInfo;
		m_Doc->setMasterPageMode(false);
		
		int ownPage = tAtt.valueAsInt("OwnPage");
		success = readObject(doc, reader, itemInfo, baseDir, false);
		if (!success) break;

		itemInfo.item->OwnPage = ownPage;
		itemInfo.item->OnMasterPage = "";

		if (itemInfo.item->isTableItem)
		{
			TableItems2.append(itemInfo.item);
			TableID2.insert(itemInfo.ownLink, itemInfo.item->ItemNr);
		}
		if (groupStack.count() > 0)
		{
			groupStack.top().append(itemInfo.item);
			while (static_cast<int>(itemInfo.item->ItemNr) == groupStack2.top())
			{
				groupStackP.push(groupStack.pop());
				groupStack2.pop();
				if (groupStack2.count() == 0)
					break;
			}
		}
		if (itemInfo.isGroupFlag)
		{
			QList<PageItem*> GroupItems;
			GroupItems.append(itemInfo.item);
			groupStack.push(GroupItems);
			groupStack2.push(itemInfo.groupLastItem + itemInfo.item->ItemNr);
		}
	}

	doc->useRaster  = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
	if (!success)
	{
		return false;
	}

	if (TableItems2.count() != 0)
	{
		for (int ttc = 0; ttc < TableItems2.count(); ++ttc)
		{
			PageItem* ta = TableItems2.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = m_Doc->Items->at(TableID2[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = m_Doc->Items->at(TableID2[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = m_Doc->Items->at(TableID2[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = m_Doc->Items->at(TableID2[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	if (groupStackP.count() > 0)
	{
		while (groupStackP.count() > 0)
		{
			QList<PageItem*> gpL = groupStackP.pop();
			PageItem* gItem = gpL.takeFirst();
			for (int id = 0; id < gpL.count(); id++)
			{
				PageItem* cItem = gpL.at(id);
				cItem->gXpos = cItem->xPos() - gItem->xPos();
				cItem->gYpos = cItem->yPos() - gItem->yPos();
				if (gItem->rotation() != 0)
				{
					QTransform ma;
					ma.rotate(-gItem->rotation());
					FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
					cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
					cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
					cItem->setRotation(cItem->rotation() - gItem->rotation());
				}
				m_Doc->DocItems.removeOne(cItem);
			}
			gItem->groupItemList = gpL;
		}
	}
	m_Doc->renumberItemsInListOrder();

	uint itemCount2 = m_Doc->Items->count();
	if (itemCount2 > itemCount1)
	{
		PageItem* currItem = doc->Items->at(itemCount1), *newItem;
		pat.pattern = currItem->DrawObj_toImage(qMax(pat.width, pat.height));
		pat.pattern = pat.pattern.copy(-pat.xoffset, -pat.yoffset, pat.width, pat.height);
		for (uint as = itemCount1; as < itemCount2; ++as)
		{
			newItem = doc->Items->takeAt(itemCount1);
			newItem->moveBy(pat.xoffset, pat.yoffset, true);
			newItem->gXpos += pat.xoffset;
			newItem->gYpos += pat.yoffset;
			newItem->ItemNr = pat.items.count();
			pat.items.append(newItem);
		}
	}
	doc->docPatterns.insert(patternName, pat);

	return success;
}

bool Scribus134Format::readItemText(PageItem *obj, ScXmlStreamAttributes& attrs, LastStyles* last, bool impo, bool VorLFound)
{
	QString tmp2;
	CharStyle newStyle;
	ScribusDoc* doc = obj->doc();
	
	readCharacterStyleAttrs(doc, attrs, newStyle);

	if (attrs.hasAttribute(QLatin1String("Unicode")))
	{
		tmp2 = QChar(attrs.valueAsInt("Unicode"));
	}
	else
	{
		tmp2 = attrs.valueAsString("CH");
		
		// legacy stuff:
		tmp2.replace(QRegExp("\r"), QChar(13));
		tmp2.replace(QRegExp("\n"), QChar(13));
		tmp2.replace(QRegExp("\t"), QChar(9));
	}
	
	// more legacy stuff:
	static const QString CFONT("CFONT");
	if (attrs.hasAttribute(CFONT))
		newStyle.setFont(m_AvailableFonts->findFont(attrs.valueAsString(CFONT), doc));
	
	static const QString CSIZE("CSIZE");
	if (attrs.hasAttribute(CSIZE))
		newStyle.setFontSize(qRound(attrs.valueAsDouble(CSIZE) * 10));
	
	static const QString CCOLOR("CCOLOR");
	if (attrs.hasAttribute(CCOLOR))
		newStyle.setFillColor(attrs.valueAsString(CCOLOR));
	
	static const QString CEXTRA("CEXTRA");
	if (attrs.hasAttribute(CEXTRA))
		newStyle.setTracking(qRound(attrs.valueAsDouble(CEXTRA) / attrs.valueAsDouble(CSIZE) * 1000.0));
	else if (attrs.hasAttribute(QLatin1String("CKERN")))
		newStyle.setTracking(attrs.valueAsInt("CKERN"));
	
	static const QString CSHADE("CSHADE");
	if (attrs.hasAttribute(CSHADE))
		newStyle.setFillShade(attrs.valueAsInt(CSHADE));
	
	static const QString CSTYLE("CSTYLE");
	if (attrs.hasAttribute(CSTYLE))
		newStyle.setFeatures(static_cast<StyleFlag>(attrs.valueAsInt(CSTYLE)).featureList());

	QString pstylename = attrs.valueAsString("PSTYLE", "");
	int calign = attrs.valueAsInt("CALIGN", -1);		
	
	int ab = attrs.valueAsInt("CAB", -1);
	if (ab >= 5) {
		pstylename = doc->paragraphStyles()[ab-5].name();
		calign = -1;
	}
	else if (ab >= 0) {
		pstylename = "";
		calign = ab;
	}
	
	static const QString CSTROKE("CSTROKE");
	if (attrs.hasAttribute(CSTROKE))
		newStyle.setStrokeColor(attrs.valueAsString(CSTROKE, CommonStrings::None));
	
	static const QString CSHADE2("CSHADE2");
	if (attrs.hasAttribute(CSHADE2))
		newStyle.setStrokeShade(attrs.valueAsInt(CSHADE2, 100));
	
	static const QString CSCALE("CSCALE");
	if (attrs.hasAttribute(CSCALE))
		newStyle.setScaleH(qMin(qMax(qRound(attrs.valueAsDouble(CSCALE, 100.0) * 10), 100), 4000));
	
	static const QString CSCALEV("CSCALEV");
	if (attrs.hasAttribute(CSCALEV))
		newStyle.setScaleV(qMin(qMax(qRound(attrs.valueAsDouble(CSCALEV, 100.0) * 10), 100), 4000));
	
	static const QString CBASE("CBASE");
	if (attrs.hasAttribute(CBASE))
		newStyle.setBaselineOffset(qRound(attrs.valueAsDouble(CBASE) * 10));
	
	static const QString CSHX("CSHX");
	if (attrs.hasAttribute(CSHX))
		newStyle.setShadowXOffset(qRound(attrs.valueAsDouble(CSHX, 5.0) * 10));
	
	static const QString CSHY("CSHY");
	if (attrs.hasAttribute(CSHY))
		newStyle.setShadowYOffset(qRound(attrs.valueAsDouble(CSHY, -5.0) * 10));
	
	static const QString COUT("CSHY");
	if (attrs.hasAttribute(COUT))
		newStyle.setOutlineWidth(qRound(attrs.valueAsDouble(COUT, 1.0) * 10));
	
	static const QString CULP("CULP");
	if (attrs.hasAttribute(CULP))
		newStyle.setUnderlineOffset(qRound(attrs.valueAsDouble(CULP, -0.1) * 10));
	
	static const QString CULW("CULW");
	if (attrs.hasAttribute(CULW))
		newStyle.setUnderlineWidth(qRound(attrs.valueAsDouble(CULW, -0.1) * 10));
	
	static const QString CSTP("CSTP");
	if (attrs.hasAttribute(CSTP))
		newStyle.setStrikethruOffset(qRound(attrs.valueAsDouble(CSTP, -0.1) * 10));
	
	static const QString CSTW("CSTW");
	if (attrs.hasAttribute(CSTW))
		newStyle.setStrikethruWidth(qRound(attrs.valueAsDouble(CSTW, -0.1) * 10));
	
	fixLegacyCharStyle(newStyle);
	
	if (impo && ab >= 0 && VorLFound)
		last->ParaStyle = DoVorl[ab].toInt();
	else
		last->ParaStyle = pstylename;
	// end of legacy stuff
	
	int iobj = attrs.valueAsInt("COBJ", -1);

	for (int cxx=0; cxx<tmp2.length(); ++cxx)
	{
		QChar ch = tmp2.at(cxx);		
		{ // Legacy mode
			if (ch == QChar(5))
				ch = SpecialChars::PARSEP;
			if (ch == QChar(4))
				ch = SpecialChars::TAB;
		}
		
		int pos = obj->itemText.length();
		if (ch == SpecialChars::OBJECT) {
			if (iobj >= 0) {
				if (iobj < doc->FrameItems.count())
					obj->itemText.insertObject(pos, doc->FrameItems.at(iobj));
				else
					qDebug() << QString("scribus134format: invalid inline frame used in text object : %1").arg(iobj);
			}
		}
		else if (ch == SpecialChars::SHYPHEN && pos > 0)
		{
//			qDebug() << QString("scribus134format: SHYPHEN at %1").arg(pos);
			ScText* lastItem = obj->itemText.item(pos-1);
			// double SHY means user provided SHY, single SHY is automatic one
			if (lastItem->effects() & ScStyle_HyphenationPossible)
			{
				lastItem->setEffects(lastItem->effects() & ~ScStyle_HyphenationPossible);
				obj->itemText.insertChars(pos, QString(ch));
			}
			else
			{
				lastItem->setEffects(lastItem->effects() | ScStyle_HyphenationPossible);
			}
		}
		else {
			obj->itemText.insertChars(pos, QString(ch));
		}
//		qDebug() << QString("style at %1: %2 ^ %3 = %4 (%5)").arg(pos).arg((uint)newStyle.effects()).arg((uint)last->Style.effects()).arg((uint)(newStyle.effects() ^ last->Style.effects())).arg(newStyle != last->Style);
		if (newStyle != last->Style) // || (newStyle.effects() ^ last->Style.effects()) == ScStyle_HyphenationPossible) 
		{  // FIXME StyleFlag operator== ignores hyphen flag
//			qDebug() << QString("new style at %1: %2 -> %3").arg(pos).arg(last->Style.asString()).arg(newStyle.asString());
			obj->itemText.setCharStyle(last->StyleStart, pos-last->StyleStart, last->Style);
			last->Style = newStyle;
			last->StyleStart = pos;
		}
		if (ch == SpecialChars::PARSEP) {
			ParagraphStyle pstyle;
			// Qt4 if (last->ParaStyle >= 0) {
			if (!last->ParaStyle.isEmpty()) {
				pstyle.setParent( last->ParaStyle );
			}
			if (calign >= 0)
				pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(calign));
//			qDebug() << QString("par style at %1: %2/%3 (%4) calign %5").arg(pos).arg(pstyle.name()).arg(pstyle.parent()).arg(last->ParaStyle).arg(calign);
			obj->itemText.applyStyle(pos, pstyle);
		}
	}

	obj->itemText.setCharStyle(last->StyleStart, obj->itemText.length()-last->StyleStart, last->Style);
	last->StyleStart = obj->itemText.length();
/*
	QString dbg("");
	for (int i=0; i < obj->itemText.length(); ++i)
	{
		dbg += obj->itemText.text(i,1);
		if (obj->itemText.item(i)->effects() & ScStyle_HyphenationPossible)
			dbg += "~";
	}
	qDebug("scribus134format: read itemtext %d '%s'", obj->itemText.length(), dbg.latin1());
	*/
	ParagraphStyle pstyle;

	if (!last->ParaStyle.isEmpty()) { // Qt4 >= 0) {
		pstyle.setParent( last->ParaStyle );
		obj->itemText.applyStyle(obj->itemText.length()-1, pstyle);
	}
	if (calign >= 0) {
		pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(calign));
		obj->itemText.applyStyle(obj->itemText.length()-1, pstyle);
	}

	return true;
}

bool Scribus134Format::readPageItemAttributes(PageItem* item, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();
	ObjAttrVector pageItemAttributes;
	while(!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (reader.isStartElement() && reader.name() == "ItemAttribute")
		{
			ScXmlStreamAttributes tAtt = reader.scAttributes();
			ObjectAttribute objattr;
			objattr.name  = tAtt.valueAsString("Name");
			objattr.type  = tAtt.valueAsString("Type");
			objattr.value = tAtt.valueAsString("Value");
			objattr.parameter = tAtt.valueAsString("Parameter");
			objattr.relationship   = tAtt.valueAsString("Relationship");
			objattr.relationshipto = tAtt.valueAsString("RelationshipTo");
			objattr.autoaddto = tAtt.valueAsString("AutoAddTo");
			pageItemAttributes.append(objattr);
		}
	}
	item->setObjectAttributes(&pageItemAttributes);
	return !reader.hasError();
}

PageItem* Scribus134Format::pasteItem(ScribusDoc *doc, ScXmlStreamAttributes& attrs, const QString& baseDir, int pagenr)
{
	int z = 0;
	struct ImageLoadRequest loadingInfo;
	PageItem::ItemType pt = static_cast<PageItem::ItemType>(attrs.valueAsInt("PTYPE"));
	bool isGroupFlag = attrs.valueAsBool("isGroupControl", 0);
	if (isGroupFlag)
		pt = PageItem::Group;
	double x   = attrs.valueAsDouble("XPOS");
	double y   = attrs.valueAsDouble("YPOS");
	double w   = attrs.valueAsDouble("WIDTH");
	double h   = attrs.valueAsDouble("HEIGHT");
	double pw  = attrs.valueAsDouble("PWIDTH");
	double scx = attrs.valueAsDouble("LOCALSCX");
	double scy = attrs.valueAsDouble("LOCALSCY");
	QString Pcolor = attrs.valueAsString("PCOLOR");
	if (Pcolor.isEmpty())
		Pcolor = CommonStrings::None;
	QString Pcolor2 = attrs.valueAsString("PCOLOR2");
	if (Pcolor2.isEmpty())
		Pcolor2 = CommonStrings::None;
	QColor tmpc;
	PageItem *currItem=NULL;
	QString tmp;
	double xf, yf, xf2;
	QString clPath;

	switch (pt)
	{
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType1:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, x, y, w, h, pw, Pcolor, Pcolor2, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		break;
	//
	case PageItem::ImageFrame:
	case PageItem::OSGFrame:
	case PageItem::LatexFrame: /*Everything that is valid for image frames is also valid for latex frames*/
		z = doc->itemAdd(pt, PageItem::Unspecified, x, y, w, h, 1, doc->itemToolPrefs().imageFillColor, CommonStrings::None, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		UndoManager::instance()->setUndoEnabled(false);
		currItem->ScaleType   = attrs.valueAsInt("SCALETYPE", 1);
		currItem->AspectRatio = attrs.valueAsInt("RATIO", 0);
		currItem->setImageXYScale(scx, scy);
		currItem->setImageXYOffset(attrs.valueAsDouble("LOCALX"), attrs.valueAsDouble("LOCALY"));
		currItem->setImageRotation(attrs.valueAsDouble("LOCALROT", 0));
		if (!currItem->asLatexFrame())
		{
			bool inlineF = attrs.valueAsBool("isInlineImage", false);
			QString dat  = attrs.valueAsString("ImageData", "");
			QByteArray inlineImageData;
			inlineImageData.append(dat);
			QString inlineImageExt = attrs.valueAsString("inlineImageExt", "");
			if (inlineF)
			{
				if (inlineImageData.size() > 0)
				{
					currItem->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX." + inlineImageExt);
					currItem->tempImageFile->open();
					QString fileName = getLongPathName(currItem->tempImageFile->fileName());
					currItem->tempImageFile->close();
					inlineImageData = qUncompress(QByteArray::fromBase64(inlineImageData));
					QFile outFil(fileName);
					if (outFil.open(QIODevice::WriteOnly))
					{
						outFil.write(inlineImageData);
						outFil.close();
						currItem->isInlineImage = true;
						currItem->Pfile = fileName;
					}
				}
			}
			else
				currItem->Pfile = Relative2Path(attrs.valueAsString("PFILE"), baseDir);
		}
		currItem->IProfile    = attrs.valueAsString("PRFILE", "");
		currItem->EmProfile   = attrs.valueAsString("EPROF" , "");
		currItem->IRender     = (eRenderIntent) attrs.valueAsInt("IRENDER", 1);
		currItem->UseEmbedded = attrs.valueAsInt("EMBEDDED", 1);
		currItem->pixm.imgInfo.lowResType = attrs.valueAsInt("ImageRes", 1);
		currItem->pixm.imgInfo.actualPageNumber = attrs.valueAsInt("Pagenumber", 0);
		if (currItem->asLatexFrame())
		{
			currItem->setImageXYOffset(attrs.valueAsDouble("LOCALX") * scx, attrs.valueAsDouble("LOCALY") * scy);
	//		currItem->setImageXYScale(1.0, 1.0);
		}
		else
			currItem->setImageXYScale(scx, scy);
		currItem->setImageRotation(attrs.valueAsDouble("LOCALROT", 0));
		clPath = attrs.valueAsString("ImageClip", "");
		if (currItem->pixm.imgInfo.PDSpathData.contains(clPath))
		{
			currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[clPath].copy();
			currItem->pixm.imgInfo.usedPath = clPath;
			QTransform cl;
			cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
			cl.scale(currItem->imageXScale(), currItem->imageYScale());
			currItem->imageClip.map(cl);
		}
		currItem->setImageShown( attrs.valueAsInt("PICART"));
/*		currItem->BBoxX = ScCLocale::toDoubleC( obj->attribute("BBOXX"));
		currItem->BBoxH = ScCLocale::toDoubleC( obj->attribute("BBOXH")); */
		currItem->setLineWidth(pw);
		UndoManager::instance()->setUndoEnabled(true);
		break;
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType3:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, x, y, w, h, pw, Pcolor, Pcolor2, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		break;
	//
	case PageItem::PathText:
		z = doc->itemAdd(PageItem::PathText, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Pcolor, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		break;
	case PageItem::TextFrame:
		z = doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Pcolor, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		break;
	case PageItem::Line:
		z = doc->itemAdd(PageItem::Line, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Pcolor2, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		break;
	case PageItem::Polygon:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		break;
	case PageItem::PolyLine:
		z = doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		break;
	case PageItem::Group:
		z = doc->itemAdd(PageItem::Group, PageItem::Unspecified, x, y, w, h, 0, CommonStrings::None, CommonStrings::None, true);
		currItem = doc->Items->at(z);
		if (pagenr > -2) 
			currItem->OwnPage = pagenr;
		currItem->groupWidth = attrs.valueAsDouble("groupWidth", w);
		currItem->groupHeight = attrs.valueAsDouble("groupHeight", h);
		doc->GroupCounter++;
		break;
	case PageItem::Symbol:
	case PageItem::RegularPolygon:
	case PageItem::Arc:
	case PageItem::Spiral:
	case PageItem::Multiple:
		Q_ASSERT(false);
		break;
	}

	UndoManager::instance()->setUndoEnabled(false);
	currItem->FrameType = attrs.valueAsInt("FRTYPE", 0);
	int startArrowIndex = attrs.valueAsInt("startArrowIndex", 0);
	if ((startArrowIndex < 0) || (startArrowIndex > static_cast<int>(doc->arrowStyles().size())))
	{
		qDebug() << QString("scribus134format: invalid arrow index: %").arg(startArrowIndex);
		startArrowIndex = 0;
	}
	currItem->setStartArrowIndex(startArrowIndex);
	int endArrowIndex = attrs.valueAsInt("endArrowIndex", 0);
	if ((endArrowIndex < 0) || (endArrowIndex > static_cast<int>(doc->arrowStyles().size())))
	{
		qDebug() << QString("scribus134format: invalid arrow index: %").arg(endArrowIndex);
		endArrowIndex = 0;
	}
	currItem->setEndArrowIndex(endArrowIndex);
	currItem->setStartArrowScale(attrs.valueAsInt("startArrowScale", 100));
	currItem->setEndArrowScale(attrs.valueAsInt("endArrowScale", 100));
	currItem->NamedLStyle = attrs.valueAsString("NAMEDLST", "");
	currItem->isBookmark  = attrs.valueAsInt("BOOKMARK");
	if ((currItem->isBookmark) && (doc->BookMarks.count() == 0))
		doc->OldBM = true;
	currItem->setImageFlippedH( attrs.valueAsInt("FLIPPEDH"));
	currItem->setImageFlippedV( attrs.valueAsInt("FLIPPEDV"));
	currItem->setCornerRadius( attrs.valueAsDouble("RADRECT", 0.0));
	currItem->ClipEdited = attrs.valueAsInt("CLIPEDIT", 0);
	currItem->setFillColor(Pcolor);
	currItem->setLineColor(Pcolor2);
	currItem->setFillShade(attrs.valueAsInt("SHADE"));
	currItem->setLineShade(attrs.valueAsInt("SHADE2"));

	ParagraphStyle pstyle;
	if (attrs.hasAttribute("LINESP"))
		pstyle.setLineSpacing(attrs.valueAsDouble("LINESP"));
	if (attrs.hasAttribute("LINESPMode"))
		pstyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(attrs.valueAsInt("LINESPMode", 0)));
	if (attrs.hasAttribute("ALIGN"))
		pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(attrs.valueAsInt("ALIGN", 0)));
	if (attrs.hasAttribute("IFONT"))
		pstyle.charStyle().setFont(m_AvailableFonts->findFont(attrs.valueAsString("IFONT"), doc));
	if (attrs.hasAttribute("ISIZE"))
		pstyle.charStyle().setFontSize(qRound(attrs.valueAsDouble("ISIZE") * 10));
	if (attrs.hasAttribute("TXTSTROKE"))
		pstyle.charStyle().setStrokeColor(attrs.valueAsString("TXTSTROKE"));
	if (attrs.hasAttribute("TXTFILL"))
		pstyle.charStyle().setFillColor(attrs.valueAsString("TXTFILL"));
	if (attrs.hasAttribute("TXTSTRSH"))
		pstyle.charStyle().setStrokeShade(attrs.valueAsInt("TXTSTRSH"));
	if (attrs.hasAttribute("TXTFILLSH"))
		pstyle.charStyle().setFillShade(attrs.valueAsInt("TXTFILLSH"));
	if (attrs.hasAttribute("TXTSCALE"))
		pstyle.charStyle().setScaleH(qRound(attrs.valueAsDouble("TXTSCALE") * 10));
	if (attrs.hasAttribute("TXTSCALEV"))
		pstyle.charStyle().setScaleV(qRound(attrs.valueAsDouble("TXTSCALEV") * 10));
	if (attrs.hasAttribute("TXTBASE"))
		pstyle.charStyle().setBaselineOffset(qRound(attrs.valueAsDouble("TXTBASE") * 10));
	if (attrs.hasAttribute("TXTSHX"))
		pstyle.charStyle().setShadowXOffset(qRound(attrs.valueAsDouble("TXTSHX") * 10));
	if (attrs.hasAttribute("TXTSHY"))
		pstyle.charStyle().setShadowYOffset(qRound(attrs.valueAsDouble("TXTSHY") * 10));
	if (attrs.hasAttribute("TXTOUT"))
		pstyle.charStyle().setOutlineWidth(qRound(attrs.valueAsDouble("TXTOUT") * 10));
	if (attrs.hasAttribute("TXTULP"))
		pstyle.charStyle().setUnderlineOffset(qRound(attrs.valueAsDouble("TXTULP") * 10));
	if (attrs.hasAttribute("TXTULW"))
		pstyle.charStyle().setUnderlineWidth(qRound(attrs.valueAsDouble("TXTULW") * 10));
	if (attrs.hasAttribute("TXTSTP"))
		pstyle.charStyle().setStrikethruOffset(qRound(attrs.valueAsDouble("TXTSTP") * 10));
	if (attrs.hasAttribute("TXTSTW"))
		pstyle.charStyle().setStrikethruWidth(qRound(attrs.valueAsDouble("TXTSTW") * 10));
	if (attrs.hasAttribute("TXTSTYLE"))
		pstyle.charStyle().setFeatures(static_cast<StyleFlag>(attrs.valueAsInt("TXTSTYLE")).featureList());
	if (attrs.hasAttribute("TXTKERN"))
		pstyle.charStyle().setTracking(qRound(attrs.valueAsDouble("TXTKERN", 0.0) * 10));
	if (attrs.hasAttribute("wordTrack"))
		pstyle.charStyle().setWordTracking(attrs.valueAsDouble("wordTrack"));
	if (attrs.hasAttribute("MinWordTrack"))
		pstyle.setMinWordTracking(attrs.valueAsDouble("MinWordTrack"));
	if (attrs.hasAttribute("MinGlyphShrink"))
		pstyle.setMinGlyphExtension(attrs.valueAsDouble("MinGlyphShrink"));
	if (attrs.hasAttribute("MaxGlyphExtend"))
		pstyle.setMaxGlyphExtension(attrs.valueAsDouble("MaxGlyphExtend"));
	if (attrs.hasAttribute("OpticalMargins"))
		pstyle.setOpticalMargins(attrs.valueAsInt("OpticalMargins"));
	if (attrs.hasAttribute("HyphenationMode"))
		pstyle.setHyphenationMode(attrs.valueAsInt("HyphenationMode"));
	if (attrs.hasAttribute("leftMargin"))
		pstyle.setLeftMargin(attrs.valueAsDouble("leftMargin"));
	if (attrs.hasAttribute("rightMargin"))
		pstyle.setRightMargin(attrs.valueAsDouble("rightMargin"));
	if (attrs.hasAttribute("firstIndent"))
		pstyle.setFirstIndent(attrs.valueAsDouble("firstIndent"));
	currItem->itemText.setDefaultStyle(pstyle);

	if (attrs.hasAttribute("PSTYLE"))
	{
		QString pstyleName = attrs.valueAsString("PSTYLE");
		if (!pstyleName.isEmpty())
		{
			ParagraphStyle defStyle(currItem->itemText.defaultStyle());
			defStyle.setParent(pstyleName);
			currItem->itemText.setDefaultStyle(defStyle);
		}
	}

	currItem->setRotation( attrs.valueAsDouble("ROT") );
	currItem->setTextToFrameDist(attrs.valueAsDouble("EXTRA"),
								attrs.valueAsDouble("REXTRA", 1.0),
								attrs.valueAsDouble("TEXTRA", 1.0),
								attrs.valueAsDouble("BEXTRA", 1.0));
	currItem->setFirstLineOffset(static_cast<FirstLineOffsetPolicy>(attrs.valueAsInt("FLOP")));

	currItem->PLineArt  = Qt::PenStyle(attrs.valueAsInt("PLINEART"));
	currItem->PLineEnd  = Qt::PenCapStyle(attrs.valueAsInt("PLINEEND", 0));
	currItem->PLineJoin = Qt::PenJoinStyle(attrs.valueAsInt("PLINEJOIN", 0));
	currItem->setPrintEnabled( attrs.valueAsInt("PRINTABLE"));
	currItem->setIsAnnotation( attrs.valueAsInt("ANNOTATION", 0));
	currItem->annotation().setType( attrs.valueAsInt("ANTYPE", 0));
	QString AnName = attrs.valueAsString("ANNAME","");
	if (!AnName.isEmpty())
	{
		if (currItem->itemName() == AnName)
			currItem->AutoName = true;
		else
			currItem->setItemName(AnName);
	}
	currItem->annotation().setAction( attrs.valueAsString("ANACTION","") );
	currItem->annotation().setE_act ( attrs.valueAsString("ANEACT","") );
	currItem->annotation().setX_act ( attrs.valueAsString("ANXACT","") );
	currItem->annotation().setD_act ( attrs.valueAsString("ANDACT","") );
	currItem->annotation().setFo_act( attrs.valueAsString("ANFOACT","") );
	currItem->annotation().setBl_act( attrs.valueAsString("ANBLACT","") );
	currItem->annotation().setK_act ( attrs.valueAsString("ANKACT","") );
	currItem->annotation().setF_act ( attrs.valueAsString("ANFACT","") );
	currItem->annotation().setV_act ( attrs.valueAsString("ANVACT","") );
	currItem->annotation().setC_act ( attrs.valueAsString("ANCACT","") );
	currItem->annotation().setActionType(attrs.valueAsInt("ANACTYP", 0));
	currItem->annotation().setExtern( attrs.valueAsString("ANEXTERN",""));
	if ((!currItem->annotation().Extern().isEmpty()) && (currItem->annotation().ActionType() != 8))
		currItem->annotation().setExtern(Relative2Path(attrs.valueAsString("ANEXTERN", "") , baseDir));
	currItem->annotation().setZiel( attrs.valueAsInt("ANZIEL", 0));
	currItem->annotation().setToolTip ( attrs.valueAsString("ANTOOLTIP",""));
	currItem->annotation().setRollOver( attrs.valueAsString("ANROLL",""));
	currItem->annotation().setDown( attrs.valueAsString("ANDOWN",""));
	currItem->annotation().setBwid( attrs.valueAsInt("ANBWID", 1));
	currItem->annotation().setBsty( attrs.valueAsInt("ANBSTY", 0));
	currItem->annotation().setFeed( attrs.valueAsInt("ANFEED", 1));
	currItem->annotation().setFlag( attrs.valueAsInt("ANFLAG", 0));
	currItem->annotation().setFont( attrs.valueAsInt("ANFONT", 4));
	currItem->annotation().setFormat( attrs.valueAsInt("ANFORMAT", 0));
	currItem->annotation().setVis( attrs.valueAsInt("ANVIS", 0));
	currItem->annotation().setIsChk( attrs.valueAsBool("ANCHK", false) );
	currItem->annotation().setAAact( attrs.valueAsBool("ANAA", false) );
	currItem->annotation().setHTML ( attrs.valueAsInt("ANHTML", 0));
	currItem->annotation().setUseIcons( attrs.valueAsBool("ANICON", false));
	currItem->annotation().setChkStil ( attrs.valueAsInt("ANCHKS", 0));
	currItem->annotation().setMaxChar ( attrs.valueAsInt("ANMC", -1));
	currItem->annotation().setBorderColor( attrs.valueAsString("ANBCOL", CommonStrings::None));
	currItem->annotation().setIPlace(attrs.valueAsInt("ANPLACE", 1));
	currItem->annotation().setScaleW(attrs.valueAsInt("ANSCALE", 0));

	if (currItem->asTextFrame() || currItem->asPathText())
	{
		UndoManager::instance()->setUndoEnabled(false);
		if (currItem->isAnnotation() && currItem->annotation().UseIcons())
		{
			currItem->ScaleType   = attrs.valueAsInt("SCALETYPE", 1);
			currItem->AspectRatio = attrs.valueAsInt("RATIO", 0);
			currItem->setImageXYScale(scx, scy);
			currItem->setImageXYOffset(attrs.valueAsDouble("LOCALX"), attrs.valueAsDouble("LOCALY"));
			currItem->setImageRotation(attrs.valueAsDouble("LOCALROT", 0));
			currItem->Pfile  = Relative2Path(attrs.valueAsString("PFILE" , ""), baseDir);
			currItem->Pfile2 = Relative2Path(attrs.valueAsString("PFILE2", ""), baseDir);
			currItem->Pfile3 = Relative2Path(attrs.valueAsString("PFILE3", ""), baseDir);
			currItem->IProfile    = attrs.valueAsString("PRFILE", "");
			currItem->EmProfile   = attrs.valueAsString("EPROF", "");
			currItem->IRender     = (eRenderIntent) attrs.valueAsInt("IRENDER" , 1);
			currItem->UseEmbedded = attrs.valueAsInt("EMBEDDED", 1);
			doc->LoadPict(currItem->Pfile, z);
			currItem->setImageXYScale(scx, scy);
			currItem->setImageShown( attrs.valueAsInt("PICART"));
/*			currItem->BBoxX = ScCLocale::toDoubleC( obj->attribute("BBOXX"));
			currItem->BBoxH = ScCLocale::toDoubleC( obj->attribute("BBOXH")); */
		}
		UndoManager::instance()->setUndoEnabled(true);
	}

	currItem->TopLine      = attrs.valueAsBool("TopLine", false);
	currItem->LeftLine     = attrs.valueAsBool("LeftLine", false);
	currItem->RightLine    = attrs.valueAsBool("RightLine", false);
	currItem->BottomLine   = attrs.valueAsBool("BottomLine", false);
	currItem->isTableItem  = attrs.valueAsBool("isTableItem", false);
	currItem->TopLinkID    = attrs.valueAsInt("TopLINK", -1);
	currItem->LeftLinkID   = attrs.valueAsInt("LeftLINK", -1);
	currItem->RightLinkID  = attrs.valueAsInt("RightLINK", -1);
	currItem->BottomLinkID = attrs.valueAsInt("BottomLINK", -1);
	currItem->PoShow       = attrs.valueAsInt("PLTSHOW", 0);
	currItem->BaseOffs     = attrs.valueAsDouble("BASEOF", 0.0);
	currItem->textPathType =  attrs.valueAsInt("textPathType", 0);
	currItem->textPathFlipped = attrs.valueAsBool("textPathFlipped", false);
	if ( attrs.hasAttribute("TEXTFLOWMODE") )
		currItem->setTextFlowMode((PageItem::TextFlowMode) attrs.valueAsInt("TEXTFLOWMODE", 0));
	else if ( attrs.valueAsInt("TEXTFLOW") )
	{
		if (attrs.valueAsInt("TEXTFLOW2", 0))
			currItem->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
		else if (attrs.valueAsInt("TEXTFLOW3", 0))
			currItem->setTextFlowMode(PageItem::TextFlowUsesContourLine);
		else
			currItem->setTextFlowMode(PageItem::TextFlowUsesFrameShape);	
	}
	else
		currItem->setTextFlowMode(PageItem::TextFlowDisabled);
	currItem->DashOffset = attrs.valueAsDouble("DASHOFF", 0.0);
	currItem->setReversed(attrs.valueAsBool("REVERS", false));
	currItem->setLocked (attrs.valueAsBool("LOCK", false));
	currItem->setSizeLocked(attrs.valueAsBool("LOCKR", false));
	currItem->setFillTransparency(attrs.valueAsDouble("TransValue", 0.0));
	currItem->fillRule    = attrs.valueAsBool("fillRule", true);
	currItem->doOverprint = attrs.valueAsBool("doOverprint", false);
	if (attrs.hasAttribute("TransValueS"))
		currItem->setLineTransparency(attrs.valueAsDouble("TransValueS", 0.0));
	else
		currItem->setLineTransparency(attrs.valueAsDouble("TransValue", 0.0));
	currItem->setFillBlendmode(attrs.valueAsInt("TransBlend", 0));
	currItem->setLineBlendmode(attrs.valueAsInt("TransBlendS", 0));
	if (attrs.valueAsInt("TRANSPARENT", 0) == 1)
		currItem->setFillColor(CommonStrings::None);
	currItem->Cols   = attrs.valueAsInt("COLUMNS", 1);
	currItem->ColGap = attrs.valueAsDouble("COLGAP", 0.0);
	if (attrs.valueAsInt("LAYER", 0) != -1)
		currItem->LayerID = attrs.valueAsInt("LAYER", 0);
	tmp = "";

	QList<ParagraphStyle::TabRecord> tbs;
	tmp = "";
	if ((attrs.hasAttribute("NUMTAB")) && (attrs.valueAsInt("NUMTAB", 0) != 0))
	{
		ParagraphStyle::TabRecord tb;
		tmp = attrs.valueAsString("TABS");
		ScTextStream tgv(&tmp, QIODevice::ReadOnly);
		int numTab = attrs.valueAsInt("NUMTAB", 0);
		for (int cxv = 0; cxv < numTab; cxv += 2)
		{
			tgv >> xf;
			tgv >> xf2;
			tb.tabPosition = xf2;
			tb.tabType = static_cast<int>(xf);
			tb.tabFillChar = QChar();
			tbs.append(tb);
		}
		tmp = "";
	}
	if (tbs.count() > 0) {
		ParagraphStyle newDefault(currItem->itemText.defaultStyle());
		newDefault.setTabValues(tbs);
		currItem->itemText.setDefaultStyle(newDefault);
	}
	
	if ((attrs.hasAttribute("NUMDASH")) && (attrs.valueAsInt("NUMDASH", 0) != 0))
	{
		tmp = attrs.valueAsString("DASHS");
		ScTextStream dgv(&tmp, QIODevice::ReadOnly);
		currItem->DashValues.clear();
		int numDash = attrs.valueAsInt("NUMDASH", 0);
		for (int cxv = 0; cxv < numDash; ++cxv)
		{
			dgv >> xf;
			currItem->DashValues.append(xf);
		}
		tmp = "";
	}
	else
		currItem->DashValues.clear();

	tmp = "";
	if (attrs.hasAttribute("NUMPO"))
	{
		currItem->PoLine.resize(attrs.valueAsUInt("NUMPO"));
		tmp = attrs.valueAsString("POCOOR");
		ScTextStream fp(&tmp, QIODevice::ReadOnly);
		uint numPo = attrs.valueAsUInt("NUMPO");
		for (uint cx=0; cx < numPo; ++cx)
		{
			fp >> xf;
			fp >> yf;
			currItem->PoLine.setPoint(cx, xf, yf);
		}
	}
	else
		currItem->PoLine.resize(0);

	tmp = "";
	if (attrs.hasAttribute("NUMCO"))
	{
		currItem->ContourLine.resize(attrs.valueAsUInt("NUMCO"));
		tmp = attrs.valueAsString("COCOOR");
		ScTextStream fp(&tmp, QIODevice::ReadOnly);
		uint numCo = attrs.valueAsUInt("NUMCO");
		for (uint cx=0; cx < numCo; ++cx)
		{
			fp >> xf;
			fp >> yf;
			currItem->ContourLine.setPoint(cx, xf, yf);
		}
	}
	else
		currItem->ContourLine = currItem->PoLine.copy();

	if (!currItem->asLine())
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	else
	{
		int ph = static_cast<int>(qMax(1.0, currItem->lineWidth() / 2.0));
		currItem->Segments.clear();
		currItem->PoLine.resize(0);
		currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
		                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
		                  -ph,static_cast<int>(currItem->height()+ph));
		currItem->setHeight(1.0);
	}

	if (currItem->asPathText())
	{
		currItem->updatePolyClip();
		currItem->Frame = true;
	}
	currItem->GrType = attrs.valueAsInt("GRTYP", 0);
	QString GrColor;
	QString GrColor2;
	int GrShade = 0;
	int GrShade2 = 0;
	if (currItem->GrType != 0)
	{
		if (currItem->GrType == 8)
		{
			currItem->setPattern( attrs.valueAsString("pattern", "") );
			double patternScaleX   = attrs.valueAsDouble("pScaleX", 100.0);
			double patternScaleY   = attrs.valueAsDouble("pScaleY", 100.0);
			double patternOffsetX  = attrs.valueAsDouble("pOffsetX", 0.0);
			double patternOffsetY  = attrs.valueAsDouble("pOffsetY", 0.0);
			double patternRotation = attrs.valueAsDouble("pRotation", 0.0);
			currItem->setPatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, 0, 0);
		}
		else
		{
			currItem->GrStartX = attrs.valueAsDouble("GRSTARTX", 0.0);
			currItem->GrStartY = attrs.valueAsDouble("GRSTARTY", 0.0);
			currItem->GrEndX   = attrs.valueAsDouble("GRENDX", currItem->width());
			currItem->GrEndY   = attrs.valueAsDouble("GRENDY", 0.0);
			GrColor = attrs.valueAsString("GRCOLOR","");
			if (!GrColor.isEmpty())
			{
				GrColor2 = attrs.valueAsString("GRCOLOR2","");
				GrShade  = attrs.valueAsInt("GRSHADE", 100);
				GrShade2 = attrs.valueAsInt("GRSHADE2", 100);
			}
		}
	}
	if ((currItem->GrType != 0) && (currItem->GrType != 8))
	{
		currItem->fill_gradient.clearStops();
		if ((!GrColor.isEmpty()) && (!GrColor2.isEmpty()))
		{
			if (currItem->GrType == 5)
			{
				if ((GrColor != CommonStrings::None) && (!GrColor.isEmpty()))
					currItem->SetQColor(&tmpc, GrColor, GrShade);
				currItem->fill_gradient.addStop(tmpc, 0.0, 0.5, 1.0, GrColor, GrShade);
				if ((GrColor2 != CommonStrings::None) && (!GrColor2.isEmpty()))
					currItem->SetQColor(&tmpc, GrColor2, GrShade2);
				currItem->fill_gradient.addStop(tmpc, 1.0, 0.5, 1.0, GrColor2, GrShade2);
			}
			else
			{
				if ((GrColor2 != CommonStrings::None) && (!GrColor2.isEmpty()))
					currItem->SetQColor(&tmpc, GrColor2, GrShade2);
				currItem->fill_gradient.addStop(tmpc, 0.0, 0.5, 1.0, GrColor2, GrShade2);
				if ((GrColor != CommonStrings::None) && (!GrColor.isEmpty()))
					currItem->SetQColor(&tmpc, GrColor, GrShade);
				currItem->fill_gradient.addStop(tmpc, 1.0, 0.5, 1.0, GrColor, GrShade);
			}
		}
//		currItem->updateGradientVectors();
	}
	switch (currItem->GrType)
	{
		case 1:
		case 2:
		case 3:
		case 4:
			currItem->GrType = 6;
			break;
		case 5:
			currItem->GrType = 7;
			break;
		default:
			break;
	}
	//currItem->setRedrawBounding();
	//currItem->OwnPage = view->OnPage(currItem);
	UndoManager::instance()->setUndoEnabled(true);
	return currItem;
}

bool Scribus134Format::readLatexInfo(PageItem_LatexFrame* latexitem, ScXmlStreamReader& reader)
{
	ScXmlStreamAttributes attrs = reader.scAttributes();
	QStringRef tagName = reader.name();

	latexitem->setConfigFile(attrs.valueAsString("ConfigFile"), true);
	latexitem->setDpi(attrs.valueAsInt("DPI"));
	latexitem->setUsePreamble(attrs.valueAsBool("USE_PREAMBLE"));

	QString formula;
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (reader.isCharacters())
			formula += reader.text().toString();
		if (reader.isStartElement() && reader.name() == "PROPERTY")
		{
			ScXmlStreamAttributes tAtt = reader.scAttributes();
			QString name  = tAtt.valueAsString("name");
			QString value = tAtt.valueAsString("value");
			if (name.isEmpty()) continue;
			latexitem->editorProperties[name] = value;
		}
	}
	formula = formula.trimmed();
	latexitem->setFormula(formula, false);

	return !reader.hasError();
}

bool Scribus134Format::loadPage(const QString & fileName, int pageNumber, bool Mpage, QString renamedPageName)
{
// 	qDebug() << QString("loading page %2 from file '%1' from 1.3.x plugin").arg(fileName).arg(pageNumber);
	if (m_Doc==0 || m_AvailableFonts==0)
	{
		Q_ASSERT(m_Doc==0 || m_AvailableFonts==0);
		return false;
	}
	ParagraphStyle vg;
	struct ScribusDoc::BookMa bok;
	QMap<int, ScribusDoc::BookMa> bookmarks;

	Page* newPage = NULL;
	
	QString tmp;
	QMap<int,int> TableID;
	QList<PageItem*> TableItems;
	QMap<PageItem*, int> groupID;
	double pageX = 0, pageY = 0;
	bool newVersion = false;
	bool vorLFound  = false;
	QMap<int,int> layerTrans;
	int maxLayer = 0, maxLevel = 0, a = 0;

	layerTrans.clear();
	uint layerCount=m_Doc->layerCount();
	for (uint la2 = 0; la2 < layerCount; ++la2)
	{
		maxLayer = qMax(m_Doc->Layers[la2].ID, maxLayer);
		maxLevel = qMax(m_Doc->Layers[la2].Level, maxLevel);
	}

	groupRemap.clear();
	itemRemap.clear();
	itemNext.clear();
	itemCount = 0;
	itemRemapM.clear();
	itemNextM.clear();
	itemCountM = 0;
	itemRemapF.clear();
	itemNextF.clear();

	DoVorl.clear();
	DoVorl[0] = "0";
	DoVorl[1] = "1";
	DoVorl[2] = "2";
	DoVorl[3] = "3";
	DoVorl[4] = "4";
	VorlC = 5;

 	QString f(readSLA(fileName));
	if (f.isEmpty())
	{
		setFileReadError();
		return false;
	}

	QString fileDir = QFileInfo(fileName).absolutePath();

	bool firstElement = true;
	bool success = true;
	
	ScXmlStreamReader reader(f);
	ScXmlStreamAttributes attrs;
	while(!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (tType != QXmlStreamReader::StartElement)
			continue;
		QStringRef tagName = reader.name();
		attrs = reader.scAttributes();

		if (firstElement)
		{
			if (tagName != "SCRIBUSUTF8NEW")
			{
				success = false;
				break;
			}
			if (attrs.hasAttribute("Version"))
				newVersion = true;
			firstElement = false;
		}

		if (tagName == "COLOR" && attrs.valueAsString("NAME") != CommonStrings::None)
		{
			success = readColor(m_Doc->PageColors, attrs);
			if (!success) break;
		}
		if (tagName == "JAVA")
		{
			QString name = attrs.valueAsString("NAME");
			if (!name.isEmpty())
				m_Doc->JavaScripts[name] = attrs.valueAsString("SCRIPT");
		}
		if (tagName == "LAYERS")
		{
			ScLayer newLayer;
			readLayers(newLayer, attrs);
			const ScLayer* la2 = m_Doc->Layers.layerByName(newLayer.Name);
			if (la2)
				layerTrans.insert(newLayer.ID, la2->ID);
			else
			{
				maxLayer++;
				maxLevel++;
				layerTrans.insert(newLayer.ID, maxLayer);
				newLayer.ID = maxLayer;
				newLayer.Level = maxLevel;
				m_Doc->Layers.append(newLayer);
			}
		}
		if (tagName == "Arrows")
		{
			success = readArrows(m_Doc, attrs);
			if (!success) break;
		}
		if(tagName == "MultiLine")
		{
			multiLine ml;
			QString mlName  = attrs.valueAsString("Name");
			QString mlName2 = mlName;
			readMultiline(ml, reader);
			int copyC = 1;
			QMap<QString,multiLine>::ConstIterator mlit = m_Doc->MLineStyles.find(mlName2);
			if (mlit != m_Doc->MLineStyles.end() && ml != mlit.value())
			{
				while (m_Doc->MLineStyles.contains(mlName2))
				{
					mlName2 = QObject::tr("Copy #%1 of ").arg(copyC)+mlName;
					copyC++;
				}
			}
			m_Doc->MLineStyles.insert(mlName2, ml);
		}
		if (tagName == "Pattern")
		{
			success = readPattern(m_Doc, reader, fileDir);
			if (!success) break;
		}
		if (tagName == "Bookmark")
		{
			int bmElem = 0;
			struct ScribusDoc::BookMa bookmark;
			success = readBookMark(bookmark, bmElem, attrs);
			if (!success) break;
			bookmarks.insert(bmElem, bookmark);
		}
		if(tagName == "STYLE")
		{
			getStyle(vg, reader, NULL, m_Doc, true);
			vorLFound = true;
		}
		if (((tagName == "PAGE") || (tagName == "MASTERPAGE")) && (attrs.valueAsInt("NUM") == pageNumber))
		{
			if (Mpage && (tagName != "MASTERPAGE"))
				continue;
			a = m_Doc->currentPage()->pageNr();
			newPage = m_Doc->Pages->at(a);
			if (Mpage)
			{
				newPage->LeftPg = attrs.valueAsInt("LEFT", 0);
				
				if (!renamedPageName.isEmpty())
					newPage->setPageName(renamedPageName);
				else
					newPage->setPageName(attrs.valueAsString("NAM",""));
			}
			if (attrs.hasAttribute("Size"))
				newPage->m_pageSize = attrs.valueAsString("Size");
			if (attrs.hasAttribute("Orientation"))
				newPage->setOrientation(attrs.valueAsInt("Orientation"));
			if (attrs.hasAttribute("PAGEWIDTH"))
				newPage->setWidth( attrs.valueAsDouble("PAGEWIDTH") );
			else
				newPage->setWidth( attrs.valueAsDouble("PAGEWITH") );
			newPage->setHeight( attrs.valueAsDouble("PAGEHEIGHT") );
			newPage->setInitialHeight(newPage->height());
			newPage->setInitialWidth(newPage->width());
			newPage->initialMargins.Top    = qMax(0.0, attrs.valueAsDouble("BORDERTOP"));
			newPage->initialMargins.Bottom = qMax(0.0, attrs.valueAsDouble("BORDERBOTTOM"));
			newPage->initialMargins.Left   = qMax(0.0, attrs.valueAsDouble("BORDERLEFT"));
			newPage->initialMargins.Right  = qMax(0.0, attrs.valueAsDouble("BORDERRIGHT"));
			newPage->marginPreset = attrs.valueAsInt("PRESET", 0);
			newPage->Margins.Top = newPage->initialMargins.Top;
			newPage->Margins.Bottom = newPage->initialMargins.Bottom;
			pageX = attrs.valueAsDouble( attrs.valueAsString("PAGEXPOS"));
			pageY = attrs.valueAsDouble( attrs.valueAsString("PAGEYPOS"));
			// guides reading
			tmp = "";
			newPage->guides.setHorizontalAutoGap(attrs.valueAsDouble("AGhorizontalAutoGap", 0.0));
			newPage->guides.setVerticalAutoGap(attrs.valueAsDouble("AGverticalAutoGap", 0.0));
			newPage->guides.setHorizontalAutoCount(attrs.valueAsInt("AGhorizontalAutoCount", 0));
			newPage->guides.setVerticalAutoCount(attrs.valueAsInt("AGverticalAutoCount", 0));
			newPage->guides.setHorizontalAutoRefer(attrs.valueAsInt("AGhorizontalAutoRefer", 0));
			newPage->guides.setVerticalAutoRefer(attrs.valueAsInt("AGverticalAutoRefer", 0));
			GuideManagerIO::readVerticalGuides(attrs.valueAsString("VerticalGuides"),
											newPage,
											GuideManagerCore::Standard,
											attrs.hasAttribute("NumVGuides"));
			GuideManagerIO::readHorizontalGuides(attrs.valueAsString("HorizontalGuides"),
											newPage,
											GuideManagerCore::Standard,
											attrs.hasAttribute("NumHGuides"));
			GuideManagerIO::readSelection(attrs.valueAsString("AGSelection"), newPage);

			newPage->guides.addHorizontals(newPage->guides.getAutoHorizontals(newPage), GuideManagerCore::Auto);
			newPage->guides.addVerticals(newPage->guides.getAutoVerticals(newPage), GuideManagerCore::Auto);
		}
		if ((tagName == "PAGEOBJECT") || (tagName == "MASTEROBJECT") || (tagName == "FRAMEOBJECT"))
		{
			if ((Mpage && tagName != "MASTEROBJECT") || (!Mpage && tagName == "MASTEROBJECT"))
			{
				// Go to end of node
				reader.readToElementEnd();
				continue;
			}
			if (attrs.valueAsInt("OwnPage") != pageNumber)
			{			
				if (tagName == "PAGEOBJECT")
					itemRemap[itemCount++] = -1;
				else if (tagName == "MASTEROBJECT")
					itemRemapM[itemCountM++] = -1;
				reader.readToElementEnd();
			}
			else
			{
				// first of linked chain?
				if (tagName == "PAGEOBJECT")
				{
					itemRemap[itemCount++] = m_Doc->DocItems.count();
					if (attrs.valueAsInt("NEXTITEM") != -1)
						itemNext[m_Doc->DocItems.count()] = attrs.valueAsInt("NEXTITEM");
				}
				else if (tagName == "MASTEROBJECT")
				{
					itemRemapM[itemCountM++] = m_Doc->MasterItems.count();
					if (attrs.valueAsInt("NEXTITEM") != -1)
						itemNextM[m_Doc->MasterItems.count()] = attrs.valueAsInt("NEXTITEM");
				}

				ItemInfo itemInfo;
				success = readObject(m_Doc, reader, itemInfo, fileDir, true);
				if (!success) break;

				PageItem* newItem = itemInfo.item;
				newItem->moveBy(-pageX + newPage->xOffset(), - pageY + newPage->yOffset());
				newItem->OwnPage = m_Doc->currentPageNumber();
				if (tagName == "PAGEOBJECT")
					newItem->OnMasterPage = "";
				newItem->LayerID = layerTrans.value(newItem->LayerID, newItem->LayerID);

				if (newItem->isTableItem)
				{
					TableItems.append(newItem);
					TableID.insert(itemInfo.ownLink, newItem->ItemNr);
				}
				if (tagName == "FRAMEOBJECT")
				{
					m_Doc->FrameItems.append(m_Doc->Items->takeAt(newItem->ItemNr));
					newItem->ItemNr = m_Doc->FrameItems.count()-1;
				}
			}
		}
	}

	if (reader.hasError())
	{
		setDomParsingError(reader.errorString(), reader.lineNumber(), reader.columnNumber());
		return false;
	}

	QMap<int, ScribusDoc::BookMa>::Iterator it;
	for (it = bookmarks.begin(); it != bookmarks.end(); ++it)
	{
		int elem = it.key();
		if (elem < m_Doc->Items->count())
		{
			ScribusDoc::BookMa bookmark = it.value();
			bookmark.PageObject = m_Doc->Items->at(elem);
			m_Doc->BookMarks.append( bookmark );
		}
	}

	if (TableItems.count() != 0)
	{
		for (int ttc = 0; ttc < TableItems.count(); ++ttc)
		{
			PageItem* ta = TableItems.at(ttc);
			if (ta->TopLinkID != -1)
				ta->TopLink = m_Doc->Items->at(TableID[ta->TopLinkID]);
			else
				ta->TopLink = 0;
			if (ta->LeftLinkID != -1)
				ta->LeftLink = m_Doc->Items->at(TableID[ta->LeftLinkID]);
			else
				ta->LeftLink = 0;
			if (ta->RightLinkID != -1)
				ta->RightLink = m_Doc->Items->at(TableID[ta->RightLinkID]);
			else
				ta->RightLink = 0;
			if (ta->BottomLinkID != -1)
				ta->BottomLink = m_Doc->Items->at(TableID[ta->BottomLinkID]);
			else
				ta->BottomLink = 0;
		}
	}
	
	// reestablish textframe links
	if (itemNext.count() != 0 && !Mpage)
	{
		QMap<int,int>::Iterator lc;
		for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
		{
			if (itemRemap[lc.value()] >= 0)
			{
				if ((lc.key() < m_Doc->Items->count()) && (itemRemap[lc.value()] < m_Doc->Items->count()))
				{
					PageItem * Its = m_Doc->DocItems.at(lc.key());
					PageItem * Itn = m_Doc->DocItems.at(itemRemap[lc.value()]);
					if (Itn->prevInChain() || Its->nextInChain()) 
					{
						qDebug() << "scribus134format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
				}
			}
		}
	}
	else if (itemNextM.count() != 0 && Mpage)
	{
		QMap<int,int>::Iterator lc;
		for (lc = itemNextM.begin(); lc != itemNextM.end(); ++lc)
		{
			if (itemRemapM[lc.value()] >= 0)
			{
				if ((lc.key() < m_Doc->MasterItems.count()) && (itemRemapM[lc.value()] < m_Doc->MasterItems.count()))
				{
					PageItem * Its = m_Doc->MasterItems.at(lc.key());
					PageItem * Itn = m_Doc->MasterItems.at(itemRemapM[lc.value()]);
					if (Itn->prevInChain() || Its->nextInChain())
					{
						qDebug() << "scribus134format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
				}
			}
		}
	}
	
	// reestablish first/lastAuto
	m_Doc->FirstAuto = m_Doc->LastAuto;
	if (m_Doc->LastAuto)
	{
		while (m_Doc->LastAuto->nextInChain())
			m_Doc->LastAuto = m_Doc->LastAuto->nextInChain();
		while (m_Doc->FirstAuto->prevInChain())
			m_Doc->FirstAuto = m_Doc->FirstAuto->prevInChain();
	}

	return true;
}

void Scribus134Format::getStyle(ParagraphStyle& style, ScXmlStreamReader& reader, StyleSet<ParagraphStyle> *tempStyles, ScribusDoc* doc, bool fl)
{
	bool fou(false);
	QString tmpf, tmV;
	const StyleSet<ParagraphStyle> * docParagraphStyles = tempStyles? tempStyles : & doc->paragraphStyles();
	PrefsManager* prefsManager = PrefsManager::instance();
	readParagraphStyle(doc, reader, style);
	for (int xx=0; xx<docParagraphStyles->count(); ++xx)
	{
		if (style.name() == (*docParagraphStyles)[xx].name())
		{
			if (style.equiv((*docParagraphStyles)[xx]))
			{
				if (fl)
				{
					DoVorl[VorlC] = tmV.setNum(xx);
					VorlC++;
				}
				fou = true;
			}
			else
			{
				style.setName("Copy of "+(*docParagraphStyles)[xx].name());
				fou = false;
			}
			break;
		}
	}
	if (!fou)
	{
		for (int xx=0; xx< docParagraphStyles->count(); ++xx)
		{
			if (style.equiv((*docParagraphStyles)[xx]) && fl)
			{
				style.setName((*docParagraphStyles)[xx].name());
				fou = true;
// 				if (fl)
				{
					DoVorl[VorlC] = tmV.setNum(xx);
					VorlC++;
				}
				break;
			}
		}
	}
	if (!fou)
	{
		if (tempStyles)
			tempStyles->create(style);
		else
		{
			StyleSet<ParagraphStyle> tmp;
			tmp.create(style);
			doc->redefineStyles(tmp, false);
		}
		if (fl)
		{
			DoVorl[VorlC] = tmV.setNum(docParagraphStyles->count()-1);
			VorlC++;
		}
	}
}

bool Scribus134Format::readStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles)
{
	ParagraphStyle pstyle;
	QString f (readSLA(fileName));
	if (f.isEmpty())
		return false;

	bool firstElement = true;
	bool success = true;

	ScXmlStreamReader reader(f);
	ScXmlStreamAttributes attrs;
	while(!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (tType != QXmlStreamReader::StartElement)
			continue;
		QStringRef tagName = reader.name();
		if (firstElement)
		{
			if (tagName != "SCRIBUSUTF8NEW")
			{
				success = false;
				break;
			}
			firstElement = false;
			continue;
		}
		if (tagName == "STYLE")
		{
			pstyle.erase();
			getStyle(pstyle, reader, &docParagraphStyles, doc, false);
		}
	}
	return success;
}

bool Scribus134Format::readCharStyles(const QString& fileName, ScribusDoc* doc, StyleSet<CharStyle> &docCharStyles)
{
	CharStyle cstyle;
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	bool firstElement = true;
	bool success = true;

	ScXmlStreamReader reader(f);
	ScXmlStreamAttributes attrs;
	while(!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (tType != QXmlStreamReader::StartElement)
			continue;
		QStringRef tagName = reader.name();
		if (firstElement)
		{
			if (tagName != "SCRIBUSUTF8NEW")
			{
				success = false;
				break;
			}
			firstElement = false;
			continue;
		}
		if (tagName == "CHARSTYLE")
		{
			cstyle.erase();
			readNamedCharacterStyleAttrs(doc, attrs, cstyle);
			docCharStyles.create(cstyle);
		}
	}
	return true;
}

bool Scribus134Format::readLineStyles(const QString& fileName, QMap<QString,multiLine> *styles)
{
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	bool firstElement = true;
	bool success = true;

	ScXmlStreamReader reader(f);
	ScXmlStreamAttributes attrs;
	while(!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (tType != QXmlStreamReader::StartElement)
			continue;
		QStringRef tagName = reader.name();
		if (firstElement)
		{
			if (tagName != "SCRIBUSUTF8NEW")
			{
				success = false;
				break;
			}
			firstElement = false;
			continue;
		}
		if (tagName == "MultiLine")
		{
			multiLine ml;
			attrs = reader.scAttributes();
			QString mlName  = attrs.valueAsString("Name");
			QString mlName2 = mlName;
			readMultiline(ml, reader);
			int copyC = 1;
			QMap<QString,multiLine>::ConstIterator mlit = styles->find(mlName2);
			if (mlit != styles->end() && ml != mlit.value())
			{
				while (styles->contains(mlName2))
				{
					mlName2 = tr("Copy #%1 of ").arg(copyC)+mlName;
					copyC++;
				}
			}
			styles->insert(mlName2, ml);
		}
	}
	return true;
}

bool Scribus134Format::readColors(const QString& fileName, ColorList & colors)
{
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	bool firstElement = true;
	bool success = true;

	ScXmlStreamReader reader(f);
	ScXmlStreamAttributes attrs;
	while(!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (tType != QXmlStreamReader::StartElement)
			continue;
		QStringRef tagName = reader.name();
		if (firstElement)
		{
			if (tagName != "SCRIBUSUTF8NEW")
			{
				success = false;
				break;
			}
			firstElement = false;
			continue;
		}
		if(tagName == "COLOR" && attrs.valueAsString("NAME") != CommonStrings::None)
		{
			attrs = reader.scAttributes();
			if (attrs.valueAsString("NAME") != CommonStrings::None)
			{
				readColor(colors, attrs);
			}
		}
	}
	return true;
}

bool Scribus134Format::readPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames)
{
	QString pageName;
	int counter = 0;
	int counter2 = 0;
	QString f(readSLA(fileName));
	if (f.isEmpty())
		return false;
	bool firstElement = true;
	bool success = true;

	ScXmlStreamReader reader(f);
	ScXmlStreamAttributes attrs;
	while(!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (tType != QXmlStreamReader::StartElement)
			continue;
		QStringRef tagName = reader.name();
		if (firstElement)
		{
			if (tagName != "SCRIBUSUTF8NEW")
			{
				success = false;
				break;
			}
			firstElement = false;
			continue;
		}
		if(tagName == "PAGE")
			counter++;
		if(tagName == "MASTERPAGE")
		{
			pageName = reader.scAttributes().valueAsString("NAM");
			if (!pageName.isEmpty())
			{
				counter2++;
				masterPageNames.append(pageName);
			}
		}
	}
	*num1 = counter;
	*num2 = counter2;
	return success;
}





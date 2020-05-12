/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribus150format.h"
#include "scribus150formatimpl.h"

#include <algorithm>

#include "../../formatidlist.h"
#include "commonstrings.h"
#include "ui/missing.h"
#include "hyphenator.h"
#include "langmgr.h"
#include "notesstyles.h"
#include "pageitem_latexframe.h"
#include "pageitem_line.h"
#include "pageitem_noteframe.h"
#include "pagesize.h"
#include "prefsmanager.h"
#include "qtiocompressor.h"
#include "scclocale.h"
#include "scconfig.h"
#include "sccolorengine.h"
#include "scpattern.h"
#include "scribuscore.h"
#include "scribusdoc.h"

#include "sctextstream.h"
#include "scxmlstreamreader.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_math.h"
#include "util_color.h"
#include "util_text.h"
#ifdef HAVE_OSG
	#include "pageitem_osgframe.h"
#endif
#include "pageitem_table.h"
#include "pageitem_regularpolygon.h"
#include "pageitem_arc.h"
#include "pageitem_spiral.h"
#include "pagestructs.h"

#include <QApplication>
#include <QByteArray>
#include <QCursor>
// #include <QDebug>
#include <QFileInfo>
#include <QList>
#include <QScopedPointer>

// See scplugin.h and pluginmanager.{cpp,h} for detail on what these methods
// do. That documentatation is not duplicated here.
// Please don't implement the functionality of your plugin here; do that
// in scribus150formatimpl.h and scribus150formatimpl.cpp .

Scribus150Format::Scribus150Format()
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file formats.
	registerFormats();
	languageChange();
}

Scribus150Format::~Scribus150Format()
{
	unregisterAll();
}

void Scribus150Format::languageChange()
{
	FileFormat* fmt = getFormatByID(FORMATID_SLA150IMPORT);
	fmt->trName = tr("Scribus 1.5.0+ Document");
	fmt->filter = fmt->trName + " (*.sla *.SLA *.sla.gz *.SLA.GZ *.scd *.SCD *.scd.gz *.SCD.GZ)";
}

QString Scribus150Format::fullTrName() const
{
	return QObject::tr("Scribus 1.5.0+ Support");
}

const ScActionPlugin::AboutData* Scribus150Format::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8(
			"Franz Schmid <franz@scribus.info>, "
			"The Scribus Team");
	about->shortDescription = tr("Scribus 1.5.0+ File Format Support");
	about->description = tr("Allows Scribus to read Scribus 1.5.0 and higher formatted files.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void Scribus150Format::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void Scribus150Format::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Scribus 1.5.0+ Document");
	fmt.formatId = FORMATID_SLA150IMPORT;
	fmt.load = true;
	fmt.save = true;
	fmt.colorReading = true;
	fmt.filter = fmt.trName + " (*.sla *.SLA *.sla.gz *.SLA.GZ *.scd *.SCD *.scd.gz *.SCD.GZ)";
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/x-scribus");
	fmt.fileExtensions = QStringList() << "sla" << "sla.gz" << "scd" << "scd.gz";
	fmt.priority = 64;
	fmt.nativeScribus = true;
	registerFormat(fmt);
}

bool Scribus150Format::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	QByteArray docBytes("");
	if (fileName.right(2) == "gz")
	{
		QFile file(fileName);
		QtIOCompressor compressor(&file);
		compressor.setStreamFormat(QtIOCompressor::GzipFormat);
		compressor.open(QIODevice::ReadOnly);
		docBytes = compressor.read(1024);
		compressor.close();
		if (docBytes.isEmpty())
			return false;
	}
	else
	{
		// Not gzip encoded, just load it
		loadRawText(fileName, docBytes);
	}
	QRegExp regExp150("Version=\"1.5.[0-9]");
	int startElemPos = docBytes.left(512).indexOf("<SCRIBUSUTF8NEW ");
	if (startElemPos >= 0)
	{
		bool is150 = ( regExp150.indexIn(docBytes.mid(startElemPos, 64)) >= 0 );
		return is150;
	}
	return false;
}

QIODevice* Scribus150Format::slaReader(const QString & fileName)
{
	if (!fileSupported(nullptr, fileName))
		return nullptr;

	QIODevice* ioDevice = nullptr;
	if (fileName.right(2) == "gz")
	{
		aFile.setFileName(fileName);
		QtIOCompressor *compressor = new QtIOCompressor(&aFile);
		compressor->setStreamFormat(QtIOCompressor::GzipFormat);
		if (!compressor->open(QIODevice::ReadOnly))
		{
			delete compressor;
			return nullptr;
		}
		ioDevice = compressor;
	}
	else
	{
		ioDevice = new QFile(fileName);
		if (!ioDevice->open(QIODevice::ReadOnly))
		{
			delete ioDevice;
			return nullptr;
		}
	}
	return ioDevice;
}

void Scribus150Format::getReplacedFontData(bool & getNewReplacement, QMap<QString,QString> &getReplacedFonts, QList<ScFace> &getDummyScFaces)
{
	getNewReplacement = false;
	getReplacedFonts.clear();
}

bool Scribus150Format::loadElements(const QString& data, const QString& fileDir, int toLayer, double Xp_in, double Yp_in, bool loc)
{
	ParagraphStyle vg;
	isNewFormat = false;
	LayerToPaste = toLayer;
	Xp = Xp_in;
	Yp = Yp_in;
	GrX = 0.0;
	GrY = 0.0;

	QList<PageItem*> TableItems;
	QList<PageItem*> TableItemsF;
	QList<PageItem*> TableItemsM;
	QList<PageItem*> WeldItems;
	QMap<int,PageItem*> TableID;
	QMap<int,PageItem*> TableIDF;
	QMap<int,PageItem*> TableIDM;
	QMap<int,PageItem*> WeldID;
	QStack< QList<PageItem*> > groupStackFI;
	QStack< QList<PageItem*> > groupStackMI;
	QStack< QList<PageItem*> > groupStackPI;
	QStack< QList<PageItem*> > groupStackF;
	QStack< QList<PageItem*> > groupStackM;
	QStack< QList<PageItem*> > groupStackP;
	QStack<int> groupStackFI2;
	QStack<int> groupStackMI2;
	QStack<int> groupStackPI2;

	itemRemap.clear();
	itemNext.clear();
	itemCount = 0;
	itemRemapM.clear();
	itemNextM.clear();
	itemCountM = 0;
	itemRemapF.clear();
	itemNextF.clear();
	FrameItems.clear();
	LinkID.clear();

	markeredItemsMap.clear();
	markeredMarksMap.clear();
	nsetRangeItemNamesMap.clear();
	notesFramesData.clear();
	notesMasterMarks.clear();
	notesNSets.clear();

	bool firstElement = true;
	bool success = true;

	ReadObjectParams readObjectParams;
	readObjectParams.baseDir = fileDir;
	readObjectParams.itemKind = PageItem::StandardItem;
	readObjectParams.loadingPage = true;

	ScXmlStreamReader reader(data);
	ScXmlStreamAttributes attrs;
	while (!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (tType != QXmlStreamReader::StartElement)
			continue;
		QStringRef tagName = reader.name();
		attrs = reader.scAttributes();

		if (firstElement)
		{
			if (tagName == "SCRIBUSELEM" || tagName == "SCRIBUSELEMUTF8")
			{
				if (!loc)
				{
					GrX = attrs.valueAsDouble("XP");
					GrY = attrs.valueAsDouble("YP");
				}
			}
			else
			{
				success = false;
				break;
			}
			firstElement = false;
		}
		// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
		if (tagName == "COLOR" && attrs.valueAsString("NAME") != CommonStrings::None)
			readColor(m_Doc->PageColors, attrs);
		if (tagName == "Gradient")
		{
			VGradient gra;
			QString grName = attrs.valueAsString("Name");
			success = readGradient(m_Doc, gra, reader);
			if (!success)
				break;
			gra.setRepeatMethod((VGradient::VGradientRepeatMethod)(attrs.valueAsInt("Ext", VGradient::pad)));
			if (!grName.isEmpty())
			{
				m_Doc->docGradients.insert(grName, gra);
			}
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
			StyleSet<CharStyle> temp;
			ScXmlStreamAttributes attrs = reader.scAttributes();
			readNamedCharacterStyleAttrs(m_Doc, attrs, cstyle);
			temp.create(cstyle);
			m_Doc->redefineCharStyles(temp, false);
		}
		if (tagName == "TableStyle")
		{
			TableStyle tstyle;
			readTableStyle(m_Doc, reader, tstyle);
			StyleSet<TableStyle> temp;
			temp.create(tstyle);
			m_Doc->redefineTableStyles(temp, false);
		}
		if (tagName == "CellStyle")
		{
			CellStyle tstyle;
			readCellStyle(m_Doc, reader, tstyle);
			StyleSet<CellStyle> temp;
			temp.create(tstyle);
			m_Doc->redefineCellStyles(temp, false);
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
				m_Doc->docLineStyles.insert(mlName, ml);
			}
		}
		if ((tagName == "ITEM") || (tagName == "PAGEOBJECT") || (tagName == "FRAMEOBJECT"))
		{
			ItemInfo itemInfo;
			success = readObject(m_Doc, reader, readObjectParams, itemInfo);
			if (!success)
				break;
			itemInfo.item->m_layerID = LayerToPaste;
			if (isNewFormat)
			{
				if (itemInfo.nextItem != -1)
					itemNext[itemInfo.itemID] = itemInfo.nextItem;
				if (itemInfo.item->isTableItem)
					TableItems.append(itemInfo.item);
				if (itemInfo.isWeldFlag)
					WeldItems.append(itemInfo.item);
			}
			else
			{
			// first of linked chain?
				if (tagName == "ITEM")
				{
					if (itemInfo.nextItem != -1)
						itemNext[itemInfo.ownNr] = itemInfo.nextItem;
				}
				if (itemInfo.item->isTableItem)
				{
					if (tagName == "ITEM")
					{
						TableItems.append(itemInfo.item);
						TableID.insert(itemInfo.ownLink, itemInfo.item);
					}
				}
				if (itemInfo.isWeldFlag)
				{
					if (tagName == "ITEM")
					{
						WeldItems.append(itemInfo.item);
						WeldID.insert(itemInfo.ownWeld, itemInfo.item);
					}
				}
			}

			if ((tagName == "PAGEOBJECT") && (groupStackPI.count() > 0))
			{
				groupStackPI.top().append(itemInfo.item);
				while (itemInfo.ownNr == groupStackPI2.top())
				{
					groupStackP.push(groupStackPI.pop());
					groupStackPI2.pop();
					if (groupStackPI2.count() == 0)
						break;
				}
			}
			else if ((tagName == "FRAMEOBJECT") && (groupStackFI.count() > 0))
			{
				groupStackFI.top().append(itemInfo.item);
				while (itemInfo.ownNr == groupStackFI2.top())
				{
					groupStackF.push(groupStackFI.pop());
					groupStackFI2.pop();
					if (groupStackFI2.count() == 0)
						break;
				}
			}
			else if ((tagName == "MASTEROBJECT") && (groupStackMI.count() > 0))
			{
				groupStackMI.top().append(itemInfo.item);
				while (itemInfo.ownNr == groupStackMI2.top())
				{
					groupStackM.push(groupStackMI.pop());
					groupStackMI2.pop();
					if (groupStackMI2.count() == 0)
						break;
				}
			}

			if (itemInfo.isGroupFlag)
			{
				QList<PageItem*> groupItems;
				groupItems.append(itemInfo.item);
				if (tagName == "PAGEOBJECT")
				{
					groupStackPI.push(groupItems);
					groupStackPI2.push(itemInfo.groupLastItem + itemInfo.ownNr);
				}
				else if (tagName == "FRAMEOBJECT")
				{
					groupStackFI.push(groupItems);
					groupStackFI2.push(itemInfo.groupLastItem + itemInfo.ownNr);
				}
				else
				{
					groupStackMI.push(groupItems);
					groupStackMI2.push(itemInfo.groupLastItem + itemInfo.ownNr);
				}
			}
		}
		if (tagName == "Pattern")
		{
			success = readPattern(m_Doc, reader, fileDir);
			if (!success) break;
		}
		if (tagName == "NotesStyles")
		{
			success = readNotesStyles(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "NotesFrames")
		{
			success = readNotesFrames(reader);
			if (!success) break;
		}
		if (tagName == "Notes")
		{
			success = readNotes(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "Marks")
		{
			success = readMarks(m_Doc, reader);
			if (!success) break;
		}
	}
	if (reader.hasError())
	{
		setDomParsingError(reader.errorString(), reader.lineNumber(), reader.columnNumber());
		return false;
	}
	if (isNewFormat)
	{
		if (TableItems.count() != 0)
		{
			for (int ttc = 0; ttc < TableItems.count(); ++ttc)
			{
				PageItem* ta = TableItems.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = LinkID[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = LinkID[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = LinkID[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = LinkID[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (WeldItems.count() != 0)
		{
			for (int ttc = 0; ttc < WeldItems.count(); ++ttc)
			{
				PageItem* ta = WeldItems.at(ttc);
				for (int i = 0; i < ta->weldList.count(); ++i)
				{
					PageItem::WeldingInfo wInf = ta->weldList.at(i);
					ta->weldList[i].weldItem   = LinkID.value(wInf.weldID, 0);
					if (ta->weldList[i].weldItem == nullptr)
						ta->weldList.removeAt(i--);
				}
			}
		}

		//update names to pointers
		updateNames2Ptr();

		if (itemNext.count() != 0)
		{
			QMap<int,int>::Iterator lc;
			for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
			{
				if (lc.value() >= 0)
				{
					PageItem * Its = LinkID[lc.key()];
					PageItem * Itn = LinkID[lc.value()];
					if (!Its->canBeLinkedTo(Itn))
					{
						qDebug() << "scribus150format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
				}
			}
		}
	}
	else
	{
		if (TableItemsF.count() != 0)
		{
			for (int ttc = 0; ttc < TableItemsF.count(); ++ttc)
			{
				PageItem* ta = TableItemsF.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = TableIDF[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = TableIDF[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = TableIDF[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = TableIDF[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (TableItemsM.count() != 0)
		{
			for (int ttc = 0; ttc < TableItemsM.count(); ++ttc)
			{
				PageItem* ta = TableItemsM.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = TableIDM[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = TableIDM[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = TableIDM[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = TableIDM[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (TableItems.count() != 0)
		{
			for (int ttc = 0; ttc < TableItems.count(); ++ttc)
			{
				PageItem* ta = TableItems.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = TableID[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = TableID[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = TableID[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = TableID[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (WeldItems.count() != 0)
		{
			for (int ttc = 0; ttc < WeldItems.count(); ++ttc)
			{
				PageItem* ta = WeldItems.at(ttc);
				for (int i = 0 ; i < ta->weldList.count(); ++i)
				{
					PageItem::WeldingInfo wInf = ta->weldList.at(i);
					ta->weldList[i].weldItem = WeldID.value(wInf.weldID, 0);
					if (ta->weldList[i].weldItem == nullptr)
						ta->weldList.removeAt(i--);
				}
			}
		}

		//update names to pointers
		updateNames2Ptr();

		// reestablish textframe links
		if (itemNext.count() != 0)
		{
			QMap<int,int>::Iterator lc;
			for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
			{
				if (lc.value() >= 0)
				{
					PageItem *Its(nullptr), *Itn(nullptr);
					if (lc.key() < m_Doc->DocItems.count())
						Its = m_Doc->DocItems.at(lc.key());
					if (lc.value() < m_Doc->DocItems.count())
						Itn = m_Doc->DocItems.at(lc.value());
					if (!Its || !Itn || !Its->canBeLinkedTo(Itn))
					{
						qDebug() << "scribus150format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
				}
			}
		}
	}

	while (groupStackP.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackP.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->DocItems.removeOne(cItem);
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackP, &m_Doc->DocItems);
		if (!converted)
			gItem->groupItemList = gpL;
	}

	while (groupStackF.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackF.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->FrameItems.remove(m_Doc->FrameItems.key(cItem));
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackF, nullptr);
		if (!converted)
			gItem->groupItemList = gpL;
	}

	while (groupStackM.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackM.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->MasterItems.removeOne(cItem);
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackM, &m_Doc->MasterItems);
		if (!converted)
			gItem->groupItemList = gpL;
	}

	return true;
}

bool Scribus150Format::loadPalette(const QString & fileName)
{
	if (m_Doc==nullptr || m_AvailableFonts==nullptr)
	{
		Q_ASSERT(m_Doc==nullptr || m_AvailableFonts==nullptr);
		return false;
	}
	ParagraphStyle vg;
	Xp = 0.0;
	Yp = 0.0;
	GrX = 0.0;
	GrY = 0.0;
	isNewFormat = false;

	QMap<int,PageItem*> TableID;
	QMap<int,PageItem*> TableIDM;
	QMap<int,PageItem*> TableIDF;
	QList<PageItem*> TableItems;
	QList<PageItem*> TableItemsM;
	QList<PageItem*> TableItemsF;
	QMap<int,PageItem*> WeldID;
	QList<PageItem*> WeldItems;
	QStack< QList<PageItem*> > groupStackFI;
	QStack< QList<PageItem*> > groupStackMI;
	QStack< QList<PageItem*> > groupStackPI;
	QStack< QList<PageItem*> > groupStackF;
	QStack< QList<PageItem*> > groupStackM;
	QStack< QList<PageItem*> > groupStackP;
	QStack<int> groupStackFI2;
	QStack<int> groupStackMI2;
	QStack<int> groupStackPI2;

	QByteArray docBytes("");
	loadRawText(fileName, docBytes);
	QString f = QString::fromUtf8(docBytes);
	if (f.isEmpty())
	{
		setFileReadError();
		return false;
	}
	QString fileDir = QFileInfo(fileName).absolutePath();

	if (m_mwProgressBar!=nullptr)
	{
		m_mwProgressBar->setMaximum(f.length());
		m_mwProgressBar->setValue(0);
	}

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
	WeldItems.clear();

	m_Doc->GroupCounter = 1;
	m_Doc->LastAuto = nullptr;
//	m_Doc->PageColors.clear();
//	m_Doc->Layers.clear();

	bool firstElement = true;
	bool success = true;
	int  progress = 0;

	ReadObjectParams readObjectParams;
	readObjectParams.baseDir = fileDir;
	readObjectParams.itemKind = PageItem::StandardItem;
	readObjectParams.loadingPage = false;

	ScXmlStreamReader reader(f);
	ScXmlStreamAttributes attrs;
	while (!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (tType != QXmlStreamReader::StartElement)
			continue;
		QStringRef tagName = reader.name();
		attrs = reader.scAttributes();

		if (m_mwProgressBar != nullptr)
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
			if (tagName != "SCRIBUSCOLORS")
			{
				success = false;
				break;
			}
			firstElement = false;
		}
		// 10/25/2004 pv - None is "reserved" color. cannot be defined in any file...
		if (tagName == "COLOR" && attrs.valueAsString("NAME") != CommonStrings::None)
			readColor(m_Doc->PageColors, attrs);
		if (tagName == "Gradient")
		{
			VGradient gra;
			QString grName = attrs.valueAsString("Name");
			success = readGradient(m_Doc, gra, reader);
			if (!success)
				break;
			gra.setRepeatMethod((VGradient::VGradientRepeatMethod)(attrs.valueAsInt("Ext", VGradient::pad)));
			if (!grName.isEmpty())
			{
				m_Doc->docGradients.insert(grName, gra);
			}
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
				m_Doc->docLineStyles.insert(mlName, ml);
			}
		}
		if (tagName == "PAGEOBJECT" || tagName == "MASTEROBJECT" || tagName == "FRAMEOBJECT")
		{
			ItemInfo itemInfo;
			success = readObject(m_Doc, reader, readObjectParams, itemInfo);
			if (!success) break;

			if (isNewFormat)
			{
				if (itemInfo.nextItem != -1)
					itemNext[itemInfo.itemID] = itemInfo.nextItem;
				if (itemInfo.item->isTableItem)
					TableItems.append(itemInfo.item);
				if (itemInfo.isWeldFlag)
					WeldItems.append(itemInfo.item);
			}
			else
			{
				// first of linked chain?
				if (tagName == "PAGEOBJECT")
				{
					if (itemInfo.nextItem != -1)
						itemNext[itemInfo.ownNr] = itemInfo.nextItem;
				}
				else if (tagName == "MASTEROBJECT")
				{
					if (itemInfo.nextItem != -1)
						itemNextM[itemInfo.ownNr] = itemInfo.nextItem;
				}
				if (itemInfo.item->isTableItem)
				{
					if (tagName == "PAGEOBJECT")
					{
						TableItems.append(itemInfo.item);
						TableID.insert(itemInfo.ownLink, itemInfo.item);
					}
					else if (tagName == "FRAMEOBJECT")
					{
						TableItemsF.append(itemInfo.item);
						TableIDF.insert(itemInfo.ownLink, itemInfo.item);
					}
					else
					{
						TableItemsM.append(itemInfo.item);
						TableIDM.insert(itemInfo.ownLink, itemInfo.item);
					}
				}
				if (itemInfo.isWeldFlag)
				{
					WeldItems.append(itemInfo.item);
					WeldID.insert(itemInfo.ownWeld, itemInfo.item);
				}
			}

			if ((tagName == "PAGEOBJECT") && (groupStackPI.count() > 0))
			{
				groupStackPI.top().append(itemInfo.item);
				while (itemInfo.ownNr == groupStackPI2.top())
				{
					groupStackP.push(groupStackPI.pop());
					groupStackPI2.pop();
					if (groupStackPI2.count() == 0)
						break;
				}
			}
			else if ((tagName == "FRAMEOBJECT") && (groupStackFI.count() > 0))
			{
				groupStackFI.top().append(itemInfo.item);
				while (itemInfo.ownNr == groupStackFI2.top())
				{
					groupStackF.push(groupStackFI.pop());
					groupStackFI2.pop();
					if (groupStackFI2.count() == 0)
						break;
				}
			}
			else if ((tagName == "MASTEROBJECT") && (groupStackMI.count() > 0))
			{
				groupStackMI.top().append(itemInfo.item);
				while (itemInfo.ownNr == groupStackMI2.top())
				{
					groupStackM.push(groupStackMI.pop());
					groupStackMI2.pop();
					if (groupStackMI2.count() == 0)
						break;
				}
			}

			if (itemInfo.isGroupFlag)
			{
				QList<PageItem*> groupItems;
				groupItems.append(itemInfo.item);
				if (tagName == "PAGEOBJECT")
				{
					groupStackPI.push(groupItems);
					groupStackPI2.push(itemInfo.groupLastItem + itemInfo.ownNr);
				}
				else if (tagName == "FRAMEOBJECT")
				{
					groupStackFI.push(groupItems);
					groupStackFI2.push(itemInfo.groupLastItem + itemInfo.ownNr);
				}
				else
				{
					groupStackMI.push(groupItems);
					groupStackMI2.push(itemInfo.groupLastItem + itemInfo.ownNr);
				}
			}
		}
		if (tagName == "Pattern")
		{
			success = readPattern(m_Doc, reader, fileDir);
			if (!success) break;
		}
		if (tagName == "NotesStyles")
		{
			success = readNotesStyles(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "NotesFrames")
		{
			success = readNotesFrames(reader);
			if (!success) break;
		}
		if (tagName == "Notes")
		{
			success = readNotes(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "Marks")
		{
			success = readMarks(m_Doc, reader);
			if (!success) break;
		}
	}

	if (reader.hasError())
	{
		setDomParsingError(reader.errorString(), reader.lineNumber(), reader.columnNumber());
		return false;
	}
	if (isNewFormat)
	{
		if (TableItems.count() != 0)
		{
			for (int ttc = 0; ttc < TableItems.count(); ++ttc)
			{
				PageItem* ta = TableItems.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = LinkID[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = LinkID[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = LinkID[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = LinkID[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (WeldItems.count() != 0)
		{
			for (int ttc = 0; ttc < WeldItems.count(); ++ttc)
			{
				PageItem* ta = WeldItems.at(ttc);
				for (int i = 0 ; i < ta->weldList.count(); ++i)
				{
					PageItem::WeldingInfo wInf = ta->weldList.at(i);
					ta->weldList[i].weldItem = LinkID.value(wInf.weldID, 0);
					if (ta->weldList[i].weldItem == nullptr)
						ta->weldList.removeAt(i--);
				}
			}
		}
		if (itemNext.count() != 0)
		{
			QMap<int,int>::Iterator lc;
			for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
			{
				if (lc.value() >= 0)
				{
					PageItem * Its = LinkID[lc.key()];
					PageItem * Itn = LinkID[lc.value()];
					if (!Its->canBeLinkedTo(Itn))
					{
						qDebug() << "scribus150format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
				}
			}
		}
	}
	else
	{
		if (TableItemsF.count() != 0)
		{
			for (int ttc = 0; ttc < TableItemsF.count(); ++ttc)
			{
				PageItem* ta = TableItemsF.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = TableIDF[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = TableIDF[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = TableIDF[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = TableIDF[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (TableItemsM.count() != 0)
		{
			for (int ttc = 0; ttc < TableItemsM.count(); ++ttc)
			{
				PageItem* ta = TableItemsM.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = TableIDM[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = TableIDM[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = TableIDM[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = TableIDM[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (TableItems.count() != 0)
		{
			for (int ttc = 0; ttc < TableItems.count(); ++ttc)
			{
				PageItem* ta = TableItems.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = TableID[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = TableID[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = TableID[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = TableID[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (WeldItems.count() != 0)
		{
			for (int ttc = 0; ttc < WeldItems.count(); ++ttc)
			{
				PageItem* ta = WeldItems.at(ttc);
				for (int i = 0 ; i < ta->weldList.count(); ++i)
				{
					PageItem::WeldingInfo wInf = ta->weldList.at(i);
					ta->weldList[i].weldItem = WeldID.value(wInf.weldID, 0);
					if (ta->weldList[i].weldItem == nullptr)
						ta->weldList.removeAt(i--);
				}
			}
		}
		// reestablish textframe links
		if (itemNext.count() != 0)
		{
			QMap<int,int>::Iterator lc;
			for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
			{
				if (lc.value() >= 0)
				{
					PageItem *Its(nullptr), *Itn(nullptr);
					if (lc.key() < m_Doc->DocItems.count())
						Its = m_Doc->DocItems.at(lc.key());
					if (lc.value() < m_Doc->DocItems.count())
						Itn = m_Doc->DocItems.at(lc.value());
					if (!Its || !Itn || !Its->canBeLinkedTo(Itn))
					{
						qDebug() << "scribus150format: corruption in linked textframes detected";
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
					PageItem *Its(nullptr), *Itn(nullptr);
					if (lc.key() < m_Doc->MasterItems.count())
						Its = m_Doc->MasterItems.at(lc.key());
					if (lc.value() < m_Doc->MasterItems.count())
						Itn = m_Doc->MasterItems.at(lc.value());
					if (!Its || !Itn || !Its->canBeLinkedTo(Itn))
					{
						qDebug() << "scribus150format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
				}
			}
		}
	}

	while (groupStackP.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackP.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->DocItems.removeOne(cItem);
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackP, &m_Doc->DocItems);
		if (!converted)
			gItem->groupItemList = gpL;
	}

	while (groupStackF.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackF.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->FrameItems.remove(m_Doc->FrameItems.key(cItem));
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackF, nullptr);
		if (!converted)
			gItem->groupItemList = gpL;
	}

	while (groupStackM.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackM.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->MasterItems.removeOne(cItem);
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackM, &m_Doc->MasterItems);
		if (!converted)
			gItem->groupItemList = gpL;
	}

	if (m_Doc->Layers.count() == 0)
		m_Doc->Layers.newLayer( QObject::tr("Background") );
	if (m_mwProgressBar!=nullptr)
	{
		m_mwProgressBar->setValue(reader.characterOffset());
		m_mwProgressBar->reset();
	}
	return true;
}

bool Scribus150Format::loadFile(const QString & fileName, const FileFormat & /* fmt */, int /* flags */, int /* index */)
{
	if (m_Doc==nullptr || m_AvailableFonts==nullptr)
	{
		Q_ASSERT(m_Doc==nullptr || m_AvailableFonts==nullptr);
		return false;
	}
	ParagraphStyle vg;
	Xp = 0.0;
	Yp = 0.0;
	GrX = 0.0;
	GrY = 0.0;
	struct ScribusDoc::BookMa bok;
	QMap<int, ScribusDoc::BookMa> bookmarks;

	isNewFormat = false;

	QMap<int,PageItem*> TableID;
	QMap<int,PageItem*> TableIDM;
	QMap<int,PageItem*> TableIDF;
	QList<PageItem*> TableItems;
	QList<PageItem*> TableItemsM;
	QList<PageItem*> TableItemsF;
	QMap<int,PageItem*> WeldID;
	QList<PageItem*> WeldItems;
	QStack< QList<PageItem*> > groupStackFI;
	QStack< QList<PageItem*> > groupStackMI;
	QStack< QList<PageItem*> > groupStackPI;
	QStack< QList<PageItem*> > groupStackF;
	QStack< QList<PageItem*> > groupStackM;
	QStack< QList<PageItem*> > groupStackP;
	QStack<int> groupStackFI2;
	QStack<int> groupStackMI2;
	QStack<int> groupStackPI2;
	
	markeredItemsMap.clear();
	markeredMarksMap.clear();
	nsetRangeItemNamesMap.clear();
	notesFramesData.clear();
	notesMasterMarks.clear();
	notesNSets.clear();

	QScopedPointer<QIODevice> ioDevice(slaReader(fileName));
	if (ioDevice.isNull())
	{
		setFileReadError();
		return false;
	}
	QString fileDir = QFileInfo(fileName).absolutePath();
	int firstPage = 0;
	int layerToSetActive = 0;
	
	if (m_mwProgressBar!=nullptr)
	{
		m_mwProgressBar->setMaximum(ioDevice->size());
		m_mwProgressBar->setValue(0);
	}
	// Stop autosave timer,it will be restarted only if doc has autosave feature is enabled
	if (m_Doc->autoSaveTimer->isActive())
		m_Doc->autoSaveTimer->stop();

	itemRemap.clear();
	itemNext.clear();
	itemCount = 0;
	itemRemapM.clear();
	itemNextM.clear();
	itemCountM = 0;
	itemRemapF.clear();
	itemNextF.clear();

	FrameItems.clear();
	TableItems.clear();
	TableID.clear();
	TableItemsM.clear();
	TableIDM.clear();
	TableItemsF.clear();
	TableIDF.clear();
	WeldItems.clear();
	WeldID.clear();
	LinkID.clear();

	m_Doc->GroupCounter = 1;
	m_Doc->LastAuto = nullptr;
	m_Doc->PageColors.clear();
	m_Doc->Layers.clear();

	ReadObjectParams readObjectParams;
	readObjectParams.baseDir = fileDir;
	readObjectParams.itemKind = PageItem::StandardItem;
	readObjectParams.loadingPage = false;

	bool firstElement = true;
	bool success = true;
	bool hasPageSets = false;
	int  progress = 0;

	ScXmlStreamReader reader(ioDevice.data());
	ScXmlStreamAttributes attrs;
	while (!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (tType != QXmlStreamReader::StartElement)
			continue;
		QStringRef tagName = reader.name();
		attrs = reader.scAttributes();

		if (m_mwProgressBar != nullptr)
		{
			int newProgress = qRound(ioDevice->pos() / (double) ioDevice->size() * 100);
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
			readColor(m_Doc->PageColors, attrs);
		if (tagName == "Gradient")
		{
			VGradient gra;
			QString grName = attrs.valueAsString("Name");
			success = readGradient(m_Doc, gra, reader);
			if (!success)
				break;
			gra.setRepeatMethod((VGradient::VGradientRepeatMethod)(attrs.valueAsInt("Ext", VGradient::pad)));
			if (!grName.isEmpty())
				m_Doc->docGradients.insert(grName, gra);
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
		if (tagName == "TableStyle")
		{
			TableStyle tstyle;
			readTableStyle(m_Doc, reader, tstyle);
			StyleSet<TableStyle> temp;
			temp.create(tstyle);
			m_Doc->redefineTableStyles(temp, false);
		}
		if (tagName == "CellStyle")
		{
			CellStyle tstyle;
			readCellStyle(m_Doc, reader, tstyle);
			StyleSet<CellStyle> temp;
			temp.create(tstyle);
			m_Doc->redefineCellStyles(temp, false);
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
				m_Doc->docLineStyles.insert(mlName, ml);
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
			if (!success)
				break;
		}
		if (tagName == "PAGEOBJECT" || tagName == "MASTEROBJECT" || tagName == "FRAMEOBJECT")
		{
			ItemInfo itemInfo;
			success = readObject(m_Doc, reader, readObjectParams, itemInfo);
			if (!success)
				break;

			// first of linked chain?
			if (isNewFormat)
			{
				if (itemInfo.nextItem != -1)
					itemNext[itemInfo.itemID] = itemInfo.nextItem;
				if (itemInfo.item->isTableItem)
					TableItems.append(itemInfo.item);
				if (itemInfo.isWeldFlag)
					WeldItems.append(itemInfo.item);
			}
			else
			{
				if (tagName == "PAGEOBJECT")
				{
					if (itemInfo.nextItem != -1)
						itemNext[itemInfo.ownNr] = itemInfo.nextItem;
				}
				else if (tagName == "MASTEROBJECT")
				{
					if (itemInfo.nextItem != -1)
						itemNextM[itemInfo.ownNr] = itemInfo.nextItem;
				}
				if (itemInfo.item->isTableItem)
				{
					if (tagName == "PAGEOBJECT")
					{
						TableItems.append(itemInfo.item);
						TableID.insert(itemInfo.ownLink, itemInfo.item);
					}
					else if (tagName == "FRAMEOBJECT")
					{
						TableItemsF.append(itemInfo.item);
						TableIDF.insert(itemInfo.ownLink, itemInfo.item);
					}
					else
					{
						TableItemsM.append(itemInfo.item);
						TableIDM.insert(itemInfo.ownLink, itemInfo.item);
					}
				}

				if (itemInfo.isWeldFlag)
				{
					WeldItems.append(itemInfo.item);
					WeldID.insert(itemInfo.ownWeld, itemInfo.item);
				}
			}

			if ((tagName == "PAGEOBJECT") && (groupStackPI.count() > 0))
			{
				groupStackPI.top().append(itemInfo.item);
				while (itemInfo.ownNr == groupStackPI2.top())
				{
					groupStackP.push(groupStackPI.pop());
					groupStackPI2.pop();
					if (groupStackPI2.count() == 0)
						break;
				}
			}
			else if ((tagName == "FRAMEOBJECT") && (groupStackFI.count() > 0))
			{
				groupStackFI.top().append(itemInfo.item);
				while (itemInfo.ownNr == groupStackFI2.top())
				{
					groupStackF.push(groupStackFI.pop());
					groupStackFI2.pop();
					if (groupStackFI2.count() == 0)
						break;
				}
			}
			else if ((tagName == "MASTEROBJECT") && (groupStackMI.count() > 0))
			{
				groupStackMI.top().append(itemInfo.item);
				while (itemInfo.ownNr == groupStackMI2.top())
				{
					groupStackM.push(groupStackMI.pop());
					groupStackMI2.pop();
					if (groupStackMI2.count() == 0)
						break;
				}
			}

			if (itemInfo.isGroupFlag)
			{
				QList<PageItem*> groupItems;
				groupItems.append(itemInfo.item);
				if (tagName == "PAGEOBJECT")
				{
					groupStackPI.push(groupItems);
					groupStackPI2.push(itemInfo.groupLastItem + itemInfo.ownNr);
				}
				else if (tagName == "FRAMEOBJECT")
				{
					groupStackFI.push(groupItems);
					groupStackFI2.push(itemInfo.groupLastItem + itemInfo.ownNr);
				}
				else
				{
					groupStackMI.push(groupItems);
					groupStackMI2.push(itemInfo.groupLastItem + itemInfo.ownNr);
				}
			}
		}
		if (tagName == "Pattern")
		{
			success = readPattern(m_Doc, reader, fileDir);
			if (!success)
				break;
		}
		if (tagName == "NotesStyles")
		{
			success = readNotesStyles(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "NotesFrames")
		{
			success = readNotesFrames(reader);
			if (!success) break;
		}
		if (tagName == "Notes")
		{
			success = readNotes(m_Doc, reader);
			if (!success) break;
		}
		if (tagName == "Marks")
		{
			success = readMarks(m_Doc, reader);
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
		PageItem* item = LinkID.value(elem, (PageItem*) nullptr);
		if (!item)
			continue;
		ScribusDoc::BookMa bookmark = it.value();
		bookmark.PageObject = item;
		m_Doc->BookMarks.append( bookmark );
	}
	std::stable_sort(m_Doc->BookMarks.begin(), m_Doc->BookMarks.end());

	if (isNewFormat)
	{
		if (TableItems.count() != 0)
		{
			for (int ttc = 0; ttc < TableItems.count(); ++ttc)
			{
				PageItem* ta = TableItems.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = LinkID[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = LinkID[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = LinkID[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = LinkID[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (WeldItems.count() != 0)
		{
			for (int ttc = 0; ttc < WeldItems.count(); ++ttc)
			{
				PageItem* ta = WeldItems.at(ttc);
				for (int i = 0 ; i < ta->weldList.count(); ++i)
				{
					PageItem::WeldingInfo wInf = ta->weldList.at(i);
					ta->weldList[i].weldItem = LinkID.value(wInf.weldID, 0);
					if (ta->weldList[i].weldItem == nullptr)
						ta->weldList.removeAt(i--);
				}
			}
		}
		if (itemNext.count() != 0)
		{
			QMap<int,int>::Iterator lc;
			for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
			{
				if (lc.value() >= 0)
				{
					PageItem * Its = LinkID[lc.key()];
					PageItem * Itn = LinkID[lc.value()];
					if (!Its->canBeLinkedTo(Itn))
					{
						qDebug() << "scribus150format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
				}
			}
		}
	}
	else
	{
		if (TableItemsF.count() != 0)
		{
			for (int ttc = 0; ttc < TableItemsF.count(); ++ttc)
			{
				PageItem* ta = TableItemsF.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = TableIDF[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = TableIDF[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = TableIDF[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = TableIDF[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (TableItemsM.count() != 0)
		{
			for (int ttc = 0; ttc < TableItemsM.count(); ++ttc)
			{
				PageItem* ta = TableItemsM.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = TableIDM[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = TableIDM[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = TableIDM[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = TableIDM[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (TableItems.count() != 0)
		{
			for (int ttc = 0; ttc < TableItems.count(); ++ttc)
			{
				PageItem* ta = TableItems.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = TableID[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = TableID[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = TableID[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = TableID[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (WeldItems.count() != 0)
		{
			for (int ttc = 0; ttc < WeldItems.count(); ++ttc)
			{
				PageItem* ta = WeldItems.at(ttc);
				for (int i = 0 ; i < ta->weldList.count(); ++i)
				{
					PageItem::WeldingInfo wInf = ta->weldList.at(i);
					ta->weldList[i].weldItem = WeldID.value(wInf.weldID, 0);
					if (ta->weldList[i].weldItem == nullptr)
						ta->weldList.removeAt(i--);
				}
			}
		}
		if (itemNext.count() != 0)
		{
			QMap<int,int>::Iterator lc;
			for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
			{
				if (lc.value() >= 0)
				{
					PageItem *Its(nullptr), *Itn(nullptr);
					if (lc.key() < m_Doc->DocItems.count())
						Its = m_Doc->DocItems.at(lc.key());
					if (lc.value() < m_Doc->DocItems.count())
						Itn = m_Doc->DocItems.at(lc.value());
					if (!Its || !Itn || !Its->canBeLinkedTo(Itn))
					{
						qDebug() << "scribus150format: corruption in linked textframes detected";
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
					PageItem *Its(nullptr), *Itn(nullptr);
					if (lc.key() < m_Doc->MasterItems.count())
						Its = m_Doc->MasterItems.at(lc.key());
					if (lc.value() < m_Doc->MasterItems.count())
						Itn = m_Doc->MasterItems.at(lc.value());
					if (!Its || !Itn || !Its->canBeLinkedTo(Itn))
					{
						qDebug() << "scribus150format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
				}
			}
		}
	}
	//CB Add this in to set this in the file in memory. Its saved, why not load it.
	//Will of course be replaced by per page settings although we still probably need a document default
	if (!hasPageSets)
	{
		m_Doc->setPageSetFirstPage(m_Doc->pagePositioning(), firstPage);
//->Prefs		m_Doc->pageSets[m_Doc->currentPageLayout].FirstPage = firstPage;
//		m_Doc->pageSets[m_Doc->currentPageLayout].GapHorizontal = dc.attribute("GapHorizontal", "0").toDouble();
//		m_Doc->pageSets[m_Doc->currentPageLayout].GapVertical = 0.0;
//		m_Doc->pageSets[m_Doc->currentPageLayout].GapBelow = dc.attribute("GapVertical", "40").toDouble();
	}
	m_Doc->setActiveLayer(layerToSetActive);
	m_Doc->setMasterPageMode(false);
	m_Doc->reformPages();
	m_Doc->refreshGuides();

	// #12282 : some docs have language dependent style names specified in style properties
	// #14129 : some others reference deleted character styles
	m_Doc->fixCharacterStyles();
	m_Doc->fixParagraphStyles();
	m_Doc->fixNotesStyles();

	// #14603 : it seems we need this also for some 1.5.x docs
	m_Doc->fixItemPageOwner();

	if (m_Doc->Layers.count() == 0)
		m_Doc->Layers.newLayer( QObject::tr("Background") );
	if (!EffVal.isEmpty())
	{
		for (int pdoE = 0; pdoE < EffVal.count(); ++pdoE)
		{
			if (pdoE < m_Doc->Pages->count())
				m_Doc->Pages->at(pdoE)->PresentVals = EffVal[pdoE];
		}
	}

	while (groupStackP.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackP.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->DocItems.removeOne(cItem);
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackP, &m_Doc->DocItems);
		if (!converted)
			gItem->groupItemList = gpL;
	}

	while (groupStackF.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackF.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->FrameItems.remove(m_Doc->FrameItems.key(cItem));
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackF, nullptr);
		if (!converted)
			gItem->groupItemList = gpL;
	}

	while (groupStackM.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackM.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->MasterItems.removeOne(cItem);
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackM, &m_Doc->MasterItems);
		if (!converted)
			gItem->groupItemList = gpL;
	}

	//update names to pointers
	updateNames2Ptr();

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
	if (m_Doc->autoSave() && ScCore->usingGUI())
		m_Doc->restartAutoSaveTimer();
//	m_Doc->autoSaveTimer->start(m_Doc->autoSaveTime());

	if (m_mwProgressBar!=nullptr)
		m_mwProgressBar->setValue(reader.characterOffset());
	return true;
}

// Low level plugin API
int scribus150format_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* scribus150format_getPlugin()
{
	Scribus150Format* plug = new Scribus150Format();
	Q_CHECK_PTR(plug);
	return plug;
}

void scribus150format_freePlugin(ScPlugin* plugin)
{
	Scribus150Format* plug = dynamic_cast<Scribus150Format*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}


namespace {
	const int NOVALUE = -16000;

	void fixLegacyCharStyle(CharStyle& cstyle) 
	{
		if (! cstyle.font().usable())
			cstyle.resetFont();
		if (cstyle.fontSize() <= NOVALUE / 10)
			cstyle.resetFontSize();
//		if (cstyle.effects() == 65535)
//			cstyle.resetEffects();
		if (cstyle.fillColor().isEmpty())
			cstyle.resetFillColor();
		if (cstyle.fillShade() <= NOVALUE)
			cstyle.resetFillShade();
		if (cstyle.strokeColor().isEmpty())
			cstyle.resetStrokeColor();
		if (cstyle.strokeShade() <= NOVALUE)
			cstyle.resetStrokeShade();
		if (cstyle.shadowXOffset() <= NOVALUE / 10)
			cstyle.resetShadowXOffset();
		if (cstyle.shadowYOffset() <= NOVALUE / 10)
			cstyle.resetShadowYOffset();
		if (cstyle.outlineWidth() <= NOVALUE / 10)
			cstyle.resetOutlineWidth();
		if (cstyle.underlineOffset() <= NOVALUE / 10)
			cstyle.resetUnderlineOffset();
		if (cstyle.underlineWidth() <= NOVALUE / 10)
			cstyle.resetUnderlineWidth();
		if (cstyle.strikethruOffset() <= NOVALUE / 10)
			cstyle.resetStrikethruOffset();
		if (cstyle.strikethruWidth() <= NOVALUE / 10)
			cstyle.resetStrikethruWidth();
		if (cstyle.scaleH() <= NOVALUE / 10)
			cstyle.resetScaleH();
		if (cstyle.scaleV() <= NOVALUE / 10)
			cstyle.resetScaleV();
		if (cstyle.baselineOffset() <= NOVALUE / 10)
			cstyle.resetBaselineOffset();
		if (cstyle.tracking() <= NOVALUE / 10)
			cstyle.resetTracking();
	}

	void fixLegacyParStyle(ParagraphStyle& pstyle) 
	{
		if (pstyle.lineSpacing() <= NOVALUE)
			pstyle.resetLineSpacing();
		if (pstyle.leftMargin() <= NOVALUE)
			pstyle.resetLeftMargin();
		if (pstyle.rightMargin() <= NOVALUE)
			pstyle.resetRightMargin();
		if (pstyle.firstIndent() <= NOVALUE)
			pstyle.resetFirstIndent();
		if (pstyle.alignment() < 0)
			pstyle.resetAlignment();
		if (pstyle.gapBefore() <= NOVALUE)
			pstyle.resetGapBefore();
		if (pstyle.gapAfter() <= NOVALUE)
			pstyle.resetGapAfter();
		if (pstyle.dropCapLines() < 0)
			pstyle.resetDropCapLines();
		if (pstyle.parEffectOffset() <= NOVALUE)
			pstyle.resetParEffectOffset();
		fixLegacyCharStyle(pstyle.charStyle());
	}

}// namespace

void Scribus150Format::readDocAttributes(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	m_Doc->setPageSize(attrs.valueAsString("PAGESIZE"));
	m_Doc->setPageOrientation(attrs.valueAsInt("ORIENTATION", 0));
	m_Doc->FirstPnum  = attrs.valueAsInt("FIRSTNUM", 1);
	m_Doc->setPagePositioning(attrs.valueAsInt("BOOK", 0));

	m_Doc->setUsesAutomaticTextFrames( attrs.valueAsInt("AUTOTEXT") );
	m_Doc->PageSp  = attrs.valueAsInt("AUTOSPALTEN");
	m_Doc->PageSpa = attrs.valueAsDouble("ABSTSPALTEN");
	m_Doc->setUnitIndex( attrs.valueAsInt("UNITS", 0) );

	static const QString LANGUAGE("LANGUAGE");
	if (attrs.hasAttribute(LANGUAGE))
	{
		QString l(attrs.valueAsString(LANGUAGE));
		if (LanguageManager::instance()->langTableIndex(l) != -1)
			m_Doc->setLanguage(l); //new style storage
		else
		{ //old style storage
			QString lnew = LanguageManager::instance()->getAbbrevFromLang(l, false);
			if (lnew.isEmpty())
				lnew = LanguageManager::instance()->getAbbrevFromLang(l, false);
			m_Doc->setLanguage(lnew);
		}
	}

	if (attrs.hasAttribute("PAGEWIDTH"))
		m_Doc->setPageWidth(attrs.valueAsDouble("PAGEWIDTH"));
	else
		m_Doc->setPageWidth(attrs.valueAsDouble("PAGEWITH"));
	m_Doc->setPageHeight(attrs.valueAsDouble("PAGEHEIGHT"));
	m_Doc->margins()->setLeft(qMax(0.0, attrs.valueAsDouble("BORDERLEFT")));
	m_Doc->margins()->setRight(qMax(0.0, attrs.valueAsDouble("BORDERRIGHT")));
	m_Doc->margins()->setTop(qMax(0.0, attrs.valueAsDouble("BORDERTOP")));
	m_Doc->margins()->setBottom(qMax(0.0, attrs.valueAsDouble("BORDERBOTTOM")));
	m_Doc->setMarginPreset(attrs.valueAsInt("PRESET", 0));
	m_Doc->bleeds()->setTop(attrs.valueAsDouble("BleedTop", 0.0));
	m_Doc->bleeds()->setLeft(attrs.valueAsDouble("BleedLeft", 0.0));
	m_Doc->bleeds()->setRight(attrs.valueAsDouble("BleedRight", 0.0));
	m_Doc->bleeds()->setBottom(attrs.valueAsDouble("BleedBottom", 0.0));
	m_Doc->setHyphAutomatic(attrs.valueAsBool("AUTOMATIC", true));
	m_Doc->setHyphAutoCheck(attrs.valueAsBool("AUTOCHECK", false));
	m_Doc->GuideLock = attrs.valueAsBool("GUIDELOCK", false);

	m_Doc->rulerXoffset = attrs.valueAsDouble("rulerXoffset", 0.0);
	m_Doc->rulerYoffset = attrs.valueAsDouble("rulerYoffset", 0.0);
	m_Doc->SnapGuides   = attrs.valueAsBool("SnapToGuides", false);
	m_Doc->SnapElement  = attrs.valueAsBool("SnapToElement", false);
	m_Doc->SnapGrid     = attrs.valueAsBool("SnapToGrid", false);

	m_Doc->setAutoSave(attrs.valueAsBool("AutoSave", false));
	m_Doc->setAutoSaveTime(attrs.valueAsInt("AutoSaveTime", 600000));
	m_Doc->setAutoSaveKeep(attrs.valueAsBool("AutoSaveKeep", false));
	m_Doc->setAutoSaveCount(attrs.valueAsInt("AutoSaveCount", 1));
	m_Doc->setAutoSaveInDocDir(attrs.valueAsBool("AUtoSaveInDocDir", true));
	m_Doc->setAutoSaveDir(attrs.valueAsString("AutoSaveDir", ""));
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
		//->Prefs m_Doc->papColor = QColor(attrs.valueAsString("PAGEC"));

	m_Doc->setMarginColored(attrs.valueAsBool("RANDF", false));

	readCMSSettings(doc, attrs);
	readDocumentInfo(doc, attrs);
	readGuideSettings(doc, attrs);
	readToolSettings(doc, attrs);
	readTypographicSettings(doc, attrs);
}

void Scribus150Format::readCMSSettings(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	doc->cmsSettings().SoftProofOn     = attrs.valueAsBool("DPSo", false);
	doc->cmsSettings().SoftProofFullOn = attrs.valueAsBool("DPSFo", false);
	doc->cmsSettings().CMSinUse   = attrs.valueAsBool("DPuse", false);
	doc->cmsSettings().GamutCheck = attrs.valueAsBool("DPgam", false);
	doc->cmsSettings().BlackPoint = attrs.valueAsBool("DPbla", true);
	doc->cmsSettings().DefaultMonitorProfile   = PrefsManager::instance().appPrefs.colorPrefs.DCMSset.DefaultMonitorProfile;
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

void Scribus150Format::readDocumentInfo(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
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

void Scribus150Format::readGuideSettings(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	PrefsManager& prefsManager = PrefsManager::instance();
	doc->guidesPrefs().minorGridSpacing = attrs.valueAsDouble("MINGRID", prefsManager.appPrefs.guidesPrefs.minorGridSpacing);
	doc->guidesPrefs().majorGridSpacing = attrs.valueAsDouble("MAJGRID", prefsManager.appPrefs.guidesPrefs.majorGridSpacing);
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
	m_Doc->drawAsPreview		    = false /*attrs.valueAsBool("previewMode", false)*/;
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
	if (attrs.hasAttribute("BACKG"))
	{
		doc->guidesPrefs().renderStackOrder.clear();
		if (attrs.valueAsBool("BACKG", true))
			doc->guidesPrefs().renderStackOrder << 0 << 1 << 2 << 3 << 4;
		else
			doc->guidesPrefs().renderStackOrder << 4 << 0 << 1 << 2 << 3;
	}
	if (attrs.hasAttribute("renderStack"))
	{
		doc->guidesPrefs().renderStackOrder.clear();
		QString renderStack = attrs.valueAsString("renderStack", "0 1 2 3 4");
		ScTextStream fp(&renderStack, QIODevice::ReadOnly);
		QString val;
		while (!fp.atEnd())
		{
			fp >> val;
			doc->guidesPrefs().renderStackOrder << val.toInt();
		}
	}
	doc->guidesPrefs().gridType = attrs.valueAsInt("GridType", 0);
	doc->guidesPrefs().guideRad = attrs.valueAsDouble("GuideRad", 10.0);
	doc->guidesPrefs().grabRadius  = attrs.valueAsInt("GRAB", 4);
}

void Scribus150Format::readToolSettings(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	const ItemToolPrefs& defToolPrefs = PrefsManager::instance().appPrefs.itemToolPrefs;

	QString textFont = attrs.valueAsString("DFONT");
	m_AvailableFonts->findFont(textFont, doc);

	doc->itemToolPrefs().textFont = textFont;
	doc->itemToolPrefs().textSize = qRound(attrs.valueAsDouble("DSIZE", 12.0) * 10);
	doc->itemToolPrefs().textColumns   = attrs.valueAsInt("DCOL", 1);
	doc->itemToolPrefs().textColumnGap    = attrs.valueAsDouble("DGAP", 0.0);

	const MarginStruct& defDistances = defToolPrefs.textDistances;
	doc->itemToolPrefs().textDistances.setLeft(attrs.valueAsDouble("TextDistLeft", defDistances.left()));
	doc->itemToolPrefs().textDistances.setRight(attrs.valueAsDouble("TextDistRight", defDistances.right()));
	doc->itemToolPrefs().textDistances.setBottom(attrs.valueAsDouble("TextDistBottom", defDistances.bottom()));
	doc->itemToolPrefs().textDistances.setTop(attrs.valueAsDouble("TextDistTop", defDistances.top()));

	doc->itemToolPrefs().polyCorners      = attrs.valueAsInt("POLYC", 4);
	doc->itemToolPrefs().polyFactor = attrs.valueAsDouble("POLYF", 0.5);
	doc->itemToolPrefs().polyRotation     = attrs.valueAsDouble("POLYR", 0.0);
	doc->itemToolPrefs().polyInnerRot     = attrs.valueAsDouble("POLYIR", 0.0);
	doc->itemToolPrefs().polyCurvature    = attrs.valueAsDouble("POLYCUR", 0.0);
	doc->itemToolPrefs().polyOuterCurvature    = attrs.valueAsDouble("POLYOCUR", 0.0);
	doc->itemToolPrefs().polyUseFactor    = attrs.valueAsBool("POLYS", false);

	doc->itemToolPrefs().arcStartAngle = attrs.valueAsDouble("arcStartAngle", 30.0);
	doc->itemToolPrefs().arcSweepAngle = attrs.valueAsDouble("arcSweepAngle", 300.0);
	doc->itemToolPrefs().spiralStartAngle = attrs.valueAsDouble("spiralStartAngle", 0.0);
	doc->itemToolPrefs().spiralEndAngle = attrs.valueAsDouble("spiralEndAngle", 1080.0);
	doc->itemToolPrefs().spiralFactor = attrs.valueAsDouble("spiralFactor", 1.2);

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
	doc->itemToolPrefs().lineColorShade  = attrs.valueAsInt("LINESHADE", 100);
	doc->itemToolPrefs().shapeFillColorShade      = attrs.valueAsInt("BRUSHSHADE", 100);
	doc->itemToolPrefs().calligraphicPenFillColor = attrs.valueAsString("calligraphicPenFillColor", "Black");
	doc->itemToolPrefs().calligraphicPenLineColor = attrs.valueAsString("calligraphicPenLineColor", "Black");
	doc->itemToolPrefs().calligraphicPenFillColorShade = attrs.valueAsInt("calligraphicPenFillColorShade", 100);
	doc->itemToolPrefs().calligraphicPenLineColorShade = attrs.valueAsInt("calligraphicPenLineColorShade", 100);
	doc->itemToolPrefs().calligraphicPenLineWidth = attrs.valueAsDouble("calligraphicPenLineWidth", 1.0);
	doc->itemToolPrefs().calligraphicPenAngle = attrs.valueAsDouble("calligraphicPenAngle", 0.0);
	doc->itemToolPrefs().calligraphicPenWidth = attrs.valueAsDouble("calligraphicPenWidth", 10.0);
	doc->itemToolPrefs().calligraphicPenStyle = static_cast<Qt::PenStyle>(attrs.valueAsInt("calligraphicPenStyle"));
	doc->opToolPrefs().dispX       = attrs.valueAsDouble("dispX", 10.0);
	doc->opToolPrefs().dispY       = attrs.valueAsDouble("dispY", 10.0);
	doc->opToolPrefs().constrain   = attrs.valueAsDouble("constrain", 15.0);
	doc->itemToolPrefs().textTabFillChar = attrs.valueAsString("TabFill","");
	doc->itemToolPrefs().textTabWidth   = attrs.valueAsDouble("TabWidth", 36.0);
	if (attrs.hasAttribute("CPICT"))
		doc->itemToolPrefs().imageFillColor = attrs.valueAsString("CPICT");
	doc->itemToolPrefs().imageFillColorShade = attrs.valueAsInt("PICTSHADE", 100);
	if (attrs.hasAttribute("CSPICT"))
		doc->itemToolPrefs().imageStrokeColor = attrs.valueAsString("CSPICT");
	doc->itemToolPrefs().imageStrokeColorShade = attrs.valueAsInt("PICTSSHADE", 100);
}

void Scribus150Format::readTypographicSettings(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
{
	doc->typographicPrefs().valueSuperScript   = attrs.valueAsInt("VHOCH");
	doc->typographicPrefs().scalingSuperScript = attrs.valueAsInt("VHOCHSC");
	doc->typographicPrefs().valueSubScript     = attrs.valueAsInt("VTIEF");
	doc->typographicPrefs().scalingSubScript   = attrs.valueAsInt("VTIEFSC");
	doc->typographicPrefs().valueSmallCaps     = attrs.valueAsInt("VKAPIT");
	doc->guidesPrefs().valueBaselineGrid      = attrs.valueAsDouble("BASEGRID", 12.0);
	doc->guidesPrefs().offsetBaselineGrid     = attrs.valueAsDouble("BASEO", 0.0);
	// #9621 : autolinespacing is now expressed as a percentage of the font height
	doc->typographicPrefs().autoLineSpacing    = attrs.valueAsInt("AUTOL", 1, 500, 100);
	doc->typographicPrefs().valueUnderlinePos  = attrs.valueAsInt("UnderlinePos", -1);
	doc->typographicPrefs().valueUnderlineWidth  = attrs.valueAsInt("UnderlineWidth", -1);
	doc->typographicPrefs().valueStrikeThruPos   = attrs.valueAsInt("StrikeThruPos", -1);
	doc->typographicPrefs().valueStrikeThruWidth = attrs.valueAsInt("StrikeThruWidth", -1);
}

bool Scribus150Format::readPageSets(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	struct PageSet pageS;
	ScXmlStreamAttributes attrs;

	doc->clearPageSets();
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		QStringRef tagName = reader.name();
		if (reader.isStartElement())
			attrs = reader.attributes();
		if (reader.isEndElement() && tagName == "PageSets")
			break;
		if (reader.isStartElement() && tagName == "Set")
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
		if (reader.isEndElement() && tagName == "Set")
		{
//			doc->pageSets.append(pageS);
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

bool Scribus150Format::readCheckProfile(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
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
	checkerSettings.checkNotCMYKOrSpot   = attrs.valueAsBool("checkNotCMYKOrSpot", false);
	checkerSettings.checkDeviceColorsAndOutputIntent = attrs.valueAsBool("checkDeviceColorsAndOutputIntent", false);
	checkerSettings.checkFontNotEmbedded = attrs.valueAsBool("checkFontNotEmbedded", false);
	checkerSettings.checkFontIsOpenType  = attrs.valueAsBool("checkFontIsOpenType", false);
	checkerSettings.checkAppliedMasterDifferentSide  = attrs.valueAsBool("checkAppliedMasterDifferentSide", true);
	checkerSettings.checkEmptyTextFrames     = attrs.valueAsBool("checkEmptyTextFrames", true);
	doc->set1CheckerProfile(profileName, checkerSettings);
	return true;
}

void Scribus150Format::readColor(ColorList& colors, ScXmlStreamAttributes& attrs)
{
	ScColor color;
	if (attrs.hasAttribute("SPACE"))
	{
		QString space = attrs.valueAsString("SPACE");
		if (space == "CMYK")
		{
			double c = attrs.valueAsDouble("C", 0) / 100.0;
			double m = attrs.valueAsDouble("M", 0) / 100.0;
			double y = attrs.valueAsDouble("Y", 0) / 100.0;
			double k = attrs.valueAsDouble("K", 0) / 100.0;
			color.setCmykColorF(c, m, y, k);
		}
		else if (space == "RGB")
		{
			double r = attrs.valueAsDouble("R", 0) / 255.0;
			double g = attrs.valueAsDouble("G", 0) / 255.0;
			double b = attrs.valueAsDouble("B", 0) / 255.0;
			color.setRgbColorF(r, g, b);
		}
		else if (space == "Lab")
		{
			double L = attrs.valueAsDouble("L", 0);
			double a = attrs.valueAsDouble("A", 0);
			double b = attrs.valueAsDouble("B", 0);
			color.setLabColor(L, a, b);
		}
	}
	else if (attrs.hasAttribute("CMYK"))
		color.setNamedColor(attrs.valueAsString("CMYK"));
	else if (attrs.hasAttribute("RGB"))
		color.fromQColor(QColor(attrs.valueAsString("RGB")));
	else
	{
		double L = attrs.valueAsDouble("L", 0);
		double a = attrs.valueAsDouble("A", 0);
		double b = attrs.valueAsDouble("B", 0);
		color.setLabColor(L, a, b);
	}
	color.setSpotColor( attrs.valueAsBool("Spot", false) );
	color.setRegistrationColor( attrs.valueAsBool("Register", false) );
	QString name(attrs.valueAsString("NAME", color.name()));
	if (name == "All")
	{
		color.setSpotColor(true);
		color.setRegistrationColor(true);
		color.setCmykColorF(1.0, 1.0, 1.0, 1.0);
	}
	// #10323 : break loading of doc which contain colors with different names
	// and same definition
	// colors.tryAddColor(name, color);
	if (name.length() > 0 && !colors.contains(name))
		colors.insert(name, color);
}

bool Scribus150Format::readGradient(ScribusDoc *doc, VGradient &gra, ScXmlStreamReader& reader)
{
	gra = VGradient(VGradient::linear);
	gra.clearStops();
	ScXmlStreamAttributes rattrs = reader.scAttributes();
	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
	{
		ScXmlStreamReader::TokenType tType = reader.readNext();
		if (tType == ScXmlStreamReader::EndElement && reader.name() == tagName)
			break;
		if (tType == ScXmlStreamReader::StartElement && reader.name() == "CSTOP")
		{
			ScXmlStreamAttributes attrs = reader.scAttributes();
			QString name = attrs.valueAsString("NAME");
			double ramp  = attrs.valueAsDouble("RAMP", 0.0);
			int shade    = attrs.valueAsInt("SHADE", 100);
			double opa   = attrs.valueAsDouble("TRANS", 1.0);
			gra.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
		}
	}
	return !reader.hasError();
}

void Scribus150Format::readCharacterStyleAttrs(ScribusDoc *doc, ScXmlStreamAttributes& attrs, CharStyle & newStyle)
{
	static const QString CPARENT("CPARENT");
	if (attrs.hasAttribute(CPARENT))
		newStyle.setParent(attrs.valueAsString(CPARENT));

	static const QString FONT("FONT");
	if (attrs.hasAttribute(FONT))
	{
		const ScFace& face = m_AvailableFonts->findFont(attrs.valueAsString(FONT), doc);
		if (!face.isNone())
			newStyle.setFont(face);
	}

	static const QString FONTSIZE("FONTSIZE");
	if (attrs.hasAttribute(FONTSIZE))
		newStyle.setFontSize(qRound(attrs.valueAsDouble(FONTSIZE) * 10));

	static const QString FONTFEATURES("FONTFEATURES");
	if (attrs.hasAttribute(FONTFEATURES))
		newStyle.setFontFeatures(attrs.valueAsString(FONTFEATURES));

	static const QString FCOLOR("FCOLOR");
	if (attrs.hasAttribute(FCOLOR))
		newStyle.setFillColor(attrs.valueAsString(FCOLOR));

	static const QString HyphenChar("HyphenChar");
	if (attrs.hasAttribute(HyphenChar))
		newStyle.setHyphenChar(attrs.valueAsInt(HyphenChar));

	static const QString HyphenWordMin("HyphenWordMin");
	if (attrs.hasAttribute(HyphenWordMin))
		newStyle.setHyphenWordMin(attrs.valueAsInt(HyphenWordMin));

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

	static const QString BCOLOR("BGCOLOR");
	if (attrs.hasAttribute(BCOLOR))
		newStyle.setBackColor(attrs.valueAsString(BCOLOR, CommonStrings::None));
	static const QString BSHADE("BGSHADE");
	if (attrs.hasAttribute(BSHADE))
		newStyle.setBackShade(attrs.valueAsInt(BSHADE, 100));

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
	{
		QString l(attrs.valueAsString(LANGUAGE));
		if (LanguageManager::instance()->langTableIndex(l) != -1)
			newStyle.setLanguage(l); //new style storage
		else
		{ //old style storage
			QString lnew = LanguageManager::instance()->getAbbrevFromLang(l, false);
			if (lnew.isEmpty())
				lnew = LanguageManager::instance()->getAbbrevFromLang(l, false);
			newStyle.setLanguage(lnew);
		}
	}

	static const QString SHORTCUT("SHORTCUT");
	if (attrs.hasAttribute(SHORTCUT))
		newStyle.setShortcut(attrs.valueAsString(SHORTCUT));

	static const QString WORDTRACK("wordTrack");
	if (attrs.hasAttribute(WORDTRACK))
		newStyle.setWordTracking(attrs.valueAsDouble(WORDTRACK));
}

void Scribus150Format::readNamedCharacterStyleAttrs(ScribusDoc *doc, ScXmlStreamAttributes& attrs, CharStyle & newStyle)
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

	// Check that a style is not its own parent
	QString parentStyle = newStyle.parent();
	if (parentStyle == newStyle.name())
		newStyle.setParent(QString());
}

void Scribus150Format::readParagraphStyle(ScribusDoc *doc, ScXmlStreamReader& reader, ParagraphStyle& newStyle)
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
	if (!parentStyle.isEmpty() && (parentStyle != newStyle.name()))
	{
		if (m_Doc->styleExists(parentStyle))
			newStyle.setParent(parentStyle);
		else if (parStyleMap.contains(parentStyle))
			newStyle.setParent(parStyleMap.value(parentStyle));
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

	static const QString DIRECTION("DIRECTION");
	if (attrs.hasAttribute(DIRECTION))
		newStyle.setDirection(static_cast<ParagraphStyle::DirectionType>(attrs.valueAsInt(DIRECTION)));

	static const QString VOR("VOR");
	if (attrs.hasAttribute(VOR))
		newStyle.setGapBefore(attrs.valueAsDouble(VOR));

	static const QString NACH("NACH");
	if (attrs.hasAttribute(NACH))
		newStyle.setGapAfter(attrs.valueAsDouble(NACH));

	static const QString ParagraphEffectCharStyle("ParagraphEffectCharStyle");
	if (attrs.hasAttribute(ParagraphEffectCharStyle))
		newStyle.setPeCharStyleName(attrs.valueAsString(ParagraphEffectCharStyle));

	static const QString ParagraphEffectOffset("ParagraphEffectOffset");
	if (attrs.hasAttribute(ParagraphEffectOffset))
		newStyle.setParEffectOffset(attrs.valueAsDouble(ParagraphEffectOffset));

	static const QString ParagraphEffectIndent("ParagraphEffectIndent");
	if (attrs.hasAttribute(ParagraphEffectIndent))
		newStyle.setParEffectIndent(attrs.valueAsDouble(ParagraphEffectIndent));

	static const QString DROP("DROP");
	if (attrs.hasAttribute(DROP))
		newStyle.setHasDropCap(static_cast<bool>(attrs.valueAsInt(DROP)));

	static const QString DROPCHSTYLE("DROPCHSTYLE");
	if (attrs.hasAttribute(DROPCHSTYLE))
		newStyle.setPeCharStyleName(attrs.valueAsString(DROPCHSTYLE));

	static const QString DROPLIN("DROPLIN");
	if (attrs.hasAttribute(DROPLIN))
		newStyle.setDropCapLines(attrs.valueAsInt(DROPLIN));

	static const QString DROPDIST("DROPDIST");
	if (attrs.hasAttribute(DROPDIST))
		newStyle.setParEffectOffset(attrs.valueAsDouble(DROPDIST));

	static const QString Bullet("Bullet");
	if (attrs.hasAttribute(Bullet))
		newStyle.setHasBullet(static_cast<bool>(attrs.valueAsInt(Bullet)));

	static const QString BulletStr("BulletStr");
	if (attrs.hasAttribute(BulletStr))
		newStyle.setBulletStr(attrs.valueAsString(BulletStr));

	static const QString Numeration("Numeration");
	if (attrs.hasAttribute(Numeration))
		newStyle.setHasNum(static_cast<bool>(attrs.valueAsInt(Numeration)));

	static const QString NumerationName("NumerationName");
	if (attrs.hasAttribute(NumerationName))
		newStyle.setNumName(attrs.valueAsString(NumerationName));

	static const QString NumerationFormat("NumerationFormat");
	if (attrs.hasAttribute(NumerationFormat))
		newStyle.setNumFormat(attrs.valueAsInt(NumerationFormat));

	static const QString NumerationLevel("NumerationLevel");
	if (attrs.hasAttribute(NumerationLevel))
		newStyle.setNumLevel(attrs.valueAsInt(NumerationLevel));

	static const QString NumerationStart("NumerationStart");
	if (attrs.hasAttribute(NumerationStart))
		newStyle.setNumStart(attrs.valueAsInt(NumerationStart));

	static const QString NumearationPrefix("NumerationPrefix");
	if (attrs.hasAttribute(NumearationPrefix))
		newStyle.setNumPrefix(attrs.valueAsString(NumearationPrefix));

	static const QString NumerationSuffix("NumerationSuffix");
	if (attrs.hasAttribute(NumerationSuffix))
		newStyle.setNumSuffix(attrs.valueAsString(NumerationSuffix));

	static const QString NumerationRestart("NumerationRestart");
	if (attrs.hasAttribute(NumerationRestart))
		newStyle.setNumRestart(attrs.valueAsInt(NumerationRestart));

	static const QString NumerationOther("NumeartionOther");
	if (attrs.hasAttribute(NumerationOther))
		newStyle.setNumOther(static_cast<bool>(attrs.valueAsInt(NumerationOther)));

	static const QString NumearationHigher("NumerationHigher");
	if (attrs.hasAttribute(NumearationHigher))
		newStyle.setNumHigher(static_cast<bool>(attrs.valueAsInt(NumearationHigher)));

	static const QString PSHORTCUT("PSHORTCUT");
	if (attrs.hasAttribute(PSHORTCUT))
		newStyle.setShortcut(attrs.valueAsString(PSHORTCUT));

	static const QString OpticalMargins("OpticalMargins");
	if (attrs.hasAttribute(OpticalMargins))
		newStyle.setOpticalMargins(attrs.valueAsInt(OpticalMargins));

	static const QString HyphenConsecutiveLines("HyphenConsecutiveLines");
	if (attrs.hasAttribute(HyphenConsecutiveLines))
		newStyle.setHyphenConsecutiveLines(attrs.valueAsInt(HyphenConsecutiveLines));

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

	static const QString KeepLinesStart("KeepLinesStart");
	if (attrs.hasAttribute(KeepLinesStart))
		newStyle.setKeepLinesStart(attrs.valueAsInt(KeepLinesStart));

	static const QString KeepLinesEnd("KeepLinesEnd");
	if (attrs.hasAttribute(KeepLinesEnd))
		newStyle.setKeepLinesEnd(attrs.valueAsInt(KeepLinesEnd));

	static const QString KeepWithNext("KeepWithNext");
	if (attrs.hasAttribute(KeepWithNext))
		newStyle.setKeepWithNext(attrs.valueAsInt(KeepWithNext));

	static const QString KeepTogether("KeepTogether");
	if (attrs.hasAttribute(KeepTogether))
		newStyle.setKeepTogether(attrs.valueAsInt(KeepTogether));
	static const QString BCOLOR("BCOLOR");
	if (attrs.hasAttribute(BCOLOR))
		newStyle.setBackgroundColor(attrs.valueAsString(BCOLOR, CommonStrings::None));
	static const QString BSHADE("BSHADE");
	if (attrs.hasAttribute(BSHADE))
		newStyle.setBackgroundShade(attrs.valueAsInt(BSHADE, 100));

	readCharacterStyleAttrs( doc, attrs, newStyle.charStyle());

	//	newStyle.tabValues().clear();
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
	
	fixLegacyParStyle(newStyle);
}

void Scribus150Format::readTableStyle(ScribusDoc *doc, ScXmlStreamReader& reader, TableStyle& newStyle)
{
	ScXmlStreamAttributes attrs = reader.scAttributes();
	newStyle.erase();
	newStyle.setName(attrs.valueAsString("NAME", ""));
	// The default style attribute must be correctly set before trying to assign a parent
	if (attrs.hasAttribute("DefaultStyle"))
		newStyle.setDefaultStyle(attrs.valueAsInt("DefaultStyle"));
	else if (newStyle.name() == CommonStrings::DefaultTableStyle || newStyle.name() == CommonStrings::trDefaultTableStyle)
		newStyle.setDefaultStyle(true);
	else
		newStyle.setDefaultStyle(false);
	QString parentStyle = attrs.valueAsString("PARENT", "");
	if (!parentStyle.isEmpty() && (parentStyle != newStyle.name()))
		newStyle.setParent(parentStyle);
	if (attrs.hasAttribute("FillColor"))
		newStyle.setFillColor(attrs.valueAsString("FillColor"));
	if (attrs.hasAttribute("FillShade"))
		newStyle.setFillShade(attrs.valueAsDouble("FillShade"));
	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (reader.name() == "TableBorderLeft")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			newStyle.setLeftBorder(border);
		}
		else if (reader.name() == "TableBorderRight")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			newStyle.setRightBorder(border);
		}
		else if (reader.name() == "TableBorderTop")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			newStyle.setTopBorder(border);
		}
		else if (reader.name() == "TableBorderBottom")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			newStyle.setBottomBorder(border);
		}
	}
}

void Scribus150Format::readCellStyle(ScribusDoc *doc, ScXmlStreamReader& reader, CellStyle& newStyle)
{
	ScXmlStreamAttributes attrs = reader.scAttributes();
	newStyle.erase();
	newStyle.setName(attrs.valueAsString("NAME", ""));
	// The default style attribute must be correctly set before trying to assign a parent
	if (attrs.hasAttribute("DefaultStyle"))
		newStyle.setDefaultStyle(attrs.valueAsInt("DefaultStyle"));
	else if (newStyle.name() == CommonStrings::DefaultCellStyle || newStyle.name() == CommonStrings::trDefaultCellStyle)
		newStyle.setDefaultStyle(true);
	else
		newStyle.setDefaultStyle(false);
	QString parentStyle = attrs.valueAsString("PARENT", "");
	if (!parentStyle.isEmpty() && (parentStyle != newStyle.name()))
		newStyle.setParent(parentStyle);
	if (attrs.hasAttribute("FillColor"))
		newStyle.setFillColor(attrs.valueAsString("FillColor"));
	if (attrs.hasAttribute("FillShade"))
		newStyle.setFillShade(attrs.valueAsDouble("FillShade"));
	if (attrs.hasAttribute("LeftPadding"))
		newStyle.setLeftPadding(attrs.valueAsDouble("LeftPadding", 0.0));
	if (attrs.hasAttribute("RightPadding"))
		newStyle.setRightPadding(attrs.valueAsDouble("RightPadding", 0.0));
	if (attrs.hasAttribute("TopPadding"))
		newStyle.setTopPadding(attrs.valueAsDouble("TopPadding", 0.0));
	if (attrs.hasAttribute("BottomPadding"))
		newStyle.setBottomPadding(attrs.valueAsDouble("BottomPadding", 0.0));
	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (reader.name() == "TableBorderLeft")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			newStyle.setLeftBorder(border);
		}
		else if (reader.name() == "TableBorderRight")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			newStyle.setRightBorder(border);
		}
		else if (reader.name() == "TableBorderTop")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			newStyle.setTopBorder(border);
		}
		else if (reader.name() == "TableBorderBottom")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			newStyle.setBottomBorder(border);
		}
	}
}

void Scribus150Format::readLayers(ScLayer& layer, ScXmlStreamAttributes& attrs)
{
	int lId   = attrs.valueAsInt("NUMMER");
	int level = attrs.valueAsInt("LEVEL");
	layer = ScLayer( attrs.valueAsString("NAME"), level, lId);
	layer.isViewable   = attrs.valueAsInt("SICHTBAR");
	layer.isPrintable  = attrs.valueAsInt("DRUCKEN");
	layer.isEditable   = attrs.valueAsInt("EDIT", 1);
	layer.flowControl  = attrs.valueAsInt("FLOW", 1);
	layer.isSelectable = attrs.valueAsInt("SELECT", 0);
	layer.transparency = attrs.valueAsDouble("TRANS", 1.0);
	layer.blendMode    = attrs.valueAsInt("BLEND", 0);
	layer.outlineMode  = attrs.valueAsInt("OUTL", 0);
	if (attrs.hasAttribute("LAYERC"))
		layer.markerColor =  QColor(attrs.valueAsString("LAYERC","#000000"));
}

bool Scribus150Format::readArrows(ScribusDoc* doc, ScXmlStreamAttributes& attrs)
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

bool Scribus150Format::readMultiline(multiLine& ml, ScXmlStreamReader& reader)
{
	ml = multiLine();
	ScXmlStreamAttributes rattrs = reader.scAttributes();
	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
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

bool Scribus150Format::readBookMark(ScribusDoc::BookMa& bookmark, int& elem, ScXmlStreamAttributes& attrs)
{
	elem = attrs.valueAsInt("Element");
	bookmark.PageObject = nullptr;
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

bool Scribus150Format::readPDFOptions(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	ScXmlStreamAttributes attrs = reader.scAttributes();

	doc->pdfOptions().firstUse   = attrs.valueAsBool("firstUse", true);
	doc->pdfOptions().Articles   = attrs.valueAsBool("Articles");
	doc->pdfOptions().Thumbnails = attrs.valueAsBool("Thumbnails");
	doc->pdfOptions().Compress   = attrs.valueAsBool("Compress");
	doc->pdfOptions().CompressMethod = (PDFOptions::PDFCompression) attrs.valueAsInt("CMethod", 0);
	doc->pdfOptions().Quality    = attrs.valueAsInt("Quality", 0);
	doc->pdfOptions().RecalcPic  = attrs.valueAsBool("RecalcPic");
	doc->pdfOptions().embedPDF   = attrs.valueAsBool("EmbedPDF", false);
	doc->pdfOptions().Bookmarks  = attrs.valueAsBool("Bookmarks");
	doc->pdfOptions().MirrorH    = attrs.valueAsBool("MirrorH", false);
	doc->pdfOptions().MirrorV    = attrs.valueAsBool("MirrorV", false);
	doc->pdfOptions().RotateDeg  = attrs.valueAsInt("RotateDeg", 0);
	doc->pdfOptions().pageRangeSelection = attrs.valueAsInt("rangeSel", 0);
	doc->pdfOptions().pageRangeString = attrs.valueAsString("rangeTxt", "");
	doc->pdfOptions().doClip     = attrs.valueAsBool("Clip", false);
	doc->pdfOptions().PresentMode = attrs.valueAsBool("PresentMode");
	doc->pdfOptions().PicRes     = attrs.valueAsInt("PicRes");
	// Fixme: check input pdf version
	doc->pdfOptions().Version    = (PDFVersion::Version) attrs.valueAsInt("Version");
	doc->pdfOptions().Resolution = attrs.valueAsInt("Resolution");
	doc->pdfOptions().Binding    = attrs.valueAsInt("Binding");
	doc->pdfOptions().fileName   = "";

	doc->pdfOptions().FontEmbedding = (PDFOptions::PDFFontEmbedding) attrs.valueAsInt("FontEmbedding", 0);
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
	doc->pdfOptions().bleeds.setTop(attrs.valueAsDouble("BTop", 0.0));
	doc->pdfOptions().bleeds.setLeft(attrs.valueAsDouble("BLeft", 0.0));
	doc->pdfOptions().bleeds.setRight(attrs.valueAsDouble("BRight", 0.0));
	doc->pdfOptions().bleeds.setBottom(attrs.valueAsDouble("BBottom", 0.0));
	doc->pdfOptions().useDocBleeds  = attrs.valueAsBool("useDocBleeds", true);
	doc->pdfOptions().cropMarks     = attrs.valueAsBool("cropMarks", false);
	doc->pdfOptions().bleedMarks    = attrs.valueAsBool("bleedMarks", false);
	doc->pdfOptions().registrationMarks = attrs.valueAsBool("registrationMarks", false);
	doc->pdfOptions().colorMarks    = attrs.valueAsBool("colorMarks", false);
	doc->pdfOptions().docInfoMarks  = attrs.valueAsBool("docInfoMarks", false);
	doc->pdfOptions().markLength    = attrs.valueAsDouble("markLength", 20.0);
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
	doc->pdfOptions().openAfterExport     = attrs.valueAsBool("openAfterExport", false);
	doc->pdfOptions().PageLayout    = attrs.valueAsInt("PageLayout", 0);
	doc->pdfOptions().openAction    = attrs.valueAsString("openAction", "");

	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && (reader.name() == tagName))
			break;
		if (!reader.isStartElement())
			continue;
		QStringRef tName = reader.name();
		attrs = reader.scAttributes();
		if (tName == "LPI")
		{
			struct LPIData lpo;
			lpo.Angle     = attrs.valueAsInt("Angle");
			lpo.Frequency = attrs.valueAsInt("Frequency");
			lpo.SpotFunc  = attrs.valueAsInt("SpotFunction");
			doc->pdfOptions().LPISettings[attrs.valueAsString("Color")] = lpo;
		}
		if (tName == "Fonts")
		{
			QString fname = attrs.valueAsString("Name");
			if (!doc->pdfOptions().EmbedList.contains(fname))
				doc->pdfOptions().EmbedList.append(fname);
		}
		if (tName == "Subset")
		{
			QString sname = attrs.valueAsString("Name");
			if (!doc->pdfOptions().SubsetList.contains(sname))
				doc->pdfOptions().SubsetList.append(sname);
		}
		if (tName == "Effekte")
		{
			struct PDFPresentationData ef;
			ef.pageEffectDuration =  attrs.valueAsInt("pageEffectDuration");
			ef.pageViewDuration =  attrs.valueAsInt("pageViewDuration");
			ef.effectType = attrs.valueAsInt("effectType");
			ef.Dm = attrs.valueAsInt("Dm");
			ef.M  = attrs.valueAsInt("M");
			ef.Di = attrs.valueAsInt("Di");
			EffVal.append(ef);
		}
	}
	return !reader.hasError();
}

bool Scribus150Format::readPrinterOptions(ScribusDoc* doc, ScXmlStreamReader& reader)
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
	doc->Print_Options.doGCR    = attrs.valueAsBool("doGCR");
	doc->Print_Options.doClip   = attrs.valueAsBool("doClip");
	doc->Print_Options.setDevParam  = attrs.valueAsBool("setDevParam");
	doc->Print_Options.useDocBleeds = attrs.valueAsBool("useDocBleeds");
	doc->Print_Options.cropMarks    = attrs.valueAsBool("cropMarks");
	doc->Print_Options.bleedMarks   = attrs.valueAsBool("bleedMarks");
	doc->Print_Options.registrationMarks = attrs.valueAsBool("registrationMarks");
	doc->Print_Options.colorMarks   = attrs.valueAsBool("colorMarks");
	doc->Print_Options.includePDFMarks = attrs.valueAsBool("includePDFMarks", true);
	if (attrs.hasAttribute("PrintEngine"))
		doc->Print_Options.prnEngine = (PrintEngine) attrs.valueAsInt("PrintEngine", 3);
	else
		doc->Print_Options.prnEngine = (PrintEngine) attrs.valueAsInt("PSLevel", 3);
	doc->Print_Options.markLength    = attrs.valueAsDouble("markLength");
	doc->Print_Options.markOffset    = attrs.valueAsDouble("markOffset");
	doc->Print_Options.bleeds.setTop(attrs.valueAsDouble("BleedTop"));
	doc->Print_Options.bleeds.setLeft(attrs.valueAsDouble("BleedLeft"));
	doc->Print_Options.bleeds.setRight(attrs.valueAsDouble("BleedRight"));
	doc->Print_Options.bleeds.setBottom(attrs.valueAsDouble("BleedBottom"));
	doc->Print_Options.printer  = attrs.valueAsString("printer");
	doc->Print_Options.filename = attrs.valueAsString("filename");
	doc->Print_Options.separationName = attrs.valueAsString("separationName");
	doc->Print_Options.printerCommand = attrs.valueAsString("printerCommand");
	doc->Print_Options.copies = 1;

	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
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

bool Scribus150Format::readDocItemAttributes(ScribusDoc *doc, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();
	doc->clearItemAttributes();
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (reader.isStartElement() && reader.name() == "ItemAttribute")
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

bool Scribus150Format::readTableOfContents(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();
	m_Doc->clearTocSetups();
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (reader.isStartElement() && reader.name() == "TableOfContents")
		{
			ScXmlStreamAttributes attrs = reader.scAttributes();
			ToCSetup tocsetup;
			tocsetup.name = attrs.valueAsString("Name");
			tocsetup.itemAttrName = attrs.valueAsString("ItemAttributeName");
			tocsetup.frameName    = attrs.valueAsString("FrameName");
			tocsetup.textStyle    = attrs.valueAsString("Style");
			tocsetup.listNonPrintingFrames = QVariant(attrs.valueAsString("ListNonPrinting")).toBool();
			QString numberPlacement(attrs.valueAsString("NumberPlacement"));
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

bool Scribus150Format::readNotesStyles(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		//read notes styles
		if (reader.isStartElement() && reader.name() == "notesStyle")
		{
			ScXmlStreamAttributes attrs = reader.scAttributes();
			NotesStyle NS;
			NS.setName(attrs.valueAsString("Name"));
			NS.setStart(attrs.valueAsInt("Start"));
			NS.setEndNotes(attrs.valueAsBool("Endnotes"));
			QString type = attrs.valueAsString("Type");
			if (type == "Type_1_2_3")
				NS.setType(Type_1_2_3);
			else if (type == "Type_1_2_3_ar")
				NS.setType(Type_1_2_3_ar);
			else if (type == "Type_i_ii_iii")
				NS.setType(Type_i_ii_iii);
			else if (type == "Type_I_II_III")
				NS.setType(Type_I_II_III);
			else if (type == "Type_a_b_c")
				NS.setType(Type_a_b_c);
			else if (type == "Type_A_B_C")
				NS.setType(Type_A_B_C);
			else if (type == "Type_Alphabet_ar")
				NS.setType(Type_Alphabet_ar);
			else if (type == "Type_Abjad_ar")
				NS.setType(Type_Abjad_ar);
			else if (type == "Type_Hebrew")
				NS.setType(Type_Hebrew);
			else if (type == "Type_asterix")
				NS.setType(Type_asterix);
			else if (type == "Type_CJK")
				NS.setType(Type_CJK);
			else //if (type == "Type_None")
				NS.setType(Type_None);
			// Fix deprecated numeration ranges
			NumerationRange numRange = (NumerationRange) attrs.valueAsInt("Range");
			if (numRange != NSRdocument && numRange != NSRstory)
				numRange = NSRstory;
			NS.setRange(numRange);
			NS.setPrefix(attrs.valueAsString("Prefix"));
			NS.setSuffix(attrs.valueAsString("Suffix"));
			NS.setAutoNotesHeight(attrs.valueAsBool("AutoHeight"));
			NS.setAutoNotesWidth(attrs.valueAsBool("AutoWidth"));
			NS.setAutoRemoveEmptyNotesFrames(attrs.valueAsBool("AutoRemove"));
			NS.setAutoWeldNotesFrames(attrs.valueAsBool("AutoWeld"));
			NS.setSuperscriptInNote(attrs.valueAsBool("SuperNote"));
			NS.setSuperscriptInMaster(attrs.valueAsBool("SuperMaster"));
			NS.setMarksCharStyle("");
			NS.setNotesParStyle("");
			QString name;
			name = attrs.valueAsString("MarksStyle");
			if (!name.isEmpty())
				NS.setMarksCharStyle(name);
			name = attrs.valueAsString("NotesStyle");
			if (!name.isEmpty())
				NS.setNotesParStyle(name);

			m_Doc->newNotesStyle(NS);
		}
	}
	return !reader.hasError();
}

bool Scribus150Format::readNotesFrames(ScXmlStreamReader& reader)
{
	notesFramesData.clear();
	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (reader.isStartElement())
		{
			ScXmlStreamAttributes attrs = reader.scAttributes();
			NoteFrameData eF;
			eF.NSname = attrs.valueAsString("NSname");
			eF.myID = attrs.valueAsInt("myID");
			if (reader.name() == "ENDNOTEFRAME")
			{
				eF.index = attrs.valueAsInt("index");
				eF.NSrange = (NumerationRange) attrs.valueAsInt("range");
				 // Fix deprecated numeration ranges
				if (eF.NSrange != NSRdocument && eF.NSrange != NSRstory)
					eF.NSrange = NSRstory;
				eF.itemID = attrs.valueAsInt("ItemID");
			}
			if (reader.name() == "FOOTNOTEFRAME")
				eF.itemID = attrs.valueAsInt("MasterID");
			notesFramesData.append(eF);
		}
	}
	return !reader.hasError();
}

bool Scribus150Format::readNotes(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	//read notes
	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (reader.isStartElement() && reader.name() == "Note")
		{
			ScXmlStreamAttributes attrs = reader.scAttributes();
			TextNote* note = m_Doc->newNote(nullptr);
			note->setSaxedText(attrs.valueAsString("Text"));
			//temporaly insert names of master mark and notes style into maps with note pointer
			//will be resolved to pointers by updateNames2Ptr() after all will read
			notesMasterMarks.insert(attrs.valueAsString("Master"), note);
			notesNSets.insert(note, attrs.valueAsString("NStyle"));
		}
	}
	return !reader.hasError();
}

bool Scribus150Format::readMarks(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (reader.isStartElement() && reader.name() == "Mark")
		{
			ScXmlStreamAttributes attrs = reader.scAttributes();

			QString label;
			if (attrs.hasAttribute("label"))
				label = attrs.valueAsString("label");

			MarkType type = MARKNoType;
			if (attrs.hasAttribute("type"))
				type = (MarkType) attrs.valueAsInt("type");

			if (!label.isEmpty() && type != MARKNoType)
			{
				Mark* mark = doc->newMark();
				mark->label=attrs.valueAsString("label");
				mark->setType(type);
				if (type == MARKVariableTextType && attrs.hasAttribute("str"))
					mark->setString(attrs.valueAsString("str"));
				if (type == MARK2ItemType && attrs.hasAttribute("ItemID"))
				{
					//QString itemName = attrs.valueAsString("itemName");
					markeredItemsMap.insert(mark, attrs.valueAsInt("ItemID"));
				}
				if (type == MARK2MarkType && attrs.hasAttribute("MARKlabel"))
				{
					QString mark2Label = attrs.valueAsString("MARKlabel");
					MarkType mark2Type = (MarkType) attrs.valueAsInt("MARKtype");
					Mark* mark2 = doc->getMark(mark2Label, mark2Type);
					if (mark2 != nullptr) //mark is not defined yet, insert into temp list for update to pointers later
						mark->setMark(mark2);
					else
					{
						QMap<QString, MarkType> mark2map;
						mark2map.insert(mark2Label, mark2Type);
						markeredMarksMap.insert(mark, mark2map);
					}
				}
			}
		}
	}
	return !reader.hasError();
}

bool Scribus150Format::readSections(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
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
			if (type == "Type_1_2_3_ar")
				newSection.type=Type_1_2_3_ar;
			if (type == "Type_i_ii_iii")
				newSection.type=Type_i_ii_iii;
			if (type == "Type_I_II_III")
				newSection.type=Type_I_II_III;
			if (type == "Type_a_b_c")
				newSection.type=Type_a_b_c;
			if (type == "Type_A_B_C")
				newSection.type=Type_A_B_C;
			if (type == "Type_Alphabet_ar")
				newSection.type=Type_Alphabet_ar;
			if (type == "Type_Abjad_ar")
				newSection.type=Type_Abjad_ar;
			if (type == "Type_Hebrew")
				newSection.type=Type_Hebrew;
			if (type == "Type_CJK")
				newSection.type=Type_CJK;
			if (type == "Type_None")
				newSection.type=Type_None;
			newSection.sectionstartindex = attrs.valueAsInt("Start");
			newSection.reversed = attrs.valueAsBool("Reversed");
			newSection.active = attrs.valueAsBool("Active");
			if (attrs.hasAttribute("FillChar"))
				newSection.pageNumberFillChar = QChar(attrs.valueAsInt("FillChar"));
			else
				newSection.pageNumberFillChar = QChar();
			if (attrs.hasAttribute("FieldWidth"))
				newSection.pageNumberWidth = attrs.valueAsInt("FieldWidth");
			else
				newSection.pageNumberWidth = 0;
			doc->sections().insert(newSection.number, newSection);
		}
	}
	return !reader.hasError();
}

bool Scribus150Format::readHyphen(ScribusDoc *doc, ScXmlStreamReader& reader)
{
	if (!doc->docHyphenator)
		doc->createHyphenator();

	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
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

bool Scribus150Format::readPage(ScribusDoc* doc, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();

	ScXmlStreamAttributes attrs = reader.scAttributes();
	int     pageNum  = attrs.valueAsInt("NUM");
	QString pageName = attrs.valueAsString("NAM", "");
	if (tagName == "MASTERPAGE" && pageName.isEmpty())
	{
		qDebug() << "scribus150format: corrupted masterpage with empty name detected";
		return true;
	}

	bool savedMasterPageMode = m_Doc->masterPageMode();
	m_Doc->setMasterPageMode(!pageName.isEmpty());
	ScPage* newPage = pageName.isEmpty() ? doc->addPage(pageNum) : doc->addMasterPage(pageNum, pageName);

	newPage->LeftPg   = attrs.valueAsInt("LEFT", 0);
	QString mpName    = attrs.valueAsString("MNAM", "Normal");
	newPage->setMasterPageName(m_Doc->masterPageMode() ? QString() : mpName);
	if (attrs.hasAttribute("Size"))
		newPage->setSize(attrs.valueAsString("Size"));
	if (attrs.hasAttribute("Orientation"))
		newPage->setOrientation(attrs.valueAsInt("Orientation"));
	newPage->setXOffset(attrs.valueAsDouble("PAGEXPOS"));
	newPage->setYOffset(attrs.valueAsDouble("PAGEYPOS"));
	if (attrs.hasAttribute("PAGEWIDTH"))
		newPage->setWidth(attrs.valueAsDouble("PAGEWIDTH"));
	else
		newPage->setWidth(attrs.valueAsDouble("PAGEWITH"));
	newPage->setHeight(attrs.valueAsDouble("PAGEHEIGHT"));

	//14704: Double check the page size should not be Custom in case the size doesn't match a standard size
	if (attrs.hasAttribute("Size"))
	{
		QString pageSize(attrs.valueAsString("Size"));
		PageSize ps(pageSize);
		if (!compareDouble(ps.width(), newPage->width()) || !compareDouble(ps.height(), newPage->height()))
			newPage->setSize(CommonStrings::customPageSize);
		else
			newPage->setSize(pageSize);
	}

	newPage->setInitialHeight(newPage->height());
	newPage->setInitialWidth(newPage->width());
	newPage->initialMargins.setTop(qMax(0.0, attrs.valueAsDouble("BORDERTOP")));
	newPage->initialMargins.setBottom(qMax(0.0, attrs.valueAsDouble("BORDERBOTTOM")));
	newPage->initialMargins.setLeft(qMax(0.0, attrs.valueAsDouble("BORDERLEFT")));
	newPage->initialMargins.setRight(qMax(0.0, attrs.valueAsDouble("BORDERRIGHT")));
	newPage->marginPreset = attrs.valueAsInt("PRESET", 0);
	newPage->Margins.setTop(newPage->initialMargins.top());
	newPage->Margins.setBottom(newPage->initialMargins.bottom());

	m_Doc->setMasterPageMode(savedMasterPageMode);

	// guides reading
	newPage->guides.setHorizontalAutoGap(attrs.valueAsDouble("AGhorizontalAutoGap", 0.0));
	newPage->guides.setVerticalAutoGap(attrs.valueAsDouble("AGverticalAutoGap", 0.0));
	newPage->guides.setHorizontalAutoCount(attrs.valueAsInt("AGhorizontalAutoCount", 0) );
	newPage->guides.setVerticalAutoCount(attrs.valueAsInt("AGverticalAutoCount", 0) );
	newPage->guides.setHorizontalAutoRefer(attrs.valueAsInt("AGhorizontalAutoRefer", 0) );
	newPage->guides.setVerticalAutoRefer(attrs.valueAsInt("AGverticalAutoRefer", 0) );
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
	struct PDFPresentationData ef;
	ef.pageEffectDuration =  attrs.valueAsInt("pageEffectDuration", 1);
	ef.pageViewDuration =  attrs.valueAsInt("pageViewDuration", 1);
	ef.effectType = attrs.valueAsInt("effectType", 0);
	ef.Dm = attrs.valueAsInt("Dm", 0);
	ef.M  = attrs.valueAsInt("M", 0);
	ef.Di = attrs.valueAsInt("Di", 0);
	newPage->PresentVals = ef;
	return true;
}

bool Scribus150Format::readObject(ScribusDoc* doc, ScXmlStreamReader& reader, const ReadObjectParams& readObjectParams, ItemInfo& info)
{
	QStringRef tagName = reader.name();
	ScXmlStreamAttributes attrs = reader.scAttributes();

	bool savedMasterPageMode = doc->masterPageMode();
	if (!readObjectParams.loadingPage)
	{
		if (tagName == "PAGEOBJECT" || tagName == "FRAMEOBJECT" || tagName == "PatternItem" || tagName == "ITEM")
			doc->setMasterPageMode(false);
		else
			doc->setMasterPageMode(true);
	}

	PageItem::ItemKind itemKind = PageItem::StandardItem;
	if (tagName =="FRAMEOBJECT")
		itemKind = PageItem::InlineItem;
	else if (tagName =="PatternItem")
		itemKind = PageItem::PatternItem;

	// We are loading patterns, force itemKind to PatternItem
	if (readObjectParams.itemKind == PageItem::PatternItem)
		itemKind = PageItem::PatternItem;

	int pageNr = -1;
	QString masterPageName = attrs.valueAsString("OnMasterPage");
	if ((!masterPageName.isEmpty()) && (tagName == "MASTEROBJECT"))
	{
		if (!readObjectParams.renamedMasterPage.isEmpty())
			masterPageName = readObjectParams.renamedMasterPage;
		doc->setCurrentPage(doc->MasterPages.at(doc->MasterNames[masterPageName]));
		pageNr = -2;
	}
	layerFound = false;
	clipPath.clear();

	PageItem* newItem = pasteItem(doc, attrs, readObjectParams.baseDir, itemKind, pageNr);
	newItem->setRedrawBounding();
	if (tagName == "MASTEROBJECT")
		newItem->setOwnerPage(doc->OnPage(newItem));
	else
		newItem->setOwnerPage(attrs.valueAsInt("OwnPage"));
	if ((tagName == "PAGEOBJECT") || (tagName == "ITEM"))
		newItem->setMasterPageName(QString());
	if (tagName == "ITEM")
	{
		newItem->setLayer(LayerToPaste);
		newItem->setMasterPage(doc->OnPage(newItem), doc->currentPage()->pageName());
	}
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
		if (newItem->inlineCharID == -1)
			FrameItems.append(m_Doc->Items->takeAt(m_Doc->Items->indexOf(newItem)));
		else
			doc->FrameItems.insert(newItem->inlineCharID, doc->Items->takeAt(doc->Items->indexOf(newItem)));
		newItem->setLayer(doc->firstLayerID());
	}

	info.item   = newItem;
	isNewFormat = attrs.hasAttribute("ItemID");
	if (isNewFormat)
	{
		info.itemID = attrs.valueAsInt("ItemID", 0);
		LinkID.insert(info.itemID, newItem);
	}
	info.nextItem = attrs.valueAsInt("NEXTITEM", -1);
	if (isNewFormat)
	{
		if (info.nextItem != -1)
			itemNext[info.itemID] = info.nextItem;
	}
	info.ownLink  = newItem->isTableItem ? attrs.valueAsInt("OwnLINK", 0) : 0;
	info.groupLastItem = 0;
	info.isGroupFlag = attrs.valueAsBool("isGroupControl", false);
	if (info.isGroupFlag)
		info.groupLastItem = attrs.valueAsInt("groupsLastItem", 0);
	info.isWeldFlag = attrs.valueAsBool("isWeldItem", false);
	info.ownWeld = attrs.valueAsInt("WeldSource", 0);
	info.ownNr = doc->Items->indexOf(newItem);

	struct ImageLoadRequest loadingInfo;
#ifdef HAVE_OSG
	struct PageItem_OSGFrame::viewDefinition currentView;
#endif
	QList<ParagraphStyle::TabRecord> tabValues;

	int mGArrayRows = 0;
	int mGArrayCols = 0;
	LastStyles * lastStyle = new LastStyles();
	while (!reader.atEnd() && !reader.hasError())
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
		if (tName == "S_CSTOP")
		{
			QString name = tAtt.valueAsString("NAME");
			double ramp  = tAtt.valueAsDouble("RAMP", 0.0);
			int shade    = tAtt.valueAsInt("SHADE", 100);
			double opa   = tAtt.valueAsDouble("TRANS", 1.0);
			newItem->stroke_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
		}
		if (tName == "M_CSTOP")
		{
			QString name = tAtt.valueAsString("NAME");
			double ramp  = tAtt.valueAsDouble("RAMP", 0.0);
			int shade    = tAtt.valueAsInt("SHADE", 100);
			double opa   = tAtt.valueAsDouble("TRANS", 1.0);
			newItem->mask_gradient.addStop(SetColor(doc, name, shade), ramp, 0.5, opa, name, shade);
		}
		if (tName == "MPoint")
		{
			MeshPoint mp;
			mp.colorName     = tAtt.valueAsString("NAME");
			mp.shade         = tAtt.valueAsInt("SHADE", 100);
			mp.transparency  = tAtt.valueAsDouble("TRANS", 1.0);
			mp.gridPoint     = FPoint(tAtt.valueAsDouble("GX", 0.0), tAtt.valueAsDouble("GY", 0.0));
			mp.controlTop    = FPoint(tAtt.valueAsDouble("CTX", 0.0), tAtt.valueAsDouble("CTY", 0.0));
			mp.controlBottom = FPoint(tAtt.valueAsDouble("CBX", 0.0), tAtt.valueAsDouble("CBY", 0.0));
			mp.controlLeft   = FPoint(tAtt.valueAsDouble("CLX", 0.0), tAtt.valueAsDouble("CLY", 0.0));
			mp.controlRight  = FPoint(tAtt.valueAsDouble("CRX", 0.0), tAtt.valueAsDouble("CRY", 0.0));
			mp.controlColor  = FPoint(tAtt.valueAsDouble("CCX", mp.gridPoint.x()), tAtt.valueAsDouble("CCY", mp.gridPoint.y()));
			mp.color         = SetColor(doc, mp.colorName, mp.shade);
			mp.color.setAlphaF(mp.transparency);
			newItem->meshGradientArray[mGArrayRows][mGArrayCols] = mp;
			mGArrayCols++;
			if (mGArrayCols == newItem->meshGradientArray[mGArrayRows].count())
			{
				mGArrayCols = 0;
				mGArrayRows++;
			}
		}
		if (tName == "PMPoint")
		{
			MeshPoint mp;
			mp.colorName     = tAtt.valueAsString("NAME");
			mp.shade         = tAtt.valueAsInt("SHADE", 100);
			mp.transparency  = tAtt.valueAsDouble("TRANS", 1.0);
			mp.gridPoint     = FPoint(tAtt.valueAsDouble("GX", 0.0), tAtt.valueAsDouble("GY", 0.0));
			mp.controlTop    = FPoint(tAtt.valueAsDouble("CTX", mp.gridPoint.x()), tAtt.valueAsDouble("CTY", mp.gridPoint.y()));
			mp.controlBottom = FPoint(tAtt.valueAsDouble("CBX", mp.gridPoint.x()), tAtt.valueAsDouble("CBY", mp.gridPoint.y()));
			mp.controlLeft   = FPoint(tAtt.valueAsDouble("CLX", mp.gridPoint.x()), tAtt.valueAsDouble("CLY", mp.gridPoint.y()));
			mp.controlRight  = FPoint(tAtt.valueAsDouble("CRX", mp.gridPoint.x()), tAtt.valueAsDouble("CRY", mp.gridPoint.y()));
			mp.controlColor  = FPoint(tAtt.valueAsDouble("CCX", mp.gridPoint.x()), tAtt.valueAsDouble("CCY", mp.gridPoint.y()));
			mp.color         = SetColor(doc, mp.colorName, mp.shade);
			mp.color.setAlphaF(mp.transparency);
			if (mGArrayCols == 0)
				newItem->meshGradientPatches[mGArrayRows].TL = mp;
			else if (mGArrayCols == 1)
				newItem->meshGradientPatches[mGArrayRows].TR = mp;
			else if (mGArrayCols == 2)
				newItem->meshGradientPatches[mGArrayRows].BR = mp;
			else if (mGArrayCols == 3)
				newItem->meshGradientPatches[mGArrayRows].BL = mp;
			mGArrayCols++;
			if (mGArrayCols == 4)
			{
				mGArrayCols = 0;
				mGArrayRows++;
			}
		}

		if (tName == "ITEXT")
			readItemText(newItem, tAtt, lastStyle);
		else if (tName == "TableData")
		{
			readItemTableData(newItem->asTable(), reader, doc);
		}
		else if (tName == "para")
		{
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::PARSEP);
			ParagraphStyle newStyle;
			readParagraphStyle(doc, reader, newStyle);
			newItem->itemText.setStyle(newItem->itemText.length()-1, newStyle);
			newItem->itemText.setCharStyle(newItem->itemText.length()-1, 1, lastStyle->Style);
		}
		else if (tName == "trail")
		{
			ParagraphStyle newStyle;
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
		{
			CharStyle newStyle;
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::NBHYPHEN);
			readCharacterStyleAttrs(doc, tAtt, newStyle);
			newItem->itemText.setCharStyle(newItem->itemText.length()-1, 1, newStyle);
			lastStyle->StyleStart = newItem->itemText.length()-1;
			lastStyle->Style = newStyle;
		}
		else if (tName == "nbspace")
		{
			CharStyle newStyle;
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::NBSPACE);
			readCharacterStyleAttrs(doc, tAtt, newStyle);
			newItem->itemText.setCharStyle(newItem->itemText.length()-1, 1, newStyle);
			lastStyle->StyleStart = newItem->itemText.length()-1;
			lastStyle->Style = newStyle;
		}
		else if (tName == "zwnbspace")
		{
			CharStyle newStyle;
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::ZWNBSPACE);
			readCharacterStyleAttrs(doc, tAtt, newStyle);
			newItem->itemText.setCharStyle(newItem->itemText.length()-1, 1, newStyle);
			lastStyle->StyleStart = newItem->itemText.length()-1;
			lastStyle->Style = newStyle;
		}
		else if (tName == "zwspace")
		{
			CharStyle newStyle;
			newItem->itemText.insertChars(newItem->itemText.length(), SpecialChars::ZWSPACE);
			readCharacterStyleAttrs(doc, tAtt, newStyle);
			newItem->itemText.setCharStyle(newItem->itemText.length()-1, 1, newStyle);
			lastStyle->StyleStart = newItem->itemText.length()-1;
			lastStyle->Style = newStyle;
		}
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
#ifdef HAVE_OSG
		if (tName == "OSGViews")
		{
			currentView.angleFOV = tAtt.valueAsDouble("angleFOV");
			QString tmp = "";
			tmp = tAtt.valueAsString("trackM");
			ScTextStream fp(&tmp, QIODevice::ReadOnly);
			double m1, m2, m3, m4;
			double m5, m6, m7, m8;
			double m9, m10, m11, m12;
			double m13, m14, m15, m16;
			fp >> m1 >> m2 >> m3 >> m4;
			fp >> m5 >> m6 >> m7 >> m8;
			fp >> m9 >> m10 >> m11 >> m12;
			fp >> m13 >> m14 >> m15 >> m16;
			currentView.trackerMatrix.set(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16);
			tmp = "";
			tmp = tAtt.valueAsString("trackC");
			ScTextStream fp2(&tmp, QIODevice::ReadOnly);
			double v1, v2, v3;
			fp2 >> v1 >> v2 >> v3;
			currentView.trackerCenter.set(v1, v2, v3);
			tmp = "";
			tmp = tAtt.valueAsString("cameraP");
			ScTextStream fp3(&tmp, QIODevice::ReadOnly);
			fp3 >> v1 >> v2 >> v3;
			currentView.cameraPosition.set(v1, v2, v3);
			tmp = "";
			tmp = tAtt.valueAsString("cameraU");
			ScTextStream fp4(&tmp, QIODevice::ReadOnly);
			fp4 >> v1 >> v2 >> v3;
			currentView.cameraUp.set(v1, v2, v3);
			currentView.trackerDist = tAtt.valueAsDouble("trackerDist");
			currentView.trackerSize = tAtt.valueAsDouble("trackerSize");
			currentView.illumination = static_cast<PageItem_OSGFrame::LightType>(tAtt.valueAsInt("illumination"));
			currentView.rendermode = static_cast<PageItem_OSGFrame::RenderType>(tAtt.valueAsInt("rendermode"));
			currentView.addedTransparency = tAtt.valueAsDouble("trans");
			currentView.colorAC = QColor(tAtt.valueAsString("colorAC"));
			currentView.colorFC = QColor(tAtt.valueAsString("colorFC"));
			if (newItem->asOSGFrame())
				newItem->asOSGFrame()->viewMap.insert(tAtt.valueAsString("viewName"), currentView);
		}
#endif
		if (tName == "ImageEffect")
		{
			struct ImageEffect ef;
			ef.effectParameters = tAtt.valueAsString("Param");
			ef.effectCode = tAtt.valueAsInt("Code");
			newItem->effectsInUse.append(ef);
		}
		if (tName == "StoryText")
			readStoryText(doc, reader, newItem);
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
		if ((tName == "PAGEOBJECT") || (tName == "ITEM"))
		{
			if (newItem->asGroupFrame())
			{
			//	bool success = true;
				QList<PageItem*>* docItems = doc->Items;
				QList<PageItem*> groupItems;
				doc->Items = &groupItems;
				ItemInfo itemInfo;
				// #12313: set the 'loadPage' parameter to true in order to
				// avoid the change of page mode and the doc item lists switch
				// when loading groups in masterpages
				ReadObjectParams readObjectParams2;
				readObjectParams2.baseDir = readObjectParams.baseDir;
				readObjectParams2.itemKind = (itemKind == PageItem::PatternItem) ? PageItem::PatternItem : PageItem::StandardItem;
				readObjectParams2.loadingPage = true;
				readObject(doc, reader, readObjectParams2, itemInfo);
				for (int as = 0; as < groupItems.count(); ++as)
				{
					PageItem* currItem = groupItems.at(as);
					newItem->groupItemList.append(currItem);
					currItem->Parent = newItem;
					currItem->m_layerID = newItem->m_layerID;
					currItem->OwnPage = newItem->OwnPage;
					currItem->OnMasterPage = newItem->OnMasterPage;
				}
				doc->Items = docItems;
			}
		}
		if (tName == "MARK")
		{
			if (newItem->asTextFrame())
			{
				QString l = tAtt.valueAsString("label");
				MarkType t = (MarkType) tAtt.valueAsInt("type");
				Mark* mark = nullptr;
				if (m_Doc->isLoading())
				{
					mark = m_Doc->getMark(l, t);
				}
				else
				{	//doc is not loading so it is copy/paste task
					if (t == MARKVariableTextType)
						mark = m_Doc->getMark(l, t);
					else
					{
						//create copy of mark
						Mark* oldMark = m_Doc->getMark(l, t);
						if (oldMark == nullptr)
						{
							qWarning() << "wrong copy of oldMark";
							mark = m_Doc->newMark();
							mark->setType(t);
						}
						else
						{
							mark = m_Doc->newMark(oldMark);
							getUniqueName(l,doc->marksLabelsList(t), "_");
						}
						mark->label = l;
						if (t == MARKNoteMasterType)
						{  //create copy of note
							TextNote* old = mark->getNotePtr();
							TextNote* note = m_Doc->newNote(old->notesStyle());
							mark->setNotePtr(note);
							note->setMasterMark(mark);
							note->setSaxedText(old->saxedText());
							m_Doc->setNotesChanged(true);
						}
					}
				}
				if (mark == nullptr)
					qDebug() << "Undefinied mark label ["<< l << "] type " << t;
				else
				{
					//set pointer to item holds mark in his text
					CharStyle newStyle;
					if (t == MARKAnchorType)
						mark->setItemPtr(newItem);
					mark->OwnPage = newItem->OwnPage;
					newItem->itemText.insertMark(mark, newItem->itemText.length());
					readCharacterStyleAttrs(doc, tAtt, newStyle);
					newItem->itemText.setCharStyle(newItem->itemText.length() - 1, 1, newStyle);
					lastStyle->StyleStart = newItem->itemText.length() - 1;
					lastStyle->Style = newStyle;
				}
			}
		}
		if (tName == "WeldEntry")
		{
			PageItem::WeldingInfo wInf;
			wInf.weldItem = nullptr;
			wInf.weldPoint = FPoint(tAtt.valueAsDouble("WX", 0.0), tAtt.valueAsDouble("WY", 0.0));
			wInf.weldID = tAtt.valueAsInt("Target", 0);
			newItem->weldList.append(wInf);
		}
	}
	delete lastStyle;

	if (tabValues.count() > 0) 
	{
		ParagraphStyle newDefault(newItem->itemText.defaultStyle());
		newDefault.setTabValues(tabValues);
		newItem->itemText.setDefaultStyle(newDefault);
	}

	if (newItem->fill_gradient.stops() == 0)
	{
		const ScColor& col1 = doc->PageColors[doc->itemToolPrefs().shapeFillColor];
		const ScColor& col2 = doc->PageColors[doc->itemToolPrefs().shapeLineColor];
		newItem->fill_gradient.addStop(ScColorEngine::getRGBColor(col1, doc), 0.0, 0.5, 1.0, doc->itemToolPrefs().shapeFillColor, 100);
		newItem->fill_gradient.addStop(ScColorEngine::getRGBColor(col2, doc), 1.0, 0.5, 1.0, doc->itemToolPrefs().shapeLineColor, 100);
	}

	if (newItem->stroke_gradient.stops() == 0)
	{
		const ScColor& col1 = doc->PageColors[doc->itemToolPrefs().shapeFillColor];
		const ScColor& col2 = doc->PageColors[doc->itemToolPrefs().shapeLineColor];
		newItem->stroke_gradient.addStop(ScColorEngine::getRGBColor(col1, doc), 0.0, 0.5, 1.0, doc->itemToolPrefs().shapeFillColor, 100);
		newItem->stroke_gradient.addStop(ScColorEngine::getRGBColor(col2, doc), 1.0, 0.5, 1.0, doc->itemToolPrefs().shapeLineColor, 100);
	}

	if (newItem->mask_gradient.stops() == 0)
	{
		const ScColor& col1 = doc->PageColors[doc->itemToolPrefs().shapeFillColor];
		const ScColor& col2 = doc->PageColors[doc->itemToolPrefs().shapeLineColor];
		newItem->mask_gradient.addStop(ScColorEngine::getRGBColor(col1, doc), 0.0, 0.5, 1.0, doc->itemToolPrefs().shapeFillColor, 100);
		newItem->mask_gradient.addStop(ScColorEngine::getRGBColor(col2, doc), 1.0, 0.5, 1.0, doc->itemToolPrefs().shapeLineColor, 100);
	}
	if (newItem->GrType == 13)
	{
		if (!newItem->gradient().isEmpty())
			newItem->fill_gradient = doc->docGradients[newItem->gradient()];
		newItem->createConicalMesh();
	}

	if (newItem->asPathText())
		newItem->updatePolyClip();
#ifdef HAVE_OSG
	if (newItem->asImageFrame() || newItem->asLatexFrame() || newItem->asOSGFrame())
#else
	if (newItem->asImageFrame() || newItem->asLatexFrame())
#endif
	{
		if (!newItem->Pfile.isEmpty())
		{
			double imageXOffset = newItem->imageXOffset();
			double imageYOffset = newItem->imageYOffset();
			doc->loadPict(newItem->Pfile, newItem, false);
			newItem->setImageXYOffset(imageXOffset, imageYOffset);
			if (newItem->pixm.imgInfo.PDSpathData.contains(clipPath))
			{
				newItem->imageClip = newItem->pixm.imgInfo.PDSpathData[clipPath].copy();
				newItem->pixm.imgInfo.usedPath = clipPath;
				QTransform cl;
				cl.translate(newItem->imageXOffset()*newItem->imageXScale(), newItem->imageYOffset()*newItem->imageYScale());
				cl.scale(newItem->imageXScale(), newItem->imageYScale());
				newItem->imageClip.map(cl);
			}
			if (layerFound)
			{
				newItem->pixm.imgInfo.isRequest = true;
				doc->loadPict(newItem->Pfile, newItem, true);
				newItem->setImageXYOffset(imageXOffset, imageYOffset);
			}
		}
	}
	if (!readObjectParams.loadingPage)
		doc->setMasterPageMode(savedMasterPageMode);
	return !reader.hasError();
}

bool Scribus150Format::readPattern(ScribusDoc* doc, ScXmlStreamReader& reader, const QString& baseDir)
{
	ScPattern pat;
	ScXmlStreamAttributes attrs = reader.scAttributes();
	QString patternName = attrs.valueAsString("Name");
	bool success = true;
	isNewFormat = false;

	if (patternName.isEmpty())
	{
		reader.readToElementEnd();
		return true;
	}

	QStack< QList<PageItem*> > groupStack;
	QStack< QList<PageItem*> > groupStackP;
	QStack<int> groupStack2;
	QMap<int, PageItem*> TableID2;
	QList<PageItem*> TableItems2;
	QMap<int, PageItem*> WeldID;
	QList<PageItem*> WeldItems;

	pat.setDoc(doc);
	pat.width   = attrs.valueAsDouble("width", 0.0);
	pat.height  = attrs.valueAsDouble("height", 0.0);
	pat.scaleX  = attrs.valueAsDouble("scaleX", 0.0);
	pat.scaleY  = attrs.valueAsDouble("scaleY", 0.0);
	pat.xoffset = attrs.valueAsDouble("xoffset", 0.0);
	pat.yoffset = attrs.valueAsDouble("yoffset", 0.0);
	
	ReadObjectParams readObjectParams;
	readObjectParams.baseDir = baseDir;
	readObjectParams.itemKind = PageItem::PatternItem;
	readObjectParams.loadingPage = false;

	bool savedAlignGrid = m_Doc->SnapGrid;
	bool savedAlignGuides = m_Doc->SnapGuides;
	bool savedAlignElement = m_Doc->SnapElement;
	bool savedMasterPageMode = m_Doc->masterPageMode();
	m_Doc->SnapGrid  = false;
	m_Doc->SnapGuides = false;
	m_Doc->SnapElement = false;

	m_Doc->setMasterPageMode(false);
	int itemCount1 = m_Doc->Items->count();

	QStringRef tagName = reader.name();
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (!reader.isStartElement() || reader.name() != "PatternItem") 
			continue;

		ScXmlStreamAttributes tAtt = reader.attributes();
			
		ItemInfo itemInfo;

		success = readObject(doc, reader, readObjectParams, itemInfo);
		if (!success) break;

		itemInfo.item->OwnPage = -1 /*ownPage*/;
		itemInfo.item->OnMasterPage.clear();
		if (isNewFormat)
		{
			if (itemInfo.item->isTableItem)
				TableItems2.append(itemInfo.item);
			if (itemInfo.isWeldFlag)
				WeldItems.append(itemInfo.item);
		}
		else
		{
			if (itemInfo.item->isTableItem)
			{
				TableItems2.append(itemInfo.item);
				TableID2.insert(itemInfo.ownLink, itemInfo.item);
			}
			if (itemInfo.isWeldFlag)
			{
				WeldItems.append(itemInfo.item);
				WeldID.insert(itemInfo.ownWeld, itemInfo.item);
			}
		}
		if (groupStack.count() > 0)
		{
			groupStack.top().append(itemInfo.item);
			while (itemInfo.ownNr == groupStack2.top())
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
			groupStack2.push(itemInfo.groupLastItem + itemInfo.ownNr);
		}
	}

	doc->SnapGrid   = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
	doc->SnapElement = savedAlignElement;
	if (!success)
	{
		doc->setMasterPageMode(savedMasterPageMode);
		return false;
	}
	if (isNewFormat)
	{
		if (TableItems2.count() != 0)
		{
			for (int ttc = 0; ttc < TableItems2.count(); ++ttc)
			{
				PageItem* ta = TableItems2.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = LinkID[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = LinkID[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = LinkID[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = LinkID[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (WeldItems.count() != 0)
		{
			for (int ttc = 0; ttc < WeldItems.count(); ++ttc)
			{
				PageItem* ta = WeldItems.at(ttc);
				for (int i = 0 ; i < ta->weldList.count(); ++i)
				{
					PageItem::WeldingInfo wInf = ta->weldList.at(i);
					ta->weldList[i].weldItem = LinkID.value(wInf.weldID, 0);
					if (ta->weldList[i].weldItem == nullptr)
						ta->weldList.removeAt(i--);
				}
			}
		}
	}
	else
	{
		if (TableItems2.count() != 0)
		{
			for (int ttc = 0; ttc < TableItems2.count(); ++ttc)
			{
				PageItem* ta = TableItems2.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = TableID2[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = TableID2[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = TableID2[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = TableID2[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (WeldItems.count() != 0)
		{
			for (int ttc = 0; ttc < WeldItems.count(); ++ttc)
			{
				PageItem* ta = WeldItems.at(ttc);
				for (int i = 0 ; i < ta->weldList.count(); ++i)
				{
					PageItem::WeldingInfo wInf = ta->weldList.at(i);
					ta->weldList[i].weldItem = WeldID.value(wInf.weldID, 0);
					if (ta->weldList[i].weldItem == nullptr)
						ta->weldList.removeAt(i--);
				}
			}
		}
	}

	while (groupStackP.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackP.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->DocItems.removeOne(cItem);
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackP, &m_Doc->DocItems);
		if (!converted)
			gItem->groupItemList = gpL;
	}

	int itemCount2 = m_Doc->Items->count();
	if (itemCount2 > itemCount1)
	{
		PageItem* currItem = doc->Items->at(itemCount1), *newItem;
		double minx =  std::numeric_limits<double>::max();
		double miny =  std::numeric_limits<double>::max();
		double maxx = -std::numeric_limits<double>::max();
		double maxy = -std::numeric_limits<double>::max();
		for (int i = itemCount1; i < itemCount2; ++i)
		{
			currItem = doc->Items->at(i);
			double x1, x2, y1, y2;
			currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
			minx = qMin(minx, x1);
			miny = qMin(miny, y1);
			maxx = qMax(maxx, x2);
			maxy = qMax(maxy, y2);
		}
		for (int i = itemCount1; i < itemCount2; ++i)
		{
			newItem = doc->Items->takeAt(itemCount1);
			newItem->gXpos = newItem->xPos() - minx;
			newItem->gYpos = newItem->yPos() - miny;
			newItem->gWidth = maxx - minx;
			newItem->gHeight = maxy - miny;
			newItem->setXYPos(newItem->gXpos, newItem->gYpos, true);
			newItem->moveBy(pat.xoffset, pat.yoffset, true);
			newItem->gXpos += pat.xoffset;
			newItem->gYpos += pat.yoffset;
			pat.items.append(newItem);
		}
		pat.createPreview();
	}
	doc->docPatterns.insert(patternName, pat);

	doc->setMasterPageMode(savedMasterPageMode);
	return success;
}

bool Scribus150Format::readStoryText(ScribusDoc *doc, ScXmlStreamReader& reader, PageItem* item)
{
	QStringRef tagName = reader.name();
	ScXmlStreamAttributes attrs = reader.scAttributes();

	LastStyles * lastStyle = new LastStyles();
	while (!reader.atEnd() && !reader.hasError())
	{
		ScXmlStreamReader::TokenType tType = reader.readNext();
		if (reader.isEndElement() && tagName == reader.name())
			break;
		if (tType != ScXmlStreamReader::StartElement)
			continue;
		QStringRef tName = reader.name();
		ScXmlStreamAttributes tAtt = reader.scAttributes();

		if (tName == "DefaultStyle")
		{
			ParagraphStyle newStyle;
			readParagraphStyle(doc, reader, newStyle);
			item->itemText.setDefaultStyle(newStyle);
		}

		if (tName == "ITEXT")
			readItemText(item, tAtt, lastStyle);
		else if (tName == "para")
		{
			item->itemText.insertChars(item->itemText.length(), SpecialChars::PARSEP);
			ParagraphStyle newStyle;
			readParagraphStyle(doc, reader, newStyle);
			item->itemText.setStyle(item->itemText.length()-1, newStyle);
			item->itemText.setCharStyle(item->itemText.length()-1, 1, lastStyle->Style);
		}
		else if (tName == "trail")
		{
			ParagraphStyle newStyle;
			readParagraphStyle(doc, reader, newStyle);
			item->itemText.setStyle(item->itemText.length(), newStyle);
		}
		else if (tName == "tab")
		{
			CharStyle newStyle;
			item->itemText.insertChars(item->itemText.length(), SpecialChars::TAB);
			readCharacterStyleAttrs(doc, tAtt, newStyle);
			item->itemText.setCharStyle(item->itemText.length()-1, 1, newStyle);
			lastStyle->StyleStart = item->itemText.length()-1;
			lastStyle->Style = newStyle;
		}
		else if (tName == "breakline")
			item->itemText.insertChars(item->itemText.length(), SpecialChars::LINEBREAK);
		else if (tName == "breakcol")
			item->itemText.insertChars(item->itemText.length(), SpecialChars::COLBREAK);
		else if (tName == "breakframe")
			item->itemText.insertChars(item->itemText.length(), SpecialChars::FRAMEBREAK);
		else if (tName == "nbhyphen")
		{
			CharStyle newStyle;
			item->itemText.insertChars(item->itemText.length(), SpecialChars::NBHYPHEN);
			readCharacterStyleAttrs(doc, tAtt, newStyle);
			item->itemText.setCharStyle(item->itemText.length()-1, 1, newStyle);
			lastStyle->StyleStart = item->itemText.length()-1;
			lastStyle->Style = newStyle;
		}
		else if (tName == "nbspace")
		{
			CharStyle newStyle;
			item->itemText.insertChars(item->itemText.length(), SpecialChars::NBSPACE);
			readCharacterStyleAttrs(doc, tAtt, newStyle);
			item->itemText.setCharStyle(item->itemText.length()-1, 1, newStyle);
			lastStyle->StyleStart = item->itemText.length()-1;
			lastStyle->Style = newStyle;
		}
		else if (tName == "zwnbspace")
		{
			CharStyle newStyle;
			item->itemText.insertChars(item->itemText.length(), SpecialChars::ZWNBSPACE);
			readCharacterStyleAttrs(doc, tAtt, newStyle);
			item->itemText.setCharStyle(item->itemText.length()-1, 1, newStyle);
			lastStyle->StyleStart = item->itemText.length()-1;
			lastStyle->Style = newStyle;
		}
		else if (tName == "zwspace")
		{
			CharStyle newStyle;
			item->itemText.insertChars(item->itemText.length(), SpecialChars::ZWSPACE);
			readCharacterStyleAttrs(doc, tAtt, newStyle);
			item->itemText.setCharStyle(item->itemText.length()-1, 1, newStyle);
			lastStyle->StyleStart = item->itemText.length()-1;
			lastStyle->Style = newStyle;
		}
		else if (tName == "var")
		{
			CharStyle newStyle;
			if (tAtt.value("name") == "pgno")
				item->itemText.insertChars(item->itemText.length(), SpecialChars::PAGENUMBER);
			else
				item->itemText.insertChars(item->itemText.length(), SpecialChars::PAGECOUNT);
			readCharacterStyleAttrs(doc, tAtt, newStyle);
			item->itemText.setCharStyle(item->itemText.length()-1, 1, newStyle);
			lastStyle->StyleStart = item->itemText.length()-1;
			lastStyle->Style = newStyle;
		}

		if (tName == "MARK")
		{
			QString l = tAtt.valueAsString("label");
			MarkType t = (MarkType) tAtt.valueAsInt("type");
			Mark* mark = nullptr;
			if (m_Doc->isLoading())
			{
				mark = m_Doc->getMark(l, t);
			}
			else
			{	//doc is not loading so it is copy/paste task
				if (t == MARKVariableTextType)
					mark = m_Doc->getMark(l, t);
				else
				{
					//create copy of mark
					Mark* oldMark = m_Doc->getMark(l, t);
					if (oldMark == nullptr)
					{
						qWarning() << "wrong copy of oldMark";
						mark = m_Doc->newMark();
						mark->setType(t);
					}
					else
					{
						mark = m_Doc->newMark(oldMark);
						getUniqueName(l,doc->marksLabelsList(t), "_");
					}
					mark->label = l;
					if (t == MARKNoteMasterType)
					{  //create copy of note
						TextNote* old = mark->getNotePtr();
						TextNote* note = m_Doc->newNote(old->notesStyle());
						mark->setNotePtr(note);
						note->setMasterMark(mark);
						note->setSaxedText(old->saxedText());
						m_Doc->setNotesChanged(true);
					}
				}
			}
			if (mark == nullptr)
				qDebug() << "Undefinied mark label ["<< l << "] type " << t;
			else
			{
				//set pointer to item holds mark in his text
				CharStyle newStyle;
				if (t == MARKAnchorType)
					mark->setItemPtr(item);
				mark->OwnPage = item->OwnPage;
				item->itemText.insertMark(mark, item->itemText.length());
				readCharacterStyleAttrs(doc, tAtt, newStyle);
				item->itemText.setCharStyle(item->itemText.length() - 1, 1, newStyle);
				lastStyle->StyleStart = item->itemText.length() - 1;
				lastStyle->Style = newStyle;
			}
		}
	}
	delete lastStyle;

	return !reader.hasError();
}

bool Scribus150Format::readItemText(PageItem *obj, ScXmlStreamAttributes& attrs, LastStyles* last)
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
	QString pstylename = attrs.valueAsString("PSTYLE", "");		

	fixLegacyCharStyle(newStyle);
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
		if (ch == SpecialChars::OBJECT)
		{
			if (LinkID.contains(iobj))
			{
				if (FrameItems.contains(LinkID[iobj]))
				{
					int fIndex = doc->addToInlineFrames(LinkID[iobj]);
					obj->itemText.insertObject(pos, fIndex);
				}
			}
			else
			{
				if (doc->FrameItems.contains(iobj))
					obj->itemText.insertObject(pos, iobj);
				else
					qDebug() << QString("scribus150format: invalid inline frame used in text object : %1").arg(iobj);
			}
		}
		else if (ch == SpecialChars::SHYPHEN && pos > 0)
		{
//			qDebug() << QString("scribus150format: SHYPHEN at %1").arg(pos);
			// double SHY means user provided SHY, single SHY is automatic one
			if (obj->itemText.hasFlag(pos-1, ScLayout_HyphenationPossible))
			{
				obj->itemText.clearFlag(pos-1, ScLayout_HyphenationPossible);
				obj->itemText.insertChars(pos, QString(ch));
			}
			else
			{
				obj->itemText.setFlag(pos-1, ScLayout_HyphenationPossible);
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
	qDebug("scribus150format: read itemtext %d '%s'", obj->itemText.length(), dbg.latin1());
	*/
	ParagraphStyle pstyle;

	if (!last->ParaStyle.isEmpty()) { // Qt4 >= 0) {
		pstyle.setParent( last->ParaStyle );
		obj->itemText.applyStyle(obj->itemText.length()-1, pstyle);
	}

	return true;
}

bool Scribus150Format::readPageItemAttributes(PageItem* item, ScXmlStreamReader& reader)
{
	QStringRef tagName = reader.name();
	ObjAttrVector pageItemAttributes;
	while (!reader.atEnd() && !reader.hasError())
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

PageItem* Scribus150Format::pasteItem(ScribusDoc *doc, ScXmlStreamAttributes& attrs, const QString& baseDir, PageItem::ItemKind itemKind, int pageNr)
{
	int z = 0;
	struct ImageLoadRequest loadingInfo;
	PageItem::ItemType pt = static_cast<PageItem::ItemType>(attrs.valueAsInt("PTYPE"));
	bool isGroupFlag = attrs.valueAsBool("isGroupControl", false);
	if (isGroupFlag)
		pt = PageItem::Group;
	bool isNoteFrameFlag = attrs.valueAsBool("isNoteFrame", false);
	if (isNoteFrameFlag && (pt == PageItem::TextFrame))
		pt = PageItem::NoteFrame;
	double xf, yf;
	double x   = attrs.valueAsDouble("XPOS");
	double y   = attrs.valueAsDouble("YPOS");
	x = Xp + x - GrX;
	y = Yp + y - GrY;
	double w   = attrs.valueAsDouble("WIDTH");
	double h   = attrs.valueAsDouble("HEIGHT");
	double pw  = attrs.valueAsDouble("PWIDTH");
	double imageXOffset = attrs.valueAsDouble("LOCALX");
	double imageYOffset = attrs.valueAsDouble("LOCALY");
	double imageXScale  = attrs.valueAsDouble("LOCALSCX");
	double imageYScale  = attrs.valueAsDouble("LOCALSCY");
	QString Pcolor = attrs.valueAsString("PCOLOR");
	if (Pcolor.isEmpty())
		Pcolor = CommonStrings::None;
	QString Pcolor2 = attrs.valueAsString("PCOLOR2");
	if (Pcolor2.isEmpty())
		Pcolor2 = CommonStrings::None;
	QColor tmpc;
	PageItem *currItem=nullptr;
	QString tmp, clPath;
	switch (pt)
	{
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType1:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, x, y, w, h, pw, Pcolor, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		break;
	//
	case PageItem::ImageFrame:
	case PageItem::OSGFrame:
	case PageItem::LatexFrame: /*Everything that is valid for image frames is also valid for latex frames*/
		z = doc->itemAdd(pt, PageItem::Unspecified, x, y, w, h, 1, doc->itemToolPrefs().imageFillColor, doc->itemToolPrefs().imageStrokeColor, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		UndoManager::instance()->setUndoEnabled(false);
		currItem->ScaleType   = attrs.valueAsInt("SCALETYPE", 1);
		currItem->AspectRatio = attrs.valueAsInt("RATIO", 0);
		currItem->setImageXYScale(imageXScale, imageYScale);
		currItem->setImageXYOffset(imageXOffset, imageYOffset);
		currItem->setImageRotation(attrs.valueAsDouble("LOCALROT"));
//		if (!currItem->asLatexFrame())
#ifdef HAVE_OSG
		if ((currItem->asImageFrame() || currItem->asOSGFrame()) && (!currItem->asLatexFrame()))
#else
		if ((currItem->asImageFrame()) && (!currItem->asLatexFrame()))
#endif
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
					QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX." + inlineImageExt);
					tempFile->setAutoRemove(false);
					tempFile->open();
					QString fileName = getLongPathName(tempFile->fileName());
					tempFile->close();
					inlineImageData = qUncompress(QByteArray::fromBase64(inlineImageData));
					QFile outFil(fileName);
					if (outFil.open(QIODevice::WriteOnly))
					{
						outFil.write(inlineImageData);
						outFil.close();
						currItem->isInlineImage = true;
						currItem->Pfile = fileName;
						currItem->isTempFile = true;
					}
					delete tempFile;
				}
			}
			else
				currItem->Pfile = Relative2Path(attrs.valueAsString("PFILE"), baseDir);
#ifdef HAVE_OSG
			if (currItem->asOSGFrame())
			{
				PageItem_OSGFrame *osgframe = currItem->asOSGFrame();
				osgframe->modelFile = Relative2Path(attrs.valueAsString("modelFile"), baseDir);
				osgframe->currentView = attrs.valueAsString("currentViewName", "");
				osgframe->loadModel();
			}
#endif
		}
		currItem->ImageProfile    = attrs.valueAsString("PRFILE", "");
		currItem->ImageIntent     = (eRenderIntent) attrs.valueAsInt("IRENDER", 1);
		currItem->EmbeddedProfile = attrs.valueAsString("EPROF" , "");
		currItem->UseEmbedded = attrs.valueAsInt("EMBEDDED", 1);
		currItem->pixm.imgInfo.lowResType = attrs.valueAsInt("ImageRes", 1);
		currItem->pixm.imgInfo.actualPageNumber = attrs.valueAsInt("Pagenumber", 0);
		if ((currItem->OverrideCompressionMethod = attrs.hasAttribute("COMPRESSIONMETHOD")))
			currItem->CompressionMethodIndex = attrs.valueAsInt("COMPRESSIONMETHOD", 0);
		if ((currItem->OverrideCompressionQuality = attrs.hasAttribute("COMPRESSIONQUALITY")))
			currItem->CompressionQualityIndex = attrs.valueAsInt("COMPRESSIONQUALITY");
		currItem->setImageXYScale(imageXScale, imageYScale);
		currItem->setImageRotation(attrs.valueAsDouble("LOCALROT"));
		clPath = attrs.valueAsString("ImageClip", "");
		if (!clPath.isEmpty())
		{
			clipPath = clPath;
			layerFound = true;
		}
		currItem->setImageVisible( attrs.valueAsInt("PICART"));
		currItem->setLineWidth(pw);
		UndoManager::instance()->setUndoEnabled(true);
		break;
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType3:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, x, y, w, h, pw, Pcolor, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		break;
	//
	case PageItem::PathText:
		z = doc->itemAdd(PageItem::PathText, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Pcolor, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		break;
	case PageItem::NoteFrame:
	case PageItem::TextFrame:
		z = doc->itemAdd(pt, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Pcolor, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		break;
	case PageItem::Line:
		z = doc->itemAdd(PageItem::Line, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		break;
	case PageItem::Polygon:
		z = doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		break;
	case PageItem::PolyLine:
		z = doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		break;
	case PageItem::Symbol:
		z = doc->itemAdd(PageItem::Symbol, PageItem::Unspecified, x, y, w, h, 0, CommonStrings::None, CommonStrings::None, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		currItem->setPattern( attrs.valueAsString("pattern", "") );
		break;
	case PageItem::Group:
		z = doc->itemAdd(PageItem::Group, PageItem::Unspecified, x, y, w, h, 0, CommonStrings::None, CommonStrings::None, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		currItem->groupWidth = attrs.valueAsDouble("groupWidth", w);
		currItem->groupHeight = attrs.valueAsDouble("groupHeight", h);
		doc->GroupCounter++;
		break;
	case PageItem::RegularPolygon:
		z = doc->itemAdd(PageItem::RegularPolygon, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		break;
	case PageItem::Arc:
		z = doc->itemAdd(PageItem::Arc, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		break;
	case PageItem::Spiral:
		z = doc->itemAdd(PageItem::Spiral, PageItem::Unspecified, x, y, w, h, pw, Pcolor, Pcolor2, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2) 
			currItem->setOwnerPage(pageNr);
		break;
	case PageItem::Table:
		z = doc->itemAdd(PageItem::Table, PageItem::Unspecified, x, y, w, h, 0.0, CommonStrings::None, CommonStrings::None, itemKind);
		currItem = doc->Items->at(z);
		if (pageNr > -2)
			currItem->setOwnerPage(pageNr);
		break;
	case PageItem::Multiple:
		Q_ASSERT(false);
		break;
	}

	currItem->setGroupClipping(attrs.valueAsBool("groupClips", true));
	currItem->FrameType = attrs.valueAsInt("FRTYPE", 0);
	int startArrowIndex = attrs.valueAsInt("startArrowIndex", 0);
	if ((startArrowIndex < 0) || (startArrowIndex > static_cast<int>(doc->arrowStyles().size())))
	{
		qDebug() << QString("scribus150format: invalid arrow index: %").arg(startArrowIndex);
		startArrowIndex = 0;
	}
	currItem->setStartArrowIndex(startArrowIndex);
	int endArrowIndex = attrs.valueAsInt("endArrowIndex", 0);
	if ((endArrowIndex < 0) || (endArrowIndex > static_cast<int>(doc->arrowStyles().size())))
	{
		qDebug() << QString("scribus150format: invalid arrow index: %").arg(endArrowIndex);
		endArrowIndex = 0;
	}
	currItem->setEndArrowIndex(endArrowIndex);
	currItem->setStartArrowScale(attrs.valueAsInt("startArrowScale", 100));
	currItem->setEndArrowScale(attrs.valueAsInt("endArrowScale", 100));
	currItem->NamedLStyle = attrs.valueAsString("NAMEDLST", "");
	currItem->isBookmark  = attrs.valueAsInt("BOOKMARK", 0);
	if ((currItem->isBookmark) && (doc->BookMarks.count() == 0))
		doc->OldBM = true;
	currItem->setImageFlippedH( attrs.valueAsInt("FLIPPEDH", 0));
	currItem->setImageFlippedV( attrs.valueAsInt("FLIPPEDV", 0));
	currItem->setCornerRadius( attrs.valueAsDouble("RADRECT", 0.0));
	currItem->ClipEdited = attrs.valueAsInt("CLIPEDIT", 0);
	currItem->setFillColor(Pcolor);
	currItem->setLineColor(Pcolor2);
	currItem->setFillShade(attrs.valueAsInt("SHADE", 100));
	currItem->setLineShade(attrs.valueAsInt("SHADE2", 100));

	ParagraphStyle pstyle;
	if (attrs.hasAttribute("LINESP"))
		pstyle.setLineSpacing(attrs.valueAsDouble("LINESP"));
	if (attrs.hasAttribute("LINESPMode"))
		pstyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(attrs.valueAsInt("LINESPMode", 0)));
	if (attrs.hasAttribute("ALIGN"))
		pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(attrs.valueAsInt("ALIGN", 0)));
	if (attrs.valueAsBool("REVERS"))
		pstyle.setDirection(ParagraphStyle::RTL);
	if (attrs.hasAttribute("DIRECTION"))
		pstyle.setDirection(static_cast<ParagraphStyle::DirectionType>(attrs.valueAsInt("DIRECTION", 0)));
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
	if (attrs.hasAttribute("TXTFEATURES"))
		pstyle.charStyle().setFeatures(attrs.valueAsString("TXTFEATURES").split(" ", QString::SkipEmptyParts));
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
	if (attrs.hasAttribute("keepLinesStart"))
		pstyle.setKeepLinesStart(attrs.valueAsInt("keepLinesStart"));
	if (attrs.hasAttribute("keepLinesEnd"))
		pstyle.setKeepLinesEnd(attrs.valueAsInt("keepLinesEnd"));
	if (attrs.hasAttribute("keepWithNext"))
		pstyle.setKeepWithNext(attrs.valueAsBool("keepWithNext"));
	if (attrs.hasAttribute("keepTogether"))
		pstyle.setKeepTogether(attrs.valueAsBool("keepTogether"));
	if (attrs.hasAttribute("ParagraphEffectCharStyle"))
		pstyle.setPeCharStyleName(attrs.valueAsString("ParagraphEffectCharStyle"));
	if (attrs.hasAttribute("ParagraphEffectOffset"))
		pstyle.setParEffectOffset(attrs.valueAsDouble("ParagraphEffectOffset"));
	if (attrs.hasAttribute("ParagraphEffectIndent"))
		pstyle.setParEffectIndent(attrs.valueAsDouble("ParagraphEffectIndent"));
	if (attrs.hasAttribute("DROP"))
		pstyle.setHasDropCap(static_cast<bool>(attrs.valueAsInt("DROP")));
	if (attrs.hasAttribute("DROPLIN"))
		pstyle.setDropCapLines(attrs.valueAsInt("DROPLIN"));
	if (attrs.hasAttribute("DROPDIST"))
		pstyle.setParEffectOffset(attrs.valueAsDouble("DROPDIST"));
	if (attrs.hasAttribute("Bullet"))
		pstyle.setHasBullet(static_cast<bool>(attrs.valueAsInt("Bullet")));
	if (attrs.hasAttribute("BulletStr"))
		pstyle.setBulletStr(attrs.valueAsString("BulletStr"));
	if (attrs.hasAttribute("Numeration"))
		pstyle.setHasNum(static_cast<bool>(attrs.valueAsInt("Numeration")));
	if (attrs.hasAttribute("NumerationName"))
		pstyle.setNumName(attrs.valueAsString("NumerationName"));
	if (attrs.hasAttribute("NumerationFormat"))
		pstyle.setNumFormat(attrs.valueAsInt("NumerationFormat"));
	if (attrs.hasAttribute("NumerationLevel"))
		pstyle.setNumLevel(attrs.valueAsInt("NumerationLevel"));
	if (attrs.hasAttribute("NumerationStart"))
		pstyle.setNumStart(attrs.valueAsInt("NumerationStart"));
	if (attrs.hasAttribute("NumerationPrefix"))
		pstyle.setNumPrefix(attrs.valueAsString("NumerationPrefix"));
	if (attrs.hasAttribute("NumerationSuffix"))
		pstyle.setNumSuffix(attrs.valueAsString("NumerationSuffix"));
	if (attrs.hasAttribute("NumerationRestart"))
	{
		NumerationRange numRange = (NumerationRange) attrs.valueAsInt("NumerationRestart");
		// Fix deprecated numeration ranges
		if (numRange != NSRdocument && numRange != NSRstory)
			numRange = NSRstory;
		pstyle.setNumRestart(static_cast<int>(numRange));
	}
	if (attrs.hasAttribute("NumeartionOther"))
		pstyle.setNumOther(static_cast<bool>(attrs.valueAsInt("NumeartionOther")));
	if (attrs.hasAttribute("NumerationHigher"))
		pstyle.setNumHigher(static_cast<bool>(attrs.valueAsInt("NumerationHigher")));
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
	currItem->setRotation( attrs.valueAsDouble("ROT", 0.0) );
	currItem->oldRot = currItem->rotation();
	currItem->setTextToFrameDist(attrs.valueAsDouble("EXTRA", 0.0),
								attrs.valueAsDouble("REXTRA", 0.0),
								attrs.valueAsDouble("TEXTRA", 0.0),
								attrs.valueAsDouble("BEXTRA", 0.0));
	currItem->setVerticalAlignment(attrs.valueAsInt("VAlign", 0));
	currItem->setFirstLineOffset(static_cast<FirstLineOffsetPolicy>(attrs.valueAsInt("FLOP")));

	currItem->PLineArt  = Qt::PenStyle(attrs.valueAsInt("PLINEART", 0));
	currItem->PLineEnd  = Qt::PenCapStyle(attrs.valueAsInt("PLINEEND", 0));
	currItem->PLineJoin = Qt::PenJoinStyle(attrs.valueAsInt("PLINEJOIN", 0));
	currItem->setPrintEnabled( attrs.valueAsInt("PRINTABLE", 1));
	currItem->setIsAnnotation( attrs.valueAsInt("ANNOTATION", 0));
	currItem->annotation().setType( attrs.valueAsInt("ANTYPE", 0));
	QString itemName = attrs.valueAsString("ANNAME","");
	if (!itemName.isEmpty())
	{
		if (currItem->itemName() == itemName)
			currItem->AutoName = true;
		else
		{
			currItem->setItemName(itemName);
			currItem->AutoName = false;
		}
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
	currItem->annotation().setBorderWidth( attrs.valueAsInt("ANBWID", 1));
	currItem->annotation().setBorderStyle( attrs.valueAsInt("ANBSTY", 0));
	currItem->annotation().setFeed( attrs.valueAsInt("ANFEED", 1));
	currItem->annotation().setFlag( attrs.valueAsInt("ANFLAG", 0));
	currItem->annotation().setFont( attrs.valueAsInt("ANFONT", 4));
	currItem->annotation().setFormat( attrs.valueAsInt("ANFORMAT", 0));
	currItem->annotation().setVis( attrs.valueAsInt("ANVIS", 0));
	currItem->annotation().setIsChk( attrs.valueAsBool("ANCHK", false) );
	currItem->annotation().setCheckState(currItem->annotation().IsChk());
	currItem->annotation().setAAact( attrs.valueAsBool("ANAA", false) );
	currItem->annotation().setHTML ( attrs.valueAsInt("ANHTML", 0));
	currItem->annotation().setUseIcons( attrs.valueAsBool("ANICON", false));
	currItem->annotation().setChkStil ( attrs.valueAsInt("ANCHKS", 0));
	currItem->annotation().setMaxChar ( attrs.valueAsInt("ANMC", -1));
	currItem->annotation().setBorderColor( attrs.valueAsString("ANBCOL", CommonStrings::None));
	currItem->annotation().setIPlace(attrs.valueAsInt("ANPLACE", 1));
	currItem->annotation().setScaleW(attrs.valueAsInt("ANSCALE", 0));
	currItem->annotation().setIcon(attrs.valueAsInt("ANITYP", 0));
	currItem->annotation().setAnOpen(attrs.valueAsBool("ANOPEN", false) );

	if (currItem->asTextFrame() || currItem->asPathText())
	{
		UndoManager::instance()->setUndoEnabled(false);
		if (currItem->isAnnotation() && currItem->annotation().UseIcons())
		{
			currItem->setImageXYScale(imageXScale, imageYScale);
			currItem->setImageXYOffset(imageXOffset, imageYOffset);
			currItem->setImageRotation(attrs.valueAsDouble("LOCALROT"));
			currItem->Pfile  = Relative2Path(attrs.valueAsString("PFILE" , ""), baseDir);
			currItem->Pfile2 = Relative2Path(attrs.valueAsString("PFILE2", ""), baseDir);
			currItem->Pfile3 = Relative2Path(attrs.valueAsString("PFILE3", ""), baseDir);
			currItem->ImageProfile    = attrs.valueAsString("PRFILE", "");
			currItem->ImageIntent     = (eRenderIntent) attrs.valueAsInt("IRENDER" , 1);
			currItem->EmbeddedProfile = attrs.valueAsString("EPROF", "");
			currItem->UseEmbedded = attrs.valueAsInt("EMBEDDED", 1);
			doc->loadPict(currItem->Pfile, currItem);
			currItem->setImageXYScale(imageXScale, imageYScale);
			currItem->setImageVisible( attrs.valueAsInt("PICART"));
/*			currItem->BBoxX = ScCLocale::toDoubleC( obj->attribute("BBOXX"));
			currItem->BBoxH = ScCLocale::toDoubleC( obj->attribute("BBOXH")); */
			currItem->ScaleType   = attrs.valueAsInt("SCALETYPE", 1);
			currItem->AspectRatio = attrs.valueAsInt("RATIO", 0);
		}
		UndoManager::instance()->setUndoEnabled(true);
	}

	if (currItem->asTable())
	{
		doc->dontResize = true;
		PageItem_Table *tableitem = currItem->asTable();
		int rows=attrs.valueAsInt("Rows",1);
		int cols=attrs.valueAsInt("Columns",1);
		tableitem->insertRows(1,rows-1);
		tableitem->insertColumns(1,cols-1);
		tableitem->setStyle(attrs.valueAsString("TableStyle"));
//		QString rowPositions(attrs.valueAsString("RowPositions"));
//		QStringList slRowPositions=rowPositions.split(" ");
//		qDebug()<<"RowCount"<<rows<<"row positions"<<slRowPositions.count();
		doc->dontResize = true;

		QString rowHeights(attrs.valueAsString("RowHeights",""));
		if (!rowHeights.isEmpty())
		{
			QStringList slRowHeights=rowHeights.split(" ");
			int i=0;
			foreach(const QString& pos, slRowHeights)
				tableitem->resizeRow(i++, pos.toDouble());
		}
//		QString colPositions(attrs.valueAsString("ColumnPositions"));
//		QStringList slColPositions=colPositions.split(" ");
		QString colWidths(attrs.valueAsString("ColumnWidths",""));
		if (!colWidths.isEmpty())
		{
			QStringList slColWidths=colWidths.split(" ");
			int i=0;
			foreach(const QString& pos, slColWidths)
				tableitem->resizeColumn(i++, pos.toDouble());
		}
		QString cellAreas(attrs.valueAsString("CellAreas"));
		if (!cellAreas.isEmpty())
		{
			QStringList slCellAreas=cellAreas.split(" ");
			if (slCellAreas.count()%4!=0)
				qDebug()<<"Cell Area Count on load ! % 4";
			for (int i = 0; i < slCellAreas.size(); i+=4)
			{
				int rows=slCellAreas.at(i).toInt();
				int columns=slCellAreas.at(i+1).toInt();
				int height=slCellAreas.at(i+2).toInt();
				int width=slCellAreas.at(i+3).toInt();
				tableitem->mergeCells(rows,columns,height,width);
			}
		}
		doc->dontResize = false;
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
	else if ( attrs.valueAsInt("TEXTFLOW", 0) != 0)
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
	currItem->setLocked (attrs.valueAsBool("LOCK", false));
	currItem->setSizeLocked(attrs.valueAsBool("LOCKR", false));
	currItem->fillRule    = attrs.valueAsBool("fillRule", true);
	currItem->doOverprint = attrs.valueAsBool("doOverprint", false);
	currItem->setFillTransparency(attrs.valueAsDouble("TransValue", 0.0));
	currItem->setLineTransparency(attrs.valueAsDouble("TransValueS", 0.0));
	currItem->setFillBlendmode(attrs.valueAsInt("TransBlend", 0));
	currItem->setLineBlendmode(attrs.valueAsInt("TransBlendS", 0));
	if (attrs.valueAsInt("TRANSPARENT", 0) == 1)
		currItem->setFillColor(CommonStrings::None);
	currItem->m_columns   = attrs.valueAsInt("COLUMNS", 1);
	currItem->m_columnGap = attrs.valueAsDouble("COLGAP", 0.0);
	if (attrs.valueAsInt("LAYER", 0) != -1)
	{
		currItem->setLayer(attrs.valueAsInt("LAYER", 0));
		uint layerCount = doc->Layers.count();
		bool found = false;
		for (uint i = 0; i < layerCount; ++i)
		{
			if (doc->Layers[i].ID == currItem->m_layerID)
			{
				found = true;
				break;
			}
		}
		if (!found)
			currItem->setLayer(doc->firstLayerID());
	}
	tmp = "";
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
	currItem->DashOffset = attrs.valueAsDouble("DASHOFF", 0.0);

	if (currItem->asRegularPolygon())
	{
		PageItem_RegularPolygon *regitem = currItem->asRegularPolygon();
		regitem->polyCorners      = attrs.valueAsInt("POLYC", 4);
		regitem->polyFactor       = attrs.valueAsDouble("POLYF", 0.5);
		regitem->polyRotation     = attrs.valueAsDouble("POLYR", 0.0);
		regitem->polyInnerRot     = attrs.valueAsDouble("POLYIR", 0.0);
		regitem->polyCurvature    = attrs.valueAsDouble("POLYCUR", 0.0);
		regitem->polyOuterCurvature    = attrs.valueAsDouble("POLYOCUR", 0.0);
		regitem->polyUseFactor    = attrs.valueAsBool("POLYS", false);
		regitem->recalcPath();
	}
	else if (currItem->asArc())
	{
		PageItem_Arc *arcitem = currItem->asArc();
		arcitem->arcHeight     = attrs.valueAsDouble("arcHeight", 1.0);
		arcitem->arcWidth      = attrs.valueAsDouble("arcWidth", 1.0);
		arcitem->arcStartAngle = attrs.valueAsDouble("arcStartAngle", 30.0);
		arcitem->arcSweepAngle = attrs.valueAsDouble("arcSweepAngle", 300.0);
		arcitem->recalcPath();
	}
	else if (currItem->asSpiral())
	{
		PageItem_Spiral *arcitem = currItem->asSpiral();
		arcitem->FrameType = 3; // Workaround for old docs, otherwise undo breaks spirals
		arcitem->spiralStartAngle = attrs.valueAsDouble("spiralStartAngle", 0.0);
		arcitem->spiralEndAngle = attrs.valueAsDouble("spiralEndAngle", 360.0);
		arcitem->spiralFactor = attrs.valueAsDouble("spiralFactor", 1.2);
		arcitem->recalcPath();
	}
	else
	{
		tmp = "";
		if (attrs.hasAttribute("NUMPO"))
		{
			currItem->PoLine.resize(attrs.valueAsUInt("NUMPO"));
			tmp = attrs.valueAsString("POCOOR");
			double maxVal = std::numeric_limits<double>::max() / 2.0;
			ScTextStream fp(&tmp, QIODevice::ReadOnly);
			uint numPo = attrs.valueAsUInt("NUMPO");
			for (uint cx=0; cx < numPo; ++cx)
			{
				fp >> xf;
				fp >> yf;
				if (xf >= 999999)
					xf = maxVal;
				if (yf >= 999999)
					yf = maxVal;
				currItem->PoLine.setPoint(cx, xf, yf);
			}
		}
		else
		{
			currItem->PoLine.resize(0);
			currItem->PoLine.parseSVG(attrs.valueAsString("path"));
		}
	}
	tmp = "";
	if (attrs.hasAttribute("NUMCO"))
	{
		currItem->ContourLine.resize(attrs.valueAsUInt("NUMCO"));
		tmp = attrs.valueAsString("COCOOR");
		double maxVal = std::numeric_limits<double>::max() / 2.0;
		ScTextStream fp(&tmp, QIODevice::ReadOnly);
		uint numCo = attrs.valueAsUInt("NUMCO");
		for (uint cx=0; cx < numCo; ++cx)
		{
			fp >> xf;
			fp >> yf;
			if (xf >= 999999)
				xf = maxVal;
			if (yf >= 999999)
				yf = maxVal;
			currItem->ContourLine.setPoint(cx, xf, yf);
		}
	}
	else if (attrs.hasAttribute("copath"))
	{
		currItem->ContourLine.resize(0);
		currItem->ContourLine.parseSVG(attrs.valueAsString("copath"));
	}
	else
		currItem->ContourLine = currItem->PoLine.copy();

	if (!currItem->asLine())
		currItem->Clip = flattenPath(currItem->PoLine, currItem->Segments);
	else
	{
		currItem->Segments.clear();
		currItem->PoLine.resize(0);
		currItem->setHeight(1.0);
		currItem->asLine()->setLineClip();
	}

	if (currItem->asPathText())
		currItem->updatePolyClip();
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
			double patternSkewX    = attrs.valueAsDouble("pSkewX", 0.0);
			double patternSkewY    = attrs.valueAsDouble("pSkewY", 0.0);
			currItem->setPatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
			bool mirrorX = attrs.valueAsBool("pMirrorX", false);
			bool mirrorY = attrs.valueAsBool("pMirrorY", false);
			currItem->setPatternFlip(mirrorX, mirrorY);
		}
		else if (currItem->GrType == 11)
		{
			currItem->meshGradientArray.clear();
			int mGArrayRows = attrs.valueAsInt("GMAX", 1);
			int mGArrayCols = attrs.valueAsInt("GMAY", 1);
			for (int mgr = 0; mgr < mGArrayRows; mgr++)
			{
				QList<MeshPoint> ml;
				for (int mgc = 0; mgc < mGArrayCols; mgc++)
				{
					MeshPoint mp;
					ml.append(mp);
				}
				currItem->meshGradientArray.append(ml);
			}
		}
		else if (currItem->GrType == 12)
		{
			currItem->meshGradientPatches.clear();
			int mGArrayRows = attrs.valueAsInt("GMAX", 1);
			for (int mgr = 0; mgr < mGArrayRows; mgr++)
			{
				meshGradientPatch patchM;
				currItem->meshGradientPatches.append(patchM);
			}
		}
		else if (currItem->GrType == 14)
		{
			int hatchType = attrs.valueAsInt("HatchMode", 0);
			double hatchDistance = attrs.valueAsDouble("HatchDist", 2);
			double hatchAngle = attrs.valueAsDouble("HatchAngle", 0);
			bool hatchUseBackground = attrs.valueAsBool("HatchSolidB", false);
			QString hatchBackground = attrs.valueAsString("HatchBackG", CommonStrings::None);
			QString hatchForeground = attrs.valueAsString("HatchForeC", CommonStrings::None);
			currItem->setHatchParameters(hatchType, hatchDistance, hatchAngle, hatchUseBackground, hatchBackground, hatchForeground);
		}
		else
		{
			currItem->GrStartX = attrs.valueAsDouble("GRSTARTX", 0.0);
			currItem->GrStartY = attrs.valueAsDouble("GRSTARTY", 0.0);
			currItem->GrEndX   = attrs.valueAsDouble("GRENDX", currItem->width());
			currItem->GrEndY   = attrs.valueAsDouble("GRENDY", 0.0);
			currItem->GrFocalX = attrs.valueAsDouble("GRFOCALX", 0.0);
			currItem->GrFocalY = attrs.valueAsDouble("GRFOCALY", 0.0);
			currItem->GrScale  = attrs.valueAsDouble("GRSCALE", 1.0);
			currItem->GrSkew  = attrs.valueAsDouble("GRSKEW", 0.0);
			GrColor = attrs.valueAsString("GRCOLOR","");
			if (!GrColor.isEmpty())
			{
				GrColor2 = attrs.valueAsString("GRCOLOR2","");
				GrShade  = attrs.valueAsInt("GRSHADE", 100);
				GrShade2 = attrs.valueAsInt("GRSHADE2", 100);
			}
			QString GrName = "";
			GrName = attrs.valueAsString("GRNAME","");
			if (!GrName.isEmpty())
				currItem->setGradient(GrName);
			if ((currItem->GrType == 9) || (currItem->GrType == 10))
			{
				currItem->GrControl1 = FPoint(attrs.valueAsDouble("GRC1X", 0.0), attrs.valueAsDouble("GRC1Y", 0.0));
				currItem->GrControl2 = FPoint(attrs.valueAsDouble("GRC2X", 0.0), attrs.valueAsDouble("GRC2Y", 0.0));
				currItem->GrControl3 = FPoint(attrs.valueAsDouble("GRC3X", 0.0), attrs.valueAsDouble("GRC3Y", 0.0));
				currItem->GrControl4 = FPoint(attrs.valueAsDouble("GRC4X", 0.0), attrs.valueAsDouble("GRC4Y", 0.0));
				currItem->GrControl5 = FPoint(attrs.valueAsDouble("GRC5X", 0.0), attrs.valueAsDouble("GRC5Y", 0.0));
				currItem->GrColorP1  = attrs.valueAsString("GRCOLP1", "Black");
				currItem->GrColorP2  = attrs.valueAsString("GRCOLP2", "Black");
				currItem->GrColorP3  = attrs.valueAsString("GRCOLP3", "Black");
				currItem->GrColorP4  = attrs.valueAsString("GRCOLP4", "Black");
				currItem->GrCol1transp  = attrs.valueAsDouble("GRCOLT1", 1.0);
				currItem->GrCol2transp  = attrs.valueAsDouble("GRCOLT2", 1.0);
				currItem->GrCol3transp  = attrs.valueAsDouble("GRCOLT3", 1.0);
				currItem->GrCol4transp  = attrs.valueAsDouble("GRCOLT4", 1.0);
				currItem->GrCol1Shade  = attrs.valueAsInt("GRCOLS1", 100);
				currItem->GrCol2Shade  = attrs.valueAsInt("GRCOLS2", 100);
				currItem->GrCol3Shade  = attrs.valueAsInt("GRCOLS3", 100);
				currItem->GrCol4Shade  = attrs.valueAsInt("GRCOLS4", 100);
				currItem->set4ColorColors(currItem->GrColorP1, currItem->GrColorP2, currItem->GrColorP3, currItem->GrColorP4);
			}
		}
	}
	if (((currItem->GrType != 0) && (currItem->GrType != 8) && (currItem->GrType != 14)) && (currItem->gradient().isEmpty()))
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
	currItem->setStrokePattern( attrs.valueAsString("patternS", "") );
	double patternScaleX   = attrs.valueAsDouble("pScaleXS", 100.0);
	double patternScaleY   = attrs.valueAsDouble("pScaleYS", 100.0);
	double patternOffsetX  = attrs.valueAsDouble("pOffsetXS", 0.0);
	double patternOffsetY  = attrs.valueAsDouble("pOffsetYS", 0.0);
	double patternRotation = attrs.valueAsDouble("pRotationS", 0.0);
	double patternSkewX    = attrs.valueAsDouble("pSkewXS", 0.0);
	double patternSkewY    = attrs.valueAsDouble("pSkewYS", 0.0);
	double patternSpace    = attrs.valueAsDouble("pSpaceS", 1.0);
	currItem->setStrokePatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace);
	bool mirrorX = attrs.valueAsBool("pMirrorXS", false);
	bool mirrorY = attrs.valueAsBool("pMirrorYS", false);
	bool atPath = attrs.valueAsBool("pAtPathS", false);
	currItem->setPatternFlip(mirrorX, mirrorY);
	currItem->setStrokePatternToPath(atPath);
	currItem->GrTypeStroke = attrs.valueAsInt("GRTYPS", 0);
	if (((currItem->GrTypeStroke != 0) && (currItem->GrTypeStroke != 8)) && (currItem->strokeGradient().isEmpty()))
		currItem->stroke_gradient.clearStops();
	currItem->GrStrokeStartX = attrs.valueAsDouble("GRSTARTXS", 0.0);
	currItem->GrStrokeStartY = attrs.valueAsDouble("GRSTARTYS", 0.0);
	currItem->GrStrokeEndX   = attrs.valueAsDouble("GRENDXS", currItem->width());
	currItem->GrStrokeEndY   = attrs.valueAsDouble("GRENDYS", 0.0);
	currItem->GrStrokeFocalX = attrs.valueAsDouble("GRFOCALXS", 0.0);
	currItem->GrStrokeFocalY = attrs.valueAsDouble("GRFOCALYS", 0.0);
	currItem->GrStrokeScale  = attrs.valueAsDouble("GRSCALES", 1.0);
	currItem->GrStrokeSkew  = attrs.valueAsDouble("GRSKEWS", 0.0);
	QString GrNameS = "";
	GrNameS = attrs.valueAsString("GRNAMES","");
	if (!GrNameS.isEmpty())
		currItem->setStrokeGradient(GrNameS);


	currItem->setPatternMask( attrs.valueAsString("patternM", "") );
	double patternScaleXm   = attrs.valueAsDouble("pScaleXM", 100.0);
	double patternScaleYm   = attrs.valueAsDouble("pScaleYM", 100.0);
	double patternOffsetXm  = attrs.valueAsDouble("pOffsetXM", 0.0);
	double patternOffsetYm  = attrs.valueAsDouble("pOffsetYM", 0.0);
	double patternRotationm = attrs.valueAsDouble("pRotationM", 0.0);
	double patternSkewXm    = attrs.valueAsDouble("pSkewXM", 0.0);
	double patternSkewYm    = attrs.valueAsDouble("pSkewYM", 0.0);
	currItem->setMaskTransform(patternScaleXm, patternScaleYm, patternOffsetXm, patternOffsetYm, patternRotationm, patternSkewXm, patternSkewYm);
	bool mirrorXm = attrs.valueAsBool("pMirrorXM", false);
	bool mirrorYm = attrs.valueAsBool("pMirrorYM", false);
	currItem->setMaskFlip(mirrorXm, mirrorYm);
	QString GrNameM = "";
	GrNameM = attrs.valueAsString("GRNAMEM","");
	currItem->GrMask = attrs.valueAsInt("GRTYPM", 0);
	if ((currItem->GrMask == 1) || (currItem->GrMask == 4))
		currItem->mask_gradient = VGradient(VGradient::linear);
	else if ((currItem->GrMask == 2) || (currItem->GrMask == 5))
		currItem->mask_gradient = VGradient(VGradient::radial);
	if (((currItem->GrMask == 1) || (currItem->GrMask == 2) || (currItem->GrMask == 4) || (currItem->GrMask == 5)) && (GrNameM.isEmpty()))
		currItem->mask_gradient.clearStops();
	currItem->GrMaskStartX = attrs.valueAsDouble("GRSTARTXM", 0.0);
	currItem->GrMaskStartY = attrs.valueAsDouble("GRSTARTYM", 0.0);
	currItem->GrMaskEndX   = attrs.valueAsDouble("GRENDXM", currItem->width());
	currItem->GrMaskEndY   = attrs.valueAsDouble("GRENDYM", 0.0);
	currItem->GrMaskFocalX = attrs.valueAsDouble("GRFOCALXM", 0.0);
	currItem->GrMaskFocalY = attrs.valueAsDouble("GRFOCALYM", 0.0);
	currItem->GrMaskScale  = attrs.valueAsDouble("GRSCALEM", 1.0);
	currItem->GrMaskSkew  = attrs.valueAsDouble("GRSKEWM", 0.0);
	if (!GrNameM.isEmpty())
		currItem->setGradientMask(GrNameM);
	if (attrs.hasAttribute("InID"))
		currItem->inlineCharID = attrs.valueAsInt("InID", -1);
	else
		currItem->inlineCharID = -1;
	currItem->setGradientExtend((VGradient::VGradientRepeatMethod)(attrs.valueAsInt("GRExt", VGradient::pad)));
	currItem->setStrokeGradientExtend((VGradient::VGradientRepeatMethod)(attrs.valueAsInt("GRExtS", VGradient::pad)));
	currItem->mask_gradient.setRepeatMethod((VGradient::VGradientRepeatMethod)(attrs.valueAsInt("GRExtM", VGradient::pad)));

	currItem->setHasSoftShadow(attrs.valueAsBool("HASSOFTSHADOW", false));
	currItem->setSoftShadowXOffset(attrs.valueAsDouble("SOFTSHADOWXOFFSET", 5.0));
	currItem->setSoftShadowYOffset(attrs.valueAsDouble("SOFTSHADOWYOFFSET", 5.0));
	currItem->setSoftShadowColor(attrs.valueAsString("SOFTSHADOWCOLOR", "Black"));
	currItem->setSoftShadowShade(attrs.valueAsInt("SOFTSHADOWSHADE", 100));
	currItem->setSoftShadowBlurRadius(attrs.valueAsDouble("SOFTSHADOWBLURRADIUS", 5.0));
	currItem->setSoftShadowBlendMode(attrs.valueAsInt("SOFTSHADOWBLENDMODE", 0));
	currItem->setSoftShadowOpacity(attrs.valueAsDouble("SOFTSHADOWOPACITY", 0.0));
	currItem->setSoftShadowErasedByObject(attrs.valueAsBool("SOFTSHADOWERASE", false));
	currItem->setSoftShadowHasObjectTransparency(attrs.valueAsBool("SOFTSHADOWOBJTRANS", false));

	//currItem->setRedrawBounding();
	//currItem->OwnPage = view->OnPage(currItem);
	return currItem;
}

bool Scribus150Format::readItemTableData(PageItem_Table* item, ScXmlStreamReader& reader, ScribusDoc* doc)
{
	ScXmlStreamAttributes attrs = reader.scAttributes();
	QString fColor = attrs.valueAsString("FillColor");
	if ((fColor != CommonStrings::None) && (!fColor.isEmpty()))
		item->setFillColor(fColor);
	item->setFillShade(attrs.valueAsInt("FillShade", 100));
	QStringRef tagName = reader.name();
	LastStyles lastStyle;
	doc->dontResize = true;
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (!reader.isStartElement())
			continue;
			
		if (reader.name() == "Cell")
		{
			readItemTableCell(item, reader, doc);
		}
		else if (reader.name() == "TableBorderLeft")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			item->setLeftBorder(border);
		}
		else if (reader.name() == "TableBorderRight")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			item->setRightBorder(border);
		}
		else if (reader.name() == "TableBorderTop")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			item->setTopBorder(border);
		}
		else if (reader.name() == "TableBorderBottom")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			item->setBottomBorder(border);
		}
		else
		{
			reader.skipCurrentElement();
		}
	}
	item->adjustTableToFrame();
	item->adjustFrameToTable();
	doc->dontResize = false;
	return !reader.hasError();
}

bool Scribus150Format::readItemTableCell(PageItem_Table* item, ScXmlStreamReader& reader, ScribusDoc *doc)
{
	QStringRef tagName = reader.name();
	ScXmlStreamAttributes tAtt = reader.scAttributes();
	
	int row = tAtt.valueAsInt("Row", -1);
	int col = tAtt.valueAsInt("Column", -1);
	if ((row >= 0) && (col >= 0))
	{
		if (tAtt.hasAttribute("Style"))
		{
			QString Style = tAtt.valueAsString("Style");
			if (!Style.isEmpty())
				item->cellAt(row, col).setStyle(Style);
		}
		QString fColor = tAtt.valueAsString("FillColor");
		if ((fColor != CommonStrings::None) && (!fColor.isEmpty()))
			item->cellAt(row, col).setFillColor(fColor);
		double fShade = tAtt.valueAsDouble("FillShade", -1.0);
		if (fShade >= 0 && fShade <= 100)
			item->cellAt(row, col).setFillShade(fShade);
		if (tAtt.hasAttribute("LeftPadding"))
			item->cellAt(row, col).setLeftPadding(tAtt.valueAsDouble("LeftPadding", 0.0));
		if (tAtt.hasAttribute("RightPadding"))
			item->cellAt(row, col).setRightPadding(tAtt.valueAsDouble("RightPadding", 0.0));
		if (tAtt.hasAttribute("TopPadding"))
			item->cellAt(row, col).setTopPadding(tAtt.valueAsDouble("TopPadding", 0.0));
		if (tAtt.hasAttribute("BottomPadding"))
			item->cellAt(row, col).setBottomPadding(tAtt.valueAsDouble("BottomPadding", 0.0));

		PageItem* newItem = item->cellAt(row, col).textFrame();
		newItem->m_columns   = tAtt.valueAsInt("TextColumns", 1);
		newItem->m_columnGap = tAtt.valueAsDouble("TextColGap", 0.0);
		newItem->setTextToFrameDist(tAtt.valueAsDouble("TextDistLeft", 0.0),
							tAtt.valueAsDouble("TextDistRight", 0.0),
							tAtt.valueAsDouble("TextDistTop", 0.0),
							tAtt.valueAsDouble("TextDistBottom", 0.0));
		newItem->setVerticalAlignment(tAtt.valueAsInt("TextVertAlign", 0));
		newItem->setFirstLineOffset(static_cast<FirstLineOffsetPolicy>(tAtt.valueAsInt("Flop")));
	}

	LastStyles lastStyle;
	while (!reader.atEnd() && !reader.hasError())
	{
		reader.readNext();
		if (reader.isEndElement() && reader.name() == tagName)
			break;
		if (!reader.isStartElement())
			continue;

		if (reader.name() == "TableBorderLeft")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			item->cellAt(row, col).setLeftBorder(border);
		}
		else if (reader.name() == "TableBorderRight")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			item->cellAt(row, col).setRightBorder(border);
		}
		else if (reader.name() == "TableBorderTop")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			item->cellAt(row, col).setTopBorder(border);
		}
		else if (reader.name() == "TableBorderBottom")
		{
			TableBorder border;
			QStringRef tagName = reader.name();
			while (!reader.atEnd() && !reader.hasError())
			{
				reader.readNext();
				if (reader.isEndElement() && reader.name() == tagName)
					break;
				if (reader.isStartElement() && reader.name() == "TableBorderLine")
				{
					ScXmlStreamAttributes tAttB = reader.scAttributes();
					double width = tAttB.valueAsDouble("Width", 0.0);
					QString color = tAttB.valueAsString("Color", CommonStrings::None);
					double shade = tAttB.valueAsDouble("Shade", 100.0);
					int style = tAttB.valueAsInt("PenStyle", 1);
					border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), color, shade));
				}
			}
			item->cellAt(row, col).setBottomBorder(border);
		}
		else if (reader.name() == "StoryText")
		{
			PageItem* newItem = item->cellAt(row, col).textFrame();
			readStoryText(doc, reader, newItem);
		}
		else
		{
			reader.skipCurrentElement();
		}
	}

	return !reader.hasError();
}

bool Scribus150Format::readLatexInfo(PageItem_LatexFrame* latexitem, ScXmlStreamReader& reader)
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

bool Scribus150Format::loadPage(const QString & fileName, int pageNumber, bool Mpage, const QString& renamedPageName)
{
// 	qDebug() << QString("loading page %2 from file '%1' from 1.3.x plugin").arg(fileName).arg(pageNumber);
	if (m_Doc==nullptr || m_AvailableFonts==nullptr)
	{
		Q_ASSERT(m_Doc==nullptr || m_AvailableFonts==nullptr);
		return false;
	}
	ParagraphStyle vg;
	Xp = 0.0;
	Yp = 0.0;
	GrX = 0.0;
	GrY = 0.0;
	struct ScribusDoc::BookMa bok;
	QMap<int, ScribusDoc::BookMa> bookmarks;

	ScPage* newPage = nullptr;
	
	QMap<int, PageItem*> TableID;
	QList<PageItem*> TableItems;
	QMap<int, PageItem*> WeldID;
	QList<PageItem*> WeldItems;
	QStack< QList<PageItem*> > groupStackFI;
	QStack< QList<PageItem*> > groupStackMI;
	QStack< QList<PageItem*> > groupStackPI;
	QStack< QList<PageItem*> > groupStackF;
	QStack< QList<PageItem*> > groupStackM;
	QStack< QList<PageItem*> > groupStackP;
	QStack<int> groupStackFI2;
	QStack<int> groupStackMI2;
	QStack<int> groupStackPI2;
	double pageX = 0, pageY = 0;
	QMap<int,int> layerTrans;
	int maxLayer = 0, maxLevel = 0, a = 0;

	layerTrans.clear();
	uint layerCount=m_Doc->layerCount();
	for (uint la2 = 0; la2 < layerCount; ++la2)
	{
		maxLayer = qMax(m_Doc->Layers[la2].ID, maxLayer);
		maxLevel = qMax(m_Doc->Layers[la2].Level, maxLevel);
	}

	parStyleMap.clear();
	itemRemap.clear();
	itemNext.clear();
	itemCount = 0;
	itemRemapM.clear();
	itemNextM.clear();
	itemCountM = 0;
	itemRemapF.clear();
	itemNextF.clear();

	FrameItems.clear();
	WeldItems.clear();
	WeldID.clear();
	LinkID.clear();

	markeredItemsMap.clear();
	markeredMarksMap.clear();
	nsetRangeItemNamesMap.clear();
	notesFramesData.clear();
	notesMasterMarks.clear();
	notesNSets.clear();

	QScopedPointer<QIODevice> ioDevice(slaReader(fileName));
	if (ioDevice.isNull())
	{
		setFileReadError();
		return false;
	}

	QString fileDir = QFileInfo(fileName).absolutePath();
	
	ReadObjectParams readObjectParams;
	readObjectParams.baseDir = fileDir;
	readObjectParams.itemKind = PageItem::StandardItem;
	readObjectParams.loadingPage = true;
	readObjectParams.renamedMasterPage = Mpage ? renamedPageName : QString();

	bool firstElement = true;
	bool success = true;
	isNewFormat = false;
	
	ScXmlStreamReader reader(ioDevice.data());
	ScXmlStreamAttributes attrs;
	while (!reader.atEnd() && !reader.hasError())
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
			firstElement = false;
		}

		if (tagName == "COLOR" && attrs.valueAsString("NAME") != CommonStrings::None)
			readColor(m_Doc->PageColors, attrs);
		if (tagName == "Gradient" && attrs.valueAsString("Name") != CommonStrings::None)
		{
			VGradient gra;
			QString grName = attrs.valueAsString("Name");
			success = readGradient(m_Doc, gra, reader);
			if (!success)
				break;
			gra.setRepeatMethod((VGradient::VGradientRepeatMethod)(attrs.valueAsInt("Ext", VGradient::pad)));
			if (!grName.isEmpty())
			{
				m_Doc->docGradients.insert(grName, gra);
			}
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
		if (tagName == "MultiLine")
		{
			multiLine ml;
			QString mlName  = attrs.valueAsString("Name");
			QString mlName2 = mlName;
			readMultiline(ml, reader);
			QHash<QString,multiLine>::ConstIterator mlit = m_Doc->docLineStyles.find(mlName2);
			if (mlit != m_Doc->docLineStyles.end() && ml != mlit.value())
					mlName2 = getUniqueName(mlName2, m_Doc->docLineStyles);
			m_Doc->docLineStyles.insert(mlName2, ml);
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
		if (tagName == "STYLE")
		{
			getStyle(vg, reader, nullptr, m_Doc, true);
		}
		if (((tagName == "PAGE") || (tagName == "MASTERPAGE")) && (attrs.valueAsInt("NUM") == pageNumber))
		{
			if (Mpage && (tagName != "MASTERPAGE"))
				continue;
			a = m_Doc->currentPage()->pageNr();
			newPage = m_Doc->Pages->at(a);
			SimpleState *ss = nullptr;
			if (UndoManager::undoEnabled())
			{
				ss = new SimpleState(Um::ChangePageAttrs, "", Um::ICreate);
				ss->set("PAGE_ATTRS");
				ss->set("LEFT_OLD", newPage->LeftPg);
				ss->set("NAME_OLD", newPage->pageName());
				ss->set("ORIENTATION_OLD", newPage->orientation());
				ss->set("SIZE_OLD", newPage->size());
				ss->set("WIDTH_OLD", newPage->width());
				ss->set("HEIGHT_OLD", newPage->height());
				ss->set("INIT_HEIGHT_OLD", newPage->initialHeight());
				ss->set("INIT_WIDTH_OLD", newPage->initialWidth());
				ss->set("INIT_MARGINTOP_OLD", newPage->initialMargins.top());
				ss->set("INIT_MARGINBOTTOM_OLD", newPage->initialMargins.bottom());
				ss->set("INIT_MARGINRIGHT_OLD", newPage->initialMargins.right());
				ss->set("INIT_MARGINLEFT_OLD", newPage->initialMargins.left());
				ss->set("MARGINTOP_OLD", newPage->Margins.top());
				ss->set("MARGINBOTTOM_OLD", newPage->Margins.bottom());
				ss->set("MARGINPRESET_OLD", newPage->marginPreset);
				ss->set("HORIZONTAL_AUTOGAP_OLD", newPage->guides.horizontalAutoGap());
				ss->set("VERTICAL_AUTOGAP_OLD", newPage->guides.verticalAutoGap());
				ss->set("HORIZONTAL_AUTOCOUNT_OLD", newPage->guides.horizontalAutoCount());
				ss->set("VERTICAL_AUTOCOUNT_OLD", newPage->guides.verticalAutoCount());
				ss->set("HORIZONTAL_AUTOREFER_OLD", newPage->guides.horizontalAutoRefer());
				ss->set("VERTICAL_AUTOREFER_OLD", newPage->guides.verticalAutoRefer());
			}
			if (Mpage)
			{
				newPage->LeftPg = attrs.valueAsInt("LEFT", 0);

				if (!renamedPageName.isEmpty())
					newPage->setPageName(renamedPageName);
				else
					newPage->setPageName(attrs.valueAsString("NAM",""));
			}
			if (attrs.hasAttribute("Size"))
				newPage->setSize(attrs.valueAsString("Size"));
			if (attrs.hasAttribute("Orientation"))
				newPage->setOrientation(attrs.valueAsInt("Orientation"));
			if (attrs.hasAttribute("PAGEWIDTH"))
				newPage->setWidth( attrs.valueAsDouble("PAGEWIDTH") );
			else
				newPage->setWidth( attrs.valueAsDouble("PAGEWITH") );
			newPage->setHeight( attrs.valueAsDouble("PAGEHEIGHT") );
			newPage->setInitialHeight(newPage->height());
			newPage->setInitialWidth(newPage->width());
			newPage->initialMargins.setTop(qMax(0.0, attrs.valueAsDouble("BORDERTOP")));
			newPage->initialMargins.setBottom(qMax(0.0, attrs.valueAsDouble("BORDERBOTTOM")));
			newPage->initialMargins.setLeft(qMax(0.0, attrs.valueAsDouble("BORDERLEFT")));
			newPage->initialMargins.setRight(qMax(0.0, attrs.valueAsDouble("BORDERRIGHT")));
			newPage->marginPreset = attrs.valueAsInt("PRESET", 0);
			newPage->Margins.setTop(newPage->initialMargins.top());
			newPage->Margins.setBottom(newPage->initialMargins.bottom());
			pageX = attrs.valueAsDouble("PAGEXPOS");
			pageY = attrs.valueAsDouble("PAGEYPOS");

			// guides reading
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
			if (UndoManager::undoEnabled())
			{
				ss->set("LEFT", newPage->LeftPg);
				ss->set("NAME", newPage->pageName());
				ss->set("ORIENTATION", newPage->orientation());
				ss->set("SIZE", newPage->size());
				ss->set("WIDTH", newPage->width());
				ss->set("HEIGHT", newPage->height());
				ss->set("INIT_HEIGHT", newPage->initialHeight());
				ss->set("INIT_WIDTH", newPage->initialWidth());
				ss->set("INIT_MARGINTOP", newPage->initialMargins.top());
				ss->set("INIT_MARGINBOTTOM", newPage->initialMargins.bottom());
				ss->set("INIT_MARGINRIGHT", newPage->initialMargins.right());
				ss->set("INIT_MARGINLEFT", newPage->initialMargins.left());
				ss->set("MARGINTOP", newPage->Margins.top());
				ss->set("MARGINBOTTOM", newPage->Margins.bottom());
				ss->set("MARGINPRESET", newPage->marginPreset);
				ss->set("HORIZONTAL_AUTOGAP", newPage->guides.horizontalAutoGap());
				ss->set("VERTICAL_AUTOGAP", newPage->guides.verticalAutoGap());
				ss->set("HORIZONTAL_AUTOCOUNT", newPage->guides.horizontalAutoCount());
				ss->set("VERTICAL_AUTOCOUNT", newPage->guides.verticalAutoCount());
				ss->set("HORIZONTAL_AUTOREFER", newPage->guides.horizontalAutoRefer());
				ss->set("VERTICAL_AUTOREFER", newPage->guides.verticalAutoRefer());
				undoManager->action(newPage, ss);
			}
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
				ItemInfo itemInfo;
				success = readObject(m_Doc, reader, readObjectParams, itemInfo);
				if (!success) break;

				PageItem* newItem = itemInfo.item;
				newItem->moveBy(-pageX + newPage->xOffset(), - pageY + newPage->yOffset());
				newItem->setOwnerPage(m_Doc->currentPageNumber());
				if (tagName == "PAGEOBJECT")
					newItem->setMasterPageName(QString());
				else if (Mpage && !renamedPageName.isEmpty())
					newItem->setMasterPageName(renamedPageName);
				newItem->setLayer(layerTrans.value(newItem->m_layerID, newItem->m_layerID));
				if (isNewFormat)
				{
					if (itemInfo.nextItem != -1)
						itemNext[itemInfo.itemID] = itemInfo.nextItem;
					if (itemInfo.item->isTableItem)
						TableItems.append(itemInfo.item);
					if (itemInfo.isWeldFlag)
						WeldItems.append(itemInfo.item);
				}
				else
				{
					// first of linked chain?
					if (tagName == "PAGEOBJECT")
					{
						itemRemap[itemCount++] = m_Doc->DocItems.count();
						if (attrs.valueAsInt("NEXTITEM", -1) != -1)
							itemNext[m_Doc->DocItems.count()] = attrs.valueAsInt("NEXTITEM");
					}
					else if (tagName == "MASTEROBJECT")
					{
						itemRemapM[itemCountM++] = m_Doc->MasterItems.count();
						if (attrs.valueAsInt("NEXTITEM", -1) != -1)
							itemNextM[m_Doc->MasterItems.count()] = attrs.valueAsInt("NEXTITEM");
					}
					if (newItem->isTableItem)
					{
						TableItems.append(newItem);
						TableID.insert(itemInfo.ownLink, newItem);
					}
					if (itemInfo.isWeldFlag)
					{
						WeldItems.append(itemInfo.item);
						WeldID.insert(itemInfo.ownWeld, itemInfo.item);
					}
				}

				if ((tagName == "PAGEOBJECT") && (groupStackPI.count() > 0))
				{
					groupStackPI.top().append(itemInfo.item);
					while (itemInfo.ownNr == groupStackPI2.top())
					{
						groupStackP.push(groupStackPI.pop());
						groupStackPI2.pop();
						if (groupStackPI2.count() == 0)
							break;
					}
				}
				else if ((tagName == "FRAMEOBJECT") && (groupStackFI.count() > 0))
				{
					groupStackFI.top().append(itemInfo.item);
					while (itemInfo.ownNr == groupStackFI2.top())
					{
						groupStackF.push(groupStackFI.pop());
						groupStackFI2.pop();
						if (groupStackFI2.count() == 0)
							break;
					}
				}
				else if ((tagName == "MASTEROBJECT") && (groupStackMI.count() > 0))
				{
					groupStackMI.top().append(itemInfo.item);
					while (itemInfo.ownNr == groupStackMI2.top())
					{
						groupStackM.push(groupStackMI.pop());
						groupStackMI2.pop();
						if (groupStackMI2.count() == 0)
							break;
					}
				}

				if (itemInfo.isGroupFlag)
				{
					QList<PageItem*> groupItems;
					groupItems.append(itemInfo.item);
					if (tagName == "PAGEOBJECT")
					{
						groupStackPI.push(groupItems);
						groupStackPI2.push(itemInfo.groupLastItem + itemInfo.ownNr);
					}
					else if (tagName == "FRAMEOBJECT")
					{
						groupStackFI.push(groupItems);
						groupStackFI2.push(itemInfo.groupLastItem + itemInfo.ownNr);
					}
					else
					{
						groupStackMI.push(groupItems);
						groupStackMI2.push(itemInfo.groupLastItem + itemInfo.ownNr);
					}
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
		PageItem* item = LinkID.value(elem, (PageItem*) nullptr);
		if (!item)
			continue;
		ScribusDoc::BookMa bookmark = it.value();
		bookmark.PageObject = item;
		m_Doc->BookMarks.append( bookmark );
	}

	if (isNewFormat)
	{
		if (TableItems.count() != 0)
		{
			for (int ttc = 0; ttc < TableItems.count(); ++ttc)
			{
				PageItem* ta = TableItems.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = LinkID[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = LinkID[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = LinkID[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = LinkID[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (WeldItems.count() != 0)
		{
			for (int ttc = 0; ttc < WeldItems.count(); ++ttc)
			{
				PageItem* ta = WeldItems.at(ttc);
				for (int i = 0 ; i < ta->weldList.count(); ++i)
				{
					PageItem::WeldingInfo wInf = ta->weldList.at(i);
					ta->weldList[i].weldItem = LinkID.value(wInf.weldID, 0);
					if (ta->weldList[i].weldItem == nullptr)
						ta->weldList.removeAt(i--);
				}
			}
		}
		if (itemNext.count() != 0)
		{
			QMap<int,int>::Iterator lc;
			for (lc = itemNext.begin(); lc != itemNext.end(); ++lc)
			{
				if (lc.value() >= 0)
				{
					PageItem * Its = LinkID[lc.key()];
					PageItem * Itn = LinkID[lc.value()];
					if (!Its->canBeLinkedTo(Itn))
					{
						qDebug() << "scribus150format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
				}
			}
		}
	}
	else
	{
		if (TableItems.count() != 0)
		{
			for (int ttc = 0; ttc < TableItems.count(); ++ttc)
			{
				PageItem* ta = TableItems.at(ttc);
				if (ta->TopLinkID != -1)
					ta->m_topLink = TableID[ta->TopLinkID];
				else
					ta->m_topLink = nullptr;
				if (ta->LeftLinkID != -1)
					ta->m_leftLink = TableID[ta->LeftLinkID];
				else
					ta->m_leftLink = nullptr;
				if (ta->RightLinkID != -1)
					ta->m_rightLink = TableID[ta->RightLinkID];
				else
					ta->m_rightLink = nullptr;
				if (ta->BottomLinkID != -1)
					ta->m_bottomLink = TableID[ta->BottomLinkID];
				else
					ta->m_bottomLink = nullptr;
			}
		}
		if (WeldItems.count() != 0)
		{
			for (int ttc = 0; ttc < WeldItems.count(); ++ttc)
			{
				PageItem* ta = WeldItems.at(ttc);
				for (int i = 0 ; i < ta->weldList.count(); ++i)
				{
					PageItem::WeldingInfo wInf = ta->weldList.at(i);
					ta->weldList[i].weldItem = WeldID.value(wInf.weldID, 0);
					if (ta->weldList[i].weldItem == nullptr)
						ta->weldList.removeAt(i--);
				}
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
					PageItem *Its(nullptr), *Itn(nullptr);
					if (lc.key() < m_Doc->Items->count())
						Its = m_Doc->DocItems.at(lc.key());
					if (itemRemap[lc.value()] < m_Doc->DocItems.count())
						Itn = m_Doc->DocItems.at(itemRemap[lc.value()]);
					if (!Its || !Itn || !Its->canBeLinkedTo(Itn))
					{
						qDebug() << "scribus150format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
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
					PageItem *Its(nullptr), *Itn(nullptr);
					if (lc.key() < m_Doc->MasterItems.count())
						Its = m_Doc->MasterItems.at(lc.key());
					if (itemRemapM[lc.value()] < m_Doc->MasterItems.count())
						Itn = m_Doc->MasterItems.at(itemRemapM[lc.value()]);
					if (!Its || !Itn || !Its->canBeLinkedTo(Itn))
					{
						qDebug() << "scribus150format: corruption in linked textframes detected";
						continue;
					}
					Its->link(Itn);
				}
			}
		}
	}

	while (groupStackP.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackP.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->DocItems.removeOne(cItem);
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackP, &m_Doc->DocItems);
		if (!converted)
			gItem->groupItemList = gpL;
	}

	while (groupStackF.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackF.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->FrameItems.remove(m_Doc->FrameItems.key(cItem));
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackF, nullptr);
		if (!converted)
			gItem->groupItemList = gpL;
	}

	while (groupStackM.count() > 0)
	{
		bool isTableIt = false;
		QList<PageItem*> gpL = groupStackM.pop();
		PageItem* gItem = gpL.takeFirst();
		for (int id = 0; id < gpL.count(); id++)
		{
			PageItem* cItem = gpL.at(id);
			isTableIt = cItem->isTableItem;
			cItem->gXpos = cItem->xPos() - gItem->xPos();
			cItem->gYpos = cItem->yPos() - gItem->yPos();
			cItem->Parent = gItem;
			if (gItem->rotation() != 0)
			{
				QTransform ma;
				ma.rotate(-gItem->rotation());
				FPoint n = FPoint(cItem->gXpos, cItem->gYpos);
				cItem->gXpos = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
				cItem->gYpos = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
				cItem->setRotation(cItem->rotation() - gItem->rotation());
				cItem->oldRot = cItem->rotation();
			}
			m_Doc->MasterItems.removeOne(cItem);
		}
		bool converted = false;
		if (isTableIt)
			converted = convertOldTable(m_Doc, gItem, gpL, &groupStackM, &m_Doc->MasterItems);
		if (!converted)
			gItem->groupItemList = gpL;
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

void Scribus150Format::getStyle(ParagraphStyle& style, ScXmlStreamReader& reader, StyleSet<ParagraphStyle> *tempStyles, ScribusDoc* doc, bool fl)
{
	bool  found(false);
	const StyleSet<ParagraphStyle> &docParagraphStyles = tempStyles ? *tempStyles : doc->paragraphStyles();
	readParagraphStyle(doc, reader, style);
	for (int xx = 0; xx < docParagraphStyles.count(); ++xx)
	{
		const ParagraphStyle& paraStyle = docParagraphStyles[xx];
		if (style.name() == paraStyle.name())
		{
			if (style.equiv(paraStyle))
			{
				found = true;
			}
			else
			{
				style.setName(docParagraphStyles.getUniqueCopyName(paraStyle.name()));
				found = false;
			}
			break;
		}
	}
	if (!found && fl)
	{
		for (int xx = 0; xx < docParagraphStyles.count(); ++xx)
		{
			const ParagraphStyle& paraStyle = docParagraphStyles[xx];
			if (style.equiv(paraStyle))
			{
				parStyleMap[style.name()] = paraStyle.name();
				style.setName(paraStyle.name());
				found = true;
				break;
			}
		}
	}
	if (!found)
	{
		if (tempStyles)
			tempStyles->create(style);
		else
		{
			StyleSet<ParagraphStyle> tmp;
			tmp.create(style);
			doc->redefineStyles(tmp, false);
		}
	}
}

bool Scribus150Format::readStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles)
{
	ParagraphStyle pstyle;
	bool firstElement = true;
	bool success = true;

	QScopedPointer<QIODevice> ioDevice(slaReader(fileName));
	if (ioDevice.isNull())
		return false;

	ScXmlStreamReader reader(ioDevice.data());
	ScXmlStreamAttributes attrs;
	while (!reader.atEnd() && !reader.hasError())
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

bool Scribus150Format::readCharStyles(const QString& fileName, ScribusDoc* doc, StyleSet<CharStyle> &docCharStyles)
{
	CharStyle cstyle;
	bool firstElement = true;
	//bool success = true;

	QScopedPointer<QIODevice> ioDevice(slaReader(fileName));
	if (ioDevice.isNull())
		return false;

	ScXmlStreamReader reader(ioDevice.data());
	ScXmlStreamAttributes attrs;
	while (!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (tType != QXmlStreamReader::StartElement)
			continue;
		QStringRef tagName = reader.name();
		if (firstElement)
		{
			if (tagName != "SCRIBUSUTF8NEW")
			{
			//	success = false;
				break;
			}
			firstElement = false;
			continue;
		}
		if (tagName == "CHARSTYLE")
		{
			cstyle.erase();
			attrs = reader.scAttributes();
			readNamedCharacterStyleAttrs(doc, attrs, cstyle);
			docCharStyles.create(cstyle);
		}
	}
	return true;
}

bool Scribus150Format::readLineStyles(const QString& fileName, QHash<QString,multiLine> *styles)
{
	bool firstElement = true;
	bool success = true;

	QScopedPointer<QIODevice> ioDevice(slaReader(fileName));
	if (ioDevice.isNull())
		return false;

	ScXmlStreamReader reader(ioDevice.data());
	ScXmlStreamAttributes attrs;
	while (!reader.atEnd() && !reader.hasError())
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
			QHash<QString,multiLine>::ConstIterator mlit = styles->find(mlName2);
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
	return success;
}

bool Scribus150Format::readColors(const QString& fileName, ColorList & colors)
{
	bool firstElement = true;
	bool success = true;

	QScopedPointer<QIODevice> ioDevice(slaReader(fileName));
	if (ioDevice.isNull())
		return false;

	ScXmlStreamReader reader(ioDevice.data());
	ScXmlStreamAttributes attrs;
	while (!reader.atEnd() && !reader.hasError())
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
		if (tagName == "COLOR" && attrs.valueAsString("NAME") != CommonStrings::None)
		{
			attrs = reader.scAttributes();
			if (attrs.valueAsString("NAME") != CommonStrings::None)
				readColor(colors, attrs);
		}
	}
	return success;
}

bool Scribus150Format::readPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames)
{
	QString pageName;
	int counter = 0;
	int counter2 = 0;
	bool firstElement = true;
	bool success = true;

	markeredItemsMap.clear();
	markeredMarksMap.clear();
	nsetRangeItemNamesMap.clear();
	notesFramesData.clear();
	notesMasterMarks.clear();
	notesNSets.clear();

	QScopedPointer<QIODevice> ioDevice(slaReader(fileName));
	if (ioDevice.isNull())
		return false;

	ScXmlStreamReader reader(ioDevice.data());
	ScXmlStreamAttributes attrs;
	while (!reader.atEnd() && !reader.hasError())
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
		if (tagName == "PAGE")
			counter++;
		if (tagName == "MASTERPAGE")
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

void Scribus150Format::updateNames2Ptr() //after document load - items pointers should be updated
{
	if (!markeredItemsMap.isEmpty())
	{
		QMap<Mark*, int>::Iterator markIt;
		QMap<Mark*, int>::Iterator end = markeredItemsMap.end();
		for (markIt = markeredItemsMap.begin(); markIt != end; ++markIt)
		{
			Mark* mrk = markIt.key();
			int ItemID = markIt.value();
			if (LinkID.contains(ItemID))
			{
				mrk->setItemPtr(LinkID[ItemID]);
				mrk->setString(m_Doc->getSectionPageNumberForPageIndex(mrk->getItemPtr()->OwnPage));
			}
			else
			{
				qWarning() << "Scribus150Format::updateNames2Ptr() : wrong mark [" << mrk->label << "] data - item [" << ItemID << "] not exists - DELETING MARK";
				QString markLabel(mrk->label);
				if (!m_Doc->eraseMark(mrk, true))
					qWarning() << "Erase mark [" << markLabel << "] failed - was it defined?";
			}
		}
		markeredItemsMap.clear();
	}
	if (!markeredMarksMap.isEmpty())
	{
		auto end = markeredMarksMap.end();
		for (auto markIt = markeredMarksMap.begin(); markIt != end; ++markIt)
		{
			Mark* mark = markIt.key();
			QMap<QString, MarkType> mark2map = markIt.value();
			QString label2 = mark2map.begin().key();
			MarkType type2 = mark2map.begin().value();
			Mark* mark2 = m_Doc->getMark(label2, type2);
			if (mark2 != nullptr)
			{
				mark->setMark(mark2);
			}
			else
			{
				qWarning() << "Scribus150Format::updateNames2Ptr() : wrong mark [" << mark->label << "] data - pointed mark name [" << label2 << "] not exists - DELETING MARK";
				QString markLabel(mark->label);
				if (!m_Doc->eraseMark(mark, true))
					qWarning() << "Erase mark [" << markLabel << "] failed - was it defined?";

			}
		}
		markeredMarksMap.clear();
	}

	//update endnotes frames pointers
	if (!notesFramesData.isEmpty())
	{
		for (int i = 0; i < notesFramesData.count(); ++i)
		{
			NoteFrameData eF = notesFramesData.at(i);
			NotesStyle* ns = m_Doc->getNotesStyle(eF.NSname);
			if (ns == nullptr)
				continue;

			PageItem* item = LinkID.value(eF.myID);
			if (item == nullptr || !item->isNoteFrame())
			{
				qDebug() << "Scribus150Format::updateNames2Ptr() : update end frames pointers - item is not note frame or name is wrong";
				continue;
			}

			PageItem_NoteFrame* noteFrame = item->asNoteFrame();
			noteFrame->setNoteStyle(ns);
			if (ns->isEndNotes())
			{
				if (eF.NSrange == NSRdocument)
					m_Doc->setEndNoteFrame(noteFrame, (void*) nullptr);
				else if (eF.NSrange == NSRstory)
					m_Doc->setEndNoteFrame(noteFrame, (void*) LinkID.value(eF.itemID));
			}
			else
			{
				PageItem* master = LinkID.value(eF.itemID);
				if (master == nullptr)
					continue;
				noteFrame->setMaster(master);
				master->asTextFrame()->setNoteFrame(noteFrame);
				//FIX welding with note frame
				PageItem::WeldingInfo wInf;
				for (int i = 0 ; i < master->weldList.count(); i++)
				{
					wInf = master->weldList.at(i);
					if (wInf.weldID == eF.myID)
					{
						master->weldList[i].weldItem = item;
						break;
					}
				}
			}
		}
	}
	//update pointers to notes in master notes marks
	if (!notesMasterMarks.isEmpty())
	{
		assert(!m_Doc->marksList().isEmpty() && !m_Doc->notesList().isEmpty());
		QMap<QString, TextNote*>::Iterator it;
		QMap<QString, TextNote*>::Iterator end = notesMasterMarks.end();
		for (it = notesMasterMarks.begin(); it != end; ++it)
		{
			TextNote* note = it.value();
			assert(note != nullptr);
			const QString& mrkLabel = it.key();
			Mark* mrk = m_Doc->getMark(mrkLabel, MARKNoteMasterType);
			if (mrk == nullptr)
			{
				qWarning() << "Scribus150Format::updateNames2Ptr() : cannot find master mark ("<<mrkLabel <<") for note - note will be deleted";
				m_Doc->deleteNote(note);
				continue;
			}
			note->setMasterMark(mrk);
			mrk->setNotePtr(note);
		}
		notesMasterMarks.clear();
	}
	if (!notesNSets.isEmpty())
	{
		assert(!m_Doc->notesList().isEmpty());
		auto end = notesNSets.end();
		for (auto it = notesNSets.begin(); it != end; ++it)
		{
			TextNote* note = it.key();
			assert(note != nullptr);
			QString nsLabel = it.value();
			NotesStyle* ns = m_Doc->getNotesStyle(nsLabel);
			if (ns != nullptr)
				note->setNotesStyle(ns);
			if (note->notesStyle() == nullptr)
			{
				qWarning() << "Scribus150Format::updateNames2Ptr()  : cannot find notes style ("<<nsLabel <<") for note - note will be deleted";
				m_Doc->deleteNote(note);
				continue;
			}
		}
		notesNSets.clear();
	}
}

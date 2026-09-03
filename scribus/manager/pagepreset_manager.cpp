/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
For general Scribus copyright and licensing information please refer
to the COPYING file provided with the program.
*/
#include "pagepreset_manager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <QUuid>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "api/api_application.h"
#include "commonstrings.h"
#include "langmgr.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scribusapp.h"
#include "units.h"
#include "util.h"

PagePresetManager *PagePresetManager::m_instance = nullptr;

PagePresetManager &PagePresetManager::instance()
{
	static PagePresetManager m_instance;
	return m_instance;
}

PagePresetManager::PagePresetManager(QObject *parent)
	: QObject(parent)
{
	reloadAllPresets();
	connect(ScQApp, &ScribusQApp::localeChanged, this, &PagePresetManager::localeChange);
}


void PagePresetManager::reloadAllPresets()
{
	m_pageSizeList.clear();
	m_categoryList.clear();
	m_categoryOrderList.clear();

	// Add Scribus page presets
	loadAllPresets(systemIndexFile(), PageSizeType::Preset);

	// Add users page presets
	loadAllPresets(userIndexFile(), PageSizeType::User);
}

QStringList PagePresetManager::defaultSizesList()
{
	// A3, A4, A5, A6, US letter
	static const QStringList list = {"FzlMfpPelK5894hHnoLDC_00007", "FzlMfpPelK5894hHnoLDC_00009", "FzlMfpPelK5894hHnoLDC_00010", "FzlMfpPelK5894hHnoLDC_00011", "6w0FSR1dcD7Lh6qfbD3QvD_00019"};
	return list;
}

PageCollectionInfo PagePresetManager::categoryInfoById(const QString &id)
{
	return m_categoryList.value(id);
}

PageCollectionInfo PagePresetManager::categoryInfoCustom()
{
	PageCollectionInfo pciCustom;
	pciCustom.id = CommonStrings::customPageSize;
	pciCustom.displayName = CommonStrings::trCustomPageSize;
	return pciCustom;
}

PageCollectionInfo PagePresetManager::categoryInfoPreferred()
{
	PageCollectionInfo pciPreferred;
	pciPreferred.id = QStringLiteral("Preferred");
	pciPreferred.displayName = tr("Preferred");
	return pciPreferred;
}

PageSizeInfoMap PagePresetManager::activePageSizes() const
{
	const auto &activeList = PrefsManager::instance().appPrefs.activePageSizes;

	if (activeList.isEmpty())
		return m_pageSizeList;

	PageSizeInfoMap map;

	for (const QString &id : activeList)
	{
		auto it = m_pageSizeList.find(id);
		if (it != m_pageSizeList.end())
			map.insert(id, *it);

		// This is an alternative way to find a page size, but needs more time
		// PageSizeInfo psi = pageInfoByName(id);
		// map.insert(psi.id, psi);
	}
	return map;
}

PageSizeInfoMap PagePresetManager::sizesByCategory(QString categoryId) const
{
	PageSizeInfoMap map;
	for (auto it = m_pageSizeList.constBegin(); it != m_pageSizeList.constEnd(); ++it)
		if (it.value().categoryId == categoryId)
			map.insert(it.key(), it.value());
	return map;
}

PageSizeInfoMap PagePresetManager::sizesByDimensions(QSizeF sizePt) const
{
	PageSizeInfoMap map;
	for (auto it = m_pageSizeList.constBegin(); it != m_pageSizeList.constEnd(); ++it)
		if (isSizeMatch(it.value(), sizePt))
			map.insert(it.key(), it.value());
	return map;
}

PageSizeInfo PagePresetManager::pageInfoByDimensions(QSizeF sizePt) const
{
	for (auto it = m_pageSizeList.constBegin(); it != m_pageSizeList.constEnd(); ++it)
		if (isSizeMatch(it.value(), sizePt))
			return it.value();

	PageSizeInfo psi = pageInfoCustom();
	psi.width = sizePt.width();
	psi.height = sizePt.height();
	return psi;
}

PageSizeInfo PagePresetManager::pageInfoByName(const QString &name) const
{
	// Check if ID matches
	auto it = m_pageSizeList.find(name);
	if (it != m_pageSizeList.end())
		return it.value();

	// Check if name and legacy name matches (not localized)
	for (auto it2 = m_pageSizeList.constBegin(); it2 != m_pageSizeList.constEnd(); ++it2)
		if (it2.value().displayName == name || it2.value().lagacyNames.contains(name))
			return it2.value();

	// Fallback: custom preset
	return pageInfoCustom();
}

PageSizeInfo PagePresetManager::pageInfoCustom() const
{
	PageCollectionInfo pci = PagePresetManager::instance().categoryInfoCustom();
	PageSizeInfo psi;
	psi.width = 0.0;
	psi.height = 0.0;
	psi.pageUnitIndex = 0; //pt
	psi.id = CommonStrings::customPageSize;
	psi.name = CommonStrings::trCustomPageSize;
	psi.displayName = CommonStrings::trCustomPageSize;
	psi.categoryId = pci.id;

	return psi;
}

bool PagePresetManager::isSizeMatch(const PageSizeInfo &info, const QSizeF &size)
{
	const double epsilon = 0.01;

	return (qAbs(info.width - size.width()) <= epsilon && qAbs(info.height - size.height()) <= epsilon)
		   || (qAbs(info.width - size.height()) <= epsilon && qAbs(info.height - size.width()) <= epsilon);
}

void PagePresetManager::addPageSize(const PageSizeInfo &info)
{
	PageSizeInfo pinfo = info;
	pinfo.width = value2pts(info.width, info.pageUnitIndex);
	pinfo.height = value2pts(info.height, info.pageUnitIndex);
	pinfo.label = QStringLiteral("%1 x %2 %3").arg(info.width).arg(info.height).arg(unitGetStrFromIndex(info.pageUnitIndex));

	m_pageSizeList.insert(pinfo.id, std::move(pinfo));
}

void PagePresetManager::addCategory(const PageCollectionInfo &collection)
{
	m_categoryList.insert(collection.id, collection);
}

void PagePresetManager::loadAllPresets(const QString &indexFilePath, PageSizeType type)
{
	QFile file(indexFilePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		if (type == PageSizeType::Preset)
			qWarning() << "Could not open index file:" << indexFilePath;
		return;
	}

	QString basePath = QFileInfo(indexFilePath).path();
	if (!basePath.endsWith(u'/'))
		basePath += u'/';

	QXmlStreamReader xml(&file);

	while (!xml.atEnd() && !xml.hasError())
	{
		if (xml.readNext() == QXmlStreamReader::StartElement)
		{
			const auto tagName = xml.name();

			if (tagName == u"collections")
			{
				const auto minVer = xml.attributes().value("minVersion");
				if (!hasValidVersion(minVer.toString()))
					qWarning() << "The index file is newer than the current version:" << indexFilePath;
			}
			else if (tagName == u"collection")
			{
				auto fileNameView = xml.attributes().value("name").toString();
				if (!fileNameView.isEmpty())
					parseCollectionFile(basePath + fileNameView, type);
			}
			else if (tagName == u"separator")
			{
				m_categoryOrderList.append(QStringLiteral("-"));
			}
		}
	}

	if (xml.hasError())
		qWarning() << "XML error in index:" << xml.errorString();
}

void PagePresetManager::parseCollectionFile(const QString &filePath, PageSizeType type)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		qWarning() << "Could not open collection file:" << filePath;
		return;
	}

	QXmlStreamReader xml(&file);
	PageCollectionInfo currentMeta;
	currentMeta.type = type;
	currentMeta.filePath = filePath;

	bool collectingPages = false;

	while (!xml.atEnd() && !xml.hasError())
	{
		const auto token = xml.readNext();

		if (token == QXmlStreamReader::StartElement)
		{
			const auto tagName = xml.name();

			if (tagName == u"collection")
			{
				const auto &attrs = xml.attributes();
				if (!hasValidVersion(attrs.value("minVersion").toString()))
				{
					qWarning() << "Skipped newer version preset:" << filePath;
					return;
				}

				currentMeta.id = attrs.value("id").toString();
				currentMeta.name = attrs.value("name").toString();
				currentMeta.displayName = currentMeta.name;

				m_categoryOrderList.append(currentMeta.id);
			}
			else if (tagName == u"metadata")
			{
				while (!xml.atEnd())
				{
					const auto metaToken = xml.readNext();
					if (metaToken == QXmlStreamReader::EndElement && xml.name() == u"metadata")
						break;

					if (metaToken == QXmlStreamReader::StartElement)
					{
						const auto metaTag = xml.name();
						if (metaTag == u"author")
							currentMeta.author = xml.readElementText();
						else if (metaTag == u"license")
							currentMeta.license = xml.readElementText();
						else if (metaTag == u"names")
						{
							currentMeta.localizedNames = parseNamesBlock(xml);
							updateDisplayName(currentMeta);
						}
						else
							xml.skipCurrentElement();
					}
				}
				addCategory(currentMeta);
			}
			else if (tagName == u"pages")
			{
				collectingPages = true;
			}
			else if (collectingPages && tagName == u"page")
			{
				PageSizeInfo info;
				const auto &attrs = xml.attributes();

				info.id = currentMeta.id + u'_' + attrs.value("id").toString();
				info.width = attrs.value("width").toDouble();
				info.height = attrs.value("height").toDouble();
				info.categoryId = currentMeta.id;
				info.name = attrs.value("name").toString();
				info.displayName = info.name;
				info.pageUnitIndex = unitIndexFromString(attrs.value("unit").toString());
				info.type = type;

				const auto layoutVal = attrs.value("layout");
				if (!layoutVal.isEmpty())
					info.layout = attrs.value("layout").toInt();
				// else
				// 	info.layout = prefsManager.appPrefs.docSetupPrefs.pagePositioning;

				const auto bindingDirectionVal = attrs.value("bindingDirection");
				if (!bindingDirectionVal.isEmpty())
					info.bindingDirection = attrs.value("bindingDirection").toInt();
				// else
				// 	info.bindingDirection = prefsManager.appPrefs.docSetupPrefs.bindingDirection;

				const auto firstPageVal = attrs.value("firstPage");
				if (!firstPageVal.isEmpty())
					info.firstPage = attrs.value("firstPage").toInt();
				// else
				//  info.firstPage = prefsManager.appPrefs.docSetupPrefs.pagePositioning;

				const auto legacyVal = attrs.value("legacyNames");
				if (!legacyVal.isEmpty())
					info.lagacyNames = legacyVal.toString().split(u';');

				while (!xml.atEnd())
				{
					const auto pageToken = xml.readNext();
					if (pageToken == QXmlStreamReader::EndElement && xml.name() == u"page")
						break;

					if (pageToken == QXmlStreamReader::StartElement)
					{
						const auto childName = xml.name();

						if (childName == u"names")
						{
							info.localizedNames = parseNamesBlock(xml);
							updateDisplayName(info);
						}
						else if (childName == u"margins")
						{
							const auto &attrs = xml.attributes();
							MarginStruct margins;
							margins.setLeft(value2pts(attrs.value("left").toDouble(), info.pageUnitIndex));
							margins.setTop(value2pts(attrs.value("top").toDouble(), info.pageUnitIndex));
							margins.setRight(value2pts(attrs.value("right").toDouble(), info.pageUnitIndex));
							margins.setBottom(value2pts(attrs.value("bottom").toDouble(), info.pageUnitIndex));

							info.margins = margins;
							info.marginPreset = attrs.value("preset").toInt();

							xml.skipCurrentElement();
						}
						else if (childName == u"bleeds")
						{
							const auto &attrs = xml.attributes();
							MarginStruct margins;
							margins.setLeft(value2pts(attrs.value("left").toDouble(), info.pageUnitIndex));
							margins.setTop(value2pts(attrs.value("top").toDouble(), info.pageUnitIndex));
							margins.setRight(value2pts(attrs.value("right").toDouble(), info.pageUnitIndex));
							margins.setBottom(value2pts(attrs.value("bottom").toDouble(), info.pageUnitIndex));

							info.bleeds = margins;

							xml.skipCurrentElement();
						}
						else if (childName == u"textFrame")
						{
							const auto &attrs = xml.attributes();
							QList<double> values;
							values.append(attrs.value("columns").toInt());
							values.append(value2pts(attrs.value("gap").toDouble(), info.pageUnitIndex));

							info.textFrame = values;

							xml.skipCurrentElement();
						}
						else
							xml.skipCurrentElement();
					}
				}
				addPageSize(std::move(info));
			}
		}
		else if (token == QXmlStreamReader::EndElement)
		{
			if (xml.name() == u"pages")
				collectingPages = false;
		}
	}

	if (xml.hasError())
		qWarning() << "XML error in collection" << filePath << ":" << xml.errorString();
}

template<typename T>
void PagePresetManager::updateDisplayName(T &info)
{
	if (!info.localizedNames.empty())
	{
		const QString language = PrefsManager::instance().appPrefs.uiPrefs.language;
		const QString normalizeLanguage = LanguageManager::instance()->getShortAbbrevFromAbbrevDecomposition(language);
		info.displayName = info.localizedNames.value(normalizeLanguage, info.name);
	}
}

void PagePresetManager::localeChange()
{
	for (auto &col : m_categoryList)
		updateDisplayName(col);

	for (auto &page : m_pageSizeList)
		updateDisplayName(page);
}

LocalizedStringsMap PagePresetManager::parseNamesBlock(QXmlStreamReader &xml)
{
	LocalizedStringsMap namesMap;

	while (!xml.atEnd())
	{
		const auto token = xml.readNext();
		if (token == QXmlStreamReader::EndElement && xml.name() == u"names")
			break;

		if (token == QXmlStreamReader::StartElement)
		{
			if (xml.name() == u"name")
			{
				const auto langView = xml.attributes().value("lang").toString();
				if (!langView.isEmpty())
				{
					QString val = xml.readElementText();
					if (!val.isEmpty())
						namesMap.insert(langView, std::move(val));
				}
				else
					xml.skipCurrentElement();
			}
			else
				xml.skipCurrentElement();
		}
	}
	return namesMap;
}

const QString PagePresetManager::systemIndexFile()
{
	return QDir::toNativeSeparators(ScPaths::instance().pagePresetsDir()) + QStringLiteral("index.xml");
}

const QString PagePresetManager::userIndexFile()
{
	return QDir::toNativeSeparators(ScPaths::userPagePresetsDir(true)) + QStringLiteral("index.xml");
}

bool PagePresetManager::hasValidVersion(const QString &version) const
{
	const int curr_major = version.section(".", 0, 0).toInt();
	const int curr_minor = version.section(".", 1, 1).toInt();
	const int curr_patch = version.section(".", 2, 2).toInt();
	const int curr_fullver = curr_major * 10000 + curr_minor * 100 + curr_patch;

	const int ver_major = ScribusAPI::getVersionMajor();
	const int ver_minor = ScribusAPI::getVersionMinor();
	const int ver_patch = ScribusAPI::getVersionPatch();
	const int ver_fullver = ver_major * 10000 + ver_minor * 100 + ver_patch;

	return curr_fullver <= ver_fullver;
}

// =======================
// Custom preset functions
// =======================

bool PagePresetManager::updateIndexFile(const QString &collectionFilePath, IndexAction action)
{
	QFileInfo colInfo(collectionFilePath);
	const QString fileName = colInfo.fileName();
	const QString indexFilePath = colInfo.dir().filePath(QStringLiteral("index.xml"));

	QDomDocument doc;

	if (!loadDocument(indexFilePath, doc))
	{
		if (action == IndexAction::Remove)
			return true;

		doc.clear();
		doc.appendChild(doc.createProcessingInstruction(QStringLiteral("xml"), QStringLiteral("version=\"1.0\" encoding=\"utf-8\"")));
		QDomElement root = doc.createElement(QStringLiteral("collections"));
		root.setAttribute(QStringLiteral("minVersion"), ScribusAPI::getVersion());
		root.setAttribute(QStringLiteral("version"), ScribusAPI::getVersion());
		doc.appendChild(root);
	}

	QDomElement root = doc.documentElement();
	if (root.tagName() != u"collections")
		return false;

	// Find collection
	QDomElement foundElem;
	bool found = false;
	QDomElement elem = root.firstChildElement(QStringLiteral("collection"));

	while (!elem.isNull())
	{
		if (elem.attribute(QStringLiteral("name")) == fileName)
		{
			foundElem = elem;
			found = true;
			break;
		}
		elem = elem.nextSiblingElement(QStringLiteral("collection"));
	}

	bool changed = false;

	if (action == IndexAction::Add && !found)
	{
		QDomElement newCol = doc.createElement(QStringLiteral("collection"));
		newCol.setAttribute(QStringLiteral("name"), fileName);
		root.appendChild(newCol);
		changed = true;
	}
	else if (action == IndexAction::Remove && found)
	{
		root.removeChild(foundElem);
		changed = true;
	}

	return changed ? saveDocument(indexFilePath, doc) : true;
}

bool PagePresetManager::createOrUpdateCollection(const QString &filePath, const PageCollectionInfo &meta, QString &outUuid)
{
	QDomDocument doc;
	bool exists = loadDocument(filePath, doc);
	QDomElement root;

	if (!exists)
	{
		doc.appendChild(doc.createProcessingInstruction(QStringLiteral("xml"), QStringLiteral("version=\"1.0\" encoding=\"UTF-8\"")));
		root = doc.createElement(QStringLiteral("collection"));
		doc.appendChild(root);

		QString newId = getShortUuidFromUuid(QUuid::createUuid());
		root.setAttribute(QStringLiteral("id"), newId);
		root.appendChild(doc.createElement(QStringLiteral("pages")));
		outUuid = newId;
	}
	else
	{
		root = doc.documentElement();
		if (root.tagName() != u"collection")
			return false;

		outUuid = root.attribute(QStringLiteral("id"));
		if (outUuid.isEmpty())
		{
			outUuid = getShortUuidFromUuid(QUuid::createUuid());
			root.setAttribute(QStringLiteral("id"), outUuid);
		}
	}

	root.setAttribute(QStringLiteral("name"), meta.name);
	root.setAttribute(QStringLiteral("minVersion"), ScribusAPI::getVersion());
	root.setAttribute(QStringLiteral("version"), ScribusAPI::getVersion());

	QDomElement metaNode = root.firstChildElement(QStringLiteral("metadata"));
	if (metaNode.isNull())
	{
		metaNode = doc.createElement(QStringLiteral("metadata"));
		QDomElement pagesNode = root.firstChildElement(QStringLiteral("pages"));
		if (!pagesNode.isNull())
			root.insertBefore(metaNode, pagesNode);
		else
			root.appendChild(metaNode);
	}

	setChildText(metaNode, QStringLiteral("author"), meta.author);
	setChildText(metaNode, QStringLiteral("license"), meta.license);

	if (!saveDocument(filePath, doc))
		return false;

	if (!updateIndexFile(filePath, IndexAction::Add))
		qWarning() << "Index update failed:" << filePath;

	return true;
}

bool PagePresetManager::removeCollection(const QString &filePath)
{
	if (!updateIndexFile(filePath, IndexAction::Remove))
		qWarning() << "Index update failed (removal):" << filePath;

	return QFile::remove(filePath);
}

bool PagePresetManager::isCollectionsEmpty(const QString &filePath)
{
	QDomDocument doc;
	if (!loadDocument(filePath, doc))
		return false;

	QDomNodeList pagesNodes = doc.elementsByTagName("pages");

	if (pagesNodes.isEmpty())
		return true;

	QDomElement pagesElement = pagesNodes.at(0).toElement();
	QDomNode child = pagesElement.firstChild();
	while (!child.isNull())
	{
		if (child.isElement())
		{
			if (child.toElement().tagName() == "page")
				return false;
		}
		child = child.nextSibling();
	}

	return true;
}

bool PagePresetManager::addCollectionPage(const QString &filePath, const PageSizeInfo &pageInfo)
{
	QDomDocument doc;
	if (!loadDocument(filePath, doc))
	{
		qWarning() << "File not found:" << filePath;
		return false;
	}

	QDomElement root = doc.documentElement();
	QDomElement pagesNode = root.firstChildElement(QStringLiteral("pages"));

	if (pagesNode.isNull())
	{
		pagesNode = doc.createElement(QStringLiteral("pages"));
		root.appendChild(pagesNode);
	}

	int nextIdVal = 0;
	QDomElement pageElem = pagesNode.firstChildElement(QStringLiteral("page"));
	while (!pageElem.isNull())
	{
		bool ok;
		int currentId = pageElem.attribute(QStringLiteral("id")).toInt(&ok);
		if (ok && currentId >= nextIdVal)
			nextIdVal = currentId + 1;
		pageElem = pageElem.nextSiblingElement(QStringLiteral("page"));
	}

	QString newIdString = QStringLiteral("%1").arg(nextIdVal, 5, 10, QChar('0'));

	// Page tag
	QDomElement newPage = doc.createElement(QStringLiteral("page"));
	newPage.setAttribute(QStringLiteral("id"), newIdString);
	newPage.setAttribute(QStringLiteral("unit"), unitGetUntranslatedStrFromIndex(pageInfo.pageUnitIndex));
	newPage.setAttribute(QStringLiteral("width"), QString::number(pts2value(pageInfo.width, pageInfo.pageUnitIndex)));
	newPage.setAttribute(QStringLiteral("height"), QString::number(pts2value(pageInfo.height, pageInfo.pageUnitIndex)));
	newPage.setAttribute(QStringLiteral("name"), pageInfo.name);
	newPage.setAttribute(QStringLiteral("layout"), QString::number(pageInfo.layout));
	newPage.setAttribute(QStringLiteral("firstPage"), QString::number(pageInfo.firstPage));
	newPage.setAttribute(QStringLiteral("bindingDirection"), QString::number(pageInfo.bindingDirection));

	// Margin tag
	if (!pageInfo.margins.isNull())
	{
		QDomElement element = doc.createElement(QStringLiteral("margins"));
		element.setAttribute(QStringLiteral("left"), QString::number(pts2value(pageInfo.margins.left(), pageInfo.pageUnitIndex)));
		element.setAttribute(QStringLiteral("right"), QString::number(pts2value(pageInfo.margins.right(), pageInfo.pageUnitIndex)));
		element.setAttribute(QStringLiteral("top"), QString::number(pts2value(pageInfo.margins.top(), pageInfo.pageUnitIndex)));
		element.setAttribute(QStringLiteral("bottom"), QString::number(pts2value(pageInfo.margins.bottom(), pageInfo.pageUnitIndex)));
		element.setAttribute(QStringLiteral("preset"), QString::number(pageInfo.marginPreset));
		newPage.appendChild(element);
	}

	// Bleed tag
	if (!pageInfo.bleeds.isNull())
	{
		QDomElement element = doc.createElement(QStringLiteral("bleeds"));
		element.setAttribute(QStringLiteral("left"), QString::number(pts2value(pageInfo.bleeds.left(), pageInfo.pageUnitIndex)));
		element.setAttribute(QStringLiteral("right"), QString::number(pts2value(pageInfo.bleeds.right(), pageInfo.pageUnitIndex)));
		element.setAttribute(QStringLiteral("top"), QString::number(pts2value(pageInfo.bleeds.top(), pageInfo.pageUnitIndex)));
		element.setAttribute(QStringLiteral("bottom"), QString::number(pts2value(pageInfo.bleeds.bottom(), pageInfo.pageUnitIndex)));
		newPage.appendChild(element);
	}

	// Text frame
	if (!pageInfo.textFrame.isEmpty())
	{
		QDomElement element = doc.createElement(QStringLiteral("textFrame"));
		element.setAttribute(QStringLiteral("columns"), QString::number(pageInfo.textFrame.at(0)));
		element.setAttribute(QStringLiteral("gap"), QString::number(pts2value(pageInfo.textFrame.at(1), pageInfo.pageUnitIndex)));
		newPage.appendChild(element);
	}

	pagesNode.appendChild(newPage);

	return saveDocument(filePath, doc);
}

bool PagePresetManager::updateCollectionPage(const QString &filePath, const PageSizeInfo &pageInfo)
{
	const int separatorIdx = pageInfo.id.lastIndexOf(u'_');
	if (separatorIdx == -1)
		return false;

	const QString collectionId = pageInfo.id.left(separatorIdx);
	const QString pageId = pageInfo.id.mid(separatorIdx + 1);

	QDomDocument doc;
	if (!loadDocument(filePath, doc))
		return false;

	QDomElement root = doc.documentElement();
	if (root.attribute(QStringLiteral("id")) != collectionId)
		return false;

	QDomElement pagesNode = root.firstChildElement(QStringLiteral("pages"));
	QDomElement pageElem = pagesNode.firstChildElement(QStringLiteral("page"));

	while (!pageElem.isNull())
	{
		if (pageElem.attribute(QStringLiteral("id")) == pageId)
		{
			pageElem.setAttribute(QStringLiteral("unit"), unitGetUntranslatedStrFromIndex(pageInfo.pageUnitIndex));
			pageElem.setAttribute(QStringLiteral("width"), QString::number(pts2value(pageInfo.width, pageInfo.pageUnitIndex)));
			pageElem.setAttribute(QStringLiteral("height"), QString::number(pts2value(pageInfo.height, pageInfo.pageUnitIndex)));
			pageElem.setAttribute(QStringLiteral("name"), pageInfo.name);
			pageElem.setAttribute(QStringLiteral("layout"), QString::number(pageInfo.layout));
			pageElem.setAttribute(QStringLiteral("firstPage"), QString::number(pageInfo.firstPage));
			pageElem.setAttribute(QStringLiteral("bindingDirection"), QString::number(pageInfo.bindingDirection));

			// Margin tag
			QDomElement marginsElem = pageElem.firstChildElement(QStringLiteral("margins"));

			if (pageInfo.margins.isNull())
			{
				if (!marginsElem.isNull())
					pageElem.removeChild(marginsElem);
			}
			else
			{
				if (marginsElem.isNull())
				{
					marginsElem = doc.createElement(QStringLiteral("margins"));
					pageElem.appendChild(marginsElem);
				}

				marginsElem.setAttribute(QStringLiteral("left"), QString::number(pts2value(pageInfo.margins.left(), pageInfo.pageUnitIndex)));
				marginsElem.setAttribute(QStringLiteral("top"), QString::number(pts2value(pageInfo.margins.top(), pageInfo.pageUnitIndex)));
				marginsElem.setAttribute(QStringLiteral("right"), QString::number(pts2value(pageInfo.margins.right(), pageInfo.pageUnitIndex)));
				marginsElem.setAttribute(QStringLiteral("bottom"), QString::number(pts2value(pageInfo.margins.bottom(), pageInfo.pageUnitIndex)));
				marginsElem.setAttribute(QStringLiteral("preset"), QString::number(pageInfo.marginPreset));
			}

			// Bleed tag
			QDomElement bleedsElem = pageElem.firstChildElement(QStringLiteral("bleeds"));

			if (pageInfo.bleeds.isNull())
			{
				if (!bleedsElem.isNull())
					pageElem.removeChild(bleedsElem);
			}
			else
			{
				if (bleedsElem.isNull())
				{
					bleedsElem = doc.createElement(QStringLiteral("bleeds"));
					pageElem.appendChild(bleedsElem);
				}

				bleedsElem.setAttribute(QStringLiteral("left"), QString::number(pts2value(pageInfo.bleeds.left(), pageInfo.pageUnitIndex)));
				bleedsElem.setAttribute(QStringLiteral("top"), QString::number(pts2value(pageInfo.bleeds.top(), pageInfo.pageUnitIndex)));
				bleedsElem.setAttribute(QStringLiteral("right"), QString::number(pts2value(pageInfo.bleeds.right(), pageInfo.pageUnitIndex)));
				bleedsElem.setAttribute(QStringLiteral("bottom"), QString::number(pts2value(pageInfo.bleeds.bottom(), pageInfo.pageUnitIndex)));
			}

			// Text frame
			QDomElement textFrameElem = pageElem.firstChildElement(QStringLiteral("textFrame"));

			if (pageInfo.textFrame.isEmpty())
			{
				if (!textFrameElem.isNull())
					pageElem.removeChild(textFrameElem);
			}
			else
			{
				if (textFrameElem.isNull())
				{
					textFrameElem = doc.createElement(QStringLiteral("bleeds"));
					pageElem.appendChild(textFrameElem);
				}

				textFrameElem.setAttribute(QStringLiteral("columns"), QString::number(pageInfo.textFrame.at(0)));
				textFrameElem.setAttribute(QStringLiteral("gap"), QString::number(pts2value(pageInfo.textFrame.at(1), pageInfo.pageUnitIndex)));
			}

			return saveDocument(filePath, doc);
		}
		pageElem = pageElem.nextSiblingElement(QStringLiteral("page"));
	}
	return false;
}

bool PagePresetManager::removeCollectionPage(const QString &filePath, const QString &compositeId)
{
	const int separatorIdx = compositeId.lastIndexOf(u'_');
	if (separatorIdx == -1)
		return false;

	const QString collectionId = compositeId.left(separatorIdx);
	const QString pageId = compositeId.mid(separatorIdx + 1);

	QDomDocument doc;
	if (!loadDocument(filePath, doc))
		return false;

	QDomElement root = doc.documentElement();
	if (root.attribute(QStringLiteral("id")) != collectionId)
		return false;

	QDomElement pagesNode = root.firstChildElement(QStringLiteral("pages"));
	QDomElement pageElem = pagesNode.firstChildElement(QStringLiteral("page"));

	while (!pageElem.isNull())
	{
		if (pageElem.attribute(QStringLiteral("id")) == pageId)
		{
			pagesNode.removeChild(pageElem);
			return saveDocument(filePath, doc);
		}
		pageElem = pageElem.nextSiblingElement(QStringLiteral("page"));
	}
	return false;
}

bool PagePresetManager::loadDocument(const QString &filePath, QDomDocument &doc)
{
	QFile file(filePath);
	if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	return doc.setContent(&file) ? true : false;
}

bool PagePresetManager::saveDocument(const QString &filePath, const QDomDocument &doc)
{
	QFileInfo fi(filePath);
	if (!fi.dir().exists())
		fi.dir().mkpath(QStringLiteral("."));

	QSaveFile saveFile(filePath);
	if (saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&saveFile);
		out.setEncoding(QStringConverter::Utf8);
		doc.save(out, 4);
		return saveFile.commit();
	}
	return false;
}

void PagePresetManager::setChildText(QDomElement &parent, const QString &tagName, const QString &text)
{
	QDomElement element = parent.firstChildElement(tagName);

	if (element.isNull())
	{
		element = parent.ownerDocument().createElement(tagName);
		parent.appendChild(element);
	}

	while (element.hasChildNodes())
		element.removeChild(element.firstChild());

	element.appendChild(parent.ownerDocument().createTextNode(text));
}

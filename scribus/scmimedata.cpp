/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "scmimedata.h"

#include <QApplication>
#include <QClipboard>
#include <QMimeData>

// Define to 1 if you need to debug data contained in ScElemMimeData.
// This will have the effect of enabling the pasting of stored data
// in any text editor. Leave defined to 0 otherwise as this may trigger
// unwanted pasting of text in story editor (#15402). 
#define DEBUG_SCELEMMIMEDATA 0

const QString ScMimeData::ScribusElemMimeType     = "application/x-scribus-elem";
const QString ScMimeData::ScribusFragmentMimeType = "application/x-scribus-fragment";
const QString ScMimeData::ScribusTextMimeType     = "application/x-scribus-text";

bool ScMimeData::clipboardHasScribusData()
{
	bool  hasData = false;
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	if (mimeData)
	{
		hasData |= mimeData->hasFormat(ScMimeData::ScribusElemMimeType);
		hasData |= mimeData->hasFormat(ScMimeData::ScribusFragmentMimeType);
		hasData |= mimeData->hasFormat(ScMimeData::ScribusTextMimeType);
		hasData |= mimeData->hasText();
		hasData |= mimeData->hasFormat("application/x-openoffice-gdimetafile;windows_formatname=\"GDIMetaFile\"");
		hasData |= mimeData->hasFormat("image/svg+xml");
		hasData |= mimeData->hasFormat("image/x-inkscape-svg");
		hasData |= mimeData->hasFormat("application/vnd.oasis.opendocument.graphics");
	}
	return hasData;
}

bool ScMimeData::clipboardHasScribusElem()
{
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	if (mimeData)
		return mimeData->hasFormat(ScMimeData::ScribusElemMimeType);
	return false;
}

bool ScMimeData::clipboardHasScribusFragment()
{
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	if (mimeData)
		return mimeData->hasFormat(ScMimeData::ScribusFragmentMimeType);
	return false;
}

bool ScMimeData::clipboardHasScribusText()
{
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	if (mimeData)
		return mimeData->hasFormat(ScMimeData::ScribusTextMimeType);
	return false;
}

bool ScMimeData::clipboardHasPlainText()
{
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	if (mimeData)
		return mimeData->hasText();
	return false;
}

bool ScMimeData::clipboardHasKnownData()
{
	bool  hasData = false;
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	if (mimeData)
	{
		hasData |= mimeData->hasFormat("application/x-openoffice-gdimetafile;windows_formatname=\"GDIMetaFile\"");
		hasData |= mimeData->hasFormat("image/svg+xml");
		hasData |= mimeData->hasFormat("image/x-inkscape-svg");
		hasData |= mimeData->hasFormat("application/vnd.oasis.opendocument.graphics");
	}
	return hasData;
}

QString ScMimeData::clipboardKnownDataExt()
{
	QString ext;
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	if (mimeData)
	{
		if (mimeData->hasFormat("application/x-openoffice-gdimetafile;windows_formatname=\"GDIMetaFile\""))
			ext = "svm";
		else if (mimeData->hasFormat("image/svg+xml") || mimeData->hasFormat("image/x-inkscape-svg"))
			ext = "svg";
		else if (mimeData->hasFormat("application/vnd.oasis.opendocument.graphics"))
			ext = "odg";
	}
	return ext;
}

QByteArray ScMimeData::clipboardKnownDataData()
{
	QByteArray data;
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	if (mimeData)
	{
		if (mimeData->hasFormat("application/x-openoffice-gdimetafile;windows_formatname=\"GDIMetaFile\""))
			data = mimeData->data("application/x-openoffice-gdimetafile;windows_formatname=\"GDIMetaFile\"");
		else if (mimeData->hasFormat("image/svg+xml"))
			data = mimeData->data("image/svg+xml");
		else if (mimeData->hasFormat("image/x-inkscape-svg"))
			data = mimeData->data("image/x-inkscape-svg");
		else if (mimeData->hasFormat("application/vnd.oasis.opendocument.graphics"))
			data = mimeData->data("application/vnd.oasis.opendocument.graphics");
	}
	return data;
}

QString ScMimeData::clipboardScribusElem()
{
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	const ScElemMimeData* elemData = qobject_cast<const ScElemMimeData*>(mimeData);
	if (elemData)
		return elemData->scribusElem();
	QByteArray data = QApplication::clipboard()->mimeData()->data(ScribusElemMimeType);
	return QString::fromUtf8(data.data(), data.size());
}

QByteArray ScMimeData::clipboardScribusFragment()
{
	QByteArray data = QApplication::clipboard()->mimeData()->data(ScribusFragmentMimeType);
	return data;
}

QByteArray ScMimeData::clipboardScribusText()
{
	QByteArray data = QApplication::clipboard()->mimeData()->data(ScribusTextMimeType);
	return data;
}

ScElemMimeData::ScElemMimeData()
{
	m_formats << ScMimeData::ScribusElemMimeType;
#if DEBUG_SCELEMMIMEDATA
	m_formats << "text/plain";
#endif 
}

bool ScElemMimeData::hasFormat (const QString & mimeType) const
{
	if (mimeType == ScMimeData::ScribusElemMimeType)
	{
		if (m_scribusElemData.isEmpty())
			return false;
		QString elemtag = "SCRIBUSELEM";
		return (m_scribusElemData.lastIndexOf(elemtag, 50 + elemtag.length()) >= 0);
	}
#if DEBUG_SCELEMMIMEDATA
	if (mimeType == "text/plain")
		return (!m_scribusElemData.isEmpty());
#endif
	return QMimeData::hasFormat(mimeType);
}

QVariant ScElemMimeData::retrieveData (const QString &mimeType, QVariant::Type type) const
{
	if (mimeType == ScMimeData::ScribusElemMimeType)
	{
		if (m_scribusElemData.isEmpty())
			return QVariant();
		return QVariant(m_scribusElemData);
	}
#if DEBUG_SCELEMMIMEDATA
	if (mimeType == "text/plain")
		return QVariant(m_scribusElemData);
#endif
	return QVariant();
}

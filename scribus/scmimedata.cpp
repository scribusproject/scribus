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
#include <QClipBoard>
#include <QMimeData>

const QString ScMimeData::ScribusElemMimeType     = "application/x-scribus-elem";
const QString ScMimeData::ScribusFragmentMimeType = "application/x-scribus-fragment";
const QString ScMimeData::ScribusTextMimeType     = "application/x-scribus-text";

bool ScMimeData::clipboardHasScribusData(void)
{
	bool  hasData = false;
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	if (mimeData)
	{
		hasData |= mimeData->hasFormat(ScMimeData::ScribusElemMimeType);
		hasData |= mimeData->hasFormat(ScMimeData::ScribusFragmentMimeType);
		hasData |= mimeData->hasFormat(ScMimeData::ScribusTextMimeType);
		hasData |= mimeData->hasText();
	}
	return hasData;
}

bool ScMimeData::clipboardHasScribusElem(void)
{
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	if (mimeData)
		return mimeData->hasFormat(ScMimeData::ScribusElemMimeType);
	return false;
}

bool ScMimeData::clipboardHasScribusFragment(void)
{
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	if (mimeData)
		return mimeData->hasFormat(ScMimeData::ScribusFragmentMimeType);
	return false;
}

bool ScMimeData::clipboardHasScribusText(void)
{
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	if (mimeData)
		return mimeData->hasFormat(ScMimeData::ScribusTextMimeType);
	return false;
}

QString ScMimeData::clipboardScribusElem(void)
{
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	const ScElemMimeData* elemData = dynamic_cast<const ScElemMimeData*>(mimeData);
	if (elemData)
		return elemData->scribusElem();
	QByteArray data = QApplication::clipboard()->mimeData()->data(ScribusElemMimeType);
	return QString::fromUtf8(data.data(), data.size());
}

QByteArray ScMimeData::clipboardScribusFragment(void)
{
	QByteArray data = QApplication::clipboard()->mimeData()->data(ScribusFragmentMimeType);
	return data;
}

QByteArray ScMimeData::clipboardScribusText(void)
{
	QByteArray data = QApplication::clipboard()->mimeData()->data(ScribusTextMimeType);
	return data;
}

ScElemMimeData::ScElemMimeData(void) : QMimeData()
{
	m_formats << "application/x-scribus-elem" << "text/plain";
}

bool ScElemMimeData::hasFormat (const QString & mimeType) const
{
	bool hasFmt = false;
	if (mimeType == ScMimeData::ScribusElemMimeType)
	{
		if (!m_scribusElemData.isEmpty())
		{
			QString elemtag = "SCRIBUSELEM";
			return (m_scribusElemData.lastIndexOf(elemtag, 50 + elemtag.length()) >= 0);
		}
	}
	else if (mimeType == "text/plain")
		return true;
	return QMimeData::hasFormat(mimeType);
}

QVariant ScElemMimeData::retrieveData (const QString &mimeType, QVariant::Type type) const
{
	QVariant variant;
	if (mimeType == ScMimeData::ScribusElemMimeType)
		variant = QVariant(m_scribusElemData);
	if (mimeType == "text/plain")
		variant = QVariant(m_scribusElemData);
	return variant;
}

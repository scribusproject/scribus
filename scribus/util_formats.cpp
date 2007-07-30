/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QImageReader>
#include <QMapIterator>

#include "util_formats.h"

FormatsManager* FormatsManager::_instance = 0;

FormatsManager::FormatsManager()
{
	m_fmts.insert(FormatsManager::EPS, QStringList());
	m_fmts.insert(FormatsManager::GIF, QStringList());
	m_fmts.insert(FormatsManager::JPEG, QStringList());
	m_fmts.insert(FormatsManager::PAT, QStringList());
	m_fmts.insert(FormatsManager::PDF, QStringList());
	m_fmts.insert(FormatsManager::PNG, QStringList());
	m_fmts.insert(FormatsManager::PS, QStringList());
	m_fmts.insert(FormatsManager::PSD, QStringList());
	m_fmts.insert(FormatsManager::TIFF, QStringList());
	m_fmts.insert(FormatsManager::XPM, QStringList());
	
	m_fmts[FormatsManager::EPS] << "eps" << "epsf" << "epsi";
	m_fmts[FormatsManager::GIF] << "gif";
	m_fmts[FormatsManager::JPEG] << "jpg" << "jpeg";
	m_fmts[FormatsManager::PAT] << "pat";
	m_fmts[FormatsManager::PDF] << "pdf";
	m_fmts[FormatsManager::PNG] << "png";
	m_fmts[FormatsManager::PS] << "ps";
	m_fmts[FormatsManager::PSD] << "psd";
	m_fmts[FormatsManager::TIFF] << "tif" << "tiff";
	m_fmts[FormatsManager::XPM] << "xpm";
	
	QMapIterator<int, QStringList> i(m_fmts);
	while (i.hasNext()) 
	{
		i.next();
		m_fmtList << i.value().first().upper();
	}

	m_qtSupportedImageFormats=QImageReader::supportedImageFormats();
	m_supportedImageFormats=m_qtSupportedImageFormats
	updateSupportedImageFormats(m_supportedImageFormats);
}

FormatsManager::~FormatsManager()
{
}

FormatsManager* FormatsManager::instance()
{
	if (_instance == 0)
		_instance = new FormatsManager();

	return _instance;
}

void FormatsManager::deleteInstance()
{
	if (_instance)
		delete _instance;
	_instance = 0;
}

void FormatsManager::imageFormatSupported(const QString& ext)
{
	return m_supportedImageFormats.contains(QByteArray(ext));
}

void FormatsManager::updateSupportedImageFormats(QList<QByteArray>& supportedImageFormats)
{
	QMapIterator<int, QStringList> it(m_fmts);
	while (it.hasNext())
	{
		it.next();
		QStringListIterator itSL(it.value());
		while (itSL.hasNext())
		{
			QString t(itSL.next());
			supportedImageFormats.append(t.toLocal8Bit());
		}
	}
}

void FormatsManager::fileTypeStrings(int type, QString& formatList, QString& formatText, QString& formatAll, bool lowerCaseOnly)
{
	QString fmtList = QObject::tr("All Supported Formats")+" (";
	QString fmtText;
	QMapIterator<int, QStringList> it(m_fmts);
	bool first=true;
	int n=0;
	while (it.hasNext())
	{
		it.next();
		if (type & it.key())
		{
			//Just in case the Qt used doesn't support jpeg or gif
			if ((JPEG & it.key()) && !m_supportedImageFormats.contains(QByteArray("jpg")))
				return;
			if ((GIF & it.key()) && !m_supportedImageFormats.contains(QByteArray("gif")))
				return;
			if (first)
				first=false;
			else
				fmtList += " ";
			QString text=m_fmtList[n] + " (";
			QStringListIterator itSL(it.value());
			while (itSL.hasNext())
			{
				QString t("*." + itSL.next());
				fmtList += t;
				text += t;
				if(!lowerCaseOnly)
				{
					fmtList += " " + t.upper();
					text += " " + t.upper();
				}
				if (itSL.hasNext())
				{
					fmtList += " ";
					text += " ";
				}
			}
			text += ");;";
			fmtText += text;
		}
		++n;
	}
	formatList+=fmtList += ");;";
	formatText+=fmtText;
	formatAll=QObject::tr("All Files (*)");
}

bool extensionIndicatesPDF(const QString &ext)
{
// 	QStringList strl;
// 	strl << "pdf";
// 	return strl.contains(ext, Qt::CaseInsensitive);
	return (QString::compare("pdf", ext, Qt::CaseInsensitive) == 0);
}

bool extensionIndicatesEPS(const QString &ext)
{
	QStringList strl;
	strl << "eps" << "epsf" << "epsi";
	return strl.contains(ext, Qt::CaseInsensitive);
}

bool extensionIndicatesEPSorPS(const QString &ext)
{
	QStringList strl;
	strl << "eps" << "epsf" << "epsi" << "ps";
	return strl.contains(ext, Qt::CaseInsensitive);
}

bool extensionIndicatesTIFF(const QString &ext)
{
	QStringList strl;
	strl << "tif" << "tiff";
	return strl.contains(ext, Qt::CaseInsensitive);
}

bool extensionIndicatesPSD(const QString &ext)
{
// 	QStringList strl;
// 	strl << "psd";
// 	return strl.contains(ext, Qt::CaseInsensitive);
	return (QString::compare("psd", ext, Qt::CaseInsensitive) == 0);
}

bool extensionIndicatesJPEG(const QString &ext)
{
	QStringList strl;
	strl << "jpg" << "jpeg";
	return strl.contains(ext, Qt::CaseInsensitive);
}

bool extensionIndicatesPattern(const QString &ext)
{
	QStringList strl;
	strl << "pat";
	return strl.contains(ext, Qt::CaseInsensitive);
}

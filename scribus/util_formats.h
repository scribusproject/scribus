/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

//Added by Craig Bradney in July 2007
//To be used for basic format detection and checking
//One central place for storing all our extensions we support, etc

#ifndef _UTIL_FORMATs_H
#define _UTIL_FORMATs_H

#include <QByteArray>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>
#include "scribusapi.h"

bool SCRIBUS_API extensionIndicatesEPS(const QString &ext);
bool SCRIBUS_API extensionIndicatesEPSorPS(const QString &ext);
bool SCRIBUS_API extensionIndicatesJPEG(const QString &ext);
bool SCRIBUS_API extensionIndicatesPDF(const QString &ext);
bool SCRIBUS_API extensionIndicatesPSD(const QString &ext);
bool SCRIBUS_API extensionIndicatesPattern(const QString &ext);
bool SCRIBUS_API extensionIndicatesTIFF(const QString &ext);


class SCRIBUS_API FormatsManager
{
	public:

		enum ScImageFormatType
		{
			ALLIMAGES = 1|2|4|8|16|32|64|128|256|512,
			EPS		= 1, // Encapsulated PostScript
			GIF		= 2, // GIF files
			JPEG	= 4, // JPEG
			PAT		= 8, // Pattern files
			PDF		= 16, // PDF Format
			PNG		= 32, // PNG files
			PS		= 64, // PostScript
			PSD		= 128, // Photoshop Format
			TIFF	= 256, // TIFF
			XPM		= 512, // XPM files
		};

		FormatsManager();
		~FormatsManager();

	/**
		 * @brief Returns a pointer to the FormatsManager instance
		 * @return A pointer to the FormatsManager instance
	 */
		static FormatsManager* instance();
	/**
		 * @brief Deletes the FormatsManager Instance
		 * Must be called when FormatsManager is no longer needed.
	 */
		static void deleteInstance();
		void imageFormatSupported(const QString&);
		
		void fileTypeStrings(int type, QString& formatList, QString& formatText, QString& formatAll, bool lowerCaseOnly=false);
		
	protected:
		QMap<int, QStringList> m_fmts;
		QStringList m_fmtList;
		QList<QByteArray> m_qtSupportedImageFormats
		QList<QByteArray> m_supportedImageFormats;
		void updateSupportedImageFormats(QList<QByteArray>& supportedImageFormats);
		
	private:
	/**
	 * @brief The only instance of FormatsManager available.
	 *
	 * FormatsManager is singleton and the instance can be queried with the method
	 * instance().
	 */
	static FormatsManager* _instance;
};

#endif

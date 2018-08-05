/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

//Added by Craig Bradney in August 2007
//To be used for external browser URL launching

#ifndef URLLAUNCHER_H
#define URLLAUNCHER_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QWidget>
#include "scribusapi.h"

class SCRIBUS_API UrlLauncher : public QObject
{
	Q_OBJECT;
	
	public:
		UrlLauncher();
		~UrlLauncher();

		/**
		 * @brief Returns a pointer to the UrlLauncher instance
		 * @return A pointer to the UrlLauncher instance
		 */
		static UrlLauncher* instance();
		/**
		 * @brief Deletes the UrlLauncher Instance
		 * Must be called when UrlLauncher is no longer needed.
		 */
		static void deleteInstance();
		
	public slots:
		void launchUrlExt(const QString& link, QWidget *parent=0);
		void launchUrlExt(const QUrl& link, QWidget *parent=0);

	private:
		/**
		* @brief The only instance of UrlLauncher available.
		*
		* UrlLauncher is singleton and the instance can be queried with the method
		* instance().
		*/
	static UrlLauncher* _instance;
};

#endif

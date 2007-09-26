/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _SCRIBUS_PIXMAPEXPORT_H_
#define _SCRIBUS_PIXMAPEXPORT_H_

#include <QString>
#include <QFileDialog>
#include <pluginapi.h>
#include <loadsaveplugin.h>
#include <vector>

class ScrAction;

class PLUGIN_API PixmapExportPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		PixmapExportPlugin();
		virtual ~PixmapExportPlugin();
		virtual bool run(ScribusDoc* doc, QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};

		// Special features (none)
};

extern "C" PLUGIN_API int scribusexportpixmap_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scribusexportpixmap_getPlugin();
extern "C" PLUGIN_API void scribusexportpixmap_freePlugin(ScPlugin* plugin);



/*! \brief Handles export. */
class ExportBitmap: public QObject
{
	Q_OBJECT
public:
	/*! \brief Initializing the default export variables and attributes */
	ExportBitmap();
	/*! \brief nothing doing destructor. */
	~ExportBitmap();

	/*! \brief Type of the exported image */
	QString bitmapType;
	/*! \brief Dpi of the exported image */
	int pageDPI;
	/*! \brief Enlargement of the exported image... 2x 3x etc. */
	double enlargement;
	/*! \brief Quality of the image <0; 100> */
	int quality;
	/*! \brief A place for stored images */
	QString exportDir;
	/*! \brief Overwrite the existing files? */
	bool overwrite;

	/*! \brief Exports only the actual page
	\retval bool true on success */
	bool exportCurrent(ScribusDoc* doc);
	/*! \brief Exports chosen interval of the pages
	\param pageNs interval of the page numbers
	\retval true on success */
	bool exportInterval(ScribusDoc* doc, std::vector<int> &pageNs);
private:
	/*! \brief create specified filename "docfilename-005.ext" */
	QString getFileName(ScribusDoc* doc, uint pageNr);
	/*! \brief export one specified page
	\param pageNr number of the page
	\param single bool TRUE if only the one page is exported
	\retval bool true on success
	*/
	bool exportPage(ScribusDoc* doc, uint pageNr, bool single);
};

#endif

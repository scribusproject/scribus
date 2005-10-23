#ifndef _SCRIBUS_PIXMAPEXPORT_H_
#define _SCRIBUS_PIXMAPEXPORT_H_

#include <qstring.h>
#include <qfiledialog.h>
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
		virtual bool run(QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();

		// Special features (none)
};

extern "C" PLUGIN_API int scribusexportpixmap_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scribusexportpixmap_getPlugin();
extern "C" PLUGIN_API void scribusexportpixmap_freePlugin(ScPlugin* plugin);



/*! Handles export. */
class ExportBitmap: public QObject
{
	Q_OBJECT
public:
	/*! Initializing the default export variables and attributes */
	ExportBitmap();
	/*! nothing doing destructor. */
	~ExportBitmap();

	/*! Type of the exported image */
	QString bitmapType;
	/*! Dpi of the exported image */
	int pageDPI;
	/*! Enlargement of the exported image... 2x 3x etc. */
	int enlargement;
	/*! Quality of the image <0; 100> */
	int quality;
	/*! A place for stored images */
	QString exportDir;
	/*! Overwrite the existing files? */
	bool overwrite;

	/*! Exports only the actual page */
	bool exportActual();
	/*! Exports chosen interval of the pages */
	bool exportInterval(std::vector<int> &pageNs);
private:
	/*! create specified filename "docfilename-005.ext" */
	QString getFileName(uint pageNr);
	/*! export one specified page
	\param pageNr number of the page
	\param single bool TRUE if only the one page is exported
	*/
	bool exportPage(uint pageNr, bool single);
};

ScrAction *fileNewFromTemplateAction;

#endif

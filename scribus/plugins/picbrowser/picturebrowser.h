/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PICTUREBROWSER_H
#define PICTUREBROWSER_H

#include <QDirModel>
#include "ui_picturebrowser.h"

//previewicon class
#include <QPixmap>
#include "scimage.h"
#include "scribusstructs.h"
#include "cmsettings.h"
#include "insertaframe.h"
#include "scribusdoc.h"

#include "imagedialog.h"

//previewImagesModel class
#include <QAbstractListModel>
#include <QtAlgorithms>

//threads support
#include <QThread>

//documentbrowser
#include "pageitem.h"

//xml support for collections
#include <QtXml>
#include <QXmlStreamReader>

//scribuspaths
#include "scpaths.h"
#include "scribuscore.h"

//colorspace related things
#include "util_color.h"

//open/savefile dialog
#include <QFileDialog>

//settings class
#include <prefsfile.h>
#include <prefscontext.h>

class picturebrowserSettings;
class ImageInformation;
class previewImage;
class previewImages;
class PreviewImagesModel;
class imageCollection;
class collections;
class collectionReaderThread;
class collectionListReaderThread;
class collectionWriterThread;
class collectionsWriterThread;
class loadImagesThread;
class loadImagesThreadInstance;
class findImagesThread;
class QImage;
class imageFilters;

//functions for comparing attributes of previewimages (used for sorting)
bool comparePreviewImageFileName ( const previewImage *i1, const previewImage *i2 );
bool comparePreviewImageFileType ( const previewImage *i1, const previewImage *i2 );
bool comparePreviewImageFileDate ( const previewImage *i1, const previewImage *i2 );
bool comparePreviewImageFileSize ( const previewImage *i1, const previewImage *i2 );
bool comparePreviewImageResolution ( const previewImage *i1, const previewImage *i2 );


class picturebrowserSettings
{
	public:
		picturebrowserSettings();

		void load();
		void save();
		void reset();

		//save settings?
		bool saveSettings;
		//tells if dialog is enlarged
		bool showMore;
		//the sortorder, false for ascending, true for descending
		bool sortOrder;
		//sorting, 0 for name, 1 for date, 2 for size, 3 for type
		int sortSetting;
		//previewmode, 0 for "previewimages only", 1 for "preview with information"
		int previewMode;
		//the size of the icons
		int previewIconSize;
		//tells if always-on-top is enabled
		bool alwaysOnTop;
};


//central class, manages gui
class PictureBrowser : public QDialog, Ui::PictureBrowser
{
		Q_OBJECT

	public:
		PictureBrowser ( ScribusDoc* doc, QWidget *parent = 0 );
		~PictureBrowser();
		void changedDocument ( ScribusDoc* doc );
		void closedDocument();

		//posts an image which should be loaded to the imageloading thread
		void callLoadImageThread ( int row, int pId );
		picturebrowserSettings pbSettings;

		//for statusinfo, contains how many images are currently displayed/filtered
		int imagesDisplayed, imagesFiltered;
		//contains how many icons are visible at once, is calculated from QListView height/width and previewIconSize
		int previewIconsVisible;
		int currentRow;
		bool saveSettings;

	signals:
		//signal which posts a new job to the imageloading thread
		//parameters:
		//previewImage* imageToLoad: pointer to identify the previewimage
		//QString path: path to the image
		//int size: size of the icon, the returned QImage will be resized according to this
		//int pId: a unique id to check wether the pointer is valid or a different set of images has been chosen while thread was executed
		void loadImageJob ( int, QString, int, int );
		//signals for selecting a page in the current document
		void selectPage ( int );
		void selectMasterPage ( QString );

	private slots:
		//slot for the navigation combobox, sets current browsingmode (folderbrowser, collectionsbrowser, documentbrowser)
		void navigate ( int index );
		//called when a previewicon was clicked
		void previewIconClicked ( const QModelIndex &index );
		void previewIconDoubleClicked ( const QModelIndex &index );
		//called when sorting was changed, re-sorts images and refreshes view
		void sortChanged ( int index );
		//called when previewmode was changed, changes previewmode
		void previewModeChanged ( int index );
		//called when more button was clicked, enlarges dialog
		void moreButtonClicked();
		void resetSettingsButtonClicked();
		//called when an action was selected
		void actionsGoButtonClicked();
		//called when sortorder was changed, updates view
		void sortOrderButtonClicked();
		//called when zoombutton + was clicked, enlarges previewicons
		void zoomPlusButtonClicked();
		//called when zoombutton - was clicked, scales down previewicons
		void zoomMinusButtonClicked();
		void tabWidgetCurrentChanged ( int index );
		//called when "Goto page" in documentbrowser was clicked
		void gotoPageButtonClicked();
		//called when always-on-top checkbox was clicked, toggles always-on-top-feature
		void alwaysOnTopCheckboxStateChanged();
		void saveSettingsCheckboxStateChanged();
		//called when "include subdirs" checkbox in folderbrowser was clicked, toggles recursive folderbrowsing on/off
		void subdirsCheckboxStateChanged();
		//called when a dir is clicked in the folderbrowser, calls thread to find images
		void dirChosen ( const QModelIndex &index );
		//called when a selection is made in the documentbrowser, lists the images and displays them
		void documentChosen ( QTreeWidgetItem * item, int column );
		//called when a collection is selected in the collectionsbrowser, reads the images from the file and displays them
		void collectionChosen ( QTreeWidgetItem * item, int column );
		void collectionsWidgetItemEdited ( QTreeWidgetItem * item, int column );
		//called when the thread has finished browsing a folder for images, displays the images
		void findImagesThreadFinished();
		//called when the collectionreaderthread has finished reading a file, displays the images
		void collectionReaderThreadFinished();
		void collectionListReaderThreadFinished();
		void collectionReaderThreadListFinishedSave();
		void collectionsDbWriterThreadFinished();
		void collectionWriterThreadListFinished();
		//called when entries have been selected from insertPagesCombobox
		void insertPagesComboboxCheckstateChanged ( int row );
		void insertImageButtonClicked();
		void insertPositionComboboxChanged ( int index );
		void insertSizeComboboxChanged ( int index );
		void filterCriteriaComboboxChanged ( int index );
		void filterTargetComboboxChanged ( int index );
		void filterFilterButtonClicked();
		void filterClearButtonClicked();
		void filterAddCriterionButtonClicked();
		void filterSearchDirButtonClicked();
		void collectionsNewCategoryButtonClicked();
		void collectionsNewButtonClicked();
		void collectionsImportButtonClicked();
		void collectionsExportButtonClicked();
		void collectionsDeleteButtonClicked();
		void collectionsRenameButtonClicked();
		void collectionsAddImagesButtonClicked();
		void collectionsRemoveImagesButtonClicked();
		void collectionsTagImagesButtonClicked();
		void collectionsAddImagesOkButtonClicked();
		void collectionsAddImagesCancelButtonClicked();
		void previewImageSelectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
		void collectionsSetTagsButtonClicked();
		void collectionsAddNewTagButtonClicked();

	private:
		//updates the QListView when the set of previewimages was changed (new images, zoom, previewmode changed etc)
		void loadIcons();
		void setSettings();
		void updateDocumentbrowser();
		void updateBrowser ( bool filter, bool sort, bool reload );
		void updateInformationTab ( int index );
		void updateCollectionsWidget ( bool addImages );
		void updateCollectionsAddImagesCombobox();
		//expands/shrinks dialog
		void expandDialog ( bool expand );
		void setAlwaysOnTop ( bool alwaysOnTop );
		void saveCollectionsDb();
		void applyFilters();
		void updateTagImagesTab();

		//current document
		ScribusDoc* srcDoc;
		// Main window
		ScribusMainWindow* ScMW;
		//the dirmodel for the folderbrowser
		QDirModel folderModel;
		//model for QListView containing the previewimages
		PreviewImagesModel *pModel;
		//contains the currently selected index in folderModel
		QModelIndex tmpindex;
		//tells if thread searches dirs recursively
		bool folderBrowserIncludeSubdirs;
		//index of the previewicon currently selected
		int previewIconIndex;
		//a list containing the items in the documentbrowser treewidget
		QList<QTreeWidgetItem *> documentItems;
		//contain the icons needed
		QIcon *iconArrowUp, *iconArrowDown, *iconFolderBrowser, *iconCollectionsBrowser, *iconDocumentBrowser, *iconDocument, *iconCollection, *iconZoomPlus, *iconZoomMinus, *iconOk, *iconClose, *iconNew, *iconNew2, *iconEdit, *iconRemove, *iconLoad, *iconSave, *iconPlus, *iconMinus, *iconPen;
		//thread for searching dirs
		findImagesThread *fit;
		//the current set of previewimages
		previewImages *pImages;
		//the path currently selected in folderbrowser
		QString currPath;
		//a thread for loading images
		loadImagesThread *lit;
		//a thread for reading a collectionsfile
		collectionReaderThread *crt;
		QList<collectionReaderThread *> crtList;
		//the collection currently selected
		QString currCollectionFile;
		imageCollection *currCollection;
		//a list of threads for saving a collection
		QList<collectionWriterThread *> cwtList;
		collectionListReaderThread *clrt;
		collectionsWriterThread *cdbwt;
		//custom position for inserting image enabled/disabled
		bool insertCustomPosition;
		bool insertCustomSize;
		QStringList nameFilters;
		imageFilters *filters;
		QString cdbFile;
		QList<int> selectedIndexes;

		//contains the hierarchy for the collectionsbrowser treewidget
		QList<collections *> collectionsDb;
};


//holds information about an image
class ImageInformation
{
	public:
		ImageInformation();

		int width, height;
		int type;
		int colorspace;
		int xdpi, ydpi;
		int layers;
		bool embedded;
		QString profileName;
		bool valid;
};


//class for a single previewimage, contains all information related to the image
class previewImage
{
	public:
		//constructor, sets filepath for an image
		previewImage ( QString imageFile );
		~previewImage();

		//creates the previewicon in desired size in pixels
		//parameters:
		//const QImage &image: image to create icon from
		//int size: the desired iconsize
		bool createPreviewIcon ( const QImage &image, int size );

		//creates a new imageframe on a page and inserts the image
		bool insertIntoDocument ( ScribusDoc *doc, InsertAFrameData &iafData );
		//inserts the image into a given imageframe
		bool insertIntoImageFrame ( ScribusDoc *doc, PageItem *imageFrame );

		//filter attribute. if filter settings match, its set to true and no longer considered for display/sorting
		bool filtered;
		//contains fileinformation
		QFileInfo fileInformation;
		//keeps imageinformation
		ImageInformation *imgInfo;
		//current size of previewIcon
		int currentSize;
		//tells wether previewIcon is loading in thread
		bool previewImageLoading;
		//tells if previewIcon was created previously or needs to be loaded
		bool previewIconCreated;
		//contains previewicon which is to be shown
		QPixmap previewIcon;
		//tags
		QStringList tags;
};


//class for managing a set of previewImage objects
class previewImages
{
	public:
		//constructor, adds imageFiles to this set
		previewImages ( const QStringList& imageFiles );

		//creates previewimage objects from a QStringList
		void createPreviewImagesList ( const QStringList& imageFiles );
		//creates previewimage objects from imageCollection
		void createPreviewImagesList ( const imageCollection *collection );
		//deletes all entries
		void clearPreviewImagesList();

		//little helper function to reduce code
		bool toRemove ( bool a, bool b );
		//filters by filename, invert specifies wether to include or to remove file fileName
		void filterFileName ( const QString &fileName, bool invert );
		//filters by filesize, smallerThan specifies wether to include or to remove files smaller than fileSize
		void filterFileSize ( qint64 fileSize, bool smallerThan );
		//filters by filetype, invert specifies wether to include or to remove files with fileExtension
		void filterFileType ( const QStringList &types, bool invert );
		//filters by date last modified, smallerThan specifies wether to include or to remove files younger than modified
		void filterFileModified ( const QDateTime &modified, bool smallerThan );
		//filters by imageresolution, smallerThan specifies wether to include or to remove images with lower resolution
		void filterResolution ( qint64 resolution, bool smallerThan );
		//filters by tag, invert specifies wether to include or to remove item with tag
		void filterTag ( const QStringList &tags, bool invert );
		//clears all previously applied filters
		void clearFilters();

		//sorts previewImagesList after sort criteria
		//0: sort by filename
		//1: sort by fileextension
		//2: sort by date last modified
		//3: sort by filesize
		void sortPreviewImages ( const int sort );

		//all our precious previewimages are indexed here
		QList<previewImage *> previewImagesList;
};


//a model which holds the previewimages and is connected to QListView
class PreviewImagesModel : public QAbstractListModel
{
		Q_OBJECT

	public:
		PreviewImagesModel ( QObject *parent = 0 );
		//saves pointer to calling PictureBrowser instance, for later access
		PreviewImagesModel ( PictureBrowser *parent );

		//creates the default icon which is displayed when image is not loaded yet
		//parameters:
		//int size: size of the icon to be created
		void createDefaultIcon ( int size );
		//sets the modelitemslist to previewImagesList, ignores filtered images
		void setModelItemsList ( const QList<previewImage *> &previewImagesList );
		//removes all items
		void clearModelItemsList();

		//returns previewicons and, depending on previewmode, additional text
		QVariant data ( const QModelIndex &index, int role = Qt::DisplayRole ) const;
		//returns flags for items
		Qt::ItemFlags flags ( const QModelIndex &index ) const;
		//returns the number of previewimages
		int rowCount ( const QModelIndex &parent ) const;

		QMimeData *mimeData ( const QModelIndexList &indexes ) const;
		QStringList mimeTypes() const;

		//all our precious previewimages are indexed here
		QList<previewImage *> modelItemsList;
		//a unique id telling the thread wether to cancel current loading operations because files have changed
		int pId;

	private slots:
		//called when imageloadingthread has finished
		//parameters:
		//previewImage* loadedImage: pointer to identify the loaded image
		//const QImage image: the actual image
		//int tpId: the id the thread has been called from
		void processLoadedImage ( int row, const QImage image, ImageInformation* imgInfo, int tpId );
		void processImageLoadError ( int row, int tpId, int errorcode );

	private:
		//pointer to the picturebrowser class
		PictureBrowser *pictureBrowser;
		//default icon
		QPixmap defaultIcon;
		//icon size of defaultIcon
		int defaultIconSize;
};


class imageCollection
{
	public:
		//creates collection object setting name and file
		imageCollection();

		//name of the collection
		QString name;
		//path to the collectionfile
		QString file;
		//paths to the images in the collection
		QStringList imageFiles;
		//tags to each image;
		QList<QStringList> tags;

};


//manages a set of collections
class collections
{
	public:
		//sets name
		collections ( QString collectionsName );

		//contains name of the collections
		QString name;

		QStringList collectionNames;
		QStringList collectionFiles;
};


//a thread to read collectionfiles
class collectionReaderThread : public QXmlStreamReader, public QThread
{
	public:
		//set file to read
		collectionReaderThread ( QString &xmlFile2, bool importCollection );

		//starts reading the file
		void readFile();

		//is executed after starting the thread
		void run();
		//restarts the thread
		void restart();

		//true if thread should restart
		volatile bool restartThread;

		//hierarchy for collectionsbrowser qtreewidget
		QList<collections *> collectionsSet;
		//contains the images read from the file
		imageCollection *collection;

		//tells which filetype it was, 0 for collectionsset, 1 for collection
		int type;
		//if it is an import
		bool import;
		//the file to read from
		QString xmlFile;
		QStringList addImages;

	private:
		//reads collectionsdatabase
		void readCollectionsDb();
		//reads a collectionfile
		void readCollectionFile();

		//reads the category of a collectionsdb
		void readCategory();
		//reads a collection from a collectionsdb
		void readCollection();

		//reads an image from a collectionfile
		void readImage();

		//called when an unknown element is encountered
		void readUnknownElement();

		//tells how many categories the collectionsdb contained
		int categoriesCount;
};


class collectionListReaderThread : public QThread
{
		Q_OBJECT

	public:
		//set files to read
		collectionListReaderThread ( QStringList &xmlFiles2 );

		//is executed after starting the thread
		void run();
		//restarts the thread
		void restart();

		//true if thread should restart
		volatile bool restartThread;

		QString xmlFile;
		QStringList xmlFiles;

		QList<imageCollection *> readCollections;

	private slots:
		void collectionReaderThreadFinished();

	private:
		collectionReaderThread *clrt;
};


//thread to write a collectionssetfile
class collectionsWriterThread : public QXmlStreamWriter, public QThread
{
	public:
		//sets initial values
		//parameters:
		//QString &xmlFile2: the file to write
		//int fileType: the type of file, 0 for collectionfile, 1 for collectionsdb
		//QList<collections *> saveCollections2: the collectionsset to write to the file
		collectionsWriterThread ( QString &xmlFile2, QList<collections *> saveCollections2 );
		//starts writing to the file
		void writeFile();

		//called after the thread has been started
		void run();
		//restarts the thread
		void restart();

		//true if thread should restart
		volatile bool restartThread;

	private:
		//writes a category into a collectionssetfile
		void writeCategory ( const collections *category );
		//writes a collection into a collectionssetfile
		void writeCollection ( const QString &collectionName, const QString &collectionFile );

		//contains the path to the output file
		QString xmlFile;
		//the collection to write into a collectionfile
		QList<collections *> saveCollections;
};


//thread to write a collectionfile
class collectionWriterThread : public QXmlStreamWriter, public QThread
{
	public:
		//sets initial values
		//parameters:
		//QString &xmlFile2: the file to write
		//imageCollection *saveCollection2: the collection to write to the file
		collectionWriterThread ( QString &xmlFile2, imageCollection &saveCollection2 );
		//starts writing to the file
		void writeFile();

		//called after the thread has been started
		void run();

	private:
		//writes an image into a collectionfile
		void writeImage ( const QString &imageFile, const QStringList &tags );
		void writeTags ( const QStringList &tags );

		//contains the path to the output file
		QString xmlFile;
		//the collection to write into a collectionfile
		imageCollection saveCollection;
};


//a thread to load previewimages
class loadImagesThread : public QThread
{
		Q_OBJECT

	public:
		//sets pointer to calling PictureBrowser object and to the related previewImagesModel
		loadImagesThread ( PictureBrowser *parent, PreviewImagesModel *parentModel );

		//called after the thread has been started
		void run();

	private:
		//contains a pointer to the calling PictureBrowser object
		PictureBrowser *pictureBrowser;
		//contains a pointer to the related PreviewImagesModel
		PreviewImagesModel *pModel;
};


//a helper class to get the signals/slots executed in the right thread
class loadImagesThreadInstance : public QObject
{
		Q_OBJECT

	public:
		loadImagesThreadInstance();

		//contains a pointer to the calling PictureBrowser object
		PictureBrowser *pictureBrowser;
		//contains a pointer to the related PreviewImagesModel
		PreviewImagesModel *pModel;

	signals:
		//emitted when image was loaded
		//parameters:
		//previewImage* loadedImage: pointer to the previewImage object which receives the image
		//const QImage image: the actual image
		//int tpId: unique id the thread has been called with
		void imageLoaded ( int, const QImage, ImageInformation*, int );
		void imageLoadError ( int, int, int );

	private slots:
		//called when an image should be loaded
		//parameters:
		//previewImage* loadImage: pointer to the previewImage object which should receive the image
		//QString path: path to the image
		//int size: desired size of the icon
		//int tpId: unique id
		void processLoadImageJob ( int row, QString path, int size, int tpId );
};


//thread to search images in a folder and subfolders
class findImagesThread : public QThread
{
	public:
		//parameters:
		//const QString& path2: path to be searched
		//const QStringList& nameFilters2: namefilters for the images
		//QDir::SortFlags sort2: sortflags
		//bool searchSubfolders2: tells wether to search subfolders
		findImagesThread ( const QString& path2, const QStringList& nameFilters2, QDir::SortFlags sort2, bool searchSubfolders2 );
		//called after thread has been started
		void run();
		//called when the thread should be restarted
		void restart();

		//finds the images in a given folder
		void findFiles ( const QString& path );

		//contains the images which have been found
		QStringList imageFiles;
		//tells wether the thread should restart
		volatile bool restartThread;

	private:
		//path to start searching
		QString startPath;
		//namefilters for search
		QStringList nameFilters;
		//sortflags for search
		QDir::SortFlags sort;
		//tells wether to search subfolders
		bool searchSubfolders;
};


class imageFilters
{
	public:
		imageFilters();

		QList<int> filterMap;
		QList<int> filterMap2;

		QStringList nameFilters;
		QList<bool> nameInverts;

		QList<QDateTime> dateFilters;
		QList<bool> dateInverts;

		QList<qint64> sizeFilters;
		QList<bool> sizeInverts;

		QList<QStringList> tagFilters;

		QList<QStringList> typeFilters;
};


#endif

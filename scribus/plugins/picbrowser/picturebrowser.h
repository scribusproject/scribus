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
#include "ui/insertaframe.h"
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

/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef COLORLISTBOX_H
#define COLORLISTBOX_H

#include <QColor>
#include <QListView>
#include <QListWidget>
#include <QPixmap>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "colorlistmodel.h"
#include "colorsetmanager.h"
#include "commonstrings.h"
#include "sccolor.h"
#include "scguardedptr.h"
#include "sclistboxpixmap.h"
#include "scribusapi.h"

/*! \brief Very nice list box with color names and samples.
It's inherited from QListBox with all its methods and properties.
I create it as separate class because it's used now in ColorManager
and ColorWheel too. You can see it in Extras/Color Wheel or in
Edit/Colors dialogs in action.
\author Petr Vanek <petr@yarpen.cz>
*/
class SCRIBUS_API ColorListBox : public QListView
{
	Q_OBJECT

	public:

		enum PixmapType
		{
			smallPixmap,
			widePixmap,
			fancyPixmap
		};

		/*! \brief Standard QListBox like constructor.
		Just there are initialized pixmaps for icon drawing. */
		ColorListBox(QWidget* parent = nullptr);
		ColorListBox(ColorListBox::PixmapType type, QWidget* parent = nullptr);
		~ColorListBox();

		/*! Return the current color name */
		QString currentColor() const;

		/*! Return the current row */
		int currentRow() const;

		/*! Return row count */
		int count() const;

		/*! Return model data for the given row and role */
		QVariant data(int row, int role) const;

		/*! Finds colors whose name matches the given string name using the given flags. */
		QStringList findColors(const QString &name, Qt::MatchFlags flags) const;

		/*! Return if a color is currently selected */
		bool hasSelection() const;

		/*! Insert color at specified position */
		void insertItem(int row, const ScColor& color, const QString& colorName);

		/*! Return if none color is shown at beginning oh list */
		bool isNoneColorShown() const;
		
		/*! \brief Retrieve the pixmap type used by this listbox */
		ColorListBox::PixmapType pixmapType() const { return m_type; }

		/*! \brief Fill the list box with values taken from list.
		The list is cleared itself. Then is rendered an icon with
		color attributes (RGB/CMYK/Spot etc.).
		\param list a ColorList to present. */
		void updateBox(ColorList& list);

		/*! Remove list item at specified index */
		void removeItem(int i);

		/*! Find row index for specified colorname */
		int row(const QString& colorName);

		/*! \brief Fill the list box with values taken from list.
		The list is cleared itself. Then is rendered an icon with
		color attributes (RGB/CMYK/Spot etc.).
		\param list a ColorList to present. */
		void setColors(ColorList& list, bool insertNone);

		/*! \brief Set the current color */
		void setCurrentColor(QString colorName);

		/*! \brief Set the current row */
		void setCurrentRow(int row);

		/*! \brief Set the pixmap type used by this listbox */
		void setPixmapType(ColorListBox::PixmapType type);

		//! Set if None color is shown as first item in the list
		void setShowNoneColor(bool showNone);

		/*! \brief Return text displayed at specified row */
		QString text(int row) const;
				
		/*! \brief Pointer to the color list displayed by this box */
		ColorList* cList { nullptr };

	public slots:
		void clear();

	private slots:
		void slotRightClick();

	protected slots:
		void emitCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
		void emitItemClicked(const QModelIndex &current);
		void emitItemDoubleClicked(const QModelIndex &current);

		virtual void iconSetChange();
		virtual void languageChange();

	signals:
		void currentRowChanged(int currentRow);
		void currentTextChanged(const QString &currentText);
		void contextMenuRequested();
		void itemClicked(int currentRow);
		void itemDoubleClicked(int currentRow);
		void itemSelectionChanged();

	protected:
		void changeEvent(QEvent *e) override;
		bool viewportEvent(QEvent *event) override;

		static int initialized;
		static int sortRule;
		ColorListBox::PixmapType m_type;
};

#endif

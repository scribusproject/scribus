#ifndef GRADIENTLISTBOX_H
#define GRADIENTLISTBOX_H


#include <QColor>
#include <QListView>
#include <QListWidget>
#include <QPixmap>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "models/gradientlistmodel.h"
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
class SCRIBUS_API GradientListBox : public QListView
{
	Q_OBJECT

	public:

		enum PixmapType
		{
			widePixmap
		};

		/*! \brief Standard QListBox like constructor.
		Just there are initialized pixmaps for icon drawing. */
		GradientListBox(QWidget* parent = nullptr);
		GradientListBox(GradientListBox::PixmapType type, QWidget* parent = nullptr);
		~GradientListBox();

		/*! Return the current color name */
		QString currentGradientName() const;

		VGradient currentGradient() const;

		/*! Return the current row */
		int currentRow() const;

		/*! Return row count */
		int count() const;

		/*! Return model data for the given row and role */
		QVariant data(int row, int role) const;

		/*! Finds colors whose name matches the given string name using the given flags. */
		QStringList findGradients(const QString &name, Qt::MatchFlags flags) const;

		/*! Return if a color is currently selected */
		bool hasSelection() const;

		/*! Insert color at specified position */
		void insertItem(int row, const VGradient &gradient, const QString& gradientName);

		/*! Return if none color is shown at beginning oh list */
		bool isNoneColorShown() const;

		/*! \brief Retrieve the pixmap type used by this listbox */
		GradientListBox::PixmapType pixmapType() const { return m_type; }

		/*! \brief Fill the list box with values taken from list.
		The list is cleared itself. Then is rendered an icon with
		color attributes (RGB/CMYK/Spot etc.).
		\param list a ColorList to present. */
		void updateBox(GradientList& list);

		/*! Remove list item at specified index */
		void removeItem(int i);

		/*! Find row index for specified colorname */
		int row(const QString& gradientName) const;

		/*! \brief Fill the list box with values taken from list.
		The list is cleared itself. Then is rendered an icon with
		color attributes (RGB/CMYK/Spot etc.).
		\param list a ColorList to present. */
		void setGradients(GradientList& list, bool insertNone);

		/*! \brief Set the current color */
		void setCurrentGradient(QString gradientName);

		/*! \brief Set the current row */
		void setCurrentRow(int row);

		/*! \brief Set the pixmap type used by this listbox */
		void setPixmapType(GradientListBox::PixmapType type);

		//! Set if None color is shown as first item in the list
		void setShowNoneColor(bool showNone);

		/*! \brief Return text displayed at specified row */
		QString text(int row) const;

		/*! \brief Pointer to the color list displayed by this box */
		GradientList* cList { nullptr };

	private:
		void connectSignals();
		void disconnectSignals();

	public slots:
		void clear();

	private slots:
		void slotRightClick();

	protected slots:
		void emitCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
		void emitItemClicked(const QModelIndex &current);
		void emitItemDoubleClicked(const QModelIndex &current);

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
		GradientListBox::PixmapType m_type;
};

#endif // GRADIENTLISTBOX_H

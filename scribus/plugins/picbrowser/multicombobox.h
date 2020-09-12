/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QComboBox>
#include <QListView>
#include <QStandardItemModel>

class QEvent;
class QObject;
class QModelIndex;
class QWidget;
class multiCombobox;

class multiComboboxModel : public QStandardItemModel
{
	Q_OBJECT
	public:
		multiComboboxModel ( QObject* parent = nullptr );
		Qt::ItemFlags flags ( const QModelIndex& index ) const;
};


class multiView : public QListView
{
	Q_OBJECT
	public:
		multiView ( QWidget* parent = nullptr );
		multiView ( multiCombobox* parent );
		bool eventFilter ( QObject* object, QEvent* event );

	private:
		multiCombobox *parentMcb;
};


class multiCombobox : public QComboBox
{
	Q_OBJECT
	public:
		multiCombobox ( QWidget* parent = nullptr );

		void setCheckstate ( int index, int checked );
		int checkstate ( int index );
		void switchCheckstate ( int row );

		int addItem ( const QString& text, int checked=0 );

	private:
		multiComboboxModel *mcbModel;
		multiView *mcbView;

	signals:
		void checkstateChanged ( int );
};

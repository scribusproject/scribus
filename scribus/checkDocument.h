#ifndef CHECKDOCUMENT_H
#define CHECKDOCUMENT_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListView;
class QListViewItem;
class QComboBox;
class QLabel;
class ScribusDoc;

class CheckDocument : public QDialog
{
	Q_OBJECT

public:
	CheckDocument( QWidget* parent, bool modal );
	~CheckDocument() {};
	void clearErrorList();
	void buildErrorList(ScribusDoc *doc);
	void closeEvent(QCloseEvent *ce);

	QComboBox* curCheckProfile;
	QLabel* textLabel1;
	QListView* reportDisplay;
	QMap<QListViewItem*, int> itemMap;
	QMap<QListViewItem*, int> pageMap;

public slots:
	void slotSelect(QListViewItem* ite);
	void newScan(const QString&);

signals:
	void rescan();
	void closePal();
	void selectElement(int, int);
	void selectPage(int);

protected:
	QVBoxLayout* checkDocumentLayout;
	QHBoxLayout* layout1;

protected slots:
	virtual void languageChange();

private:
	ScribusDoc* document;
	QPixmap graveError;
	QPixmap onlyWarning;
	QPixmap noErrors;

};

#endif // CHECKDOCUMENT_H

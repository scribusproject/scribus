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
class QPushButton;
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

	QListView* reportDisplay;
	QPushButton* okButton;
	QPushButton* newCheck;

public slots:
	void newScan();

signals:
	void rescan();
	void closePal();

protected:
	QVBoxLayout* checkDocumentLayout;
	QHBoxLayout* layout1;

protected slots:
	virtual void languageChange();

private:
	QPixmap graveError;
	QPixmap onlyWarning;
	QPixmap noErrors;

};

#endif // CHECKDOCUMENT_H

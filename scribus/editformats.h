#ifndef STILFORMATE_H
#define STILFORMATE_H

#include <qdialog.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qmap.h>

#include "scribusstructs.h"
class ScribusDoc;

class ChooseStyles : public QDialog
{
	Q_OBJECT

public:
	ChooseStyles( QWidget* parent, QValueList<ParagraphStyle> *styleList, QValueList<ParagraphStyle> *styleOld );
	~ChooseStyles() {};

	QListView* StyleView;
	QPushButton* OkButton;
	QPushButton* CancelButton;
	QMap<QCheckListItem*, int> storedStyles;

protected:
	QVBoxLayout* ChooseStylesLayout;
	QHBoxLayout* layout2;

};

class StilFormate : public QDialog
{
	Q_OBJECT

public:
	StilFormate( QWidget* parent, ScribusDoc *doc);
	~StilFormate() {};

	QListBox* ListBox1;
	QPushButton* LoadS;
	QPushButton* NewB;
	QPushButton* EditB;
	QPushButton* DublicateB;
	QPushButton* DeleteB;
	QPushButton* SaveB;
	QPushButton* ExitB;
	QPushButton* CancelB;
	int sFnumber;
	QValueList<ParagraphStyle> TempVorl;
	ScribusDoc *Docu;
	void UpdateFList();

public slots:
	void saveIt();
	void selFormat(QListBoxItem *c);
	void selEditFormat(QListBoxItem *c);
	void editFormat();
	void neuesFormat();
	void dupFormat();
	void deleteFormat();
	void loadStyles();

signals:
	void saveStyle(StilFormate *);

protected:
	QHBoxLayout* StilFormateLayout;
	QVBoxLayout* Layout15;
};

#endif // STILFORMATE_H

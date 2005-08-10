#ifndef LINEFORMATE_H
#define LINEFORMATE_H

#include <qdialog.h>

#include "scribusapi.h"
#include "multiline.h"
#include "scribusstructs.h"

class QListBox;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class ScribusDoc;

class SCRIBUS_API LineFormate : public QDialog
{
	Q_OBJECT

public:
	LineFormate( QWidget* parent, ScribusDoc *doc);
	~LineFormate() {};

	QListBox* ListBox1;
	QPushButton* LoadLS;
	QPushButton* NewB;
	QPushButton* EditB;
	QPushButton* DublicateB;
	QPushButton* DeleteB;
	QPushButton* SaveB;
	QPushButton* ExitB;
	QPushButton* CancelB;
	QString sFnumber;
	QMap<QString,multiLine> TempStyles;
	QMap<QString,QString> Replacement;
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
	void loadLStyles();

signals:
	void saveStyle(LineFormate *);
	
protected:
	QHBoxLayout* StilFormateLayout;
	QVBoxLayout* Layout15;
};

#endif

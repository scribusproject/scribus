#ifndef LINEFORMATE_H
#define LINEFORMATE_H

#include <qdialog.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "scribusapi.h"
#include "multiline.h"
#include "scribusstructs.h"

class Q3ListBox;
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

	Q3ListBox* ListBox1;
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
	void selFormat(Q3ListBoxItem *c);
	void selEditFormat(Q3ListBoxItem *c);
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

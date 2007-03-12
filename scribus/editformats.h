/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef STILFORMATE_H
#define STILFORMATE_H

#include <qdialog.h>
#include <q3listbox.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qmap.h>
#include <qlabel.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include "scribusapi.h"
#include "scribusstructs.h"
#include "styles/styleset.h"

class ScribusDoc;
class ScComboBox;

/*! \brief Dialog to replace deleted style with one existing */
class SCRIBUS_API DelStyle : public QDialog
{
	Q_OBJECT

public:
	DelStyle(QWidget* parent, StyleSet<ParagraphStyle>& sty, QString styleName);
	~DelStyle() {};
	const QString getReplacementStyle();

private:
	QLabel* deleteLabel;
	QLabel* styleToDelLabel;
	QLabel* replaceLabel;
	QPushButton* okButton;
	QPushButton* cancelButton;
	ScComboBox* replacementStyleData;
	Q3VBoxLayout* dialogLayout;
	Q3GridLayout* delStyleLayout;
	Q3HBoxLayout* okCancelLayout;

	QString replacementStyle;

private slots:
	virtual void ReplaceStyle(int);
};


class SCRIBUS_API ChooseStyles : public QDialog
{
	Q_OBJECT

public:
	ChooseStyles( QWidget* parent, StyleSet<ParagraphStyle> *styleList, StyleSet<ParagraphStyle> *styleOld );
	~ChooseStyles() {};

	Q3ListView* StyleView;
	QPushButton* OkButton;
	QPushButton* CancelButton;
	QMap<Q3CheckListItem*, int> storedStyles;

protected:
	Q3VBoxLayout* ChooseStylesLayout;
	Q3HBoxLayout* layout2;

};


class SCRIBUS_API StilFormate : public QDialog
{
	Q_OBJECT

public:
	StilFormate( QWidget* parent, ScribusDoc *doc);
	//~StilFormate() {};

	Q3ListBox* ListBox1;
	QPushButton* LoadS;
	QPushButton* NewB;
	QPushButton* EditB;
	QPushButton* DublicateB;
	QPushButton* DeleteB;
	QPushButton* SaveB;
	QPushButton* ExitB;
	QPushButton* CancelB;
	int sFnumber;
	StyleSet<ParagraphStyle> TempVorl;
	QMap<QString, QString> ReplaceList;
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
	void loadStyles();

signals:
	void saveStyle(StilFormate *);

protected:
	Q3HBoxLayout* StilFormateLayout;
	Q3VBoxLayout* Layout15;
};

#endif // STILFORMATE_H

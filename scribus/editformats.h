#ifndef STILFORMATE_H
#define STILFORMATE_H

#include <qdialog.h>
#include <q3listbox.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qmap.h>
#include <QVBoxLayout>
#include <Q3ValueList>
#include <QHBoxLayout>
#include <qlabel.h>

#include "scribusapi.h"
#include "scribusstructs.h"
class ScribusDoc;
class ScComboBox;

class SCRIBUS_API DelStyle : public QDialog
{
	Q_OBJECT

public:
	DelStyle(QWidget* parent, Q3ValueList<ParagraphStyle> sty, QString styleName);
	~DelStyle() {};
	const QString getReplacementStyle();

private:
	QLabel* deleteLabel;
	QLabel* styleToDelLabel;
	QLabel* replaceLabel;
	QPushButton* okButton;
	QPushButton* cancelButton;
	ScComboBox* replacementStyleData;
	QVBoxLayout* dialogLayout;
	QGridLayout* delStyleLayout;
	QHBoxLayout* okCancelLayout;

	QString replacementStyle;

private slots:
	virtual void ReplaceStyle(int);
};

class SCRIBUS_API ChooseStyles : public QDialog
{
	Q_OBJECT

public:
	ChooseStyles( QWidget* parent, Q3ValueList<ParagraphStyle> *styleList, Q3ValueList<ParagraphStyle> *styleOld );
	~ChooseStyles() {};

	Q3ListView* StyleView;
	QPushButton* OkButton;
	QPushButton* CancelButton;
	QMap<Q3CheckListItem*, int> storedStyles;

protected:
	QVBoxLayout* ChooseStylesLayout;
	QHBoxLayout* layout2;

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
	Q3ValueList<ParagraphStyle> TempVorl;
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
	QHBoxLayout* StilFormateLayout;
	QVBoxLayout* Layout15;
};

#endif // STILFORMATE_H

#ifndef MULTILINE_H
#define MULTILINE_H

#include <qdialog.h>
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpixmap.h>
#include <qlineedit.h>

#include "scribusstructs.h"

#include "linecombo.h"
#include "mspinbox.h"

class ScribusDoc;

class MultiLine : public QDialog
{
	Q_OBJECT

public:
	MultiLine( QWidget* parent, ScribusDoc* doc, multiLine ml, QString nam, QMap<QString,multiLine> *Sty);
	void updatePreview();
	QColor calcFarbe(QString name, int shade);
	void updateSList();
	void reSort();
	void RebuildList();
	~MultiLine() {};

	QLineEdit* SName;
	QLabel* Preview;
	QPushButton* AddStyle;
	QPushButton* RemoveStyle;
	QListBox* Styles;
	QGroupBox* Properties;
	LineCombo* Dashes;
	QComboBox* LineEnds;
	QComboBox* LineJoin;
	QLabel* WidthText;
	MSpinBox* LWidth;
	QComboBox* Color;
	QSpinBox* Shade;
	QPushButton* OK;
	QPushButton* Cancel;
	multiLine TempVorl;
	int CurLin;
	ScribusDoc* Docu;
	QMap<QString,multiLine> *TempStyles;
	QString GivenName;

public slots:
	void NewName();
	void DelSubLine();
	void NewSubLine();
	void NewLJoin();
	void NewLEnd();
	void NewLSty();
	void NewLColor();
	void NewLWidth();
	void NewLShade();
	void slotEditStyle( int i);

protected:
	QVBoxLayout* MultiLineLayout;
	QHBoxLayout* layout2;
	QVBoxLayout* PropertiesLayout;
	QHBoxLayout* layout1;
	QHBoxLayout* layout3;
	QHBoxLayout* layout4;
};

#endif // MULTILINE_H

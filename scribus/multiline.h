/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MULTILINE_H
#define MULTILINE_H

#include <qdialog.h>
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <q3listbox.h>
#include <q3groupbox.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qpixmap.h>
#include <qlineedit.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

#include "scribusapi.h"
#include "scribusstructs.h"

#include "linecombo.h"
#include "scrspinbox.h"

class ScribusDoc;

class SCRIBUS_API MultiLine : public QDialog
{
	Q_OBJECT

public:
	MultiLine( QWidget* parent, ScribusDoc* doc, multiLine ml, QString nam, QMap<QString,multiLine> *Sty);
	void updatePreview();
	QColor calcFarbe(const QString& name, int shade);
	void updateSList();
	void reSort();
	void RebuildList();
	~MultiLine() {};

	QLineEdit* SName;
	QLabel* Preview;
	QPushButton* AddStyle;
	QPushButton* RemoveStyle;
	Q3ListBox* Styles;
	Q3GroupBox* Properties;
	LineCombo* Dashes;
	QComboBox* LineEnds;
	QComboBox* LineJoin;
	QLabel* WidthText;
	ScrSpinBox* LWidth;
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
	Q3VBoxLayout* MultiLineLayout;
	Q3HBoxLayout* layout2;
	Q3VBoxLayout* PropertiesLayout;
	Q3HBoxLayout* layout1;
	Q3HBoxLayout* layout3;
	Q3HBoxLayout* layout4;
};

#endif // MULTILINE_H

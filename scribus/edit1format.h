#ifndef EDITSTYLE_H
#define EDITSTYLE_H

#include <qdialog.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include "scribusstructs.h"
#include "mspinbox.h"
#include "fontcombo.h"
class ScribusDoc;
#include "alignselect.h"
class StyleSelect;
#include "shadebutton.h"
class Tabruler;

class EditStyle : public QDialog
{
	Q_OBJECT

public:
	EditStyle( QWidget* parent, struct StVorL *vor, QValueList<StVorL> v, bool neu, preV *Prefs,
	           double au, int dEin, ScribusDoc *doc);
	~EditStyle() {};

	QLabel* TextLabel1;
	QLabel* TextLabel3;
	QLabel* TextLabel1_2_2;
	QLabel* TextLabel1_2_3;
	QLineEdit* Name;
	MSpinBox* LineSpVal;
	Tabruler* TabList;
	MSpinBox* AboveV;
	MSpinBox* BelowV;
	QLabel* EffeLabel;
	StyleSelect* EffeS;
	QLabel* AligLabel;
	AlignSelect* AligS;
	QGroupBox* GroupBox10;
	QGroupBox* AbstandV;
	QGroupBox* GroupFont;
	QLabel* TextF1;
	FontCombo* FontC;
	MSpinBox* SizeC;
	QLabel* TextF2;
	QLabel* StrokeIcon;
	QComboBox* TxStroke;
	ShadeButton *PM1;
	QLabel* FillIcon;
	QComboBox* TxFill;
	ShadeButton *PM2;
	QCheckBox* DropCaps;
	QCheckBox* BaseGrid;
	QLabel* CapLabel;
	QSpinBox* DropLines;
	QPushButton* Cancel;
	QPushButton* OkButton;
	struct StVorL *werte;
	QValueList<StVorL> allV;
	QString OldName;
	bool IsNew;
	double AutoVal;
	int DocsEin;
	//! Label for holding "style preview" bitmap 12/30/2004 petr vanek
	QLabel *previewText;
	//! Reference to the own ScribusDoc (preferences etc.) 12/30/2004 petr vanek
	ScribusDoc *parentDoc;

private slots:
	void Verlassen();
	void FontChange();
	void ManageDrops();
	void ColorChange();
	//! Refresh slot for "style preview" bitmap fonts 12/30/2004 petr vanek
	void FontC_activated(const QString &);

private:
	/*! Refresh label for holding "style preview" bitmap.
	12/30/2004 petr vanek */
	void updatePreview();

protected:
	QHBoxLayout* Layout17;
	QGridLayout* EditStyleLayout;
	QVBoxLayout* GroupBox10Layout;
	QGridLayout* AbstandVLayout;
	QGridLayout* GroupFontLayout;
};

#endif // EDITSTYLE_H

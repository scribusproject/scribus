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
#include "alignselect.h"
#include "shadebutton.h"

class ScribusDoc;
class StyleSelect;
class Tabruler;
class PageItem;


class EditStyle : public QDialog
{
	Q_OBJECT

public:
	EditStyle( QWidget* parent, struct ParagraphStyle *vor, QValueList<ParagraphStyle> v, bool neu, ApplicationPrefs *Prefs,
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
	QButtonGroup* AbstandV;
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
	QRadioButton* BaseGrid;
	QRadioButton* FixedLsp;
	QRadioButton* AutoLsp;
	QLabel* CapLabel;
	QSpinBox* DropLines;
	QLabel* CapLabel2;
	MSpinBox* DropDist;
	QPushButton* Cancel;
	QPushButton* OkButton;
	struct ParagraphStyle *werte;
	QValueList<ParagraphStyle> allV;
	QString OldName;
	bool IsNew;
	double AutoVal;
	int DocsEin;
	//! Label for holding "style preview" bitmap 12/30/2004 petr vanek
	QLabel *previewText;
	QCheckBox *previewCaption;
	//! sample text for preview
	QString lorem;
	//! sample text frame
	PageItem *previewItem;
	//! Reference to the own ScribusDoc (preferences etc.) 12/30/2004 petr vanek
	ScribusDoc *parentDoc;
	ApplicationPrefs *PrefsData;

private slots:
	void toggleLsp();
	void Verlassen();
	void FontChange();
	void ManageDrops();
	void ColorChange();
	/*! \brief Paragraph style preview generator.
	It takes values from the UI form, creates a temporary style.
	This style is appended into global styles list and removed from there
	after finishing. Then is created an item to draw QPixmap with it.
	\author Petr Vanek
	\date 4/7/2005
	*/
	void togglePreview();
	void updatePreview();

protected:
	QHBoxLayout* Layout17;
	QVBoxLayout* layoutPreview;
	QGridLayout* EditStyleLayout;
	QVBoxLayout* GroupBox10Layout;
	QGridLayout* AbstandVLayout;
	QGridLayout* GroupFontLayout;
};

#endif // EDITSTYLE_H

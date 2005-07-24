#ifndef MARGINDIALOG_H
#define MARGINDIALOG_H

class QDialog;
class QLayout;
class QGroupBox;
class QLabel;
class QPushbutton;
class MSpinBox;
class MarginWidget;
class ScribusDoc;

class MarginDialog : public QDialog
{
	Q_OBJECT

public:
	MarginDialog( QWidget* parent,  ScribusDoc* doc);
	~MarginDialog() {};
	QGroupBox* dsGroupBox7;
	MSpinBox* widthMSpinBox;
	MSpinBox* heightMSpinBox;
	QLabel*	widthQLabel;
	QLabel*	heightQLabel;
	MarginWidget* GroupRand;
	QPushButton* cancelButton;
	QPushButton* okButton;

protected:
	QGridLayout* dsGroupBox7Layout;
	QVBoxLayout* dialogLayout;
	QHBoxLayout* okCancelLayout;
};

#endif

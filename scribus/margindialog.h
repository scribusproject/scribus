#ifndef MARGINDIALOG_H
#define MARGINDIALOG_H

class QDialog;
class QLayout;
class QPushbutton;
class MarginWidget;
class ScribusDoc;

class MarginDialog : public QDialog
{
	Q_OBJECT

public:
	MarginDialog( QWidget* parent,  ScribusDoc* doc);
	~MarginDialog() {};
	MarginWidget* GroupRand;
	QPushButton* cancelButton;
	QPushButton* okButton;

protected:
	QVBoxLayout* dialogLayout;
	QHBoxLayout* okCancelLayout;
};

#endif

/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/
#ifndef STDIALOG_H
#define STDIALOG_H

#include <vector>
#include <utility>
#include <qcombobox.h>
#include <qstringlist.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qdialog.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qwidget.h>

typedef std::pair<QString*,QString*> Pair;

class satdialog : public QDialog
{
	Q_OBJECT

private:
	QLabel* nameLabel;
	QLabel* psizeLabel;
	QLabel* colorsLabel;
	QLabel* descrLabel;
	QLabel* usageLabel;
	QLabel* authorLabel;
	QLabel* emailLabel;
	QLabel* catsLabel;
	QPushButton* okButton;
	QPushButton* detailButton;
	QWidget* center;
	QWidget* middle;
	bool isFullDetail;
	void fullDetail();
	void minimumDetail();
	void setupCategories();
public:
	std::vector<Pair*> cats;
	QLineEdit* nameEdit;
	QComboBox* catsCombo;
	QLineEdit* psizeEdit;
	QLineEdit* colorsEdit;
	QTextEdit* descrEdit;
	QTextEdit* usageEdit;
	QLineEdit* authorEdit;
	QLineEdit* emailEdit;
	satdialog(QWidget* parent, QString tmplName = "");
	~satdialog();
private slots:
	void detailClicked();
};

#endif


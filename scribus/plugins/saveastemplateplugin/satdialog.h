/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/
#ifndef SATDIALOG_H
#define SATDIALOG_H

#include <vector>
#include <utility>
#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>
#include <qcombobox.h>
#include <qstring.h>
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
	QString author;
	QString email;
	bool isFullDetail;
	void fullDetail();
	void minimumDetail();
	void readPrefs();
	void writePrefs();
	void setupCategories();
	void setupPageSize(int w, int h);
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
	satdialog(QWidget* parent, QString tmplName = "", int pageW = 0, int pageH = 0);
	~satdialog();
private slots:
	void detailClicked();
};

#endif


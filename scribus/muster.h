#ifndef MUSTERPAGES_H
#define MUSTERPAGES_H

#include <qdialog.h>
class QListBox;
class QListBoxItem;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

class ScribusDoc;
class ScribusView;

class MusterPages : public QDialog
{
	Q_OBJECT

public:
	MusterPages( QWidget* parent, ScribusDoc *pCurrentDoc, ScribusView *pCurrentView, QString temp);
	~MusterPages() {};
	void closeEvent(QCloseEvent *closeEvent);
	void updateTemplateList(QString templateName);

public slots:
	void selectTemplate(QString name);

private:
	QListBox* templateData;
	QPushButton* appendButton;
	QPushButton* newButton;
	QPushButton* duplicateButton;
	QPushButton* deleteButton;
	QPushButton* closeButton;
	ScribusDoc *currentDoc;
	ScribusView *currentView;
	QString sMuster;

protected:
	QVBoxLayout* musterPagesLayout;
	QHBoxLayout* buttonLayout;

private slots:
	void duplicateTemplate();
	void deleteTemplate();
	void newTemplate();
	void appendPage();
	void selectTemplate(QListBoxItem *item);

signals:
	void createNew(int);
	void loadPage(QString, int, bool);
	void finished();

};

#endif // MUSTERPAGES_H

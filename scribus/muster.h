/****************************************************************************
** Form interface generated from reading ui file 'Muster.ui'
**
** Created: Thu Nov 1 09:45:36 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
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

private:
    void updateTemplateList(QString templateName);
    QListBox* templateData;
	QPushButton* appendButton;
    QPushButton* newButton;
    QPushButton* duplicateButton;
    QPushButton* deleteButton;
    QPushButton* closeButton;
    ScribusDoc *currentDoc;
    ScribusView *currentView;
    QString sMuster;
    QHBoxLayout* musterPagesLayout;
    QVBoxLayout* buttonLayout;

private slots:
	void exitEditor();
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

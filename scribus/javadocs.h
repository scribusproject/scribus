#ifndef JAVADOCS_H
#define JAVADOCS_H

#include <qdialog.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include "scribusdoc.h"
#include "scribusview.h"

class JavaDocs : public QDialog
{ 
    Q_OBJECT

public:
    JavaDocs( QWidget* parent, ScribusDoc *doc, ScribusView* vie);
    ~JavaDocs() {};

    QListBox* Scripts;
    QPushButton* EditScript;
    QPushButton* AddScript;
    QPushButton* DeleteScript;
    QPushButton* ExitDia;
    ScribusDoc *Doc;
		ScribusView *View;

private slots:
		void slotAdd();
		void slotEdit();
		void slotDelete();

protected:
    QHBoxLayout* JavaDocsLayout;
    QVBoxLayout* Layout1;
};

#endif // JAVADOCS_H

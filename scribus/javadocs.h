#ifndef JAVADOCS_H
#define JAVADOCS_H

#include <qdialog.h>
#include <q3listbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "scribusapi.h"
class ScribusDoc;
class ScribusView;

class SCRIBUS_API JavaDocs : public QDialog
{ 
    Q_OBJECT

public:
    JavaDocs( QWidget* parent, ScribusDoc *doc, ScribusView* vie);
    ~JavaDocs() {};

    Q3ListBox* Scripts;
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

signals:
	void docChanged(bool);

protected:
    QHBoxLayout* JavaDocsLayout;
    QVBoxLayout* Layout1;
};

#endif // JAVADOCS_H

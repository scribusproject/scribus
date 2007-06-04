/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef JAVADOCS_H
#define JAVADOCS_H

#include <QDialog>

#include "scribusapi.h"
class QListWidget;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class ScribusDoc;
class ScribusView;

class SCRIBUS_API JavaDocs : public QDialog
{
	Q_OBJECT

public:
	JavaDocs( QWidget* parent, ScribusDoc *doc, ScribusView* vie);
	~JavaDocs() {};

	QListWidget* Scripts;
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

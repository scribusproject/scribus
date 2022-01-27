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

	QListWidget* Scripts { nullptr };
	QPushButton* EditScript { nullptr };
	QPushButton* AddScript { nullptr };
	QPushButton* DeleteScript { nullptr };
	QPushButton* ExitDia { nullptr };

private slots:
	void slotAdd();
	void slotEdit();
	void slotDelete();
	void slotSelectionChanged();

signals:
	void docChanged(bool);

protected:
	ScribusDoc  *m_Doc { nullptr };
	ScribusView *m_View { nullptr };

	QHBoxLayout* JavaDocsLayout { nullptr };
	QVBoxLayout* Layout1 { nullptr };
};

#endif // JAVADOCS_H

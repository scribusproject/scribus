/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMSTYLEIMPORT_H
#define SMSTYLEIMPORT_H

#include <QDialog>
#include <QMap>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>

#include "scribusapi.h"
#include "styles/styleset.h"
#include "scribusstructs.h"

class QWidget;
class Q3ListView;
class QPushButton;
class Q3CheckListItem;
class Q3VBoxLayout;
class Q3HBoxLayout;
class ParagraphStyle;
class Q3ButtonGroup;
class QRadioButton;

class SCRIBUS_API ImportDialog : public QDialog
{
	Q_OBJECT

public:
	ImportDialog(QWidget* parent,
	             StyleSet<ParagraphStyle> *pstyleList,
	             StyleSet<CharStyle> *cstyleList,
	             QMap<QString, multiLine> *lstyleList);
	~ImportDialog() {};

	QMap<Q3CheckListItem*, QString> storedStyles;
	QMap<Q3CheckListItem*, QString> storedCharStyles;
	QMap<Q3CheckListItem*, QString> storedLineStyles;

	bool clashRename();

protected:
	Q3ListView* StyleView;
	QPushButton* OkButton;
	QPushButton* CancelButton;

	Q3VBoxLayout  *ChooseStylesLayout;
	Q3HBoxLayout  *layout2;
	Q3ButtonGroup *clashBox;
	QRadioButton *renameButton;
	QRadioButton *replaceButton;
};

#endif // SMSTYLEIMPORT_H

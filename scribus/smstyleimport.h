/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMSTYLEIMPORT_H
#define SMSTYLEIMPORT_H

#include <qdialog.h>
#include <qmap.h>

#include "scribusapi.h"
#include "styles/styleset.h"
#include "scribusstructs.h"

class QWidget;
class QListView;
class QPushButton;
class QCheckListItem;
class QVBoxLayout;
class QHBoxLayout;
class ParagraphStyle;
class QButtonGroup;
class QRadioButton;

class SCRIBUS_API ImportDialog : public QDialog
{
	Q_OBJECT

public:
	ImportDialog(QWidget* parent,
	             StyleSet<ParagraphStyle> *pstyleList, StyleSet<ParagraphStyle> *pstyleOld,
	             StyleSet<CharStyle> *cstyleList, StyleSet<CharStyle> *cstyleOld,
	             QMap<QString, multiLine> *lstyleList, QMap<QString, multiLine> *lstyleOld);
	~ImportDialog() {};

	QMap<QCheckListItem*, QString> storedStyles;
	QMap<QCheckListItem*, QString> storedCharStyles;
	QMap<QCheckListItem*, QString> storedLineStyles;

	bool clashRename();

protected:
	QListView* StyleView;
	QPushButton* OkButton;
	QPushButton* CancelButton;

	QVBoxLayout  *ChooseStylesLayout;
	QHBoxLayout  *layout2;
	QButtonGroup *clashBox;
	QRadioButton *renameButton;
	QRadioButton *replaceButton;
};

#endif // SMSTYLEIMPORT_H

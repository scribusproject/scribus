#ifndef FONTREPLACEDIALOG_H
#define FONTREPLACEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QTable;
class QCheckBox;
class QPushButton;
#include "scribusapi.h"
#include "scribusstructs.h"

class SCRIBUS_API FontReplaceDialog : public QDialog
{
	Q_OBJECT

public:
	FontReplaceDialog( QWidget* parent, QMap<QString, QString> *RList);
	~FontReplaceDialog() {};

	QLabel* textLabel1;
	QTable* replacementTable;
	QCheckBox* stickyReplacements;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QMap<QString, QString> *ReplaceList;

protected:
	void closeEvent(QCloseEvent *closeEvent);
	QVBoxLayout* FontReplaceDialogLayout;
	QHBoxLayout* layout1;
	QSpacerItem* spacer1;

};

#endif // FONTREPLACEDIALOG_H

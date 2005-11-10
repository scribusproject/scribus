#ifndef FONTREPLACEDIALOG_H
#define FONTREPLACEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCloseEvent>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class Q3Table;
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
	Q3Table* replacementTable;
	QCheckBox* stickyReplacements;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QMap<QString, QString> *ReplaceList;

private slots:
	void leaveOK();

protected:
	void closeEvent(QCloseEvent *closeEvent);
	QVBoxLayout* FontReplaceDialogLayout;
	QHBoxLayout* layout1;
	QSpacerItem* spacer1;

};

#endif // FONTREPLACEDIALOG_H
